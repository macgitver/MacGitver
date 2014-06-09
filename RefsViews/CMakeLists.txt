
PROJECT( MGV_MOD_REFSVIEWS )

QT_PREPARE( Core Gui Widgets )

INCLUDE_DIRECTORIES(
    ${MGV_MOD_REFSVIEWS_SOURCE_DIR}
    ${MGV_MOD_REFSVIEWS_BINARY_DIR}
)

SET( SRC_FILES

    RefsViewsModule.cpp

    RefItem.cpp
    RefRenameDialog.cpp
    RefsSortProxy.cpp

    Branches/BranchesModel.cpp
    Branches/BranchesView.cpp
    Branches/BranchesViewData.cpp
)

SET( HDR_FILES

    RefsViewsModule.h

    RefItem.hpp
    RefRenameDialog.hpp
    RefsSortProxy.hpp

    Branches/BranchesModel.hpp
    Branches/BranchesView.hpp
    Branches/BranchesViewData.hpp
)

SET( UI_FILES
    RefRenameDialog.ui
)

SET( HID_FILES

    Branches/BranchesViewActions.hid
)

QT_UIC( UIC_FILES ${UI_FILES} )
QT_MOC( MOC_FILES ${HDR_FILES} )
HIC( HIC_FILES ${HID_FILES} )

ADD_MGV_MODULE(
    RefsViews

    ${SRC_FILES}
    ${HDR_FILES}
    ${UIC_FILES} ${UI_FILES}
    ${MOC_FILES}
    ${HIC_FILES} ${HID_FILES}
)

