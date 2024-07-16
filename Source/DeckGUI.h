/*
  ==============================================================================

    DeckGUI.h
    Created: 6 Feb 2023 1:15:57pm
    Author:  Loh Qi Yang

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
//#include "PlaylistComponent.h"

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
                public juce::Button::Listener,
                public juce::Slider::Listener,
                public juce::FileDragAndDropTarget,
                public juce::Timer
{
public:
    DeckGUI(DJAudioPlayer* player, juce::AudioFormatManager & formatManagerTouse, juce::AudioThumbnailCache & cacheToUse);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    /* implement button listener */
    void buttonClicked (juce::Button *) override;
    
    void sliderValueChanged(juce::Slider *slider) override;
    
    bool isInterestedInFileDrag(const juce::StringArray &files) override;
    
    void filesDropped(const juce::StringArray &files, int x, int y) override;

    void timerCallback() override;
    
   
    
private:
    
   
    
    juce::TextButton playButton{"Play"};
    juce::TextButton stopButton{"Stop"};
    juce::TextButton loadButton{"LOAD"};
    juce::TextButton loopButton{"Replay"};
    juce::TextButton rewindButton{"<<"};
    juce::TextButton forwardButton{">>"};
    
    
    juce::Slider volSlider;
    juce::Label volLabel;
    
    juce::Slider speedSlider;
    juce::Label speedLabel;
    
    juce::Slider posSlider;
    juce::Label posLabel;
    
    
    juce::FileChooser fChooser{"Select a file..."};
    
    
    DJAudioPlayer* player;
    
    WaveformDisplay waveformDisplay;
    
    void loadFile(juce::URL audioURL);
    
    friend class PlaylistComponent;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
