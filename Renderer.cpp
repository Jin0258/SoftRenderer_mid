
#include "stdafx.h"
#include "Vector.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"
#include "Triangle.h"
#include "Texture.h"

#define LAYERSIZE 3

bool IsInRange(int x, int y)
{
	return (abs(x) < (g_nClientWidth / 2)) && (abs(y) < (g_nClientHeight / 2));
}

void PutPixel(IntPoint pt)
{
	if (!IsInRange(pt.X, pt.Y)) return;

	ULONG* dest = (ULONG*)g_pBits;

	int halfWidth = RoundToInt(g_nClientWidth * 0.5f);
	int halfHeight = RoundToInt(g_nClientHeight * 0.5f);

	DWORD offset = (halfHeight * g_nClientWidth - g_nClientWidth * pt.Y) + (halfWidth + pt.X);
	*(dest + offset) = g_CurrentColor;
}

void DrawCall(Mesh * MeshToDraw)
{
	VertexToFragment* v2f = new VertexToFragment[MeshToDraw->VSize];

	for (int i = 0; i < MeshToDraw->VSize; i++)
	{
		Vertex v = MeshToDraw->vertices[i];
		VertexDATA vdata;
		vdata.position = v.position;
		vdata.color = v.color;
		vdata.uv = v.uv;
		v2f[i] = VertexShader(vdata);
	}

	for (int i = 0; i < MeshToDraw->ISize; i += 3)
	{
		Triangle primitive(v2f[MeshToDraw->Indices[i]],
			v2f[MeshToDraw->Indices[i+1]],
			v2f[MeshToDraw->Indices[i+2]]);

		// Rasterization
		for (int y = primitive.Min.Y; y <= primitive.Max.Y; y++)
		{
			for (int x = primitive.Min.X; x <= primitive.Max.X; x++)
			{
				Vector3 target((float)x + 0.5f, (float)y + 0.5f, 0.0f);
				float outS, outT;
				primitive.CalcBaryCentricCoord(target, &outS, &outT);
				if (primitive.IsInTriangle(outS, outT))
				{
					VertexToFragment v2f_draw;
					v2f_draw.position = primitive.GetFragmentPos(target, outS, outT);
					v2f_draw.color = primitive.GetFragmentColor(target, outS, outT);
					v2f_draw.uv = primitive.GetFragmentUV(target, outS, outT);
					ULONG finalColor = FragmentShader(v2f_draw, outS, outT);
					g_CurrentColor = finalColor;
					PutPixel(IntPoint(x, y));
				}
			}
		}
	}
}

VertexToFragment VertexShader(VertexDATA in)
{
	//TRS Matrix 반영해서 변환된 position 돌려주기
	VertexToFragment v2f;

	static float offsetX = 0.0f;
	static float angle = 50.0f;
	static float scale = 1.0f;

	Matrix3 TMat, RMat, SMat;
	TMat.SetTranslation(offsetX, 0.0f);
	RMat.SetRotation(angle);
	SMat.SetScale(scale);
	Matrix3 TRSMat = TMat * RMat * SMat;

	v2f.position = in.position*TRSMat;
	v2f.color = in.color;
	v2f.uv = in.uv;

	return v2f;
}

ULONG FragmentShader(VertexToFragment in, float s, float t)
{
	//Texture에서 색상 빼오기
	if (g_Texture->IsLoaded())
	{
		return g_Texture->TextureSample(in.uv, s, t);
	}
	else
		return in.color;
}

void InitFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();
}

void UpdateFrame(void)
{
	InitFrame();

	// Draw
	//UV
	Vector2 UV1(0.125f, 0.125f),
		UV2(0.25f, 0.125f),
		UV3(0.125f, 0.25f),
		UV4(0.25f, 0.125f),
		UV5(0.125f, 0.25f),
		UV6(0.25f, 0.25f);

	//1st Sprite
	Vector3 Pt1, Pt2, Pt3, Pt4, Pt5, Pt6;
	Pt1.SetPoint(-150, 150.0f);
	Pt2.SetPoint(150.0f, 150.0f);
	Pt3.SetPoint(-150.0f, -150.0f);
	Pt4.SetPoint(150.0f, 150.0f);
	Pt5.SetPoint(-150.0f, -150.0f);
	Pt6.SetPoint(150.0f, -150.0f);

	Vertex *vertices = new Vertex[6]();

	vertices[0] = Vertex(Pt1, UV1);
	vertices[1] = Vertex(Pt2, UV2);
	vertices[2] = Vertex(Pt3, UV3);
	vertices[3] = Vertex(Pt4, UV4);
	vertices[4] = Vertex(Pt5, UV5);
	vertices[5] = Vertex(Pt6, UV6);	

	Mesh *m = new Mesh();
	m->SetVertetices(vertices,6);
	m->setIndices();

	//2nd Sprite
	Vector3 Pt7, Pt8, Pt9;

	Pt7.SetPoint(-200.0f, 100.0f);
	Pt8.SetPoint(250.0f, 50.0f);
	Pt9.SetPoint(150.0f, -50.0f);

	Vertex *vertices2 = new Vertex[3]();

	vertices2[0] = Vertex(Pt7, UV4);
	vertices2[1] = Vertex(Pt8, UV5);
	vertices2[2] = Vertex(Pt9, UV6);


	Mesh *m2 = new Mesh();
	m2->SetVertetices(vertices2, 3);
	m2->setIndices();


	//레이어 지정
	int LayerCount = LAYERSIZE;
	Mesh *meshes = new Mesh[LayerCount]();

	meshes[0] = *m;
	meshes[1] = *m2;

	for (int i = 0; i < LayerCount; i++)
	{
		DrawCall(&meshes[i]);
	}

	// Buffer Swap 
	BufferSwap();
}
