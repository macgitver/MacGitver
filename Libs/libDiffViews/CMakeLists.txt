
PROJECT( libDiffViews )

QT_PREPARE( Core Gui Widgets )

INCLUDE_DIRECTORIES(
    ${DIFFVIEWS_SOURCE_DIR}
    ${libDiffViews_SOURCE_DIR}
    ${libDiffViews_BINARY_DIR}
)

SET( SRC_FILES

    Model/Hunk.cpp
    Model/HunkPart.cpp
    Model/DiffLines.cpp
    Model/TextFilePatch.cpp
    Model/BinaryFilePatch.cpp
    Model/FilePatch.cpp
    Model/Patch.cpp

    Views/Interfaces/DiffViews.cpp
    Views/Interfaces/DiffView.cpp

    Views/Raw/RawView.cpp
    Views/Raw/RawHighlighter.cpp

    Views/Seq/SequentialView.cpp
    Views/Seq/SeqViewItem.cpp
    Views/Seq/SeqViewDelta.cpp
    Views/Seq/SeqViewHunk.cpp
    Views/Seq/SeqViewDiffStat.cpp
    Views/Seq/SeqViewContainer.cpp
)

SET( PUB_HDR_FILES

    DiffViewsApi.hpp

    Model/Hunk.hpp
    Model/HunkPart.hpp
    Model/DiffLines.hpp
    Model/FilePatch.hpp
    Model/BinaryFilePatch.hpp
    Model/TextFilePatch.hpp
    Model/Patch.hpp

    Views/Interfaces/DiffViews.hpp
    Views/Interfaces/DiffView.hpp
)

SET( PRIV_HDR_FILES

    Views/Raw/RawView.hpp
    Views/Raw/RawHighlighter.hpp

    Views/Seq/SequentialView.hpp
    Views/Seq/SeqViewItem.hpp
    Views/Seq/SeqViewDelta.hpp
    Views/Seq/SeqViewHunk.hpp
    Views/Seq/SeqViewDiffStat.hpp
    Views/Seq/SeqViewContainer.hpp
)


SET( HIC_FILES

)

SET( UI_FILES
)

SET( HDR_FILES ${PRIV_HDR_FILES} ${PUB_HDR_FILES} )

QT_MOC( MOC_FILES ${HDR_FILES} )
#HIC( HIC_FILES ${HID_FILES} )

ADD_QT_LIBRARY(
    DiffViews SHARED

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${HIC_FILES} ${HID_FILES}
)

RAD_SET_TARGET_VERSION(DiffViews MACGITVER_CORE)
RAD_INSTALL_LIBRARY(MacGitverCore DiffViews)
RAD_INSTALL_HEADERS(libDiffViews ${PUB_HDR_FILES})
RAD_SPLIT_SOURCE_TREE(DiffViews)

