/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.1

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_EF8AD2ECD21BE784__
#define __JUCE_HEADER_EF8AD2ECD21BE784__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "GCHQAudioProcessor.h"
class GCHQLookAndFeel;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class GCHQEditor  : public AudioProcessorEditor,
                    public SliderListener,
                    public ButtonListener
{
public:
    //==============================================================================
    GCHQEditor (GCHQAudioProcessor& p);
    ~GCHQEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
 public:
    void setIndicator( bool indicator );
 private:
    void updateControls();
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);

    // Binary resources:
    static const char* background_png;
    static const int background_pngSize;
    static const char* programDownPressed_png;
    static const int programDownPressed_pngSize;
    static const char* programDownUnpressed_png;
    static const int programDownUnpressed_pngSize;
    static const char* programUpPressed_png;
    static const int programUpPressed_pngSize;
    static const char* programUpUnpressed_png;
    static const int programUpUnpressed_pngSize;
    static const char* indicatorOff_png;
    static const int indicatorOff_pngSize;
    static const char* indicatorOn_png;
    static const int indicatorOn_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    GCHQAudioProcessor& processor;
    ScopedPointer<GCHQLookAndFeel> lookAndFeel;
    String currentProgramNameString, currentProgramNumberString;
    Image cachedImage_indicatorOff_png;
    Image cachedImage_indicatorOn_png;
    bool indicatorOn;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> gainInKnob;
    ScopedPointer<Slider> gainOutKnob;
    ScopedPointer<ImageButton> upButton;
    ScopedPointer<ImageButton> downButton;
    Image cachedImage_background_png;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GCHQEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_EF8AD2ECD21BE784__
