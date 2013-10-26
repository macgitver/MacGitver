
PROJECT( MGV_MOD_LOGGING )

QT_PREPARE( Core Gui Widgets WebKit WebKitWidgets )

INCLUDE_DIRECTORIES(
    ${MGV_MOD_LOGGING_SOURCE_DIR}
    ${MGV_MOD_LOGGING_BINARY_DIR}
)

SET( SRC_FILES

    LoggingView.cpp
    LoggingModule.cpp
)

SET( HDR_FILES

    LoggingView.hpp
    LoggingModule.hpp
)

QT_MOC( MOC_FILES ${HDR_FILES} )

ADD_MGV_MODULE(
    Logging

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
)

MSVC_SPLIT_SOURCES( ModLogging )

