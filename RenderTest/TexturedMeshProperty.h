#pragma once
#include "PropertyBase.h"
#include "TexturedMesh.h"

class TexturedMeshProperty : public Property {
public:
	TexturedMesh* mesh;

public:
	TexturedMeshProperty(TexturedMesh*);
};