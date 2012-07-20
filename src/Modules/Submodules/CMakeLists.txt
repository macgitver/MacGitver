
PROJECT( MGV_MOD_SUBMODULES )

INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES(
	${MGV_LIBS_SOURCE_DIR}
	${MGV_MOD_SUBMODULES_SOURCE_DIR}
	${MGV_MOD_SUBMODULES_BINARY_DIR}
)

SET( SRC_FILES

	SubmodulesCreateEditDlg.cpp
	SubmodulesModule.cpp
	SubmodulesView.cpp
)

SET( HDR_FILES

	SubmodulesCreateEditDlg.h
	SubmodulesModule.h
	SubmodulesView.h
)

SET( UI_FILES

	SubmodulesCreateEditDlg.ui
)

SET( HID_FILES

	SubmodulesViewActions.hid
)

QT4_MOC( MOC_FILES ${HDR_FILES} )
QT4_UIC( UIC_FILES ${UI_FILES} )
HIC( HIC_FILES ${HID_FILES} )

ADD_MGV_MODULE(
	Submodules

	${SRC_FILES}
	${HDR_FILES}
	${MOC_FILES}
	${UIC_FILES}
	${HIC_FILES}
)
