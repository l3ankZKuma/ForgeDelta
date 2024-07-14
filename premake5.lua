workspace "ForgeDelta"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["glad"] = "ForgeDelta/vendor/glad/include"
IncludeDir["glfw"] = "ForgeDelta/vendor/glfw/include"
IncludeDir["spdlog"] = "ForgeDelta/vendor/spdlog/include"
IncludeDir["imgui"] = "ForgeDelta/vendor/imgui"
IncludeDir["glm"] = "ForgeDelta/vendor/glm"
IncludeDir["stb_image"] = "ForgeDelta/vendor/stb_image"
IncludeDir["EASTL"] = "ForgeDelta/vendor/EASTL/include"
IncludeDir["EABase"] = "ForgeDelta/vendor/EABase/include/Common"

-- Include the projects
group "Dependencies"
    include "ForgeDelta/vendor/glfw"
    include "ForgeDelta/vendor/glad"
    include "ForgeDelta/vendor/imgui"
group ""

project "ForgeDelta"
    location "ForgeDelta"
    kind "StaticLib"
    language "C++"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "fdpch.h"
    pchsource "ForgeDelta/src/fdpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.glad}",
        "%{IncludeDir.glfw}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.EASTL}",
        "%{IncludeDir.EABase}"
    }

    links
    {
        "glfw",
        "glad",
        "imgui",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "on"
        systemversion "latest"

        defines
        {
            "FD_PLATFORM_WINDOWS",
            "FD_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "FD_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "FD_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "FD_DIST"
        runtime "Release"
        optimize "on"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "ForgeDelta/src",
        "ForgeDelta/vendor",
        "%{IncludeDir.glm}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.glad}",
        "%{IncludeDir.glfw}",
        "%{IncludeDir.EASTL}",
        "%{IncludeDir.EABase}",
    }

    links
    {
        "ForgeDelta",
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "FD_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "FD_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "FD_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "FD_DIST"
        runtime "Release"
        optimize "on"
