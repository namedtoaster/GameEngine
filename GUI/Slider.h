#ifndef _SLIDER_
#define _SLIDER_

#include "IElement.h"
#include "IInput.h"
#include "IRenderer.h"
#include <string>
#include <glm/glm.hpp>

#include <functional>

namespace UI
{

// A UI controlled slider via the mouse. Whenever the user changes the value, 
// the callback gets called with the value
class Slider : public IElement
{
public:

	typedef std::function<void(float)> DELEGATE;

	// Slider ctor
	// start: starting point of the Slider
	// end: ending point of the Slider
	// min: min value of the Slider
	// max: max value of the Slider
	// tex: texture of the ball on the Slider line that displays the current value
	// callback: the function that gets called whenever there is a change in value of the Slider
	Slider(const glm::vec2& start,
		   const glm::vec2& end,
		   float min, float max, const std::string& tex, const DELEGATE& callback);

	void Select() override;
	void Deselect() override;
	void Trigger() override;

	void Update(class IInput&, double dt) override;
	void Render(class IRenderer&) override;

	// returns the current value of the slider
	float GetValue() const;

private:

	virtual ~Slider() {}

	std::string m_SpriteTexture;
	DELEGATE m_callback;
	glm::vec2 m_pos;
	glm::vec2 m_start;
	glm::vec2 m_end;
	float m_fPercentage;
	float m_fMin;
	float m_fMax;
	bool m_bEnable;
	bool m_bUpdateEnable;

};

}

#endif // _SLIDER_