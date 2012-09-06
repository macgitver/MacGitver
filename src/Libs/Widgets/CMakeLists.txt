
PROJECT( MGV_WIDGETS )

QT_PREPARE( Core Gui Widgets )

INCLUDE_DIRECTORIES(
	${MGV_LIBS_SOURCE_DIR}
	${MGV_WIDGETS_SOURCE_DIR}
	${MGV_WIDGETS_BINARY_DIR}
)

SET( SRC_FILES

	FontSelectWidget.cpp
	LineEdit.cpp
	StringSelectorWidget.cpp
	HeaderView.cpp
)

SET( PUB_HDR_FILES

	WidgetsApi.h

	FontSelectWidget.h
	LineEdit.h
	StringSelectorWidget.h
	HeaderView.h
)

SET( PRI_HDR_FILES
	StringSelectorWidgetPrivate.h
)

SET( HDR_FILES ${PRI_HDR_FILES} ${PUB_HDR_FILES} )

QT_MOC( MOC_FILES ${HDR_FILES} )

ADD_QT_LIBRARY(
	Widgets SHARED

	${SRC_FILES}
	${HDR_FILES}
	${MOC_FILES}
)

TARGET_LINK_LIBRARIES(
	Widgets

	Config
)

INSTALL_MGV_LIB( Widgets )
