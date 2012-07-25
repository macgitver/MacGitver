
PROJECT( MGV_MOD_REMOTES )

INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES(
	${MGV_LIBS_SOURCE_DIR}
	${MGV_MOD_REMOTES_SOURCE_DIR}
	${MGV_MOD_REMOTES_BINARY_DIR}
)

SET( SRC_FILES

	RemotesModule.cpp
	RemotesView.cpp
	RemotesViewContext.cpp
	RemoteCreateEditDlg.cpp
)

SET( HDR_FILES

	RemotesModule.h
	RemotesView.h
	RemotesViewContext.h
	RemoteCreateEditDlg.h
)

SET( UI_FILES

	RemoteCreateEditDlg.ui
)

SET( HID_FILES

	RemotesModuleActions.hid
	RemotesViewActions.hid
)

QT4_MOC( MOC_FILES ${HDR_FILES} )
QT4_UIC( UIC_FILES ${UI_FILES} )
HIC( HIC_FILES ${HID_FILES} )

ADD_MGV_MODULE(
	Remotes

	${SRC_FILES}
	${HDR_FILES}
	${MOC_FILES}
	${UIC_FILES}
	${HIC_FILES}
)
