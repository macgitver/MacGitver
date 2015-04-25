
PROJECT(LOGGER)

QT_PREPARE(Core)

INCLUDE_DIRECTORIES( BEFORE
    ${MGV_CORE_SOURCE_DIR}
    ${MGV_CORE_BINARY_DIR}
)

SET( SRC_FILES

    Event.cpp
    Channel.cpp
    Template.cpp
    Manager.cpp
    Consumer.cpp
    Internal.cpp
)

SET( PUB_HDR_FILES

    Api.hpp
    Event.hpp
    Channel.hpp
    Template.hpp
    Manager.hpp
    Consumer.hpp

)

SET( PRI_HDR_FILES
    Internal.hpp
)

SET( HDR_FILES ${PRI_HDR_FILES} ${PUB_HDR_FILES} )

QT_MOC( MOC_FILES ${HDR_FILES} )

ADD_QT_LIBRARY(
    Logger SHARED

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
)

TARGET_LINK_LIBRARIES(
    Logger

    LINK_PUBLIC
        HeavenIcons
)

IF(UNIX AND NOT APPLE)
    SET_TARGET_PROPERTIES(
        Logger
        PROPERTIES  INSTALL_RPATH
        "\$ORIGIN"
    )
ENDIF()

RAD_DEFINE_VERSION(LOGGER 0 0 1)
RAD_SET_TARGET_VERSION(Logger LOGGER)
RAD_INSTALL_LIBRARY    (Logger Logger)
RAD_INSTALL_HEADERS    (libLogger ${PUB_HDR_FILES})
RAD_SPLIT_SOURCE_TREE  (libLogger)
