
PROJECT( MGV_MOD_REPOSITORY )

INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES(
    ${MGV_LIBS_SOURCE_DIR}
    ${MGV_MOD_REPOSITORY_SOURCE_DIR}
    ${MGV_MOD_REPOSITORY_BINARY_DIR}
)

SET( SRC_FILES

    RepositoryModule.cpp

    CreateRepositoryDlg.cpp
)

SET( HDR_FILES

    RepositoryModule.h

    CreateRepositoryDlg.h
)

SET( UI_FILES

    CreateRepositoryDlg.ui
)

SET( HID_FILES

    RepositoryModule.hid
)

QT4_MOC( MOC_FILES ${HDR_FILES} )
QT4_UIC( UIC_FILES ${UI_FILES} )
HIC( HIC_FILES ${HID_FILES} )

ADD_MGV_MODULE(
    Repository

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${UIC_FILES}
    ${HIC_FILES}
)
