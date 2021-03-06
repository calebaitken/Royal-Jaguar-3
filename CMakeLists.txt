cmake_minimum_required(VERSION 3.14)
project(Royal_Jaguar_3)

#option(MINGW_STDTHREADS_GENERATE_STDHEADERS "" ON)

include_directories(src/include)
include_directories(include/glew-2.1.0/include)
include_directories(include/glm-master/include)
include_directories(include/glfw-3.3.2/include)
include_directories(include/SOIL-master/include)
include_directories(include/json-develop/include)
include_directories(include/Royal-Jaguar-3/include)
include_directories(include/mingw-std-threads-master)
include_directories(include/freetype-2.9/include)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mthreads -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")

add_compile_definitions(GLFW_EXPOSE_NATIVE_WIN32)
add_compile_definitions(GLFW_EXPOSE_NATIVE_WGL)
add_compile_definitions(GLEW_STATIC)
add_compile_definitions(_WIN32_WININT)

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(include/glfw-3.3.2)
add_subdirectory(include/glew-2.1.0/build/cmake)
add_subdirectory(include/glm-master)
add_subdirectory(include/SOIL-master)
add_subdirectory(include/json-develop)
add_subdirectory(include/mingw-std-threads-master)
add_subdirectory(include/freetype-2.9)

set(CMAKE_CXX_STANDARD 17)

add_executable(Royal_Jaguar_3
		src/include/core/Memory.h src/core/Memory.cpp
		src/include/core/Input.h src/core/Input.cpp
		src/include/core/Network.h src/core/Network.cpp
		src/include/core/Scene.h src/core/Scene.cpp
		src/include/objects/Object.h
		src/objects/Empty.cpp src/include/objects/Empty.h
		src/core/Game.cpp src/include/core/Game.h
		src/graphics/Window.cpp src/include/graphics/Window.h
		src/objects/Card.cpp src/include/objects/Card.h
		src/include/objects/Deck.h src/objects/Deck.cpp
		src/include/objects/Player.h src/objects/Player.cpp)

file(COPY src/resources DESTINATION "${CMAKE_BINARY_DIR}/src")
file(COPY libs/ DESTINATION "${CMAKE_BINARY_DIR}")

target_link_libraries(Royal_Jaguar_3 freetype nlohmann_json glfw glew -lglu32 -lopengl32 glm soil -lWs2_32)