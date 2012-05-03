
PROJECT( MAC_GITVER )


SET( QT_USE_QTMAIN				1 )
INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES(
    ${MAC_GITVER_SOURCE_DIR}
    ${MAC_GITVER_BINARY_DIR}
    ${libgit2_SOURCE_DIR}/include
)

SET( SRC_FILES

    main.cpp

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

    MainWindow.cpp

    Repo/CreateRepositoryDlg.cpp

    Index/IndexWidget.cpp
    Index/IndexTree.cpp
)

SET( HDR_FILES

    MainWindow.h

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

    Repo/CreateRepositoryDlg.h

    Index/IndexWidget.h
    Index/IndexTree.h
)

SET( UI_FILES
    Repo/CreateRepositoryDlg.ui
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
