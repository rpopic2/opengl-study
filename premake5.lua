workspace "opengl-study"
    configurations { "Debug", "Release" }

project "opengl-study"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"

    libdirs { "libs" }
    includedirs { "include" }

    cppdialect "C++17"

    links { "glfw.3.3", "CoreVideo.framework", "OpenGL.framework", "IOKit.framework", "Cocoa.framework", "Carbon.framework", "GLEW.2.2.0" }
    files { "glad.c", "**.h", "**.cpp" }

    filter "configurations:Debug"
       defines { "DEBUG" }
       symbols "On"

    filter "configurations:Release"
       defines { "NDEBUG" }
       optimize "On"

