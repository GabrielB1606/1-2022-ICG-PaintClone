cmake_minimum_required(VERSION 3.0.0)
project(Paint VERSION 0.1.0)

set(CMAKE_SUPPRESS_REGENERATION true)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

add_compile_definitions(FREEGLUT_STATIC)

# all imgui .cpp files
file( GLOB IMGUI_SRC src/imgui/*.cpp )

# my project
add_executable(${PROJECT_NAME} src/main.cpp ${IMGUI_SRC} src/tinyfiledialogs/tinyfiledialogs.c)
target_include_directories( ${PROJECT_NAME} PUBLIC include/ include/imgui include/tinyfiledialogs)
target_link_directories( ${PROJECT_NAME} PRIVATE lib/ )
target_link_libraries(${PROJECT_NAME} freeglut_static comdlg32 ole32)

# my project debug
add_executable(PaintDebug src/main.cpp ${IMGUI_SRC} src/tinyfiledialogs/tinyfiledialogs.c)
target_include_directories( PaintDebug PUBLIC include/ include/imgui include/tinyfiledialogs)
target_link_directories(PaintDebug PRIVATE lib/ )
target_link_libraries(PaintDebug freeglut_staticd comdlg32 ole32)

# imgui demo
# add_executable(imgui_demo src/demo/imgui_main.cpp ${IMGUI_SRC})
# target_include_directories( imgui_demo PUBLIC include/ include/imgui )
# target_link_directories( imgui_demo PRIVATE lib/ )
# target_link_libraries(imgui_demo freeglut_staticd)

# # tinyfiledialogs demo
# add_executable(tinyfiledialogs_demo src/demo_tfd/hello.c src/tinyfiledialogs/tinyfiledialogs.c)
# target_include_directories(tinyfiledialogs_demo PUBLIC include/tinyfiledialogs)
# target_link_libraries(tinyfiledialogs_demo comdlg32 ole32)
