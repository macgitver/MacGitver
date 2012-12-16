
PROJECT( MACGITVER )

QT_PREPARE( Core Gui Widgets Xml Main )

INCLUDE_DIRECTORIES(
    ${MACGITVER_SOURCE_DIR}
    ${MACGITVER_BINARY_DIR}

    ${MGV_CORE_SOURCE_DIR}
)

SET( SRC_FILES

    main.cpp
)

SET( RCC_FILES

    MacGitver.rcc
)

QT_RCC( RCC_DATA QRC_FILES ${RCC_FILES} )

ADD_QT_EXECUTABLE(
    MacGitver

    MACOSX_BUNDLE
    WIN32

    ${SRC_FILES}
#   ${HDR_FILES}
#   ${MOC_FILES}
    ${QRC_FILES}
    ${RCC_DATA}
)

TARGET_LINK_LIBRARIES(
    MacGitver

    GitWrap
    Heaven
    MacGitverCore
)

INSTALL(
    TARGETS     MacGitver
    BUNDLE      DESTINATION .   COMPONENT Runtime
    RUNTIME     DESTINATION bin COMPONENT Runtime
    LIBRARY     DESTINATION lib COMPONENT Runtime
)

IF( UNIX )

    SET_TARGET_PROPERTIES(  MacGitver
        PROPERTIES          INSTALL_RPATH "\$ORIGIN/macgitver/lib:\$ORIGIN/macgitver/modules:\$ORIGIN/"
    )

ENDIF()
#--------------------------------------------------------------------------------
# installation stuff below
#--------------------------------------------------------------------------------

SET(plugin_dest_dir bin)
SET(qtconf_dest_dir bin)
SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/MacGitver")
IF(APPLE)
  SET( CPACK_BUNDLE_NAME "MacGitver-v0.1.0" )
  SET(plugin_dest_dir MacGitver.app/Contents/MacOS)
  SET(qtconf_dest_dir MacGitver.app/Contents/Resources)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/MacGitver.app")
ENDIF(APPLE)
IF(WIN32)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/MacGitver.exe")
ENDIF(WIN32)

#--------------------------------------------------------------------------------
# Install needed Qt plugins by copying directories from the qt installation
# One can cull what gets copied by using 'REGEX "..." EXCLUDE'
#INSTALL(DIRECTORY "${QT_PLUGINS_DIR}/imageformats" DESTINATION ${plugin_dest_dir}/plugins COMPONENT Runtime)
INSTALL(
    DIRECTORY   ${QT_PLUGINS_DIR}/imageformats
    DESTINATION ${plugin_dest_dir}/plugins/imageformats
    COMPONENT   Runtime
)

INSTALL(
    DIRECTORY   ${QT_PLUGINS_DIR}/codecs
    DESTINATION ${plugin_dest_dir}/plugins/codecs
    COMPONENT   Runtime
)

INSTALL(
    DIRECTORY   ${QT_PLUGINS_DIR}/iconengines
    DESTINATION ${plugin_dest_dir}/plugins/iconengines
    COMPONENT   Runtime
)

#--------------------------------------------------------------------------------
# install a qt.conf file
# this inserts some cmake code into the install script to write the file
INSTALL(CODE "
    file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${qtconf_dest_dir}/qt.conf\" \"\")
    " COMPONENT Runtime)


#--------------------------------------------------------------------------------
# Use BundleUtilities to get all other dependencies for the application to work.
# It takes a bundle or executable along with possible plugins and inspects it
# for dependencies.  If they are not system dependencies, they are copied.

# directories to look for dependencies
SET( DIRS ${QT_LIBRARY_DIR} )

IF( APPLE )

    LIST( APPEND DIRS "${CMAKE_BINARY_DIR}/MacGitver.app/Contents/Frameworks" )

ELSEIF( UNIX )

    LIST( APPEND DIRS "${CMAKE_BINARY_DIR}/bin" )

ELSEIF( WIN32 )

    LIST( APPEND DIRS "${CMAKE_BINARY_DIR}/bin" )

ENDIF()

INSTALL( CODE "
    CMAKE_POLICY( SET CMP0011 NEW )
    CMAKE_POLICY( SET CMP0009 NEW )
    file(GLOB_RECURSE PLUGINS
      \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\"
      \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/*.mgv\")
    include(BundleUtilities)
    fixup_bundle(\"${APPS}\" \"\${PLUGINS}\" \"${DIRS}\")
    " COMPONENT Runtime
)
