cmake_minimum_required(VERSION 3.6)

project(CoolEngine)

set(LIB_DIR ${CMAKE_SOURCE_DIR}/libs)

# cool engine lib
file(GLOB_RECURSE CORE_HPP
	./src/*.hpp)
file(GLOB_RECURSE CORE_CPP
	./src/*.cpp)
file(GLOB_RECURSE OPENGL_HPP
	./src/opengl/*.hpp)
file(GLOB_RECURSE OPENGL_CPP
	./src/opengl/*.cpp)
file(GLOB_RECURSE OPENAL_HPP
	./src/openal/*.hpp)
file(GLOB_RECURSE OPENAL_CPP
	./src/openal/*.cpp)
	
add_library(cool STATIC 
	${CORE_HPP} ${CORE_CPP} ${OPENGL_HPP} ${OPENGL_CPP} ${OPENAL_HPP} ${OPENAL_CPP})#gl.h gl.cpp main.cpp renderer.h renderable.h)
set_property(TARGET cool PROPERTY CXX_STANDARD 17)

source_group("Core Files" FILES ${CORE_HPP} ${CORE_CPP})
source_group("OpenGL Files" FILES ${OPENGL_HPP} ${OPENGL_CPP})
source_group("OpenAL Files" FILES ${OPENAL_HPP} ${OPENAL_CPP})

find_package(OpenGL REQUIRED)
set(CMAKE_SYSTEM_FRAMEWORK_PATH ${CMAKE_SYSTEM_FRAMEWORK_PATH}, "C:/Program Files (x86)/OpenAL 1.1 SDK")
find_package(OpenAL REQUIRED)

include_directories(libs/include/ src/ ${OPENGL_INCLUDE_DIR} ${OPENAL_INCLUDE_DIR})

#target_link_libraries(cool ${LIB_DIR}/glfw3.lib)
target_link_libraries(cool ${LIB_DIR}/glew32s.lib)
target_link_libraries(cool ${OPENGL_LIBRARIES})
target_link_libraries(cool ${OPENAL_LIBRARY})
target_link_libraries(cool ${LIB_DIR}/freetype.lib)
		
# text renderer
file(GLOB_RECURSE TEXT_RENDERER_FILES
	./demo/text_renderer/*.cpp
	./demo/text_renderer/*.hpp)
	
add_executable(text_renderer ${TEXT_RENDERER_FILES})
set_property(TARGET text_renderer PROPERTY CXX_STANDARD 17)
target_link_libraries(text_renderer cool)
target_link_libraries(text_renderer ${LIB_DIR}/glfw3.lib)
include_directories(libs/include/ src/)

source_group("Files" FILES ${TEXT_RENDERER_FILES})

#openal
file(GLOB_RECURSE OPENAL_FILES
	./demo/openal/*.cpp
	./demo/openal/*.hpp)
	
add_executable(openal ${OPENAL_FILES})
set_property(TARGET openal PROPERTY CXX_STANDARD 17)
target_link_libraries(openal cool)
include_directories(src/)

source_group("Files" FILES ${OPENAL_FILES})

# gui
file(GLOB_RECURSE GUI_FILES
	./demo/gui/*.cpp
	./demo/gui/*.hpp)
	
add_executable(gui ${GUI_FILES})
set_property(TARGET gui PROPERTY CXX_STANDARD 17)
target_link_libraries(gui cool)
target_link_libraries(gui ${LIB_DIR}/glfw3.lib)
include_directories(libs/include/ src/)

source_group("Files" FILES ${GUI_FILES})
