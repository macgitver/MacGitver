
PROJECT( MGV_MOD_REFSVIEWS )

QT_PREPARE( Core Gui Widgets )

INCLUDE_DIRECTORIES(
    ${MGV_MOD_REFSVIEWS_SOURCE_DIR}
    ${MGV_MOD_REFSVIEWS_BINARY_DIR}
)

SET( SRC_FILES

    RefsViewsModule.cpp

    RefsListModel.cpp
    RefsListDelegate.cpp

    RefsView.cpp
    BranchesView.cpp
    TagsView.cpp
)

SET( HDR_FILES

    RefsViewsModule.h

    RefsListModel.h
    RefsListDelegate.h

    RefsView.h
    BranchesView.h
    TagsView.h
)

QT_MOC( MOC_FILES ${HDR_FILES} )

ADD_MGV_MODULE(
    RefsViews

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
)
