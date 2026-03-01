function (target_use_glfw target)
  cmake_parse_arguments(PARSE_ARGV 1 _GLFW "STATIC;SHARED;MT;MD;COPY_DLL" "" "")

  # Default
  if (NOT _GLFW_STATIC AND NOT _GLFW_SHARED)
    set(_GLFW_SHARED TRUE)
  endif ()
  if (_GLFW_STATIC
      AND NOT _GLFW_MT
      AND NOT _GLFW_MD)
    set(_GLFW_MD TRUE)
  else ()
    set(_GLFW_COPY_DLL TRUE)
  endif ()

  # Config
  set(GLFW_ROOT "${CMAKE_SOURCE_DIR}/3rd_prebuild/glfw")
  set(GLFW_INCLUDE_DIR "${GLFW_ROOT}/include")
  set(GLFW_LIB_DIR "${GLFW_ROOT}/lib-vc2019")

  if (_GLFW_STATIC)
    if (_GLFW_MT)
      set(lib_name "glfw3_mt")
    else ()
      set(lib_name "glfw3")
    endif ()
    set(lib_path "${GLFW_LIB_DIR}/${lib_name}.lib")
    set(compile_defs GLFW_STATIC)
  else () # SHARED
    set(lib_name "glfw3dll")
    set(lib_path "${GLFW_LIB_DIR}/${lib_name}.lib")
    set(dll_path "${GLFW_LIB_DIR}/glfw3.dll")
  endif ()

  # Check
  if (NOT EXISTS "${lib_path}")
    message(FATAL_ERROR "GLFW library file not found: ${lib_path}, please check the path or option settings")
  endif ()

  # Add glfw to target
  target_include_directories(${target} PRIVATE ${GLFW_INCLUDE_DIR})
  target_link_libraries(${target} PRIVATE ${lib_path} opengl32 gdi32)
  if (compile_defs)
    target_compile_definitions(${target} PRIVATE ${compile_defs})
  endif ()

  # Copy dll
  if (_GLFW_SHARED AND _GLFW_COPY_DLL)
    if (EXISTS "${dll_path}")
      add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dll_path}" $<TARGET_FILE_DIR:${target}>
        COMMENT "Copying glfw3.dll to output directory")
      # message(STATUS "Configured automatic copying of glfw3.dll to output directory")
    else ()
      message(WARNING "glfw3.dll not found: ${dll_path}, please copy manually")
    endif ()
  endif ()

  # message(STATUS "Target ${target} has been configured with GLFW (${lib_type})")
endfunction ()
