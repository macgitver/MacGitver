
QT_PREPARE(Core)

SET(SRC_FILES

    main.cpp

    Test_RepoMan.cpp
)

ADD_CUSTOM_GTEST(
    testMacGitverCore

    ${SRC_FILES}
    ${HDR_FILES}
    ${UIC_FILES} ${UI_FILES}
)

TARGET_LINK_LIBRARIES(testMacGitverCore MacGitverCore)
ADVANCE_TO_QT_PROJECT(testMacGitverCore)
