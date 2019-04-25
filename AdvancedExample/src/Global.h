//
//  Global.hpp
//  DataGuiTest
//
//  Created by Nicole Messier on 2/19/19.
//
//
#pragma once
#include "ofMain.h"
#include "AppStructs.h"

class Global
{
public:
    
    static Global& one()
    {
        static Global instance;
        return instance;
    }
    
    Global();
    ~Global();
    
    // --- STANDARD APPLICATION ----
    float debug = false; 
    
};
