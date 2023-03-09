set(INCLUDES 
${CMAKE_CURRENT_SOURCE_DIR}
)
set(SOURCES 
${CMAKE_CURRENT_SOURCE_DIR}/core/Application.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/core/Entry.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/core/Window.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/core/GUID.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/core/Log.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/core/FileHandling.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/assets/AssetIndex.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/assets/AssetManager.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/assets/AssetSerializer.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/assets/MeshSource.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/assets/ImageSource.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/renderer/Renderer.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/renderer/Context.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/renderer/Shader.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/renderer/IndexBuffer.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/renderer/VertexArray.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/renderer/VertexBuffer.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/renderer/FrameBuffer.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/renderer/Texture.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/renderer/Mesh.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/scene/Scene.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/scene/SceneSerializer.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/project/Project.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/project/ProjectSerializer.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/scene/Entity.cpp 
)
set(LIBS
glad
glfw
glm
stb
assimp
json
imgui
entt
spdlog
)

add_library(nimo_engine STATIC ${SOURCES})
target_include_directories(nimo_engine PUBLIC ${INCLUDES})
target_link_libraries(nimo_engine PUBLIC ${LIBS})