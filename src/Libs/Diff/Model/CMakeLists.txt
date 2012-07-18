
PROJECT( MGV_DIFF_MODEL )

SET( QT_DONT_USE_QTGUI			1 )
INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES(
	${MGV_LIBS_SOURCE_DIR}
	${MGV_DIFF_MODEL_SOURCE_DIR}
	${MGV_DIFF_MODEL_BINARY_DIR}
)


SET( SRC_FILES

	Diff.cpp
	DiffHunk.cpp
	Patch.cpp
	PatchFile.cpp
	GitPatchConsumer.cpp
)

SET( PUB_HDR_FILES

	DiffModelApi.h
	Diff.h
	DiffHunk.h
	Patch.h
	PatchFile.h
	GitPatchConsumer.h
)

SET( HDR_FILES ${PRI_HDR_FILES} ${PUB_HDR_FILES} )

QT4_MOC( MOC_FILES ${HDR_FILES} )

ADD_QT_LIBRARY(
	DiffModel SHARED

	${SRC_FILES}
	${HDR_FILES}
	${MOC_FILES}
)

TARGET_LINK_LIBRARIES(
	DiffModel

	GitWrap
)
