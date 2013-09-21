
OPTION(Qt5 "Use Qt5" OFF)

# Common stuff:

MACRO(_ADD_NOCASTS)
    ADD_DEFINITIONS(
        -DQT_NO_CAST_FROM_ASCII
        -DQT_NO_CAST_TO_ASCII
        -DQT_NO_CAST_FROM_BYTEARRAY
    )
ENDMACRO()

MACRO(ADD_QT_LIBRARY _target)
    ADD_LIBRARY( ${_target} ${ARGN} )
    ADVANCE_TO_QT_PROJECT(${_target})
ENDMACRO()

MACRO(ADD_QT_EXECUTABLE _target)
    ADD_EXECUTABLE( ${_target} ${ARGN} )
    ADVANCE_TO_QT_PROJECT(${_target})
ENDMACRO(ADD_QT_EXECUTABLE)

# Paths are relative to Top-Src-Dir. Duh.
IF(Qt5)
    INCLUDE(cmake/Qt5.cmake)
ELSE()
    INCLUDE(cmake/Qt4.cmake)
ENDIF()
