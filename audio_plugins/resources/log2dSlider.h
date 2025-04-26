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

class log2dSlider  : public Component
{
public:
    log2dSlider (int _width, int _height, float _min_X_value, float _max_X_value, float _min_Y_value, float _max_Y_value, int _nDecimalPoints);
    ~log2dSlider() override;

    Rectangle<int> localBounds;

    void setDataHandles(float* _X_vector, float* _Y_values, int nPoints){
        X_vector = _X_vector;
        Y_values = _Y_values;
        num_X_points = nPoints;
        useIntValues = false;
    }
    void setDataHandlesInt(float* _X_vector, int* _Y_values, int nPoints){
        X_vector = _X_vector;
        Y_values_int = _Y_values;
        num_X_points = nPoints;
        useIntValues = true;
    }

    bool getRefreshValuesFLAG(){
        return refreshValues;
    }

    void setRefreshValuesFLAG(bool newFlag){
        refreshValues = newFlag;
    }
    
    void paint (juce::Graphics& g) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent& e) override;
    void mouseDrag (const juce::MouseEvent& e) override;

private:
    int width, height;
    float min_X_value, max_X_value, min_Y_value, max_Y_value;
    int nDecimalPoints;

    bool refreshValues;

    bool useIntValues;
    int* Y_values_int;
    float* Y_values;
    float* X_vector;
    int num_X_points;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (log2dSlider)
};
