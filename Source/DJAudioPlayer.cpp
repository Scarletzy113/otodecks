/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 6 Feb 2023 9:29:34am
    Author:  Loh Qi Yang

  ==============================================================================
*/

#include "DJAudioPlayer.h"


DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager): formatManager(_formatManager)
{
    
}
DJAudioPlayer::~DJAudioPlayer()
{
    
}

void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
}
void DJAudioPlayer::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);
}
void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) //good file!
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource (new juce::AudioFormatReaderSource (reader, true));
        transportSource.setSource (newSource.get(),0,nullptr,reader->sampleRate);
        readerSource.reset (newSource.release());
        transportSource.start();
        
    }
}
void DJAudioPlayer::setGain(double gain)
{
    if(gain <0 || gain >1)
    {
        std::cout <<"DJAudioPlayer::setGain should be between 0 and 1" << std::endl;
    }
    else
    {
        transportSource.setGain(gain);

    }
    
}
void DJAudioPlayer::setSpeed(double ratio)
{
    if(ratio <0 || ratio >100)
    {
        std::cout <<"DJAudioPlayer::setRatio should be between 0 and 100" << std::endl;
    }
    else
    {
        resampleSource.setResamplingRatio(ratio);

    }
}
void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    if(pos <0 || pos >1.0)
    {
        std::cout <<"DJAudioPlayer::setPositionRelative should be between 0 and 1" << std::endl;
    }
    else
    {
        double posInSecs = transportSource.getLengthInSeconds()* pos;
        setPosition(posInSecs);

    }

}

void DJAudioPlayer::start()
{
    transportSource.start();
}
void DJAudioPlayer::stop()
{
    transportSource.stop();
}

void DJAudioPlayer::loop()
{
    const double audioLength = transportSource.getLengthInSeconds();
    const double currentAudioPos = transportSource.getCurrentPosition();
    
    if(currentAudioPos >= audioLength)
    {
        transportSource.setPosition(0.0);
        
    }
    
}

void DJAudioPlayer::rewind()
{
    const double currentAudioPos = transportSource.getCurrentPosition();
    const double newPos = currentAudioPos - 5.0;
    if(newPos <0.0)
    {
        transportSource.setPosition(0.0);
        
    }
    else
    {
        transportSource.setPosition(newPos);
    }
    
}

void DJAudioPlayer::forward()
{
    const double currentAudioPos = transportSource.getCurrentPosition();
    const double newPos = currentAudioPos + 5.0;
    const double audioLength = transportSource.getLengthInSeconds();
    if(newPos> audioLength)
    {
        transportSource.setPosition(currentAudioPos);
        
    }
    else
    {
        transportSource.setPosition(newPos);
    }
}

double DJAudioPlayer::getPositiveRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

double DJAudioPlayer::getLengthInSeconds()
{
    return transportSource.getLengthInSeconds();
}
