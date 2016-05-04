/*
 ==============================================================================

 NodeBase.h
 Created: 2 Mar 2016 8:36:17pm
 Author:  bkupe

 ==============================================================================
 */

/*
 NodeBase is the base class for all Nodes
 it contains NodeAudioProcessor and/or NodeBase::NodeDataProcessor


 */
#ifndef NODEBASE_H_INCLUDED
#define NODEBASE_H_INCLUDED


#include "DataProcessor.h"
#include "NodeAudioProcessor.h"
#include "ControllableContainer.h"
#include "PresetManager.h"


class NodeBaseUI;
class NodeManager;


class NodeBase : public ReferenceCountedObject, public DataProcessor::Listener,public NodeAudioProcessor::NodeAudioProcessorListener, public ControllableContainer
{

public:
    class NodeDataProcessor : public DataProcessor
    {
    public:
        NodeDataProcessor() {};
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NodeDataProcessor)

    };

public:
    NodeBase(NodeManager * nodeManager,uint32 nodeId, const String &name = "[NodeBase]", NodeAudioProcessor * audioProcessor = nullptr, NodeBase::NodeDataProcessor * dataProcessor = nullptr);
    virtual ~NodeBase();

    uint32 nodeId;
    NodeManager * nodeManager;

    // owned by audio Graph in a refference Counted Array
    NodeAudioProcessor *  audioProcessor;

    //owned here, dataGraph only holds connections
    ScopedPointer<NodeDataProcessor> dataProcessor;

    bool hasAudioInputs;
    bool hasAudioOutputs;
    bool hasDataInputs;
    bool hasDataOutputs;

    void checkInputsAndOutputs();

    void remove(bool askBeforeRemove = false);


    //Controllables (from ControllableContainer)
    StringParameter * nameParam;
    BoolParameter * enabledParam;
    FloatParameter * xPosition;
    FloatParameter * yPosition;


    //audio
    void addToAudioGraphIfNeeded();
    void removeFromAudioGraphIfNeeded();


	virtual String getPresetFilter() override;

    //ui
    virtual NodeBaseUI *  createUI() {
        DBG("No implementation in child node class !");
        jassert(false);
        return nullptr;
    }

    // Inherited via DataProcessor::Listener
    virtual void inputAdded(Data *) override;
    virtual void inputRemoved(Data *) override;
    virtual void outputAdded(Data *) override;
    virtual void ouputRemoved(Data *) override;


    void numAudioInputChanged(int newNum)override;
    void numAudioOutputChanged(int newNum)override;


	var getJSONData() override;
	void loadJSONDataInternal(var data) override;


public:

    //Listener
    class NodeListener
    {
    public:
        virtual ~NodeListener() {}
		virtual void askForRemoveNode(NodeBase *) {}


    };

    ListenerList<NodeListener> nodeListeners;
    void addNodeListener(NodeListener* newListener) { nodeListeners.add(newListener); }
    void removeNodeListener(NodeListener* listener) { nodeListeners.remove(listener); }



private:

    int nodeTypeUID;
    friend class NodeFactory;

    // we use parameterValue changed her to allow Derived Node to implement onContainerParameterChanged without having to call NodeBase::onContainerParameterChanged, less error prone
    // @ben is it triggering error on your side?

    void parameterValueChanged(Parameter * p) override;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NodeBase)

};

#endif  // NODEBASE_H_INCLUDED