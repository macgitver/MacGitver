
PROJECT( MGV_MOD_HISTORY )

QT_PREPARE( Core Gui Widgets )

INCLUDE_DIRECTORIES(
    ${MGV_LIBS_SOURCE_DIR}
    ${MGV_MOD_HISTORY_SOURCE_DIR}
    ${MGV_MOD_HISTORY_BINARY_DIR}
)

SET( SRC_FILES

    HistoryView.cpp
    HistoryList.cpp
	HistoryDetails.cpp
    HistoryEntry.cpp
    HistoryBuilder.cpp
    HistoryModel.cpp
    HistoryModule.cpp
)

SET( HDR_FILES

    HistoryView.h
    HistoryList.h
	HistoryEntry.h
	HistoryDetails.h
    HistoryBuilder.h
    HistoryModel.h
    HistoryModule.h
)

QT_MOC( MOC_FILES ${HDR_FILES} )

ADD_MGV_MODULE(
    History

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
)

