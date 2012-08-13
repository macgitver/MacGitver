
PROJECT( MGV_DIFF_VIEW )

INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES(
	${MGV_LIBS_SOURCE_DIR}
	${MGV_DIFF_VIEW_SOURCE_DIR}
	${MGV_DIFF_VIEW_BINARY_DIR}
)

SET( SRC_FILES

	DiffFrame.cpp
	DiffSplitter.cpp
	DiffTreeFileList.cpp
	DiffView.cpp
	DiffWidget.cpp
)

SET( PUB_HDR_FILES

	DiffViewApi.h
	DiffFrame.h
	DiffSplitter.h
	DiffTreeFileList.h
	DiffView.h
	DiffWidget.h
)

SET( PRI_HDR_FILES
)

SET( HDR_FILES ${PRI_HDR_FILES} ${PUB_HDR_FILES} )

QT_MOC( MOC_FILES ${HDR_FILES} )

ADD_QT_LIBRARY(
	DiffView SHARED

	${SRC_FILES}
	${HDR_FILES}
	${MOC_FILES}
)

TARGET_LINK_LIBRARIES(
	DiffView

	DiffModel
	DiffRawView
	Heaven
)

INSTALL_MGV_LIB( DiffView )
