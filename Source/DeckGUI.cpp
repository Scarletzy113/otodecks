/*
  ==============================================================================

    DeckGUI.cpp
    Created: 6 Feb 2023 1:15:57pm
    Author:  Loh Qi Yang

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 juce::AudioFormatManager & formatManagerToUse,
                 juce::AudioThumbnailCache & cacheToUse
                 )
                 : player(_player), // assign _player to our player data member
                 waveformDisplay(formatManagerToUse, cacheToUse) // call the constructor on
{
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(loopButton);
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(rewindButton);
    addAndMakeVisible(forwardButton);
    
    
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(posLabel);
    
    volLabel.setText("Volume", juce::dontSendNotification);
    speedLabel.setText("Playback Speed", juce::dontSendNotification);
    posLabel.setText("Position", juce::dontSendNotification);
    
    volLabel.attachToComponent(&volSlider, false);
    speedLabel.attachToComponent(&speedSlider, false);
    posLabel.attachToComponent(&posSlider, true);
    
    
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    loopButton.addListener(this);
    rewindButton.addListener(this);
    forwardButton.addListener(this);
    
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    
    volSlider.setRange(0.0,1.0);
    speedSlider.setRange(0.0, 100.0);
    posSlider.setRange(0.0,1.0);
    
    startTimer(200);
    
   
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("DeckGUI", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 7;
    double rowW = getWidth()/3;
    
    waveformDisplay.setBounds(0, 0, getWidth(), rowH * 2);
    
    volSlider.setBounds(0, rowH * 2, rowW, rowH*2);
    speedSlider.setBounds(rowW, rowH * 2, rowW, rowH*2);
    posSlider.setBounds(getWidth()/5, rowH * 4, getWidth()/5*4, rowH);
    
    playButton.setBounds(0, rowH*5, rowW, rowH);
    stopButton.setBounds(rowW, rowH*5, rowW, rowH);
    loopButton.setBounds(rowW*2, rowH*5, rowW, rowH);
    loadButton.setBounds(0, rowH * 6, getWidth(), rowH);
    rewindButton.setBounds(rowW*2, rowH*2, rowW, rowH);
    forwardButton.setBounds(rowW*2, rowH*3, rowW, rowH);
    
    
    
    volSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    posSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    
    
    
}

void DeckGUI::buttonClicked(juce::Button * button)
{
    if (button == &playButton)
    {
        std::cout << " Play Button was clicked" << std::endl;
        player->start();
    }
    if (button ==&stopButton)
    {
        std::cout << " Stop Button was clicked" << std::endl;
        player->stop();
    }
    if (button ==&loadButton)
    {
        // this does work in 6.1 but the syntax is a little funky
            // https://docs.juce.com/master/classFileChooser.html#ac888983e4abdd8401ba7d6124ae64ff3
            // - configure the dialogue
            auto fileChooserFlags =
            juce::FileBrowserComponent::canSelectFiles;
            // - launch out of the main thread
            // - note how we use a lambda function which you've probably
            // not seen before. Please do not worry too much about that.
            fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
            {
                auto chosenFile = chooser.getResult();
                player->loadURL(juce::URL{chosenFile});
                
                waveformDisplay.loadURL(juce::URL{chooser.getResult()});
            });
    }
    if (button ==&loopButton)
    {
        std::cout <<"loop button was clicked" << std::endl;
        
        player->loop();
        player->start();
           
        
    }
    
    if (button ==&rewindButton)
    {
        std::cout <<"rewind button was clicked" << std::endl;
        player->rewind();
    }
    
    if (button ==&forwardButton)
    {
        std::cout <<"forward button was clicked" << std::endl;
        player->forward();
    }
}


void DeckGUI::sliderValueChanged(juce::Slider *slider)
{
    if(slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }
    if(slider ==&speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    if(slider ==&posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }

}


bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    std::cout<< "DeckGUI::isInterestedInFIleDrag" << std::endl;
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    std::cout << "DeckGUI::filesDropped" << std::endl;
    if(files.size()==1)
    {
        player->loadURL(juce::URL{juce::File{files[0]}});
    }
}

void DeckGUI::timerCallback()
{
    
    //DBG("DeckGUI::timerCallback");
    if(player->getPositiveRelative() > 0){
        waveformDisplay.setPositionRelative(player->getPositiveRelative());
    }
    
    
    
}


void DeckGUI::loadFile(juce::URL audioURL)
{
    player->loadURL(juce::URL{audioURL});
    
    waveformDisplay.loadURL(audioURL);
}
