
QT_PREPARE(Core)

SET(SRC_FILES

    main.cpp
    Fixture.cpp

    Test_RepoMan.cpp
)

SET(HDR_FILES

    Fixture.hpp
)

ADD_CUSTOM_GTEST(
    testMacGitverCore

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
)

TARGET_LINK_LIBRARIES(testMacGitverCore MacGitverCore)
ADVANCE_TO_QT_PROJECT(testMacGitverCore)
