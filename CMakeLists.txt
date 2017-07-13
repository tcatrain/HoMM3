cmake_minimum_required(VERSION 2.8)

# set various variables
get_filename_component( HOMM3_ROOT_DIR "." ABSOLUTE )

# cmake -G"Visual Studio 11" -DCMAKE_BUILD_TYPE=Debug
# --------------------------


#-------------------------------------------------------------------------------
# define top-level options for the whole project
option( HOMM3_USE_THREAD "Enable thread support into HoMM3" ON )
option( HOMM3_MEMORY_DEBUG "Enable memory debug implementation into HoMM3" ON )
option( HOMM3_SHARED_LIB "Enable compile HoMM3 into shared lib" ON )
#-------------------------------------------------------------------------------


#-------------------------------------------------------------------------------
# homm3_files(dirs ...)
# Parse one or more directories for sources and add them to the current
# target.
#
macro(homm3_files project_name folder)
    foreach ( homm3_file ${ARGN} )
        # determine group folder name
        set( abs_homm3_file "${folder}/${homm3_file}" )
        get_filename_component( dir ${homm3_file} DIRECTORY )
        if ( "${dir}" STREQUAL "" )
            source_group( "" FILES ${abs_homm3_file} )
        else()
            # source_group needs a \\ path for VS filters
            string( REPLACE / \\ groupName ${dir} )
            source_group( ${groupName} FILES ${abs_homm3_file} )
        endif()

        # add to global tracker variables
        list( APPEND ${project_name}_Sources ${abs_homm3_file} )
    endforeach()

    # remove duplicate sources
    if ( ${project_name}_Sources )
        list( REMOVE_DUPLICATES ${project_name}_Sources )
    endif()
endmacro()


#-------------------------------------------------------------------------------
# homm3_setup()
# Performs one-time initialization of the build system. Must be called
# at the start of the root CMakeLists.txt file.
#
macro(homm3_setup)
    # --------------------------
    # define configuration types
    set( CMAKE_CONFIGURATION_TYPES Debug Release )
    # --------------------------
    
    
    # --------------------------
    # set the build type to use
    if( NOT CMAKE_BUILD_TYPE )
        set( CMAKE_BUILD_TYPE "Debug" CACHE STRING "Compile Type" FORCE )
    endif()
    set_property( CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release )
    # --------------------------
    
    
    # --------------------------
    # set the compiler
    if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11" )
    endif()
    # --------------------------
    
    
    # --------------------------
    # check 64 bit
    if( CMAKE_SIZEOF_VOID_P EQUAL 4 )
        set( HOMM3_X32 1 )
        set( HOMM3_PLATFORM_NAME "x86" )
    else()
        set( HOMM3_X64 1)
        set( HOMM3_PLATFORM_NAME "x64" )
    endif()
    # --------------------------

    
    # --------------------------
    # detect the environment and define a #define for the environment allocating table and OS
    if( UNIX )
        set( HOMM3_LINUX 1 )
        if( HOMM3_X32 EQUAL 1 )
            set( HOMM3_PLATFORM_NAME "linux64" )
            list( APPEND HOMM3_PLATFORM_DEFINES -DHOMM3_LINUX -DHOMM3_X64 )
        else()
            set( HOMM3_PLATFORM_NAME "linux32")
            list( APPEND HOMM3_PLATFORM_DEFINES -DHOMM3_LINUX -DHOMM3_X32 )
        endif()
    else()
        set( HOMM3_WINDOWS 1 )
        if( HOMM3_X64 EQUAL 1 )
            set( HOMM3_PLATFORM_NAME "win64" )
            list( APPEND HOMM3_PLATFORM_DEFINES -DHOMM3_WINDOWS -DHOMM3_X64)
        else()
            set( HOMM3_PLATFORM_NAME "win32" )
            list( APPEND HOMM3_PLATFORM_DEFINES -DHOMM3_WINDOWS -DHOMM3_X32 )
        endif()
    endif()
    # --------------------------
    
    
    # --------------------------
    # detect the compiler type and define a #define for the compiler and set the debug flags
    if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
        list( APPEND HOMM3_PLATFORM_DEFINES -DCOMPILER_CLANG )
        set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unknown-pragmas" )
    elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
        list ( APPEND HOMM3_PLATFORM_DEFINES -DCOMPILER_GCC )
        set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unknown-pragmas" )
    elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
        list( APPEND HOMM3_PLATFORM_DEFINES -DCOMPILER_MSVC )
        set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} /W4 /DEBUG /Od /ZI" )
		set( CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG" )
    endif()
    # --------------------------
    
    
    # --------------------------
    # setup the target group variable, used to group targets into folders in IDEs
    set_property( GLOBAL PROPERTY USE_FOLDERS ON )
    set( TARGET_GROUP "" )
    # --------------------------
    
    
    # --------------------------
    # various global defines
    if( HOMM3_MEMORY_DEBUG )
        list( APPEND HOMM3_PLATFORM_DEFINES -DHOMM3_MEMORY_DEBUG )
    endif()
    
    if( HOMM3_SHARED_LIB )
        message("Building ${PROJECT_NAME} as shared library")
        list( APPEND HOMM3_PLATFORM_DEFINES -DHOMM3_SHARED_LIB )
    endif()
    # --------------------------
    
    
    # --------------------------
    # add the plateform defines to the list of definition
    foreach( homm3_define ${HOMM3_PLATFORM_DEFINES} )
        add_definitions( ${homm3_define} )
        message( STATUS "DEFINE: ${homm3_define}" )
    endforeach()
    # --------------------------
    
    
    # --------------------------
    # print build info
    message( STATUS "HOMM3_ROOT_DIR: ${HOMM3_ROOT_DIR}" )
    message( STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}" )
    message( STATUS "HOMM3_PLATFORM: ${HOMM3_PLATFORM}" )
    message( STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}" )
    # --------------------------
endmacro()


# --------------------------
# Build section
# --------------------------

project( HoMM3 C CXX )
homm3_setup()


add_subdirectory( ${HOMM3_ROOT_DIR}/Dependencies/zlib-1.2.8 )
set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/Dependencies/zlib-1.2.8)
add_subdirectory( ${HOMM3_ROOT_DIR}/HoMM3-Lod )
add_subdirectory( ${HOMM3_ROOT_DIR}/HoMM3-Lod-Test )
