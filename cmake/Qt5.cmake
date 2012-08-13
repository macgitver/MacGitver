
FIND_PROGRAM( QMAKE_EXE qmake )

EXECUTE_PROCESS(
	COMMAND				${QMAKE_EXE} -query QT_INSTALL_PREFIX
	OUTPUT_VARIABLE		QT_INSTALL_PREFIX
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

SET( CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT_INSTALL_PREFIX} )

MACRO( QT_MOC SourcesVar )

	SET( Mocables ${ARGN} )
	QT5_GET_MOC_FLAGS(_moc_INCS)

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
				QT5_CREATE_MOC_COMMAND(${_abs_FILE} ${_moc} "${_moc_INCS}" "")
				
				LIST( APPEND ${SourcesVar} ${_moc} )
				# also, keep the generated moc from beeing scanned by auto-moc'ings
				SET_SOURCE_FILES_PROPERTIES(
					${_moc}
					PROPERTIES  SKIP_AUTOMOC TRUE )

			ENDIF(_match)
		ENDIF( NOT _skip AND EXISTS ${_abs_FILE} )
	ENDFOREACH(_current_FILE)
ENDMACRO()

MACRO( QT_UIC out )
	QT5_WRAP_UI( ${out} ${ARGN} )
ENDMACRO()

function(QT_RCC infiles outfiles )

	set(options)
	set(oneValueArgs)
	set(multiValueArgs OPTIONS)

	cmake_parse_arguments(_RCC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	set(rcc_files ${_RCC_UNPARSED_ARGUMENTS})
	set(rcc_options ${_RCC_OPTIONS})
	
	foreach(it ${rcc_files})
		get_filename_component(outfilename ${it} NAME_WE)
		get_filename_component(infile ${it} ABSOLUTE)
		get_filename_component(rc_path ${infile} PATH)
		set(outfile ${CMAKE_CURRENT_BINARY_DIR}/qrc_${outfilename}.cxx)

		set(_RC_DEPENDS)
		if(EXISTS "${infile}")
			#  parse file for dependencies
			#  all files are absolute paths or relative to the location of the qrc file
			file(READ "${infile}" _RC_FILE_CONTENTS)
			string(REGEX MATCHALL "<file[^<]+" _RC_FILES "${_RC_FILE_CONTENTS}")

			foreach(_RC_FILE ${_RC_FILES})
				string(REGEX REPLACE "^<file[^>]*>" "" _RC_FILE "${_RC_FILE}")
				if(NOT IS_ABSOLUTE "${_RC_FILE}")
					set(_RC_FILE "${rc_path}/${_RC_FILE}")
				endif()
				LIST( APPEND ${infiles} ${_RC_FILE} )
				set(_RC_DEPENDS ${_RC_DEPENDS} "${_RC_FILE}")
			endforeach()
			# Since this cmake macro is doing the dependency scanning for these files,
			# let's make a configured file and add it as a dependency so cmake is run
			# again when dependencies need to be recomputed.
			qt5_make_output_file("${infile}" "" "qrc.depends" out_depends)
			configure_file("${infile}" "${out_depends}" COPY_ONLY)
		else()
				# The .qrc file does not exist (yet). Let's add a dependency and hope
			# that it will be generated later
			set(out_depends)
		endif()

		add_custom_command(OUTPUT ${outfile}
			COMMAND ${Qt5Core_RCC_EXECUTABLE}
			ARGS ${rcc_options} -name ${outfilename} -o ${outfile} ${infile}
			MAIN_DEPENDENCY ${infile}
			DEPENDS ${_RC_DEPENDS} "${out_depends}" VERBATIM)
		list(APPEND ${outfiles} ${outfile})
	endforeach()

	set(${outfiles} ${${outfiles}} PARENT_SCOPE)

endfunction()

MACRO( QT_PREPARE )

	SET( QT_USED_MODULES "" )

	FOREACH( mod ${ARGN} )

		IF( NOT ${mod} STREQUAL "-Gui" )

			IF( "${Qt5${mod}_DIR}" STREQUAL "" )
				FIND_PACKAGE( Qt5${mod} REQUIRED )
			ENDIF()

			LIST( APPEND QT_USED_MODULES ${mod} )

			INCLUDE( ${Qt5${mod}_DIR}/Qt5${mod}Config.cmake )

		ENDIF()

	ENDFOREACH()

ENDMACRO()

MACRO( ADD_QT_LIBRARY _target )

	ADD_LIBRARY( ${_target} ${ARGN} )
	QT5_USE_MODULES( ${_target} ${QT_USED_MODULES} )
	_ADD_NOCASTS()

ENDMACRO( ADD_QT_LIBRARY )

MACRO( ADD_QT_EXECUTABLE _target )

	ADD_EXECUTABLE( ${_target} ${ARGN} )
	QT5_USE_MODULES( ${_target} ${QT_USED_MODULES} )
	_ADD_NOCASTS()

ENDMACRO( ADD_QT_EXECUTABLE )
