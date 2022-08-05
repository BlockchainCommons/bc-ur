idf_component_register(SRC_DIRS "src"
        PRIV_REQUIRES mbedtls
        INCLUDE_DIRS "src")

component_compile_options(-std=c++17)
target_compile_options(${COMPONENT_LIB} PRIVATE -Ofast)
