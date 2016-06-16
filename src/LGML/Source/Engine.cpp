/*
 ==============================================================================

 Engine.cpp
 Created: 2 Apr 2016 11:03:21am
 Author:  Martin Hermant

 ==============================================================================
 */

#include "Engine.h"
#include "SerialManager.h"
#include "LGMLLogger.h"
#include "MainComponent.h"
#include "DebugHelpers.h"

const char* const filenameSuffix = ".lgml";
const char* const filenameWildcard = "*.lgml";

Engine::Engine():FileBasedDocument (filenameSuffix,
filenameWildcard,
                                    "Load a filter graph",
                                    "Save a filter graph"){
    initAudio();
    Logger::setCurrentLogger (LGMLLogger::getInstance());

    MIDIManager::getInstance()->init();
    SerialManager::getInstance()->init();
}


Engine::~Engine(){
    closeAudio();

    FastMapper::deleteInstance();
    TimeManager::deleteInstance(); //TO PREVENT LEAK OF SINGLETON
    ControllerManager::deleteInstance();
    NodeManager::deleteInstance();
    VSTManager::deleteInstance();
    PresetManager::deleteInstance();
    ControllerManager::deleteInstance();
    JsGlobalEnvironment::deleteInstance();
    Logger::setCurrentLogger(nullptr);
    LGMLLogger::deleteInstance();
    RuleManager::deleteInstance();

    MIDIManager::deleteInstance();

    SerialManager::deleteInstance();

}

void Engine::parseCommandline(const String & commandLine){

    StringArray args;
    args.addTokens (commandLine, true);
    args.trim();

    int parsingIdx=0;
    while(parsingIdx<args.size())
    {
        String command = "";
        bool isParameter = args[parsingIdx].startsWith("-");
        if(isParameter){
            command = args[parsingIdx].substring(1, args[parsingIdx].length());
            parsingIdx++;
            if(parsingIdx>=args.size()){break;}
        }

        String argument = args[parsingIdx].removeCharacters(juce::StringRef("\""));

        //DBG("parsing commandline, command : " << command << ", argument :" << argument << " / parsingIdx : " << parsingIdx);

        if(command== "f"|| parsingIdx==0){
            if (File::isAbsolutePath(argument)) {
                File f(argument);
                if (f.existsAsFile()) loadDocument(f);
            }
            else {
                NLOG("Engine","File : " << argument << " not found.");
            }
        }


        parsingIdx++;
    }

}


void Engine::initAudio(){

    graphPlayer.setProcessor(&NodeManager::getInstance()->audioGraph);
    ScopedPointer<XmlElement> savedAudioState (getAppProperties().getUserSettings()->getXmlValue ("audioDeviceState"));
    getAudioDeviceManager().initialise (64, 64, savedAudioState, true);
    getAudioDeviceManager().addChangeListener(&audioSettingsHandler);
    getAudioDeviceManager().addAudioCallback (&graphPlayer);
    getAudioDeviceManager().addAudioCallback(TimeManager::getInstance());
    DBG("init audio");

}


void Engine::suspendAudio(bool shouldBeSuspended){
    if(AudioProcessor * ap =graphPlayer.getCurrentProcessor())
        ap->suspendProcessing (shouldBeSuspended);

    TimeManager::getInstance()->lockTime(shouldBeSuspended);

}

void Engine::closeAudio(){
    getAudioDeviceManager().removeAudioCallback (&graphPlayer);
    getAudioDeviceManager().removeAudioCallback(TimeManager::getInstance());
    getAudioDeviceManager().closeAudioDevice();
}


void Engine::clear(){
    //    do we need to stop audio?
    //stopAudio();

    TimeManager::getInstance()->playState->setValue(false);

    FastMapper::getInstance()->clear();
    RuleManager::getInstance()->clear();
    ControllerManager::getInstance()->clear();
    NodeManager::getInstance()->clear();
    PresetManager::getInstance()->clear();


    changed();    //fileDocument
}

void Engine::stimulateAudio( bool s){
    if(s){
        stimulator= new AudioFucker(&getAudioDeviceManager());
        getAudioDeviceManager().addAudioCallback(stimulator);

    }
    else{
        getAudioDeviceManager().removeAudioCallback(stimulator);
        stimulator=nullptr;
    }

}

void Engine::createNewGraph(){
    clear();
    suspendAudio(true);
    ConnectableNode * node = NodeManager::getInstance()->mainContainer->addNode(NodeType::AudioDeviceInType);
    node->xPosition->setValue(150);
    node->yPosition->setValue(100);
    node = NodeManager::getInstance()->mainContainer->addNode(NodeType::AudioDeviceOutType);
    node->xPosition->setValue(450);
    node->yPosition->setValue(100);
    suspendAudio(false);
    changed();
}


void Engine::MultipleAudioSettingsHandler::changeListenerCallback(ChangeBroadcaster *){
    String configName = getConfigName();
    if(lastConfigName == configName){return;}
    ScopedPointer<XmlElement> oldSetupXml = getAppProperties().getUserSettings()->getXmlValue(oldSettingsId);
    if(!oldSetupXml)return;

    XmlElement * xml = oldSetupXml->getChildByName(configName);
    lastConfigName = configName;
    if(xml!=nullptr){
            getAudioDeviceManager().initialise(64, 64, xml->getChildElement(0), true);
    }


}

String Engine::MultipleAudioSettingsHandler::getConfigName(){
    juce::AudioDeviceManager::AudioDeviceSetup result;
    getAudioDeviceManager().getAudioDeviceSetup(result);
    String idealName = result.inputDeviceName+"_"+result.outputDeviceName;
    String escaped = StringUtil::toShortName(idealName);
    return escaped;


}


void Engine::MultipleAudioSettingsHandler::saveCurrent(){
    ScopedPointer<XmlElement> audioState (getAudioDeviceManager().createStateXml());
    getAppProperties().getUserSettings()->setValue ("audioDeviceState", audioState);
    ScopedPointer<XmlElement> oldXml = getAppProperties().getUserSettings()->getXmlValue(oldSettingsId);
    if(!oldXml){
        oldXml = new XmlElement(oldSettingsId);

    }
    
    String configName = getConfigName();

    XmlElement * oldConfig = oldXml->getChildByName(configName);

    if(oldConfig){
        oldXml->removeChildElement(oldConfig, true);
    }
    oldConfig = oldXml->createNewChildElement(configName) ;
    oldConfig->addChildElement(getAudioDeviceManager().createStateXml());


    
    getAppProperties().getUserSettings()->setValue(oldSettingsId.toString(), oldXml);
    getAppProperties().getUserSettings()->saveIfNeeded();
    
}