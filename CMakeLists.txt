cmake_minimum_required(VERSION 3.1...3.22)

project(nimo VERSION 1.0)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


add_library(glad STATIC ${CMAKE_SOURCE_DIR}/third_party/glad/src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/third_party/glad/include)

add_library(glfw INTERFACE IMPORTED GLOBAL)
target_include_directories(glfw INTERFACE ${CMAKE_SOURCE_DIR}/third_party/glfw/v3.3.7/include)
target_link_libraries(glfw INTERFACE ${CMAKE_SOURCE_DIR}/third_party/glfw/v3.3.7/lib-vc2022/glfw3.lib)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE ${CMAKE_SOURCE_DIR}/third_party/glm/v0.9.9)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${CMAKE_SOURCE_DIR}/third_party/stb)

add_library(assimp INTERFACE)
target_include_directories(assimp INTERFACE ${CMAKE_SOURCE_DIR}/third_party/assimp/v5.0.1/include)
target_link_libraries(assimp INTERFACE ${CMAKE_SOURCE_DIR}/third_party/assimp/v5.0.1/assimp-vc143-mt.lib)

add_library(json INTERFACE)
target_include_directories(json INTERFACE ${CMAKE_SOURCE_DIR}/third_party/nlohmann_json/v3.11.2/)

set(ImGUISources
${CMAKE_SOURCE_DIR}/third_party/imgui/imgui-docking/imgui.cpp
${CMAKE_SOURCE_DIR}/third_party/imgui/imgui-docking/imgui_tables.cpp
${CMAKE_SOURCE_DIR}/third_party/imgui/imgui-docking/imgui_draw.cpp
${CMAKE_SOURCE_DIR}/third_party/imgui/imgui-docking/imgui_widgets.cpp
${CMAKE_SOURCE_DIR}/third_party/imgui/imgui-docking/imgui_demo.cpp
${CMAKE_SOURCE_DIR}/third_party/imgui/imgui-docking/backends/imgui_impl_glfw.cpp
${CMAKE_SOURCE_DIR}/third_party/imgui/imgui-docking/backends/imgui_impl_opengl3.cpp
)
set(ImGUIIncludes
${CMAKE_SOURCE_DIR}/third_party/imgui/imgui-docking
${CMAKE_SOURCE_DIR}/third_party/imgui/imgui-docking/backends
${CMAKE_SOURCE_DIR}/third_party/glfw/v3.3.7/include
)
add_library(imgui ${ImGUISources})
target_include_directories(imgui PUBLIC ${ImGUIIncludes})

add_library(entt INTERFACE)
target_include_directories(entt INTERFACE ${CMAKE_SOURCE_DIR}/third_party/entt/v3.10.3/single_include)

set(NFDSources
${CMAKE_SOURCE_DIR}/third_party/nfd/v1.1.6/nfd_common.c
${CMAKE_SOURCE_DIR}/third_party/nfd/v1.1.6/nfd_win.cpp
)
set(NFDIncludes
${CMAKE_SOURCE_DIR}/third_party/nfd/v1.1.6/include
)
add_library(nfd ${NFDSources})
target_include_directories(nfd PUBLIC ${NFDIncludes})

add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE ${CMAKE_SOURCE_DIR}/third_party/spdlog/v1.11.0/include)

add_subdirectory(${CMAKE_SOURCE_DIR}/engine)
add_subdirectory(${CMAKE_SOURCE_DIR}/editor)