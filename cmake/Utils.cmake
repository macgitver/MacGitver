
IF( UNIX )

    MACRO( GCC_ADD_FLAG _flag )

        IF( NOT DEFINED CMAKE_CXX_FLAGS )
            SET( CMAKE_CXX_FLAGS "${_flag}" CACHE STRING "C++ Build Flags" FORCE )
        ENDIF()

        IF( NOT "${CMAKE_CXX_FLAGS}" MATCHES ".*${_flag}.*" )
            SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_flag}" CACHE STRING "C++ Build Flags" FORCE )
        ENDIF()

    ENDMACRO()

ENDIF()

MACRO( SET_OUTPUT_PATHS )

    IF( APPLE )
        MESSAGE( STATUS "Build strategy for Mac OS X" )
        # For MacOSX:
        # Don't set EXECUTEABLE_OUTPUT_PATH
        # But point LIBRARY_OUTPUT_PATH into the App-Bundle
        SET( LIBRARY_OUTPUT_DIRECTORY
             ${CMAKE_BINARY_DIR}/${UTILS_APP_NAME}.app/Contents/MacOSX
        )

    ELSEIF( WIN32 )
        MESSAGE( STATUS "Build strategy for Windows" )
        # For Windows:
        # If we're an MSVC IDE Build, do nothing - otherwise treat same as unicies:
        #IF( NOT MSVC_IDE )
            SET( EXECUTABLE_OUTPUT_DIRECTORY    ${CMAKE_BINARY_DIR}/bin )
            SET( LIBRARY_OUTPUT_DIRECTORY       ${CMAKE_BINARY_DIR}/bin )
        #ENDIF()

    ELSEIF( UNIX )
        MESSAGE( STATUS "Build strategy for Linux/Unicies" )
        # For linux and other unicies but not macosx, output binaries and libraries to
        # $BUILD_DIR/bin

        SET( EXECUTABLE_OUTPUT_DIRECTORY    ${CMAKE_BINARY_DIR}/bin )
        SET( LIBRARY_OUTPUT_DIRECTORY       ${CMAKE_BINARY_DIR}/bin )

    ELSE()

        MESSAGE( FATAL_ERROR "Only MacOSX, Windows and linux is supported" )

    ENDIF()
ENDMACRO()

# Turn the target ${_target} into a framework.
#
FUNCTION( SET_FRAMEWORK _target )

    SET_TARGET_PROPERTIES(
        ${_target}
        PROPERTIES  FRAMEWORK TRUE
    )

    IF( APPLE )
        # If we're actually on MacOSX, we set the lib-output-dir to Contetns/Frameworks instead
        # Contets/MacOSX now
        SET_TARGET_PROPERTIES(
            ${_target}
            PROPERTIES  LIBRARY_OUTPUT_DIRECTORY
                        ${CMAKE_BINARY_DIR}/${UTILS_APP_NAME}.app/Contents/Frameworks
        )
    ENDIF()

ENDFUNCTION()

# Turn the target ${_target} into a framework and install it.
#
# On MacOSX, do nothing but invoke SET_FRAMEWORK - BundleFrameworks will copy it at the right
# localtions. On other Plattforms install Runtime to bin and Library to lib for component
# ${_component}
FUNCTION( INSTALL_FRAMEWORK _target _component )

    SET_FRAMEWORK( ${_target} )

    IF( APPLE )
        IF( UTILS_USE_PRIVATE_LIBS )

            SET( dest "${UTILS_APP_NAME}.app/Contents/Frameworks" )

            INSTALL(
                TARGETS     ${_target}
                FRAMEWORK   DESTINATION ${dest} COMPONENT ${_component}
                BUNDLE      DESTINATION ${dest} COMPONENT ${_component}
                RUNTIME     DESTINATION ${dest} COMPONENT ${_component}
                LIBRARY     DESTINATION ${dest} COMPONENT ${_component}
            )

        ELSE()

            MESSAGE( FATAL_ERROR
                     "Erm, what shall we do if we're installing FWs and not using them privately?" )

        ENDIF()

    ELSEIF( UNIX )
        # On unix we insall libraries to libexec/macgitver and executables to bin.
        # That means, each library needs the "$ORIGIN" r-path.

        IF( UTILS_USE_PRIVATE_LIBS )
            SET( dest "libexec/${UTILS_APP_NAME}" )
        ELSE()
            SET( dest "lib" )
        ENDIF()

        SET_TARGET_PROPERTIES(
            ${_target}
            PROPERTIES  INSTALL_RPATH "\$ORIGIN/../../lib:\$ORIGIN"
                        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${dest}
        )

        INSTALL(
            TARGETS     ${_target}
            LIBRARY     DESTINATION ${dest} COMPONENT ${_component}
        )

    ELSEIF( WIN32 )

        SET_TARGET_PROPERTIES(
            ${_target}
            PROPERTIES  LIBRARY_OUTPUT_DIRECTORY
                        "${CMAKE_BINARY_DIR}/bin"
                        RUNTIME_OUTPUT_DIRECTORY
                        "${CMAKE_BINARY_DIR}/bin"
        )

        INSTALL(
            TARGETS     ${_target}
            RUNTIME     DESTINATION bin COMPONENT ${_component}
            LIBRARY     DESTINATION lib COMPONENT ${_component}
        )

    ENDIF()

ENDFUNCTION()

