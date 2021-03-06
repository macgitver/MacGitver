
PROJECT( MGV_MOD_WORKINGTREE )

IF(WIN32)
	SET(QT_EXTRAS WinExtras)
ENDIF()

QT_PREPARE(Core Gui Widgets ${QT_EXTRAS})

INCLUDE_DIRECTORIES( BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

SET( SRC_FILES

    CommitDialog.cpp

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

    CommitDialog.h

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

SET( UI_FILES

    CommitDialog.ui
)

SET( HID_FILES

    WorkingTreeActions.hid
    WorkingTreeCtxMenu.hid
    StageViewCtxMenu.hid
)

QT_UIC( UIC_FILES ${UI_FILES} )
QT_MOC( MOC_FILES ${HDR_FILES} )
HIC( HIC_FILES ${HID_FILES} )

ADD_MGV_MODULE(
    WorkingTree

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${UIC_FILES} ${UI_FILES}
    ${HIC_FILES} ${HID_FILES}
)

TARGET_LINK_LIBRARIES(
    ModWorkingTree

    LINK_PRIVATE
        DiffViews
)

