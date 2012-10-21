
PROJECT( MGV_MOD_SUBMODULES )

QT_PREPARE( Core Gui Widgets )

INCLUDE_DIRECTORIES(
    ${MGV_LIBS_SOURCE_DIR}
    ${MGV_MOD_SUBMODULES_SOURCE_DIR}
    ${MGV_MOD_SUBMODULES_BINARY_DIR}
)

SET( SRC_FILES

    SubmodulesCreateEditDlg.cpp
    SubmodulesModule.cpp
    SubmodulesView.cpp
    SubmodulesDelegate.cpp
)

SET( HDR_FILES

    SubmodulesCreateEditDlg.h
    SubmodulesModule.h
    SubmodulesView.h
    SubmodulesDelegate.h
)

SET( UI_FILES

    SubmodulesCreateEditDlg.ui
)

SET( HID_FILES

    SubmodulesViewActions.hid
)

QT_MOC( MOC_FILES ${HDR_FILES} )
QT_UIC( UIC_FILES ${UI_FILES} )
HIC( HIC_FILES ${HID_FILES} )

ADD_MGV_MODULE(
    Submodules

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${UIC_FILES}
    ${HIC_FILES}
)
