/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath> 
#include <algorithm> 


//==============================================================================
NewProjectAudioProcessor::NewProjectAudioProcessor()
{
	removeCounter = 0;
	lastUIWidth = 400;
    lastUIHeight = 200;
}

NewProjectAudioProcessor::~NewProjectAudioProcessor()
{
}

//==============================================================================
const String NewProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int NewProjectAudioProcessor::getNumParameters()
{
    return 7;
}

float NewProjectAudioProcessor::getParameter (int index)
{
	switch (index)
    {
		case innerLevelParam:  return innerLevel;
		case outerLevelParam:  return outerLevel;
		case keep:			   return keepNum;
		case remove:           return removeNum;
		case removeAmt:        return removeAmount;
        case noisecutoff:      return noiseCutoff;
        case noiseamount:      return noiseAmount;

        default:            return 0.0f;
    }
}

void NewProjectAudioProcessor::setParameter (int index, float newValue)
{
	    switch (index)
    {
		case innerLevelParam: innerLevel = newValue; break;
		case outerLevelParam: outerLevel = newValue; break;
		case keep: keepNum = newValue; break;
		case remove: removeNum = newValue; break;
		case removeAmt: removeAmount = newValue; break;
        case noisecutoff:  noiseCutoff = newValue;  break;
        case noiseamount:  noiseAmount = newValue;  break;

        default:            break;
    }
}

const String NewProjectAudioProcessor::getParameterName (int index)
{
    switch (index)
    {
		case innerLevelParam:  return "inner level";
		case outerLevelParam:  return "outer level";
		case keep:  return "frames to keep";
		case remove:  return "frames to alter";
		case removeAmt:  return "alteration amount";
        case noiseamount:     return "noise amt";
        case noisecutoff:     return "noise cutoff";

        default:            break;
    }

    return String::empty;
}

const String NewProjectAudioProcessor::getParameterText (int index)
{
    return String::empty;
}

const String NewProjectAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String NewProjectAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool NewProjectAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool NewProjectAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool NewProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double NewProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NewProjectAudioProcessor::getNumPrograms()
{
    return 0;
}

int NewProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NewProjectAudioProcessor::setCurrentProgram (int index)
{
}

const String NewProjectAudioProcessor::getProgramName (int index)
{
    return String::empty;
}

void NewProjectAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void NewProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void NewProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void NewProjectAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    
		int startSample = 0;
		int numSamples = buffer.getNumSamples();
		int numChannels = buffer.getNumChannels();
		while (startSample < numSamples) {
			for (int i = numChannels; --i >= 0;) {
				//a = amplitude
				float a = *buffer.getSampleData(i, startSample);

				//noiser
				if (std::abs(a) < noiseCutoff) {
					if (a > 0) {
						a += noiseAmount * ((rand() % 100) / (float)100) * (1-a);
					}
					else {
						a += noiseAmount * ((rand() % 100) / (float)100) * (-1-a);
					}
				}

				//remover
				if (removeCounter > (int)(keepNum * 100)) {
					a *= removeAmount;
				}

				//inner level
				if (std::abs(a) < innerLevel) {
					if (a < 0) {
						a = -1 * innerLevel;
					}
					else {
						a = innerLevel;
					}
				}
				//outer level
				if (std::abs(a) > outerLevel && outerLevel !=0) {
					if (a < 0) {
						a = -1 * outerLevel;
					}
					else {
						a = outerLevel;
					}
				}

				*buffer.getSampleData(i, startSample) = a;
				startSample++;
	            

			}
			removeCounter++;
			if (removeCounter > (int)(keepNum * 100) + (int)(removeNum * 100)) {
				removeCounter = 0;
			}
		}
		
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
}

//==============================================================================
bool NewProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* NewProjectAudioProcessor::createEditor()
{
    return new NewProjectAudioProcessorEditor (this);
}

//==============================================================================
void NewProjectAudioProcessor::getStateInformation (MemoryBlock& destData)

{

    // You should use this method to store your parameters in the memory block.

    // Here's an example of how you can use XML to make it easy and more robust:



    // Create an outer XML element..

    XmlElement xml ("MYPLUGINSETTINGS");



    // add some attributes to it..

    xml.setAttribute ("uiWidth", lastUIWidth);

    xml.setAttribute ("uiHeight", lastUIHeight);

	xml.setAttribute ("innerlevel", innerLevel);
	xml.setAttribute ("outerlevel", outerLevel);
	xml.setAttribute ("keepnum", keepNum);
	xml.setAttribute ("removenum", removeNum);	
	xml.setAttribute ("removeamount", removeAmount);
	xml.setAttribute ("noiseCutoff", noiseCutoff);
	xml.setAttribute ("noiseAmount", noiseAmount);


	//xml.setAttribute ("numHarmonics", numHarmonics);


    //xml.setAttribute ("delay", delay);



    // then use this helper function to stuff it into the binary blob and return it..

    copyXmlToBinary (xml, destData);

}



void NewProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)

{

    // You should use this method to restore your parameters from this memory block,

    // whose contents will have been created by the getStateInformation() call.



    // This getXmlFromBinary() helper function retrieves our XML from the binary blob..

    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));



    if (xmlState != nullptr)

    {

        // make sure that it's actually our type of XML object..

        if (xmlState->hasTagName ("MYPLUGINSETTINGS"))

        {

            // ok, now pull out our parameters..

            lastUIWidth  = xmlState->getIntAttribute ("uiWidth", lastUIWidth);

            lastUIHeight = xmlState->getIntAttribute ("uiHeight", lastUIHeight);

			innerLevel  = (float) xmlState-> getDoubleAttribute ("innerLevel", innerLevel);
			outerLevel  = (float) xmlState-> getDoubleAttribute ("outerLevel", innerLevel);
			keepNum  = (float) xmlState-> getIntAttribute ("keepNum", keepNum);
			removeNum  = (float) xmlState-> getIntAttribute ("removeNum", removeNum);
			removeAmount  = (float) xmlState-> getDoubleAttribute ("removeAmount", removeAmount);
            noiseCutoff  = (float) xmlState-> getDoubleAttribute ("noiseCutoff", noiseCutoff);
            noiseAmount  = (float) xmlState-> getDoubleAttribute ("noiseAmount", noiseAmount);

        }

    }

}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}
