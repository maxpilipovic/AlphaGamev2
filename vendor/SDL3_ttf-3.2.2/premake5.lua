project "SDL_ttf"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "SDL3_ttf-3.2.2/src/**.c",
        "SDL3_ttf-3.2.2/src/**.h",
        "SDL3_ttf-3.2.2/include/**.h"
    }

    includedirs
    {
        "SDL3_ttf-3.2.2/include",
        "SDL3_ttf-3.2.2/include/SDL3_ttf",
        "SDL3_ttf-3.2.2/src",
        "../sdl3/include",
        "../FreeType/include"
    }
    
    links
    {
        "FreeType"
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