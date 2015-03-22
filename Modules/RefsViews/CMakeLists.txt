
PROJECT( MGV_MOD_REFSVIEWS )

QT_PREPARE( Core Gui Widgets )

INCLUDE_DIRECTORIES( BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

SET( SRC_FILES

    RefsViewsModule.cpp

    RefRenameDialog.cpp
    RefsViewDelegate.cpp

    Branches/RefItem.cpp
    Branches/BranchesModel.cpp
    Branches/BranchesView.cpp
    Branches/BranchesViewData.cpp
)

SET( HDR_FILES

    RefsViewsModule.h

    RefRenameDialog.hpp
    RefsViewDelegate.h

    Branches/RefItem.hpp
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

