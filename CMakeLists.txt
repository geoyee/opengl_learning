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
include(UseGLEWithX64)
include(UseImGuiWithOpenGL)
include(CompilerSettings)

add_definitions(-DSTB_IMAGE_IMPLEMENTATION)

set(GLM_BUILD_LIBRARY
    OFF
    CACHE BOOL "Build dynamic/static library")
set(GLM_ENABLE_CXX_17
    ON
    CACHE BOOL "Enable C++ 17")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd/glm)

use_imgui_with_opengl3_glfw(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd/imgui)

file(
  GLOB sample_entries
  RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/samples"
  "${CMAKE_CURRENT_SOURCE_DIR}/samples/*")
foreach (entry IN LISTS sample_entries)
  set(sample_dir "${CMAKE_CURRENT_SOURCE_DIR}/samples/${entry}")
  if (IS_DIRECTORY "${sample_dir}")
    file(
      GLOB_RECURSE
      sample_sources
      "${sample_dir}/*.cpp"
      "${sample_dir}/*.cxx"
      "${sample_dir}/*.cc"
      "${sample_dir}/*.hpp"
      "${sample_dir}/*.h")
    if (sample_sources)
      add_executable(${entry} ${sample_sources} ${stb_image_header})
      target_include_directories(${entry} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/3rd/stb")
      target_link_libraries(${entry} PRIVATE glm::glm-header-only imgui::imgui)
      target_use_glfw(${entry})
      target_use_glew(${entry})
      target_setting(${entry})
      message(STATUS "Add sample: ${entry}")
      if (EXISTS "${sample_dir}/resource")
        add_custom_command(
          TARGET ${entry}
          POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${sample_dir}/resource
                  $<TARGET_FILE_DIR:${entry}>/resource
          COMMENT "Copying resource to output directory")
      endif ()
    else ()
      message(WARNING "No source files found in sample directory: ${entry}")
    endif ()
  endif ()
endforeach ()
