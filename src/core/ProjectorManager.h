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
    
    ProjectorManager();
    ~ProjectorManager();
    
    vector<Projector*> projectors;
    
    /*
     Projector Data
     */
    void addProjector(Projector *projector);

    
private:
    
};
