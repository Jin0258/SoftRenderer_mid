#pragma once
#include "stdafx.h"
#include "Vertex.h"

struct Mesh
{
public:
	Mesh() : vertices(NULL), Indices(NULL), ISize(0), VSize(0) { }
	~Mesh();

	Vertex* vertices;
	int* Indices;
	int ISize;
	int VSize;

	void initialize();
	void SetVertetices(Vertex *vert, size_t NewSize);
	void setIndices();
};
