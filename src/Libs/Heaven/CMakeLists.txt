
PROJECT( MGV_HEAVEN )

INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES(
	${MGV_LIBS_SOURCE_DIR}
	${MGV_HEAVEN_SOURCE_DIR}
	${MGV_HEAVEN_BINARY_DIR}
)

SET( SRC_FILES

	Actions/Action.cpp
	Actions/ActionContainer.cpp
	Actions/Menu.cpp
	Actions/MenuBar.cpp
	Actions/MergePlace.cpp
	Actions/MergesManager.cpp
	Actions/Separator.cpp
	Actions/ToolBar.cpp
	Actions/UiContainer.cpp
	Actions/UiManager.cpp
	Actions/UiObject.cpp
	Actions/WidgetAction.cpp
	Actions/WidgetActionWrapper.cpp

	Views/Container.cpp
	Views/Decorator.cpp
	Views/GlobalView.cpp
	Views/MainWindow.cpp
	Views/Mode.cpp
	Views/TopLevelWidget.cpp
	Views/View.cpp
	Views/ViewContext.cpp

	Widgets/ModeSwitchWidget.cpp
	Widgets/ModeSwitchCombo.cpp
)

SET( PUB_HDR_FILES

	Heaven.h
	HeavenApi.h

	Actions/Action.h
	Actions/ActionContainer.h
	Actions/Menu.h
	Actions/MenuBar.h
	Actions/MergePlace.h
	Actions/ToolBar.h
	Actions/WidgetAction.h

	Views/Container.h
	Views/GlobalView.h
	Views/MainWindow.h
	Views/Mode.h
	Views/TopLevelWidget.h
	Views/View.h
	Views/ViewContext.h
	
	Widgets/ModeSwitchWidget.h
)

SET( PRI_HDR_FILES

	Actions/ActionContainerPrivate.h
	Actions/ActionPrivate.h
	Actions/MenuBarPrivate.h
	Actions/MenuPrivate.h
	Actions/MergePlacePrivate.h
	Actions/MergesManager.h
	Actions/Separator.h
	Actions/ToolBarPrivate.h
	Actions/UiContainer.h
	Actions/UiManager.h
	Actions/UiObject.h
	Actions/WidgetActionPrivate.h
	Actions/WidgetActionWrapper.h

	Views/MainWindowPrivate.h
	Views/ModePrivate.h
	Views/Decorator.h

	Widgets/ModeSwitchCombo.h
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
