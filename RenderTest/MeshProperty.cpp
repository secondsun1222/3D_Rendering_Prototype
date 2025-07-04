#include "MeshProperty.h"

MeshProperty::MeshProperty(Mesh* _mesh)
	:Property(PropertyType::MESH)
	,mesh(_mesh)
{
}
