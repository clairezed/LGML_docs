/*
  ==============================================================================

    PresetManager.h
    Created: 25 Apr 2016 2:02:05pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef PRESETMANAGER_H_INCLUDED
#define PRESETMANAGER_H_INCLUDED

#include "JuceHeader.h"

class ControllableContainer;

enum PresetChoice
{
    SaveCurrent = -3,
    SaveToNew = -2,
    ResetToDefault = -1
};

class PresetManager
{
public:
    class PresetValue
    {
    public:
        PresetValue(const String &_controlAddress, var _value) : paramControlAddress(_controlAddress), presetValue(_value) {}
        String paramControlAddress;
        var presetValue;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetValue)
    };

    class Preset
    {
    public:
        Preset(const String & _name, String _filter) : filter(_filter), name(_name) {}

        String filter; //Used to filter which preset to propose depending on the object (specific nodes, vst, controller, etc.)
        String name;
        OwnedArray<PresetValue> presetValues;
        int presetId; //change each time the a preset list is created, but we don't care because ControllableContainer keeps the pointer to the Preset

        void addPresetValue(const String &controlAddress, var value)
        {
            presetValues.add(new PresetValue(controlAddress, value));
        }

        void addPresetValues(Array<PresetValue *> _presetValues)
        {
            presetValues.addArray(_presetValues);
        }

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Preset)
    };

    juce_DeclareSingleton(PresetManager,true)

    OwnedArray<Preset> presets;

    PresetManager();
    virtual ~PresetManager();

    Preset * addPresetFromControllableContainer(const String &name, String filter, ControllableContainer * container, bool recursive = false, bool includeNotExposed = false);
    ComboBox * getPresetSelector(String filter);
    Preset * getPreset(String filter, const String &name);
    void fillWithPresets(ComboBox * cb, String filter);

    void clear();
    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetManager)
};


#endif  // PRESETMANAGER_H_INCLUDED
