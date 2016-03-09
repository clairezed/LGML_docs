/*
  ==============================================================================

    NodeBaseHeaderUI.cpp
    Created: 8 Mar 2016 5:53:52pm
    Author:  bkupe

  ==============================================================================
*/

#include "NodeBaseHeaderUI.h"

NodeBaseHeaderUI::NodeBaseHeaderUI()
  {
	  node = nullptr;
	  nodeUI = nullptr;

	  setSize(20, 30);
  }

  NodeBaseHeaderUI::~NodeBaseHeaderUI()
  {
	  if (node && node->hasAudioOutputs) {
		  node->audioProcessor->removeListener(&vuMeter);
	  }
  }

  void NodeBaseHeaderUI::setNodeAndNodeUI(NodeBase * node, NodeBaseUI * nodeUI)
{
	this->node = node;
	this->nodeUI = nodeUI;

	if (node != nullptr && node->hasAudioOutputs) {
		node->audioProcessor->addListener(&vuMeter);
		addAndMakeVisible(vuMeter);
	}

	init();
}

  void NodeBaseHeaderUI::init()
  {
	  //to override

	  titleUI = node->nameParam->getUI();
	  addAndMakeVisible(titleUI);

	  enabledUI = node->enabledParam->createToggle();
	  addAndMakeVisible(enabledUI);
  }

void NodeBaseHeaderUI::resized()
{
	if (enabledUI != nullptr)
	{
		Rectangle<int> r = getLocalBounds();
		r.reduce(5, 2);
		r.removeFromLeft(enabledUI->getWidth());
		r.removeFromRight(100);
		titleUI->setBounds(r);
		enabledUI->setTopLeftPosition(5, 5);
	}

	if (node && node->hasAudioOutputs) {
		Rectangle<int> vuMeterRect = getLocalBounds().removeFromRight(14).reduced(4);
		vuMeter.setBounds(vuMeterRect);
	}
}
