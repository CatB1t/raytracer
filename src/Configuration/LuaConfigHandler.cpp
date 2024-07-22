#include "Configuration/LuaConfigHandler.hpp"
#include "lua.hpp"
#include <cstdio>

#define _RAYTRACER_LUA_SCENE_ "_RAYTRACER_LUA_SCENE_ADDRESS_"

Scene *LuaConfigHandler::getScene() const { return m_scene; }

LuaConfigHandler::LuaConfigHandler(const std::string &filepath)
    : m_filepath(filepath) {}

bool LuaConfigHandler::initialize() {
  lua_State *state = luaL_newstate();
  _define_globals(state);

  m_scene = new Scene();
  lua_pushstring(state, _RAYTRACER_LUA_SCENE_);
  lua_pushlightuserdata(state, m_scene);
  lua_settable(state, LUA_REGISTRYINDEX);

  if (luaL_dofile(state, m_filepath.c_str()) != LUA_OK) {
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

int LuaConfigHandler::_create_camera(lua_State *L) {
  printf("Create Camera Called\n");
  return 0;
}
int LuaConfigHandler::_create_sphere(lua_State *L) {

  if (!lua_istable(L, 1)) {
    luaL_error(L, "Argument must be a table");
  }

  Sphere sphere;

  lua_getfield(L, 1, "radius");
  sphere.radius = luaL_checkinteger(L, -1);
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

  lua_pushstring(L, _RAYTRACER_LUA_SCENE_);
  lua_gettable(L, LUA_REGISTRYINDEX);
  Scene *scn_ptr = (Scene *)lua_touserdata(L, -1);
  scn_ptr->spheres.push_back(sphere);
  lua_pop(L, 1);
  return 0;
}

int LuaConfigHandler::_create_dirlight(lua_State *L) {
  printf("Create Directional Light Called\n");
  return 0;
}

int LuaConfigHandler::_create_pointlight(lua_State *L) {
  printf("Create Point Light Called\n");
  return 0;
}
