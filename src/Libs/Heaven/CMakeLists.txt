
PROJECT( MGV_HEAVEN )

INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES(
    ${MGV_LIBS_SOURCE_DIR}
    ${MGV_HEAVEN_SOURCE_DIR}
    ${MGV_HEAVEN_BINARY_DIR}
)

SET( SRC_FILES

    Container.cpp
    View.cpp
    TopLevelWidget.cpp
    Decorator.cpp

    Menu.cpp
    Action.cpp
    MenuBar.cpp
    ToolBar.cpp
    ActionContainer.cpp
    MergePlace.cpp
    UiManager.cpp
    MergesManager.cpp
    UiObject.cpp
    UiContainer.cpp
)

SET( PUB_HDR_FILES

    HeavenApi.h
    Heaven.h
    Container.h
    View.h
    TopLevelWidget.h

    Menu.h
    Action.h
    MenuBar.h
    ToolBar.h
    ActionContainer.h
    MergePlace.h
)

SET( PRI_HDR_FILES

    Decorator.h
    MergesManager.h
    ActionPrivate.h
    MenuPrivate.h
    MenuBarPrivate.h
    ToolBarPrivate.h
    ActionContainerPrivate.h
    MergePlacePrivate.h
    UiContainer.h
    UiObject.h
    UiManager.h
)

SET( HDR_FILES ${PRI_HDR_FILES} ${PUB_HDR_FILES} )

QT4_MOC( MOC_FILES ${HDR_FILES} )

ADD_LIBRARY(
    Heaven SHARED

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
)

TARGET_LINK_LIBRARIES(
    Heaven

    ${QT_LIBRARIES}
)
