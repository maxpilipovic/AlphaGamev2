project "SDL_image"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.c",
        "src/**.h",
        "include/**.h"
    }

    excludes
    {
        "src/IMG_ImageIO.m",  -- macOS/iOS specific
        "src/IMG_WIC.c"       -- Windows Image Component (optional)
    }

    includedirs
    {
        "include",
        "include/SDL3_image",
        "../sdl3/include"
    }

    filter "system:windows"
        systemversion "latest"
        defines { "_CRT_SECURE_NO_WARNINGS" }

    filter "system:linux"
        pic "On"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        runtime "Release"
        optimize "on"