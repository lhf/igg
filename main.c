/*
* main.c
* inspect glyph geometry using FreeType
* Luiz Henrique de Figueiredo <lhf@tecgraf.puc-rio.br>
* 23 May 2024 14:22:21
* This code is hereby placed in the public domain and also under the MIT license.
*/

#include <stdio.h>
#include <stdlib.h>

#include "outline.h"

static int outline_begin(int a, int b, void *data)
{
	(void) data;
	printf("B %d %d\n",a,b);
	return 0;
}

static int outline_move_to(Point p, void *data)
{
	(void) data;
	printf("M %g %g\n",X(p),Y(p));
	return 0;
}

static int outline_line_to(Point p, void *data)
{
	(void) data;
	printf("L %g %g\n",X(p),Y(p));
	return 0;
}

static int outline_conic_to(Point c, Point p, void *data)
{
	(void) data;
	printf("Q %g %g %g %g\n",X(c),Y(c),X(p),Y(p));
	return 0;
}

static int outline_cubic_to(Point c1, Point c2, Point p, void *data)
{
	(void) data;
	printf("C %g %g %g %g %g %g\n",X(c1),Y(c1),X(c2),Y(c2),X(p),Y(p));
	return 0;
}

static int outline_end(void *data)
{
	(void) data;
	printf("E\n");
	return 0;
}

#include "outline.c"

static void doit(int g, const char *f, int size, void *data)
{
	const char *rc=outline(g,f,size,data);
	if (rc!=NULL) {
		fprintf(stderr,"fd: %s failed\n",rc);
		exit(1);
	}
}

int main(int argc, char **argv)
{
	int g;
	const char *f;
	int size=512;
	void *data=NULL;
	if (argc>1) g=*argv[1]; else g='g';
	if (argc>2) f= argv[2]; else f="/System/Library/Fonts/Times.ttc";
	doit(g,f,size,data);
	doit(g,f,size,data);
	doit(g,f,size*2,data);
	return 0;
}
