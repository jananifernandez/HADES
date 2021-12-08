/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.8

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "PluginProcessor.h"

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class EditWindow360  : public Component
{
public:
    //==============================================================================
    EditWindow360 (float* _pData, float _minVal, float _maxVal, int _dB_flag);
    ~EditWindow360() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void refresh(){

        /* Redraw visual depiction of what is going on... */
        if(refreshRequired)
            repaint();
    }

    void setRefreshNeeded(bool flag){
        refreshRequired = flag;
    }

    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent& e) override;
    void mouseDrag (const juce::MouseEvent& e) override;
    void mouseUp (const juce::MouseEvent& e) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    bool refreshRequired;
    float* pData;
    float minVal;
    float maxVal;
    int gainFLAG;

    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditWindow360)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

