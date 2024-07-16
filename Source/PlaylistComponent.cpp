/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 13 Feb 2023 11:16:24am
    Author:  Loh Qi Yang

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

#include <string>

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1,
                                     DeckGUI* _deckGUI2,
                                     DJAudioPlayer* _playerForParsingMetaData
                                    ) : deckGUI1(_deckGUI1),
                                        deckGUI2(_deckGUI2),
                                        playerForParsingMetaData(_playerForParsingMetaData)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
//
//    std::ofstream myPlaylist("myPlaylist.txt");
//    myPlaylist << "false" << std::endl;
//
//
//    if(!std::filesystem::exists("savedPlaylist.txt"))
//    {
//        std::ofstream savedPlaylist("savedPlaylist.txt");
//        savedPlaylist.close();
//    }
//
//
//    std::ifstream openSavedPlaylist("savedPlaylist");
//    std::vector <std::string> linesVector;
//    std::string singleLine;
//
//    while(std::getline(openSavedPlaylist, singleLine ))
//    {
//        juce::File playlistFile(juce::String(singleLine.c_str()));
//        playlist.add(playlistFile);
//
//    }

    
    
    
    
    //component addAndMakeVisible
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(libraryLoad);
    addAndMakeVisible(searchSong);
    addAndMakeVisible(deck1);
    addAndMakeVisible(deck2);
//    addAndMakeVisible(loadPlaylist);
//    addAndMakeVisible(savePlaylist);
//
    //Listeners
    libraryLoad.addListener(this);
    searchSong.addListener(this);
    deck1.addListener(this);
    deck2.addListener(this);
//    loadPlaylist.addListener(this);
//    savePlaylist.addListener(this);
    
    //search config
    searchSong.setTextToShowWhenEmpty("Enter Song Name Here...", juce::Colours::orange);
    searchSong.onReturnKey = [this] {searchLibrary(searchSong.getText());};
    
    tableComponent.getHeader().addColumn("Song Name", 1,1);
    tableComponent.getHeader().addColumn("Duration", 2, 1);
    tableComponent.getHeader().addColumn("", 3, 1);
    tableComponent.setModel(this);
    
    saveLibrary();
    loadLibrary();
    
    
    
   
    
}

PlaylistComponent::~PlaylistComponent()
{
   saveLibrary();

    
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::green);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::blue);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    double rowH = getHeight()/10;
    double rowW = getWidth();
    
    libraryLoad.setBounds(0, 0, rowW, rowH);
    
    deck1.setBounds(0, rowH, rowW/2, rowH*2);
    deck2.setBounds(rowW/2, rowH, rowW/2, rowH*2);
    searchSong.setBounds(0, rowH*3, rowW, rowH);
    tableComponent.setBounds(0, rowH*4, getWidth(), getHeight());
//    loadPlaylist.setBounds(0, rowH*2, rowW/2, rowH);
//    savePlaylist.setBounds(rowW/2, rowH*2, rowW/2, rowH);
    
    
    tableComponent.getHeader().setColumnWidth(1, (rowW/7)*4 );
    tableComponent.getHeader().setColumnWidth(2, (rowW/7)*2);
    tableComponent.getHeader().setColumnWidth(3, rowW/7);
    
}
int PlaylistComponent::getNumRows()
{
    return playlist.size();

}

void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    //highlight selected row
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::orange);
    }
    else{
        g.fillAll(juce::Colours::darkgrey);
    }
}
void PlaylistComponent::paintCell (juce::Graphics & g,
                    int rowNumber,
                    int columnId,
                    int width,
                    int height,
                    bool rowIsSelected)
{
    if(columnId ==1){
        g.drawText (playlist[rowNumber].title,
                    2, 0,
                    width - 4, height,
                    juce::Justification::centredLeft,
                    true);
    }
    if(columnId ==2)
    {
        g.drawText (playlist[rowNumber].length,
                    2, 0,
                    width - 4, height,
                    juce::Justification::centredLeft,
                    true);
    }
   
    
}
void PlaylistComponent::cellClicked (int rowNumber, int columnId, const juce::MouseEvent &)
{
    std::cout << rowNumber << "," << columnId << std::endl;
}

juce::Component* PlaylistComponent::refreshComponentForCell (
                                                       int rowNumber,
                                                       int columnId,
                                                       bool isRowSelected,
                                                       Component *existingComponentToUpdate)
{
    
    
    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {

            juce::TextButton* btn = new juce::TextButton("X");
            juce::String id{std::to_string(rowNumber)};
            btn->setComponentID(id);
            existingComponentToUpdate = btn;
            btn->addListener(this);
            
            
        }
    }
    
    return  existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
//    int id = std::stoi(button->getComponentID().toStdString());
//    DBG("PlaylistComponent::buttonClicked " << trackTitles[id]);
    if(button == &libraryLoad)
    {
        DBG("load Library");
        importToLibrary();
        tableComponent.updateContent();
    }
    else if (button ==&deck1)
    {
        DBG("load to Deck1");
        loadIntoPlayer(deckGUI1);
        
    }
    else if (button ==&deck2)
    {
        DBG("load to Deck2");
        loadIntoPlayer(deckGUI2);
        
    }
    else{
        int id = std::stoi(button->getComponentID().toStdString());
        DBG(playlist[id].title + "deleted");
        deleteFromPlaylist(id);
        tableComponent.updateContent();
        
    }

}
bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray& files)
{
    std::cout<< "PlaylistComponent::isInterestedInFIleDrag" << std::endl;
    return true;
}

void PlaylistComponent::filesDropped(const juce::StringArray& files, int x, int y)
{
    std::cout << "PlaylistComponent::filesDropped" << std::endl;
    
        
    
}

void PlaylistComponent::loadIntoPlayer(DeckGUI* deckGUI)
{
    int selectedRow{ tableComponent.getSelectedRow() };
    if (selectedRow != -1)
    {
        DBG("Load: " << playlist[selectedRow].title << " to Player");
        deckGUI->loadFile(playlist[selectedRow].URL);
    }
    else
    {
        juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon,
            "Error:",
            "Please select a track before adding to deck",
            "OK",
            nullptr
        );
    }
}

void PlaylistComponent::importToLibrary()
{
    DBG("PlaylistComponent::importToLibrary called");

    //initialize file chooser
    juce::FileChooser chooser{ "Select files" };
    if (chooser.browseForMultipleFilesToOpen())
    {
        for (const juce::File& file : chooser.getResults())
        {
            juce::String fileNameWithoutExtension{ file.getFileNameWithoutExtension() };
            if (!isInPlaylist(fileNameWithoutExtension)) // check if song was loaded in before
            {
                Playlist newSong{ file };
                juce::URL audioURL{ file };
                newSong.length = getLength(audioURL) ;
                playlist.push_back(newSong);
                DBG("loaded file: " << newSong.title);
            }
            else // display duplicated song message
            {
                juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon,
                    "Warning:",
                    fileNameWithoutExtension + " is in the playlist",
                    "OK",
                    nullptr
                );
            }
        }
    }
}

bool PlaylistComponent::isInPlaylist(juce::String fileNameWithoutExtension)
{
    return (std::find(playlist.begin(), playlist.end(), fileNameWithoutExtension) != playlist.end());
}

void PlaylistComponent::deleteFromPlaylist(int id)
{
    playlist.erase(playlist.begin() + id);
}

juce::String PlaylistComponent::getLength(juce::URL audioURL)
{
    playerForParsingMetaData->loadURL(audioURL);
    double seconds{ playerForParsingMetaData->getLengthInSeconds() };
    juce::String minutes{ secondsToMinutes(seconds) };
    return minutes;
}

juce::String PlaylistComponent::secondsToMinutes(double seconds)
{
    //find seconds and minutes and make into string
    int secondsRounded{ int(std::round(seconds)) };
    juce::String min{ std::to_string(secondsRounded / 60) };
    juce::String sec{ std::to_string(secondsRounded % 60) };
    
    if (sec.length() < 2) // if seconds is 1 digit or less
    {
        //add '0' to seconds until seconds is length 2
        sec = sec.paddedLeft('0', 2);
    }
    return juce::String{ min + ":" + sec };
}

void PlaylistComponent::searchLibrary(juce::String searchText)
{
    DBG("Searching library for: " << searchText);
    if (searchText != "")
    {
        int rowNumber = locInPlaylist(searchText);
        tableComponent.selectRow(rowNumber);
    }
    else
    {
        tableComponent.deselectAllRows();
    }
}

int PlaylistComponent::locInPlaylist(juce::String searchText)
{
    // finds index where track title contains searchText
    auto it = find_if(playlist.begin(), playlist.end(),
        [&searchText](const Playlist& obj) {return obj.title.contains(searchText); });
    int i = -1;

    if (it != playlist.end())
    {
        i = std::distance(playlist.begin(), it);
    }

    return i;
}

void PlaylistComponent::saveLibrary()
{
    // create .csv to save library
    std::ofstream myPlaylist("/Users/lohqiyang/Desktop/myPlaylist.csv");
    if (myPlaylist.is_open())
    {
        DBG("file opened");
    }
    
    // save library to file
    for (Playlist& t : playlist)
    {
        DBG(t.file.getFullPathName());
        myPlaylist << t.file.getFullPathName() << "," << t.length << "\n";
    }


    //myPlaylist.close();


}

void PlaylistComponent::loadLibrary()
{
    // create input stream from saved library
    std::ifstream myPlaylist("/Users/lohqiyang/Desktop/myPlaylist.csv");
    std::string filePath;
    std::string length;
    DBG("playlist loaded");
     //Read data, line by line
    if (myPlaylist.is_open())
    {
        DBG("Playlist open");
        while(getline(myPlaylist, filePath, ',')) {
            juce::File file{ filePath };
            Playlist newSong{ file };

            getline(myPlaylist, length);
            newSong.length = length;
            playlist.push_back(newSong);
            
        }
    }
    else if(!myPlaylist.is_open()){
        DBG("file not open");
    }
    myPlaylist.close();
}



