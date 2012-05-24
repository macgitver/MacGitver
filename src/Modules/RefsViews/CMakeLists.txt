
PROJECT( MGV_MOD_REFSVIEWS )

INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES(
    ${MGV_LIBS_SOURCE_DIR}
    ${MGV_MOD_REFSVIEWS_SOURCE_DIR}
    ${MGV_MOD_REFSVIEWS_BINARY_DIR}
)

SET( SRC_FILES

    RefsViewsModule.cpp
    RefsView.cpp
    BranchesView.cpp
    TagsView.cpp
)

SET( PUB_HDR_FILES

)

SET( PRI_HDR_FILES

    RefsViewsModule.h
    RefsView.h
    BranchesView.h
    TagsView.h
)

SET( HDR_FILES ${PRI_HDR_FILES} ${PUB_HDR_FILES} )

QT4_MOC( MOC_FILES ${HDR_FILES} )

ADD_MGV_MODULE(
    RefsViews

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
)
