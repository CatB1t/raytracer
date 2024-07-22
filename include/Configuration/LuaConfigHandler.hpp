#pragma once

#include "Core/Scene.hpp"
#include "lua.hpp"
#include <string>
#include <unordered_map>

class LuaConfigHandler {
public:
  LuaConfigHandler(const std::string &filepath);
  bool initialize();
  Scene *getScene() const;

private:
  std::string m_filepath;
  Scene *m_scene;

  template<typename T>
  inline static T _pop_value(lua_State *L, int index, T (*fn_ptr) (lua_State *L, int index)) {
    lua_pushinteger(L, index);
    lua_gettable(L, 2);
    return fn_ptr(L, -1);
  }

  void _define_globals(lua_State *L);
  static int _create_camera(lua_State *L);
  static int _create_sphere(lua_State *L);
  static int _create_dirlight(lua_State *L);
  static int _create_pointlight(lua_State *L);

  std::unordered_map<std::string, lua_CFunction> _fn_mappings = {
      {"create_camera", _create_camera},
      {"create_sphere", _create_sphere},
      {"create_dirlight", _create_dirlight},
      {"create_pointlight", _create_pointlight},
  };
};
