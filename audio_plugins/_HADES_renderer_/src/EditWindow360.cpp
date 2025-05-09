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

#include "EditWindow360.h"

#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

const float circleWidth = 284.0f;
const float circleRadius = circleWidth / 2.0f;
const float view_x = 50;
const float view_y = 42;

EditWindow360::EditWindow360 (float* _pData, float _minVal, float _maxVal, int _dB_flag)
{
    setSize (382, 382);

    refreshRequired = true;
    pData = _pData;
    minVal = _minVal;
    maxVal = _maxVal;
    gainFLAG = _dB_flag;
}

EditWindow360::~EditWindow360()
{
    setLookAndFeel(nullptr);
}

void EditWindow360::paint (juce::Graphics& g)
{
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
}

void EditWindow360::resized()
{
	repaint();
}

void EditWindow360::mouseDown (const juce::MouseEvent& /*e*/)
{
}

void EditWindow360::mouseDrag (const juce::MouseEvent& e)
{
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
}

void EditWindow360::mouseUp (const juce::MouseEvent& /*e*/)
{
}
