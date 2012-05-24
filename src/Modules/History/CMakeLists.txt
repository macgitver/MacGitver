
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

SET( HDR_FILES

    HistoryView.h
    HistoryList.h
    HistoryEntry.h
    HistoryBuilder.h
    HistoryModel.h
    HistoryModule.h
)

QT4_MOC( MOC_FILES ${HDR_FILES} )

ADD_MGV_MODULE(
    History

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
)

