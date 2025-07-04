#include "TexturedMesh.h"

TexturedMesh::TexturedMesh(
	std::vector<Vec3> _vertexList,
	std::vector<Line> _lineList,
	std::vector<UVFace> _UVFaceList,
	std::vector<WCHAR*> _textureList)
	:vertexList(_vertexList)
	, lineList(_lineList)
	, UVFaceList(_UVFaceList)
	, textureList(_textureList)
{}
