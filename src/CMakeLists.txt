
PROJECT( MAC_GITVER )


SET( QT_USE_QTMAIN				1 )
INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES(
    ${MAC_GITVER_SOURCE_DIR}
    ${MAC_GITVER_BINARY_DIR}
    ${libgit2_SOURCE_DIR}/include
)

SET( SRC_FILES

    Libs/Diff/Diff.cpp
    Libs/Diff/DiffHunk.cpp
    Libs/Diff/Patch.cpp
    Libs/Diff/PatchFile.cpp

    Libs/Git/Git.cpp
    Libs/Git/Repository.cpp
    Libs/Git/Index.cpp
    Libs/Git/ObjectId.cpp
    Libs/Git/Object.cpp
    Libs/Git/ObjectTree.cpp
    Libs/Git/ObjectBlob.cpp
    Libs/Git/ObjectTag.cpp
    Libs/Git/ObjectCommit.cpp
    Libs/Git/Signature.cpp
    Libs/Git/Reference.cpp
    Libs/Git/RevisionWalker.cpp

    Libs/Heaven/HContainer.cpp
    Libs/Heaven/HView.cpp
    Libs/Heaven/HTopLevelWidget.cpp
    Libs/Heaven/HDecorator.cpp

    Libs/Core/MacGitver.cpp
    Libs/Core/Module.cpp
    Libs/Core/Modules.cpp
    Libs/Core/ConfigPage.cpp
    Libs/Core/ConfigDlg.cpp
    Libs/Core/MainWindow.cpp

    Main/main.cpp

    Dlgs/Repository/CreateRepositoryDlg.cpp

    Views/Branches/BranchesView.cpp

    Views/Diff/DiffFrame.cpp
    Views/Diff/DiffRawHighlighter.cpp
    Views/Diff/DiffSplitter.cpp
    Views/Diff/DiffTreeFileList.cpp
    Views/Diff/DiffView.cpp
    Views/Diff/DiffWidget.cpp

    Modules/History/HistoryView.cpp
    Modules/History/HistoryList.cpp
    Modules/History/HistoryEntry.cpp
    Modules/History/HistoryBuilder.cpp
    Modules/History/HistoryModel.cpp
    Modules/History/HistoryModule.cpp

    Views/Refs/RefsView.cpp

    Views/Tags/TagsView.cpp

    Views/WorkingTree/IndexWidget.cpp
    Views/WorkingTree/IndexTree.cpp

)

SET( HDR_FILES

    Libs/Diff/Diff.h
    Libs/Diff/DiffHunk.h
    Libs/Diff/Patch.h
    Libs/Diff/PatchFile.h

    Libs/Git/Git.h
    Libs/Git/Git_p.h
    Libs/Git/Repository.h
    Libs/Git/Index.h
    Libs/Git/ObjectId.h
    Libs/Git/Object.h
    Libs/Git/ObjectTree.h
    Libs/Git/ObjectBlob.h
    Libs/Git/ObjectTag.h
    Libs/Git/ObjectCommit.h
    Libs/Git/Signature.h
    Libs/Git/Reference.h
    Libs/Git/RevisionWalker.h

    Libs/Heaven/HContainer.h
    Libs/Heaven/HView.h
    Libs/Heaven/HTopLevelWidget.h
    Libs/Heaven/HDecorator.h

    Libs/Core/MacGitver.h
    Libs/Core/Module.h
    Libs/Core/Modules.h
    Libs/Core/ConfigPage.h
    Libs/Core/ConfigDlg.h
    Libs/Core/MainWindow.h

    Dlgs/Repository/CreateRepositoryDlg.h

    Views/Branches/BranchesView.h

    Views/Diff/DiffFrame.h
    Views/Diff/DiffRawHighlighter.h
    Views/Diff/DiffSplitter.h
    Views/Diff/DiffTreeFileList.h
    Views/Diff/DiffView.h
    Views/Diff/DiffWidget.h

    Modules/History/HistoryView.h
    Modules/History/HistoryList.h
    Modules/History/HistoryEntry.h
    Modules/History/HistoryBuilder.h
    Modules/History/HistoryModel.h
    Modules/History/HistoryModule.h

    Views/Refs/RefsView.h

    Views/Tags/TagsView.h

    Views/WorkingTree/IndexWidget.h
    Views/WorkingTree/IndexTree.h
)

SET( UI_FILES

    Libs/Core/ConfigDlg.ui
    Dlgs/Repository/CreateRepositoryDlg.ui
)

SET( RCC_FILES

    Main/MacGitver.rcc
)

QT4_MOC( MOC_FILES ${HDR_FILES} )
QT4_UIC( UIC_FILES ${UI_FILES} )
QT4_ADD_RESOURCES( QRC_FILES ${RCC_FILES} )

ADD_EXECUTABLE(
    MacGitver WIN32

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${UIC_FILES}
    ${QRC_FILES}
)

TARGET_LINK_LIBRARIES(
    MacGitver

    git2
    ${QT_LIBRARIES}
)
