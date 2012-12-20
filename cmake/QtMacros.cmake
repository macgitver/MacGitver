
OPTION( Qt5 "Use Qt5" OFF )

# Common stuff:

MACRO( _ADD_NOCASTS )
	ADD_DEFINITIONS(
		-DQT_NO_CAST_FROM_ASCII
		-DQT_NO_CAST_TO_ASCII
		-DQT_NO_CAST_FROM_BYTEARRAY
	)
ENDMACRO()

# Paths are relative to Top-Src-Dir. Duh.
IF( Qt5 )
	INCLUDE( cmake/Qt5.cmake )
ELSE()
	INCLUDE( cmake/Qt4.cmake )
ENDIF()
