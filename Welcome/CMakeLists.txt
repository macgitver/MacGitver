
PROJECT( MGV_MOD_WELCOME )

QT_PREPARE( Core Gui Widgets WebKit )

INCLUDE_DIRECTORIES(
    ${MGV_MOD_WELCOME_SOURCE_DIR}
    ${MGV_MOD_WELCOME_BINARY_DIR}
)

SET( SRC_FILES

    WelcomeView.cpp
    WelcomeModule.cpp
)

SET( HDR_FILES

    WelcomeView.h
    WelcomeModule.h
)

SET( HID_FILES

)

SET( RCC_FILES

    ModWelcome.rcc
)

QT_MOC( MOC_FILES ${HDR_FILES} )
QT_RCC( RCC_DATA QRC_FILES ${RCC_FILES} )
HIC( HIC_FILES ${HID_FILES} )

ADD_MGV_MODULE(
    Welcome

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${QRC_FILES}
    ${HIC_FILES} ${HID_FILES}
    ${RCC_DATA}
)

MSVC_SPLIT_SOURCES( ModWelcome )
