#include "ConnectorComponent.h"
#include "NodeContainerViewer.h"
#include "NodeFactory.h"
#include "Style.h"

ConnectorComponent::ConnectorComponent(ConnectorIOType _ioType, NodeConnection::ConnectionType _dataType, ConnectableNode * _cnode) :
    ioType(_ioType),
	dataType(_dataType),
	node((NodeBase *)_cnode),
	isHovered(false)
{
	jassert(node->type != NodeType::ContainerType);
	((NodeBase *)node)->addNodeBaseListener(this);



    boxColor = (dataType == NodeConnection::ConnectionType::AUDIO)? AUDIO_COLOR : DATA_COLOR;
    setSize(10,10);


    generateToolTip();

	updateVisibility();
}

ConnectorComponent::~ConnectorComponent(){
	 if(node.get())node->removeNodeBaseListener(this);
}

void ConnectorComponent::generateToolTip(){
    String tooltip;
    tooltip += dataType == NodeConnection::ConnectionType::AUDIO?"Audio\n":"Data\n";
    if (dataType == NodeConnection::ConnectionType::AUDIO)
    {
		bool isInput = ioType == ConnectorIOType::INPUT;
		AudioProcessorGraph::Node * tAudioNode = node->getAudioNode(isInput);
		if (tAudioNode != nullptr)
		{
			tooltip += isInput? tAudioNode->getProcessor()->getTotalNumInputChannels() : tAudioNode->getProcessor()->getTotalNumOutputChannels();
			tooltip += " channels";
		}
		else
		{
			tooltip = "[Error accessing audio processor]";
		}
    }
    else
    {
        StringArray dataInfos = ioType == ConnectorIOType::INPUT ? node->getInputDataInfos() : node->getOutputDataInfos();
        tooltip += dataInfos.joinIntoString("\n");
    }
      setTooltip(tooltip);
}
void ConnectorComponent::paint(Graphics & g)
{
    g.setGradientFill(ColourGradient(isHovered?boxColor.brighter(5.f):boxColor, (float)(getLocalBounds().getCentreY()),(float)(getLocalBounds().getCentreY()), boxColor.darker(), 0.f,0.f, true));
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 2);
}

void ConnectorComponent::mouseDown(const MouseEvent &)
{
    NodeContainerViewer * containerViewer = getNodeContainerViewer();

    if (dataType == NodeConnection::ConnectionType::DATA)
    {

		containerViewer->createDataConnectionFromConnector(this);
    }
    else
    {
		containerViewer->createAudioConnectionFromConnector(this);
    }
}

void ConnectorComponent::mouseEnter (const MouseEvent&){
    isHovered = true;
    repaint();
}
void ConnectorComponent::mouseExit  (const MouseEvent&){
    isHovered = false;
    repaint();
}

void ConnectorComponent::updateVisibility()
{

	bool isAudio = dataType == NodeConnection::ConnectionType::AUDIO;
	bool isInput = ioType == ConnectorIOType::INPUT;

	if (isAudio) setVisible(isInput ? node->hasAudioInputs() : node->hasAudioOutputs());
	else setVisible(isInput ? node->hasDataInputs() : node->hasDataOutputs());

	connectorListeners.call(&ConnectorListener::connectorVisibilityChanged, this);
}

void ConnectorComponent::numAudioInputChanged(NodeBase *, int)
{
	if (dataType != NodeConnection::ConnectionType::AUDIO || ioType != ConnectorIOType::INPUT) return;
	generateToolTip();
}

void ConnectorComponent::numAudioOutputChanged(NodeBase *, int)
{
	if (dataType != NodeConnection::ConnectionType::AUDIO || ioType != ConnectorIOType::OUTPUT) return;
	generateToolTip();
}

void ConnectorComponent::numDataInputChanged(NodeBase *, int)
{
	if (dataType != NodeConnection::ConnectionType::DATA || ioType != ConnectorIOType::INPUT) return;
	generateToolTip();
	updateVisibility();
}

void ConnectorComponent::numDataOutputChanged(NodeBase *, int)
{
	if (dataType != NodeConnection::ConnectionType::DATA || ioType != ConnectorIOType::OUTPUT) return;
	generateToolTip();
	updateVisibility();
}

NodeContainerViewer * ConnectorComponent::getNodeContainerViewer() const noexcept
{
    return findParentComponentOfClass<NodeContainerViewer>();
}

ConnectableNodeUI * ConnectorComponent::getNodeUI() const noexcept
{
    return findParentComponentOfClass<ConnectableNodeUI>();
}
