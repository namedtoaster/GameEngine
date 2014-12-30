#include "Mesh.h"
#include "ResourceManager.h"

Mesh::Mesh(PLYResource* pPly) : m_mode(GL_TRIANGLES), m_count(6), m_type(GL_UNSIGNED_SHORT)
{
	const std::vector<unsigned short>& faces = pPly->GetFaces();

	m_buffer.reset(new VertexBuffer(pPly->GetVertices().data(),
									pPly->GetVertexStructureSize(),
									pPly->GetNumVertices(),
									GL_STATIC_DRAW,
									faces.data(), faces.size()));
}

void Mesh::Bind()
{
	m_buffer->BindVAO();
}

void Mesh::Draw() const
{
	glDrawElements(m_mode, m_count, m_type, 0);
}
