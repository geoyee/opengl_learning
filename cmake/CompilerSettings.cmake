function (target_setting target)
  get_target_property(target_type ${target} TYPE)

  if (target_type STREQUAL "INTERFACE_LIBRARY")
    set(compile_scope INTERFACE)
    set(link_scope INTERFACE)
  else ()
    set(compile_scope PRIVATE)
    if (target_type STREQUAL "EXECUTABLE" OR target_type STREQUAL "SHARED_LIBRARY")
      set(link_scope PRIVATE)
    else ()
      set(link_scope INTERFACE)
    endif ()
  endif ()

  if (MSVC)
    target_compile_options(
      ${target}
      ${compile_scope}
      /utf-8
      /W4
      /EHsc
      /Zc:__cplusplus
      /Zc:preprocessor
      /Gy
      $<$<CONFIG:Debug>:/Od
      /Zi
      /RTC1
      /GS>
      $<$<CONFIG:Release>:/O2>
      $<$<CONFIG:RelWithDebInfo>:/O2
      /Zi>
      $<$<CONFIG:MinSizeRel>:/O1>)

    if (target_type STREQUAL "EXECUTABLE")
      target_link_options(${target} ${link_scope} $<$<CONFIG:Release>:/OPT:REF /OPT:ICF>
                          $<$<CONFIG:RelWithDebInfo>:/OPT:REF /OPT:ICF>)
    endif ()
  else ()
    include(CheckCXXCompilerFlag)

    set(warning_flags
        -Wall
        -Wextra
        -Wconversion
        -Wsign-compare
        -Werror=uninitialized
        -Werror=return-type
        -Werror=unused-result
        -Werror=suggest-override
        -Wzero-as-null-pointer-constant
        -Wmissing-declarations
        -Wold-style-cast
        -Wnon-virtual-dtor)

    foreach (flag ${warning_flags})
      string(REPLACE "-" "_" fname "${flag}")
      string(REPLACE "=" "_" fname "${fname}")
      check_cxx_compiler_flag(${flag} HAS_${fname})
      if (HAS_${fname})
        target_compile_options(${target} ${compile_scope} ${flag})
      endif ()
    endforeach ()

    target_compile_options(
      ${target}
      ${compile_scope}
      $<$<CONFIG:Debug>:-O0
      -g
      -fsanitize=address,undefined
      -fno-omit-frame-pointer>
      $<$<CONFIG:Release>:-O2
      -DNDEBUG>
      $<$<CONFIG:RelWithDebInfo>:-O2
      -g
      -DNDEBUG>
      $<$<CONFIG:MinSizeRel>:-Os
      -DNDEBUG>
      ${ARG_FLAGS})

    target_link_options(${target} ${link_scope} $<$<CONFIG:Debug>:-fsanitize=address,undefined>)

    if (target_type STREQUAL "SHARED_LIBRARY" OR target_type STREQUAL "STATIC_LIBRARY")
      set_target_properties(${target} PROPERTIES POSITION_INDEPENDENT_CODE ON)
    endif ()
  endif ()

  if (NOT target_type STREQUAL "INTERFACE_LIBRARY")
    set_target_properties(
      ${target}
      PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE ON
                 INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON
                 INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL ON
                 INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)
  endif ()
endfunction ()
