#pragma once

// Implemented per platform
#include <string>

namespace Terra {

	class FileDialog
	{
	public:
		// Return empty strings if cancelled
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};


}