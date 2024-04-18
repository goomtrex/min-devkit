cmake_minimum_required(VERSION 3.20)
project(Min-DevKit)

enable_testing()

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13")

string(REGEX REPLACE "(.*)/" "" THIS_PACKAGE_NAME "${CMAKE_CURRENT_SOURCE_DIR}")

if (APPLE)
    if (${CMAKE_GENERATOR} MATCHES "Xcode")
            if (${XCODE_VERSION} VERSION_LESS 10)
                message(STATUS "Xcode 10 or higher is required. Please install from the Mac App Store.")
                return ()
            elseif(${XCODE_VERSION} VERSION_GREATER_EQUAL 12)
                set(C74_BUILD_FAT YES)
            endif ()
    endif ()

    if (NOT CMAKE_OSX_ARCHITECTURES)
        if(C74_BUILD_FAT)
            set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "macOS architecture" FORCE)
        else()
            set(CMAKE_OSX_ARCHITECTURES ${CMAKE_SYSTEM_PROCESSOR} CACHE STRING "macOS architecture" FORCE)
        endif()
        message("CMAKE_OSX_ARCHITECTURES set to ${CMAKE_OSX_ARCHITECTURES}")
    endif()
endif()


# Misc setup and subroutines
include(${CMAKE_CURRENT_SOURCE_DIR}/source/min-api/script/min-package.cmake)


# Add the Lib, if it exists
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/source/min-lib/CMakeLists.txt")
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source/min-lib)
endif ()


# XTAL & friends...
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


# Generate a project for every folder in the "source/projects" folder
SUBDIRLIST(PROJECT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/source/projects)
foreach (project_dir ${PROJECT_DIRS})
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/source/projects/${project_dir}/CMakeLists.txt")
        message("Generating: ${project_dir}")
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source/projects/${project_dir})
    endif ()
endforeach ()

# Comment the line below if you want automatic cmake regneration enabled
set(CMAKE_SUPPRESS_REGENERATION true)
