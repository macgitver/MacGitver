
PROJECT( MACGITVER )

QT_PREPARE( Core Gui Widgets Xml Main )

INCLUDE_DIRECTORIES(
    ${MACGITVER_SOURCE_DIR}
    ${MACGITVER_BINARY_DIR}

    ${MGV_CORE_SOURCE_DIR}
)

SET( SRC_FILES

    main.cpp
)

SET( RCC_FILES

    MacGitver.rcc
)

QT_RCC( RCC_DATA QRC_FILES ${RCC_FILES} )

ADD_QT_EXECUTABLE(
    MacGitver

    MACOSX_BUNDLE
    WIN32

    ${SRC_FILES}
#   ${HDR_FILES}
#   ${MOC_FILES}
    ${QRC_FILES}
    ${RCC_DATA}
)

TARGET_LINK_LIBRARIES(
    MacGitver

    GitWrap
    Heaven
    MacGitverCore
)

IF( UNIX )

    INSTALL(
        TARGETS     MacGitver
        BUNDLE      DESTINATION bin
        RUNTIME     DESTINATION bin
        LIBRARY     DESTINATION bin
    )

    SET_TARGET_PROPERTIES(  MacGitver
        PROPERTIES          INSTALL_RPATH "\$ORIGIN/macgitver/lib:\$ORIGIN/macgitver/modules:\$ORIGIN/"
    )

ENDIF()

