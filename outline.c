/*
* outline.c
* inspect glyph geometry using FreeType
* Luiz Henrique de Figueiredo <lhf@tecgraf.puc-rio.br>
* 24 May 2024 08:03:54
* This code is hereby placed in the public domain and also under the MIT license.
*/

#define TRY(err,what)	if ((err)!=0) return what

static const FT_Outline_Funcs outliners = {
	outline_move_to, outline_line_to, outline_conic_to, outline_cubic_to,
	0, 0
};

static unsigned int hash(const char *s)
{
	unsigned int h=0;
	for (; *s!=0; s++)
		h ^= ((h<<5) + (h>>2) + ((unsigned char)(*s)));
	return h;
}

static const char *outline(int g, const char *font, int size, void *data)
{
	static FT_Library library=NULL;
	static FT_Face face=NULL;
	static unsigned int oh=0;
	static unsigned int h=0;
	static int osize=0;
	static int og=0;
	if (library==NULL) {
		TRY(FT_Init_FreeType(&library),"FT_Init_FreeType");
	}
	h=hash(font);
	if (oh!=h) {
		oh=h;
		osize=0;
		og=0;
		TRY(FT_New_Face(library,font,0,&face),"FT_New_Face");
		TRY(!FT_IS_SCALABLE(face),"FT_IS_SCALABLE");
	}
	if (osize!=size) {
		osize=size;
		og=0;
		TRY(FT_Set_Char_Size(face,size*64,0,0,0),"FT_Set_Char_Size");
	}
	if (og!=g) {
		og=g;
		g= (g>0) ? FT_Get_Char_Index(face,g) : -g;
		TRY(FT_Load_Glyph(face,g,FT_LOAD_NO_HINTING),"FT_Load_Glyph");
	}
	outline_begin(og,g,data);
	TRY(FT_Outline_Decompose(&face->glyph->outline,&outliners,data),"FT_Outline_Decompose");
	outline_end(data);
	return NULL;
}
