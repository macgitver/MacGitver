
MACRO(ADD_MGV_MODULE _Name )

	ADD_LIBRARY( Mod${_Name} MODULE ${ARGN} )

	SET_TARGET_PROPERTIES(
		Mod${_Name}
		PROPERTIES  OUTPUT_NAME     "${_Name}"
					PREFIX          "Mod"
					SUFFIX          ".mgv"
	)

	TARGET_LINK_LIBRARIES(
		Mod${_Name}

		${QT_LIBRARIES}
		GitWrap Interfaces Heaven MacGitverCore
	)

ENDMACRO(ADD_MGV_MODULE)

ADD_SUBDIRECTORY( GitConfig )
ADD_SUBDIRECTORY( History )
ADD_SUBDIRECTORY( RefsViews )
ADD_SUBDIRECTORY( Remotes )
ADD_SUBDIRECTORY( Repository )
ADD_SUBDIRECTORY( WorkingTree )
