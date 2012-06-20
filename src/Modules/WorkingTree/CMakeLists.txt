
PROJECT( MGV_MOD_WORKINGTREE )

INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES(
	${MGV_LIBS_SOURCE_DIR}
	${MGV_MOD_WORKINGTREE_SOURCE_DIR}
	${MGV_MOD_WORKINGTREE_BINARY_DIR}
)

SET( SRC_FILES

	IndexTree.cpp
	IndexWidget.cpp
	WorkingTreeModule.cpp
	WorkingTreeAbstractItem.cpp
	WorkingTreeDirItem.cpp
	WorkingTreeFileItem.cpp
	WorkingTreeItemView.cpp
	WorkingTreeModel.cpp
)

SET( HDR_FILES

	IndexTree.h
	IndexWidget.h
	WorkingTreeModule.h
	WorkingTreeAbstractItem.h
	WorkingTreeDirItem.h
	WorkingTreeFileItem.h
	WorkingTreeItemView.h
	WorkingTreeModel.h
	WorkingTreeFilters.h
)

SET( HID_FILES

	WorkingTreeActions.hid
)

QT4_MOC( MOC_FILES ${HDR_FILES} )
HIC( HIC_FILES ${HID_FILES} )

ADD_MGV_MODULE(
	WorkingTree

	${SRC_FILES}
	${HDR_FILES}
	${MOC_FILES}
	${HIC_FILES}
)

TARGET_LINK_LIBRARIES(
	ModWorkingTree

	DiffRawView
)
