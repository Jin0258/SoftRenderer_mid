#pragma once
#include "Mesh.h"
#include "Triangle.h"

void InitFrame(void);
void UpdateFrame(void);
bool IsInRange(int x, int y);
void PutPixel(IntPoint pt);
void DrawCall(Mesh *mesh);

VertexToFragment VertexShader(VertexDATA in);
ULONG FragmentShader(VertexToFragment in, float s, float t);