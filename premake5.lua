-- premake5.lua
workspace "GameProject"
    architecture "x64"
    startproject "Game"
    
    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }
    
    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Architecture remapping for SDL3
archmap = {}
archmap["x86_64"] = "x64"
archmap["x86"] = "x86"
archmap["ARM64"] = "arm64"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["SDL3"] = "vendor/sdl3/include"
IncludeDir["Astra"] = "vendor/Astra/include"

project "Game"
    location "Game"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
    rtti "off"
    exceptionhandling "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    buildoptions { 
        "/Zc:__cplusplus",     -- Enable proper __cplusplus macro
        "/arch:AVX"             -- Enable up to AVX (includes SSE4.2)
    }
    defines { 
            "_CRT_SECURE_NO_WARNINGS",
            "__SSE2__",             -- Define SSE2 support
            "__SSE4_2__"            -- Define SSE4.2 support (for CRC32)
        }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.SDL3}",
        "%{IncludeDir.Astra}"
    }
    
    libdirs
    {
        "vendor/sdl3/lib/%{archmap[cfg.architecture] or cfg.architecture}"
    }
    
    links
    {
        "SDL3"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "PLATFORM_WINDOWS"
        }
        
        postbuildcommands
        {
            ("{COPYFILE} %{wks.location}/vendor/sdl3/lib/%{archmap[cfg.architecture] or cfg.architecture}/SDL3.dll %{cfg.targetdir}")
        }

    filter "system:linux"
        pic "On"

        defines
        {
            "PLATFORM_LINUX"
        }

        links
        {
            "pthread",
            "dl"
        }

    filter "configurations:Debug"
        defines { "DEBUG", "ASTRA_BUILD_DEBUG" }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines { "RELEASE", "ASTRA_BUILD_RELEASE" }
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines { "DIST", "ASTRA_BUILD_RELEASE" }
        runtime "Release"
        optimize "on"