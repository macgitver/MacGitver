
PROJECT( MacGitver )
CMAKE_MINIMUM_REQUIRED( VERSION 2.8.8 )

FIND_PACKAGE( Qt4 4.8.2 REQUIRED )

INCLUDE( cmake/Utils.cmake )		# Tools for cmake
INCLUDE( cmake/QtMacros.cmake )		# Our own set of Qt Macros
INCLUDE( cmake/hic.cmake )			# Our HI-Compiler

IF( UNIX )
	GCC_ADD_FLAG( "-Wall" )
	GCC_ADD_FLAG( "-fvisibility=hidden" )
ENDIF()

SET_OUTPUT_PATHS()

OPTION(LIBGIT2_SUBMOD "Use libgit2 from our submodule" ON)

IF(LIBGIT2_SUBMOD)

	# submodule containing libgit2
	OPTION (THREADSAFE "Build libgit2 as threadsafe" ON)
	OPTION (BUILD_CLAR "Build Tests using the Clar suite" OFF)
	OPTION (BUILD_SHARED_LIBS "Build Shared Library (OFF for Static)" ON)
	OPTION (STDCALL "Buildl libgit2 with the __stdcall convention" OFF)

	ADD_SUBDIRECTORY( libgit2 )

ELSE()

	FIND_PACKAGE(
		git2
		PATHS		/usr/lib
					/usr/local/lib
					${CMAKE_INSTALL_PREFIX}/lib
	)

ENDIF()

# Tools required to build MacGitver
ADD_SUBDIRECTORY( tools )

# MacGitver itself
ADD_SUBDIRECTORY( src )
