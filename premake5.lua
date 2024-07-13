-- premake5.lua
workspace "Raytracer"
  configurations { "Debug", "Release" }

project "Raytracer"
  kind "ConsoleApp"
  language "C++"
  targetdir "build/%{cfg.buildcfg}"
  
  files { "src/*.cpp", "src/*.hpp" }

  filter {"configurations:Debug"}
    defines {"DEBUG"}
    symbols "On"

  filter {"configurations:Release"}
    defines {"NDEBUG"}
    symbols "On"

