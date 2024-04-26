# Copyright 2018 The Min-DevKit Authors. All rights reserved.
# Use of this source code is governed by the MIT License found in the License.md file.

cmake_minimum_required(VERSION 3.20)


################################################################################
# MAX EXTERNAL

include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)# Before!
include_directories("${C74_INCLUDES}")

set(PROJECT_FILES ${PROJECT_NAME}.cpp)
add_library(${PROJECT_NAME} MODULE ${PROJECT_FILES})

include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)# After!


################################################################################
# UNIT TEST

#include(${C74_MIN_API_DIR}/test/min-object-unittest.cmake)
