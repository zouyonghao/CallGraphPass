add_library(DynamicFunctionPass MODULE
        # List your source files here.
        DynamicFunctionPass.cpp
        )

# Use C++11 to compile your pass (i.e., supply -std=c++11).
target_compile_features(DynamicFunctionPass PRIVATE cxx_range_for cxx_auto_type)

# LLVM is (typically) built with no C++ RTTI. We need to match that;
# otherwise, we'll get linker errors about missing RTTI data.
set_target_properties(DynamicFunctionPass PROPERTIES
        COMPILE_FLAGS "-fno-rtti"
        )
