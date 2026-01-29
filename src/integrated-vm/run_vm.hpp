#pragma once

#include <vector>
#include <cln/cln.h>


namespace fl::vm
{
	
	std::vector<cln::cl_I> runVM(const std::vector<std::string>& instructions);
	
} // namespace fl::vm
