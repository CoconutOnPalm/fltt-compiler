#!/usr/bin/env python3
import json
import re
import subprocess
import sys
import os
from pathlib import Path
from typing import List, Tuple, Optional, Any

# Constants
CONFIG_FILE = Path("tests/config.json")
TESTS_FILE = Path("tests/tests.json")
OUTPUT_PREFIX = "> "
INPUT_PREFIX = "? "
REMOVE_ANSI = re.compile(r"\x1b[^m]*m")
COST_PATTERN = re.compile(r"koszt: ([\d,]+); w tym i\/o: ([\d,]+)")

# ANSI Colors
class Colors:
    GREEN = '\033[92m'
    RED = '\033[91m'
    BLUE = '\033[94m'
    CYAN = '\033[96m'
    MAGENTA = '\033[95m'
    YELLOW = '\033[93m'
    BOLD = '\033[1m'
    FAINT = '\033[2m'
    RESET = '\033[0m'
    
    # ANSI escape sequences add hidden chars; these constants help with alignment
    STATUS_ANSI_LEN = len(GREEN) + len(BOLD) + len(RESET)
    FILENAME_ANSI_LEN = len(CYAN) + len(RESET)
    COST_ANSI_LEN = len(BLUE) + len(RESET)
    IO_COST_ANSI_LEN = len(MAGENTA) + len(FAINT) + len(RESET)

    @staticmethod
    def success(text: str) -> str:
        return f"{Colors.GREEN}{Colors.BOLD}{text}{Colors.RESET}"

    @staticmethod
    def failure(text: str) -> str:
        return f"{Colors.RED}{Colors.BOLD}{text}{Colors.RESET}"
    
    @staticmethod
    def filename(text: str) -> str:
        return f"{Colors.CYAN}{text}{Colors.RESET}"
    
    @staticmethod
    def cost(text: str) -> str:
        return f"{Colors.BLUE}{text}{Colors.RESET}"
    
    @staticmethod
    def io_cost(text: str) -> str:
        return f"{Colors.MAGENTA}{Colors.FAINT}{text}{Colors.RESET}"
    
    @staticmethod
    def bold(text: str) -> str:
        return f"{Colors.BOLD}{text}{Colors.RESET}"
    
    @staticmethod
    def faint(text: str) -> str:
        return f"{Colors.FAINT}{text}{Colors.RESET}"

def load_json(path: Path) -> Any:
    try:
        with open(path, 'r') as f:
            return json.load(f)
    except Exception as e:
        print(f"Error loading {path}: {e}")
        sys.exit(1)

def resolve_source_file(tests_dir: Path, filename: str) -> Path:
    """
    Tries to find the source file. Handles missing extensions if necessary.
    """
    file_path = tests_dir / filename
    if file_path.exists():
        return file_path
    
    # Try adding .imp extension if not present
    if not filename.endswith('.imp'):
        imp_path = tests_dir / f"{filename}.imp"
        if imp_path.exists():
            return imp_path
            
    # Return original path to let caller fail usually
    return file_path

def compile_program(compiler: str, source: Path, dest: Path) -> Tuple[bool, str]:
    """
    Compiles the program. Returns (success, error_message).
    """
    # Ensure destination directory exists
    dest.parent.mkdir(parents=True, exist_ok=True)
    
    cmd = [compiler, str(source), str(dest)]
    try:
        result = subprocess.run(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        if result.returncode != 0:
            return False, result.stderr + "\n" + result.stdout
        return True, ""
    except Exception as e:
        return False, str(e)

def run_vm(vm_path: str, program_path: Path, input_data: List[int]) -> Tuple[bool, List[str], str, str, str]:
    """
    Runs the VM with the compiled program. Returns (success, output_lines, error_message, cost, io_cost).
    """
    input_str = "\n".join(map(str, input_data)) + "\n"
    cmd = [vm_path, str(program_path)]

    try:
        process = subprocess.Popen(
            cmd,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )
        stdout, stderr = process.communicate(input=input_str, timeout=10)
        
        if process.returncode != 0:
            return False, [], stderr, "", ""
        
        output = stdout.splitlines()
        output = [line.replace(INPUT_PREFIX, "") for line in output]
        
        # Extract cost info from output (always the last line)
        cost = ""
        io_cost = ""
        if output:
            last_line = output[-1]
            last_line = REMOVE_ANSI.sub("", last_line)
            match = COST_PATTERN.search(last_line)
            if match:
                cost = match.group(1).replace(",", "'")
                io_cost = match.group(2).replace(",", "'")
            
        return True, output, "", cost, io_cost
    except Exception as e:
        return False, [], str(e), "", ""

def parse_vm_output(lines: List[str]) -> List[int]:
    """
    Extracts integers from lines starting with OUTPUT_PREFIX.
    """
    result = []
    for line in lines:
        if line.startswith(OUTPUT_PREFIX):
            try:
                # Extract part after prefix
                val_str = line[len(OUTPUT_PREFIX):].strip()
                if val_str:
                    result.append(int(val_str))
            except ValueError:
                pass # Ignore non-integer outputs formatted like output
    return result

def main():
    if not CONFIG_FILE.exists() or not TESTS_FILE.exists():
        print(f"Configuration files not found. Ensure {CONFIG_FILE} and {TESTS_FILE} exist.")
        sys.exit(1)

    config = load_json(CONFIG_FILE)
    tests = load_json(TESTS_FILE)

    # Resolve paths relative to CWD
    compiler_cfg = config.get("compiler-exe", "kompilator")
    vm_cfg = config.get("vm-exe", "vm")
    
    compiler_path = Path(compiler_cfg)
    vm_path = Path(vm_cfg)
    
    # Ensure executables are invoked from CWD properly
    # If path is just a filename, prepend ./ to avoid PATH lookup
    # If path has slashes, it's relative or absolute, but user requested ./ prefix style for compiler
    if not compiler_path.is_absolute() and not str(compiler_path).startswith("./"):
        compiler_executable = f"./{compiler_path}"
    else:
        compiler_executable = str(compiler_path)
        
    if not vm_path.is_absolute() and not str(vm_path).startswith("./"):
         vm_executable = f"./{vm_path}"
    else:
         vm_executable = str(vm_path)

    tests_dir = Path(config.get("tests-dir", "tests/programs"))
    compiled_dir = Path(config.get("compiled-dir", "tests/.compiled"))

    if not Path(compiler_executable).exists() and not compiler_path.exists():
         # Check both with and without ./ just in case logic is fuzzy
        print(f"Compiler not found at: {compiler_path}")
    
    failed_tests = []
    total_tests = len(tests)
    passed_count = 0
    
    # Calculate max filename length for alignment (+ ansi code length)
    max_filename_len = max(len(tc["file"]) for tc in tests) if tests else 20

    # Result column width (visible chars); ANSI codes add extra hidden chars
    result_width = 7  # "SUCCESS" or "FAILED "
    result_col = result_width + Colors.STATUS_ANSI_LEN

    print("Running tests...\n")
    print(f"{'File':<{max_filename_len}}  {'Result':<{result_width}}  {'Cost':>13}  {'I/O':>8}")
    print("-" * (max_filename_len + 2 + result_width + 2 + 12 + 2 + 8))

    max_filename_len += Colors.FILENAME_ANSI_LEN

    for test_case in tests:
        filename = test_case["file"]
        expected_in = test_case.get("in", [])
        expected_out = test_case.get("out", [])
        
        source_file = resolve_source_file(tests_dir, filename)
        
        # Determine output file path. Use original filename structure for consistency
        # e.g., output will be named 'test-assign' or 'test-array.imp' in the compiled dir
        compiled_file = compiled_dir / filename
        
        # 1. Compile
        success, err = compile_program(compiler_executable, source_file, compiled_file)
        if not success:
            print(f"{filename:<{max_filename_len}}  {Colors.failure('FAILED'):<{result_col}}  {'N/A':>13}  {'N/A':>8}  (Compilation Error)")
            print(f"Error output:\n{err}")
            failed_tests.append(filename)
            continue
            
        # 2. Run VM
        success, output_lines, err, cost, io_cost = run_vm(vm_executable, compiled_file, expected_in)
        if not success:
            print(f"{filename:<{max_filename_len}}  {Colors.failure('FAILED'):<{result_col}}  {'N/A':>13}  {'N/A':>8}  (Runtime/VM Error)")
            print(f"Error output:\n{err}")
            failed_tests.append(filename)
            continue
            
        # 3. Validate
        parsed_out = parse_vm_output(output_lines)
        
        cost_str = cost if cost else "N/A"
        io_str = io_cost if io_cost else "N/A"
        
        if parsed_out == expected_out:
            print(f"{Colors.filename(filename):<{max_filename_len}}  {Colors.success('SUCCESS'):<{result_col}}  {Colors.cost(cost_str):>{12 + Colors.COST_ANSI_LEN}}  {Colors.io_cost(io_str):>{8 + Colors.IO_COST_ANSI_LEN}}")
            passed_count += 1
        else:
            print(f"{Colors.filename(filename):<{max_filename_len}}  {Colors.failure('FAILED'):<{result_col}}  {Colors.cost(cost_str):>{12 + Colors.COST_ANSI_LEN}}  {Colors.io_cost(io_str):>{8 + Colors.IO_COST_ANSI_LEN}}")
            print(f"{Colors.faint('Input:')}    {expected_in}")
            print(f"{Colors.faint('Expected:')} {expected_out}")
            print(f"{Colors.faint('Got:')}      {parsed_out}")
            failed_tests.append(filename)
            
    print("\nSUMMARY:")
    if passed_count == total_tests:
        print(f"{Colors.success('ALL TESTS PASSED')} ({Colors.GREEN}{passed_count}{Colors.RESET}{Colors.FAINT}/{Colors.GREEN}{total_tests}{Colors.RESET})")
    else:
        print(f"Passed: {Colors.RED}{passed_count}{Colors.FAINT}/{Colors.RED}{total_tests}{Colors.RESET}")
        print("Failed tests:")
        for t in failed_tests:
            print(f" - {t}")
        sys.exit(1)

if __name__ == "__main__":
    main()
