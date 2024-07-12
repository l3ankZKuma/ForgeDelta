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


#include"ForgeDelta/Renderer/Renderer.h"
#include"ForgeDelta/Renderer/RendererCommand.h"
#include"ForgeDelta/Renderer/RendererAPI.h"

//----------------------------------------------



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