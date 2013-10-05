
PROJECT( MGV_CORE )

QT_PREPARE( Core Gui Widgets Xml )

INCLUDE_DIRECTORIES(
    ${libDiffViews_includes}
    ${libHeaven_includes}
    ${libGitWrap_includes}
    ${libMacGitverCore_includes}
    # And our own source + binary directory
    ${MGV_CORE_SOURCE_DIR}
    ${MGV_CORE_BINARY_DIR}
)

SET( SRC_FILES

    App/MacGitver.cpp
    App/MgvPrimaryWindow.cpp

    Log/LogEvent.cpp
    Log/LogChannel.cpp
    Log/LogTemplate.cpp
    Log/LogManager.cpp
    Log/LogConsumer.cpp

    Config/Config.cpp
    Config/ConfigUser.cpp
    Config/UserLevelDefinition.cpp

    Config/Ui/ConfigDialog.cpp
    Config/Ui/ConfigPage.cpp
    Config/Ui/GeneralConfigPage.cpp
    Config/Ui/ConfigPageProvider.cpp

    MacGitver/Module.cpp
    MacGitver/Modules.cpp
    MacGitver/GitPatchConsumer.cpp

    RepoMan/Base.cpp
    RepoMan/Dumper.cpp
    RepoMan/Events.cpp
    RepoMan/CollectionNode.cpp
    RepoMan/Ref.cpp
    RepoMan/Repo.cpp
    RepoMan/RepoMan.cpp
    RepoMan/Remote.cpp
    RepoMan/Tag.cpp
    RepoMan/Branch.cpp
    RepoMan/RefLog.cpp
    RepoMan/Submodule.cpp
    RepoMan/RefTreeNode.cpp
    RepoMan/Namespace.cpp

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
    Config/UserLevelDefinition.h
    Config/Ui/ConfigPage.hpp

    Log/LogEvent.hpp
    Log/LogChannel.hpp
    Log/LogTemplate.hpp
    Log/LogManager.hpp
    Log/LogConsumer.hpp

    MacGitver/Module.h
    MacGitver/Modules.h
    MacGitver/GitPatchConsumer.hpp
    MacGitver/IRepositoryContext.hpp

    RepoMan/Base.hpp
    RepoMan/Dumper.hpp
    RepoMan/Events.hpp
    RepoMan/CollectionNode.hpp
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

    Config/Ui/ConfigDialog.hpp
    Config/Ui/GeneralConfigPage.hpp

    Widgets/StringSelectorWidgetPrivate.h
    Widgets/FlatTreeModelPrivate.h
    Widgets/RepoStateWidget.hpp
)

SET( UI_FILES

    Config/Ui/ConfigDialog.ui
    Config/Ui/GeneralConfigPage.ui

    App/AboutDlg.ui
)

SET( HID_FILES

    App/MgvPrimaryWindowActions.hid

)

SET( HDR_FILES ${PRI_HDR_FILES} ${PUB_HDR_FILES} )

QT_UIC( UIC_FILES ${UI_FILES} )
QT_MOC( MOC_FILES ${HDR_FILES} )
HIC( HIC_FILES ${HID_FILES} )

ADD_QT_LIBRARY(
    MacGitverCore SHARED

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${UIC_FILES} ${UI_FILES}
    ${HIC_FILES} ${HID_FILES}
)

TARGET_LINK_LIBRARIES(
    MacGitverCore

    LINK_PRIVATE
    Heaven
    GitWrap
    DiffViews
)

INSTALL_FRAMEWORK( MacGitverCore Runtime )

MSVC_SPLIT_SOURCES( MacGitverCore )

ADD_DOXYGEN_TARGET(
    MacGitverCore
    ${MGV_CORE_SOURCE_DIR}/doc

    ${MGV_CORE_SOURCE_DIR}
)
