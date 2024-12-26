cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(OpenGLTemplate CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/INCLUDE/GLM/DETAIL/glm.cpp ${CMAKE_CURRENT_SOURCE_DIR}/SRC/* ${CMAKE_CURRENT_SOURCE_DIR}/source/**)
list(FILTER sources EXCLUDE REGEX ".*\\.obj$")

add_executable(opengl-template ${sources} source/Plane.cpp source/Plane.h)

find_package(OpenGL REQUIRED)
if (OpenGL_FOUND)
    target_include_directories(opengl-template PRIVATE ${OPENGL_INCLDUE_DIRS})
    target_link_libraries(opengl-template PRIVATE ${OPENGL_LIBRARIES})
else()
    message(FATAL_ERROR "OpenGL was not found on the system")
endif()

target_include_directories(opengl-template PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/INCLUDE)
target_link_libraries(opengl-template PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/LIB/x64/GL/freeglut.lib ${CMAKE_CURRENT_SOURCE_DIR}/LIB/x64/GL/glew32.lib ${CMAKE_CURRENT_SOURCE_DIR}/LIB/x64/GL/glew32s.lib ${CMAKE_CURRENT_SOURCE_DIR}/LIB/x64/SOIL/SOIL.lib)

file(GLOB resources ${CMAKE_CURRENT_SOURCE_DIR}/source/resources/*)
foreach(resource ${resources})
    get_filename_component(resource_name ${resource} NAME)
    add_custom_command(TARGET opengl-template POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${resource}
        $<TARGET_FILE_DIR:opengl-template>/${resource_name})
endforeach()

add_custom_command(TARGET opengl-template POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/LIB/x64/GL/DLLs
    $<TARGET_FILE_DIR:opengl-template>)
add_custom_command(TARGET opengl-template POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/LIB/x64/GLFW/DLLs
    $<TARGET_FILE_DIR:opengl-template>)
add_custom_command(TARGET opengl-template POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/LIB/x64/SOIL
    $<TARGET_FILE_DIR:opengl-template>)

# Enable C++17
set_target_properties(opengl-template PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    FOLDER ${PROJECT_NAME})