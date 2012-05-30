
PROJECT( MGV_GITWRAP )

SET( QT_DONT_USE_QTGUI		1 )
INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES(
	${MGV_GITWRAP_SOURCE_DIR}
	${MGV_GITWRAP_BINARY_DIR}
	${libgit2_SOURCE_DIR}/include
)

SET( SRC_FILES

	Error.cpp
	Config.cpp
	ChangeListConsumer.cpp
	DiffList.cpp
	Git.cpp
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
	RevisionWalker.cpp
	Signature.cpp
)

SET( PUB_HDR_FILES

	Error.h
	Config.h
	ChangeListConsumer.h
	Git.h
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
)

SET( PRI_HDR_FILES

	Git_p.h
	ConfigPrivate.h
	DiffListPrivate.h
	IndexPrivate.h
	ObjectPrivate.h
	ReferencePrivate.h
	RemotePrivate.h
	RepoObject.h
	RepositoryPrivate.h
	RevisionWalkerPrivate.h
)

SET( HDR_FILES ${PRI_HDR_FILES} ${PUB_HDR_FILES} )

ADD_LIBRARY(
	GitWrap SHARED

	${SRC_FILES}
	${HDR_FILES}
)

TARGET_LINK_LIBRARIES(
	GitWrap

	${QT_LIBRARIES}
)

TARGET_LINK_LIBRARIES(
	GitWrap

	LINK_PRIVATE	git2
)
