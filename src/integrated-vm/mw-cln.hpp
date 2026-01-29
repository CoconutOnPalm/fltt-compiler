#pragma once

#include <vector>
#include <cln/cln.h>


namespace fl::vm
{

	std::vector<cln::cl_I> run_machine(std::vector<std::pair<int, long long>>& program);
	
} // namespace fl::vm
