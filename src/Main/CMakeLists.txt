
PROJECT( MACGITVER )

SET( QT_USE_QTMAIN				1 )
SET( QT_USE_QTXML				1 )

INCLUDE( ${QT_USE_FILE} )

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
	SwitchModeComboBox.cpp
)

SET( HDR_FILES

	MacGitverMain.h
	MainWindow.h
	ConfigDialog.h
	GeneralConfigPage.h
	SwitchModeComboBox.h
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

QT4_MOC( MOC_FILES ${HDR_FILES} )
QT4_UIC( UIC_FILES ${UI_FILES} )
QT4_RCC( RCC_DATA QRC_FILES ${RCC_FILES} )

HIC( HIC_FILES ${HID_FILES} )

ADD_EXECUTABLE(
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

	${QT_LIBRARIES}
	GitWrap
	Config
	Heaven
	DiffModel
	DiffRawView
	DiffView
	Interfaces
	Widgets
	MacGitverCore
)
