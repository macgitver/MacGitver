
PROJECT( MAC_GITVER )


SET( QT_USE_QTMAIN				1 )
INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES(
    ${MAC_GITVER_SOURCE_DIR}
    ${MAC_GITVER_BINARY_DIR}
    ${libgit2_SOURCE_DIR}/include
)

SET( SRC_FILES

    Core/Git.cpp
    Core/Repository.cpp
    Core/Index.cpp
    Core/ObjectId.cpp
    Core/Object.cpp
    Core/ObjectTree.cpp
    Core/ObjectBlob.cpp
    Core/ObjectTag.cpp
    Core/ObjectCommit.cpp
    Core/Signature.cpp

    Heaven/HContainer.cpp
    Heaven/HView.cpp
    Heaven/HTopLevelWidget.cpp
    Heaven/HDecorator.cpp

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

    Views/Refs/RefsView.cpp

    Views/Tags/TagsView.cpp

    Views/WorkingTree/IndexWidget.cpp
    Views/WorkingTree/IndexTree.cpp

)

SET( HDR_FILES

    Core/Git.h
    Core/Git_p.h
    Core/Repository.h
    Core/Index.h
    Core/ObjectId.h
    Core/Object.h
    Core/ObjectTree.h
    Core/ObjectBlob.h
    Core/ObjectTag.h
    Core/ObjectCommit.h
    Core/Signature.h

    Heaven/HContainer.h
    Heaven/HView.h
    Heaven/HTopLevelWidget.h
    Heaven/HDecorator.h

    Main/MainWindow.h

    Dlgs/Repository/CreateRepositoryDlg.h

    Views/Branches/BranchesView.h

    Views/Diff/Diff.h
    Views/Diff/DiffDisplay.h
    Views/Diff/DiffTreeFileList.h
    Views/Diff/DiffView.h

    Views/History/HistoryView.h
    Views/History/HistoryList.h

    Views/Refs/RefsView.h

    Views/Tags/TagsView.h

    Views/WorkingTree/IndexWidget.h
    Views/WorkingTree/IndexTree.h
)

SET( UI_FILES
    Dlgs/Repository/CreateRepositoryDlg.ui
)

QT4_MOC( MOC_FILES ${HDR_FILES} )
QT4_UIC( UIC_FILES ${UI_FILES} )

ADD_EXECUTABLE(
    MacGitver WIN32

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${UIC_FILES}
)

TARGET_LINK_LIBRARIES(
    MacGitver

    git2
    ${QT_LIBRARIES}
)
