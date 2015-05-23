
PROJECT(ACTIVITIES_GUI)

QT_PREPARE(Core Widgets)

INCLUDE_DIRECTORIES(
    ${ACTIVITIES_GUI_BINARY_DIR}
)

SET( SRC_FILES
    ProgressDlg.cpp
    ProgressItems.cpp
)

SET( PUB_HDR_FILES
    ProgressDlg.hpp
    ProgressItems.hpp
)

SET( PRI_HDR_FILES
)

SET( UI_FILES
    ProgressDlg.ui
    ProgressWdgt.ui
)

SET( HDR_FILES ${PRI_HDR_FILES} ${PUB_HDR_FILES} )

QT_UIC( UIC_FILES ${UI_FILES} )
QT_MOC( MOC_FILES ${HDR_FILES} )

ADD_QT_LIBRARY(
    ActivitiesGui SHARED

    ${SRC_FILES}
    ${HDR_FILES}
    ${UIC_FILES}
    ${MOC_FILES}
)

TARGET_LINK_LIBRARIES(
    ActivitiesGui
)

IF(UNIX AND NOT APPLE)
    SET_TARGET_PROPERTIES(
        ActivitiesGui
        PROPERTIES  INSTALL_RPATH
        "\$ORIGIN"
    )
ENDIF()

RAD_DEFINE_VERSION(ACTIVITIES_GUI 0 0 1)
RAD_SET_TARGET_VERSION(ActivitiesGui ACTIVITIES_GUI)
RAD_INSTALL_LIBRARY    (ActivitiesGui ActivitiesGui)
RAD_INSTALL_HEADERS    (libActivitiesGui ${PUB_HDR_FILES})
RAD_SPLIT_SOURCE_TREE  (libActivitiesGui)
