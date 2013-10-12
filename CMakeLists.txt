
PROJECT( MacGitver )
CMAKE_MINIMUM_REQUIRED( VERSION 2.8.9 )

CMAKE_POLICY( SET CMP0009 NEW )

SET( UTILS_APP_NAME         MacGitver   CACHE INTERNAL "Foo" )
SET( UTILS_USE_PRIVATE_LIBS 1           CACHE INTERNAL "Foo" )

INCLUDE( cmake/Utils.cmake )
INCLUDE( cmake/Doxygen.cmake )
SET_OUTPUT_PATHS()
INCLUDE( cmake/QtMacros.cmake )
INCLUDE( cmake/GoogleTest.cmake )

IF( UNIX )
    GCC_ADD_FLAG( "-Wall" )
    GCC_ADD_FLAG( "-fvisibility=hidden" )
ENDIF()

ADD_SUBDIRECTORY(libGitWrap)
ADD_SUBDIRECTORY(libHeaven)
ADD_SUBDIRECTORY(libDiffViews)

ADD_SUBDIRECTORY(libMacGitverCore)
ADD_SUBDIRECTORY(MacGitverModules)
ADD_SUBDIRECTORY(MacGitverMain)

ADD_SUBDIRECTORY(AddOns)
