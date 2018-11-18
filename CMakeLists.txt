cmake_minimum_required(VERSION 3.6)

project(CoolEngine)

set(LIB_DIR ${CMAKE_SOURCE_DIR}/libs)

file(GLOB_RECURSE CORE_FILES
	./src/*.cpp
	./src/*.hpp)
file(GLOB_RECURSE OPENGL_FILES
	./src/opengl/*.cpp
	./src/opengl/*.hpp)
file(GLOB_RECURSE OPENAL_FILES
	./src/openal/*.cpp
	./src/openal/*.hpp)
add_executable(game ${CORE_FILES} ${OPENGL_FILES} ${OPENAL_FILES})#gl.h gl.cpp main.cpp renderer.h renderable.h)

source_group("Core Files" FILES ${CORE_FILES})
source_group("OpenGL Files" FILES ${OPENGL_FILES})
source_group("OpenAL Files" FILES ${OPENAL_FILES})

#target_sources(game )
set_property(TARGET game PROPERTY CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
set(CMAKE_SYSTEM_FRAMEWORK_PATH ${CMAKE_SYSTEM_FRAMEWORK_PATH}, "C:/Program Files (x86)/OpenAL 1.1 SDK")
find_package(OpenAL REQUIRED)

include_directories(libs/include/ src/ ${OPENGL_INCLUDE_DIR} ${OPENAL_INCLUDE_DIR})

target_link_libraries(game ${LIB_DIR}/glfw3.lib)
target_link_libraries(game ${LIB_DIR}/glew32s.lib)
target_link_libraries(game ${OPENGL_LIBRARIES})
target_link_libraries(game ${OPENAL_LIBRARY})
target_link_libraries(game ${LIB_DIR}/freetype.lib)
				
