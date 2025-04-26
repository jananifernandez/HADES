/*
 * This file is part of HADES
 * Copyright (c) 2021 - Janani Fernandez & Leo McCormack
 *
 * HADES is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * HADES is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * See <http://www.gnu.org/licenses/> for a copy of the GNU General Public
 * License.
 */

#pragma once

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

class PluginEditor  : public AudioProcessorEditor,
                      public MultiTimer,
                      private FilenameComponentListener,
                      public juce::Slider::Listener,
                      public juce::ComboBox::Listener,
                      public juce::Button::Listener
{
public:
    PluginEditor (PluginProcessor* ownerFilter);
    ~PluginEditor() override;

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;

private:
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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
