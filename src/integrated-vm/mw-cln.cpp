/*
 * Kod interpretera maszyny rejestrowej do projektu z JFTT2025
 *
 * Autor: Maciek Gębala
 * http://ki.pwr.edu.pl/gebala/
 * 2025-11-15
 * (wersja cln)
 *
 * Modified by Adam Kostrzewski
*/

#include <iostream>
#include <locale>

#include <utility>
#include <vector>
#include <map>

#include <cstdlib> 	// rand()
#include <ctime>

#include <cln/cln.h>

#include "instructions.hpp"
#include "colors.hpp"

#include "../utils/panic.hpp"


namespace fl::vm
{

	std::vector<cln::cl_I> run_machine(std::vector<std::pair<int, long long>>& program)
	{
		std::vector<cln::cl_I> program_out; 

		std::map<cln::cl_I, cln::cl_I> pam;

		cln::cl_I r[8], tmp;
		long long lr;

		long long t, io;

		lr = 0;
		std::srand(std::time(0));

		for (int i = 0; i < 8; i++) r[i] = rand();

		t = 0;
		io = 0;

		while (program[lr].first != HALT)	// HALT
		{
			switch (program[lr].first)
			{
			case READ: 		panic("unexpected 'READ' inside integrated vm"); lr++; break;
			case WRITE:		program_out.push_back(r[0]); io += 100; lr++; break;

			case LOAD:		r[0] = pam[program[lr].second]; t += 50; lr++; break;
			case STORE:		pam[program[lr].second] = r[0]; t += 50; lr++; break;
			case RLOAD:		r[0] = pam[r[program[lr].second]]; t += 50; lr++; break;
			case RSTORE:	pam[r[program[lr].second]] = r[0]; t += 50; lr++; break;

			case ADD:		r[0] += r[program[lr].second]; t += 5; lr++; break;
			case SUB:		r[0] -= r[0] >= r[program[lr].second] ? r[program[lr].second] : r[0]; t += 5; lr++; break;
			case SWP:		tmp = r[program[lr].second]; r[program[lr].second] = r[0]; r[0] = tmp; t += 5; lr++; break;

			case RST:		r[program[lr].second] = 0; t += 1; lr++; break;
			case INC:		r[program[lr].second]++; t += 1; lr++; break;
			case DEC:		if (r[program[lr].second] > 0) r[program[lr].second]--; t += 1; lr++; break;
			case SHL:		r[program[lr].second] <<= 1; t += 1; lr++; break;
			case SHR:		r[program[lr].second] >>= 1; t += 1; lr++; break;

			case JUMP: 		lr = program[lr].second; t += 1; break;
			case JPOS:		if (r[0] > 0) lr = program[lr].second; else lr++; t += 1; break;
			case JZERO:		if (r[0] == 0) lr = program[lr].second; else lr++; t += 1; break;

			case CALL:		r[0] = lr + 1; lr = program[lr].second; t += 1; break;
			case RTRN:		lr = cln::cl_I_to_int(r[0]); t += 1; break;

			default: break;
			}

			if (lr < 0 || lr >= (int)program.size())
			{
				panic("[vm]: use of non-existing instruction: {}", lr);
				std::exit(-1);
			}
		}

		return program_out;
	}

} // namespace fl::vm
