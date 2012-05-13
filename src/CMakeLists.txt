
PROJECT( MAC_GITVER )


SET( QT_USE_QTMAIN				1 )
INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES(
    ${MAC_GITVER_SOURCE_DIR}
    ${MAC_GITVER_BINARY_DIR}
    ${libgit2_SOURCE_DIR}/include
)

SET( SRC_FILES

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

    Main/main.cpp
    Main/MainWindow.cpp

    Dlgs/Repository/CreateRepositoryDlg.cpp

    Views/Branches/BranchesView.cpp

    Views/Diff/Diff.cpp
    Views/Diff/DiffDisplay.cpp
    Views/Diff/DiffTreeFileList.cpp
    Views/Diff/DiffView.cpp

    Views/History/HistoryView.cpp
    Views/History/HistoryList.cpp
    Views/History/HistoryEntry.cpp
    Views/History/HistoryBuilder.cpp
    Views/History/HistoryModel.cpp

    Views/Refs/RefsView.cpp

    Views/Tags/TagsView.cpp

    Views/WorkingTree/IndexWidget.cpp
    Views/WorkingTree/IndexTree.cpp

)

SET( HDR_FILES

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

    Main/MainWindow.h

    Dlgs/Repository/CreateRepositoryDlg.h

    Views/Branches/BranchesView.h

    Views/Diff/Diff.h
    Views/Diff/DiffDisplay.h
    Views/Diff/DiffTreeFileList.h
    Views/Diff/DiffView.h

    Views/History/HistoryView.h
    Views/History/HistoryList.h
    Views/History/HistoryEntry.h
    Views/History/HistoryBuilder.h
    Views/History/HistoryModel.h

    Views/Refs/RefsView.h

    Views/Tags/TagsView.h

    Views/WorkingTree/IndexWidget.h
    Views/WorkingTree/IndexTree.h
)

SET( UI_FILES
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
