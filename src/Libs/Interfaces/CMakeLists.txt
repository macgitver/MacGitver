
PROJECT( MGV_INTERFACES )

INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES(
	${MGV_LIBS_SOURCE_DIR}
	${MGV_INTERFACES_SOURCE_DIR}
	${MGV_INTERFACES_BINARY_DIR}
)

SET( SRC_FILES

	IConfigDialog.cpp
	IConfigPage.cpp
	IMainWindow.cpp
	IModule.cpp
	IRepositoryCore.cpp
)

SET( PUB_HDR_FILES

	InterfaceApi.h

	IConfigDialog.h
	IConfigPage.h
	IMainWindow.h
	IModule.h
	IRepositoryCore.h
)

SET( HDR_FILES ${PRI_HDR_FILES} ${PUB_HDR_FILES} )

QT4_MOC( MOC_FILES ${HDR_FILES} )

ADD_QT_LIBRARY(
	Interfaces SHARED

	${SRC_FILES}
	${HDR_FILES}
	${MOC_FILES}
)

INSTALL_MGV_LIB( Interfaces )
