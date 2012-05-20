
PROJECT( MGV_HEAVEN )

INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES(
    ${MGV_LIBS_SOURCE_DIR}
    ${MGV_HEAVEN_SOURCE_DIR}
    ${MGV_HEAVEN_BINARY_DIR}
)

SET( SRC_FILES

    Container.cpp
    View.cpp
    TopLevelWidget.cpp
    Decorator.cpp

)

SET( PUB_HDR_FILES

    HeavenApi.h
    Heaven.h
    Container.h
    View.h
    TopLevelWidget.h
)

SET( PRI_HDR_FILES

    Decorator.h
)

SET( HDR_FILES ${PRI_HDR_FILES} ${PUB_HDR_FILES} )

QT4_MOC( MOC_FILES ${HDR_FILES} )

ADD_LIBRARY(
    Heaven SHARED

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
)

TARGET_LINK_LIBRARIES(
    Heaven

    ${QT_LIBRARIES}
)
