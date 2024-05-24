/*
* outline.h
* inspect glyph geometry using FreeType
* Luiz Henrique de Figueiredo <lhf@tecgraf.puc-rio.br>
* 23 May 2024 14:22:29
* This code is hereby placed in the public domain and also under the MIT license.
*/

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H

#define F(t)	((t)/64.0)
#define X(p)	F(p->x)
#define Y(p)	F(p->y)

#define Point	const FT_Vector *
