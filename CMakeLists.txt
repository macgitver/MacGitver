
PROJECT(MacGitver)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

CMAKE_POLICY( SET CMP0009 NEW )

FIND_PACKAGE(RAD-Tools REQUIRED)
FIND_PACKAGE(GitWrap REQUIRED)
FIND_PACKAGE(Heaven REQUIRED)

RAD_INIT(
    FEATURES
        QtMacros
        DoxygenTarget
        SplitSources
        Installation
)

#INCLUDE(cmake/Utils.cmake)
#INCLUDE(cmake/GoogleTest.cmake)
#INCLUDE(cmake/MacGitver.cmake)

IF(NOT MSVC)
    GCC_ADD_FLAG("-std=c\\\\+\\\\+11")
ENDIF()

IF(UNIX)
    GCC_ADD_FLAG("-Wall")
    GCC_ADD_FLAG("-fvisibility=hidden")
    GCC_ADD_FLAG("-fvisibility-inlines-hidden")
    IF(APPLE)
        # TODO: Disable this for Mac OS X < 10.7 (Actually, XCode < 5.0)
        GCC_ADD_FLAG("-fno-exceptions")
    ENDIF()
ENDIF()

ENABLE_TESTING()

SET(UTILS_APP_NAME MacGitver)

RAD_DEFINE_VERSION(MACGITVER_CORE 0 0 1)

ADD_SUBDIRECTORY(CfgComp)
INCLUDE(cmake/CfgComp.cmake)

ADD_SUBDIRECTORY(Libs)
ADD_SUBDIRECTORY(Main)
ADD_SUBDIRECTORY(Modules)

ADD_SUBDIRECTORY(testMacGitverCore)

INCLUDE(InstallRequiredSystemLibraries)
