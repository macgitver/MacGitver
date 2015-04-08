
PROJECT( MGV_MOD_REMOTES )

QT_PREPARE( Core Gui Widgets )

INCLUDE_DIRECTORIES( BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

SET( SRC_FILES

    CloneRepositoryDlg.cpp
    RemotesView.cpp
    RemotesViewContext.cpp
    RemoteCreateEditDlg.cpp
)

SET( HDR_FILES

    CloneRepositoryDlg.hpp
    RemotesModule.h
    RemotesView.h
    RemotesViewContext.h
    RemoteCreateEditDlg.h
)

SET( UI_FILES

    RemoteCreateEditDlg.ui
    CloneOptionsWdgt.ui
    CloneWdgt.ui
)

SET( HID_FILES

    RemotesModuleActions.hid
)

QT_MOC( MOC_FILES ${HDR_FILES} )
QT_UIC( UIC_FILES ${UI_FILES} )
HIC( HIC_FILES ${HID_FILES} )

ADD_MGV_MODULE(
    Remotes

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${UIC_FILES} ${UI_FILES}
    ${HIC_FILES} ${HID_FILES}
)

