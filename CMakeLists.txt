
PROJECT( MacGitver )
CMAKE_MINIMUM_REQUIRED( VERSION 2.8.9 )

CMAKE_POLICY( SET CMP0009 NEW )

FIND_PACKAGE(RAD-Tools REQUIRED)
FIND_PACKAGE(Heaven REQUIRED)
FIND_PACKAGE(GitWrap)

RAD_INIT(
    FEATURES
        QtMacros
        DoxygenTarget
        SplitSources
        Installation
)

INCLUDE(cmake/Utils.cmake)
INCLUDE(cmake/GoogleTest.cmake)
INCLUDE(cmake/MacGitver.cmake)

IF(UNIX)
    GCC_ADD_FLAG("-Wall")
    GCC_ADD_FLAG("-fvisibility=hidden")
    GCC_ADD_FLAG("-fvisibility-inlines-hidden")
    IF(APPLE)
        # TODO: Disable this for Mac OS X < 10.7 (Actually, XCode < 5.0)
        GCC_ADD_FLAG("-std=c\\\\+\\\\+11")
        GCC_ADD_FLAG("-fno-exceptions")
    ENDIF()
ENDIF()

ENABLE_TESTING()

ADD_SUBDIRECTORY(libMacGitverCore)
ADD_SUBDIRECTORY(MacGitverModules)
ADD_SUBDIRECTORY(MacGitverMain)

ADD_SUBDIRECTORY(AddOns)
