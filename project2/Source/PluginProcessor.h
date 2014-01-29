/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
*/
class SoftSynthesizerProject2AudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    SoftSynthesizerProject2AudioProcessor();
    ~SoftSynthesizerProject2AudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources();

    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;
	double generateLfoAmplitude(double offset);

    //==============================================================================
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;

    //==============================================================================
    const String getName() const;

    int getNumParameters();

    float getParameter (int index);
    void setParameter (int index, float newValue);

    const String getParameterName (int index);
    const String getParameterText (int index);

    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;

    bool acceptsMidi() const;
    bool producesMidi() const;
    bool silenceInProducesSilenceOut() const;
    double getTailLengthSeconds() const;

    //==============================================================================
    int getNumPrograms();
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);

	//==============================================================================
	//Hacky Public Things, Because they are like this in demo and c++ scares me
	MidiKeyboardState keyboardState;
	AudioPlayHead::CurrentPositionInfo lastPosInfo;

	enum Parameters
    {
        gainParam = 0,
		numHarmonicsParam,
        totalNumParams
    };

	float gain; 
	static int numHarmonics;





private:
    //==============================================================================
	Synthesiser synth;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoftSynthesizerProject2AudioProcessor)

};

#endif  // PLUGINPROCESSOR_H_INCLUDED
