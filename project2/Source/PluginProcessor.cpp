/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

int SoftSynthesizerProject2AudioProcessor::numHarmonics = 8;

//==============================================================================
/** A demo synth sound that's just a basic sine wave.. */

class SquareWaveSound : public SynthesiserSound

{
public:
	SquareWaveSound() {}

	bool appliesToNote (const int /*midiNoteNumber*/)           { return true; }
	bool appliesToChannel (const int /*midiChannel*/)           { return true; }
};

class SquareWaveVoice : public SynthesiserVoice {
private: float angleDelta, currentAngle, level, lfoCurrentAngle, lfoAngleDelta, lfoLevel, lfoSpeed;
		 bool isTremoloing; 
public: SquareWaveVoice() : angleDelta(0.0) {
			lfoCurrentAngle = 0.0;
			lfoSpeed = 5.0;
			lfoAngleDelta = (lfoSpeed / getSampleRate()) * 2.0 * double_Pi;	
			lfoLevel = 0.2;
		};

		//SETUP: Starting, stopping note

		//this was in the demo code, don't know what it does! lol
		bool canPlaySound (SynthesiserSound* sound) {
			return dynamic_cast <SquareWaveSound*> (sound) != 0;
		}
		//things to do before starting midi note: get angle rate from frequency of note
		void startNote(int midiNoteNumber, float velocity,
			SynthesiserSound* /*sound*/, int /*currentPitchWheelPosition*/) {
			    currentAngle = 0.0;
				level = velocity * 0.15;
				double cyclesPerSecond = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
                double cyclesPerSample = cyclesPerSecond / getSampleRate();
                angleDelta = cyclesPerSample * 2.0 * double_Pi;
				//double frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
				//angleDelta = findAngleDeltaPerSample(frequency);

		}

		//finds angle change in radians per frame/sample
		float findAngleDeltaPerSample(double frequency) {
			double sampleRate = getSampleRate();
			double angleDeltaNotInRadians = frequency / sampleRate;
			double angleDeltaInRadians = angleDeltaNotInRadians * 2 * double_Pi;
			return (float)angleDeltaInRadians;
		}
		//reset everything or trigger note release
		void stopNote(bool allowTailOff) {
			clearCurrentNote();
            angleDelta = 0.0;
			isTremoloing = false;
		}

		//things I am not implementing but are required.
		void juce::SynthesiserVoice::pitchWheelMoved(int) {};
		void juce::SynthesiserVoice::controllerMoved(int,int) {};

		//Rendering Note: write things to an output buffer please

		void renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) {
			if (angleDelta != 0) {
				while (--numSamples >= 0) {
					const float currentSample = 
						generateSquareSample(SoftSynthesizerProject2AudioProcessor::numHarmonics) 
						* generateLfoAmplitude(0.5);
					for (int i = outputBuffer.getNumChannels(); --i >= 0;)
					      *outputBuffer.getSampleData (i, startSample) += currentSample;
					startSample++;
				}
			}
		}

		//returns amplitude of square wave and advances angle of wave one frame
		float generateSquareSample(int numOddHarmonics) {
			float result = 0.0;

			//add up odd harmonics to make square wave approximation
			for (int i = 1; i <= numOddHarmonics * 2; i+= 2) {	
			    result += (float)((((float)1)/i) * sin(i * currentAngle) * level /** tailValue*/);
			 }

			currentAngle += angleDelta;
			if (currentAngle > 2 * double_Pi) {
				currentAngle -= 2 * double_Pi;
			}

			return result;
		}

		double generateLfoAmplitude(double offset) {
			double result = offset + sin(lfoCurrentAngle) * lfoLevel;
			if (lfoCurrentAngle == 2 * double_Pi) {
				lfoCurrentAngle -= 2 * double_Pi;
			}
			lfoCurrentAngle += lfoAngleDelta;
			return result;
		}

};

//==============================================================================
SoftSynthesizerProject2AudioProcessor::SoftSynthesizerProject2AudioProcessor()
{
	gain = 1.0f;
	for (int i = 4; --i >= 0;) {
		synth.addVoice (new SquareWaveVoice());
	}
	synth.addSound(new SquareWaveSound());

}

SoftSynthesizerProject2AudioProcessor::~SoftSynthesizerProject2AudioProcessor()
{
}

//==============================================================================
const String SoftSynthesizerProject2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int SoftSynthesizerProject2AudioProcessor::getNumParameters()
{
    return 2;
}

float SoftSynthesizerProject2AudioProcessor::getParameter (int index)
{
	switch (index)
    {
        case gainParam:     return gain;
		case numHarmonicsParam:  return numHarmonics;
        default:            return 0.0f;
    }
}

void SoftSynthesizerProject2AudioProcessor::setParameter (int index, float newValue)
{
	    switch (index)
    {
        case gainParam:     gain = newValue;  break;
		case numHarmonicsParam:  numHarmonics = (int)(1 + (newValue*30)); break;
        default:            break;
    }
}

const String SoftSynthesizerProject2AudioProcessor::getParameterName (int index)
{
    switch (index)
    {
        case gainParam:     return "gain";
		case numHarmonicsParam:  return "number ofsquare harmonics";
        default:            break;
    }

    return String::empty;
}

const String SoftSynthesizerProject2AudioProcessor::getParameterText (int index)
{
    return String::empty;
}

const String SoftSynthesizerProject2AudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String SoftSynthesizerProject2AudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool SoftSynthesizerProject2AudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool SoftSynthesizerProject2AudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool SoftSynthesizerProject2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SoftSynthesizerProject2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SoftSynthesizerProject2AudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double SoftSynthesizerProject2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SoftSynthesizerProject2AudioProcessor::getNumPrograms()
{
    return 0;
}

int SoftSynthesizerProject2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void SoftSynthesizerProject2AudioProcessor::setCurrentProgram (int index)
{
}

const String SoftSynthesizerProject2AudioProcessor::getProgramName (int index)
{
    return String::empty;
}

void SoftSynthesizerProject2AudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SoftSynthesizerProject2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	synth.setCurrentPlaybackSampleRate (sampleRate);
    keyboardState.reset();
}

void SoftSynthesizerProject2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
	keyboardState.reset();
}

void SoftSynthesizerProject2AudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	const int numSamples = buffer.getNumSamples();
	//all we want is pure synthy goodness
	//buffer.clear();

	//we want to put in that pure synthy goodness
	synth.renderNextBlock(buffer, midiMessages, 0,  numSamples);


    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }

	 AudioPlayHead::CurrentPositionInfo newTime;

    if (getPlayHead() != nullptr && getPlayHead()->getCurrentPosition (newTime))
    {
        // Successfully got the current time from the host..
        lastPosInfo = newTime;
    }
    else
    {
        // If the host fails to fill-in the current time, we'll just clear it to a default..
        lastPosInfo.resetToDefault();
    }
}



//==============================================================================
bool SoftSynthesizerProject2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SoftSynthesizerProject2AudioProcessor::createEditor()
{
    return new SoftSynthesizerProject2AudioProcessorEditor (this);
}

//==============================================================================
void SoftSynthesizerProject2AudioProcessor::getStateInformation (MemoryBlock& destData)

{

    // You should use this method to store your parameters in the memory block.

    // Here's an example of how you can use XML to make it easy and more robust:



    // Create an outer XML element..

    XmlElement xml ("MYPLUGINSETTINGS");



    // add some attributes to it..

    //xml.setAttribute ("uiWidth", lastUIWidth);

    //xml.setAttribute ("uiHeight", lastUIHeight);

    xml.setAttribute ("gain", gain);
	xml.setAttribute ("numHarmonics", numHarmonics);


    //xml.setAttribute ("delay", delay);



    // then use this helper function to stuff it into the binary blob and return it..

    copyXmlToBinary (xml, destData);

}



void SoftSynthesizerProject2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)

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

            //lastUIWidth  = xmlState->getIntAttribute ("uiWidth", lastUIWidth);

            //lastUIHeight = xmlState->getIntAttribute ("uiHeight", lastUIHeight);



            gain  = (float) xmlState->getDoubleAttribute ("gain", gain);
			numHarmonics  = (float) xmlState->getIntAttribute ("numHarmonics", numHarmonics);


            //delay = (float) xmlState->getDoubleAttribute ("delay", delay);

        }

    }

}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SoftSynthesizerProject2AudioProcessor();
}
