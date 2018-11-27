cmake_minimum_required(VERSION 3.6)

project(UtilityHeap)

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
file(GLOB_RECURSE GLFW_FILES
	./src/glfw/*.hpp
	./src/glfw/*.cpp)
file(GLOB_RECURSE FREETYPE_FILES
	./src/freetype/*.hpp
	./src/freetype/*.cpp)
	
add_library(cool STATIC 
	${CORE_HPP} ${CORE_CPP} ${OPENGL_HPP} ${OPENGL_CPP} ${OPENAL_HPP} ${OPENAL_CPP} ${GLFW_FILES})#gl.h gl.cpp main.cpp renderer.h renderable.h)
set_property(TARGET cool PROPERTY CXX_STANDARD 17)

source_group("Core" FILES ${CORE_HPP} ${CORE_CPP})
source_group("OpenGL" FILES ${OPENGL_HPP} ${OPENGL_CPP})
source_group("OpenAL" FILES ${OPENAL_HPP} ${OPENAL_CPP})
source_group("GLFW" FILES ${GLFW_FILES})
source_group("Freetype" FILES ${FREETYPE_FILES})

find_package(OpenGL REQUIRED)

include_directories(libs/include/ src/ ${OPENGL_INCLUDE_DIR} ${OPENAL_INCLUDE_DIR})

target_link_libraries(cool ${LIB_DIR}/glfw3.lib)
target_link_libraries(cool ${LIB_DIR}/glew32s.lib)
target_link_libraries(cool ${OPENGL_LIBRARIES})
target_link_libraries(cool ${LIB_DIR}/OpenAL32.lib)
target_link_libraries(cool ${LIB_DIR}/freetype.lib)
		
# text renderer
file(GLOB_RECURSE TEXT_RENDERER_FILES
	./demo/text_renderer/*.cpp
	./demo/text_renderer/*.hpp)
	
add_executable(text_renderer ${TEXT_RENDERER_FILES})
set_property(TARGET text_renderer PROPERTY CXX_STANDARD 17)
target_link_libraries(text_renderer cool)
#target_link_libraries(text_renderer ${LIB_DIR}/glfw3.lib)
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
#target_link_libraries(gui ${LIB_DIR}/glfw3.lib)
include_directories(libs/include/ src/)

source_group("Files" FILES ${GUI_FILES})
