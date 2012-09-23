
QT_PREPARE( Core -Gui )

SET( SRC_FILES
    main.cpp
    HICObject.cpp
    HICProperty.cpp
    HIDLexer.cpp
    HIDParser.cpp
    HIDToken.cpp
    HIGeneratorBase.cpp
    HIGenHeader.cpp
    HIGenSource.cpp
)

SET( HDR_FILES
    HICObject.h
    HICProperty.h
    HIDLexer.h
    HIDToken.h
    HIDParser.h
    HIGeneratorBase.h
    HIGenHeader.h
    HIGenSource.h
)

ADD_QT_EXECUTABLE(
    hic

    ${SRC_FILES}
    ${HDR_FILES}

    # Not compilable, but shall show up in IDE
    HIDGrammar.md
)
