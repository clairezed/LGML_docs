/*
 ==============================================================================

 VSTNodeUI.cpp
 Created: 24 Mar 2016 9:44:38pm
 Author:  Martin Hermant

 ==============================================================================
 */

#include "VSTNodeUI.h"


VSTNodeUI::VSTNodeUI(VSTNode * _owner):
VSTListShowButton("VSTs"),
showPluginWindowButton("showWindow"),
owner(_owner){
    owner->addChangeListener(this);
    owner->addControllableContainerListener(this);
}
VSTNodeUI::~VSTNodeUI(){
    owner->removeChangeListener(this);
    owner->removeControllableContainerListener(this);

}

void VSTNodeUI::init() {
    VSTListShowButton.addListener(this);
    showPluginWindowButton.addListener(this);
    addAndMakeVisible(showPluginWindowButton);
    addAndMakeVisible(VSTListShowButton);
    setSize(200, 100);
    updateVSTParameters();

}

void VSTNodeUI::updateVSTParameters(){
    paramSliders.clear();

    int maxParameter = 20;
    int pCount = 0;

    for(auto &p:owner->VSTParameters){
        FloatSliderUI * slider = p->createSlider();
        paramSliders.add(slider);
        addAndMakeVisible(slider);
        pCount++;
        if(pCount>maxParameter){
            break;
        }
    }

    resized();
}

void VSTNodeUI::controllableAdded(Controllable *) {};
void VSTNodeUI::controllableRemoved(Controllable * c){
    for(auto &p:paramSliders){
        if(p->floatParam == c){
            removeChildComponent (p);
            paramSliders.removeObject(p);
            break;
        }
    };
}
void VSTNodeUI::controllableContainerAdded(ControllableContainer *){};
void VSTNodeUI::controllableContainerRemoved(ControllableContainer *) {};
void VSTNodeUI::controllableFeedbackUpdate(Controllable *) {};



void VSTNodeUI::changeListenerCallback(ChangeBroadcaster * c) {
    if(c == owner){
        updateVSTParameters();
    }
}

void VSTNodeUI::resized(){
    Rectangle<int> area = getLocalBounds();
    Rectangle<int> headerArea = area.removeFromTop(40);
    VSTListShowButton.setBounds(headerArea.removeFromLeft(headerArea.getWidth()/2));
    showPluginWindowButton.setBounds(headerArea);
    layoutSliderParameters(area.reduced(2));

}

void VSTNodeUI::layoutSliderParameters(Rectangle<int> pArea){
    if(paramSliders.size() == 0) return;
    int maxLines = 4;

    int numLines = jmin(maxLines,paramSliders.size());
    int numCols = (paramSliders.size()-1)/maxLines + 1;

    int w = pArea.getWidth()/numCols;
    int h =pArea.getHeight()/numLines;
    int idx = 0;
    for(int i = 0 ; i < numCols ; i ++){
        Rectangle<int> col = pArea.removeFromLeft(w);
        for(int j = 0 ; j < numLines ; j++){
            paramSliders.getUnchecked(idx)->setBounds(col.removeFromTop(h).reduced(1));
            idx++;
            if(idx>=paramSliders.size()){
                break;
            }
        }
        if(idx>=paramSliders.size()){
            break;
        }
    }
}


void VSTNodeUI::vstSelected (int modalResult, Component *  originComp)
{
    int index = VSTManager::getInstance()->knownPluginList.getIndexChosenByMenu(modalResult);
    if(index>=0 ){
        VSTNodeUI * originVSTNodeUI =  dynamic_cast<VSTNodeUI*>(originComp);
        if(originVSTNodeUI){
            originVSTNodeUI->owner->identifierString->setValue(VSTManager::getInstance()->knownPluginList.getType (index)->createIdentifierString());
//            originVSTNodeUI->owner->generatePluginFromDescription(VSTManager::getInstance()->knownPluginList.getType (index));
        }
    }
}

void VSTNodeUI::buttonClicked (Button* button)
{
    if (button == &VSTListShowButton){
        PopupMenu  VSTList;
        VSTManager::getInstance()->knownPluginList.addToMenu(VSTList, KnownPluginList::SortMethod::sortByCategory);
        owner->closePluginWindow();
        VSTList.showAt (&VSTListShowButton,0,0,0,0, ModalCallbackFunction::forComponent(&VSTNodeUI::vstSelected, (Component*)this));

    }
    if(button == &showPluginWindowButton){
        owner->createPluginWindow();
    }
}