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

class EditWindow360  : public Component
{
public:
    EditWindow360 (float* _pData, float _minVal, float _maxVal, int _dB_flag);
    ~EditWindow360() override;
    
    void refresh(){

        /* Redraw visual depiction of what is going on... */
        if(refreshRequired)
            repaint();
    }

    void setRefreshNeeded(bool flag){
        refreshRequired = flag;
    }

    void paint (juce::Graphics& g) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent& e) override;
    void mouseDrag (const juce::MouseEvent& e) override;
    void mouseUp (const juce::MouseEvent& e) override;

private:
    bool refreshRequired;
    float* pData;
    float minVal;
    float maxVal;
    int gainFLAG;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditWindow360)
};
