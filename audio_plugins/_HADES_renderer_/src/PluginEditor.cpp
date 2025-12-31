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

#include "PluginEditor.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor(p), processor(p), progressbar(progress), fileChooserMAIR ("File", File(), true, false, false,
      "*.sofa;*.nc;", String(),
      "Load SOFA File"), fileChooserHRIR ("File", File(), true, false, false,
      "*.sofa;*.nc;", String(),
      "Load SOFA File")
{
    s_diff2dir.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (s_diff2dir.get());
    s_diff2dir->setRange (0, 2, 0.01);
    s_diff2dir->setDoubleClickReturnValue(true, 1.0f);
    s_diff2dir->setSliderStyle (juce::Slider::LinearVertical);
    s_diff2dir->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    s_diff2dir->addListener (this);
    s_diff2dir->setBounds (384, 354, 40, 74);
    
    label_IR_fs_array.reset (new juce::Label ("new label", juce::String()));
    addAndMakeVisible (label_IR_fs_array.get());
    label_IR_fs_array->setFont (juce::FontOptions (15.00f, juce::Font::plain).withStyle ("Regular"));
    label_IR_fs_array->setJustificationType (juce::Justification::centredLeft);
    label_IR_fs_array->setEditable (false, false, false);
    label_IR_fs_array->setBounds (164, 166, 51, 20);

    label_DAW_fs_array.reset (new juce::Label ("new label", juce::String()));
    addAndMakeVisible (label_DAW_fs_array.get());
    label_DAW_fs_array->setFont (juce::FontOptions (15.00f, juce::Font::plain).withStyle ("Regular"));
    label_DAW_fs_array->setJustificationType (juce::Justification::centredLeft);
    label_DAW_fs_array->setEditable (false, false, false);
    label_DAW_fs_array->setBounds (164, 189, 51, 20);

    CBbeamformer.reset (new juce::ComboBox ("new combo box"));
    addAndMakeVisible (CBbeamformer.get());
    CBbeamformer->setEditableText (false);
    CBbeamformer->setJustificationType (juce::Justification::centredLeft);
    CBbeamformer->setTextWhenNothingSelected (TRANS("Default"));
    CBbeamformer->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    CBbeamformer->addListener (this);
    CBbeamformer->setBounds (122, 289, 98, 20);

    label_N_nMics.reset (new juce::Label ("new label", juce::String()));
    addAndMakeVisible (label_N_nMics.get());
    label_N_nMics->setFont (juce::FontOptions (15.00f, juce::Font::plain).withStyle ("Regular"));
    label_N_nMics->setJustificationType (juce::Justification::centredLeft);
    label_N_nMics->setEditable (false, false, false);
    label_N_nMics->setBounds (164, 97, 51, 20);

    label_N_nDirs_array.reset (new juce::Label ("new label", juce::String()));
    addAndMakeVisible (label_N_nDirs_array.get());
    label_N_nDirs_array->setFont (juce::FontOptions (15.00f, juce::Font::plain).withStyle ("Regular"));
    label_N_nDirs_array->setJustificationType (juce::Justification::centredLeft);
    label_N_nDirs_array->setEditable (false, false, false);
    label_N_nDirs_array->setBounds (164, 120, 51, 20);

    label_IR_length_array.reset (new juce::Label ("new label", juce::String()));
    addAndMakeVisible (label_IR_length_array.get());
    label_IR_length_array->setFont (juce::FontOptions (15.00f, juce::Font::plain).withStyle ("Regular"));
    label_IR_length_array->setJustificationType (juce::Justification::centredLeft);
    label_IR_length_array->setEditable (false, false, false);
    label_IR_length_array->setBounds (164, 143, 51, 20);

    TBuseDefaultHRIRs.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (TBuseDefaultHRIRs.get());
    TBuseDefaultHRIRs->setButtonText (juce::String());
    TBuseDefaultHRIRs->addListener (this);
    TBuseDefaultHRIRs->setBounds (400, 91, 32, 24);

    label_HRIR_fs_bin.reset (new juce::Label ("new label", juce::String()));
    addAndMakeVisible (label_HRIR_fs_bin.get());
    label_HRIR_fs_bin->setFont (juce::FontOptions (15.00f, juce::Font::plain).withStyle ("Regular"));
    label_HRIR_fs_bin->setJustificationType (juce::Justification::centredLeft);
    label_HRIR_fs_bin->setEditable (false, false, false);
    label_HRIR_fs_bin->setBounds (377, 166, 51, 20);

    label_DAW_fs_bin.reset (new juce::Label ("new label", juce::String()));
    addAndMakeVisible (label_DAW_fs_bin.get());
    label_DAW_fs_bin->setFont (juce::FontOptions (15.00f, juce::Font::plain).withStyle ("Regular"));
    label_DAW_fs_bin->setJustificationType (juce::Justification::centredLeft);
    label_DAW_fs_bin->setEditable (false, false, false);
    label_DAW_fs_bin->setBounds (377, 189, 51, 20);

    label_N_nDirs_bin.reset (new juce::Label ("new label", juce::String()));
    addAndMakeVisible (label_N_nDirs_bin.get());
    label_N_nDirs_bin->setFont (juce::FontOptions (15.00f, juce::Font::plain).withStyle ("Regular"));
    label_N_nDirs_bin->setJustificationType (juce::Justification::centredLeft);
    label_N_nDirs_bin->setEditable (false, false, false);
    label_N_nDirs_bin->setBounds (377, 121, 51, 20);

    label_IR_length_bin.reset (new juce::Label ("new label", juce::String()));
    addAndMakeVisible (label_IR_length_bin.get());
    label_IR_length_bin->setFont (juce::FontOptions (15.00f, juce::Font::plain).withStyle ("Regular"));
    label_IR_length_bin->setJustificationType (juce::Justification::centredLeft);
    label_IR_length_bin->setEditable (false, false, false);
    label_IR_length_bin->setBounds (377, 143, 51, 20);

    TBenableCM.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (TBenableCM.get());
    TBenableCM->setButtonText (juce::String());
    TBenableCM->addListener (this);
    TBenableCM->setBounds (195, 310, 32, 24);

    SL_analysis_avg.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (SL_analysis_avg.get());
    SL_analysis_avg->setRange (0, 1, 0.01);
    SL_analysis_avg->setSliderStyle (juce::Slider::LinearHorizontal);
    SL_analysis_avg->setTextBoxStyle (juce::Slider::TextBoxRight, false, 45, 20);
    SL_analysis_avg->addListener (this);
    SL_analysis_avg->setBounds (328, 241, 98, 22);

    SL_synthesis_avg.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (SL_synthesis_avg.get());
    SL_synthesis_avg->setRange (0, 1, 0.01);
    SL_synthesis_avg->setSliderStyle (juce::Slider::LinearHorizontal);
    SL_synthesis_avg->setTextBoxStyle (juce::Slider::TextBoxRight, false, 45, 20);
    SL_synthesis_avg->addListener (this);
    SL_synthesis_avg->setBounds (328, 265, 98, 22);

    SL_left_ref_idx.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (SL_left_ref_idx.get());
    SL_left_ref_idx->setRange (0, 64, 1);
    SL_left_ref_idx->setSliderStyle (juce::Slider::LinearHorizontal);
    SL_left_ref_idx->setTextBoxStyle (juce::Slider::TextBoxRight, false, 45, 20);
    SL_left_ref_idx->addListener (this);
    SL_left_ref_idx->setBounds (394, 289, 32, 20);

    SL_right_ref_idx.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (SL_right_ref_idx.get());
    SL_right_ref_idx->setRange (0, 64, 1);
    SL_right_ref_idx->setSliderStyle (juce::Slider::LinearHorizontal);
    SL_right_ref_idx->setTextBoxStyle (juce::Slider::TextBoxRight, false, 45, 20);
    SL_right_ref_idx->addListener (this);
    SL_right_ref_idx->setBounds (394, 313, 32, 20);

    CBdoaEstimator.reset (new juce::ComboBox ("new combo box"));
    addAndMakeVisible (CBdoaEstimator.get());
    CBdoaEstimator->setEditableText (false);
    CBdoaEstimator->setJustificationType (juce::Justification::centredLeft);
    CBdoaEstimator->setTextWhenNothingSelected (TRANS("Default"));
    CBdoaEstimator->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    CBdoaEstimator->addListener (this);
    CBdoaEstimator->setBounds (122, 245, 98, 20);

    CBdiffEstimator.reset (new juce::ComboBox ("new combo box"));
    addAndMakeVisible (CBdiffEstimator.get());
    CBdiffEstimator->setEditableText (false);
    CBdiffEstimator->setJustificationType (juce::Justification::centredLeft);
    CBdiffEstimator->setTextWhenNothingSelected (TRANS("Default"));
    CBdiffEstimator->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    CBdiffEstimator->addListener (this);
    CBdiffEstimator->setBounds (122, 267, 98, 20);

    setSize (842, 448);

    /* handles */
    hHdR = processor.getFXHandle();

    /* Look and Feel */
    setLookAndFeel(&LAF);

    /* remove slider bit of these sliders */
    SL_left_ref_idx->setColour(Slider::trackColourId, Colours::transparentBlack);
    SL_left_ref_idx->setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    SL_left_ref_idx->setSliderSnapsToMousePosition(false);
    SL_right_ref_idx->setColour(Slider::trackColourId, Colours::transparentBlack);
    SL_right_ref_idx->setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    SL_right_ref_idx->setSliderSnapsToMousePosition(false);

    /* add combobox options */
    CBdoaEstimator->addItem(TRANS("MUSIC"), HADES_RENDERER_USE_MUSIC);
    CBdiffEstimator->addItem(TRANS("COMEDIE"), HADES_RENDERER_USE_COMEDIE);
    CBbeamformer->addItem(TRANS("None"), HADES_RENDERER_BEAMFORMER_NONE);
    CBbeamformer->addItem(TRANS("FaS"), HADES_RENDERER_BEAMFORMER_FILTER_AND_SUM);
    CBbeamformer->addItem(TRANS("BMVDR"), HADES_RENDERER_BEAMFORMER_BMVDR);

    /* create 2d Sliders */
    streamBalance2dSlider.reset (new log2dSlider(360, 62, 100, 20e3, 0, 2, 2));
    addAndMakeVisible (streamBalance2dSlider.get());
    streamBalance2dSlider->setAlwaysOnTop(true);
    streamBalance2dSlider->setTopLeftPosition(25, 360);
    streamBalance2dSlider->setRefreshValuesFLAG(true);

    /* Radial editor window */
    float* pDirGain;
    hades_renderer_getRadialEditorPtr(hHdR, &pDirGain);
    dirGainEditor.reset (new EditWindow360(pDirGain, -12.0f, 6.0f, 1));
    addAndMakeVisible (dirGainEditor.get());
    dirGainEditor->setAlwaysOnTop(true);
    dirGainEditor->setTopLeftPosition(448, 58);

    /* file loaders */
    addAndMakeVisible (fileChooserMAIR);
    fileChooserMAIR.addListener (this);
    fileChooserMAIR.setBounds (20, 65, 198, 20);
    StringArray filenames;
    filenames.add(hades_renderer_getSofaFilePathMAIR(hHdR));
    fileChooserMAIR.setRecentlyUsedFilenames(filenames);
    fileChooserMAIR.setFilenameIsEditable(true);
    addAndMakeVisible (fileChooserHRIR);
    fileChooserHRIR.addListener (this);
    fileChooserHRIR.setBounds (229, 65, 198, 20);
    StringArray filenames2;
    filenames2.add(hades_renderer_getSofaFilePathMAIR(hHdR));
    fileChooserHRIR.setRecentlyUsedFilenames(filenames2);
    fileChooserHRIR.setFilenameIsEditable(true);

    /* ProgressBar */
    progress = 0.0;
    progressbar.setBounds(getLocalBounds().getCentreX()-175, getLocalBounds().getCentreY()-17, 350, 35);
    progressbar.ProgressBar::setAlwaysOnTop(true);
    progressbar.setColour(ProgressBar::backgroundColourId, Colours::gold);
    progressbar.setColour(ProgressBar::foregroundColourId, Colours::white);

    /* grab current parameter settings */
    s_diff2dir->setValue(hades_renderer_getStreamBalanceAllBands(hHdR), dontSendNotification);
    CBdoaEstimator->setSelectedId((int)hades_renderer_getDoAestimator(hHdR), dontSendNotification);
    CBdiffEstimator->setSelectedId((int)hades_renderer_getDiffusenessEstimator(hHdR), dontSendNotification);
    CBbeamformer->setSelectedId((int)hades_renderer_getBeamformer(hHdR), dontSendNotification);
    TBuseDefaultHRIRs->setToggleState((bool)hades_renderer_getUseDefaultHRIRsflag(hHdR), dontSendNotification);
    TBenableCM->setToggleState((bool)hades_renderer_getEnableCovMatching(hHdR), dontSendNotification);
    SL_analysis_avg->setValue((double)hades_renderer_getAnalysisAveraging(hHdR), dontSendNotification);
    SL_synthesis_avg->setValue((double)hades_renderer_getSynthesisAveraging(hHdR), dontSendNotification);
    SL_left_ref_idx->setValue((double)hades_renderer_getReferenceSensorIndex(hHdR,0), dontSendNotification);
    SL_right_ref_idx->setValue((double)hades_renderer_getReferenceSensorIndex(hHdR,1), dontSendNotification);
    label_IR_fs_array->setText(String(hades_renderer_getIRsamplerateArray(hHdR)), dontSendNotification);
    label_DAW_fs_array->setText(String(hades_renderer_getDAWsamplerate(hHdR)), dontSendNotification);
    label_N_nMics->setText(String(hades_renderer_getNmicsArray(hHdR)), dontSendNotification);
    label_N_nDirs_array->setText(String(hades_renderer_getNDirsArray(hHdR)), dontSendNotification);
    label_IR_length_array->setText(String(hades_renderer_getIRlengthArray(hHdR)), dontSendNotification);
    label_HRIR_fs_bin->setText(String(hades_renderer_getIRsamplerateBin(hHdR)), dontSendNotification);
    label_DAW_fs_bin->setText(String(hades_renderer_getDAWsamplerate(hHdR)), dontSendNotification);
    label_N_nDirs_bin->setText(String(hades_renderer_getNDirsBin(hHdR)), dontSendNotification);
    label_IR_length_bin->setText(String(hades_renderer_getIRlengthBin(hHdR)), dontSendNotification);

    /* Plugin description */
    pluginDescription.reset (new juce::ComboBox ("new combo box"));
    addAndMakeVisible (pluginDescription.get());
    pluginDescription->setBounds (0, 0, 200, 32);
    pluginDescription->setAlpha(0.0f);
    pluginDescription->setEnabled(false);
//    pluginDescription->setTooltip(TRANS(""));
    addAndMakeVisible (publicationLink);
    publicationLink.setColour (HyperlinkButton::textColourId, Colours::lightblue);
    publicationLink.setBounds(getBounds().getWidth()-80, 4, 80, 12);
    publicationLink.setJustificationType(Justification::centredLeft);

    /* Specify screen refresh rate */
    startTimer(100);

    currentWarning = k_warning_none;
}

PluginEditor::~PluginEditor()
{
    stopTimer();
    setLookAndFeel(nullptr);
}

void PluginEditor::paint (juce::Graphics& g)
{
    using namespace ColoursUI;

    drawPluginBackgroundAndBanner(g, getBounds());

    /* Panels */
    drawPanel(g, {12, 58,212,158}, panelFill,      panelStroke);
    drawPanel(g, {12, 58,212, 33}, panelFillLight, panelStroke);
    drawPanel(g, {223,58,213,158}, panelFill, panelStroke);
    drawPanel(g, {223,58,213, 58}, panelFillLight, panelStroke);
    drawPanelRect(g, {448, 58,382,382}, panelFill, panelStroke);
    drawPanel(g, {12,238,422, 98}, panelFill, panelStroke);
    drawPanel(g, {12,335,422,105}, panelFill, panelStroke);

    /* Title */
    drawLabel(g, {12,1,120,32}, "HADES|", 18.f);
    drawLabel(g, {82,1,106,32}, "Renderer", 18.f, juce::Justification::centredLeft, juce::Colour(0xff00ffe6));

    /* Section titles */
    drawLabel(g, {66, 33,149,30}, "Load Array IRs", 15.f);
    drawLabel(g, {290,33,149,30}, "Load HRIRs",     15.f);
    drawLabel(g, {504,33,328,30}, "Direct Stream Gain Over Direction (dB)", 15.f);

    /* Left‑column labels */
    drawLabel(g, {22, 91,138,30}, "Number of Sensors:",    13.5f);
    drawLabel(g, {22,114,170,30}, "Number of Directions:", 13.5f);
    drawLabel(g, {22,137,162,30}, "IR length:",            13.5f);
    drawLabel(g, {22,159,162,30}, "IR SampleRate:",        13.5f);
    drawLabel(g, {22,184,162,30}, "DAW SampleRate:",       13.5f);

    /* Middle‑column labels */
    drawLabel(g, {232,88,173,30}, "Use Default HRIR set:", 15.f);
    drawLabel(g, {230,115,170,30}, "Number of Directions:", 13.5f);
    drawLabel(g, {230,137,162,30}, "HRIR length:",          13.5f);
    drawLabel(g, {230,161,162,30}, "HRIR SampleRate:",      13.5f);
    drawLabel(g, {230,184,162,30}, "DAW SampleRate:",       13.5f);

    /* Rendering Settings */
    drawLabel(g, {166,215,149,30}, "Rendering Settings", 15.f);
    drawLabel(g, {20,238,162,30}, "DoA Estimator:",            13.5f);
    drawLabel(g, {20,262,162,30}, "Diff Estimator:",           13.5f);
    drawLabel(g, {20,284,162,30}, "Beamforming:",              13.5f);
    drawLabel(g, {20,307,196,30}, "Enable Covariance Matching:", 13.5f);
    drawLabel(g, {236,237,162,30}, "Analysis Avg:",           13.5f);
    drawLabel(g, {236,261,162,30}, "Synthesis Avg:",          13.5f);
    drawLabel(g, {236,285,164,30}, "Left Ref Sensor Index:",  13.5f);
    drawLabel(g, {236,309,164,30}, "Right Ref Sensor Index:", 13.5f);

    /* Diffuse to Direct block */
    drawLabel(g, {21,331,331,30}, "Diffuse to Direct Balance Per Frequency Band", 15.f);

    /* Frequency labels */
    drawLabel(g, {19,415,35,30},  "100", 12.f);
    drawLabel(g, {174,415,35,30}, "1k",  12.f);
    drawLabel(g, {328,415,35,30}, "10k", 12.f);
    drawLabel(g, {210,415,91,30}, "Frequency (Hz)", 12.f);
    drawLabel(g, {371,415,35,30}, "20k", 12.f);

    /* Diff / Dir markers */
    drawLabel(g, {413,342,17,30}, "Dir",  11.f);
    drawLabel(g, {413,413,27,30}, "Diff", 11.f);

    g.setColour(Colours::white);
    g.setFont(juce::FontOptions (11.00f, Font::plain));
    g.drawText(TRANS("Ver ") + JucePlugin_VersionString + BUILD_VER_SUFFIX + TRANS(", Build Date ") + __DATE__ + TRANS(" "),
        175, 16, 530, 11,
        Justification::centredLeft, true);

    /* display warning message */
    g.setColour(Colours::red);
    g.setFont(juce::FontOptions (11.00f, Font::plain));
    switch (currentWarning){
        case k_warning_none:
            break;
        case k_warning_frameSize:
            g.drawText(TRANS("Set frame size to multiple of ") + String(hades_renderer_getFrameSize()),
                       getBounds().getWidth()-225, 16, 530, 11,
                       Justification::centredLeft, true);
            break;
        case k_warning_supported_fs:
            g.drawText(TRANS("Sample rate (") + String(hades_renderer_getDAWsamplerate(hHdR)) + TRANS(") is unsupported"),
                       getBounds().getWidth()-225, 16, 530, 11,
                       Justification::centredLeft, true);
            break;
        case k_warning_mismatch_fs:
            g.drawText(TRANS("Sample rate mismatch between DAW/IRs/HRIRs"),
                       getBounds().getWidth()-225, 16, 530, 11,
                       Justification::centredLeft, true);
            break;
        case k_warning_NinputCH:
            g.drawText(TRANS("Insufficient number of input channels (") + String(processor.getTotalNumInputChannels()) +
                       TRANS("/") + String(hades_renderer_getNmicsArray(hHdR)) + TRANS(")"),
                       getBounds().getWidth()-225, 16, 530, 11,
                       Justification::centredLeft, true);
            break;
        case k_warning_NoutputCH:
            g.drawText(TRANS("Insufficient number of output channels (") + String(processor.getTotalNumOutputChannels()) +
                       TRANS("/") + String(2) + TRANS(")"),
                       getBounds().getWidth()-225, 16, 530, 11,
                       Justification::centredLeft, true);
            break;
    }
}

void PluginEditor::resized()
{
    repaint();
}

void PluginEditor::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == s_diff2dir.get())
    {
        hades_renderer_setStreamBalanceAllBands(hHdR, (float)s_diff2dir->getValue());
        streamBalance2dSlider->setRefreshValuesFLAG(true);
    }
    else if (sliderThatWasMoved == SL_analysis_avg.get())
    {
        hades_renderer_setAnalysisAveraging(hHdR, (float)SL_analysis_avg->getValue());
    }
    else if (sliderThatWasMoved == SL_synthesis_avg.get())
    {
        hades_renderer_setSynthesisAveraging(hHdR, (float)SL_synthesis_avg->getValue());
    }
    else if (sliderThatWasMoved == SL_left_ref_idx.get())
    {
        hades_renderer_setReferenceSensorIndex(hHdR, 0, (int)SL_left_ref_idx->getValue());
    }
    else if (sliderThatWasMoved == SL_right_ref_idx.get())
    {
        hades_renderer_setReferenceSensorIndex(hHdR, 1, (int)SL_right_ref_idx->getValue());
    }
}

void PluginEditor::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == CBbeamformer.get())
    {
        hades_renderer_setBeamformer(hHdR, (HADES_RENDERER_BEAMFORMER_TYPE)CBbeamformer->getSelectedId());
    }
    else if (comboBoxThatHasChanged == CBdoaEstimator.get())
    {
        hades_renderer_setDoAestimator(hHdR, (HADES_RENDERER_DOA_ESTIMATORS)CBdoaEstimator->getSelectedId());
    }
    else if (comboBoxThatHasChanged == CBdiffEstimator.get())
    {
        hades_renderer_setDiffusenessEstimator(hHdR, (HADES_RENDERER_DIFFUSENESS_ESTIMATORS)CBdiffEstimator->getSelectedId());
    }
}

void PluginEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == TBuseDefaultHRIRs.get())
    {
        hades_renderer_setUseDefaultHRIRsflag(hHdR, (int)TBuseDefaultHRIRs->getToggleState());
    }
    else if (buttonThatWasClicked == TBenableCM.get())
    {
        hades_renderer_setEnableCovMatching(hHdR, (int)TBenableCM->getToggleState());
    }
}

void PluginEditor::timerCallback()
{
    /* parameters whos values can change internally should be periodically refreshed */
    s_diff2dir->setValue(hades_renderer_getStreamBalanceAllBands(hHdR), dontSendNotification);
    SL_left_ref_idx->setValue((double)hades_renderer_getReferenceSensorIndex(hHdR,0), dontSendNotification);
    SL_right_ref_idx->setValue((double)hades_renderer_getReferenceSensorIndex(hHdR,1), dontSendNotification);
    label_IR_fs_array->setText(String(hades_renderer_getIRsamplerateArray(hHdR)), dontSendNotification);
    label_DAW_fs_array->setText(String(hades_renderer_getDAWsamplerate(hHdR)), dontSendNotification);
    label_N_nMics->setText(String(hades_renderer_getNmicsArray(hHdR)), dontSendNotification);
    label_N_nDirs_array->setText(String(hades_renderer_getNDirsArray(hHdR)), dontSendNotification);
    label_IR_length_array->setText(String(hades_renderer_getIRlengthArray(hHdR)), dontSendNotification);
    label_HRIR_fs_bin->setText(String(hades_renderer_getIRsamplerateBin(hHdR)), dontSendNotification);
    label_DAW_fs_bin->setText(String(hades_renderer_getDAWsamplerate(hHdR)), dontSendNotification);
    label_N_nDirs_bin->setText(String(hades_renderer_getNDirsBin(hHdR)), dontSendNotification);
    label_IR_length_bin->setText(String(hades_renderer_getIRlengthBin(hHdR)), dontSendNotification);

    /* refresh */
    int nPoints;
    float* pX_vector;
    float* pY_values;
    if (streamBalance2dSlider->getRefreshValuesFLAG() && hades_renderer_getCodecStatus(hHdR)==CODEC_STATUS_INITIALISED){
        hades_renderer_setStreamBalanceFromLocal(hHdR);
        hades_renderer_getStreamBalanceLocalPtrs(hHdR, &pX_vector, &pY_values, &nPoints);
        streamBalance2dSlider->setDataHandles(pX_vector, pY_values, nPoints);
        streamBalance2dSlider->repaint();
        streamBalance2dSlider->setRefreshValuesFLAG(false);
    }
    dirGainEditor->refresh();

    /* Progress bar */
    if(hades_renderer_getCodecStatus(hHdR)==CODEC_STATUS_INITIALISING){
        addAndMakeVisible(progressbar);
        progress = (double)hades_renderer_getProgressBar0_1(hHdR);
        char text[HADES_PROGRESSBARTEXT_CHAR_LENGTH];
        hades_renderer_getProgressBarText(hHdR, (char*)text);
        progressbar.setTextToDisplay(String(text));
    }
    else
        removeChildComponent(&progressbar);

    /* Some parameters shouldn't be editable during initialisation*/
    if(hades_renderer_getCodecStatus(hHdR)==CODEC_STATUS_INITIALISING){
        if(fileChooserMAIR.isEnabled())
            fileChooserMAIR.setEnabled(false);
        if(fileChooserHRIR.isEnabled())
            fileChooserHRIR.setEnabled(false);
    }
    else {
        if(!fileChooserMAIR.isEnabled())
            fileChooserMAIR.setEnabled(true);
        if(!fileChooserHRIR.isEnabled())
            fileChooserHRIR.setEnabled(true);
    }

    /* display warning message, if needed */
    if ((processor.getCurrentBlockSize() % hades_renderer_getFrameSize()) != 0){
        currentWarning = k_warning_frameSize;
        repaint(0,0,getWidth(),32);
    }
    else if ( !((hades_renderer_getDAWsamplerate(hHdR) == 44.1e3) || (hades_renderer_getDAWsamplerate(hHdR) == 48e3)) ){
        currentWarning = k_warning_supported_fs;
        repaint(0,0,getWidth(),32);
    }
    else if ((hades_renderer_getDAWsamplerate(hHdR) != hades_renderer_getIRsamplerateArray(hHdR)) ||
              hades_renderer_getDAWsamplerate(hHdR) != hades_renderer_getIRsamplerateBin(hHdR)){
        currentWarning = k_warning_mismatch_fs;
        repaint(0,0,getWidth(),32);
    }
    else if (processor.getCurrentNumInputs() < hades_renderer_getNmicsArray(hHdR)){
        currentWarning = k_warning_NinputCH;
        repaint(0,0,getWidth(),32);
    }
    else if (processor.getCurrentNumOutputs() < 2){
        currentWarning = k_warning_NoutputCH;
        repaint(0,0,getWidth(),32);
    }
    else if(currentWarning){
        currentWarning = k_warning_none;
        repaint(0,0,getWidth(),32);
    }
}
