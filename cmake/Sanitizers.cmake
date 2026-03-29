function(target_enable_sanitizers tgt)
  # Check if our custom option is enabled and we are in a Debug build
  if(CHIEFRAY_ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    
    if(MSVC)
      target_compile_options(${tgt} PRIVATE /fsanitize=address /Zi)
      # MSVC automatically links the ASan runtime when the compiler flag is set
      set_target_properties(${tgt} PROPERTIES MSVC_DEBUG_INFORMATION_FORMAT ProgramDatabase)
      target_link_options(${tgt} PRIVATE /INCREMENTAL:NO)
      
    else()
        # GCC/Clang-specific configuration
        target_compile_options(${tgt} PRIVATE
            $<$<COMPILE_LANGUAGE:C,CXX>:-fsanitize=address,undefined>
        )
        target_link_options(${tgt} PRIVATE -fsanitize=address,undefined)
    endif()
    
  endif()
endfunction()
