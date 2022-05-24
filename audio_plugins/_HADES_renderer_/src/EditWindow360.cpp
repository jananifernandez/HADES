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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "EditWindow360.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

const float circleWidth = 284.0f;
const float circleRadius = circleWidth / 2.0f;
const float view_x = 50;
const float view_y = 42;
//[/MiscUserDefs]

//==============================================================================
EditWindow360::EditWindow360 (float* _pData, float _minVal, float _maxVal, int _dB_flag)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]


    //[UserPreSize]
    //[/UserPreSize]

    setSize (382, 382);


    //[Constructor] You can add your own custom stuff here..
    refreshRequired = true;
    pData = _pData;
    minVal = _minVal;
    maxVal = _maxVal;
    gainFLAG = _dB_flag;
    //[/Constructor]
}

EditWindow360::~EditWindow360()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    setLookAndFeel(nullptr);
    //[/Destructor]
}

//==============================================================================
void EditWindow360::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..

    juce::Colour flareColour = juce::Colour (0x44f4f4f4), transparentColour = juce::Colour (0x00f4f4f4);
    juce::Colour cyanColour = juce::Colour (0xff2a9da5),  cyanFlareColour = juce::Colour (0x102a9da5);

    /****** DRAW TOP VIEW ******/
    /* Background and border */
    float centre_x = view_x+circleRadius;
    float centre_y = view_y+circleRadius;
    g.setGradientFill (juce::ColourGradient (flareColour, view_x+circleRadius, view_y + circleRadius,
                                             transparentColour, view_x+circleRadius, view_y + circleWidth, true));
    g.fillEllipse (view_x, view_y, circleWidth, circleWidth);
    g.setGradientFill (juce::ColourGradient (transparentColour, view_x+circleRadius, view_y + circleRadius,
                                             cyanFlareColour, view_x+circleRadius, view_y + circleWidth, true));
    g.fillEllipse (view_x, view_y, circleWidth, circleWidth);
    g.setColour (cyanColour);
    g.drawEllipse (view_x, view_y, circleWidth, circleWidth, 3.000f);

    /* Grid lines */
    g.setColour(Colours::lightgrey);
    g.setOpacity(0.175f);
    g.drawLine(view_x+circleRadius, view_y, view_x+circleRadius, view_y+circleWidth,1.0f);
    g.drawLine(view_x, view_y+circleRadius, view_x+circleWidth, view_y+circleRadius,1.0f);
    for(float i=minVal; i<=maxVal; (i += gainFLAG ? 3.0f : 0.25f)){
        float guideWidth = (i-minVal) * circleWidth/(maxVal-minVal);
        g.drawEllipse (view_x+circleRadius-guideWidth/2.0f, view_y+circleRadius-guideWidth/2.0f, guideWidth, guideWidth, i==1.0f ? 2.000f : 1.0f);
    }
    g.setOpacity(0.125f); /*ARTISTIC*/
    for(float i=minVal; i<=maxVal*2; (i += gainFLAG ? 3.0f : 0.25f)){
        float guideWidth = (i-minVal) * circleWidth/(maxVal-minVal);
        g.drawEllipse (view_x+circleRadius-guideWidth/2.0f, view_y+circleRadius-guideWidth/2.0f, guideWidth, guideWidth, i==1.0f ? 2.000f : 1.0f);
    }
    g.setOpacity(0.1f);
    for(int i=0; i<360; i+=15){
        float r = circleRadius*2;/*ARTISTIC*/
        float theta = M_PI/180.0f * (float)i;
        g.drawLine(centre_x, centre_y, centre_x-r*cosf(theta), centre_y-r*sinf(theta));
    }
    g.setOpacity(0.06f); /*ARTISTIC*/
    for(int i=0; i<360; i+=5){
        float r = circleRadius*2;
        float theta = M_PI/180.0f * (float)i;
        g.drawLine(centre_x, centre_y, centre_x-r*cosf(theta), centre_y-r*sinf(theta));
    }

    /* Draw the radial thing */
    g.setColour(cyanColour);
    for(int i=0; i<360; i++){
        float r = (pData[i]-minVal) * circleRadius/(maxVal-minVal);
        float r2 = (pData[(i+1)%359]-minVal) * circleRadius/(maxVal-minVal);
        float theta = (-M_PI/180.0f * (float)i + M_PI/2.0f);    /*  "+M_PI/2.0f" to bring 0azi to the top, and "-1*" to follow right-hand-rule */
        float theta2 = (-M_PI/180.0f * (float)(i+1) + M_PI/2.0f);
        g.setOpacity(0.25f);
        g.drawLine(centre_x, centre_y, centre_x-r*cosf(theta), centre_y-r*sinf(theta));
        g.setOpacity(0.75f);
        g.drawLine(centre_x-r*cosf(theta), centre_y-r*sinf(theta), centre_x-r2*cosf(theta2), centre_y-r2*sinf(theta2), 1.0f);
    }

    /* Labels */
    g.setColour(Colours::lightgrey);
    g.setOpacity(1.0f);
    g.setFont(13.0f);
    for(float i=minVal; i<=maxVal; (i += gainFLAG ? 3.0f : 0.5f)){
        float guideWidth = (i-minVal) * circleWidth/(maxVal-minVal);
        if(gainFLAG){
            g.drawText(String(i), centre_x-32, centre_y - guideWidth/2.0f-8, 64, 16, Justification::centred);
        }
        else{
            if(i==minVal)
                g.drawText("Diffuse", centre_x-32, centre_y - guideWidth/2.0f-8, 64, 16, Justification::centred);
            else if(i==maxVal){
                g.drawText("Direct", centre_x-32, centre_y - guideWidth/2.0f, 64, 16, Justification::centred);
                g.drawText("Direct", centre_x-32, centre_y + guideWidth/2.0f-16, 64, 16, Justification::centred);
            }
            else if(i==1.0f){
                g.drawText("Balanced", centre_x-32, centre_y - guideWidth/2.0f-8, 64, 16, Justification::centred);
                g.drawText("Balanced", centre_x-32, centre_y + guideWidth/2.0f-8, 64, 16, Justification::centred);
            }
        }
    }
    g.setColour(cyanColour);
    g.setFont(16.0f);
    g.drawText("Front", centre_x-32, centre_y - (circleRadius+16)-8, 64, 16, Justification::centred);
    g.drawText("Back", centre_x-32, centre_y + (circleRadius+16)-8, 64, 16, Justification::centred);
    g.drawText("Left", centre_x - (circleRadius+16)-34, centre_y-8, 64, 16, Justification::centred);
    g.drawText("Right", centre_x + (circleRadius+16)-31, centre_y-8, 74, 16, Justification::centred);


    refreshRequired = false;
    //[/UserPaint]
}

void EditWindow360::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..


	repaint();
    //[/UserResized]
}

void EditWindow360::mouseDown (const juce::MouseEvent& e)
{
    //[UserCode_mouseDown] -- Add your code here...

    //[/UserCode_mouseDown]
}

void EditWindow360::mouseDrag (const juce::MouseEvent& e)
{
    //[UserCode_mouseDrag] -- Add your code here...
    Point<float> point;
    float centre_x, centre_y;
    centre_x = view_x+circleRadius;
    centre_y = view_y+circleRadius;
    point.setXY((float)e.getPosition().getX(), (float)e.getPosition().getY());

    float theta = atan2f(centre_x-point.getX(),centre_y-point.getY());
    int azi = (int)(180.0f/M_PI * theta);
    float r = sqrtf(powf(fabsf(centre_x-point.getX()),2.0f) + powf(fabsf(centre_y-point.getY()),2.0f));
    for(int ii=-7; ii<7; ii++){
        int i = azi+ii;
        i = i<0 ? 360+i : i;
        i = i>359 ? i-360 : i;
        pData[i] =  jlimit(minVal, maxVal, r*(maxVal-minVal)/circleRadius + minVal) ;
    }
    refreshRequired = true;

    //[/UserCode_mouseDrag]
}

void EditWindow360::mouseUp (const juce::MouseEvent& e)
{
    //[UserCode_mouseUp] -- Add your code here...
    //[/UserCode_mouseUp]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="EditWindow360" componentName=""
                 parentClasses="public Component" constructorParams="float* _pData, float _minVal, float _maxVal, int _dB_flag"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="1" initialWidth="382" initialHeight="382">
  <METHODS>
    <METHOD name="mouseDown (const juce::MouseEvent&amp; e)"/>
    <METHOD name="mouseDrag (const juce::MouseEvent&amp; e)"/>
    <METHOD name="mouseUp (const juce::MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

