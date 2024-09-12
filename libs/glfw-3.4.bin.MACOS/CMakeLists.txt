#
#add_library(glfw3 STATIC lib-universal/libglfw3.a )
#
#set_target_properties(glfw3 PROPERTIES LINKER_LANGUAGE C)
#
#target_include_directories(glfw3
#        PUBLIC
#        ./include
#        )



ADD_LIBRARY(glfw3 STATIC IMPORTED)
SET_TARGET_PROPERTIES(glfw3 PROPERTIES
        IMPORTED_LOCATION lib-universal/libglfw3.a)
#TARGET_LINK_LIBRARIES(mytarget A boost_unit_test_framework C)

target_include_directories(glfw3
        INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
        )
