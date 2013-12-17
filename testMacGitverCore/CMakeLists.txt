
ADD_DEFINITIONS(-DDATADIR=\"${CMAKE_CURRENT_LIST_DIR}/Data\")

QT_PREPARE(Core Gui Widgets)    # We need QtGui in order to pull libMacGitverCore

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

SET(SRC_FILES

    Infra/main.cpp
    Infra/Fixture.cpp
    Infra/TempDirProvider.cpp
    Infra/TempRepo.cpp
    Infra/EventCatcher.cpp

    Test_RepoMan.cpp
    Test_Repo.cpp
    Test_Tag.cpp
    Test_Submodule.cpp
    Test_Branch.cpp
    Test_Remote.cpp
    Test_Namespace.cpp
    Test_RefTreeNode.cpp
    Test_RefLog.cpp

    Test_SHMParser.cpp
)

SET(HDR_FILES

    Infra/Fixture.hpp
    Infra/TempDirProvider.hpp
    Infra/TempRepo.hpp
    Infra/EventCatcher.hpp

    Test_RepoMan.hpp
    Test_Repo.hpp
    Test_Submodule.hpp
    Test_Tag.hpp
    Test_Branch.hpp
    Test_Remote.hpp
    Test_Namespace.hpp
    Test_RefTreeNode.hpp
    Test_RefLog.hpp

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
