/*
  ==============================================================================

    VSTNode.cpp
    Created: 2 Mar 2016 8:37:24pm
    Author:  bkupe

  ==============================================================================
*/

#include "VSTNode.h"






#include "NodeBaseUI.h"
NodeBaseUI * VSTNode::createUI(){return new NodeBaseUI(this);}