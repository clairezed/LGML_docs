/*
==============================================================================

ConnectorComponent.h
Created: 7 Mar 2016 11:48:35am
Author:  bkupe

==============================================================================
*/

#ifndef CONNECTORCOMPONENT_H_INCLUDED
#define CONNECTORCOMPONENT_H_INCLUDED

#include "JuceHeader.h"
#include "NodeBaseUI.h"
#include "NodeManager.h"

class NodeBaseUI;
class NodeManagerUI;
class NodeBase;

//==============================================================================
/*
*/
class ConnectorComponent : public Component, public SettableTooltipClient
{
public:

    enum ConnectorIOType
    {
        INPUT, OUTPUT
    };


    enum ConnectorDisplayLevel
    {
        MINIMAL, CONNECTED, ALL
    };


    NodeConnection::ConnectionType dataType;
    ConnectorIOType ioType;
    NodeBase * node;

    //layout

    //style
    Colour boxColor;
    bool isHovered;

    ConnectorComponent(ConnectorIOType ioType, NodeConnection::ConnectionType dataType, NodeBase * node);

    void paint(Graphics &g)override;

    void mouseDown(const MouseEvent &e) override;
    void mouseEnter (const MouseEvent&)override;
    void mouseExit  (const MouseEvent&)override;


    //void selectDataAndElementPopup(String &selectedDataName, String &selectedElementName, DataProcessor::DataType &selectedDataType, const DataProcessor::DataType &filterType = DataProcessor::DataType::Unknown);

    NodeManagerUI * getNodeManagerUI() const noexcept;
    NodeBaseUI * getNodeUI() const noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConnectorComponent)
};


#endif  // CONNECTORCOMPONENT_H_INCLUDED
