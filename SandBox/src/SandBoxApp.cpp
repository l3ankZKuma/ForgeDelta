#include <ForgeDelta.h>
#include"imgui.h"


class Layers : public ForgeDelta::Layer {
public:
  Layers() : Layer("Layers") {

  }

  void OnAttach() override {

  }

  void OnDetach() override {

  }

  void OnUpdate(ForgeDelta::TimeStep ts) override {

  }

  void OnEvent(ForgeDelta::Event& e) override {

  }

  void OnImGuiRender() override {


  }

};

class SandBox : public ForgeDelta::Application {

  public:

  SandBox() {
    PushLayer(new Layers());
  }

  ~SandBox() {

  }



};



ForgeDelta::Application* ForgeDelta::CreateApplication() {
  return new SandBox();
}

 