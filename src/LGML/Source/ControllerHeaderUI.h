/*
  ==============================================================================

    ControllerHeaderUI.h
    Created: 8 Mar 2016 10:48:47pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef CONTROLLERHEADERUI_H_INCLUDED
#define CONTROLLERHEADERUI_H_INCLUDED


#include "ControllerUI.h"

#include "BoolToggleUI.h"
#include "StringParameterUI.h"

class ControllerHeaderUI : public Component
{
public:
    ControllerHeaderUI();
    virtual ~ControllerHeaderUI();

    Controller * controller;
    ControllerUI * cui;

    ScopedPointer<StringParameterUI> titleUI;
    ScopedPointer<BoolToggleUI> enabledUI;

    void resized();

    void setControllerAndUI(Controller * controller, ControllerUI * cui);
    virtual void init(); //override for proper init with controller and ui

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ControllerHeaderUI)
};


#endif  // CONTROLLERHEADERUI_H_INCLUDED