
PROJECT( MacGitver )
CMAKE_MINIMUM_REQUIRED( VERSION 2.8.9 )

INCLUDE( cmake/Utils.cmake )		# Tools for cmake
SET_OUTPUT_PATHS()
INCLUDE( cmake/QtMacros.cmake )		# Our own set of Qt Macros
INCLUDE( cmake/CfgComp.cmake )		# Out CfgComp tool

IF( UNIX )
	GCC_ADD_FLAG( "-Wall" )
	GCC_ADD_FLAG( "-fvisibility=hidden" )
ENDIF()

# Tools required to build MacGitver
ADD_SUBDIRECTORY( tools )

ADD_SUBDIRECTORY( 3rd )

# MacGitver itself
ADD_SUBDIRECTORY( src )
