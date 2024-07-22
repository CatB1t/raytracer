-- premake5.lua
workspace "Raytracer"
  configurations { "Debug", "Release" }
  location "build/"

local build_path = "build/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
local lua_dir = "../vendor/lua/"

-- Lua
project "LuaCAPI"
  kind "Makefile"

  buildcommands {
      "make -C " .. lua_dir
   }

   rebuildcommands {
      "make rebuild -C " .. lua_dir
   }

   cleancommands {
      "make clean -C " .. lua_dir
   }


project "Raytracer"
  targetname "raytracer"

  architecture "x86_64"
  kind "ConsoleApp"
  language "C++"

  targetdir (build_path)
  objdir (build_path .. "/obj")

  includedirs { "./include/", "./vendor/lua/src/", "./vendor/stb/" }
  files { "./src/**.cpp" }

  libdirs {"./vendor/lua/src/"}
  links {"lua"}

  filter {"configurations:Debug"}
    defines {"DEBUG"}
    symbols "On"

  filter {"configurations:Release"}
    defines {"NDEBUG"}
    symbols "On"

