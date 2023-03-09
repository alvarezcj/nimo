#include "Context.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "core/Log.h"

nimo::Context::Context(){
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        NIMO_CRITICAL("Failed to initialize OpenGL context");
    }
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
nimo::Context::~Context(){
    
}