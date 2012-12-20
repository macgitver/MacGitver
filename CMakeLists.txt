
ADD_SUBDIRECTORY( Main )

# To Create a package, one can run "cpack -G DragNDrop CPackConfig.cmake" on Mac OS X
# where CPackConfig.cmake is created by including CPack
# And then there's ways to customize this as well

SET( CPACK_PACKAGE_NAME                  "MacGitver" )
SET( CPACK_PACKAGE_FILE_NAME             "MacGitver-v0.1.0" )
SET( CPACK_PACKAGE_VERSION               "0.1.0" )
SET( CPACK_PACKAGE_CONTACT               "MacGitver-Developers <macgitver@babbelbox.org>" )
SET( CPACK_PACKAGE_DESCRIPTION_SUMMARY   "A platform independent Git user interface based on Qt." )

IF( APPLE )
    SET( CPACK_BINARY_DRAGNDROP          ON )
ELSEIF( WIN32 )
    SET( CPACK_BINARY_NSIS               ON )
ENDIF()

INCLUDE( InstallRequiredSystemLibraries )
INCLUDE( CPack )
