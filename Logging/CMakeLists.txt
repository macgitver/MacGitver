
PROJECT(MGV_MOD_LOGGING)

QT_PREPARE(Core Gui Widgets WebKit WebKitWidgets)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

SET(SRC_FILES

    LoggingView.cpp
    LoggingModule.cpp
    LoggingMode.cpp
)

SET(HDR_FILES

    LoggingView.hpp
    LoggingModule.hpp
    LoggingMode.hpp
)

SET(RCC_FILES

    Resources/LoggingModule.rcc
)

QT_MOC(MOC_FILES ${HDR_FILES})
QT_RCC(RCC_DATA QRC_FILES ${RCC_FILES})

ADD_MGV_MODULE(
    Logging

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${RCC_DATA}
    ${QRC_FILES}
    ${RCC_FILES}
)

MSVC_SPLIT_SOURCES(ModLogging)
