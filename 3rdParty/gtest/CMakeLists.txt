
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/3rdParty)

ADD_LIBRARY( gtest STATIC gtest-all.cc gtest.h )
ADD_LIBRARY( gtest_main STATIC gtest_main.cc )

QT_PREPARE(Core -Gui)
ADD_LIBRARY( gtest_qt_main STATIC gtest_qt_main.cpp )
ADVANCE_TO_QT_PROJECT(gtest_qt_main)

TARGET_LINK_LIBRARIES(gtest_main gtest)
TARGET_LINK_LIBRARIES(gtest_qt_main gtest)
