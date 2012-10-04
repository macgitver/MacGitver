
PROJECT( MGV_MOD_HISTORY )

QT_PREPARE( Core Gui Widgets )

INCLUDE_DIRECTORIES(
	${MGV_LIBS_SOURCE_DIR}
	${MGV_MOD_HISTORY_SOURCE_DIR}
	${MGV_MOD_HISTORY_BINARY_DIR}
)

SET( SRC_FILES

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
)

SET( HDR_FILES

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
)

SET( UI_FILES

	HistoryConfigPage.ui
)

SET( HID_FILES

	HistoryViewActions.hid
)

CFGCOMP( CFG_FILES HistoryConfig.ccfg )
QT_MOC( MOC_FILES ${HDR_FILES} )
QT_UIC( UIC_FILES ${UI_FILES} )
HIC( HIC_FILES ${HID_FILES} )

ADD_MGV_MODULE(
	History

	${SRC_FILES}
	${HDR_FILES}
	${MOC_FILES}
	${UIC_FILES}
	${HIC_FILES}
	${CFG_FILES}
)

TARGET_LINK_LIBRARIES( ModHistory DiffRawView )
