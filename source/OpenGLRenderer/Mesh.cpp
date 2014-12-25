#include "Mesh.h"
#include "ResourceManager.h"

Mesh::Mesh(PLYResource* pPly) : m_mode(GL_TRIANGLES), m_count(6), m_type(GL_UNSIGNED_BYTE)
{
	/*unsigned char indexBuffer[6] = { 0, 2, 1,	2, 3, 1 };
	VertexPT verticies[] =
	{
		{glm::vec3(-0.586330, -0.891078, -0.145187), glm::vec2(0)},
		{glm::vec3(1.413670, -0.891078, -0.145187), glm::vec2(0,1)},
		{glm::vec3(1.413670, 1.108922, -0.145187), glm::vec2(1,0)},
		{glm::vec3(-0.586330, 1.108922, -0.145187), glm::vec2(1)}
	};*/

	const std::vector<int>& faces = pPly->GetFaces();

	m_buffer.reset(new VertexBuffer(pPly->GetVertices().data(),
									pPly->GetVertexStructureSize(),
									pPly->GetNumVertices(),
									GL_STATIC_DRAW,

									// todo: remove this cast, this is just here for the code to compile
									(const unsigned char*)faces.data(), faces.size()));
}

void Mesh::Bind()
{
	m_buffer->BindVAO();
}

void Mesh::Draw() const
{
	glDrawElements(m_mode, m_count, m_type, 0);
}
