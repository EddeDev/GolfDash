workspace "GolfDash"
    architecture "x86_64"
    configurations { "Debug", "Release", "Dist" }
    flags "MultiProcessorCompile"
    startproject "GolfDash"

    targetdir ("Build/Bin/%{cfg.buildcfg}/%{prj.name}")
    objdir ("Build/Obj/%{cfg.buildcfg}/%{prj.name}")

project "GolfDash"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    location "GolfDash"
    staticruntime "Off"

    pchheader "GolfDashPCH.h"
    pchsource "%{prj.name}/Source/GolfDashPCH.cpp"

    files
    {
        "%{prj.name}/Source/**.h",
        "%{prj.name}/Source/**.cpp"
    }
    
    includedirs
    {
        "%{prj.name}/Source",
        "%{prj.name}/Libraries/GLFW/include",
        "%{prj.name}/Libraries/Glad/include"
    }

    links
    {
        "GLFW",
        "Glad"
    }

    filter "system:Windows"
        systemversion "latest"
        defines "GD_OS_WINDOWS"

    filter "configurations:Debug"
        defines "GD_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "GD_RELEASE", "NDEBUG" }
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines { "GD_DIST", "NDEBUG" }
        runtime "Release"
        optimize "On"
        symbols "Off"

group "Libraries"
    include "GolfDash/Libraries/GLFW"
    include "GolfDash/Libraries/Glad"
group ""