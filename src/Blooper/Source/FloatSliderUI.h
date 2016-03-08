/*
  ==============================================================================

    FloatSliderUI.h
    Created: 8 Mar 2016 3:46:27pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef FLOATSLIDERUI_H_INCLUDED
#define FLOATSLIDERUI_H_INCLUDED

#include "ParameterUI.h"
#include "FloatParameter.h"

class FloatSliderUI    : public ParameterUI, public Component
{
public:
    FloatSliderUI(Parameter * parameter = nullptr);
    ~FloatSliderUI();

	enum Direction { HORIZONTAL, VERTICAL };

	FloatParameter * floatParam;

	//settings
	Direction orientation;
	bool changeParamOnMouseUpOnly;
	bool assignOnMousePosDirect;

	//interaction
	float initValue; 

	void paint(Graphics &g) override;
	void mouseDown(const MouseEvent &e) override;
	void mouseDrag(const MouseEvent &e) override;
	void mouseUp(const MouseEvent &e) override;

	float getValueFromMouse();
	float getValueFromPosition(const Point<int> &pos);

	virtual void setParamNormalizedValue(float value);
	virtual float getParamNormalizedValue();

protected:
	void parameterValueChanged(Parameter *) override { repaint(); };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FloatSliderUI)
};


#endif  // FLOATSLIDERUI_H_INCLUDED
