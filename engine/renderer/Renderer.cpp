#include "Renderer.h"
#include "glad/glad.h"

void nimo::Renderer::BeginFrame(std::shared_ptr<FrameBuffer> target){
    if(target)
        target->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); 
}
void nimo::Renderer::EndFrame(){
    FrameBuffer::unbind();
}