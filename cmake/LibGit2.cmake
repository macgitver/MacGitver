
OPTION(LIBGIT2_SUBMOD "Use libgit2 from our submodule" ON)

IF(LIBGIT2_SUBMOD)

	# submodule containing libgit2
	OPTION (THREADSAFE "Build libgit2 as threadsafe" ON)
	OPTION (BUILD_CLAR "Build Tests using the Clar suite" OFF)
	OPTION (BUILD_SHARED_LIBS "Build Shared Library (OFF for Static)" ON)
	OPTION (STDCALL "Buildl libgit2 with the __stdcall convention" OFF)

	# Paths is relative to Top-Src-Dir. Duh.
	ADD_SUBDIRECTORY( libgit2 )

ELSE()

	FIND_PACKAGE(
		git2
		PATHS	/usr/lib
				/usr/local/lib
				${CMAKE_INSTALL_PREFIX}/lib
	)

ENDIF()

