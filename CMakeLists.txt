cmake_minimum_required(VERSION 3.14)
project(Royal_Jaguar_3)

#option(MINGW_STDTHREADS_GENERATE_STDHEADERS "" ON)

include_directories(include)
include_directories(include/glew-2.1.0/include)
include_directories(include/glm-master/include)
include_directories(include/glfw-3.3.2/include)
include_directories(include/SOIL-master/include)
include_directories(include/json-develop/include)
include_directories(include/Royal-Jaguar-3/include)
include_directories(include/mingw-std-threads-master)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mthreads -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
add_compile_definitions(GLEW_STATIC)
add_compile_definitions(_WIN32_WININT)

set(JSON_BuildTests OFF CACHE INTERNAL    "")

add_subdirectory(include/glfw-3.3.2)
add_subdirectory(include/glew-2.1.0/build/cmake)
add_subdirectory(include/glm-master)
add_subdirectory(include/SOIL-master)
add_subdirectory(include/json-develop)
add_subdirectory(include/mingw-std-threads-master)

set(CMAKE_CXX_STANDARD 11)

add_executable(Royal_Jaguar_3
		src/SystemControl.cpp
		src/SystemControl.h
		include/Royal-Jaguar-3/JsonTools.cpp
		include/Royal-Jaguar-3/include/JsonTools/JsonTools.h
		src/ShaderProgram.h
		src/ShaderProgram.cpp
		src/SpriteRender.cpp
		src/SpriteRender.h
		src/Window.cpp
		src/Window.h
		src/GameObject.cpp
		src/GameObject.h
		src/Network.cpp
		src/Network.h)

file(COPY src/resources DESTINATION "${CMAKE_BINARY_DIR}/src")
file(COPY libs/ DESTINATION "${CMAKE_BINARY_DIR}")
#file(COPY src/shaders/sprite.vert DESTINATION "${CMAKE_BINARY_DIR}/src/shaders")
#file(COPY src/resources/ash_uvgrid01.png DESTINATION "${CMAKE_BINARY_DIR}/resources")
#file(COPY src/resources/ash_uvgrid07.png DESTINATION "${CMAKE_BINARY_DIR}/resources")

target_link_libraries(Royal_Jaguar_3 nlohmann_json glfw glew -lglu32 -lopengl32 glm soil -lWs2_32)