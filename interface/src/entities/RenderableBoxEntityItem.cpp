//
//  RenderableBoxEntityItem.cpp
//  interface/src
//
//  Created by Brad Hefta-Gaub on 8/6/14.
//  Copyright 2014 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#include <glm/gtx/quaternion.hpp>

#include <FBXReader.h>

#include "InterfaceConfig.h"

#include <BoxEntityItem.h>
#include <ModelEntityItem.h>
#include <PerfStat.h>


#include "Menu.h"
#include "EntityTreeRenderer.h"
#include "RenderableBoxEntityItem.h"


EntityItem* RenderableBoxEntityItem::factory(const EntityItemID& entityID, const EntityItemProperties& properties) {
    return new RenderableBoxEntityItem(entityID, properties);
}

void RenderableBoxEntityItem::render(RenderArgs* args) {
    PerformanceTimer perfTimer("RenderableBoxEntityItem::render");
    assert(getType() == EntityTypes::Box);
    glm::vec3 position = getPositionInMeters();
    glm::vec3 center = getCenter() * (float)TREE_SCALE;
    glm::vec3 dimensions = getDimensions() * (float)TREE_SCALE;
    glm::vec3 halfDimensions = dimensions / 2.0f;
    glm::quat rotation = getRotation();

    const bool useGlutCube = true;
    
    if (useGlutCube) {
        glColor3ub(getColor()[RED_INDEX], getColor()[GREEN_INDEX], getColor()[BLUE_INDEX]);
        glPushMatrix();
            glTranslatef(position.x, position.y, position.z);
            glm::vec3 axis = glm::axis(rotation);
            glRotatef(glm::degrees(glm::angle(rotation)), axis.x, axis.y, axis.z);
            glPushMatrix();
                glm::vec3 positionToCenter = center - position;
                glTranslatef(positionToCenter.x, positionToCenter.y, positionToCenter.z);
                glScalef(dimensions.x, dimensions.y, dimensions.z);
                Application::getInstance()->getDeferredLightingEffect()->renderSolidCube(1.0f);
            glPopMatrix();
        glPopMatrix();
    } else {
        static GLfloat vertices[] = { 1, 1, 1,  -1, 1, 1,  -1,-1, 1,   1,-1, 1,   // v0,v1,v2,v3 (front)
                                      1, 1, 1,   1,-1, 1,   1,-1,-1,   1, 1,-1,   // v0,v3,v4,v5 (right)
                                      1, 1, 1,   1, 1,-1,  -1, 1,-1,  -1, 1, 1,   // v0,v5,v6,v1 (top)
                                     -1, 1, 1,  -1, 1,-1,  -1,-1,-1,  -1,-1, 1,   // v1,v6,v7,v2 (left)
                                     -1,-1,-1,   1,-1,-1,   1,-1, 1,  -1,-1, 1,   // v7,v4,v3,v2 (bottom)
                                      1,-1,-1,  -1,-1,-1,  -1, 1,-1,   1, 1,-1 }; // v4,v7,v6,v5 (back)

        // normal array
        static GLfloat normals[]  = { 0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,   // v0,v1,v2,v3 (front)
                                    1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,   // v0,v3,v4,v5 (right)
                                    0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,   // v0,v5,v6,v1 (top)
                                   -1, 0, 0,  -1, 0, 0,  -1, 0, 0,  -1, 0, 0,   // v1,v6,v7,v2 (left)
                                    0,-1, 0,   0,-1, 0,   0,-1, 0,   0,-1, 0,   // v7,v4,v3,v2 (bottom)
                                    0, 0,-1,   0, 0,-1,   0, 0,-1,   0, 0,-1 }; // v4,v7,v6,v5 (back)

        // index array of vertex array for glDrawElements() & glDrawRangeElement()
        static GLubyte indices[]  = { 0, 1, 2,   2, 3, 0,      // front
                                      4, 5, 6,   6, 7, 4,      // right
                                      8, 9,10,  10,11, 8,      // top
                                     12,13,14,  14,15,12,      // left
                                     16,17,18,  18,19,16,      // bottom
                                     20,21,22,  22,23,20 };    // back



        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);
        glNormalPointer(GL_FLOAT, 0, normals);
        glVertexPointer(3, GL_FLOAT, 0, vertices);

        glColor3ub(getColor()[RED_INDEX], getColor()[GREEN_INDEX], getColor()[BLUE_INDEX]);
        
        Application::getInstance()->getDeferredLightingEffect()->bindSimpleProgram();
        
        glPushMatrix();
            glTranslatef(position.x, position.y, position.z);
            glm::vec3 axis = glm::axis(rotation);
            glRotatef(glm::degrees(glm::angle(rotation)), axis.x, axis.y, axis.z);
            glPushMatrix();
                glm::vec3 positionToCenter = center - position;
                glTranslatef(positionToCenter.x, positionToCenter.y, positionToCenter.z);
                // we need to do half the size because the geometry in the VBOs are from -1,-1,-1 to 1,1,1 
                glScalef(halfDimensions.x, halfDimensions.y, halfDimensions.z);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);
            glPopMatrix();
        glPopMatrix();

        Application::getInstance()->getDeferredLightingEffect()->releaseSimpleProgram();
        
        glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
        glDisableClientState(GL_NORMAL_ARRAY);
    }    
};
