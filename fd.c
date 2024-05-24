/*
* fd.c
* inspect glyph geometry using FreeType via Lua
* Luiz Henrique de Figueiredo <lhf@tecgraf.puc-rio.br>
* 23 May 2024 14:22:19
* This code is hereby placed in the public domain and also under the MIT license.
*/

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "outline.h"

static int outline_begin(int a, int b, void *data)
{
	lua_State *L=data;
	lua_pushvalue(L,-1);
	lua_pushliteral(L,"B");
	lua_pushinteger(L,a);
	lua_pushinteger(L,b);
	lua_call(L,3,0);
	return 0;
}

static int outline_move_to(Point p, void *data)
{
	lua_State *L=data;
	lua_pushvalue(L,-1);
	lua_pushliteral(L,"M");
	lua_pushnumber(L,X(p));
	lua_pushnumber(L,Y(p));
	lua_call(L,3,0);
	return 0;
}

static int outline_line_to(Point p, void *data)
{
	lua_State *L=data;
	lua_pushvalue(L,-1);
	lua_pushliteral(L,"L");
	lua_pushnumber(L,X(p));
	lua_pushnumber(L,Y(p));
	lua_call(L,3,0);
	return 0;
}

static int outline_conic_to(Point c, Point p, void *data)
{
	lua_State *L=data;
	lua_pushvalue(L,-1);
	lua_pushliteral(L,"Q");
	lua_pushnumber(L,X(c));
	lua_pushnumber(L,Y(c));
	lua_pushnumber(L,X(p));
	lua_pushnumber(L,Y(p));
	lua_call(L,5,0);
	return 0;
}

static int outline_cubic_to(Point c1, Point c2, Point p, void *data)
{
	lua_State *L=data;
	lua_pushvalue(L,-1);
	lua_pushliteral(L,"C");
	lua_pushnumber(L,X(c1));
	lua_pushnumber(L,Y(c1));
	lua_pushnumber(L,X(c2));
	lua_pushnumber(L,Y(c2));
	lua_pushnumber(L,X(p));
	lua_pushnumber(L,Y(p));
	lua_call(L,7,0);
	return 0;
}

static int outline_end(void *data)
{
	lua_State *L=data;
	lua_pushvalue(L,-1);
	lua_pushliteral(L,"E");
	lua_call(L,1,0);
	return 0;
}

#include "outline.c"

static int doit(lua_State *L)
{
	lua_settop(L,3);
	const char *f=luaL_checkstring(L,2);
	int size=512;
	int g;
	if (lua_type(L,1)==LUA_TSTRING)
		g=*lua_tostring(L,1);
	else
		g=luaL_checkinteger(L,1);
	const char *rc=outline(g,f,size,L);
	if (rc!=NULL) luaL_error(L,"%s failed",rc);
	return 0;
}

LUALIB_API int luaopen_outline(lua_State *L)
{
	lua_pushcfunction(L,doit);
	return 1;
}

#ifdef STANDALONE
static void fatal(const char *format, const char *what)
{
	fprintf(stderr,"fd: ");
	fprintf(stderr,format,what);
	fprintf(stderr,"\n");
	exit(1);
}

int main(int argc, char **argv)
{
	lua_State *L=luaL_newstate();
	if (L==NULL) fatal("cannot create Lua state: not enough memory","");
	luaL_openlibs(L);
	luaL_requiref(L,"outline",luaopen_outline,0);
	lua_createtable(L,2,0);
	if (argc>2) { lua_pushstring(L,argv[2]); lua_rawseti(L,-2,1); }
	if (argc>3) { lua_pushstring(L,argv[3]); lua_rawseti(L,-2,2); }
	lua_setglobal(L,"arg");
	if (luaL_dofile(L,argv[1])) fatal("%s",lua_tostring(L,-1));
	lua_close(L);
	return 0;
}
#endif
