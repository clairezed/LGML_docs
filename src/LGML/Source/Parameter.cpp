/*
  ==============================================================================

    Parameter.cpp
    Created: 8 Mar 2016 1:08:19pm
    Author:  bkupe

  ==============================================================================
*/

#include "Parameter.h"
#include "JsHelpers.h"


Parameter::Parameter(const Type &type, const String &niceName, const String &description, var initialValue, var minValue = var(), var maxValue = var(), bool enabled) :
	Controllable(type, niceName, description, enabled),
	isEditable(true),
    isSavable(true),
	isPresettable(true),
	isOverriden(false),
    queuedNotifier(100)
{
    minimumValue = minValue;
    maximumValue = maxValue;
    defaultValue = initialValue;

    resetValue();
}

  void Parameter::resetValue()
{
    isOverriden = false;
    setValue(defaultValue);
}

void Parameter::setValue(var _value, bool silentSet, bool force)
{

    if (!force && this->value == _value) return;
    setValueInternal(_value);


    if(_value != defaultValue) isOverriden = true;

    if (!silentSet) notifyValueChanged();
}

void Parameter::setRange(var min, var max){
    minimumValue = min;
    maximumValue = max;
    listeners.call(&Listener::parameterRangeChanged,this);
}

void Parameter::setValueInternal(var & _value) //to override by child classes
{

    value = _value;
#ifdef JUCE_DEBUG
    checkVarIsConsistentWithType();
#endif
}

void Parameter::checkVarIsConsistentWithType(){
    if      (type == Type::STRING)  jassert(value.isString());
    else if (type == Type::INT)     jassert(value.isInt());
    else if (type == Type::BOOL)    jassert(value.isBool());
    else if (type == Type::FLOAT)   jassert(value.isDouble());

}

void Parameter::setNormalizedValue(const float & normalizedValue, bool silentSet, bool force)
{
    setValue(jmap<float>(normalizedValue, (float)minimumValue, (float)maximumValue), silentSet, force);
}

float Parameter::getNormalizedValue() {
    return jmap<float>((float)value, (float)minimumValue, (float)maximumValue, 0.f, 1.f);
}

void Parameter::notifyValueChanged() {
    listeners.call(&Listener::parameterValueChanged, this);
    queuedNotifier.addMessage(new ParamWithValue(this,value));
}

DynamicObject * Parameter::createDynamicObject()
{
	DynamicObject * dObject = Controllable::createDynamicObject();
    static const Identifier _jsGetIdentifier("get");
	dObject->setMethod(_jsGetIdentifier, Parameter::getValue);

    static const Identifier _jsSetIdentifier("set");
    dObject->setMethod(_jsSetIdentifier, setControllableValue);

	return dObject;
}

var Parameter::getValue(const juce::var::NativeFunctionArgs & a)
{
	Parameter * c = getObjectPtrFromJS<Parameter>(a);
	if(c == nullptr) return var();
	return c->value;
}

//JS Helper
