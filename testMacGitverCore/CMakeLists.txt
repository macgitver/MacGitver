
ADD_DEFINITIONS(-DDATADIR=\"${CMAKE_CURRENT_LIST_DIR}/Data\")

QT_PREPARE(Core)

SET(SRC_FILES

    main.cpp
    Fixture.cpp
    TempDirProvider.cpp
    TempRepo.cpp
    EventCatcher.cpp

    Test_RepoMan.cpp
    Test_Repo.cpp
    Test_Tag.cpp
    Test_Submodule.cpp
    Test_Branch.cpp
    Test_Remote.cpp
    Test_Namespace.cpp
    Test_RefTreeNode.cpp
    Test_RefLog.cpp
)

SET(HDR_FILES

    Fixture.hpp
    TempDirProvider.hpp
    TempRepo.hpp
    EventCatcher.hpp

    Test_RepoMan.hpp
    Test_Repo.hpp
    Test_Submodule.hpp
    Test_Tag.hpp
    Test_Branch.hpp
    Test_Remote.hpp
    Test_Namespace.hpp
    Test_RefTreeNode.hpp
    Test_RefLog.hpp
)

QT_MOC( MOC_FILES ${HDR_FILES} )

ADD_CUSTOM_GTEST(
    testMacGitverCore

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
)

TARGET_LINK_LIBRARIES(testMacGitverCore MacGitverCore GitWrap)
ADVANCE_TO_QT_PROJECT(testMacGitverCore)
