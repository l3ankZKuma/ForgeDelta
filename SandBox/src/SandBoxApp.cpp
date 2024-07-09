#include <ForgeDelta.h>
#include"fdpch.h"

void SandBox() {


  auto app = ForgeDelta::Manager::GetInstance().GetApplicationData();


}



ForgeDelta::ApplicationData* ForgeDelta::CreateApplication() {
  return new ApplicationData();
}

 