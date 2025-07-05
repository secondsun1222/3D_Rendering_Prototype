#pragma once

#include "Mesh.h"
#include "Vec2.h"

struct UVFace { int texture; int v1, v2, v3; Vec2 v1UV, v2UV, v3UV; };
struct Material { WCHAR* texture; double shine; };

class TexturedMesh
{
public:
	std::vector<Vec3>			vertexList;
	std::vector<Line>			lineList;
	std::vector<UVFace>			UVFaceList;

	std::vector<Material>	materialList;

public:
	TexturedMesh(
		std::vector<Vec3>, 
		std::vector<Line>,
		std::vector<UVFace>,
		std::vector<Material>
	);
};