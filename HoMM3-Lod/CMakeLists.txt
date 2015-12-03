# set various variables
get_filename_component(HOMM3_LOD_ROOT_DIR "." ABSOLUTE)

set(PROJECT_NAME HOMM3_LOD)
set(HOMM3_LOD_SOURCE "${HOMM3_LOD_ROOT_DIR}/Source")
set(HOMM3_LOD_INCLUDE "${HOMM3_LOD_ROOT_DIR}/Include")

# --------------------------
# Messages to add
# message("Building ${PROJECT_NAME} as static library")
# message("Building ${PROJECT_NAME} as shared library")


# --------------------------
# Sources declaration : cpp
homm3_files(${PROJECT_NAME} ${HOMM3_LOD_SOURCE}
	Resource/Lod.cpp
)

# --------------------------
# Sources declaration : hpp
homm3_files(${PROJECT_NAME} ${HOMM3_LOD_INCLUDE}
	Resource/Lod.hpp
)


# --------------------------
# Build section
# --------------------------


# Build as shared or static library
if ( ${HOMM3_SHARED_LIB} )
	add_library(${PROJECT_NAME} SHARED 
		${${PROJECT_NAME}_Sources}
	)
else()
	add_library(${PROJECT_NAME} 
		${${PROJECT_NAME}_Sources}
	)
endif()

# Add project's includes folder to the include directories
target_include_directories(${PROJECT_NAME}
	PUBLIC ${HOMM3_LOD_ROOT_DIR}/Include ${zlib_SOURCE_DIR}
)

# Link required libraries to the project
target_link_libraries(${PROJECT_NAME}
	PUBLIC zlib
)

#target_compile_features(${PROJECT_NAME}
#	PUBLIC
#		cxx_defaulted_functions
#		cxx_delegating_constructors
#		cxx_deleted_functions
#		cxx_final
#		cxx_nullptr
#		cxx_override
#		cxx_rvalue_references
#)