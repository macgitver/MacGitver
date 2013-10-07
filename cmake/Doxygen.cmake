
FIND_PACKAGE(Doxygen)

FUNCTION( DOXYGEN_LOCATE_FILE var name1 name2 )
    SET( dirs ${ARGN} )

    FOREACH( dir ${dirs} )
        IF( EXISTS ${dir}/${name1} )
            SET( ${var} ${dir}/${name1} PARENT_SCOPE )
            BREAK()
        ENDIF()
        IF( EXISTS ${dir}/${name2} )
            SET( ${var} ${dir}/${name2} PARENT_SCOPE )
            BREAK()
        ENDIF()
    ENDFOREACH()

ENDFUNCTION()

FUNCTION( ADD_DOXYGEN_TARGET targetName configDir )

    IF(DOXYGEN_FOUND)

        SET( dirs ${ARGN} )
        SET( doxyBase ${CMAKE_SOURCE_DIR}/doxygen )

        SET( targetDir ${CMAKE_BINARY_DIR}/docs/${targetName} )

        FILE( MAKE_DIRECTORY ${targetDir}/int ${targetDir}/pub )

        DOXYGEN_LOCATE_FILE( pubLayout public.layout.xml layout.xml
                ${configDir}
                ${doxyBase}
        )
        DOXYGEN_LOCATE_FILE( pubHeader public.header.htm header.htm
                ${configDir}
                ${doxyBase}
        )
        DOXYGEN_LOCATE_FILE( pubFooter public.footer.htm footer.htm
                ${configDir}
                ${doxyBase}
        )
        DOXYGEN_LOCATE_FILE( pubStyle public.style.css style.css
                ${configDir}
                ${doxyBase}
        )


        DOXYGEN_LOCATE_FILE( intLayout internal.layout.xml layout.xml
                ${configDir}
                ${doxyBase}
        )
        DOXYGEN_LOCATE_FILE( intHeader internal.header.htm header.htm
                ${configDir}
                ${doxyBase}
        )
        DOXYGEN_LOCATE_FILE( intFooter internal.footer.htm footer.htm
                ${configDir}
                ${doxyBase}
        )
        DOXYGEN_LOCATE_FILE( intStyle internal.style.css style.css
                ${configDir}
                ${doxyBase}
        )

        FILE( WRITE ${targetDir}/Doxygen.common
            "@INCLUDE = ${doxyBase}/Doxygen.common\n"
            "\n"
        )

        SET( exclude 0 )
        FOREACH( dir ${dirs} )
            IF( dir STREQUAL "EXCLUDE" )
                SET( exclude 1 )
            ELSE()
                IF( exclude )
                    FILE( APPEND ${targetDir}/Doxygen.common
                        "EXCLUDE += ${dir}\n"
                    )
                    SET( exclude 0 )
                ELSE()
                    FILE( APPEND ${targetDir}/Doxygen.common
                        "INPUT += ${dir}\n"
                    )
                ENDIF()
            ENDIF()
        ENDFOREACH()

        FILE( WRITE ${targetDir}/Doxygen.pub
            "@INCLUDE         = ${targetDir}/Doxygen.common\n"
            "@INCLUDE         = ${doxyBase}/Doxygen.pub\n"
            "\n"
            "HTML_HEADER      = ${pubHeader}\n"
            "HTML_FOOTER      = ${pubFooter}\n"
            "HTML_STYLESHEET  = ${pubStyle}\n"
            "LAYOUT_FILE      = ${pubLayout}\n"
            "\n"
            "OUTPUT_DIRECTORY = ${targetDir}/pub\n"
            "\n"
        )

        FILE( WRITE ${targetDir}/Doxygen.int
            "@INCLUDE         = ${targetDir}/Doxygen.common\n"
            "@INCLUDE         = ${doxyBase}/Doxygen.int\n"
            "\n"
            "HTML_HEADER      = ${intHeader}\n"
            "HTML_FOOTER      = ${intFooter}\n"
            "HTML_STYLESHEET  = ${intStyle}\n"
            "LAYOUT_FILE      = ${intLayout}\n"
            "\n"
            "OUTPUT_DIRECTORY = ${targetDir}/int\n"
            "\n"
        )

        IF( EXISTS ${configDir}/Doxygen.common )
            FILE( APPEND ${targetDir}/Doxygen.common
                "@INCLUDE = ${configDir}/Doxygen.common\n"
                "\n"
            )
        ENDIF()

        IF( EXISTS ${configDir}/Doxygen.pub )
            FILE( APPEND ${targetDir}/Doxygen.pub
                "@INCLUDE = ${configDir}/Doxygen.pub\n"
                "\n"
            )
        ENDIF()

        IF( EXISTS ${configDir}/Doxygen.int )
            FILE( APPEND ${targetDir}/Doxygen.int
                "@INCLUDE = ${configDir}/Doxygen.int\n"
                "\n"
            )
        ENDIF()

        ADD_CUSTOM_TARGET(
            ${targetName}_intdocs
            ${DOXYGEN_EXECUTABLE} Doxygen.int
            WORKING_DIRECTORY           ${targetDir}
            COMMENT                     "Building internal ${targetName} documentation"
        )

        ADD_CUSTOM_TARGET(
            ${targetName}_pubdocs
            ${DOXYGEN_EXECUTABLE} Doxygen.pub
            WORKING_DIRECTORY           ${targetDir}
            COMMENT                     "Building public ${targetName} documentation"
        )

        ADD_CUSTOM_TARGET(
            ${targetName}_docs
        )

        ADD_DEPENDENCIES(
            ${targetName}_docs

            ${targetName}_pubdocs
            ${targetName}_intdocs
        )

        ADD_DEPENDENCIES( internal_docs ${targetName}_intdocs )
        ADD_DEPENDENCIES( public_docs ${targetName}_pubdocs )

    ENDIF()

ENDFUNCTION()

IF(DOXYGEN_FOUND)
    ADD_CUSTOM_TARGET( docs )
    ADD_CUSTOM_TARGET( internal_docs )
    ADD_CUSTOM_TARGET( public_docs )

    ADD_DEPENDENCIES( docs internal_docs public_docs )
ENDIF()
