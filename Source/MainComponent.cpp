#include "MainComponent.h"
#include <iostream>

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1280, 960);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }
    
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playlistComponent);
    formatManager.registerBasicFormats();
    
    startTimer(500);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    

    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
    
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}


void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
    
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    double rowW = getWidth()/8;
    double rowH = getHeight();
    deckGUI1.setBounds(rowW*2,0,rowW*3,rowH);
    deckGUI2.setBounds(rowW*5,0,rowW*3,rowH);
    playlistComponent.setBounds(0, 0, rowW*2, rowH);
}

void MainComponent::buttonClicked(juce::Button * button)
{
//    if (button == &playButton)
//    {
//        std::cout << " Play Button was clicked" << std::endl;
//        player1.start();
//    }
//    if (button ==&stopButton)
//    {
//        std::cout << " Stop Button was clicked" << std::endl;
//        player1.stop();
//    }
//    if (button ==&loadButton)
//    {
//        // this does work in 6.1 but the syntax is a little funky
//            // https://docs.juce.com/master/classFileChooser.html#ac888983e4abdd8401ba7d6124ae64ff3
//            // - configure the dialogue
//            auto fileChooserFlags =
//            juce::FileBrowserComponent::canSelectFiles;
//            // - launch out of the main thread
//            // - note how we use a lambda function which you've probably
//            // not seen before. Please do not worry too much about that.
//            fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
//            {
//                juce::File chosenFile = chooser.getResult();
//                player1.loadURL(juce::URL{chosenFile});
//            });
//    }
}

void MainComponent::sliderValueChanged(juce::Slider *slider)
{
//    if(slider == &volSlider)
//    {
//        player1.setGain(slider->getValue());
//    }
//    if(slider ==&speedSlider)
//    {
//        player1.setSpeed(slider->getValue());
//    }
//    if(slider ==&posSlider)
//    {
//        player1.setPositionRelative(slider->getValue());
//    }

}

void MainComponent::timerCallback() { //Function loops every 500ms, checks whether user has loaded a playlist audio file into a deckGUI
    
}
