/*
  ==============================================================================

    AudioInNode.h
    Created: 7 Mar 2016 8:03:48pm
    Author:  Martin Hermant

  ==============================================================================
*/

#ifndef AUDIOINNODE_H_INCLUDED
#define AUDIOINNODE_H_INCLUDED



#include "NodeBase.h"

AudioDeviceManager & getAudioDeviceManager() ;

class AudioInNode : public NodeBase
{

public:


    class AudioInProcessor : public juce::AudioProcessorGraph::AudioGraphIOProcessor, public NodeAudioProcessor,public ChangeListener
    {
    public:
        AudioInProcessor():
        NodeAudioProcessor("AudioIn"),
        AudioGraphIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::IODeviceType::audioInputNode)
        {
        NodeAudioProcessor::busArrangement.inputBuses.clear();

        }

        ~AudioInProcessor(){


        }

        void changeListenerCallback (ChangeBroadcaster* source)override;
        void updateIO();
        void processBlockInternal(AudioBuffer<float>& buffer,MidiBuffer& midiMessages)override {
            AudioProcessorGraph::AudioGraphIOProcessor::processBlock(buffer, midiMessages);

        }


        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioInProcessor)
    };







    AudioInNode(NodeManager * nodeManager,uint32 nodeId)  :
    NodeBase(nodeManager,nodeId,"AudioInNode",new AudioInProcessor){
        AudioInProcessor * ap = dynamic_cast<AudioInProcessor*>(audioProcessor);
        getAudioDeviceManager().addChangeListener(ap);
        ap->updateIO();
    };
    ~AudioInNode(){getAudioDeviceManager().removeChangeListener(dynamic_cast<AudioInProcessor*>(audioProcessor));};

    virtual NodeBaseUI * createUI() override;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioInNode)
};







#endif  // AUDIOINNODE_H_INCLUDED