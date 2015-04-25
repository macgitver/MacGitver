
PROJECT( MGV_MOD_REPOSITORY )

QT_PREPARE( Core Gui Widgets )

INCLUDE_DIRECTORIES( BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

SET( SRC_FILES

    RepositoryModule.cpp

    RepoInfoModel.cpp
    RepoTreeView.cpp
    RepositoryContext.cpp

    CloneRepositoryDlg.cpp
    CreateRepositoryDlg.cpp
    ProgressDlg.cpp
)

SET( HDR_FILES

    RepositoryModule.h

    RepoInfoModel.hpp
    RepoTreeView.hpp
    RepositoryContext.hpp

    CloneRepositoryDlg.hpp
    CreateRepositoryDlg.h
    ProgressDlg.hpp
)

SET( UI_FILES

    CloneOptionsWdgt.ui
    ProgressDlg.ui
    CloneRepositoryDlg.ui
    CreateRepositoryDlg.ui
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

