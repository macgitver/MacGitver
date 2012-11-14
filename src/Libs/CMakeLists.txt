
PROJECT( MGV_LIBS )

MACRO( INSTALL_MGV_LIB _Name )

    IF( UNIX )

        INSTALL(
            TARGETS		${_Name}
            RUNTIME		DESTINATION bin/macgitver/lib
            LIBRARY		DESTINATION bin/macgitver/lib
        )

    ENDIF()

ENDMACRO()

ADD_SUBDIRECTORY( Config )
ADD_SUBDIRECTORY( Widgets )
ADD_SUBDIRECTORY( Interfaces )
ADD_SUBDIRECTORY( MacGitver )
