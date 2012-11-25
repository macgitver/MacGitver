
PROJECT( MGV_CORE )

QT_PREPARE( Core Gui Widgets Xml )

INCLUDE_DIRECTORIES(
    ${MGV_CORE_SOURCE_DIR}
    ${MGV_CORE_BINARY_DIR}
)

SET( SRC_FILES

    Config/Config.cpp
    Config/ConfigUser.cpp
    Config/UserLevelDefinition.cpp

    Interfaces/IConfigDialog.cpp
    Interfaces/IConfigPage.cpp
    Interfaces/IMainWindow.cpp
    Interfaces/IRepositoryCore.cpp
    Interfaces/ILog.cpp

    MacGitver/MacGitver.cpp
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
)

SET( PUB_HDR_FILES

    MacGitverApi.hpp

    Config/Config.h
    Config/ConfigUser.h
    Config/UserLevelDefinition.h

    Interfaces/IConfigDialog.h
    Interfaces/IConfigPage.h
    Interfaces/IMainWindow.h
    Interfaces/IRepositoryCore.h
    Interfaces/ILog.h

    MacGitver/MacGitver.h
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
)

SET( PRI_HDR_FILES

    Widgets/StringSelectorWidgetPrivate.h
    Widgets/FlatTreeModelPrivate.h
    Widgets/RepoStateWidget.hpp
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
    DiffViews
)

#INSTALL_MGV_LIB( MacGitverCore )
