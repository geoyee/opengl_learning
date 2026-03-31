function (target_use_glew target)
  cmake_parse_arguments(PARSE_ARGV 1 _GLEW "STATIC;SHARED;COPY_DLL" "" "")

  # Default
  if (NOT _GLEW_STATIC AND NOT _GLEW_SHARED)
    set(_GLEW_SHARED TRUE)
  endif ()
  if (_GLEW_SHARED)
    set(_GLEW_COPY_DLL TRUE)
  endif ()

  # Config
  set(GLEW_ROOT "${CMAKE_SOURCE_DIR}/3rd_prebuild/glew")
  set(GLEW_INCLUDE_DIR "${GLEW_ROOT}/include")
  set(GLEW_BIN_DIR "${GLEW_ROOT}/bin/Release/x64")
  set(GLEW_LIB_DIR "${GLEW_ROOT}/lib/Release/x64")

  if (_GLEW_STATIC)
    set(lib_name "glew32s")
    set(lib_path "${GLEW_LIB_DIR}/${lib_name}.lib")
    set(compile_defs GLEW_STATIC)
  else () # SHARED
    set(lib_name "glew32")
    set(lib_path "${GLEW_LIB_DIR}/${lib_name}.lib")
    set(dll_path "${GLEW_BIN_DIR}/glew32.dll")
  endif ()

  # Check
  if (NOT EXISTS "${lib_path}")
    message(FATAL_ERROR "GLEW library file not found: ${lib_path}, please check the path or option settings")
  endif ()

  # Add GLEW to target
  target_include_directories(${target} PRIVATE ${GLEW_INCLUDE_DIR})
  target_link_libraries(${target} PRIVATE ${lib_path})
  if (compile_defs)
    target_compile_definitions(${target} PRIVATE ${compile_defs})
  endif ()

  # Copy dll
  if (_GLEW_SHARED AND _GLEW_COPY_DLL)
    if (EXISTS "${dll_path}")
      add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dll_path}" $<TARGET_FILE_DIR:${target}>
        COMMENT "")
      # message(STATUS "Configured automatic copying of glew32.dll to output directory")
    else ()
      message(WARNING "glew32.dll not found: ${dll_path}, please copy manually")
    endif ()
  endif ()

  # message(STATUS "Target ${target} has been configured with GLEW (${lib_type})")
endfunction ()
