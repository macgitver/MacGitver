
# Always require the lastest and greatest of Qt
FIND_PACKAGE( Qt4 4.8.5 REQUIRED )

MACRO(QT_MOC SourcesVar )

    SET( Mocables ${ARGN} )
    SET( _Force 0 )
    SET( _mocfiles )

    FOREACH(_current_FILE ${Mocables})
        IF( _current_FILE STREQUAL "FORCE" )
            SET( _Force 1 )

        ELSEIF( _Force )
            GET_FILENAME_COMPONENT(_abs_FILE ${_current_FILE} ABSOLUTE)
            GET_FILENAME_COMPONENT(_basename ${_current_FILE} NAME_WE)
            SET(_moc    ${CMAKE_CURRENT_BINARY_DIR}/moc_${_basename}.cpp)
            QT4_GENERATE_MOC(${_abs_FILE} ${_moc})

            LIST( APPEND ${SourcesVar} ${_moc} )

        ELSE()
            GET_FILENAME_COMPONENT(_abs_FILE ${_current_FILE} ABSOLUTE)
            GET_SOURCE_FILE_PROPERTY(_skip ${_abs_FILE} SKIP_AUTOMOC)

            IF( NOT _skip AND EXISTS ${_abs_FILE} )
                FILE(READ ${_abs_FILE} _contents)
                STRING(REGEX MATCHALL "Q_OBJECT" _match "${_contents}")

                IF(_match)
                    GET_FILENAME_COMPONENT(_basename ${_current_FILE} NAME_WE)
                    SET(_moc ${CMAKE_CURRENT_BINARY_DIR}/moc_${_basename}.cpp)
                    QT4_GENERATE_MOC(${_abs_FILE} ${_moc})

                    LIST( APPEND _mocfiles ${_moc} )
                ENDIF()
            ENDIF()
        ENDIF()
    ENDFOREACH()
    LIST( APPEND ${SourcesVar} ${_mocfiles} )
ENDMACRO(QT_MOC)

MACRO(QT_UIC outfiles )
    QT4_WRAP_UI(${outfiles} ${ARGN})
ENDMACRO( QT_UIC )


MACRO(QT_RCC infiles outfiles )
    IF(CMAKE_VERSION VERSION_LESS 2.8.12)

        # For CMake < 2.8.12 we're using our modified version of QT4_ADD_RESOURCES, which is able
        # to return a list of the resource files included in the .qrc file and thus enables us to
        # show these files in QtCreator or Visual Studio.

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
        ENDFOREACH(it)

    ELSE()
        # For CMake >= 2.8.12, use the stock macro from CMake. This will of course leave the infiles
        # variable empty and resources will not show up in MSVC or QtCreator.

        QT4_ADD_RESOURCES(${outfiles} ${ARGN})
    ENDIF()

ENDMACRO(QT_RCC)

MACRO( QT_PREPARE )

    FOREACH( use ${ARGN} )

        IF( ${use} STREQUAL "-Gui" )
            SET( QT_DONT_USE_QTGUI 1 )
        ELSEIF( ${use} STREQUAL "Widgets" )
            # Dont do anything for Widgets
        ELSEIF( ${use} STREQUAL "WebKitWidgets" )
            # Dont do anything for WebKitWidgets
        ELSEIF( ${use} STREQUAL "Core" )
            # Dont do anything for Core
        ELSEIF( ${use} STREQUAL "WinMain" )
            SET(QT_USE_QTMAIN 1)
        ELSE()
            STRING( TOUPPER ${use} useUpper )
            SET( QT_USE_QT${useUpper} 1 )
        ENDIF()
    ENDFOREACH()

    INCLUDE( ${QT_USE_FILE} )

ENDMACRO()

MACRO(ADVANCE_TO_QT_PROJECT _target)
    TARGET_LINK_LIBRARIES( ${_target} LINK_PRIVATE ${QT_LIBRARIES} )
    _ADD_NOCASTS()
ENDMACRO()
