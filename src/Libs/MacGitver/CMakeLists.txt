
PROJECT( MGV_CORE )

QT_PREPARE( Core Gui Widgets )

INCLUDE_DIRECTORIES(
    ${MGV_LIBS_SOURCE_DIR}
    ${MGV_CORE_SOURCE_DIR}
    ${MGV_CORE_BINARY_DIR}
)

SET( SRC_FILES

    MacGitver.cpp
    Module.cpp
    Modules.cpp
    FSWatcher.cpp
)

SET( PUB_HDR_FILES

    MacGitverApi.h
    MacGitver.h
    Module.h
    Modules.h
    FSWatcher.h
)

SET( PRI_HDR_FILES

)

SET( HDR_FILES ${PRI_HDR_FILES} ${PUB_HDR_FILES} )

QT_MOC( MOC_FILES ${HDR_FILES} )

ADD_QT_LIBRARY(
    MacGitverCore SHARED

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
)

TARGET_LINK_LIBRARIES(
    MacGitverCore

    LINK_PRIVATE
    Heaven
    GitWrap
)

TARGET_LINK_LIBRARIES(
    MacGitverCore

    LINK_PUBLIC     Interfaces
)

INSTALL_MGV_LIB( MacGitverCore )
