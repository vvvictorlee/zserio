set(ZSERIO_GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen)
set(ZSERIO_GENERATED_SOURCES
    ${ZSERIO_GEN_DIR}/explicit_parameters/ExplicitParametersDb.cpp
    ${ZSERIO_GEN_DIR}/explicit_parameters/ExplicitParametersDb.h

    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_bitmask_param/BitmaskParamTable.cpp
    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_bitmask_param/BitmaskParamTable.h
    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_bitmask_param/TestBlob.cpp
    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_bitmask_param/TestBlob.h
    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_bitmask_param/TestBitmask.cpp
    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_bitmask_param/TestBitmask.h

    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_blob_param/BlobParamTable.cpp
    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_blob_param/BlobParamTable.h
    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_blob_param/Header.cpp
    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_blob_param/Header.h
    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_blob_param/TestBlob.cpp
    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_blob_param/TestBlob.h

    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_enum_param/EnumParamTable.cpp
    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_enum_param/EnumParamTable.h
    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_enum_param/TestBlob.cpp
    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_enum_param/TestBlob.h
    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_enum_param/TestEnum.cpp
    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_enum_param/TestEnum.h

    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_simple_param/SimpleParamTable.cpp
    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_simple_param/SimpleParamTable.h
    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_simple_param/TestBlob.cpp
    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_simple_param/TestBlob.h

    ${ZSERIO_GEN_DIR}/explicit_parameters/multiple_explicit_params/MultipleParamsTable.cpp
    ${ZSERIO_GEN_DIR}/explicit_parameters/multiple_explicit_params/MultipleParamsTable.h
    ${ZSERIO_GEN_DIR}/explicit_parameters/multiple_explicit_params/TestBlob.cpp
    ${ZSERIO_GEN_DIR}/explicit_parameters/multiple_explicit_params/TestBlob.h

    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_same_as_field/SameAsFieldTable.cpp
    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_same_as_field/SameAsFieldTable.h
    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_same_as_field/TestBlob.cpp
    ${ZSERIO_GEN_DIR}/explicit_parameters/explicit_same_as_field/TestBlob.h
)

zserio_add_library(TARGET explicit_parameters_zs
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/zs
    MAIN_SOURCE explicit_parameters.zs
    OUT_DIR ${ZSERIO_GEN_DIR}
    OUT_FILES ${ZSERIO_GENERATED_SOURCES}
    ZSERIO_CORE_DIR ${ZSERIO_JAR_DIR}
    ZSERIO_OPTIONS -withoutSourcesAmalgamation
)

target_include_directories(explicit_parameters_zs SYSTEM PRIVATE ${SQLITE_INCDIR})

add_test_sources(
    ${ZSERIO_CPP_DIR}/ExplicitBitmaskParamTest.cpp
    ${ZSERIO_CPP_DIR}/ExplicitBlobParamTest.cpp
    ${ZSERIO_CPP_DIR}/ExplicitEnumParamTest.cpp
    ${ZSERIO_CPP_DIR}/ExplicitSameAsFieldTest.cpp
    ${ZSERIO_CPP_DIR}/ExplicitSimpleParamTest.cpp
    ${ZSERIO_CPP_DIR}/MultipleExplicitParamsTest.cpp
)

add_test_dependencies(explicit_parameters_zs)
