/*
 ==============================================================================

 This file was auto-generated!

 ==============================================================================
 */

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED



#include "NodeManager.h"
#include "NodeManagerUI.h"

#include "ControlManager.h"
#include "ControllerManagerUI.h"
#include "ControllableInspector.h"

#include "TimeManagerUI.h"

#include "RuleManager.h"
#include "RuleManagerUI.h"

#include "ShapeShifterManager.h"

#include "Engine.h"

#include "LookAndFeelOO.h"
//==============================================================================
/*
 This component lives inside our window, and this is where you should put all
 your controls and content.
 */

ApplicationCommandManager& getCommandManager();
ApplicationProperties& getAppProperties();
AudioDeviceManager& getAudioDeviceManager();


class MainContentComponent   : public Component,public ApplicationCommandTarget,public MenuBarModel

{
public:

    TooltipWindow tooltipWindow; // to add tooltips to an application, you
    // just need to create one of these and leave it
    // there to do its work..

	ShapeShifterManager shapeShifterManager;

	ScopedPointer<TimeManagerUI> timeManagerUI;

	ScopedPointer<NodeManagerUI> nodeManagerUI;
	
	ScopedPointer<ControllerManagerUI> controllerManagerUI;

	ScopedPointer<RuleManagerUI> ruleManagerUI;


	ScopedPointer<ControllableInspector> controllableInspector;
	static SelectableComponentHandler mainSelectableHandler;


    Engine * engine;
    AudioDeviceSelectorComponent audioSettingsComp;

    ScopedPointer<LookAndFeelOO> lookAndFeelOO;

    //==============================================================================
    MainContentComponent(Engine * e);
    ~MainContentComponent();

    //==============================================================================
    // see MainComponent.cpp

    //==============================================================================
    void paint (Graphics& g) override{
		g.fillAll (BG_COLOR.darker());
	}
    void resized() override;


    void showAudioSettings();


    //==============================================================================
    // see MainComponentCommands.cpp

    // inherited from MenuBarModel , ApplicationCommandTarget
    ApplicationCommandTarget* getNextCommandTarget() override {return findFirstTargetParentComponent();}
    void getAllCommands (Array<CommandID>& commands) override;
    virtual void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result) override ;
    virtual bool perform (const InvocationInfo& info) override ;
    StringArray getMenuBarNames() override ;
    virtual PopupMenu getMenuForIndex (int topLevelMenuIndex,const String& menuName) override;
    void menuItemSelected (int /*menuItemID*/,int /*topLevelMenuIndex*/) override{}



private:
    //==============================================================================

    // Your private member variables go here...

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};





#endif  // MAINCOMPONENT_H_INCLUDED
