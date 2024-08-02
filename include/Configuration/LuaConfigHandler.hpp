#pragma once

#include "Core/Scene.hpp"
#include "Graphics/RGBColor.hpp"
#include "Vector/Vector3D.hpp"
#include "lua.hpp"
#include <string>
#include <unordered_map>
#include <filesystem>

class LuaConfigHandler {
public:
  LuaConfigHandler();
  bool generate_example_config(std::filesystem::path path);
  bool read_config(const std::string &filepath, Scene* scene);

private:
  static RGBColor _pop_color(lua_State *L, int table_index, const char* field_name);
  static Vector3D _pop_vector3d(lua_State *L, int table_index, const char* field_name);

  template<typename T>
  static T _pop_table_field(lua_State *L, int table_index, const char* field_name, T (*fn_ptr) (lua_State *L, int index)) {
    lua_getfield(L, table_index, field_name);
    T val = fn_ptr(L, -1);
    lua_pop(L, 1);
    return val;
  }

  template<typename T>
  inline static T _pop_table_field(lua_State *L, int table_index, int index, T (*fn_ptr) (lua_State *L, int index)) {
    lua_pushinteger(L, index);
    lua_gettable(L, table_index);
    T val = fn_ptr(L, -1);
    lua_pop(L, 1);
    return val;
  }

  void _define_globals(lua_State *L);
  static int _set_tracing_depth(lua_State *L);
  static int _create_camera(lua_State *L);
  static int _set_backgroundcolor(lua_State *L);
  static int _set_ambientlight(lua_State *L);
  static int _create_sphere(lua_State *L);
  static int _create_dirlight(lua_State *L);
  static int _create_pointlight(lua_State *L);

  std::unordered_map<std::string, lua_CFunction> _fn_mappings = {
      {"set_tracing_depth", _set_tracing_depth},
      {"create_camera", _create_camera},
      {"set_ambientlight", _set_ambientlight},
      {"set_backgroundcolor", _set_backgroundcolor},
      {"create_sphere", _create_sphere},
      {"create_dirlight", _create_dirlight},
      {"create_pointlight", _create_pointlight},
  };
};
