cmake_minimum_required(VERSION 3.6)

project(UtilityHeap)

set(LIB_DIR ${CMAKE_SOURCE_DIR}/libs)

# utility-heap lib
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
	
add_library(utility-heap STATIC 
	${CORE_HPP} ${CORE_CPP} ${OPENGL_HPP} ${OPENGL_CPP} ${OPENAL_HPP} ${OPENAL_CPP} ${GLFW_FILES})#gl.h gl.cpp main.cpp renderer.h renderable.h)
set_property(TARGET utility-heap PROPERTY CXX_STANDARD 17)

source_group("Core" FILES ${CORE_HPP} ${CORE_CPP})
source_group("OpenGL" FILES ${OPENGL_HPP} ${OPENGL_CPP})
source_group("OpenAL" FILES ${OPENAL_HPP} ${OPENAL_CPP})
source_group("GLFW" FILES ${GLFW_FILES})
source_group("Freetype" FILES ${FREETYPE_FILES})

find_package(OpenGL REQUIRED)

include_directories(libs/include/ src/ ${OPENGL_INCLUDE_DIR} ${OPENAL_INCLUDE_DIR})

target_link_libraries(utility-heap ${LIB_DIR}/glfw3.lib)
target_link_libraries(utility-heap ${LIB_DIR}/glew32s.lib)
target_link_libraries(utility-heap ${OPENGL_LIBRARIES})
target_link_libraries(utility-heap ${LIB_DIR}/OpenAL32.lib)
target_link_libraries(utility-heap ${LIB_DIR}/freetype.lib)

# triangle
file(GLOB_RECURSE TRIANGLE_FILES
	./demo/triangle/*.cpp
	./demo/triangle/*.hpp)
	
add_executable(demo-triangle ${TRIANGLE_FILES})
set_property(TARGET demo-triangle PROPERTY CXX_STANDARD 17)
target_link_libraries(demo-triangle utility-heap)
include_directories(libs/include/ src/)

source_group("Files" FILES ${TRIANGLE_FILES})

# text renderer
file(GLOB_RECURSE TEXT_RENDERER_FILES
	./demo/text_renderer/*.cpp
	./demo/text_renderer/*.hpp)
	
add_executable(demo-text-renderer ${TEXT_RENDERER_FILES})
set_property(TARGET demo-text-renderer PROPERTY CXX_STANDARD 17)
target_link_libraries(demo-text-renderer utility-heap)
include_directories(libs/include/ src/)

source_group("Files" FILES ${TEXT_RENDERER_FILES})

#openal
file(GLOB_RECURSE OPENAL_FILES
	./demo/openal/*.cpp
	./demo/openal/*.hpp)
	
add_executable(demo-openal ${OPENAL_FILES})
set_property(TARGET demo-openal PROPERTY CXX_STANDARD 17)
target_link_libraries(demo-openal utility-heap)
include_directories(src/)

source_group("Files" FILES ${OPENAL_FILES})

# gui
file(GLOB_RECURSE GUI_FILES
	./demo/gui/*.cpp
	./demo/gui/*.hpp)
	
add_executable(demo-gui ${GUI_FILES})
set_property(TARGET demo-gui PROPERTY CXX_STANDARD 17)
target_link_libraries(demo-gui utility-heap)
include_directories(libs/include/ src/)

source_group("Files" FILES ${GUI_FILES})

# pro player
file(GLOB_RECURSE PRO_PLAYER_FILES
	./demo/proplayer/*.cpp
	./demo/proplayer/*.hpp)
	
add_executable(demo-pro-player ${PRO_PLAYER_FILES})
set_property(TARGET demo-pro-player PROPERTY CXX_STANDARD 17)
target_link_libraries(demo-pro-player utility-heap)
include_directories(libs/include/ src/)

source_group("Files" FILES ${PRO_PLAYER_FILES})
