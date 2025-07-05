#include "TexturedMesh.h"

TexturedMesh::TexturedMesh(
	std::vector<Vec3> _vertexList,
	std::vector<Line> _lineList,
	std::vector<UVFace> _UVFaceList,
	std::vector<Material> _materialList)
	:vertexList(_vertexList)
	, lineList(_lineList)
	, UVFaceList(_UVFaceList)
	, materialList(_materialList)
{}
