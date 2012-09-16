
PROJECT( MGV_GITWRAP )

QT_PREPARE( Core -Gui )

INCLUDE_DIRECTORIES(
	${MGV_GITWRAP_SOURCE_DIR}
	${MGV_GITWRAP_BINARY_DIR}
	${libgit2_SOURCE_DIR}/include
)

SET( SRC_FILES

	Config.cpp
	ChangeListConsumer.cpp
	DiffList.cpp
	GitWrap.cpp
	Index.cpp
	Object.cpp
	ObjectBlob.cpp
	ObjectCommit.cpp
	ObjectId.cpp
	ObjectTag.cpp
	ObjectTree.cpp
	PatchConsumer.cpp
	RefSpec.cpp
	Reference.cpp
	Remote.cpp
	RepoObject.cpp
	Repository.cpp
	Result.cpp
	RevisionWalker.cpp
	Signature.cpp
	Submodule.cpp
	TreeBuilder.cpp
	TreeEntry.cpp
	BackgroundTasks.cpp
	BackgroundFetch.cpp
	BackgroundCheckout.cpp
	BackgroundClone.cpp
)

SET( PUB_HDR_FILES

	Result.h
	Config.h
	ChangeListConsumer.h
	GitWrap.h
	DiffList.h
	Index.h
	Object.h
	ObjectBlob.h
	ObjectCommit.h
	ObjectId.h
	ObjectTag.h
	ObjectTree.h
	RefSpec.h
	Reference.h
	Remote.h
	Repository.h
	RevisionWalker.h
	Signature.h
	Submodule.h
	TreeBuilder.h
	TreeEntry.h
	BackgroundTasks.h
	BackgroundFetch.h
	BackgroundCheckout.h
	BackgroundClone.h
)

SET( PRI_HDR_FILES

	GitWrapPrivate.h
	ConfigPrivate.h
	DiffListPrivate.h
	IndexPrivate.h
	ObjectPrivate.h
	ReferencePrivate.h
	RemotePrivate.h
	RepoObject.h
	RepositoryPrivate.h
	RevisionWalkerPrivate.h
	TreeBuilderPrivate.h
	TreeEntryPrivate.h
)

SET( HDR_FILES ${PRI_HDR_FILES} ${PUB_HDR_FILES} )

QT_MOC( MOC_FILES ${HDR_FILES} )

ADD_QT_LIBRARY(
	GitWrap SHARED

	${SRC_FILES}
	${HDR_FILES}
	${MOC_FILES}
)

TARGET_LINK_LIBRARIES(
	GitWrap

	LINK_PRIVATE	git2
)

INSTALL_MGV_LIB( GitWrap )
