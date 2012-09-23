
QT_PREPARE( Core -Gui )

SET( SRC_FILES
    main.cpp
    HIC.cpp
    HICObject.cpp
    HICProperty.cpp
    HIDLexer.cpp
)

SET( HDR_FILES
    HIC.h
    HICObject.h
    HICProperty.h
    HIDLexer.h
    HIDToken.h
)

ADD_QT_EXECUTABLE(
    hic

    ${SRC_FILES}
    ${HDR_FILES}

    HIDGrammar.md
)
