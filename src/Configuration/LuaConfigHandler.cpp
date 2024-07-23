#include "Configuration/LuaConfigHandler.hpp"
#include "Graphics/RGBColor.hpp"
#include "Lights/DirectionalLight.hpp"
#include "Lights/PointLight.hpp"
#include "Vector/Vector3D.hpp"
#include "lua.hpp"
#include <cstdio>

#define _RAYTRACER_LUA_SCENE_ "_RAYTRACER_LUA_SCENE_ADDRESS_"

Scene *LuaConfigHandler::getScene() const { return m_scene; }

LuaConfigHandler::LuaConfigHandler() {}

bool LuaConfigHandler::read_config(const std::string &filepath) {
  lua_State *state = luaL_newstate();
  _define_globals(state);

  m_scene = new Scene();
  lua_pushstring(state, _RAYTRACER_LUA_SCENE_);
  lua_pushlightuserdata(state, m_scene);
  lua_settable(state, LUA_REGISTRYINDEX);

  if (luaL_dofile(state, filepath.c_str()) != LUA_OK) {
    fprintf(stderr, "Lua failed: %s\n", lua_tostring(state, -1));
    lua_close(state);
    return false;
  }

  lua_close(state);
  return true;
}

void LuaConfigHandler::_define_globals(lua_State *L) {
  for (const std::pair<std::string, lua_CFunction> &key_fn : _fn_mappings) {
    lua_pushcfunction(L, key_fn.second);
    lua_setglobal(L, key_fn.first.c_str());
  }
}

RGBColor LuaConfigHandler::_pop_color(lua_State *L, int table_index, const char* field_name) {
  RGBColor color;

  if (field_name)
    lua_getfield(L, table_index, field_name);

  color.r = _pop_value<lua_Integer>(L, 1, luaL_checkinteger);
  color.g = _pop_value<lua_Integer>(L, 2, luaL_checkinteger);
  color.b = _pop_value<lua_Integer>(L, 3, luaL_checkinteger);

  if (field_name)
    lua_pop(L, 1);

  return color;
}

Vector3D LuaConfigHandler::_pop_vector3d(lua_State *L, int table_index, const char* field_name) {
  Vector3D vec;

  if (field_name)
    lua_getfield(L, table_index, field_name);

  vec.x = _pop_value<lua_Number>(L, 1, luaL_checknumber);
  vec.y = _pop_value<lua_Number>(L, 2, luaL_checknumber);
  vec.z = _pop_value<lua_Number>(L, 3, luaL_checknumber);

  if (field_name)
    lua_pop(L, 1);

  return vec;
}

int LuaConfigHandler::_set_tracing_depth(lua_State *L) {
  lua_pushstring(L, _RAYTRACER_LUA_SCENE_);
  lua_gettable(L, LUA_REGISTRYINDEX);
  Scene *scn_ptr = (Scene *)lua_touserdata(L, -1);
  lua_pop(L, 1);

  scn_ptr->tracing_depth = luaL_checkinteger(L, 1);
  return 0;
}

int LuaConfigHandler::_create_camera(lua_State *L) {
  if (!lua_istable(L, 1)) {
    luaL_error(L, "Argument must be a table");
  }

  lua_pushstring(L, _RAYTRACER_LUA_SCENE_);
  lua_gettable(L, LUA_REGISTRYINDEX);
  Scene *scn_ptr = (Scene *)lua_touserdata(L, -1);
  lua_pop(L, 1);

  scn_ptr->camera_position = _pop_vector3d(L, 1, "position");
  return 0;
}

int LuaConfigHandler::_set_backgroundcolor(lua_State *L) {
  if (!lua_istable(L, 1)) {
    luaL_error(L, "Argument must be a table");
  }

  lua_pushstring(L, _RAYTRACER_LUA_SCENE_);
  lua_gettable(L, LUA_REGISTRYINDEX);
  Scene *scn_ptr = (Scene *)lua_touserdata(L, -1);
  lua_pop(L, 1);

  scn_ptr->background_color = _pop_color(L, 1, nullptr);
  return 0;
}

int LuaConfigHandler::_set_ambientlight(lua_State *L) {
  lua_pushstring(L, _RAYTRACER_LUA_SCENE_);
  lua_gettable(L, LUA_REGISTRYINDEX);
  Scene *scn_ptr = (Scene *)lua_touserdata(L, -1);
  lua_pop(L, 1);

  scn_ptr->ambient_light.intensity = luaL_checknumber(L, -1);
  return 0;
}

int LuaConfigHandler::_create_sphere(lua_State *L) {
  if (!lua_istable(L, 1)) {
    luaL_error(L, "Argument must be a table");
  }

  Sphere sphere;

  lua_getfield(L, 1, "radius");
  sphere.radius = luaL_checknumber(L, -1);
  lua_pop(L, 1);

  sphere.center = _pop_vector3d(L, 1, "position");
  sphere.color = _pop_color(L, 1, "color");

  lua_getfield(L, 1, "spec");
  sphere.specular = luaL_checknumber(L, -1);
  lua_pop(L, 1);

  lua_getfield(L, 1, "ref");
  sphere.reflective = luaL_checknumber(L, -1);
  lua_pop(L, 1);

  lua_pushstring(L, _RAYTRACER_LUA_SCENE_);
  lua_gettable(L, LUA_REGISTRYINDEX);
  Scene *scn_ptr = (Scene *)lua_touserdata(L, -1);
  lua_pop(L, 1);

  scn_ptr->spheres.push_back(sphere);
  return 0;
}

int LuaConfigHandler::_create_dirlight(lua_State *L) {
  if (!lua_istable(L, 1)) {
    luaL_error(L, "Argument must be a table");
  }

  DirectionalLight light;

  lua_getfield(L, 1, "intensity");
  light.intensity = luaL_checknumber(L, -1);
  lua_pop(L, 1);

  light.direction = _pop_vector3d(L, 1, "direction");

  lua_pushstring(L, _RAYTRACER_LUA_SCENE_);
  lua_gettable(L, LUA_REGISTRYINDEX);
  Scene *scn_ptr = (Scene *)lua_touserdata(L, -1);
  lua_pop(L, 1);
  scn_ptr->directional_lights.push_back(light);

  return 0;
}

int LuaConfigHandler::_create_pointlight(lua_State *L) {
  if (!lua_istable(L, 1)) {
    luaL_error(L, "Argument must be a table");
  }

  PointLight light;

  lua_getfield(L, 1, "intensity");
  light.intensity = luaL_checknumber(L, -1);
  lua_pop(L, 1);

  light.position = _pop_vector3d(L, 1, "position");

  lua_pushstring(L, _RAYTRACER_LUA_SCENE_);
  lua_gettable(L, LUA_REGISTRYINDEX);
  Scene *scn_ptr = (Scene *)lua_touserdata(L, -1);
  lua_pop(L, 1);
  scn_ptr->point_lights.push_back(light);
  return 0;
}
