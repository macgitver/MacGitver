
PROJECT( MACGITVER )

QT_PREPARE( Core Gui Widgets Xml Main )

INCLUDE_DIRECTORIES(
    ${MACGITVER_SOURCE_DIR}
    ${MACGITVER_BINARY_DIR}

    ${MGV_LIBS_SOURCE_DIR}
)

SET( SRC_FILES

    main.cpp
    MacGitverMain.cpp
    MainWindow.cpp
    ConfigDialog.cpp
    GeneralConfigPage.cpp
)

SET( HDR_FILES

    MacGitverMain.h
    MainWindow.h
    ConfigDialog.h
    GeneralConfigPage.h
)

SET( UI_FILES
    ConfigDialog.ui
    GeneralConfigPage.ui
    AboutDlg.ui
)

SET( RCC_FILES

    MacGitver.rcc
)

SET( HID_FILES

    MainWindowActions.hid

)

QT_MOC( MOC_FILES ${HDR_FILES} )
QT_UIC( UIC_FILES ${UI_FILES} )
QT_RCC( RCC_DATA QRC_FILES ${RCC_FILES} )

HIC( HIC_FILES ${HID_FILES} )

ADD_QT_EXECUTABLE(
    MacGitver WIN32

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${UIC_FILES}
    ${QRC_FILES}
    ${HIC_FILES}
    ${RCC_DATA}
)

TARGET_LINK_LIBRARIES(
    MacGitver

    GitWrap
    Config
    Heaven
    Interfaces
    Widgets
    MacGitverCore
)

IF( UNIX )

    INSTALL(
        TARGETS		MacGitver
        RUNTIME		DESTINATION bin
        LIBRARY		DESTINATION bin
    )

    SET_TARGET_PROPERTIES(	MacGitver
        PROPERTIES			INSTALL_RPATH "\$ORIGIN/macgitver/lib:\$ORIGIN/macgitver/modules:\$ORIGIN/"
    )

ENDIF()
