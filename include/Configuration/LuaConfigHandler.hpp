#pragma once

#include "Core/Scene.hpp"
#include "lua.hpp"
#include <string>

class LuaConfigHandler {
public:
  LuaConfigHandler(std::string &filepath);
  Scene* getScene() const;

private:
  static int _create_camera(lua_State *L);
  static int _create_sphere(lua_State *L);
  static int _create_dirlight(lua_State *L);
  static int _create_pointlight(lua_State *L);
};
