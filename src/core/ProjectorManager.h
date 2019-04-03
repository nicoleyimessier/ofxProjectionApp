//
//  ProjectorManager.h
//  ProjectionApp
//
//  Created by Nicole Messier on 4/2/19.
//
//

#pragma once
#include "ofMain.h"
#include "Projector.h"
    
class ProjectorManager
{
public:
    
    static ProjectorManager& one()
    {
        static ProjectorManager instance;
        return instance;
    }
    
    ProjectorManager(){};
    ~ProjectorManager(){};
    
    vector<Projector*> projectors;
    
    /*
     Projector Data
     */
    void addProjector(int _order, int _numWarps, ofVec2f _size, ofVec2f _pos)
    {
        Projector *temp = new Projector();
        temp->setup(_order, _numWarps, _size, _pos);
        projectors.push_back(temp);
    
    };

    
private:
    
};
    
