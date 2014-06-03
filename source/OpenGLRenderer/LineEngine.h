#ifndef _LINEENGINE_
#define _LINEENGINE_

#include "IRenderer.h"
#include "IRenderable.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "VertexBuffer.h"

// Manages the rendering of lines
class LineRenderer : public IRenderable
{
public:

	LineRenderer(const glm::vec3* pArray, unsigned int uiLength, float fWidth, const glm::vec4& color, const glm::mat4& T);

	virtual void Setup(ApplyShader& shader, const IResource* resource);
	virtual void Render(ApplyShader& shader, const IResource* resource);

private:

	std::vector<glm::vec3> m_line;
	float m_width;
	glm::vec4 m_color;
	glm::mat4 m_transformation;
};

#endif // _LINEENGINE_
