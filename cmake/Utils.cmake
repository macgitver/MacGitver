
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
    SET( EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin )
    SET( LIBRARY_OUTPUT_PATH    ${CMAKE_BINARY_DIR}/bin )
ENDMACRO()

# Turn the target ${_target} into a framework.
#
FUNCTION( SET_FRAMEWORK _target )

    SET_TARGET_PROPERTIES( ${_target}
        PROPERTIES  FRAMEWORK TRUE
    )

ENDFUNCTION()

# Turn the target ${_target} into a framework and install it.
#
# On MacOSX, do nothing but invoke SET_FRAMEWORK - BundleFrameworks will copy it at the right
# localtions. On other Plattforms install Runtime to bin and Library to lib for component
# ${_component}
FUNCTION( INSTALL_FRAMEWORK _target _component )

    SET_FRAMEWORK( ${_target} )

    IF( NOT APPLE )
        INSTALL(
            TARGETS     ${_target}
            RUNTIME     DESTINATION bin COMPONENT ${_component}
            LIBRARY     DESTINATION lib COMPONENT ${_component}
        )
    ENDIF()

ENDFUNCTION()
