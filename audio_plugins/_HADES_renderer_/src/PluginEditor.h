/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.1.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "EditWindow360.h"
#include "../../resources/log2dSlider.h"
#include "../../resources/HADESLookAndFeel.h"

typedef enum _HADES_WARNINGS{
    k_warning_none,
    k_warning_frameSize,
    k_warning_supported_fs,
    k_warning_mismatch_fs,
    k_warning_NinputCH,
    k_warning_NoutputCH
}HADES_WARNINGS;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PluginEditor  : public AudioProcessorEditor,
                      public MultiTimer,
                      private FilenameComponentListener,
                      public juce::Slider::Listener,
                      public juce::ComboBox::Listener,
                      public juce::Button::Listener
{
public:
    //==============================================================================
    PluginEditor (PluginProcessor* ownerFilter);
    ~PluginEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.


    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    PluginProcessor* hVst;
    void* hHdR;
    void timerCallback(int timerID) override;
    double progress = 0.0;
    ProgressBar progressbar;

    /* Look and Feel */
    HADESLookAndFeel LAF;

    /* 2d slider handles */
    std::unique_ptr<log2dSlider> streamBalance2dSlider;

    /* Radial editor windows */
    std::unique_ptr<EditWindow360> dirGainEditor;

    /* sofa file loading */
    FilenameComponent fileChooserMAIR, fileChooserHRIR;
    void filenameComponentChanged (FilenameComponent* fileComponentChanged) override  {
        if(fileComponentChanged == &fileChooserMAIR){
            File currentSOFAFile = fileChooserMAIR.getCurrentFile();
            String directory = currentSOFAFile.getFullPathName();
            currentSOFAFile = currentSOFAFile.getChildFile(directory);
            directory = currentSOFAFile.getFullPathName();
            const char* new_cstring = (const char*)directory.toUTF8();
            hades_renderer_setSofaFilePathMAIR(hHdR, new_cstring);
        }
        else if (fileComponentChanged == &fileChooserHRIR){
            File currentSOFAFile = fileChooserHRIR.getCurrentFile();
            String directory = currentSOFAFile.getFullPathName();
            currentSOFAFile = currentSOFAFile.getChildFile(directory);
            directory = currentSOFAFile.getFullPathName();
            const char* new_cstring = (const char*)directory.toUTF8();
            hades_renderer_setSofaFilePathHRIR(hHdR, new_cstring);
        }
    }

    /* warnings */
    HADES_WARNINGS currentWarning;

    /* tooltips */
    SharedResourcePointer<TooltipWindow> tipWindow;
    std::unique_ptr<juce::ComboBox> pluginDescription; /* Dummy combo box to provide plugin description tooltip */
    HyperlinkButton publicationLink { "(Related Publication)", { "https://leomccormack.github.io/sparta-site/docs/help/related-publications/fernandez2022enhancing.pdf" } };

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Slider> s_diff2dir;
    std::unique_ptr<juce::Label> label_IR_fs_array;
    std::unique_ptr<juce::Label> label_DAW_fs_array;
    std::unique_ptr<juce::ComboBox> CBbeamformer;
    std::unique_ptr<juce::Label> label_N_nMics;
    std::unique_ptr<juce::Label> label_N_nDirs_array;
    std::unique_ptr<juce::Label> label_IR_length_array;
    std::unique_ptr<juce::ToggleButton> TBuseDefaultHRIRs;
    std::unique_ptr<juce::Label> label_HRIR_fs_bin;
    std::unique_ptr<juce::Label> label_DAW_fs_bin;
    std::unique_ptr<juce::Label> label_N_nDirs_bin;
    std::unique_ptr<juce::Label> label_IR_length_bin;
    std::unique_ptr<juce::ToggleButton> TBenableCM;
    std::unique_ptr<juce::Slider> SL_analysis_avg;
    std::unique_ptr<juce::Slider> SL_synthesis_avg;
    std::unique_ptr<juce::Slider> SL_left_ref_idx;
    std::unique_ptr<juce::Slider> SL_right_ref_idx;
    std::unique_ptr<juce::ComboBox> CBdoaEstimator;
    std::unique_ptr<juce::ComboBox> CBdiffEstimator;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

