#pragma once


#include "ForgeDelta/Core/Application.h"
#include "ForgeDelta/Core/Layer.h"

#include "ForgeDelta/Core/Log.h"

#include "ForgeDelta/Core/TimeStep.h"
#include "ForgeDelta/Core/Window.h"

//------------------[Renderer]------------------

#include "ForgeDelta/Renderer/Renderer.h"
#include "ForgeDelta/Renderer/Buffer.h"
#include "ForgeDelta/Renderer/Shader.h"
#include "ForgeDelta/Renderer/VertexArray.h"
  
//----------------------------------------------


//Manager

// Entry Point
#include"ForgeDelta/EntryPoint.h"

#ifndef FD_PLATFORM_WINDOWS
  #error ForgeDelta only supports Windows!
#endif

