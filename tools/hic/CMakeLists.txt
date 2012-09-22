
QT_PREPARE( Core -Gui )

SET( SRC_FILES
	hic.cpp
)

SET( HDR_FILES
	hic.h
)

ADD_QT_EXECUTABLE(
	hic

	${SRC_FILES}
	${HDR_FILES}

	HIDGrammar.md
)
