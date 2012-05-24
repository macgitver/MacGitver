
MACRO( HIC _outputvar )

    SET( _hics ${ARGN} )
    FOREACH( _hic ${_hics} )

        GET_FILENAME_COMPONENT(_abs_FILE ${_hic} ABSOLUTE)
        GET_FILENAME_COMPONENT(_basename ${_hic} NAME_WE)

        SET( _out1 ${CMAKE_CURRENT_BINARY_DIR}/hic_${_basename}.h )
        SET( _out2 ${CMAKE_CURRENT_BINARY_DIR}/hic_${_basename}.cpp )

        ADD_CUSTOM_COMMAND(
            OUTPUT			${_out1} ${_out2}
            COMMAND			hic
            ARGS			${_abs_FILE} ${_out1} ${_out2}
            MAIN_DEPENDENCY	${_abs_FILE}
            DEPENDS         hic
            COMMENT			"HIC'ing ${_basename}.hid"

        )

        LIST( APPEND ${_outputvar} ${_out1} ${_out2} )

        # we know there's nothing to moc inside
        SET_SOURCE_FILES_PROPERTIES(
            ${_out1}
            PROPERTIES  SKIP_AUTOMOC TRUE )

    ENDFOREACH()

ENDMACRO()
