
# Always require the lastest and greatest of Qt
FIND_PACKAGE( Qt4 4.8.2 REQUIRED )

MACRO(QT_MOC SourcesVar )

    SET( Mocables ${ARGN} )
    SET( _Force 0 )
    SET( _mocfiles )
    QT4_GET_MOC_FLAGS(_moc_INCS)

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
                    QT4_CREATE_MOC_COMMAND(${_abs_FILE} ${_moc} "${_moc_INCS}" "")

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

MACRO (QT_UIC outfiles )

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

ENDMACRO( QT_UIC )


MACRO(QT_RCC infiles outfiles )

    QT4_EXTRACT_OPTIONS(rcc_files rcc_options ${ARGN})

    FOREACH (it ${rcc_files})
        GET_FILENAME_COMPONENT(outfilename ${it} NAME_WE)
        GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
        GET_FILENAME_COMPONENT(rc_path ${infile} PATH)
        SET(outfile ${CMAKE_CURRENT_BINARY_DIR}/qrc_${outfilename}.cxx)

        FILE(READ "${infile}" _RC_FILE_CONTENTS)
        STRING(REGEX MATCHALL "<file[^<]+" _RC_FILES "${_RC_FILE_CONTENTS}")
        SET(_RC_DEPENDS)
        FOREACH(_RC_FILE ${_RC_FILES})
            STRING(REGEX REPLACE "^<file[^>]*>" "" _RC_FILE "${_RC_FILE}")
            IF(NOT IS_ABSOLUTE "${_RC_FILE}")
                SET(_RC_FILE "${rc_path}/${_RC_FILE}")
            ENDIF(NOT IS_ABSOLUTE "${_RC_FILE}")
            LIST( APPEND ${infiles} ${_RC_FILE} )
            SET(_RC_DEPENDS ${_RC_DEPENDS} "${_RC_FILE}")
        ENDFOREACH(_RC_FILE)

        QT4_MAKE_OUTPUT_FILE("${infile}" "" "qrc.depends" out_depends)
        CONFIGURE_FILE("${infile}" "${out_depends}" COPY_ONLY)

        ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
            COMMAND ${QT_RCC_EXECUTABLE}
            ARGS ${rcc_options} -name ${outfilename} -o ${outfile} ${infile}
            MAIN_DEPENDENCY ${infile}
            DEPENDS ${_RC_DEPENDS} "${out_depends}" VERBATIM)

        SET(${outfiles} ${${outfiles}} ${outfile})

    ENDFOREACH (it)

ENDMACRO(QT_RCC)

MACRO( QT_PREPARE )

    FOREACH( use ${ARGN} )

        IF( ${use} STREQUAL "-Gui" )
        SET( QT_DONT_USE_QTGUI 1 )
        ELSEIF( ${use} STREQUAL "Widgets" )
            # Dont do anything for Widgets
        ELSEIF( ${use} STREQUAL "Core" )
            # Dont do anything for Core
        ELSE()
            STRING( TOUPPER ${use} useUpper )
            SET( QT_USE_QT${useUpper} 1 )
        ENDIF()
    ENDFOREACH()

    INCLUDE( ${QT_USE_FILE} )

ENDMACRO()

MACRO( ADD_QT_LIBRARY _target )

    ADD_LIBRARY( ${_target} ${ARGN} )
    TARGET_LINK_LIBRARIES( ${_target} ${QT_LIBRARIES} )
    _ADD_NOCASTS()

ENDMACRO( ADD_QT_LIBRARY )

MACRO( ADD_QT_EXECUTABLE _target )

    ADD_EXECUTABLE( ${_target} ${ARGN} )
    TARGET_LINK_LIBRARIES( ${_target} ${QT_LIBRARIES} )
    _ADD_NOCASTS()

ENDMACRO( ADD_QT_EXECUTABLE )
