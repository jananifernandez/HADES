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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "PluginEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

//[/MiscUserDefs]

//==============================================================================
PluginEditor::PluginEditor (PluginProcessor* ownerFilter)
    : AudioProcessorEditor(ownerFilter), progressbar(progress), fileChooserMAIR ("File", File(), true, false, false,
      "*.sofa;*.nc;", String(),
      "Load SOFA File"), fileChooserHRIR ("File", File(), true, false, false,
      "*.sofa;*.nc;", String(),
      "Load SOFA File")
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    s_diff2dir.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (s_diff2dir.get());
    s_diff2dir->setRange (0, 2, 0.01);
    s_diff2dir->setSliderStyle (juce::Slider::LinearVertical);
    s_diff2dir->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    s_diff2dir->setColour (juce::Slider::backgroundColourId, juce::Colour (0xff5c5d5e));
    s_diff2dir->setColour (juce::Slider::trackColourId, juce::Colour (0xff315b6d));
    s_diff2dir->setColour (juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_diff2dir->setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0x00ffffff));
    s_diff2dir->addListener (this);

    s_diff2dir->setBounds (384, 354, 40, 74);

    label_IR_fs_array.reset (new juce::Label ("new label",
                                              juce::String()));
    addAndMakeVisible (label_IR_fs_array.get());
    label_IR_fs_array->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label_IR_fs_array->setJustificationType (juce::Justification::centredLeft);
    label_IR_fs_array->setEditable (false, false, false);
    label_IR_fs_array->setColour (juce::Label::outlineColourId, juce::Colour (0x68a3a2a2));
    label_IR_fs_array->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label_IR_fs_array->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label_IR_fs_array->setBounds (164, 166, 51, 20);

    label_DAW_fs_array.reset (new juce::Label ("new label",
                                               juce::String()));
    addAndMakeVisible (label_DAW_fs_array.get());
    label_DAW_fs_array->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label_DAW_fs_array->setJustificationType (juce::Justification::centredLeft);
    label_DAW_fs_array->setEditable (false, false, false);
    label_DAW_fs_array->setColour (juce::Label::outlineColourId, juce::Colour (0x68a3a2a2));
    label_DAW_fs_array->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label_DAW_fs_array->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label_DAW_fs_array->setBounds (164, 189, 51, 20);

    CBbeamformer.reset (new juce::ComboBox ("new combo box"));
    addAndMakeVisible (CBbeamformer.get());
    CBbeamformer->setEditableText (false);
    CBbeamformer->setJustificationType (juce::Justification::centredLeft);
    CBbeamformer->setTextWhenNothingSelected (TRANS("Default"));
    CBbeamformer->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    CBbeamformer->addListener (this);

    CBbeamformer->setBounds (122, 289, 98, 20);

    label_N_nMics.reset (new juce::Label ("new label",
                                          juce::String()));
    addAndMakeVisible (label_N_nMics.get());
    label_N_nMics->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label_N_nMics->setJustificationType (juce::Justification::centredLeft);
    label_N_nMics->setEditable (false, false, false);
    label_N_nMics->setColour (juce::Label::outlineColourId, juce::Colour (0x68a3a2a2));
    label_N_nMics->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label_N_nMics->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label_N_nMics->setBounds (164, 97, 51, 20);

    label_N_nDirs_array.reset (new juce::Label ("new label",
                                                juce::String()));
    addAndMakeVisible (label_N_nDirs_array.get());
    label_N_nDirs_array->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label_N_nDirs_array->setJustificationType (juce::Justification::centredLeft);
    label_N_nDirs_array->setEditable (false, false, false);
    label_N_nDirs_array->setColour (juce::Label::outlineColourId, juce::Colour (0x68a3a2a2));
    label_N_nDirs_array->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label_N_nDirs_array->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label_N_nDirs_array->setBounds (164, 120, 51, 20);

    label_IR_length_array.reset (new juce::Label ("new label",
                                                  juce::String()));
    addAndMakeVisible (label_IR_length_array.get());
    label_IR_length_array->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label_IR_length_array->setJustificationType (juce::Justification::centredLeft);
    label_IR_length_array->setEditable (false, false, false);
    label_IR_length_array->setColour (juce::Label::outlineColourId, juce::Colour (0x68a3a2a2));
    label_IR_length_array->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label_IR_length_array->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label_IR_length_array->setBounds (164, 143, 51, 20);

    TBuseDefaultHRIRs.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (TBuseDefaultHRIRs.get());
    TBuseDefaultHRIRs->setButtonText (juce::String());
    TBuseDefaultHRIRs->addListener (this);

    TBuseDefaultHRIRs->setBounds (400, 91, 32, 24);

    label_HRIR_fs_bin.reset (new juce::Label ("new label",
                                              juce::String()));
    addAndMakeVisible (label_HRIR_fs_bin.get());
    label_HRIR_fs_bin->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label_HRIR_fs_bin->setJustificationType (juce::Justification::centredLeft);
    label_HRIR_fs_bin->setEditable (false, false, false);
    label_HRIR_fs_bin->setColour (juce::Label::outlineColourId, juce::Colour (0x68a3a2a2));
    label_HRIR_fs_bin->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label_HRIR_fs_bin->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label_HRIR_fs_bin->setBounds (377, 166, 51, 20);

    label_DAW_fs_bin.reset (new juce::Label ("new label",
                                             juce::String()));
    addAndMakeVisible (label_DAW_fs_bin.get());
    label_DAW_fs_bin->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label_DAW_fs_bin->setJustificationType (juce::Justification::centredLeft);
    label_DAW_fs_bin->setEditable (false, false, false);
    label_DAW_fs_bin->setColour (juce::Label::outlineColourId, juce::Colour (0x68a3a2a2));
    label_DAW_fs_bin->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label_DAW_fs_bin->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label_DAW_fs_bin->setBounds (377, 189, 51, 20);

    label_N_nDirs_bin.reset (new juce::Label ("new label",
                                              juce::String()));
    addAndMakeVisible (label_N_nDirs_bin.get());
    label_N_nDirs_bin->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label_N_nDirs_bin->setJustificationType (juce::Justification::centredLeft);
    label_N_nDirs_bin->setEditable (false, false, false);
    label_N_nDirs_bin->setColour (juce::Label::outlineColourId, juce::Colour (0x68a3a2a2));
    label_N_nDirs_bin->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label_N_nDirs_bin->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label_N_nDirs_bin->setBounds (377, 121, 51, 20);

    label_IR_length_bin.reset (new juce::Label ("new label",
                                                juce::String()));
    addAndMakeVisible (label_IR_length_bin.get());
    label_IR_length_bin->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label_IR_length_bin->setJustificationType (juce::Justification::centredLeft);
    label_IR_length_bin->setEditable (false, false, false);
    label_IR_length_bin->setColour (juce::Label::outlineColourId, juce::Colour (0x68a3a2a2));
    label_IR_length_bin->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label_IR_length_bin->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

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
    SL_analysis_avg->setColour (juce::Slider::backgroundColourId, juce::Colour (0xff5c5d5e));
    SL_analysis_avg->setColour (juce::Slider::trackColourId, juce::Colour (0xff315b6e));
    SL_analysis_avg->addListener (this);

    SL_analysis_avg->setBounds (328, 241, 98, 22);

    SL_synthesis_avg.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (SL_synthesis_avg.get());
    SL_synthesis_avg->setRange (0, 1, 0.01);
    SL_synthesis_avg->setSliderStyle (juce::Slider::LinearHorizontal);
    SL_synthesis_avg->setTextBoxStyle (juce::Slider::TextBoxRight, false, 45, 20);
    SL_synthesis_avg->setColour (juce::Slider::backgroundColourId, juce::Colour (0xff5c5d5e));
    SL_synthesis_avg->setColour (juce::Slider::trackColourId, juce::Colour (0xff315b6e));
    SL_synthesis_avg->addListener (this);

    SL_synthesis_avg->setBounds (328, 265, 98, 22);

    SL_left_ref_idx.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (SL_left_ref_idx.get());
    SL_left_ref_idx->setRange (0, 64, 1);
    SL_left_ref_idx->setSliderStyle (juce::Slider::LinearHorizontal);
    SL_left_ref_idx->setTextBoxStyle (juce::Slider::TextBoxRight, false, 45, 20);
    SL_left_ref_idx->setColour (juce::Slider::backgroundColourId, juce::Colour (0xff5c5d5e));
    SL_left_ref_idx->setColour (juce::Slider::trackColourId, juce::Colour (0xff315b6e));
    SL_left_ref_idx->addListener (this);

    SL_left_ref_idx->setBounds (394, 289, 32, 20);

    SL_right_ref_idx.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (SL_right_ref_idx.get());
    SL_right_ref_idx->setRange (0, 64, 1);
    SL_right_ref_idx->setSliderStyle (juce::Slider::LinearHorizontal);
    SL_right_ref_idx->setTextBoxStyle (juce::Slider::TextBoxRight, false, 45, 20);
    SL_right_ref_idx->setColour (juce::Slider::backgroundColourId, juce::Colour (0xff5c5d5e));
    SL_right_ref_idx->setColour (juce::Slider::trackColourId, juce::Colour (0xff315b6e));
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


    //[UserPreSize]
    //[/UserPreSize]

    setSize (842, 448);


    //[Constructor] You can add your own custom stuff here..

    /* handles */
    hVst = ownerFilter;
    hHdR = hVst->getFXHandle();

    /* init OpenGL */
#ifndef PLUGIN_EDITOR_DISABLE_OPENGL
    openGLContext.setMultisamplingEnabled(true);
    openGLContext.attachTo(*this);
#endif

    /* Look and Feel */
    LAF.setDefaultColours();
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

    /* tooltips */
//    CBdecOrder->setTooltip("Decoding order. Note that the plug-in will require (order+1)^2 Ambisonic (spherical harmonic) signals as input.");
//    fileChooser.setTooltip("Optionally, a custom HRIR set may be loaded via the SOFA standard. Note that if the plug-in fails to load the specified .sofa file, it will revert to the default HRIR data.");
//    SL_diffusionLevel->setTooltip("Slider to control the decorrelation of the ambient stream (0: none, 1: fully decorrelated)");
//    s_cov_avg->setTooltip("Covariance matrix averaging coefficient (one-pole).");
//    s_synth_avg->setTooltip("Synthesis matrix averaging coefficient (one-pole).");
//    s_diff2dir->setTooltip("This sets the diffuse-to-direct balance for all frequencies (default is in the middle). Use the 2-D slider to change the balance for specific frequencies.");
//    s_decBal->setTooltip("This sets the decoding balance between linear (Ambisonics) and parametric (COMPASS) for all frequencies. Use the 2-D slider to change the balance for specific frequencies.");
//    CBchannelOrder->setTooltip("Ambisonic channel ordering convention (Note that AmbiX: ACN/SN3D).");
//    CBnormalisation->setTooltip("Ambisonic normalisation scheme (Note that AmbiX: ACN/SN3D).");
//    TBenableRotation->setTooltip("Enables/Disables sound-field rotation prior to decoding.");
//    s_yaw->setTooltip("Sets the 'Yaw' rotation angle (in degrees).");
//    s_pitch->setTooltip("Sets the 'Pitch' rotation angle (in degrees).");
//    s_roll->setTooltip("Sets the 'Roll' rotation angle (in degrees).");
//    t_flipYaw->setTooltip("Flips the sign (+/-) of the 'Yaw' rotation angle.");
//    t_flipPitch->setTooltip("Flips the sign (+/-) of the 'Pitch' rotation angle.");
//    t_flipRoll->setTooltip("Flips the sign (+/-) of the 'Roll' rotation angle.");
//    te_oscport->setTooltip("The OSC port at which to receive the rotation angles. To facilitate head-tracking, send the rotation angles (in degrees) to this port ID as a 3-element vector 'ypr[3]', following the yaw-pitch-roll convention.");
//    TBrpyFlag->setTooltip("If enabled, the plug-in will use the roll-pitch-yaw rotation order convention. If disabled, it will use the yaw-pitch-roll convention.");
//    label_N_dirs->setTooltip("Number of HRIR directions in the current HRIR set.");
//    label_N_Tri->setTooltip("Number of triangles found when computing the Convex Hull of the HRIR grid.");
//    label_HRIR_fs->setTooltip("Sampling rate used when measuring/modelling the HRIRs.");
//    label_DAW_fs->setTooltip("Current sampling rate, as dictated by the DAW/Host.");
//    CBdoaEstimator->setTooltip("Direction of arrival (DoA) estimator to use. Multiple Signal Classification (MUSIC), or Estimation of Signal Parameters via Rotational Invariance Techniques (ESPRIT). MUSIC is generally the more robust DoA estimator and is therefore the default, but the method relies on scanning a dense number of directions on the sphere to determine the source directions; therefore, it's quite computationally expensive. ESPRIT, on the other hand, does not rely on any scanning grid and instead directly extracts the source directions. This makes the method computationally efficient, with the penalty of (often slightly) less performance than MUSIC and is more restricted in the number of simultaneous directions it can estimate.");
//    CBprocMode->setTooltip("COMPASS formulation to use for processing.");

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
    startTimer(TIMER_GUI_RELATED, 60);

    currentWarning = k_warning_none;

    //[/Constructor]
}

PluginEditor::~PluginEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    s_diff2dir = nullptr;
    label_IR_fs_array = nullptr;
    label_DAW_fs_array = nullptr;
    CBbeamformer = nullptr;
    label_N_nMics = nullptr;
    label_N_nDirs_array = nullptr;
    label_IR_length_array = nullptr;
    TBuseDefaultHRIRs = nullptr;
    label_HRIR_fs_bin = nullptr;
    label_DAW_fs_bin = nullptr;
    label_N_nDirs_bin = nullptr;
    label_IR_length_bin = nullptr;
    TBenableCM = nullptr;
    SL_analysis_avg = nullptr;
    SL_synthesis_avg = nullptr;
    SL_left_ref_idx = nullptr;
    SL_right_ref_idx = nullptr;
    CBdoaEstimator = nullptr;
    CBdiffEstimator = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    setLookAndFeel(nullptr);
//    decBalance2dSlider = nullptr;
//    streamBalance2dSlider = nullptr;
    //[/Destructor]
}

//==============================================================================
void PluginEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colours::white);

    {
        int x = 0, y = 201, width = 842, height = 247;
        juce::Colour fillColour1 = juce::Colour (0xff19313f), fillColour2 = juce::Colour (0xff041518);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             8.0f - 0.0f + x,
                                             448.0f - 201.0f + y,
                                             fillColour2,
                                             8.0f - 0.0f + x,
                                             392.0f - 201.0f + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        int x = 0, y = 30, width = 842, height = 171;
        juce::Colour fillColour1 = juce::Colour (0xff19313f), fillColour2 = juce::Colour (0xff041518);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             8.0f - 0.0f + x,
                                             32.0f - 30.0f + y,
                                             fillColour2,
                                             8.0f - 0.0f + x,
                                             80.0f - 30.0f + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        int x = 448, y = 58, width = 382, height = 382;
        juce::Colour fillColour = juce::Colour (0x10f4f4f4);
        juce::Colour strokeColour = juce::Colour (0x67a0a0a0);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 1);

    }

    {
        float x = 1.0f, y = 2.0f, width = 840.0f, height = 31.0f;
        juce::Colour fillColour1 = juce::Colour (0xff041518), fillColour2 = juce::Colour (0xff19313f);
        juce::Colour strokeColour = juce::Colour (0xffb9b9b9);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             0.0f - 1.0f + x,
                                             32.0f - 2.0f + y,
                                             fillColour2,
                                             842.0f - 1.0f + x,
                                             32.0f - 2.0f + y,
                                             false));
        g.fillRoundedRectangle (x, y, width, height, 5.000f);
        g.setColour (strokeColour);
        g.drawRoundedRectangle (x, y, width, height, 5.000f, 2.000f);
    }

    {
        int x = 223, y = 58, width = 213, height = 158;
        juce::Colour fillColour = juce::Colour (0x10f4f4f4);
        juce::Colour strokeColour = juce::Colour (0x67a0a0a0);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 1);

    }

    {
        int x = 12, y = 58, width = 212, height = 158;
        juce::Colour fillColour = juce::Colour (0x10f4f4f4);
        juce::Colour strokeColour = juce::Colour (0x67a0a0a0);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 1);

    }

    {
        int x = 12, y = 58, width = 212, height = 33;
        juce::Colour fillColour = juce::Colour (0x08f4f4f4);
        juce::Colour strokeColour = juce::Colour (0x67a0a0a0);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 1);

    }

    {
        int x = 66, y = 33, width = 149, height = 30;
        juce::String text (TRANS("Load Array IRs"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 12, y = 335, width = 422, height = 105;
        juce::Colour fillColour = juce::Colour (0x10f4f4f4);
        juce::Colour strokeColour = juce::Colour (0x67a0a0a0);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 1);

    }

    {
        int x = 21, y = 331, width = 331, height = 30;
        juce::String text (TRANS("Diffuse to Direct Balance Per Frequency Band"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 19, y = 415, width = 35, height = 30;
        juce::String text (TRANS("100"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (12.00f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 174, y = 415, width = 35, height = 30;
        juce::String text (TRANS("1k"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (12.00f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 328, y = 415, width = 35, height = 30;
        juce::String text (TRANS("10k"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (12.00f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 210, y = 415, width = 91, height = 30;
        juce::String text (TRANS("Frequency (Hz)"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (12.00f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 371, y = 415, width = 35, height = 30;
        juce::String text (TRANS("20k"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (12.00f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 413, y = 413, width = 27, height = 30;
        juce::String text (TRANS("Diff"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 413, y = 342, width = 17, height = 30;
        juce::String text (TRANS("Dir"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 0, y = 0, width = 842, height = 2;
        juce::Colour strokeColour = juce::Colour (0xffb9b9b9);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 2);

    }

    {
        int x = 0, y = 0, width = 2, height = 448;
        juce::Colour strokeColour = juce::Colour (0xffb9b9b9);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 2);

    }

    {
        int x = 1298, y = 0, width = 2, height = 448;
        juce::Colour strokeColour = juce::Colour (0xffb9b9b9);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 2);

    }

    {
        int x = 0, y = 446, width = 1234, height = 2;
        juce::Colour strokeColour = juce::Colour (0xffb9b9b9);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 2);

    }

    {
        int x = 22, y = 91, width = 138, height = 30;
        juce::String text (TRANS("Number of Sensors:"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (13.50f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 22, y = 114, width = 170, height = 30;
        juce::String text (TRANS("Number of Directions:"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (13.50f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 22, y = 137, width = 162, height = 30;
        juce::String text (TRANS("IR length: "));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (13.50f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 22, y = 159, width = 162, height = 30;
        juce::String text (TRANS("IR SampleRate:  "));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (13.50f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 22, y = 184, width = 162, height = 30;
        juce::String text (TRANS("DAW SampleRate:  "));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (13.50f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 12, y = 1, width = 120, height = 32;
        juce::String text (TRANS("HADES|"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (18.80f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 80, y = 1, width = 106, height = 32;
        juce::String text (TRANS("Renderer"));
        juce::Colour fillColour = juce::Colour (0xff00ffe6);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (18.00f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 290, y = 33, width = 149, height = 30;
        juce::String text (TRANS("Load HRIRs"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 223, y = 58, width = 213, height = 58;
        juce::Colour fillColour = juce::Colour (0x08f4f4f4);
        juce::Colour strokeColour = juce::Colour (0x67a0a0a0);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 1);

    }

    {
        int x = 232, y = 88, width = 173, height = 30;
        juce::String text (TRANS("Use Default HRIR set:"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 230, y = 115, width = 170, height = 30;
        juce::String text (TRANS("Number of Directions:"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (13.50f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 230, y = 137, width = 162, height = 30;
        juce::String text (TRANS("HRIR length: "));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (13.50f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 230, y = 161, width = 162, height = 30;
        juce::String text (TRANS("HRIR SampleRate:  "));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (13.50f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 230, y = 184, width = 162, height = 30;
        juce::String text (TRANS("DAW SampleRate:  "));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (13.50f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 12, y = 240, width = 422, height = 96;
        juce::Colour fillColour = juce::Colour (0x10f4f4f4);
        juce::Colour strokeColour = juce::Colour (0x67a0a0a0);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 1);

    }

    {
        int x = 166, y = 215, width = 149, height = 30;
        juce::String text (TRANS("Rendering Settings"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 20, y = 284, width = 162, height = 30;
        juce::String text (TRANS("Beamforming:  "));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (13.50f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 20, y = 307, width = 196, height = 30;
        juce::String text (TRANS("Enable Covariance Matching:  "));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (13.50f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 236, y = 237, width = 162, height = 30;
        juce::String text (TRANS("Analysis Avg:  "));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (13.50f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 236, y = 261, width = 162, height = 30;
        juce::String text (TRANS("Synthesis Avg:  "));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (13.50f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 236, y = 285, width = 164, height = 30;
        juce::String text (TRANS("Left Ref Sensor Index:"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (13.50f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 20, y = 238, width = 162, height = 30;
        juce::String text (TRANS("DoA Estimator:  "));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (13.50f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 20, y = 262, width = 162, height = 30;
        juce::String text (TRANS("Diff Estimator:  "));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (13.50f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 236, y = 309, width = 164, height = 30;
        juce::String text (TRANS("Right Ref Sensor Index:"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (13.50f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 1232, y = 0, width = 2, height = 448;
        juce::Colour strokeColour = juce::Colour (0xffb9b9b9);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 2);

    }

    {
        int x = 504, y = 33, width = 328, height = 30;
        juce::String text (TRANS("Direct Stream Gain Over Direction (dB)"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 840, y = 0, width = 2, height = 448;
        juce::Colour strokeColour = juce::Colour (0xffb9b9b9);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 2);

    }

    //[UserPaint] Add your own custom painting code here..

    g.setColour(Colours::white);
	g.setFont(Font(11.00f, Font::plain));
	g.drawText(TRANS("Ver ") + JucePlugin_VersionString + BUILD_VER_SUFFIX + TRANS(", Build Date ") + __DATE__ + TRANS(" "),
		175, 16, 530, 11,
		Justification::centredLeft, true);

    /* display warning message */
    g.setColour(Colours::red);
    g.setFont(Font(11.00f, Font::plain));
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
            g.drawText(TRANS("Insufficient number of input channels (") + String(hVst->getTotalNumInputChannels()) +
                       TRANS("/") + String(hades_renderer_getNmicsArray(hHdR)) + TRANS(")"),
                       getBounds().getWidth()-225, 16, 530, 11,
                       Justification::centredLeft, true);
            break;
        case k_warning_NoutputCH:
            g.drawText(TRANS("Insufficient number of output channels (") + String(hVst->getTotalNumOutputChannels()) +
                       TRANS("/") + String(2) + TRANS(")"),
                       getBounds().getWidth()-225, 16, 530, 11,
                       Justification::centredLeft, true);
            break;
    }

    //[/UserPaint]
}

void PluginEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..


	repaint();
    //[/UserResized]
}

void PluginEditor::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == s_diff2dir.get())
    {
        //[UserSliderCode_s_diff2dir] -- add your slider handling code here..
        hades_renderer_setStreamBalanceAllBands(hHdR, (float)s_diff2dir->getValue());
        streamBalance2dSlider->setRefreshValuesFLAG(true);
        //[/UserSliderCode_s_diff2dir]
    }
    else if (sliderThatWasMoved == SL_analysis_avg.get())
    {
        //[UserSliderCode_SL_analysis_avg] -- add your slider handling code here..
        hades_renderer_setAnalysisAveraging(hHdR, (float)SL_analysis_avg->getValue());
        //[/UserSliderCode_SL_analysis_avg]
    }
    else if (sliderThatWasMoved == SL_synthesis_avg.get())
    {
        //[UserSliderCode_SL_synthesis_avg] -- add your slider handling code here..
        hades_renderer_setSynthesisAveraging(hHdR, (float)SL_synthesis_avg->getValue());
        //[/UserSliderCode_SL_synthesis_avg]
    }
    else if (sliderThatWasMoved == SL_left_ref_idx.get())
    {
        //[UserSliderCode_SL_left_ref_idx] -- add your slider handling code here..
        hades_renderer_setReferenceSensorIndex(hHdR, 0, (int)SL_left_ref_idx->getValue());
        //[/UserSliderCode_SL_left_ref_idx]
    }
    else if (sliderThatWasMoved == SL_right_ref_idx.get())
    {
        //[UserSliderCode_SL_right_ref_idx] -- add your slider handling code here..
        hades_renderer_setReferenceSensorIndex(hHdR, 1, (int)SL_right_ref_idx->getValue());
        //[/UserSliderCode_SL_right_ref_idx]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void PluginEditor::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == CBbeamformer.get())
    {
        //[UserComboBoxCode_CBbeamformer] -- add your combo box handling code here..
        hades_renderer_setBeamformer(hHdR, (HADES_RENDERER_BEAMFORMER_TYPE)CBbeamformer->getSelectedId());
        //[/UserComboBoxCode_CBbeamformer]
    }
    else if (comboBoxThatHasChanged == CBdoaEstimator.get())
    {
        //[UserComboBoxCode_CBdoaEstimator] -- add your combo box handling code here..
        hades_renderer_setDoAestimator(hHdR, (HADES_RENDERER_DOA_ESTIMATORS)CBdoaEstimator->getSelectedId());
        //[/UserComboBoxCode_CBdoaEstimator]
    }
    else if (comboBoxThatHasChanged == CBdiffEstimator.get())
    {
        //[UserComboBoxCode_CBdiffEstimator] -- add your combo box handling code here..
        hades_renderer_setDiffusenessEstimator(hHdR, (HADES_RENDERER_DIFFUSENESS_ESTIMATORS)CBdiffEstimator->getSelectedId());
        //[/UserComboBoxCode_CBdiffEstimator]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void PluginEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == TBuseDefaultHRIRs.get())
    {
        //[UserButtonCode_TBuseDefaultHRIRs] -- add your button handler code here..
        hades_renderer_setUseDefaultHRIRsflag(hHdR, (int)TBuseDefaultHRIRs->getToggleState());
        //[/UserButtonCode_TBuseDefaultHRIRs]
    }
    else if (buttonThatWasClicked == TBenableCM.get())
    {
        //[UserButtonCode_TBenableCM] -- add your button handler code here..
        hades_renderer_setEnableCovMatching(hHdR, (int)TBenableCM->getToggleState());
        //[/UserButtonCode_TBenableCM]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void PluginEditor::timerCallback(int timerID)
{
    switch(timerID){
        case TIMER_PROCESSING_RELATED:
            /* Handled in PluginProcessor */
            break;

        case TIMER_GUI_RELATED:

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
            if ((hVst->getCurrentBlockSize() % hades_renderer_getFrameSize()) != 0){
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
            else if (hVst->getCurrentNumInputs() < hades_renderer_getNmicsArray(hHdR)){
                currentWarning = k_warning_NinputCH;
                repaint(0,0,getWidth(),32);
            }
            else if (hVst->getCurrentNumOutputs() < 2){
                currentWarning = k_warning_NoutputCH;
                repaint(0,0,getWidth(),32);
            }
            else if(currentWarning){
                currentWarning = k_warning_none;
                repaint(0,0,getWidth(),32);
            }
            break;
    }
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PluginEditor" componentName=""
                 parentClasses="public AudioProcessorEditor, public MultiTimer, private FilenameComponentListener"
                 constructorParams="PluginProcessor* ownerFilter" variableInitialisers="AudioProcessorEditor(ownerFilter), progressbar(progress), fileChooserMAIR (&quot;File&quot;, File(), true, false, false,&#10;                       &quot;*.sofa;*.nc;&quot;, String(),&#10;                       &quot;Load SOFA File&quot;), fileChooserHRIR (&quot;File&quot;, File(), true, false, false,&#10;                       &quot;*.sofa;*.nc;&quot;, String(),&#10;                       &quot;Load SOFA File&quot;)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="842" initialHeight="448">
  <BACKGROUND backgroundColour="ffffffff">
    <RECT pos="0 201 842 247" fill="linear: 8 448, 8 392, 0=ff19313f, 1=ff041518"
          hasStroke="0"/>
    <RECT pos="0 30 842 171" fill="linear: 8 32, 8 80, 0=ff19313f, 1=ff041518"
          hasStroke="0"/>
    <RECT pos="448 58 382 382" fill="solid: 10f4f4f4" hasStroke="1" stroke="0.8, mitered, butt"
          strokeColour="solid: 67a0a0a0"/>
    <ROUNDRECT pos="1 2 840 31" cornerSize="5.0" fill="linear: 0 32, 842 32, 0=ff041518, 1=ff19313f"
               hasStroke="1" stroke="2, mitered, butt" strokeColour="solid: ffb9b9b9"/>
    <RECT pos="223 58 213 158" fill="solid: 10f4f4f4" hasStroke="1" stroke="0.8, mitered, butt"
          strokeColour="solid: 67a0a0a0"/>
    <RECT pos="12 58 212 158" fill="solid: 10f4f4f4" hasStroke="1" stroke="0.8, mitered, butt"
          strokeColour="solid: 67a0a0a0"/>
    <RECT pos="12 58 212 33" fill="solid: 8f4f4f4" hasStroke="1" stroke="0.8, mitered, butt"
          strokeColour="solid: 67a0a0a0"/>
    <TEXT pos="66 33 149 30" fill="solid: ffffffff" hasStroke="0" text="Load Array IRs"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <RECT pos="12 335 422 105" fill="solid: 10f4f4f4" hasStroke="1" stroke="0.8, mitered, butt"
          strokeColour="solid: 67a0a0a0"/>
    <TEXT pos="21 331 331 30" fill="solid: ffffffff" hasStroke="0" text="Diffuse to Direct Balance Per Frequency Band"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="19 415 35 30" fill="solid: ffffffff" hasStroke="0" text="100"
          fontname="Default font" fontsize="12.0" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="174 415 35 30" fill="solid: ffffffff" hasStroke="0" text="1k"
          fontname="Default font" fontsize="12.0" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="328 415 35 30" fill="solid: ffffffff" hasStroke="0" text="10k"
          fontname="Default font" fontsize="12.0" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="210 415 91 30" fill="solid: ffffffff" hasStroke="0" text="Frequency (Hz)"
          fontname="Default font" fontsize="12.0" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="371 415 35 30" fill="solid: ffffffff" hasStroke="0" text="20k"
          fontname="Default font" fontsize="12.0" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="413 413 27 30" fill="solid: ffffffff" hasStroke="0" text="Diff"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="413 342 17 30" fill="solid: ffffffff" hasStroke="0" text="Dir"
          fontname="Default font" fontsize="11.0" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <RECT pos="0 0 842 2" fill="solid: 61a52a" hasStroke="1" stroke="2, mitered, butt"
          strokeColour="solid: ffb9b9b9"/>
    <RECT pos="0 0 2 448" fill="solid: 61a52a" hasStroke="1" stroke="2, mitered, butt"
          strokeColour="solid: ffb9b9b9"/>
    <RECT pos="1298 0 2 448" fill="solid: 61a52a" hasStroke="1" stroke="2, mitered, butt"
          strokeColour="solid: ffb9b9b9"/>
    <RECT pos="0 446 1234 2" fill="solid: 61a52a" hasStroke="1" stroke="2, mitered, butt"
          strokeColour="solid: ffb9b9b9"/>
    <TEXT pos="22 91 138 30" fill="solid: ffffffff" hasStroke="0" text="Number of Sensors:"
          fontname="Default font" fontsize="13.5" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="22 114 170 30" fill="solid: ffffffff" hasStroke="0" text="Number of Directions:"
          fontname="Default font" fontsize="13.5" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="22 137 162 30" fill="solid: ffffffff" hasStroke="0" text="IR length: "
          fontname="Default font" fontsize="13.5" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="22 159 162 30" fill="solid: ffffffff" hasStroke="0" text="IR SampleRate:  "
          fontname="Default font" fontsize="13.5" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="22 184 162 30" fill="solid: ffffffff" hasStroke="0" text="DAW SampleRate:  "
          fontname="Default font" fontsize="13.5" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="12 1 120 32" fill="solid: ffffffff" hasStroke="0" text="HADES|"
          fontname="Default font" fontsize="18.8" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="80 1 106 32" fill="solid: ff00ffe6" hasStroke="0" text="Renderer"
          fontname="Default font" fontsize="18.0" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="290 33 149 30" fill="solid: ffffffff" hasStroke="0" text="Load HRIRs"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <RECT pos="223 58 213 58" fill="solid: 8f4f4f4" hasStroke="1" stroke="0.8, mitered, butt"
          strokeColour="solid: 67a0a0a0"/>
    <TEXT pos="232 88 173 30" fill="solid: ffffffff" hasStroke="0" text="Use Default HRIR set:"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="230 115 170 30" fill="solid: ffffffff" hasStroke="0" text="Number of Directions:"
          fontname="Default font" fontsize="13.5" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="230 137 162 30" fill="solid: ffffffff" hasStroke="0" text="HRIR length: "
          fontname="Default font" fontsize="13.5" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="230 161 162 30" fill="solid: ffffffff" hasStroke="0" text="HRIR SampleRate:  "
          fontname="Default font" fontsize="13.5" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="230 184 162 30" fill="solid: ffffffff" hasStroke="0" text="DAW SampleRate:  "
          fontname="Default font" fontsize="13.5" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <RECT pos="12 240 422 96" fill="solid: 10f4f4f4" hasStroke="1" stroke="0.8, mitered, butt"
          strokeColour="solid: 67a0a0a0"/>
    <TEXT pos="166 215 149 30" fill="solid: ffffffff" hasStroke="0" text="Rendering Settings"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="20 284 162 30" fill="solid: ffffffff" hasStroke="0" text="Beamforming:  "
          fontname="Default font" fontsize="13.5" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="20 307 196 30" fill="solid: ffffffff" hasStroke="0" text="Enable Covariance Matching:  "
          fontname="Default font" fontsize="13.5" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="236 237 162 30" fill="solid: ffffffff" hasStroke="0" text="Analysis Avg:  "
          fontname="Default font" fontsize="13.5" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="236 261 162 30" fill="solid: ffffffff" hasStroke="0" text="Synthesis Avg:  "
          fontname="Default font" fontsize="13.5" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="236 285 164 30" fill="solid: ffffffff" hasStroke="0" text="Left Ref Sensor Index:"
          fontname="Default font" fontsize="13.5" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="20 238 162 30" fill="solid: ffffffff" hasStroke="0" text="DoA Estimator:  "
          fontname="Default font" fontsize="13.5" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="20 262 162 30" fill="solid: ffffffff" hasStroke="0" text="Diff Estimator:  "
          fontname="Default font" fontsize="13.5" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="236 309 164 30" fill="solid: ffffffff" hasStroke="0" text="Right Ref Sensor Index:"
          fontname="Default font" fontsize="13.5" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <RECT pos="1232 0 2 448" fill="solid: 61a52a" hasStroke="1" stroke="2, mitered, butt"
          strokeColour="solid: ffb9b9b9"/>
    <TEXT pos="504 33 328 30" fill="solid: ffffffff" hasStroke="0" text="Direct Stream Gain Over Direction (dB)"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <RECT pos="840 0 2 448" fill="solid: 61a52a" hasStroke="1" stroke="2, mitered, butt"
          strokeColour="solid: ffb9b9b9"/>
  </BACKGROUND>
  <SLIDER name="new slider" id="b82f31194d53ffb4" memberName="s_diff2dir"
          virtualName="" explicitFocusOrder="0" pos="384 354 40 74" bkgcol="ff5c5d5e"
          trackcol="ff315b6d" textboxtext="ffffffff" textboxbkgd="ffffff"
          min="0.0" max="2.0" int="0.01" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="f8b5274e0c8768f4" memberName="label_IR_fs_array"
         virtualName="" explicitFocusOrder="0" pos="164 166 51 20" outlineCol="68a3a2a2"
         edTextCol="ff000000" edBkgCol="0" labelText="" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="c59fb2aab2496c4e" memberName="label_DAW_fs_array"
         virtualName="" explicitFocusOrder="0" pos="164 189 51 20" outlineCol="68a3a2a2"
         edTextCol="ff000000" edBkgCol="0" labelText="" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="new combo box" id="a465903000494955" memberName="CBbeamformer"
            virtualName="" explicitFocusOrder="0" pos="122 289 98 20" editable="0"
            layout="33" items="" textWhenNonSelected="Default" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="57ac3ad75ce7e4c2" memberName="label_N_nMics"
         virtualName="" explicitFocusOrder="0" pos="164 97 51 20" outlineCol="68a3a2a2"
         edTextCol="ff000000" edBkgCol="0" labelText="" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="4358fabfd06714ba" memberName="label_N_nDirs_array"
         virtualName="" explicitFocusOrder="0" pos="164 120 51 20" outlineCol="68a3a2a2"
         edTextCol="ff000000" edBkgCol="0" labelText="" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="6f16ddfe2924d418" memberName="label_IR_length_array"
         virtualName="" explicitFocusOrder="0" pos="164 143 51 20" outlineCol="68a3a2a2"
         edTextCol="ff000000" edBkgCol="0" labelText="" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="new toggle button" id="f7f951a1b21e1a11" memberName="TBuseDefaultHRIRs"
                virtualName="" explicitFocusOrder="0" pos="400 91 32 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="new label" id="38eb15d2da8894dc" memberName="label_HRIR_fs_bin"
         virtualName="" explicitFocusOrder="0" pos="377 166 51 20" outlineCol="68a3a2a2"
         edTextCol="ff000000" edBkgCol="0" labelText="" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="f48b8809471400fe" memberName="label_DAW_fs_bin"
         virtualName="" explicitFocusOrder="0" pos="377 189 51 20" outlineCol="68a3a2a2"
         edTextCol="ff000000" edBkgCol="0" labelText="" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="b05e699924050def" memberName="label_N_nDirs_bin"
         virtualName="" explicitFocusOrder="0" pos="377 121 51 20" outlineCol="68a3a2a2"
         edTextCol="ff000000" edBkgCol="0" labelText="" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="9590662d8dbe4192" memberName="label_IR_length_bin"
         virtualName="" explicitFocusOrder="0" pos="377 143 51 20" outlineCol="68a3a2a2"
         edTextCol="ff000000" edBkgCol="0" labelText="" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="new toggle button" id="d16cae20beea97a8" memberName="TBenableCM"
                virtualName="" explicitFocusOrder="0" pos="195 310 32 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="new slider" id="181508282c18d28c" memberName="SL_analysis_avg"
          virtualName="" explicitFocusOrder="0" pos="328 241 98 22" bkgcol="ff5c5d5e"
          trackcol="ff315b6e" min="0.0" max="1.0" int="0.01" style="LinearHorizontal"
          textBoxPos="TextBoxRight" textBoxEditable="1" textBoxWidth="45"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="new slider" id="9f8a89e1adcd1292" memberName="SL_synthesis_avg"
          virtualName="" explicitFocusOrder="0" pos="328 265 98 22" bkgcol="ff5c5d5e"
          trackcol="ff315b6e" min="0.0" max="1.0" int="0.01" style="LinearHorizontal"
          textBoxPos="TextBoxRight" textBoxEditable="1" textBoxWidth="45"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="new slider" id="a822918790e3106c" memberName="SL_left_ref_idx"
          virtualName="" explicitFocusOrder="0" pos="394 289 32 20" bkgcol="ff5c5d5e"
          trackcol="ff315b6e" min="0.0" max="64.0" int="1.0" style="LinearHorizontal"
          textBoxPos="TextBoxRight" textBoxEditable="1" textBoxWidth="45"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="new slider" id="2aa784b6d9234263" memberName="SL_right_ref_idx"
          virtualName="" explicitFocusOrder="0" pos="394 313 32 20" bkgcol="ff5c5d5e"
          trackcol="ff315b6e" min="0.0" max="64.0" int="1.0" style="LinearHorizontal"
          textBoxPos="TextBoxRight" textBoxEditable="1" textBoxWidth="45"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <COMBOBOX name="new combo box" id="26fe21c133fb2405" memberName="CBdoaEstimator"
            virtualName="" explicitFocusOrder="0" pos="122 245 98 20" editable="0"
            layout="33" items="" textWhenNonSelected="Default" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="new combo box" id="cf5db8f8b597a07d" memberName="CBdiffEstimator"
            virtualName="" explicitFocusOrder="0" pos="122 267 98 20" editable="0"
            layout="33" items="" textWhenNonSelected="Default" textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

