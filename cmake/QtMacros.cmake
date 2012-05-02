
MACRO(QT4_MOC_FORCE SourcesVar )

	SET( Mocables ${ARGN} )
	QT4_GET_MOC_FLAGS(_moc_INCS)

	SET(_matching_FILES )
	FOREACH (_current_FILE ${Mocables})

		GET_FILENAME_COMPONENT(_abs_FILE ${_current_FILE} ABSOLUTE)
		# if "SKIP_AUTOMOC" is set to true, we will not handle this file here.
		# This is required to make uic work correctly:
		# we need to add generated .cpp files to the sources (to compile them),
		# but we cannot let automoc handle them, as the .cpp files don't exist yet when
		# cmake is run for the very first time on them -> however the .cpp files might
		# exist at a later run. at that time we need to skip them, so that we don't add two
		# different rules for the same moc file
		GET_SOURCE_FILE_PROPERTY(_skip ${_abs_FILE} SKIP_AUTOMOC)

		IF ( NOT _skip )

			GET_FILENAME_COMPONENT(_basename ${_current_FILE} NAME_WE)
			SET(_moc    ${CMAKE_CURRENT_BINARY_DIR}/moc_${_basename}.cpp)
			QT4_CREATE_MOC_COMMAND(${_abs_FILE} ${_moc} "${_moc_INCS}" "")

			LIST( APPEND ${SourcesVar} ${_moc} )
			# also, keep the generated moc from beeing scanned by auto-moc'ings
			SET_SOURCE_FILES_PROPERTIES(
				${_moc}
				PROPERTIES  SKIP_AUTOMOC TRUE )

		ENDIF ( NOT _skip )

	ENDFOREACH (_current_FILE)

ENDMACRO(QT4_MOC_FORCE)

MACRO(QT4_MOC SourcesVar )

  SET( Mocables ${ARGN} )
  QT4_GET_MOC_FLAGS(_moc_INCS)

  SET(_matching_FILES )
  FOREACH (_current_FILE ${Mocables})

	GET_FILENAME_COMPONENT(_abs_FILE ${_current_FILE} ABSOLUTE)
	# if "SKIP_AUTOMOC" is set to true, we will not handle this file here.
	# This is required to make uic work correctly:
	# we need to add generated .cpp files to the sources (to compile them),
	# but we cannot let automoc handle them, as the .cpp files don't exist yet when
	# cmake is run for the very first time on them -> however the .cpp files might
	# exist at a later run. at that time we need to skip them, so that we don't add two
	# different rules for the same moc file
	GET_SOURCE_FILE_PROPERTY(_skip ${_abs_FILE} SKIP_AUTOMOC)

	IF ( NOT _skip AND EXISTS ${_abs_FILE} )

	  FILE(READ ${_abs_FILE} _contents)

	  STRING(REGEX MATCHALL "Q_OBJECT" _match "${_contents}")
	  IF(_match)

			GET_FILENAME_COMPONENT(_basename ${_current_FILE} NAME_WE)
			SET(_moc    ${CMAKE_CURRENT_BINARY_DIR}/moc_${_basename}.cpp)
			QT4_CREATE_MOC_COMMAND(${_abs_FILE} ${_moc} "${_moc_INCS}" "")

			LIST( APPEND ${SourcesVar} ${_moc} )
			# also, keep the generated moc from beeing scanned by auto-moc'ings
			SET_SOURCE_FILES_PROPERTIES(
				${_moc}
				PROPERTIES  SKIP_AUTOMOC TRUE )

	  ENDIF(_match)
	ENDIF ( NOT _skip AND EXISTS ${_abs_FILE} )
  ENDFOREACH (_current_FILE)
ENDMACRO(QT4_MOC)

MACRO (QT4_UIC outfiles )
	QT4_EXTRACT_OPTIONS(ui_files ui_options ${ARGN})

	FOREACH (it ${ui_files})

		GET_FILENAME_COMPONENT( itbase  ${it} NAME     )
		GET_FILENAME_COMPONENT( outfile ${it} NAME_WE  )
		GET_FILENAME_COMPONENT( infile  ${it} ABSOLUTE )

		SET( outfile ${CMAKE_CURRENT_BINARY_DIR}/ui_${outfile}.h )
		ADD_CUSTOM_COMMAND(
			OUTPUT			${outfile}
			COMMAND			${QT_UIC_EXECUTABLE}
			ARGS			${ui_options} -o ${outfile} ${infile}
			MAIN_DEPENDENCY	${infile}
			COMMENT			"UIC'ing ${itbase}"
		)
		SET( ${outfiles} ${${outfiles}} ${outfile} )
	ENDFOREACH( it )

ENDMACRO( QT4_UIC )
