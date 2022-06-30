cmake_minimum_required(VERSION 3.0.0)
project(Paint VERSION 0.1.0)

set(CMAKE_SUPPRESS_REGENERATION true)

add_compile_definitions(FREEGLUT_STATIC)

add_executable(Paint src/main.cpp src/imgui/imgui.cpp src/imgui/imgui_impl_glut.cpp src/imgui/imgui_impl_opengl2.cpp src/imgui/imgui_demo.cpp src/imgui/imgui_draw.cpp src/imgui/imgui_tables.cpp src/imgui/imgui_widgets.cpp)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME} PUBLIC include/ include/imgui )
target_link_directories( ${PROJECT_NAME} PRIVATE lib/ )
target_link_libraries(${PROJECT_NAME} freeglut_staticd)