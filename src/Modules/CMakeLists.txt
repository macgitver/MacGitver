
MACRO(ADD_MGV_MODULE _Name )

    SET( _Target Mod${_Name} )

    ADD_QT_LIBRARY( ${_Target} MODULE ${ARGN} )

    SET_TARGET_PROPERTIES(
        ${_Target}
        PROPERTIES  OUTPUT_NAME     "${_Name}"
                    PREFIX          "Mod"
                    SUFFIX          ".mgv"
    )

    TARGET_LINK_LIBRARIES(
        ${_Target}

        GitWrap Heaven MacGitverCore
    )

    IF( APPLE )

        # This is probably bad...
        SET_TARGET_PROPERTIES(
            ${_Target}
            PROPERTIES  LIBRARY_OUTPUT_DIRECTORY
                        "${CMAKE_BINARY_DIR}/${UTILS_APP_NAME}.app/Contents/MacOS/modules"
        )

    ELSEIF( UNIX )

        SET_TARGET_PROPERTIES(
            ${_Target}
            PROPERTIES  INSTALL_RPATH
                        "\$ORIGIN/..:\$ORIGIN/../../lib"
                        LIBRARY_OUTPUT_DIRECTORY
                        "${CMAKE_BINARY_DIR}/libexec/${UTILS_APP_NAME}/modules"
        )

        INSTALL(
            TARGETS     ${_Target}
            LIBRARY     DESTINATION libexec/${UTILS_APP_NAME}/modules
        )

    ELSEIF( WIN32 )

        INSTALL(
            TARGETS     ${_Target}
            RUNTIME     DESTINATION bin/modules
            LIBRARY     DESTINATION bin/modules
        )

    ENDIF()

ENDMACRO(ADD_MGV_MODULE)

INCLUDE_DIRECTORIES( ${MGV_CORE_SOURCE_DIR} )

ADD_SUBDIRECTORY( GitConfig )
ADD_SUBDIRECTORY( History )
ADD_SUBDIRECTORY( Logging )
ADD_SUBDIRECTORY( RefsViews )
ADD_SUBDIRECTORY( Remotes )
ADD_SUBDIRECTORY( Repository )
ADD_SUBDIRECTORY( Submodules )
ADD_SUBDIRECTORY( Welcome )
ADD_SUBDIRECTORY( WorkingTree )
