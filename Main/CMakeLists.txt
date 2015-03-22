
PROJECT( MACGITVER )

QT_PREPARE( Core Gui Widgets Xml WinMain )

INCLUDE_DIRECTORIES( BEFORE
    ${MACGITVER_LIBS_SOURCE_DIR}
    ${MACGITVER_SOURCE_DIR}
    ${MACGITVER_BINARY_DIR}
)

INCLUDE_DIRECTORIES( AFTER
    # Ensure external libs are last
    ${HEAVEN_INCLUDE_DIRS}
    ${GITWRAP_INCLUDE_DIRS}
)

SET( SRC_FILES

    main.cpp
)

SET( RCC_FILES

    MacGitver.rcc
)

QT_RCC( RCC_DATA QRC_FILES ${RCC_FILES} )

SET(MACOSX_BUNDLE_GUI_IDENTIFIER    "org.macgitver")
SET(MACOSX_BUNDLE_BUNDLE_NAME       "MacGitver")
#SET(MACOSX_BUNDLE_NSMAIN_NIB_FILE   "MainMenu")
SET(MACOSX_BUNDLE_NSPRINCIPAL_CLASS "NSApplication")

IF(APPLE)
    SET(RES_DIR ${CMAKE_BINARY_DIR}/MacGitver.app/Contents/Resources)
    FILE(MAKE_DIRECTORY ${RES_DIR})
    SET(ICNS_FILE ${RES_DIR}/MacGitver.icns)

    ADD_CUSTOM_COMMAND(
	OUTPUT       ${ICNS_FILE}
	COMMAND      iconutil -c icns -o ${ICNS_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/MGV.iconset
    )
ENDIF()


ADD_QT_EXECUTABLE(
    MacGitver

    MACOSX_BUNDLE
    WIN32

    ${SRC_FILES}
    ${QRC_FILES}
    ${RCC_DATA}
    Info-CMake.plist
    ${ICNS_FILE}
)

SET_TARGET_PROPERTIES(
    MacGitver
    PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info-CMake.plist
)

TARGET_LINK_LIBRARIES(
    MacGitver

    LINK_PRIVATE
        GitWrap
        MacGitverCore
)

#--------------------------------------------------------------------------------
# installation stuff below
#--------------------------------------------------------------------------------

INSTALL(
    TARGETS     MacGitver
    BUNDLE      DESTINATION .   COMPONENT Runtime
    RUNTIME     DESTINATION bin COMPONENT Runtime
    LIBRARY     DESTINATION lib COMPONENT Runtime
)

IF( APPLE OR WIN32 )
    # directories to look for dependencies
    SET( DIRS ${QT_LIBRARY_DIR} )

    IF( APPLE )
        SET( CPACK_BUNDLE_NAME "MacGitver-v0.1.0" )

        LIST( APPEND DIRS "${CMAKE_BINARY_DIR}/MacGitver.app/Contents/Frameworks" )

        SET(plugin_dest_dir MacGitver.app/Contents/MacOS)
        SET(qtconf_dest_dir MacGitver.app/Contents/Resources)
        SET(APPS "\${CMAKE_INSTALL_PREFIX}/MacGitver.app")

        SET_TARGET_PROPERTIES(  MacGitver
            PROPERTIES          RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
        )

    ELSEIF( WIN32 )

        LIST( APPEND DIRS "${CMAKE_BINARY_DIR}/bin" )

        SET(plugin_dest_dir bin )
        SET(qtconf_dest_dir bin )
        SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/MacGitver.exe")

        SET_TARGET_PROPERTIES(  MacGitver
            PROPERTIES          RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        )

    ENDIF()

    INSTALL(
        DIRECTORY   ${QT_PLUGINS_DIR}/imageformats
        DESTINATION ${plugin_dest_dir}/plugins/imageformats
        COMPONENT   Runtime
        FILES_MATCHING
        PATTERN     "*${CMAKE_SHARED_LIBRARY_SUFFIX}"
        PATTERN     "*d4${CMAKE_SHARED_LIBRARY_SUFFIX}" EXCLUDE
    )

    INSTALL(
        DIRECTORY   ${QT_PLUGINS_DIR}/codecs
        DESTINATION ${plugin_dest_dir}/plugins/codecs
        COMPONENT   Runtime
        FILES_MATCHING
        PATTERN     "*${CMAKE_SHARED_LIBRARY_SUFFIX}"
        PATTERN     "*d4${CMAKE_SHARED_LIBRARY_SUFFIX}" EXCLUDE
    )

    INSTALL(
        DIRECTORY   ${QT_PLUGINS_DIR}/iconengines
        DESTINATION ${plugin_dest_dir}/plugins/iconengines
        COMPONENT   Runtime
        FILES_MATCHING
        PATTERN     "*${CMAKE_SHARED_LIBRARY_SUFFIX}"
        PATTERN     "*d4${CMAKE_SHARED_LIBRARY_SUFFIX}" EXCLUDE
    )

    INSTALL(CODE "
        file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${qtconf_dest_dir}/qt.conf\" \"\")
        " COMPONENT Runtime)

    #--------------------------------------------------------------------------------
    # Use BundleUtilities to get all other dependencies for the application to work.
    # It takes a bundle or executable along with possible plugins and inspects it
    # for dependencies.  If they are not system dependencies, they are copied.


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

ELSEIF( UNIX )
    # On unix we insall libraries to libexec/macgitver and executables to bin.
    # That means, each excutable needs the "$ORIGIN/../libexec/{app}" r-path.

    SET_TARGET_PROPERTIES(  MacGitver
        PROPERTIES          INSTALL_RPATH "\$ORIGIN/../libexec/MacGitver:\$ORIGIN/../lib"
                            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)


ENDIF()
