#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"



//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent,
                        public juce::Button::Listener,
                        public juce::Slider::Listener,
                        public juce::Timer

{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    /* implement button listener */
    void buttonClicked (juce::Button *) override;
    
    void sliderValueChanged(juce::Slider *slider) override;
    
    void timerCallback() override;
    
    
    

private:
    //==============================================================================
    // Your private member variables go here...

    
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbCache{100};
    DJAudioPlayer playerForParsingMetaData{formatManager};
    PlaylistComponent playlistComponent{&deckGUI1,&deckGUI2,&playerForParsingMetaData};
    double rowclicked;
    
    
    
    DJAudioPlayer player1{formatManager};
    DeckGUI deckGUI1{&player1, formatManager, thumbCache};

    DJAudioPlayer player2{formatManager};
    DeckGUI deckGUI2{&player2, formatManager, thumbCache};
    
    juce::MixerAudioSource mixerSource;
    std::vector<std::string> lines;
    
    
    

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};