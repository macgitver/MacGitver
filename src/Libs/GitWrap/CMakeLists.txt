
PROJECT( MGV_GITWRAP )

SET( QT_DONT_USE_GUI        1 )
INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES(
    ${MGV_GITWRAP_SOURCE_DIR}
    ${MGV_GITWRAP_BINARY_DIR}
    ${libgit2_SOURCE_DIR}/include
)

SET( SRC_FILES

    Git.cpp
    Repository.cpp
    Index.cpp
    ObjectId.cpp
    Object.cpp
    ObjectTree.cpp
    ObjectBlob.cpp
    ObjectTag.cpp
    ObjectCommit.cpp
    Signature.cpp
    Reference.cpp
    RevisionWalker.cpp
)

SET( PUB_HDR_FILES

    Git.h
    Repository.h
    Index.h
    ObjectId.h
    Object.h
    ObjectTree.h
    ObjectBlob.h
    ObjectTag.h
    ObjectCommit.h
    Signature.h
    Reference.h
    RevisionWalker.h
)

SET( PRI_HDR_FILES

    Git_p.h

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

    LINK_PRIVATE    git2
)
