function (use_imgui_with_opengl3_glfw)
  set(options)
  set(oneValueArgs IMGUI_DIR)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "" ${ARGN})

  if (NOT ARG_IMGUI_DIR)
    message(FATAL_ERROR "IMGUI_DIR is required.")
  endif ()

  add_library(
    imgui STATIC
    ${ARG_IMGUI_DIR}/imgui.cpp
    ${ARG_IMGUI_DIR}/imgui_draw.cpp
    ${ARG_IMGUI_DIR}/imgui_tables.cpp
    ${ARG_IMGUI_DIR}/imgui_widgets.cpp
    ${ARG_IMGUI_DIR}/imgui_demo.cpp
    ${ARG_IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${ARG_IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)

  if (MSVC)
    target_compile_options(imgui PRIVATE /nologo /Zi /utf-8)
    target_compile_options(imgui PRIVATE $<$<CONFIG:Release>:/MD>)
    target_compile_options(imgui PRIVATE $<$<CONFIG:Debug>:/MDd>)
  endif ()

  target_include_directories(imgui PUBLIC ${ARG_IMGUI_DIR} ${ARG_IMGUI_DIR}/backends)
  target_use_glfw(imgui)
  add_library(imgui::imgui ALIAS imgui)

  message(STATUS "Built imgui static library for GLFW+OpenGL3")
endfunction ()
