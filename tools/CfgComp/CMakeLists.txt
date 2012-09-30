
PROJECT( CfgComp )

QT_PREPARE( Core -Gui Xml )

SET( SRC_FILES
	CfgComp.cpp
	Model.cpp
	VariantTypes.cpp
)

SET( HDR_FILES
	Model.hpp
	VariantTypes.hpp
)

ADD_QT_EXECUTABLE(
	CfgComp

	${SRC_FILES}
	${HDR_FILES}
)
