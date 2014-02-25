
PROJECT( MGV_MOD_HISTORY )

QT_PREPARE( Core Gui Widgets Xml WebKit WebKitWidgets )

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

SET(SRC_FILES

    HistoryView.cpp
    HistoryListDelegate.cpp
    HistoryList.cpp
    HistoryDetails.cpp
    HistoryEntry.cpp
    HistoryBuilder.cpp
    HistoryModel.cpp
    HistoryModule.cpp
    HistoryConfigPage.cpp
    HistoryDiff.cpp
    HistoryMode.cpp
)

SET(HDR_FILES

    HistoryView.h
    HistoryListDelegate.h
    HistoryList.h
    HistoryEntry.h
    HistoryDetails.h
    HistoryBuilder.h
    HistoryModel.h
    HistoryModule.h
    HistoryConfigPage.h
    HistoryDiff.h
    HistoryMode.hpp
)

SET(UI_FILES

    HistoryConfigPage.ui
)

SET(HID_FILES

    HistoryViewActions.hid
)

SET(RCC_FILES
    Resources/HistoryModule.rcc
)

CFGCOMP( CFG_FILES HistoryConfig.ccfg )
QT_RCC( RCC_DATA QRC_FILES ${RCC_FILES} )
QT_MOC( MOC_FILES ${HDR_FILES} )
QT_UIC( UIC_FILES ${UI_FILES} )
HIC( HIC_FILES ${HID_FILES} )

ADD_MGV_MODULE(
    History

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${UIC_FILES} ${UI_FILES}
    ${HIC_FILES} ${HID_FILES}
    ${CFG_FILES}
    ${RCC_DATA}
    ${QRC_FILES}
)

TARGET_LINK_LIBRARIES(
    ModHistory

    LINK_PRIVATE
        DiffViews
)

MSVC_SPLIT_SOURCES(ModHistory)
