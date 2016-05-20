/*
  ==============================================================================

    ConnectableNodeUI.h
    Created: 18 May 2016 11:34:23pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef CONNECTABLENODEUI_H_INCLUDED
#define CONNECTABLENODEUI_H_INCLUDED

#include "InspectableComponent.h"
#include "ConnectableNode.h"
#include "ConnectorComponent.h"

#include "ConnectableNodeHeaderUI.h"
#include "ConnectableNodeContentUI.h"
#include "ConnectableNodeAudioCtlUI.h"


class ConnectableNodeUI : 
	public InspectableComponent,
	public Parameter::Listener
{
public:
	ConnectableNodeUI(ConnectableNode * cn, ConnectableNodeContentUI * contentUI = nullptr, ConnectableNodeHeaderUI * headerUI = nullptr);
	virtual ~ConnectableNodeUI();

	ConnectableNode * connectableNode;


	class ConnectorContainer : public Component
	{
	public:
		OwnedArray<ConnectorComponent> connectors;

		ConnectorComponent::ConnectorDisplayLevel displayLevel;
		ConnectorComponent::ConnectorIOType type;

		ConnectorContainer(ConnectorComponent::ConnectorIOType type);



		void setConnectorsFromNode(ConnectableNode * node);
		void addConnector(ConnectorComponent::ConnectorIOType ioType, NodeConnection::ConnectionType dataType, ConnectableNode * node);
		void resized();

		ConnectorComponent * getFirstConnector(NodeConnection::ConnectionType dataType);



		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConnectorContainer)
	};

	//ui
	class MainContainer : public Component
	{
	public:

		//ui y
		MainContainer(ConnectableNodeUI * nodeUI, ConnectableNodeContentUI * content = nullptr, ConnectableNodeHeaderUI * header = nullptr);

		//reference
		ConnectableNodeUI * connectableNodeUI;

		//containers
		ScopedPointer<ConnectableNodeHeaderUI> headerContainer;
		ScopedPointer<ConnectableNodeContentUI> contentContainer;
		ScopedPointer<ConnectableNodeAudioCtlUI> audioCtlUIContainer;

		const int audioCtlContainerPadRight = 3;

		void setNodeAndNodeUI(ConnectableNode * node, ConnectableNodeUI * nodeUI);
		void paint(Graphics &g) override;
		void resized() override;
		void childBoundsChanged(Component*)override;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContainer)
	};

	

	//layout
	int connectorWidth;

	//interaction
	Point<int> nodeInitPos;
	bool dragIsLocked;


	MainContainer mainContainer;
	ConnectableNodeContentUI * getContentContainer() { return mainContainer.contentContainer; }
	ConnectableNodeHeaderUI * getHeaderContainer() { return mainContainer.headerContainer; }
	
	ConnectorContainer inputContainer;
	ConnectorContainer outputContainer;


	// receives x y position from node parameters
	void paint(Graphics&)override;
	void paintOverChildren(Graphics &) override {} //cancel default yellow border behavior

	void resized()override;


	

	Array<ConnectorComponent *> getComplementaryConnectors(ConnectorComponent * baseConnector); 

	//Need to clean out and decide whether there can be more than 1 data connector / audio connector on nodes
	ConnectorComponent * getFirstConnector(NodeConnection::ConnectionType connectionType, ConnectorComponent::ConnectorIOType ioType);

	 
	void childBoundsChanged(Component*)override;
	void parameterValueChanged(Parameter * p) override;

	void mouseDown(const MouseEvent &e) override;
	void mouseUp(const MouseEvent &e) override;
	void mouseDrag(const MouseEvent &e)  override;
	bool keyPressed(const KeyPress &key) override;
	void moved()override;

};


#endif  // CONNECTABLENODEUI_H_INCLUDED
