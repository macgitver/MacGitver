
LIST_SUBDIRS(subdirs ${CMAKE_CURRENT_LIST_DIR})

FOREACH(subdir ${subdirs})
    IF(EXISTS ${subdir}/CMakeLists.txt)
        GET_FILENAME_COMPONENT(basename ${subdir} NAME_WE)
        MESSAGE(STATUS "Including addon ${basename}")
        ADD_SUBDIRECTORY(${subdir})
    ENDIF()
ENDFOREACH()
