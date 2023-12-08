#ifndef _VERTEXFORMAT_H
#define _VERTEXFORMAT_H

#include <d3d9.h>

struct s_vertex
{
	D3DCOLOR	color;	
	float		x, y, z;
	float		u,v;
	float		nx,ny,nz;
	float		tanx, tany, tanz;
	float		bix, biy, biz;	
};

extern D3DVERTEXELEMENT9 s_vertexElements[];

#endif