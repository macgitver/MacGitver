
SET( libMacGitverCore_includes
    ${CMAKE_CURRENT_LIST_DIR}
    CACHE INTERNAL "libMacGitverCore include directories"
)

INCLUDE_DIRECTORIES(
    ${libGitWrap_includes}
    ${libHeaven_includes}
    ${libDiffViews_includes}
    ${libMacGitverCore_includes}
)

ADD_SUBDIRECTORY( CfgComp )
INCLUDE( cmake/CfgComp.cmake )

ADD_SUBDIRECTORY( libMacGitverCore )

