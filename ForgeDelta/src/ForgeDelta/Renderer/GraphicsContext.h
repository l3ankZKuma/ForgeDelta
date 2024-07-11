#pragma once


struct GLFwindow;

namespace ForgeDelta {

  class GraphicsContext {
  public:
    
    GraphicsContext(GLFWwindow * windowHandle);
    ~GraphicsContext();


    void Init();
    void SwapBuffers();
    void Clear();
     

  private:

    inline static GLFWwindow* s_WindowHandle = nullptr;

   };

}