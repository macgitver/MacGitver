
MACRO( CFGCOMP _outputvar )

	SET( _cfgs ${ARGN} )
	FOREACH( _cfg ${_cfgs} )

		GET_FILENAME_COMPONENT(_abs_FILE ${_cfg} ABSOLUTE)
		GET_FILENAME_COMPONENT(_basename ${_cfg} NAME_WE)

		SET( _out1 ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.hpp )
		SET( _out2 ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.cpp )

		ADD_CUSTOM_COMMAND(
			OUTPUT			${_out1} ${_out2}
			COMMAND			CfgComp
			ARGS			${_abs_FILE} ${CMAKE_CURRENT_BINARY_DIR}
			MAIN_DEPENDENCY	${_abs_FILE}
			DEPENDS         hic
			COMMENT			"CFG'ing ${_basename}.ccfg"

		)

		LIST( APPEND ${_outputvar} ${_out1} ${_out2} )

		# we know there's nothing to moc inside
		SET_SOURCE_FILES_PROPERTIES(
			${_out1}
			PROPERTIES  SKIP_AUTOMOC TRUE )

	ENDFOREACH()

ENDMACRO()
