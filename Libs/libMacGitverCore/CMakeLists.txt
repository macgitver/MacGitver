
PROJECT(MGV_CORE)

QT_PREPARE(Core Gui Widgets Xml)

INCLUDE_DIRECTORIES( BEFORE
    ${MGV_CORE_SOURCE_DIR}
    ${MGV_CORE_BINARY_DIR}
)

SET( SRC_FILES

    App/MacGitver.cpp
    App/MgvPrimaryWindow.cpp

    Config/Config.cpp
    Config/ConfigUser.cpp

    Config/Ui/ConfigDialog.cpp
    Config/Ui/ConfigPage.cpp
    Config/Ui/GeneralConfigPage.cpp
    Config/Ui/ConfigPageProvider.cpp
    Config/Ui/RepoManConfigPage.cpp

    MacGitver/Module.cpp
    MacGitver/Modules.cpp
    MacGitver/GitPatchConsumer.cpp
    MacGitver/AutoRefresher.cpp

    RepoMan/Events.cpp
    RepoMan/RepoMan.cpp

    RepoMan/Frontend/Base.cpp
    RepoMan/Frontend/Branch.cpp
    RepoMan/Frontend/Namespace.cpp
    RepoMan/Frontend/Ref.cpp
    RepoMan/Frontend/RefLog.cpp
    RepoMan/Frontend/RefTreeNode.cpp
    RepoMan/Frontend/Remote.cpp
    RepoMan/Frontend/Repo.cpp
    RepoMan/Frontend/Submodule.cpp
    RepoMan/Frontend/Tag.cpp
    RepoMan/Frontend/Head.cpp

    RepoMan/Data/Base.cpp

    RepoMan/Private/Dumper.cpp

    SHMParser/ShellExpand.cpp

    Widgets/ExpandableDlg.cpp
    Widgets/FontSelectWidget.cpp
    Widgets/LineEdit.cpp
    Widgets/StringSelectorWidget.cpp
    Widgets/HeaderView.cpp
    Widgets/FlatTreeComboBox.cpp
    Widgets/FlatTreeModel.cpp
    Widgets/FlatTreeDelegate.cpp
    Widgets/SHA1Input.cpp
    Widgets/ShortCommitModel.cpp
    Widgets/RepoStateWidget.cpp
    Widgets/TreeViewCtxMenu.cpp
)

SET( PUB_HDR_FILES

    MacGitverApi.hpp

    App/MacGitver.hpp
    App/MgvPrimaryWindow.hpp

    Config/Config.h
    Config/ConfigUser.h
    Config/Ui/ConfigPage.hpp

    MacGitver/Module.h
    MacGitver/Modules.h
    MacGitver/GitPatchConsumer.hpp
    MacGitver/IRepositoryContext.hpp

    RepoMan/Core.hpp
    RepoMan/Base.hpp
    RepoMan/Events.hpp
    RepoMan/Ref.hpp
    RepoMan/Repo.hpp
    RepoMan/RepoMan.hpp
    RepoMan/Remote.hpp
    RepoMan/Tag.hpp
    RepoMan/Branch.hpp
    RepoMan/RefLog.hpp
    RepoMan/Submodule.hpp
    RepoMan/RefTreeNode.hpp
    RepoMan/Namespace.hpp
    RepoMan/Head.hpp

    SHMParser/ShellExpand.hpp

    Widgets/ExpandableDlg.hpp
    Widgets/FontSelectWidget.h
    Widgets/LineEdit.h
    Widgets/StringSelectorWidget.h
    Widgets/HeaderView.h
    Widgets/FlatTreeComboBox.h
    Widgets/FlatTreeModel.h
    Widgets/FlatTreeDelegate.h
    Widgets/SHA1Input.h
    Widgets/ShortCommitModel.h
    Widgets/TreeViewCtxMenu.hpp
)

SET( PRI_HDR_FILES

    App/MacGitverPrivate.hpp
    App/MgvPrimaryWindowPrivate.hpp

    MacGitver/AutoRefresher.hpp

    Config/Ui/ConfigDialog.hpp
    Config/Ui/GeneralConfigPage.hpp
    Config/Ui/RepoManConfigPage.hpp

    RepoMan/Private/Dumper.hpp

    RepoMan/Data/Base.hpp
    RepoMan/Data/Branch.hpp
    RepoMan/Data/Head.hpp
    RepoMan/Data/Namespace.hpp
    RepoMan/Data/Ref.hpp
    RepoMan/Data/RefLog.hpp
    RepoMan/Data/RefTreeNode.hpp
    RepoMan/Data/Remote.hpp
    RepoMan/Data/Repo.hpp
    RepoMan/Data/RepoManData.hpp
    RepoMan/Data/Submodule.hpp
    RepoMan/Data/Tag.hpp

    Widgets/StringSelectorWidgetPrivate.h
    Widgets/FlatTreeModelPrivate.h
    Widgets/RepoStateWidget.hpp
)

SET( UI_FILES

    App/AboutDlg.ui

    Config/Ui/ConfigDialog.ui
    Config/Ui/GeneralConfigPage.ui
    Config/Ui/RepoManConfigPage.ui

    Widgets/ExpandableDlg.ui
)

SET( HID_FILES

    App/MgvPrimaryWindowActions.hid
    RepoMan/Private/RepoManActions.hid

)

SET(RCC_FILES
    Images.rcc
)

SET( HDR_FILES ${PRI_HDR_FILES} ${PUB_HDR_FILES} )

SET(CCFG_FILES
    MacGitver/AutoRefresherCfg.ccfg
)

CFGCOMP(CFG_FILES ${CCFG_FILES})
QT_UIC( UIC_FILES ${UI_FILES} )
QT_MOC( MOC_FILES ${HDR_FILES} )
QT_RCC( RCC_DATA QRC_FILES ${RCC_FILES} )
HIC( HIC_FILES ${HID_FILES} )

ADD_QT_LIBRARY(
    MacGitverCore SHARED

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${UIC_FILES} ${UI_FILES}
    ${HIC_FILES} ${HID_FILES}
    ${QRC_FILES}
    ${RCC_DATA}
    ${CFG_FILES} ${CCFG_FILES}
)

TARGET_LINK_LIBRARIES(
    MacGitverCore

    LINK_PUBLIC
        HeavenActions
        HeavenIcons
        BlueSky
        Logger

    LINK_PRIVATE
        GitWrap
        DiffViews
        # Stairway
)

IF(UNIX AND NOT APPLE)
    SET_TARGET_PROPERTIES(
        MacGitverCore
        PROPERTIES  INSTALL_RPATH
        "\$ORIGIN"
    )
ENDIF()

RAD_SET_TARGET_VERSION( MacGitverCore MACGITVER_CORE )
RAD_INSTALL_LIBRARY( MacGitverCore MacGitverCore )
RAD_INSTALL_HEADERS( libMacGitverCore ${PUB_HDR_FILES} )
RAD_SPLIT_SOURCE_TREE( MacGitverCore )

RAD_ADD_DOXYGEN_TARGET(
    MacGitverCore
    ${MGV_CORE_SOURCE_DIR}/doc

    ${MGV_CORE_SOURCE_DIR}
)
