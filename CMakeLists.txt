cmake_minimum_required (VERSION 3.20)
project (Min-DevKit)

enable_testing ()

set (CMAKE_OSX_DEPLOYMENT_TARGET "10.13")

string (REGEX REPLACE "(.*)/" "" THIS_PACKAGE_NAME "${CMAKE_CURRENT_SOURCE_DIR}")


# Apple/Xcode initialization:

if (APPLE AND ${CMAKE_GENERATOR} MATCHES "Xcode")
	if (${XCODE_VERSION} VERSION_LESS 10)
		message (STATUS "Xcode 10 or higher is required. Please install from the App Store.")
		return ()
	endif ()
	if (NOT CMAKE_OSX_ARCHITECTURES)
		if (${XCODE_VERSION} VERSION_LESS 12)
			set (CMAKE_OSX_ARCHITECTURES ${CMAKE_SYSTEM_PROCESSOR} CACHE STRING "macOS architecture" FORCE)
		else ()
			set (CMAKE_OSX_ARCHITECTURES            "x86_64;arm64" CACHE STRING "macOS architecture" FORCE)
		endif ()
		message ("CMAKE_OSX_ARCHITECTURES := ${CMAKE_OSX_ARCHITECTURES}")
	endif ()
endif ()


# MIN-API initialization:

set (C74_MIN_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source/min-api)
include (${C74_MIN_API_DIR}/script/min-package.cmake)


# MIN-LIB initialization:

set (C74_MIN_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source/min-lib)
if (EXISTS "${C74_MIN_LIB_DIR}/CMakeLists.txt")
	add_subdirectory (${C74_MIN_LIB_DIR})
endif ()


# XTAL initialization:
# 
# NOTE: Would prefer to `(?:target_)?link_libraries` to include `range-v3` etc by association,
# but the (transitive) `#includes` cannot be found.
#
# NOTE: The `min-api/max-sdk-base` submodules have been forked to accommodate `C++20`.
# In addition, `#define PI` etc has been removed for `range-v3` compatibility
# (`PI` appears as a template-parameter in `range/v3/algorithm/partial_sort_copy.hpp`).

find_package (xtal CONFIG REQUIRED)
include_directories (${xtal_INCLUDE_DIRS})
   link_directories (${xtal_LIBRARY_DIRS})
	add_definitions (${xtal_CFLAGS_OTHER})

find_package (simde CONFIG REQUIRED)
include_directories (${simde_INCLUDE_DIRS})
   link_directories (${simde_LIBRARY_DIRS})
	add_definitions (${simde_CFLAGS_OTHER})

find_package (range-v3 CONFIG REQUIRED)
include_directories (${range-v3_INCLUDE_DIRS})
   link_directories (${range-v3_LIBRARY_DIRS})
	add_definitions (${range-v3_CFLAGS_OTHER})


# Generate a project for every folder in "${C74_PROJECT_DIR}":

set (C74_PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source/projects)
SUBDIRLIST (C74_PROJECT_SUBDIRS ${C74_PROJECT_DIR})

foreach (C74_PROJECT_SUBDIR ${C74_PROJECT_SUBDIRS})
	if (EXISTS "${C74_PROJECT_DIR}/${C74_PROJECT_SUBDIR}/CMakeLists.txt")
		message ("Generating: ${C74_PROJECT_SUBDIR}")
		add_subdirectory (${C74_PROJECT_DIR}/${C74_PROJECT_SUBDIR})
	endif ()
endforeach ()

# Comment the line below if you want automatic cmake regneration enabled
set (CMAKE_SUPPRESS_REGENERATION true)
