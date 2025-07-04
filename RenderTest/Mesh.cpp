#include "Mesh.h"



Mesh::Mesh()
{
}

Mesh::Mesh(Vec3* _vertex_arr, int _vertex_cnt,
	Face* _face_arr, int _face_cnt)
{
	for (int i = 0; i < _vertex_cnt; i++)
		vertexList.push_back(_vertex_arr[i]);

	for (int i = 0; i < _face_cnt; i++)
		faceList.push_back(_face_arr[i]);
}

Mesh::Mesh(Vec3* _vertex_arr, int _vertex_cnt,
	Line* _line_arr, int _line_cnt,
	Face* _face_arr, int _face_cnt)
{	
	for (int i = 0; i < _vertex_cnt; i++)
		vertexList.push_back(_vertex_arr[i]);

	for (int i = 0; i < _line_cnt; i++)
		lineList.push_back(_line_arr[i]);

	for (int i = 0; i < _face_cnt; i++)
		faceList.push_back(_face_arr[i]);
}

Mesh::Mesh(std::vector<Vec3> _vertexList, std::vector<Line> _lineList, std::vector<Face> _faceList)
	:vertexList(_vertexList)
	, lineList(_lineList)
	, faceList(_faceList)
{
}

Mesh::~Mesh()
{
}
