/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 6 Feb 2023 9:29:34am
    Author:  Loh Qi Yang

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//#include "DeckGUI.h"



class DJAudioPlayer  : public juce::AudioSource {
public:
    
    DJAudioPlayer(juce::AudioFormatManager& formatManager);
    ~DJAudioPlayer();
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    void loadURL(juce::URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);
    
    /*getting relative position of the playhead*/
    double getPositiveRelative();
    double getLengthInSeconds();
    
    void start();
    void stop();
    void loop();
    void rewind();
    void forward();
    
private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};
    
    
};
