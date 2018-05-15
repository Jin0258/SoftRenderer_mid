#pragma once
#include "stdafx.h"
#include "Vector.h"
struct Vertex
{
public:
	Vertex() {}
	Vertex(Vector3 v) { position = v; }
	Vertex(Vector3 v,Vector2 uvpos) { position = v, uv = uvpos; }
	Vector3 position;
	ULONG color;
	Vector2 uv;
};
