//
// Copyright (c) 2011 Andreas Krinke andreas.krinke@gmx.de
// Copyright (c) 2009 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#ifndef FONTSTASH_H
#define FONTSTASH_H

#include "OpenGLInclude.h"

struct vec2
{
	vec2(float x, float y)
	{
		p[0] = x;
		p[1] = y;
	}
	float p[2];
};

struct vec4
{
	vec4(float x,float y, float z, float w)
	{
		p[0] = x;
		p[1] = y;
		p[2] = z;
		p[3] = w;
        
	}
    
	float p[4];
};

typedef struct
{
    vec4 position;
    vec4 colour;
	vec2 uv;
} Vertex;

struct sth_stash* sth_create(int cachew, int cacheh);

int sth_add_font(struct sth_stash* stash, const char* path);
int sth_add_font_from_memory(struct sth_stash* stash, unsigned char* buffer);

int sth_add_bitmap_font(struct sth_stash* stash, int ascent, int descent, int line_gap);
void sth_add_glyph(struct sth_stash* stash, int idx, GLuint id, const char* s,
                  short size, short base, int x, int y, int w, int h,
                  float xoffset, float yoffset, float xadvance);

void sth_begin_draw(struct sth_stash* stash);
void sth_end_draw(struct sth_stash* stash);

void sth_draw_texture(struct sth_stash* stash,
				   int idx, float size,
				   float x, float y,
				   int screenwidth, int screenheight,
				   const char* s, float* dx);

void sth_flush_draw(struct sth_stash* stash);

void sth_draw_text(struct sth_stash* stash,
				   int idx, float size,
				   float x, float y, const char* string, float* dx, int screenwidth, int screenheight);

void sth_dim_text(struct sth_stash* stash, int idx, float size, const char* string,
				  float* minx, float* miny, float* maxx, float* maxy);

void sth_vmetrics(struct sth_stash* stash,
				  int idx, float size,
				  float* ascender, float* descender, float * lineh);

void sth_delete(struct sth_stash* stash);



#endif // FONTSTASH_H
