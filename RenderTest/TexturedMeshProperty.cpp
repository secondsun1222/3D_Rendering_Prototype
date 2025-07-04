#include "TexturedMeshProperty.h"

TexturedMeshProperty::TexturedMeshProperty(TexturedMesh* _mesh)
	:Property(PropertyType::TEXTURED_MESH)
	, mesh(_mesh)
{
}
