workspace "GolfDash"
    architecture "x86_64"
    configurations { "Debug", "Release", "Dist" }
    flags "MultiProcessorCompile"
    startproject "GolfDash"

    targetdir ("Build/Bin/%{cfg.buildcfg}/%{prj.name}")
    objdir ("Build/Obj/%{cfg.buildcfg}/%{prj.name}")

project "GolfDash"
    language "C++"
    cppdialect "C++latest"
    location "GolfDash"
    staticruntime "Off"

    icon "%{prj.name}/GolfDash.ico"

    pchheader "GolfDashPCH.h"
    pchsource "%{prj.name}/Source/GolfDashPCH.cpp"

    files
    {
        "%{prj.name}/Source/**.h",
        "%{prj.name}/Source/**.cpp",

        "%{prj.name}/Assets/**.glsl",
        "%{prj.name}/Assets/**.png",
        "%{prj.name}/Assets/**.psd",

        "%{prj.name}/Libraries/glm/glm/**.hpp",
        "%{prj.name}/Libraries/glm/glm/**.inl",

        "%{prj.name}/Libraries/stb_image/stb_image.h"
    }
    
    includedirs
    {
        "%{prj.name}/Source",
        "%{prj.name}/Libraries/GLFW/include",
        "%{prj.name}/Libraries/Glad/include",
        "%{prj.name}/Libraries/zlib/include",
        "%{prj.name}/Libraries/glm",
        "%{prj.name}/Libraries/stb_image"
    }

    links
    {
        "GLFW",
        "Glad",
        "zlib"
    }
    
    defines "_CRT_SECURE_NO_WARNINGS"

    filter "system:Windows"
        systemversion "latest"
        defines "GD_OS_WINDOWS"

    filter "configurations:Debug"
        kind "ConsoleApp"
        defines "GD_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        kind "ConsoleApp"
        defines { "GD_RELEASE", "NDEBUG" }
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        kind "WindowedApp"
        defines { "GD_DIST", "NDEBUG" }
        runtime "Release"
        optimize "On"
        symbols "Off"

group "Libraries"
    include "GolfDash/Libraries/GLFW"
    include "GolfDash/Libraries/Glad"
    include "GolfDash/Libraries/zlib"
group ""