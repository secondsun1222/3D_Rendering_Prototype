#pragma once
#include "pch.h"
#include "Vec3.h"

struct Line { int v1, v2; };
struct Face { int v1, v2, v3; };

class Mesh
{
public:
	std::vector<Vec3> vertexList;
	std::vector<Line> lineList;
	std::vector<Face> faceList;

public:
	Mesh();
	Mesh(Vec3* _vertex_arr, int _vertex_cnt,
		Face* _face_arr, int _face_cnt);
	Mesh(Vec3* _vertex_arr, int _vertex_cnt,
		Line* _line_arr, int _line_cnt,
		Face* _face_arr, int _face_cnt);
	Mesh(std::vector<Vec3>, std::vector<Line>, std::vector<Face>);
	~Mesh();
};