-- premake5.lua
workspace "Raytracer"
  configurations { "Debug", "Release" }

build_path = "build/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Raytracer"
  architecture "x86_64"
  kind "ConsoleApp"
  language "C++"
  targetdir (build_path)
  objdir (build_path .. "/obj")
  includedirs { "./include/" }
  files { "src/*.cpp", "src/*.hpp" }

  filter {"configurations:Debug"}
    defines {"DEBUG"}
    symbols "On"

  filter {"configurations:Release"}
    defines {"NDEBUG"}
    symbols "On"

