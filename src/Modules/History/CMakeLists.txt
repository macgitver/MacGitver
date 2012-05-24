
PROJECT( MGV_MOD_HISTORY )

INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES(
    ${MGV_LIBS_SOURCE_DIR}
    ${MGV_MOD_HISTORY_SOURCE_DIR}
    ${MGV_MOD_HISTORY_BINARY_DIR}
)

SET( SRC_FILES

    HistoryView.cpp
    HistoryList.cpp
    HistoryEntry.cpp
    HistoryBuilder.cpp
    HistoryModel.cpp
    HistoryModule.cpp
)

SET( PUB_HDR_FILES

)

SET( PRI_HDR_FILES

    HistoryView.h
    HistoryList.h
    HistoryEntry.h
    HistoryBuilder.h
    HistoryModel.h
    HistoryModule.h
)

SET( HDR_FILES ${PRI_HDR_FILES} ${PUB_HDR_FILES} )

QT4_MOC( MOC_FILES ${HDR_FILES} )

ADD_LIBRARY(
    ModHistory SHARED

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
)

TARGET_LINK_LIBRARIES(
    ModHistory

    ${QT_LIBRARIES}
	GitWrap
	Interfaces
	MacGitverCore
	Heaven
)
