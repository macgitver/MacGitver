
PROJECT( MacGitver )

CMAKE_MINIMUM_REQUIRED( VERSION 2.8.8 )

FIND_PACKAGE( Qt4 4.8.2 REQUIRED )

SET( EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin )
SET( LIBRARY_OUTPUT_PATH    ${CMAKE_BINARY_DIR}/bin )

INCLUDE( cmake/QtMacros.cmake )			# Our own set of Qt4 Macros
INCLUDE( cmake/hic.cmake )              # The HI-Compiler

IF( UNIX )

    IF( NOT DEFINED CMAKE_CXX_FLAGS )
        SET( CMAKE_CXX_FLAGS "-Wall" CACHE STRING "C++ Build Flags" FORCE )
    ENDIF( NOT DEFINED CMAKE_CXX_FLAGS )

    IF( NOT "${CMAKE_CXX_FLAGS}" MATCHES ".*-Wall.*" )
        SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall" CACHE STRING "C++ Build Flags" FORCE )
    ENDIF( NOT "${CMAKE_CXX_FLAGS}" MATCHES ".*-Wall.*" )

    # We can NOW be sure that it is defined
    IF( NOT "${CMAKE_CXX_FLAGS}" MATCHES ".*-fvisibility=.*" )
        SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden" CACHE STRING "C++ Build Flags" FORCE )
    ENDIF( NOT "${CMAKE_CXX_FLAGS}" MATCHES ".*-fvisibility=.*" )

ENDIF( UNIX )

# submodule containing libgit2
OPTION (THREADSAFE "Build libgit2 as threadsafe" ON)
OPTION (BUILD_CLAR "Build Tests using the Clar suite" OFF)
OPTION (BUILD_SHARED_LIBS "Build Shared Library (OFF for Static)" ON)
OPTION (STDCALL "Buildl libgit2 with the __stdcall convention" OFF)
ADD_SUBDIRECTORY( libgit2 )

# Tools required to build MacGitver
ADD_SUBDIRECTORY( tools )

# MacGitver itself
ADD_SUBDIRECTORY( src )
