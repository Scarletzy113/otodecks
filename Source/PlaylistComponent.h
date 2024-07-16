/*
  ==============================================================================

    PlaylistComponent.h
    Created: 13 Feb 2023 11:16:24am
    Author:  Loh Qi Yang

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include "Playlist.h"
#include "DeckGUI.h"
#include "DJAudioPlayer.h"


//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                            public juce::TableListBoxModel,
                            public juce::Button::Listener,
                            public juce::FileDragAndDropTarget,
                            public juce::TextEditor::Listener
{
public:
    PlaylistComponent(DeckGUI* _deckGUI1, DeckGUI* _deckGUI2, DJAudioPlayer* _playerForParsingMetaData);
    ~PlaylistComponent() override;

    
    void paint (juce::Graphics&) override;
    void resized() override;
    
    int getNumRows () override;
    void paintRowBackground (juce::Graphics &,
                        int rowNumber,
                        int width,
                        int height,
                        bool rowIsSelected) override;
    void paintCell (juce::Graphics &,
                        int rowNumber,
                        int columnId,
                        int width,
                        int height,
                        bool rowIsSelected) override;
    void cellClicked (int rowNumber, int columnId, const juce::MouseEvent &) override;
    
    Component* refreshComponentForCell (int rowNumber,
                                        int columnId,
                                        bool isRowSelected,
                                        Component *existingComponentToUpdate) override;
    
    void buttonClicked(juce::Button* button) override;
    
    bool isInterestedInFileDrag(const juce::StringArray &files) override;
    
    void filesDropped(const juce::StringArray &files, int x, int y) override;
    
    //void setTracks(juce::Array<juce::File> tracksFile);
    
   
    
       
    
private:
   
    

    std::vector<Playlist> playlist;
    
    juce::TableListBox tableComponent;
    //juce::AudioFormatManager formatManager;
   
   
    
    juce::TextButton libraryLoad{ "Load into library" };
    juce::TextEditor searchSong;
    juce::TextButton deck1{"Load into Deck 1"};
    juce::TextButton deck2{"Load into Deck 2"};
//    juce::TextButton loadPlaylist{"Playlist"};
//    juce::TextButton savePlaylist{"Save Changes"};
//    
    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;
    DJAudioPlayer* playerForParsingMetaData;
    
    juce::String getLength(juce::URL audioURL);
    juce::String secondsToMinutes(double seconds);

    void importToLibrary();
    void searchLibrary(juce::String searchText);
    void saveLibrary();
    void loadLibrary();
    void deleteFromPlaylist(int id);
    bool isInPlaylist(juce::String fileNameWithoutExtension);
    int locInPlaylist(juce::String searchText);
    void loadIntoPlayer(DeckGUI* deckGUI);

    
    
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
