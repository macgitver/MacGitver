
PROJECT( MGV_MOD_WORKINGTREE )

QT_PREPARE( Core Gui Widgets )

INCLUDE_DIRECTORIES(
    ${MGV_MOD_WORKINGTREE_SOURCE_DIR}
    ${MGV_MOD_WORKINGTREE_BINARY_DIR}
)

SET( SRC_FILES

    IndexWidget.cpp
    IndexTreeItemView.cpp
    WorkingTreeModule.cpp
    WorkingTreeAbstractItem.cpp
    WorkingTreeDirItem.cpp
    WorkingTreeFileItem.cpp
    WorkingTreeItemView.cpp
    WorkingTreeModel.cpp
    WorkingTreeFilterModel.cpp
)

SET( HDR_FILES

    IndexWidget.h
    IndexTreeItemView.h
    WorkingTreeModule.h
    WorkingTreeAbstractItem.h
    WorkingTreeDirItem.h
    WorkingTreeFileItem.h
    WorkingTreeItemView.h
    WorkingTreeModel.h
    WorkingTreeFilterModel.h
)

SET( HID_FILES

    WorkingTreeActions.hid
    WorkingTreeCtxMenu.hid
    StageViewCtxMenu.hid
)

QT_MOC( MOC_FILES ${HDR_FILES} )
HIC( HIC_FILES ${HID_FILES} )

ADD_MGV_MODULE(
    WorkingTree

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${HIC_FILES} ${HID_FILES}
)

TARGET_LINK_LIBRARIES(
    ModWorkingTree

    DiffViews
)

MSVC_SPLIT_SOURCES( ModWorkingTree )

