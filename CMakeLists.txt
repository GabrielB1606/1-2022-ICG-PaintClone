cmake_minimum_required(VERSION 3.0.0)
project(Paint VERSION 0.1.0)

set(CMAKE_SUPPRESS_REGENERATION true)

add_compile_definitions(FREEGLUT_STATIC)

# all imgui .cpp files
file( GLOB IMGUI_SRC src/imgui/*.cpp )

add_executable(${PROJECT_NAME} src/main.cpp ${IMGUI_SRC})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME} PUBLIC include/ include/imgui )
target_link_directories( ${PROJECT_NAME} PRIVATE lib/ )
target_link_libraries(${PROJECT_NAME} freeglut_staticd)

# imgui demo
add_executable(imgui_demo src/imgui_main.cpp ${IMGUI_SRC})
target_include_directories( imgui_demo PUBLIC include/ include/imgui )
target_link_directories( imgui_demo PRIVATE lib/ )
target_link_libraries(imgui_demo freeglut_staticd)
