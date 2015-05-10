
ADD_DEFINITIONS(-DDATADIR=\"${CMAKE_CURRENT_LIST_DIR}/Data\")

QT_PREPARE(Core Gui Widgets)    # We need QtGui in order to pull libMacGitverCore


INCLUDE_DIRECTORIES( BEFORE
    ${MACGITVER_LIBS_SOURCE_DIR}

    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

INCLUDE_DIRECTORIES( AFTER
    ${GITWRAP_INCLUDE_DIRS}
    ${HEAVEN_INCLUDE_DIRS}
)

SET(SRC_FILES

    Infra/main.cpp
    Infra/Fixture.cpp
    Infra/TempDirProvider.cpp

    Test_SHMParser.cpp
)

SET(HDR_FILES

    Infra/Fixture.hpp
    Infra/TempDirProvider.hpp

    Test_SHMParser.hpp
)

QT_MOC( MOC_FILES ${HDR_FILES} )

ADD_CUSTOM_GTEST(
    testMacGitverCore

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
)

TARGET_LINK_LIBRARIES(
    testMacGitverCore

    LINK_PRIVATE
        MacGitverCore
        GitWrap
)

ADVANCE_TO_QT_PROJECT(testMacGitverCore)
