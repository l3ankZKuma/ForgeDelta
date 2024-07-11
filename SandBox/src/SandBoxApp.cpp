#include <ForgeDelta.h>
#include"fdpch.h"



class SandBox : public ForgeDelta::Application {
  public:


  SandBox() {

  }

  ~SandBox() {

  }



};


ForgeDelta::Application* ForgeDelta::CreateApplication() {
  return new SandBox();
}

 