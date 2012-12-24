
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

    App/CoreLog.cpp
    App/MacGitver.cpp
    App/MgvPrimaryWindow.cpp

    Config/Config.cpp
    Config/ConfigUser.cpp
    Config/UserLevelDefinition.cpp

    Config/Ui/ConfigDialog.cpp
    Config/Ui/ConfigPage.cpp
    Config/Ui/GeneralConfigPage.cpp

    Interfaces/IRepositoryCore.cpp

    MacGitver/Module.cpp
    MacGitver/Modules.cpp
    MacGitver/FSWatcher.cpp
    MacGitver/GitPatchConsumer.cpp
    MacGitver/RepositoryInfo.cpp
    MacGitver/RepoManager.cpp

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

    App/CoreLog.hpp
    App/MacGitver.hpp
    App/MgvPrimaryWindow.hpp

    Config/Config.h
    Config/ConfigUser.h
    Config/UserLevelDefinition.h
    Config/Ui/ConfigPage.hpp

    Interfaces/IRepositoryCore.h

    MacGitver/Module.h
    MacGitver/Modules.h
    MacGitver/FSWatcher.h
    MacGitver/GitPatchConsumer.hpp
    MacGitver/RepositoryInfo.hpp
    MacGitver/RepoManager.hpp

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
    ${UIC_FILES}
    ${HIC_FILES}
)

TARGET_LINK_LIBRARIES(
    MacGitverCore

    LINK_PRIVATE
    Heaven
    GitWrap
    DiffViews
)

INSTALL_FRAMEWORK( MacGitverCore Runtime )

