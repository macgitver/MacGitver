
FIND_PROGRAM( QMAKE_EXE qmake )

EXECUTE_PROCESS(
    COMMAND				${QMAKE_EXE} -query QT_INSTALL_PREFIX
    OUTPUT_VARIABLE		QT_INSTALL_PREFIX
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

SET( CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT_INSTALL_PREFIX} )

MACRO(QT_MOC SourcesVar )

    SET( Mocables ${ARGN} )
    SET( _Force 0 )
    SET( _mocfiles )
    QT5_GET_MOC_FLAGS(_moc_INCS)

    FOREACH(_current_FILE ${Mocables})

        IF( _current_FILE STREQUAL "FORCE" )

            SET( _Force 1 )

        ELSEIF( _Force )

            GET_FILENAME_COMPONENT(_abs_FILE ${_current_FILE} ABSOLUTE)
            GET_FILENAME_COMPONENT(_basename ${_current_FILE} NAME_WE)
            SET(_moc    ${CMAKE_CURRENT_BINARY_DIR}/moc_${_basename}.cpp)
            QT4_CREATE_MOC_COMMAND(${_abs_FILE} ${_moc} "${_moc_INCS}" "")

            LIST( APPEND ${SourcesVar} ${_moc} )
            # also, keep the generated moc from beeing scanned by auto-moc'ings
            SET_SOURCE_FILES_PROPERTIES(
                ${_moc}
                PROPERTIES  SKIP_AUTOMOC TRUE )

        ELSE()

            GET_FILENAME_COMPONENT(_abs_FILE ${_current_FILE} ABSOLUTE)
            GET_SOURCE_FILE_PROPERTY(_skip ${_abs_FILE} SKIP_AUTOMOC)

            IF( NOT _skip AND EXISTS ${_abs_FILE} )

                FILE(READ ${_abs_FILE} _contents)

                STRING(REGEX MATCHALL "Q_OBJECT" _match "${_contents}")
                IF(_match)

                    GET_FILENAME_COMPONENT(_basename ${_current_FILE} NAME_WE)
                    SET(_moc ${CMAKE_CURRENT_BINARY_DIR}/moc_${_basename}.cpp.moc)
                    QT5_CREATE_MOC_COMMAND(${_abs_FILE} ${_moc} "${_moc_INCS}" "")

                    LIST( APPEND _mocfiles ${_moc} )
                    # also, keep the generated moc from beeing scanned by auto-moc'ings
                    SET_SOURCE_FILES_PROPERTIES(
                        ${_moc}
                        PROPERTIES  SKIP_AUTOMOC TRUE )

                ENDIF()

            ENDIF()

        ENDIF()

    ENDFOREACH()

    SET( _mocer ${CMAKE_CURRENT_BINARY_DIR}/MocFiles.cpp )
    FILE( WRITE ${_mocer} "// MOC files for ${CMAKE_CURRENT_PROJECT_NAME}\n\n" )

    FOREACH( _current_FILE ${_mocfiles} )
        FILE( APPEND ${_mocer} "#include \"${_current_FILE}\"\n" )
    ENDFOREACH()

    LIST( APPEND ${SourcesVar} ${_mocfiles} ${_mocer} )

ENDMACRO(QT_MOC)

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

        IF( ${mod} STREQUAL "Main" )

            # Don't do anything

        ELSEIF( NOT ${mod} STREQUAL "-Gui" )

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
