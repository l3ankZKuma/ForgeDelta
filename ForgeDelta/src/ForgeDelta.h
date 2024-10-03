#pragma once

//------------------[Core]----------------------

#include "ForgeDelta/Core/Application.h"
#include "ForgeDelta/Core/Layer.h"

#include "ForgeDelta/Core/Log.h"

#include "ForgeDelta/Core/TimeStep.h"
#include "ForgeDelta/Core/Window.h"

#include"ForgeDelta/Core/Timer.h"


//------------------[Renderer]------------------


#include "ForgeDelta/Renderer/Renderer.h"
#include "ForgeDelta/Renderer/Buffer.h"
#include "ForgeDelta/Renderer/Shader.h"
#include "ForgeDelta/Renderer/VertexArray.h"
#include"ForgeDelta/Renderer/Framebuffer.h"
#include"ForgeDelta/Renderer/Texture.h"


#include"ForgeDelta/Renderer/Renderer.h"
#include"ForgeDelta/Renderer/RenderCommand.h"
#include"ForgeDelta/Renderer/RendererAPI.h"

#include"ForgeDelta/Renderer/Renderer2D.h"



//-------------------[Debug}--------------------


#include "ForgeDelta/Debug/Instrumentor.h"




//------------------[Camera]------------------

#include"ForgeDelta/Renderer/Camera/Orthographic2DCamera.h"
#include"ForgeDelta/Core/OrthographicCamera2DController.h"


//----------------------------------------------



//Manager

// Entry Point
#include"ForgeDelta/EntryPoint.h"

#ifndef FD_PLATFORM_WINDOWS
  #error ForgeDelta only supports Windows!
#endif


//pch
#include "fdpch.h"