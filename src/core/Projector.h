//
//  Projector.h
//  ProjectionApp
//
//  Created by Nicole Messier on 4/2/19.
//
//

//This is essentially just to hold projector data

#pragma once

#include "ofMain.h"

class Projector
{
public:
    Projector();
    ~Projector();
    
    void setup(int _order, int _numWarps, ofVec2f _size = ofVec2f(0.0f, 0.0f), ofVec2f _pos  = ofVec2f(0.0f, 0.0f))
    {
        order = _order;
        numWarps = _numWarps;
        size = _size;
        pos = _pos;
    };

    int order = 0;
    int numWarps = 0;
    ofVec2f size = ofVec2f(0.0f, 0.0f);
    ofVec2f pos = ofVec2f(0.0f, 0.0f);
    
};
