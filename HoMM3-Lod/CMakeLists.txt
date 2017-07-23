# set various variables
get_filename_component( HOMM3_LOD_ROOT_DIR "." ABSOLUTE )

set( PROJECT_NAME HoMM3_Lod )
set( HOMM3_LOD_SOURCE "${HOMM3_LOD_ROOT_DIR}/Source" )
set( HOMM3_LOD_INCLUDE "${HOMM3_LOD_ROOT_DIR}/Include" )

# --------------------------
# Sources declaration : cpp
homm3_files( ${PROJECT_NAME} ${HOMM3_LOD_SOURCE}
    Resource/Lod.cpp
    Resource/Def.cpp
    Resource/Pcx.cpp
    Image/Bitmap.cpp
    Compression/ZHelper.cpp
    Compression/PackedRLEHelper.cpp
)
# --------------------------


# --------------------------
# Sources declaration : hpp
homm3_files(${PROJECT_NAME} ${HOMM3_LOD_INCLUDE}
    Types.hpp
    Resource/AResource.hpp
    Resource/Lod.hpp
    Resource/Def.hpp
    Resource/Pcx.hpp
    Image/Bitmap.hpp
    Compression/ACompressionHelper.hpp
    Compression/ZHelper.hpp
    Compression/PackedRLEHelper.hpp
)
# --------------------------


# --------------------------
# print build info
message( STATUS "HOMM3_LOD_ROOT_DIR: ${HOMM3_LOD_ROOT_DIR}" )
message( STATUS "HOMM3_SHARED_LIB: ${HOMM3_SHARED_LIB}" )
# --------------------------


# --------------------------
# Build section
# --------------------------

# Build as shared or static library
if ( HOMM3_SHARED_LIB )
    add_library( ${PROJECT_NAME} SHARED
        ${${PROJECT_NAME}_Sources}
    )
    set( ZLIB_LINK zlib )
else()
    add_library( ${PROJECT_NAME}
        ${${PROJECT_NAME}_Sources}
    )
    set( ZLIB_LINK zlibstatic )
endif()

# Add project's includes folder to the include directories
target_include_directories( ${PROJECT_NAME}
    PUBLIC ${HOMM3_LOD_ROOT_DIR}/Include ${zlib_SOURCE_DIR} "${ZLIB_INCLUDE_DIR}"
)

# Link required libraries to the project
target_link_libraries( ${PROJECT_NAME}
    PUBLIC ${ZLIB_LINK}
)

# copy of dll from zlib
add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:${ZLIB_LINK}>
	$<TARGET_FILE_DIR:${PROJECT_NAME}>
)
