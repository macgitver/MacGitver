
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/3rdParty)
ADD_LIBRARY( gtest STATIC gtest-all.cc gtest_main.cc gtest.h )
