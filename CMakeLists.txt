
PROJECT( MacGitver )
CMAKE_MINIMUM_REQUIRED( VERSION 2.8.9 )

INCLUDE( cmake/Utils.cmake )		# Tools for cmake
INCLUDE( cmake/QtMacros.cmake )		# Our own set of Qt Macros
INCLUDE( cmake/hic.cmake )			# Our HI-Compiler
INCLUDE( cmake/LibGit2.cmake )		# Our LibGit2 integration

IF( UNIX )
	GCC_ADD_FLAG( "-Wall" )
	GCC_ADD_FLAG( "-fvisibility=hidden" )
ENDIF()

SET_OUTPUT_PATHS()

# Tools required to build MacGitver
ADD_SUBDIRECTORY( tools )

# MacGitver itself
ADD_SUBDIRECTORY( src )
