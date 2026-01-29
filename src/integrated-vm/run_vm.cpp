#include "run_vm.hpp"

#include "mw-cln.hpp"


extern void vm_run_parser(std::vector<std::pair<int,long long>>& program, const std::vector<std::string>& instructions);


namespace fl::vm
{
	
    std::vector<cln::cl_I> runVM(const std::vector<std::string>& instructions)
    {
		std::vector<std::pair<int,long long>> program;
		vm_run_parser(program, instructions);
		return run_machine(program);
    }

} // namespace fl::vm
