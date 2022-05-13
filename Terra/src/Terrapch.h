#pragma once
//	Include in every cpp file

//	Setting pch manually:
//	sbpch.cpp -> properties -> precompiled headers -> create pch
//	GameEngine -> properties -> Use pch -> set as sbpch.h


#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>

#include "Terra/Core/Log.h"
#include "Terra/Debug/Instrumentor.h"


#ifdef SB_PLATFORM_WINDOWS
	#include "Platform/Windows/Win32Config.h"
#endif

