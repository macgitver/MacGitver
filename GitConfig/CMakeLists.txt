
PROJECT( MGV_MOD_GITCONFIG )

QT_PREPARE( Core Gui Widgets )

INCLUDE_DIRECTORIES(
    ${MGV_MOD_GITCONFIG_SOURCE_DIR}
    ${MGV_MOD_GITCONFIG_BINARY_DIR}
)

SET( SRC_FILES

    GitConfigDialog.cpp
    GitConfigModule.cpp
    GitConfigWidget.cpp
)

SET( HDR_FILES

    GitConfigDialog.h
    GitConfigModule.h
    GitConfigWidget.h
)

SET( UI_FILES

    GitConfigDialog.ui
    GitConfigWidget.ui
)

SET( HID_FILES

    GitConfigModuleActions.hid
)

QT_MOC( MOC_FILES ${HDR_FILES} )
QT_UIC( UIC_FILES ${UI_FILES} )
HIC( HIC_FILES ${HID_FILES} )

ADD_MGV_MODULE(
    GitConfig

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${UIC_FILES} ${UI_FILES}
    ${HIC_FILES} ${HID_FILES}
)

