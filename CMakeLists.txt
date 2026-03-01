cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(LINK_WHAT_YOU_USE ON)

project(opengl_learning LANGUAGES C CXX)

if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "Please create a build directory and run CMake from there")
endif ()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(UseGLFWithX64)
include(CompilerSettings)

file(
  GLOB sample_entries
  RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/samples"
  "${CMAKE_CURRENT_SOURCE_DIR}/samples/*")
foreach (entry IN LISTS sample_entries)
  set(sample_dir "${CMAKE_CURRENT_SOURCE_DIR}/samples/${entry}")
  if (IS_DIRECTORY "${sample_dir}")
    file(GLOB_RECURSE sample_sources "${sample_dir}/*.cpp" "${sample_dir}/*.cxx" "${sample_dir}/*.cc")
    if (sample_sources)
      add_executable(${entry} ${sample_sources})
      target_use_glfw(${entry})
      target_setting(${entry})
      message(STATUS "Add target: ${entry}")
    else ()
      message(WARNING "No source files found in sample directory: ${entry}")
    endif ()
  endif ()
endforeach ()
