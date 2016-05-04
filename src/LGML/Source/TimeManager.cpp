/*
 ==============================================================================

 TimeManager.cpp
 Created: 2 Mar 2016 8:33:44pm
 Author:  bkupe

 ==============================================================================
 */

#include "TimeManager.h"


juce_ImplementSingleton(TimeManager);

#include "NodeBase.h"


TimeManager::TimeManager():
timeInSample(0),
playState(false),
beatTimeInSample(22050),
sampleRate(44100),
isSettingTempo(false),
ControllableContainer("time")
{

    BPM = addFloatParameter("bpm","current BPM",120,10,600);
    playState = addBoolParameter("Play_Stop", "play or stop global transport", false);
    isSettingTempo = addBoolParameter("isSettingTempo", "is someone setting tempo (recording first loop)", false);
    currentBar  = addIntParameter("currentBar", "currentBar in transport", 0, 0, 4);
    currentBeat  = addIntParameter("currentBeat", "currentBeat in transport", 0, 0, 4);
    beatPerBar = addIntParameter("beatPerBar", "beat Per Bar", 4, 1, 8);
    playTrigger = addTrigger("play", "trigger play");
    stopTrigger = addTrigger("stop", "trigger stop");
    quantizedBarFraction = addIntParameter("globalQuantization", "Global quantization in fraction of a bar", 1, 0, 16);
}
TimeManager::~TimeManager()
{
}


void TimeManager::incrementClock(int time){
    int lastBeat = getBeat();

    if(playState->boolValue()){
        timeInSample+=time;
    }
    int newBeat = getBeat();
    if(lastBeat!=newBeat){
        currentBeat->setValue(newBeat);
        if(newBeat%((int)beatPerBar->value) == 0){
            currentBar->setValue(getBar());

        }
    }
}


void TimeManager::audioDeviceIOCallback (const float** /*inputChannelData*/,
                                         int /*numInputChannels*/,
                                         float** outputChannelData,
                                         int numOutputChannels,
                                         int numSamples) {
    incrementClock(numSamples);

    for (int i = 0; i < numOutputChannels; ++i)
        zeromem (outputChannelData[i], sizeof (float) * (size_t) numSamples);
}

bool TimeManager::askForBeingMasterNode(NodeBase * n){
    potentialTimeMasterNode.addIfNotAlreadyThere(n);
    return potentialTimeMasterNode.size()==1;
}

bool TimeManager::isMasterNode(NodeBase * n){
    return potentialTimeMasterNode.size()>0 && n==potentialTimeMasterNode[0];
}
bool TimeManager::hasMasterNode(){
    return potentialTimeMasterNode.size()>0;
}
void TimeManager::releaseMasterNode(NodeBase * n){
    potentialTimeMasterNode.removeFirstMatchingValue(n);
    if(potentialTimeMasterNode.size()==0){stopTrigger->trigger();}
}

void TimeManager::onContainerParameterChanged(Parameter * p){
    if(p==playState){
        if(!playState->boolValue()){
            timeInSample = 0;
        }
        else{}
    }
    else if(p==BPM){
        setBPMInternal(BPM->floatValue());
    }
    else if(p==beatPerBar){
        currentBeat->maximumValue = beatPerBar->intValue();
    }

};

void TimeManager::onContainerTriggerTriggered(Trigger * t) {
    if(t == playTrigger){
        playState->setValue(false);
        playState->setValue(true);
    }

    if(t==stopTrigger){
        playState->setValue(false);
    }
}


void TimeManager::setSampleRate(int sr){
    sampleRate = sr;
    // actualize beatTime in sample
    beatTimeInSample = (int)(sampleRate*60.0f / (float)BPM->value);
}

void TimeManager::setBPMInternal(double){
    isSettingTempo->setValue(false);
    beatTimeInSample =(int)(sampleRate*60.0f / (float)BPM->value);
    timeInSample = 0;
}



int TimeManager::setBPMForLoopLength(int time){
    double time_seconds = time* 1.0/ sampleRate;
    double beatTime = time_seconds* 1.0/beatPerBar->intValue();
    int barLength = 1;

    // over 150 bpm
    if(beatTime < .40){beatTime*=2;barLength/=2;}
    // under 60 bpm
    else if(beatTime > 1){beatTime/=2;barLength*=2;}

    BPM->setValue( 60.0/beatTime);
    return barLength;
}

int TimeManager::getNextGlobalQuantifiedTime(){
    return getNextQuantifiedTime(quantizedBarFraction->intValue());
}
int TimeManager::getNextQuantifiedTime(int barFraction){
    if(barFraction==0){
		return (int)timeInSample;
	}
    const int samplesPerUnit = (beatTimeInSample*beatPerBar->intValue()/barFraction);
    return (int) ((floor(timeInSample/samplesPerUnit) + 1)*samplesPerUnit);
}


int TimeManager::getBeat(){return (int)(floor(timeInSample*1.0/beatTimeInSample));}
double TimeManager::getBeatPercent(){return timeInSample*1.0/beatTimeInSample-getBeat();}

int TimeManager::getBar(){return (int)(floor(getBeat()*1.0/beatPerBar->intValue() ));}