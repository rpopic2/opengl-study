workspace "opengl-study"
    configurations { "Debug", "Release" }

project "opengl-study"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"

    libdirs { "libs" }
    includedirs { "include" }


    links { "glfw.3.3" }
    links { "OpenGL.framework" }
    files { "**.h", "**.cpp" }

    filter "configurations:Debug"
       defines { "DEBUG" }
       symbols "On"

    filter "configurations:Release"
       defines { "NDEBUG" }
       optimize "On"

