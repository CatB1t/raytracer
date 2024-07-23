#include "Configuration/LuaConfigHandler.hpp"
#include "Lights/DirectionalLight.hpp"
#include "Lights/PointLight.hpp"
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

  lua_pushstring(L, "position");
  lua_gettable(L, 1);

  scn_ptr->camera_position.x = _pop_value<lua_Number>(L, 1, luaL_checknumber);
  scn_ptr->camera_position.y = _pop_value<lua_Number>(L, 2, luaL_checknumber);
  scn_ptr->camera_position.z = _pop_value<lua_Number>(L, 3, luaL_checknumber);
  lua_pop(L, 1);

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

  lua_pushstring(L, "color");
  lua_gettable(L, 1);

  scn_ptr->background_color.r = _pop_value<lua_Integer>(L, 1, luaL_checkinteger);
  scn_ptr->background_color.g = _pop_value<lua_Integer>(L, 2, luaL_checkinteger);
  scn_ptr->background_color.b = _pop_value<lua_Integer>(L, 3, luaL_checkinteger);
  lua_pop(L, 1);

  return 0;
}

int LuaConfigHandler::_set_ambientlight(lua_State *L) {
  if (!lua_istable(L, 1)) {
    luaL_error(L, "Argument must be a table");
  }

  lua_pushstring(L, _RAYTRACER_LUA_SCENE_);
  lua_gettable(L, LUA_REGISTRYINDEX);
  Scene *scn_ptr = (Scene *)lua_touserdata(L, -1);
  lua_pop(L, 1);

  lua_getfield(L, 1, "intensity");
  scn_ptr->ambient_light.intensity = luaL_checknumber(L, -1);
  lua_pop(L, 1);

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

  lua_pushstring(L, "position");
  lua_gettable(L, 1);

  sphere.center.x = _pop_value<lua_Number>(L, 1, luaL_checknumber);
  sphere.center.y = _pop_value<lua_Number>(L, 2, luaL_checknumber);
  sphere.center.z = _pop_value<lua_Number>(L, 3, luaL_checknumber);
  lua_pop(L, 1);

  lua_pushstring(L, "color");
  lua_gettable(L, 1);

  sphere.color.r = _pop_value<lua_Integer>(L, 1, luaL_checkinteger);
  sphere.color.g = _pop_value<lua_Integer>(L, 2, luaL_checkinteger);
  sphere.color.b = _pop_value<lua_Integer>(L, 3, luaL_checkinteger);
  lua_pop(L, 1);

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

  lua_pushstring(L, "direction");
  lua_gettable(L, 1);

  light.direction.x = _pop_value<lua_Number>(L, 1, luaL_checknumber);
  light.direction.y = _pop_value<lua_Number>(L, 2, luaL_checknumber);
  light.direction.z = _pop_value<lua_Number>(L, 3, luaL_checknumber);
  lua_pop(L, 1);

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

  lua_pushstring(L, "position");
  lua_gettable(L, 1);

  light.position.x = _pop_value<lua_Number>(L, 1, luaL_checknumber);
  light.position.y = _pop_value<lua_Number>(L, 2, luaL_checknumber);
  light.position.z = _pop_value<lua_Number>(L, 3, luaL_checknumber);
  lua_pop(L, 1);

  lua_pushstring(L, _RAYTRACER_LUA_SCENE_);
  lua_gettable(L, LUA_REGISTRYINDEX);
  Scene *scn_ptr = (Scene *)lua_touserdata(L, -1);
  lua_pop(L, 1);
  scn_ptr->point_lights.push_back(light);
  return 0;
}
