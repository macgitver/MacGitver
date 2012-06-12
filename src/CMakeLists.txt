
PROJECT( MAC_GITVER )

ADD_SUBDIRECTORY( Libs )
ADD_SUBDIRECTORY( Modules )

SET( QT_USE_QTMAIN				1 )
INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES(
	${MAC_GITVER_SOURCE_DIR}
	${MAC_GITVER_BINARY_DIR}

	${MGV_LIBS_SOURCE_DIR}
)

SET( SRC_FILES

	Main/main.cpp
	Main/MacGitverMain.cpp
	Main/MainWindow.cpp
	Main/ConfigDialog.cpp
	Main/GeneralConfigPage.cpp
)

SET( HDR_FILES

	Main/MacGitverMain.h
	Main/MainWindow.h
	Main/ConfigDialog.h
	Main/GeneralConfigPage.h
)

SET( UI_FILES
	Main/ConfigDialog.ui
	Main/GeneralConfigPage.ui
	Main/AboutDlg.ui
)

SET( RCC_FILES

	Main/MacGitver.rcc
)

SET( HID_FILES

	Main/MainWindowActions.hid

)

QT4_MOC( MOC_FILES ${HDR_FILES} )
QT4_UIC( UIC_FILES ${UI_FILES} )
QT4_ADD_RESOURCES( QRC_FILES ${RCC_FILES} )

HIC( HIC_FILES ${HID_FILES} )

ADD_EXECUTABLE(
	MacGitver WIN32

	${SRC_FILES}
	${HDR_FILES}
	${MOC_FILES}
	${UIC_FILES}
	${QRC_FILES}
 #   ${HID_FILES}
	${HIC_FILES}
)

TARGET_LINK_LIBRARIES(
	MacGitver

	${QT_LIBRARIES}
	GitWrap
	Heaven
	DiffModel
	DiffRawView
	DiffView
	Interfaces
	Widgets
	MacGitverCore
)
