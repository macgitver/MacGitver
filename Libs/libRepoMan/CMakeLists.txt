
PROJECT(REPOMAN)

QT_PREPARE(Core Gui Widgets)

INCLUDE_DIRECTORIES( BEFORE
    ${REPOMAN_SOURCE_DIR}
    ${REPOMAN_BINARY_DIR}
)

SET( SRC_FILES

    Events.cpp
    RepoMan.cpp

    Frontend/Base.cpp
    Frontend/Branch.cpp
    Frontend/Namespace.cpp
    Frontend/Reference.cpp
    Frontend/RefLog.cpp
    Frontend/RefTreeNode.cpp
    Frontend/Remote.cpp
    Frontend/Repo.cpp
    Frontend/Submodule.cpp
    Frontend/Tag.cpp
    Frontend/Head.cpp

    Data/Base.cpp
    Data/Branch.cpp
    Data/Namespace.cpp
    Data/Reference.cpp
    Data/RefLog.cpp
    Data/RefTreeNode.cpp
    Data/Remote.cpp
    Data/Repo.cpp
    Data/Submodule.cpp
    Data/Tag.cpp
    Data/Head.cpp

    Private/Dumper.cpp

    Backend/RepoMan.cpp
)

SET( PUB_HDR_FILES

    Core.hpp
    Events.hpp
    RepoMan.hpp

    Frontend/Base.hpp
    Frontend/Reference.hpp
    Frontend/Repo.hpp
    Frontend/Remote.hpp
    Frontend/Tag.hpp
    Frontend/Branch.hpp
    Frontend/RefLog.hpp
    Frontend/Submodule.hpp
    Frontend/RefTreeNode.hpp
    Frontend/Namespace.hpp
    Frontend/Head.hpp
)

SET( PRI_HDR_FILES

    Private/Dumper.hpp

    Data/Base.hpp
    Data/Branch.hpp
    Data/Namespace.hpp
    Data/Reference.hpp
    Data/RefLog.hpp
    Data/RefTreeNode.hpp
    Data/Remote.hpp
    Data/Repo.hpp
    Data/RepoMan.hpp
    Data/Submodule.hpp
    Data/Tag.hpp
    Data/Head.hpp

    Frontend/BaseInternal.hpp

    Backend/RepoMan.hpp
    Backend/RepoLocker.hpp
)

SET( HID_FILES

    Private/RepoManActions.hid
)

SET( HDR_FILES ${PRI_HDR_FILES} ${PUB_HDR_FILES} )

QT_MOC( MOC_FILES ${HDR_FILES} )
HIC( HIC_FILES ${HID_FILES} )

ADD_QT_LIBRARY(
    RepoMan SHARED

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${HIC_FILES} ${HID_FILES}
)

TARGET_LINK_LIBRARIES(
    RepoMan

    LINK_PUBLIC
        HeavenActions
        HeavenIcons
        BlueSky

    LINK_PRIVATE
        GitWrap
)

IF(UNIX AND NOT APPLE)
    SET_TARGET_PROPERTIES(
        RepoMan
        PROPERTIES  INSTALL_RPATH
        "\$ORIGIN"
    )
ENDIF()

RAD_DEFINE_VERSION(REPOMAN 0 0 1)
RAD_SET_TARGET_VERSION( RepoMan REPOMAN )
RAD_INSTALL_LIBRARY( RepoMan RepoMan )
RAD_INSTALL_HEADERS( libRepoMan ${PUB_HDR_FILES} )
RAD_SPLIT_SOURCE_TREE( RepoMan )
