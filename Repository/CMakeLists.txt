
PROJECT( MGV_MOD_REPOSITORY )

QT_PREPARE( Core Gui Widgets )

INCLUDE_DIRECTORIES(
    ${MGV_MOD_REPOSITORY_SOURCE_DIR}
    ${MGV_MOD_REPOSITORY_BINARY_DIR}
)

SET( SRC_FILES

    RepositoryModule.cpp

    RepoInfoModel.cpp
    RepoTreeView.cpp

    CreateRepositoryDlg.cpp
    CloneRepositoryDlg.cpp
    ProgressDlg.cpp
)

SET( HDR_FILES

    RepositoryModule.h

    RepoInfoModel.hpp
    RepoTreeView.hpp

    CreateRepositoryDlg.h
    CloneRepositoryDlg.h
    ProgressDlg.hpp
)

SET( UI_FILES

    CreateRepositoryDlg.ui
    CloneRepositoryDlg.ui
    ProgressDlg.ui
)

SET( HID_FILES

    RepositoryModule.hid
    RepoTreeViewCtxMenu.hid
)

QT_MOC( MOC_FILES ${HDR_FILES} )
QT_UIC( UIC_FILES ${UI_FILES} )
HIC( HIC_FILES ${HID_FILES} )

ADD_MGV_MODULE(
    Repository

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${UIC_FILES} ${UI_FILES}
    ${HIC_FILES} ${HID_FILES}
)

MSVC_SPLIT_SOURCES( ModRepository )
