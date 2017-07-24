# set various variables
get_filename_component( HOMM3_LOD_TEST_ROOT_DIR "." ABSOLUTE )

set( PROJECT_NAME HoMM3_Lod_Test )
set( HOMM3_LOD_TEST_SOURCE "${HOMM3_LOD_TEST_ROOT_DIR}/Source" )
set( HOMM3_LOD_TEST_INCLUDE "${HOMM3_LOD_TEST_ROOT_DIR}/Include" )


# --------------------------
# Sources declaration : cpp
homm3_files( ${PROJECT_NAME} ${HOMM3_LOD_TEST_SOURCE}
    HoMM3-Lod-Test.cpp
    Compression/ZHelperUnitTest.cpp
    Compression/PackedRLEHelperUnitTest.cpp
    Resource/LodUnitTest.cpp
    Resource/DefUnitTest.cpp
    Resource/PcxUnitTest.cpp
)
# --------------------------

# --------------------------
# Sources declaration : hpp
homm3_files( ${PROJECT_NAME} ${HOMM3_LOD_TEST_INCLUDE} )
# --------------------------

# --------------------------
# print build info
message( STATUS "HOMM3_LOD_TEST_ROOT_DIR: ${HOMM3_LOD_TEST_ROOT_DIR}" )
message( STATUS "HOMM3_SHARED_LIB: ${HOMM3_SHARED_LIB}" )
# --------------------------

# --------------------------
# Build section
# --------------------------

# Build as executable
add_executable( ${PROJECT_NAME}
    ${${PROJECT_NAME}_Sources}
)

# Add project's includes folder to the include directories
target_include_directories( ${PROJECT_NAME}
    PUBLIC
        ${HOMM3_LOD_TEST_ROOT_DIR}/Include
        ${HoMM3_Lod_SOURCE_DIR}/Include
		${ZLIB_INCLUDE_DIR}
		${GOOGLETEST_INCLUDE_DIR}
)

# Link required libraries to the project
target_link_libraries( ${PROJECT_NAME}
    PUBLIC
		HoMM3_Lod
		gtest
		gtest_main
		gmock
		gmock_main
		sfml-graphics
		sfml-audio
)

# copy of dll from HoMM3_Lod
add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:HoMM3_Lod>              
	$<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# copy of dll from zlib (only working if zlib is build with the project)
add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:zlib>              
	$<TARGET_FILE_DIR:${PROJECT_NAME}>
)
