
PROJECT(MacGitverCore)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

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

RAD_DEFINE_VERSION(MACGITVER_CORE 0 0 1)

SET(libMacGitverCore_includes
    ${CMAKE_CURRENT_LIST_DIR}
    CACHE INTERNAL "libMacGitverCore include directories"
)

INCLUDE_DIRECTORIES(
    ${GITWRAP_INCLUDE_DIRS}
    ${HEAVEN_INCLUDE_DIRS}
    ${libDiffViews_includes}
    ${libMacGitverCore_includes}
)

ADD_SUBDIRECTORY(CfgComp)
INCLUDE(cmake/CfgComp.cmake)

ADD_SUBDIRECTORY(libDiffViews)
ADD_SUBDIRECTORY(libMacGitverCore)
ADD_SUBDIRECTORY(Main)

ADD_SUBDIRECTORY(testMacGitverCore)

INCLUDE(InstallRequiredSystemLibraries)
