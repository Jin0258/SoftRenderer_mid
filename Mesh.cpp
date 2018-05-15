
#include "stdafx.h"
#include "Mesh.h"

Mesh::~Mesh()
{
	if (NULL != vertices)
	{
		delete[] vertices;
		vertices = NULL;
	}

	if (NULL != Indices)
	{
		delete[] Indices;
		Indices = NULL;
	}
}

void Mesh::initialize()
{
	vertices = NULL;
	Indices = NULL;
	ISize = 0;
	VSize = 0;
}

void Mesh::SetVertetices(Vertex *vert, size_t NewSize)
{	
	initialize();// √ ±‚»≠

	vertices = vert;
	VSize = (int)NewSize;
}

//void Mesh::setIndices(int * NewIndices, size_t NewSize)
void Mesh::setIndices()
{
	ISize = VSize;
	Indices = new int[ISize];

	for (int i = 0; i < ISize; i++)
	{
		Indices[i] = i;
	}
}

