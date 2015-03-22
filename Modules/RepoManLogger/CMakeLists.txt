
PROJECT(MGV_MOD_RMLOGGER)

QT_PREPARE( Core Gui Widgets )

INCLUDE_DIRECTORIES( BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

SET( SRC_FILES

    RepoManLoggerModule.cpp
    Listener.cpp
)

SET( HDR_FILES

    RepoManLoggerModule.hpp
    Listener.hpp
    TemplateNames.hpp
)

SET( UI_FILES
)

SET( HID_FILES
)

QT_MOC( MOC_FILES ${HDR_FILES} )
QT_UIC( UIC_FILES ${UI_FILES} )
HIC( HIC_FILES ${HID_FILES} )

ADD_MGV_MODULE(
    RepoManLogger

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${UIC_FILES} ${UI_FILES}
    ${HIC_FILES} ${HID_FILES}
)

