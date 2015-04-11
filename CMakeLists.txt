
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
        SingleBinDir
        Installation
)

INCLUDE(cmake/Utils.cmake)
INCLUDE(cmake/GoogleTest.cmake)
INCLUDE(cmake/MacGitver.cmake)
INCLUDE(cmake/CfgComp.cmake)

IF(NOT MSVC)
    GCC_ADD_FLAG("-std=c\\\\+\\\\+11")
    GCC_ADD_FLAG("-stdlib=libc\\\\+\\\\+")
ENDIF()

IF(UNIX)
    GCC_ADD_FLAG("-Wall")
    GCC_ADD_FLAG("-W")
    GCC_ADD_FLAG("-fvisibility=hidden")
    GCC_ADD_FLAG("-fvisibility-inlines-hidden")
ENDIF()

ENABLE_TESTING()

SET(UTILS_APP_NAME MacGitver)

RAD_DEFINE_VERSION(MACGITVER_CORE 0 0 1)
RAD_DEFINE_VERSION(REPOMAN 0 0 1)

ADD_SUBDIRECTORY(CfgComp)

ADD_SUBDIRECTORY(Libs)
ADD_SUBDIRECTORY(Main)
ADD_SUBDIRECTORY(Modules)

ADD_SUBDIRECTORY(AddOns)

ADD_SUBDIRECTORY(testMacGitverCore)

INCLUDE(InstallRequiredSystemLibraries)

