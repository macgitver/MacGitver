
PROJECT( MacGitver )

CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

FIND_PACKAGE( Qt4 4.8.0 REQUIRED )

SET( EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin )
SET( LIBRARY_OUTPUT_PATH    ${CMAKE_BINARY_DIR}/bin )

INCLUDE( cmake/QtMacros.cmake )			# Our own set of Qt4 Macros
INCLUDE( cmake/hic.cmake )              # The HI-Compiler
#INCLUDE( cmake/Xslt.cmake )				# Generic Xslt support

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

ADD_SUBDIRECTORY( libgit2  )
ADD_SUBDIRECTORY( tools )
ADD_SUBDIRECTORY( src )
