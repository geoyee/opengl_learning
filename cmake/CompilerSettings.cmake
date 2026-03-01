function (target_setting target)
  if (MSVC)
    target_compile_options(
      ${target}
      INTERFACE /utf-8
                /W4
                /EHsc
                /Zc:__cplusplus
                /Zc:preprocessor
                /Gy
                $<$<CONFIG:Debug>:/Od>
                $<$<CONFIG:Release>:/O2
                /GL>)
    target_link_options(
      ${target}
      INTERFACE
      $<$<CONFIG:Release>:
      /LTCG
      /OPT:REF
      /OPT:ICF
      >)
  else ()
    target_compile_options(
      ${target}
      INTERFACE -fPIC
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
                -Wnon-virtual-dtor
                $<$<CONFIG:Debug>:-g>
                $<$<CONFIG:Release>:-g2
                -flto>)
    target_link_options(${target} INTERFACE $<$<CONFIG:Release>:-flto>)
  endif ()

  set_target_properties(${target} PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
endfunction ()
