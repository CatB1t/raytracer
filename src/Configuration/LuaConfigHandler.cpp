#include "Configuration/LuaConfigHandler.hpp"
#include "Graphics/RGBColor.hpp"
#include "Lights/DirectionalLight.hpp"
#include "Lights/PointLight.hpp"
#include "Vector/Vector3D.hpp"
#include "lua.hpp"
#include <cstdio>
#include <fstream>

#define _RAYTRACER_LUA_SCENE_ "_RAYTRACER_LUA_SCENE_ADDRESS_"

Scene *LuaConfigHandler::getScene() const { return m_scene; }

LuaConfigHandler::LuaConfigHandler() {}

bool LuaConfigHandler::generate_example_config(std::filesystem::path path) {
  if(std::filesystem::exists(path)) {
    printf("file %s already exists.\n", path.c_str());
    return false;
  }

  constexpr const char *str = R"(-- Raytracer example config
local RED = { 255, 0, 0 }
local GREEN = { 0, 255, 0 }
local BLACK = { 0, 0, 0 }

-- sets maximum ray tracing depth, try setting it to 0 and see the difference!
set_tracing_depth(3)
set_backgroundcolor (BLACK)

create_camera { position = {0, 0, 0} }

-- Spheres
-- create_sphere (radius, position, color, specular, reflective)
create_sphere { radius = 1, position = { 0, -1, 3 }, color = RED , spec = 500, ref = 0.2 }
create_sphere { radius = 1, position = { -2, 0, 4 }, color = GREEN , spec = 10, ref = 0.4 }
create_sphere { radius = 1, position = { 2, 0, 4 }, color = { 0, 0, 255 } , spec = 500, ref = 0.3 }
create_sphere { radius = 5000, position = { 0,-5001, 0}, color = { 255, 255, 0 } , spec = 1000, ref = 0.5 }

-- Lighting
set_ambientlight(0.2)
create_dirlight { intensity = 0.6, direction = { 2, 1, 0 } }
create_pointlight { intensity = 0.2, position = { 1, 4, 4 } })";

  std::fstream file {path.c_str(), std::ios_base::out};
  file << str << std::endl;
  file.close();

  return true;
}

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
  assert(table_index >= 1 && "negative indexing is not supported for table");
  RGBColor color;

  if (field_name) {
    lua_getfield(L, table_index, field_name);
    table_index += 1;
  }

  color.r = _pop_table_field<lua_Integer>(L, table_index, 1, luaL_checkinteger);
  color.g = _pop_table_field<lua_Integer>(L, table_index, 2, luaL_checkinteger);
  color.b = _pop_table_field<lua_Integer>(L, table_index, 3, luaL_checkinteger);

  if (field_name)
    lua_pop(L, 1);

  return color;
}

Vector3D LuaConfigHandler::_pop_vector3d(lua_State *L, int table_index, const char* field_name) {
  assert(table_index >= 1 && "negative indexing is not supported for table");
  Vector3D vec;

  if (field_name) {
    lua_getfield(L, table_index, field_name);
    table_index += 1;
  }

  vec.x = _pop_table_field<lua_Number>(L, table_index, 1, luaL_checknumber);
  vec.y = _pop_table_field<lua_Number>(L, table_index, 2, luaL_checknumber);
  vec.z = _pop_table_field<lua_Number>(L, table_index, 3, luaL_checknumber);

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
  sphere.radius = _pop_table_field<lua_Number>(L, 1, "radius", luaL_checknumber);
  sphere.center = _pop_vector3d(L, 1, "position");
  sphere.color = _pop_color(L, 1, "color");
  sphere.specular = _pop_table_field<lua_Number>(L, 1, "spec", luaL_checknumber);
  sphere.reflective = _pop_table_field<lua_Number>(L, 1, "ref", luaL_checknumber);

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
  light.intensity = _pop_table_field<lua_Number>(L, 1, "intensity", luaL_checknumber);
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
  light.intensity = _pop_table_field(L, 1, "intensity", luaL_checknumber);
  light.position = _pop_vector3d(L, 1, "position");

  lua_pushstring(L, _RAYTRACER_LUA_SCENE_);
  lua_gettable(L, LUA_REGISTRYINDEX);
  Scene *scn_ptr = (Scene *)lua_touserdata(L, -1);
  lua_pop(L, 1);
  scn_ptr->point_lights.push_back(light);
  return 0;
}
