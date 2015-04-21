
IF( UNIX )

    MACRO( GCC_ADD_FLAG _flag )

        IF( NOT DEFINED CMAKE_CXX_FLAGS )
            SET( CMAKE_CXX_FLAGS "${_flag}" CACHE STRING "C++ Build Flags" FORCE )
        ENDIF()

        IF( NOT "${CMAKE_CXX_FLAGS}" MATCHES ".*${_flag}.*" )
            STRING(REPLACE "\\" "" _flag2 ${_flag})
            SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_flag2}" CACHE STRING "C++ Build Flags" FORCE )
        ENDIF()

    ENDMACRO()

ENDIF()

MACRO(LIST_SUBDIRS _out _dir)
    FILE(GLOB _list ${_dir}/*)
    SET(${_out} "")
    FOREACH(_subdir ${_list})
        IF(IS_DIRECTORY ${_subdir})
            LIST(APPEND ${_out} ${_subdir})
        ENDIF()
    ENDFOREACH()
ENDMACRO()
