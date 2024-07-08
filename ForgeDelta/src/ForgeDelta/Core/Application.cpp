#include "fdpch.h"
#include "Application.h"
#include<ForgeDelta/Core/Log.h>

namespace ForgeDelta {

  void InitializeApplication(Application* app) {
    ForgeDelta::InitLogSystem();
  }

  void ShutdownApplication(Application* app) {
    // Shutdown code here
  }

  void RunApplication(Application* app) {
    while (true) {
      FD_CORE_INFO("APPLICATION RUNNING");
      OnUpdate(app);
    }
  }

  void OnUpdate(Application* app) {
    // Update code here
  }

  // void OnEvent(Application* app, Event* e) {
  //   // Event handling code here
  // }

}
