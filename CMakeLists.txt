
PROJECT( MacGitver )

CMAKE_MINIMUM_REQUIRED( VERSION 2.8.9 )
CMAKE_POLICY( SET CMP0009 NEW )

SET( UTILS_APP_NAME         MacGitver   CACHE INTERNAL "Foo" )
SET( UTILS_USE_PRIVATE_LIBS 1           CACHE INTERNAL "Foo" )

INCLUDE( cmake/Utils.cmake )
SET_OUTPUT_PATHS()
INCLUDE( cmake/QtMacros.cmake )

IF( UNIX OR APPLE )
    GCC_ADD_FLAG( "-Wall" )
    GCC_ADD_FLAG( "-fvisibility=hidden" )
ENDIF()

ADD_SUBDIRECTORY( 3rd )

# MacGitver itself
ADD_SUBDIRECTORY( tools )
INCLUDE( cmake/CfgComp.cmake )
ADD_SUBDIRECTORY( src )

# To Create a package, one can run "cpack -G DragNDrop CPackConfig.cmake" on Mac OS X
# where CPackConfig.cmake is created by including CPack
# And then there's ways to customize this as well

SET( CPACK_PACKAGE_NAME                  "MacGitver" )
SET( CPACK_PACKAGE_FILE_NAME             "MacGitver-v0.1.0" )
SET( CPACK_PACKAGE_VERSION               "0.1.0" )
SET( CPACK_PACKAGE_CONTACT               "MacGitver-Developers <macgitver@babbelbox.org>" )
SET( CPACK_PACKAGE_DESCRIPTION_SUMMARY   "A platform independent Git user interface based on Qt." )
SET( CPACK_BINARY_DRAGNDROP              ON )

INCLUDE( InstallRequiredSystemLibraries )
INCLUDE( CPack )
