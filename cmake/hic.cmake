
MACRO( HIC _outputvar )

    SET( _hics ${ARGN} )
    FOREACH( _hic ${_hics} )

        GET_FILENAME_COMPONENT(_abs_FILE ${_hic} ABSOLUTE)
        GET_FILENAME_COMPONENT(_basename ${_hic} NAME_WE)

        SET( _out ${CMAKE_CURRENT_BINARY_DIR}/hic_${_basename}.h )

        ADD_CUSTOM_COMMAND(
            OUTPUT			${_out}
            COMMAND			hic
            ARGS			${_abs_FILE} ${_out}
            MAIN_DEPENDENCY	${_abs_FILE}
            COMMENT			"HIC'ing ${_basename}"

        )

        LIST( APPEND ${_outputvar} ${_out} )

        # we know there's nothing to moc inside
        SET_SOURCE_FILES_PROPERTIES(
            ${_out}
            PROPERTIES  SKIP_AUTOMOC TRUE )

    ENDFOREACH()

ENDMACRO()
