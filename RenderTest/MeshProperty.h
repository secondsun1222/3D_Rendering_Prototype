#pragma once
#include "PropertyBase.h"
#include "Mesh.h"

class MeshProperty : public Property {
public:
	Mesh* mesh;
	
public:
	MeshProperty(Mesh*);
};