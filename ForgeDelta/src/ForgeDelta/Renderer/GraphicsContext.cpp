#include "fdpch.h"
#include "GraphicsContext.h"

namespace ForgeDelta {

  GraphicsContext::GraphicsContext(GLFWwindow* windowHandle){
        s_WindowHandle = windowHandle;
  }

  GraphicsContext::~GraphicsContext() {

  }

  void GraphicsContext::Init() {
    glfwMakeContextCurrent(s_WindowHandle);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  }

  void GraphicsContext::SwapBuffers() {
    glfwSwapBuffers(s_WindowHandle);
  }

  void GraphicsContext::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

 
}