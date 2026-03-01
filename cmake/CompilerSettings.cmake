function (target_setting target)
  get_target_property(target_type ${target} TYPE)

  if (target_type STREQUAL "INTERFACE_LIBRARY")
    set(scope INTERFACE)
  else ()
    set(scope PRIVATE)
  endif ()

  if (MSVC)
    target_compile_options(
      ${target}
      ${scope}
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
      target_link_options(${target} PRIVATE $<$<CONFIG:Release>:/OPT:REF /OPT:ICF> $<$<CONFIG:RelWithDebInfo>:/OPT:REF
                          /OPT:ICF>)
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
        target_compile_options(${target} ${scope} ${flag})
      endif ()
    endforeach ()

    target_compile_options(
      ${target}
      ${scope}
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

    target_link_options(${target} ${scope} $<$<CONFIG:Debug>:-fsanitize=address,undefined>)

    if (target_type STREQUAL "SHARED_LIBRARY" OR target_type STREQUAL "STATIC_LIBRARY")
      set_target_properties(${target} PROPERTIES POSITION_INDEPENDENT_CODE ON)
    endif ()
  endif ()

  if (NOT target_type STREQUAL "INTERFACE_LIBRARY")
    set_target_properties(
      ${target}
      PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE ON
                 INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON
                 INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL ON)
  endif ()
endfunction ()
