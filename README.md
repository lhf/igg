Inspect glyph geometry
======================

This is a tool for inspecting glyph geometry.
It uses [FreeType](https://freetype.org) to read font files and extract glyph data via [FT_Outline_Decompose](http://freetype.org/freetype2/docs/reference/ft2-outline_processing.html#ft_outline_decompose).

The file outline.c contains all code that interfaces with FreeType.
The code for this is in outline.c. Clients
include outline.h, define outline callbacks, include outline.c and call a function outline.
``
static const char *outline(int g, const char *font, int size, void *data)
``
There is a simple client in main.c, which can be used as a template.
There is a Lua client in fd.c, which leaves the processing to a Lua script.
As a demo, ps.lua outputs PostScript disections of glyphs.

To build, set the locations of FreeType headers and library in Makefile.

This code is hereby placed in the public domain and also under the MIT license.
