#include <GL/glut.h>
#include <string>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
# include <vector>

#include "ProjectMain.hpp"

using namespace ProjectMain;

// MyVirtualWorld Constructor
MyVirtualWorld::MyVirtualWorld():battlefield(this),background(this)
{
    numberOfTexture = 0;
    textureNumbers = NULL;
    textureison = false;
}
// MyVirtualWorld Destructor
MyVirtualWorld::~MyVirtualWorld()
{
    if (textureNumbers != NULL)
     delete[] textureNumbers;
}

void MyVirtualWorld::tickTime() {
    // Animation logic
    timenew = glutGet(GLUT_ELAPSED_TIME);
    elapseTime = timenew - timeold;
    timeold = timenew;

    if (minunAttacking) {
        float speed = 0.15f;
        float maxOffset = -30.0f;

        if (minunGoingForward) {
            minunAttackOffset -= speed * elapseTime;

            if (minunAttackOffset <= maxOffset) {
                minunAttackOffset = maxOffset;
                minunGoingForward = false; // Start coming back
            }
        } else {
            minunAttackOffset += speed * elapseTime;

            if (minunAttackOffset >= 0.0f) {
                minunAttackOffset = 0.0f;
                minunGoingForward = true; // Reset direction
                minunAttacking = false; // Stop animation
            }
        }
        glutPostRedisplay(); // Request redraw
    }

    if (weedleAttacking) {
        float speed = 0.15f;
        float maxOffset = 30.0f;

        if (weedleGoingForward) {
            weedleAttackOffset += speed * elapseTime;

            if (weedleAttackOffset >= maxOffset) {
                weedleAttackOffset = maxOffset;
                weedleGoingForward = false;
            }
        } else {
            weedleAttackOffset -= speed * elapseTime;

            if (weedleAttackOffset <= 0.0f) {
                weedleAttackOffset = 0.0f;
                weedleGoingForward = true;
                weedleAttacking = false;
            }
        }
        glutPostRedisplay();
    }

    // Start attacking animation alongside with minun animation
    if (minunAttacking && !minunAttackEffectActive) {
        startMinunAttackEffect();
    }

    // Start attacking animation alongside with weedle animation
    if (weedleAttacking && !weedleAttackEffectActive) {
        startWeedleAttackEffect();
    }

    if (minunAttackEffectActive) {
        updateProjectiles(minunProjectiles, minunAttackEffectActive);
    }

    if (weedleAttackEffectActive) {
        updateProjectiles(weedleProjectiles, weedleAttackEffectActive);
    }

}//end of TickTime

void MyVirtualWorld::init() {

    timeold = glutGet(GLUT_ELAPSED_TIME);
    setupLights();
    setupTextures();

    // For minun body
    static GLfloat profilepoints[] = {
        0.0f,  0.0f,  0.0f,
        0.5f,  0.13f, 0.0f,
        0.87f, 0.5f,  0.0f,
        0.95f, 0.75f, 0.0f,
        1.0f,  1.0f,  0.0f,
        0.95f, 1.25f, 0.0f,
        0.87f, 1.5f,  0.0f,
        0.785f,1.685f,0.0f,
        0.585f,1.985f,0.0f,
        0.485f,2.185f,0.0f,
        0.0f,  2.185f,0.0f
    };

    minun.setup(profilepoints, 11, 0, 360, 10);

}//end of init

void MyVirtualWorld::setupTextures()
{
    numberOfTexture = 3;
    textureNumbers = new GLuint[numberOfTexture];
    //Set pixel storage mode
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    //set texture environment parameters
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    //Generate texture map identifiers
    glGenTextures(numberOfTexture, textureNumbers);
    //Read image data into memory
    MyImage myimage[numberOfTexture];
    myimage[0].loadJPG("image/brick1.jpg");
    myimage[1].loadJPG("image/wood1.jpg");
    myimage[2].loadJPG("image/roof1.jpg");
    //Try this: myimage[2].loadBMP("images/rocks.bmp");
    //for each texture map identifiers
    for (int i=0; i<numberOfTexture; i++)
    {
        //Bind a named texture to a texturing target
        glBindTexture(GL_TEXTURE_2D, textureNumbers[i]);
        //Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        //Set texture map
        glTexImage2D(GL_TEXTURE_2D, // defining a 2D texture
            0, // level (not multi-resolution)
            GL_RGBA, // format of OpenGL's texture
            myimage[i].width, myimage[i].height,// dimensions of the image
            0, // border
            GL_RGB, GL_UNSIGNED_BYTE, // format of image
            myimage[i].buffer // pixel data of image
        );
    }
    textureison = true;
} //end of setupTextures

void MyVirtualWorld::toggleTextures()
{
    // use when F3 is pressed
    textureison = !textureison;
}

void Background::drawhousebody() {
    glDisable(GL_CULL_FACE);

    if (world->textureison) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, world->textureNumbers[0]);
    }
    glColor3f(0.545f, 0.271f, 0.075f);
    glBegin(GL_QUADS);
        // Bottom (Y-) face
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-8.0f, 0.0f, 10.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-8.0f, 0.0f, -10.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(8.0f, 0.0f, -10.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(8.0f, 0.0f, 10.0f);

        // Top (Y+) face
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-8.0f, 20.0f, 10.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-8.0f, 20.0f, -10.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(8.0f, 20.0f, -10.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(8.0f, 20.0f, 10.0f);

        // Left (X-) face
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-8.0f, 0.0f, 10.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-8.0f, 0.0f, -10.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-8.0f, 20.0f, -10.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-8.0f, 20.0f, 10.0f);

        // Right (X+) face
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(8.0f, 0.0f, -10.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(8.0f, 0.0f, 10.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(8.0f, 20.0f, 10.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(8.0f, 20.0f, -10.0f);

        // Front (Z+) face
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-8.0f, 0.0f, 10.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-8.0f, 20.0f, 10.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(8.0f, 20.0f, 10.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(8.0f, 0.0f, 10.0f);

        // Back (Z-) face
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-8.0f, 0.0f, -10.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-8.0f, 20.0f, -10.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(8.0f, 20.0f, -10.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(8.0f, 0.0f, -10.0f);
    glEnd();

    glEnable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);
} //end of drawhousebody

void Background::drawdoor() {
    glDisable(GL_CULL_FACE);

    // Set door color (black)
    glColor3f(1.00f, 1.00f, 1.00f);
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-1.0f, 0.0f, 0.0f);  // Bottom left
        glVertex3f(1.0f, 0.0f, 0.0f);   // Bottom right
        glVertex3f(1.0f, 5.0f, 0.0f);   // Top right
        glVertex3f(-1.0f, 5.0f, 0.0f);  // Top left
    glEnd();

    // Black handle
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POINTS);
        glPointSize(0.5f);
        glVertex3f(0.7f, 2.5f, 0.1f);
    glEnd();

    glEnable(GL_CULL_FACE);
} //end of drawdoor

void Background::drawcirclewindow() {
    glDisable(GL_CULL_FACE);

    // Set circle window parameters
    float radius = 2.5f; // since height and width = 10.0f
    int segments = 100;  // smoothness of the circle
    float cx = 0.0f;     // center x
    float cy = 10.0f;    // center y
    float cz = 0.1f;     // z position to slightly pop out

    // Draw the circular window
    glColor3f(0.5f, 0.8f, 1.0f); // Light blue for glass
    glBegin(GL_POLYGON);
        glNormal3f(0.0f, 0.0f, 1.0f);
        for (int i = 0; i < segments; i++) {
            float theta = 2.0f * 3.14159f * float(i) / float(segments);
            float x = radius * cos(theta);
            float y = radius * sin(theta);
            glVertex3f(cx + x, cy + y, cz);
        }
    glEnd();

    // Draw cross bars (horizontal and vertical lines)
    glLineWidth(2.0f);
    glColor3f(0.0f, 0.0f, 0.0f); // Black cross
    glBegin(GL_LINES);
        glNormal3f(0.0f, 0.0f, 1.0f);
        // Horizontal bar
        glVertex3f(cx - radius, cy, cz + 0.01f);
        glVertex3f(cx + radius, cy, cz + 0.01f);
        // Vertical bar
        glVertex3f(cx, cy - radius, cz + 0.01f);
        glVertex3f(cx, cy + radius, cz + 0.01f);
    glEnd();

    glEnable(GL_CULL_FACE);
} //end of drawcirclewindow

void Background::drawsquarewindow() {
    glDisable(GL_CULL_FACE);

    // Set square window parameters
    float halfSize = 2.5f;
    float cx = 0.0f;
    float cy = 0.0f;
    float cz = 0.0f;

    // Draw window background (glass)
    glColor3f(0.5f, 0.8f, 1.0f);
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(cx - halfSize, cy - halfSize, cz);
        glVertex3f(cx + halfSize, cy - halfSize, cz);
        glVertex3f(cx + halfSize, cy + halfSize, cz);
        glVertex3f(cx - halfSize, cy + halfSize, cz);
    glEnd();

    // Draw black border
    glLineWidth(3.0f);
    glColor3f(0.0f, 0.0f, 0.0f);

    glBegin(GL_LINE_LOOP);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(cx - halfSize, cy - halfSize, cz + 0.01f);
        glVertex3f(cx + halfSize, cy - halfSize, cz + 0.01f);
        glVertex3f(cx + halfSize, cy + halfSize, cz + 0.01f);
        glVertex3f(cx - halfSize, cy + halfSize, cz + 0.01f);
    glEnd();

    // Draw horizontal bar in the center
    glBegin(GL_LINES);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(cx , cy - halfSize, cz + 0.01f);
        glVertex3f(cx , cy + halfSize, cz + 0.01f);
    glEnd();

    glEnable(GL_CULL_FACE);
} //end of drawsquarewindow

void Background::drawbalcony() {
    glDisable(GL_CULL_FACE);

    // Balcony floor parameters
    float floorWidth = 10.0f;
    float floorDepth = 3.0f;
    float floorHeight = 0.0f;

    // Draw the balcony floor
    glColor3f(0.6f, 0.4f, 0.2f);
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-floorWidth / 2, floorHeight, 0.0f);
        glVertex3f(floorWidth / 2, floorHeight, 0.0f);
        glVertex3f(floorWidth / 2, floorHeight, floorDepth);
        glVertex3f(-floorWidth / 2, floorHeight, floorDepth);
    glEnd();

    // Draw iron fences
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    int numFences = 11;
    float fenceSpacing = floorWidth / (numFences - 1);
    float fenceHeight = 3.0f;

    for (int i = 0; i < numFences; ++i) {
        float x = -floorWidth / 2 + i * fenceSpacing;
        glBegin(GL_LINES);
            glVertex3f(x, floorHeight, floorDepth);
            glVertex3f(x, floorHeight + fenceHeight, floorDepth);
        glEnd();
    }

    // Top rail (front)
    glBegin(GL_LINES);
        glVertex3f(-floorWidth / 2, floorHeight + fenceHeight, floorDepth);
        glVertex3f(floorWidth / 2, floorHeight + fenceHeight, floorDepth);
    glEnd();

    // Top rail (left side)
    glBegin(GL_LINES); // vertical bar
        glVertex3f(-floorWidth / 2, floorHeight, 0.0f);
        glVertex3f(-floorWidth / 2, floorHeight + fenceHeight, 0.0f);
        glVertex3f(-floorWidth / 2, floorHeight, floorDepth);
        glVertex3f(-floorWidth / 2, floorHeight + fenceHeight, floorDepth);
    glEnd();
    glBegin(GL_LINES); // top rail
        glVertex3f(-floorWidth / 2, floorHeight + fenceHeight, 0.0f);
        glVertex3f(-floorWidth / 2, floorHeight + fenceHeight, floorDepth);
    glEnd();

    // Top rail (right side)
    glBegin(GL_LINES); // vertical bar
        glVertex3f(floorWidth / 2, floorHeight, 0.0f);
        glVertex3f(floorWidth / 2, floorHeight + fenceHeight, 0.0f);
        glVertex3f(floorWidth / 2, floorHeight, floorDepth);
        glVertex3f(floorWidth / 2, floorHeight + fenceHeight, floorDepth);
    glEnd();
    glBegin(GL_LINES); // top rail
        glVertex3f(floorWidth / 2, floorHeight + fenceHeight, 0.0f);
        glVertex3f(floorWidth / 2, floorHeight + fenceHeight, floorDepth);
    glEnd();

    glEnable(GL_CULL_FACE);
} //end ofdrawbalcony

void Background::drawroof() {
    glDisable(GL_CULL_FACE);

    // Apply texture when toggle is on
    if (world->textureison) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, world->textureNumbers[2]);
    }

    // Roof parameters
    float width = 20.0f;
    float height = 8.0f;
    float depth = 20.0f;
    float frontZ = 10.0f;
    float backZ = frontZ - depth;

    float halfWidth = width / 2.0f;


    glColor3f(0.5f, 0.1f, 0.1f);
    //Front
    glBegin(GL_TRIANGLES);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfWidth, 0.0f, frontZ);      // Bottom left
        glTexCoord2f(1.0f, 1.0f); glVertex3f(halfWidth, 0.0f, frontZ);       // Bottom right
        glTexCoord2f(0.5f, 0.0f); glVertex3f(0.0f, height, frontZ);          // Top center
    glEnd();

    //Back
    glBegin(GL_TRIANGLES);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(-halfWidth, 0.0f, backZ);       // Bottom left
        glTexCoord2f(1.0f, 1.0f);glVertex3f(halfWidth, 0.0f, backZ);        // Bottom right
        glTexCoord2f(0.5f, 0.0f);glVertex3f(0.0f, height, backZ);           // Top center
    glEnd();

    //Left
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(-halfWidth, 0.0f, frontZ);      // Front bottom
        glTexCoord2f(1.0f, 1.0f);glVertex3f(-halfWidth, 0.0f, backZ);       // Back bottom
        glTexCoord2f(1.0f, 0.0f);glVertex3f(0.0f, height, backZ);           // Back top
        glTexCoord2f(0.0f, 0.0f);glVertex3f(0.0f, height, frontZ);
    glEnd();

    //Right
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(halfWidth, 0.0f, frontZ);       // Front bottom
        glTexCoord2f(1.0f, 1.0f);glVertex3f(halfWidth, 0.0f, backZ);        // Back bottom
        glTexCoord2f(1.0f, 0.0f);glVertex3f(0.0f, height, backZ);           // Back top
        glTexCoord2f(0.0f, 0.0f);glVertex3f(0.0f, height, frontZ);          // Front top
    glEnd();

    //Bottom
    glBegin(GL_QUADS);
        glVertex3f(-halfWidth, 0.0f, frontZ);      // Front left
        glVertex3f(halfWidth, 0.0f, frontZ);       // Front right
        glVertex3f(halfWidth, 0.0f, backZ);        // Back right
        glVertex3f(-halfWidth, 0.0f, backZ);       // Back left
    glEnd();

    glEnable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);
} //end of drawroof

void Background::drawhouse(){
    // Function to combine modular parts
    glDisable(GL_CULL_FACE);

    drawhousebody();

    glPushMatrix();
        glTranslatef(0.0f,0.0f,10.1f);
        drawdoor();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.0f,5.0f,10.1f);
        drawcirclewindow();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-4.5f,4.5f,10.1f);
        drawsquarewindow();
        glTranslatef(9.0f,0.0f,0.0f);
        drawsquarewindow();
    glPopMatrix();

    glPushMatrix();
        glScalef(1.2f,1.0f,1.0f);
        glTranslatef(0.0f,10.0f,10.1f);
        drawbalcony();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.0f,20.0f,0.0f);
        drawroof();
    glPopMatrix();
}//end of drawhouse

void Background::drawHouseArc() {
    //To loop the house in circular manner
    int numHouses = 12;
    float radius = 60.0f;
    float centerX = 0.0f;
    float centerZ = 0.0f;
    float y = 0.0f;

    for (int i = 0; i < numHouses; ++i) {
        // Spread evenly in the circle
        float angleDeg = 360.0f * i / (numHouses - 1);
        float angleRad = angleDeg * 3.14159f / 180.0f;

        float x = centerX + radius * cos(angleRad);
        float z = centerZ - radius * sin(angleRad);

        // Calculate the angle to rotate the house so every houses faces the center
        float rotateY = angleDeg + 270.0f;

        glPushMatrix();
            glTranslatef(x, y, z);
            glRotatef(rotateY, 0.0f, 1.0f, 0.0f); // rotate to face the center
            drawhouse();
        glPopMatrix();
    }
}//end of drawHouseArc

void Background::drawtree() {
    glDisable(GL_CULL_FACE);
    // Draw trunk
    glColor3f(0.55f, 0.27f, 0.07f);

    glPushMatrix();
        glBegin(GL_TRIANGLES);
            // Front face
            glNormal3f(0.0f, 0.5f, 1.0f);
            glVertex3f( 0.2f, 0.0f,  0.2f);
            glVertex3f(-0.2f, 0.0f,  0.2f);
            glVertex3f( 0.0f, 2.0f,  0.0f);

            // Right face
            glNormal3f(1.0f, 0.5f, 0.0f);
            glVertex3f( 0.2f, 0.0f, -0.2f);
            glVertex3f( 0.2f, 0.0f,  0.2f);
            glVertex3f( 0.0f, 2.0f,  0.0f);

            // Back face
            glNormal3f(0.0f, 0.5f, -1.0f);
            glVertex3f(-0.2f, 0.0f, -0.2f);
            glVertex3f( 0.2f, 0.0f, -0.2f);
            glVertex3f( 0.0f, 2.0f,  0.0f);

            // Left face
            glNormal3f(-1.0f, 0.5f, 0.0f);
            glVertex3f(-0.2f, 0.0f,  0.2f);
            glVertex3f(-0.2f, 0.0f, -0.2f);
            glVertex3f( 0.0f, 2.0f,  0.0f);
        glEnd();
    glPopMatrix();

    // Draw the leaves
    glColor3f(0.0f, 0.6f, 0.0f);
    glPushMatrix();
        glTranslatef(0.0f, 1.5f, 0.0f);
        glRotatef(-90, 1.0f, 0.0f, 0.0f);
        glutSolidCone(0.8, 2.5, 12, 4);
    glPopMatrix();
    glEnable(GL_CULL_FACE);
} //end of drawtree

void Background::drawforest(int count, float spread) {
    glDisable(GL_CULL_FACE);

    // random value
    srand(100);

    int drawn = 0;
    while (drawn < count) {
        float x = ((rand() % 100) / 100.0f - 0.5f) * spread;
        float z = ((rand() % 100) / 100.0f - 0.5f) * spread;

        float distance = sqrt(x * x + z * z);
        if (distance < 30.0f)
            continue; // Skip if too close to center to avoid conflict with battle arena

        // Generate a random scale between 0.5f and 1.5f
        float scale = 0.5f + ((rand() % 100) / 100.0f) * (5.0f - 3.0f);

        glPushMatrix();
            glTranslatef(x, 0.0f, z);
            glScalef(scale, scale, scale);
            drawtree();
        glPopMatrix();

        ++drawn;
    }

    glEnable(GL_CULL_FACE);
}//end of drawforest

void Background::drawgrass() {
    glPushMatrix();
        glColor3f(0.0f, 0.8f, 0.0f);
        glBegin(GL_TRIANGLES);
            glVertex3f(0.0f, 0.0f, 0.0f);      // bottom
            glVertex3f(0.1f, 3.0f, 0.0f);     // tip right
            glVertex3f(-0.1f, 3.0f, 0.0f);    // tip left
        glEnd();
    glPopMatrix();
}//end of drawgrass

void Background::drawgrasses(int count, float spread) {
    srand(100);

    int drawn = 0;
    while (drawn < count) {
        float x = ((rand() % 100) / 100.0f - 0.5f) * spread;
        float z = ((rand() % 100) / 100.0f - 0.5f) * spread;

        float distance = sqrt(x * x + z * z);
        if (distance < 30.0f)
            continue; // Skip if too close to center to avoid conflict with battle arena

        float angle = rand() % 360;

        glPushMatrix();
            glTranslatef(x, 0.0f, z);
            glRotatef(angle, 0, 1, 0);
            drawgrass();
        glPopMatrix();

        ++drawn;
    }
}//end of drawgrasses

void Weedle::drawhead()
{
    glDisable(GL_CULL_FACE);

    GLUquadric* head = gluNewQuadric();

    // U
    glPushMatrix();
        glColor3f(1.0f, 1.0f, 1.0f);
        glRotatef(90, 1.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, -7.0f);
        gluCylinder(head, 0.3f, 1.0f, 3.0f, 100, 100);
        glutSolidSphere(0.3f, 8, 8);
    glPopMatrix();

    // The shape of head
    glPushMatrix();
        glColor3f(0.71f, 0.55f, 0.38f);
        glutSolidSphere(5, 100, 100);
    glPopMatrix();

    // The shape of the nose
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, 5.0f);
        glScalef(1.5f, 1.0f, 1.0f);
        glColor3f(0.92f, 0.53f, 0.83f);
        glutSolidSphere(1.0f, 50, 50);
    glPopMatrix();

    // Draw eyes
    glPushMatrix();
        glColor3f(0.3f, 0.3f, 0.3f);
        glTranslated(1.5f, 2.0f, 4.0f);
        glutSolidSphere(0.5f, 100, 100);
        glTranslated(-3.0f, 0.0f, 0.0f);
        glutSolidSphere(0.5f, 100, 100);
    glPopMatrix();

    glEnable(GL_CULL_FACE);
}//end of drawhead

void Weedle::drawbody()
{
    glPushMatrix();
        glColor3f(0.71f, 0.55f, 0.38f);
        glutSolidSphere(5, 100, 100);
    glPopMatrix();

    glPushMatrix();
        glColor3f(0.92f, 0.53f, 0.83f);
        glTranslatef(3.0f, 0.0f, 4.0f);
        glutSolidSphere(0.8, 100, 100);
        glTranslatef(-6.0f, 0.0f, 0.0f);
        glutSolidSphere(0.8, 100, 100);
    glPopMatrix();
}//end of drawbody

void Weedle::drawtail()
{
    glDisable(GL_CULL_FACE);
        GLUquadric* tail = gluNewQuadric();

        glColor3f(0.9f, 0.9f, 0.9f);
        glutSolidSphere(0.5f, 100, 100);
        glTranslatef(0.0f, 3.0f, 0.0f);
        glRotatef(90, 1.0f,0.0f, 0.0f);
        gluCylinder(tail, 0.1f, 0.5f, 3.0f, 100, 100);
        glutSolidSphere(0.1f, 8, 8);
    glEnable(GL_CULL_FACE);
} //end of drawtail

void Weedle::drawweedle()
{
    // function to combine all modular parts
    glPushMatrix();
        glTranslatef(0.0f, 15.0f, 0.0f);
        drawhead();
        glTranslatef(0.0f, -7.5f, -2.5f);
        glScalef(0.9f, 0.9f, 0.9f);
        drawbody();
        glTranslatef(0.0f, -6.5f, -2.5f);
        glScalef(0.9f, 0.9f, 0.9f);
        drawbody();
        glTranslatef(0.0f, -5.5f, -4.0f);
        glScalef(0.9f, 0.9f, 0.9f);
        drawbody();
        glTranslatef(0.0f, -4.5f, -3.9f);
        glScalef(0.9f, 0.9f, 0.9f);
        glRotatef(35.0, 1.0f, 0.0f, 0.0f);
        drawbody();
        glTranslatef(0.0f, -4.5f, -2.0f);
        glScalef(0.9f, 0.9f, 0.9f);
        glRotatef(35.0, 1.0f, 0.0f, 0.0f);
        drawbody();
        glTranslatef(0.0f, -6.0f, 0.0f);
        glScalef(0.9f, 0.9f, 0.9f);
        glRotatef(35.0, 1.0f, 0.0f, 0.0f);
        drawbody();
        glTranslatef(0.0f, -6.0f, 2.5f);
        glScalef(0.9f, 0.9f, 0.9f);
        glRotatef(10.0, 1.0f, 0.0f, 0.0f);
        drawbody();
        glTranslatef(0.0f, -3.5f, -4.0f);
        glRotatef(-130.0, 1.0f, 0.0f, 0.0f);
        glScalef(5.0f, 5.0f, 5.0f);
        drawtail();
    glPopMatrix();
}//end of drawweedle

Minun::Minun()
{
    // Setup Quadric Object
    pObj = gluNewQuadric();
    gluQuadricNormals(pObj, GLU_SMOOTH);

    //initial velocity (in unit per second)
    velx = 40.0f;
    vely = 0.0f;
    velz = 30.0f;
}

Minun::~Minun()
{
    gluDeleteQuadric(pObj);
    if(surfacepoints)
        delete[] surfacepoints;
}

void Minun::setup(const GLfloat* profilePoints,
                  GLint numOfProfilePoints,
                  GLfloat degreeStart,
                  GLfloat degreeEnd,
                  GLfloat degreeStep)
{
    profilepoints = profilePoints;
    numofprofilepoints = numOfProfilePoints;

    if(surfacepoints)
        delete[] surfacepoints;

    numofverticallines = 1 + static_cast<int>(floor((degreeEnd - degreeStart)/degreeStep));
    numofsurfacepoints = numofprofilepoints * numofverticallines;
    surfacepoints = new GLfloat[3 * numofsurfacepoints];

    int surfptsindex = 0;
    GLfloat degree = degreeStart;

    for(int i=0; i<numofverticallines; ++i, degree+=degreeStep)
    {
        GLfloat radian = degree*M_PI/180.0;
        GLfloat c = cos(radian);
        GLfloat s = sin(radian);
        GLfloat x,y,z;

        for (int j=0, profileindex=0; j<numofprofilepoints; ++j)
        {
            surfacepoints[surfptsindex] = c*profilepoints[profileindex] + s*profilepoints[profileindex+2];
            surfacepoints[surfptsindex+1] = profilepoints[profileindex+1];
            surfacepoints[surfptsindex+2] = -s*profilepoints[profileindex] + c*profilepoints[profileindex+2];
            profileindex += 3;
            surfptsindex += 3;
        }
    }
} //end of setup

void Minun::leg()
{
    glDisable(GL_CULL_FACE);

    glColor3f(0.99f, 0.95f, 0.77f);
    glRotatef(-270, 1.0f, 0.0f, 0.0f);
    // Draw a circle for cap the leg
    gluDisk(pObj, 0.0f, 1.5f, 100, 1);
    gluCylinder(pObj, 2.3f, 1.5f, 4.5f, 100, 100);

    glEnable(GL_CULL_FACE);
}//end of leg

void Minun::body()
{
    glDisable(GL_CULL_FACE);
    int index = 0;
    glColor3f(0.99f, 0.95f, 0.77f);

    for (int i=0; i<numofverticallines-1; ++i)
    {
        glBegin(GL_QUAD_STRIP);
            for (int j=0; j<numofprofilepoints; ++j)
            {
                glVertex3fv( surfacepoints+index );
                glVertex3fv( surfacepoints+index+3*numofprofilepoints );
                index+=3;
            }
        glEnd();
    }

    glEnable(GL_CULL_FACE);
}//end of body

void Minun::hand()
{
    glDisable(GL_CULL_FACE);
        glPushMatrix();
        // Blue color for Munun
        glColor3f(0.58f, 0.80f, 0.93f);
        glScaled(2.0f, 1.0f, 1.0f);
        gluSphere(pObj, 1.0, 20, 20);
        glPopMatrix();

        glPushMatrix();
        //Yellow color for Minun
        glColor3f(0.99f, 0.95f, 0.77f);
        glRotatef(90, 0.0f, 1.0f, 0.0f);
        gluCylinder(pObj, 1.0f, 2.9f, 7.8f, 100, 100);
        glPopMatrix();
    glEnable(GL_CULL_FACE);
} //end of hand

void Minun::head()
{
    glDisable(GL_CULL_FACE);

    glPushMatrix();
        glColor3f(0.99f, 0.95f, 0.77f);
        glScalef(20.0f,15.5f,20.0f);
        gluSphere(pObj, 0.5, 30, 30);
    glPopMatrix();

    glPushMatrix();
        glColor3f(0.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, -2.3f, 8.1f);
        glScalef(3.5f, 2.0f, 2.8f);
        gluSphere(pObj, 0.5, 1000, 1000);
    glPopMatrix();

    // call mouth function
    glPushMatrix();
        glTranslatef(0.0f, -1.5f, 0.0f);
        glRotatef(-27, 0.0f, 0.0f, 1.0f);
        mouth();
    glPopMatrix();

    // call eyes function
    glPushMatrix();
        //left eyes
        glTranslatef(-2.5f, 1.0f, 9.5f);
        eyes();

        // right eye
        glTranslatef(5.0f, 0.0f, 0.0f);
        eyes();
    glPopMatrix();

    // Call cheeks function
    glPushMatrix();
        //left cheeks
        glTranslatef(-6.0f, -1.5f, 6.0f);
        glScalef(1.2f, 1.2f, 1.2f);
        glRotatef(-40, 0.0f, 1.0f, 0.0f);
        glRotatef(20, 1.0f, 0.0f, 0.0f);
        cheeks();
    glPopMatrix();

    glPushMatrix();
        //left cheeks
        glTranslatef(6.0f, -1.5f, 6.0f);
        glScalef(1.2f, 1.2f, 1.2f);
        glRotatef(40, 0.0f, 1.0f, 0.0f);
        glRotatef(20, 1.0f, 0.0f, 0.0f);
        cheeks();
    glPopMatrix();

    glEnable(GL_CULL_FACE);
}//end of head

void Minun::ear()
{
    //Sphere of the ear
    glDisable(GL_CULL_FACE);

    glPushMatrix();
        glColor3f(0.99f, 0.95f, 0.77f);
        glScalef(2.0f, 2.0f, 0.9f);
        gluSphere(pObj, 1.0, 100, 100);
    glPopMatrix();

    glPushMatrix();
        glColor3f(0.58f, 0.80f, 0.93f);
        glScalef(2.9f, 5.6f, 0.5f);
        glTranslatef(0.0f, 0.65f, 0.0f);
        gluSphere(pObj, 1.0, 100, 100);
    glPopMatrix();

    glEnable(GL_CULL_FACE);
}//end of ear

void drawCuboid()
{
    float w = 0.25f;
    float h = 3.5f;
    float d = 1.0f;

    glBegin(GL_QUADS);

    // Front face (+Z)
    glVertex3f(-w, -h,  d);
    glVertex3f( w, -h,  d);
    glVertex3f( w,  h,  d);
    glVertex3f(-w,  h,  d);

    // Back face (-Z)
    glVertex3f(-w, -h, -d);
    glVertex3f(-w,  h, -d);
    glVertex3f( w,  h, -d);
    glVertex3f( w, -h, -d);

    // Left face (-X)
    glVertex3f(-w, -h, -d);
    glVertex3f(-w, -h,  d);
    glVertex3f(-w,  h,  d);
    glVertex3f(-w,  h, -d);

    // Right face (+X)
    glVertex3f( w, -h, -d);
    glVertex3f( w,  h, -d);
    glVertex3f( w,  h,  d);
    glVertex3f( w, -h,  d);

    // Top face (+Y)
    glVertex3f(-w,  h, -d);
    glVertex3f(-w,  h,  d);
    glVertex3f( w,  h,  d);
    glVertex3f( w,  h, -d);

    // Bottom face (-Y)
    glVertex3f(-w, -h, -d);
    glVertex3f( w, -h, -d);
    glVertex3f( w, -h,  d);
    glVertex3f(-w, -h,  d);

    glEnd();
}//end of drawCuboid

void Minun::tail()
{
    glDisable(GL_CULL_FACE);

    glPushMatrix();
        glColor3f(0.99f, 0.95f, 0.77f);
        glScalef(0.3f, 0.5f, 0.5f);
        gluSphere(pObj, 2.0, 50, 50);
    glPopMatrix();

    glPushMatrix();
        glColor3f(0.58f, 0.80f, 0.93f);
        glTranslatef(0.0f, -0.3f, -0.8f);
        glRotatef(-25.0f, 1.0f, 0.0f, 0.0f);
        drawCuboid();
    glPopMatrix();

    glEnable(GL_CULL_FACE);
}//end of tail

void Minun::eyes()
{
    glPushMatrix();
        glColor3f(0.12f, 0.21f, 0.32f);
        glScalef(1.0f, 1.7f, 1.0f);
        glutSolidSphere(1.0f, 20, 20);
    glPopMatrix();
}//end of eyes

void Minun::cheeks()
{
    glPushMatrix();
        glColor3f(0.99f, 0.95f, 0.77f);
        glTranslatef(0.0f, 0.0f, 0.7f);
        glScalef(1.0f, 0.8f, 1.0f);
        glutSolidSphere(1.0f, 100, 100);
    glPopMatrix();

    glPushMatrix();
        glColor3f(0.58f, 0.80f, 0.93f);
        glutSolidSphere(1.5f, 100, 100);
    glPopMatrix();
}//end of cheeks

void Minun::mouth()
{
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);

    // radius of face sphere
    float mouthRadius = 9.3f;
    float arcRadius = 3.0f;
    float centerY = 0.0f;
    float centerZ = sqrt(mouthRadius * mouthRadius - centerY * centerY);

    for(int i = -120; i<=0; i+=10)
    {
        float theta = i * M_PI / 180.0f;
        float x = arcRadius * cos(theta);
        float y = centerY + arcRadius * sin(theta);
        float z = sqrt(mouthRadius * mouthRadius - x * x - y * y);

        glVertex3f(x, y, z);
    }

    glEnd();
}//end of mouth

void Minun::assemble()
{
    // Function to combine all parts of minun

    // Draw Body
    glPushMatrix();
        glScalef(5.0f, 5.0f, 5.0f);
        body();
    glPopMatrix();

    // Draw hand
    glPushMatrix();
        // Left hand
        glTranslatef(-6.5f, 6.0f, 0.0f);
        glRotatef(25, 0.0f, 0.0f, 1.0f);
        hand();
    glPopMatrix();

    glPushMatrix();
        // Right hand
        glTranslatef(6.5f, 6.3f, 0.0f);
        glRotatef(180, 0.0f, 1.0f, 0.0f);
        glRotatef(23, 0.0f, 0.0f, 1.0f);
        hand();
    glPopMatrix();
    // End draw hand

    // Draw head
    glPushMatrix();
        glTranslatef(0.0f, 13.5f, 0.0f);
        glScalef(0.5f, 0.5f, 0.5f);
        head();
    glPopMatrix();

    // Draw ear start
    glPushMatrix();
        // Draw left ear
        glTranslatef(-3.2f, 17.5f, 0.0f);
        glRotatef(40, 0.0f, 0.0f, 1.0f);
        ear();
    glPopMatrix();

    glPushMatrix();
        // Draw right ear
        glTranslatef(3.2f, 17.65f, 0.0f);
        glRotatef(-40, 0.0f, 0.0f, 1.0f);
        ear();
    glPopMatrix();
    // Draw ear end

    //Start draw leg
    glPushMatrix();
        //Draw left leg
        glTranslatef(-2.0f, 1.8f, 0.0f);
        glRotated(-12.0, 0.0f, 0.0f, 1.0f);
        leg();
    glPopMatrix();

    glPushMatrix();
        //Draw right leg
        glTranslatef(2.0f, 1.8f, 0.0f);
        glRotated(12.0, 0.0f, 0.0f, 1.0f);
        leg();
    glPopMatrix();
    //End draw leg

    // Draw Tail
    glPushMatrix();
        glTranslatef(0.0f, 1.7f, -4.5f);
        tail();
    glPopMatrix();
}//end of assemble

void Battlefield::drawfloor()
{
    glDisable(GL_CULL_FACE);

    float tileHeight = 1.0f;

    for (float x = -arenaHalfWidth; x < arenaHalfWidth; x += gridSize) {
        for (float z = -arenaHalfLength; z < arenaHalfLength; z += gridSize) {
            glColor3f(0.5f, 1.0f, 0.5f);

            // Red zone (front)
            if (z < -20)
                glColor3f(1.0f, 0.2f, 0.2f);

            // Blue zone (back)
            if (z > 20)
                glColor3f(0.2f, 0.2f, 1.0f);

            float y = 0.0f;

            // Draw cube
            float x0 = x, x1 = x + gridSize;
            float z0 = z, z1 = z + gridSize;
            float y0 = y, y1 = y - tileHeight;

            // Top face
            glBegin(GL_QUADS);
                glNormal3f(0.0f, 1.0f, 0.0f);
                glVertex3f(x0, y0, z0);
                glVertex3f(x1, y0, z0);
                glVertex3f(x1, y0, z1);
                glVertex3f(x0, y0, z1);
            glEnd();

            // Front face
            glBegin(GL_QUADS);
                glNormal3f(0.0f, 0.0f, 1.0f);
                glColor3f(0.4f, 0.4f, 0.4f);
                glVertex3f(x0, y0, z1);
                glVertex3f(x1, y0, z1);
                glVertex3f(x1, y1, z1);
                glVertex3f(x0, y1, z1);
            glEnd();

            // Back face
            glBegin(GL_QUADS);
                glNormal3f(0.0f, 0.0f, -1.0f);
                glColor3f(0.4f, 0.4f, 0.4f);
                glVertex3f(x0, y0, z0);
                glVertex3f(x0, y1, z0);
                glVertex3f(x1, y1, z0);
                glVertex3f(x1, y0, z0);
            glEnd();

            // Left face
            glBegin(GL_QUADS);
                glNormal3f(-1.0f, 0.0f, 0.0f);
                glColor3f(0.7f, 0.7f, 0.7f);
                glVertex3f(x0, y0, z0);
                glVertex3f(x0, y0, z1);
                glVertex3f(x0, y1, z1);
                glVertex3f(x0, y1, z0);
            glEnd();

            // Right face
            glBegin(GL_QUADS);
                glNormal3f(1.0f, 0.0f, 0.0f);
                glColor3f(0.7f, 0.7f, 0.7f);
                glVertex3f(x1, y0, z0);
                glVertex3f(x1, y1, z0);
                glVertex3f(x1, y1, z1);
                glVertex3f(x1, y0, z1);
            glEnd();

            // Bottom face
            glBegin(GL_QUADS);
                glNormal3f(0.0f, -1.0f, 0.0f);
                glColor3f(0.5f, 0.5f, 0.5f);
                glVertex3f(x0, y1, z0);
                glVertex3f(x1, y1, z0);
                glVertex3f(x1, y1, z1);
                glVertex3f(x0, y1, z1);
            glEnd();
        }
    }
}//end of drawfloor

void Battlefield::drawPokeBall(){
    glDisable(GL_CULL_FACE);
    //Poke ball
    float yOffset = 0.01f;
    float radius = 5.0f;
    float buttonOuter = 1.3f;
    float buttonInner = 0.8f;

    // Top half - Red
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, yOffset, 0.0f);
    for (int i = 0; i <= 180; i++) {
        float angle = i * PI / 180.0f;
        glVertex3f(cos(angle) * radius, yOffset, sin(angle) * radius);
    }
    glEnd();

    // Bottom half - White
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, yOffset + 0.001f, 0.0f);
    for (int i = 180; i <= 360; i++) {
        float angle = i * PI / 180.0f;
        glVertex3f(cos(angle) * radius, yOffset + 0.001f, sin(angle) * radius);
    }
    glEnd();

    // Outer black ring
    float bandWidth = 0.3f;
    glColor3f(0.0f, 0.0f, 0.0f);
    (GL_QUAD_STRIP);
    glNormal3f(0.0f, 1.0f, 0.0f);
    for (int i = 0; i <= 360; i++) {
        float angle = i * PI / 180.0f;
        float x = cos(angle) * radius;
        float z = sin(angle) * radius;

        glVertex3f(x, yOffset + 0.002f, z + bandWidth / 2.0f);
        glVertex3f(x, yOffset + 0.002f, z - bandWidth / 2.0f);
    }
    glEnd();

    // inner black ring
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, yOffset + 0.004f, 0.0f);
    for (int i = 0; i <= 360; i++) {
        float angle = i * PI / 180.0f;
        glVertex3f(cos(angle) * buttonOuter, yOffset + 0.004f, sin(angle) * buttonOuter);
    }
    glEnd();

    // white button
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, yOffset + 0.005f, 0.0f);
    for (int i = 0; i <= 360; i++) {
        float angle = i * PI / 180.0f;
        glVertex3f(cos(angle) * buttonInner, yOffset + 0.005f, sin(angle) * buttonInner);
    }
    glEnd();

    // Outer ring of Poke Ball
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    glNormal3f(0.0f, 1.0f, 0.0f);
    for (int i = 0; i <= 360; i++) {
        float angle = i * PI / 180.0f;
        float outlineRadius = radius + 0.05f;
        glVertex3f(cos(angle) * outlineRadius, yOffset + 0.006f, sin(angle) * outlineRadius);
    }
    glEnd();
    glEnable(GL_CULL_FACE);
}//end of pokeball

void Battlefield::drawArenaWalls() {
    // LEFT wall box
    glDisable(GL_CULL_FACE);
    if (world->textureison) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, world->textureNumbers[1]);
    }

    glBegin(GL_QUADS);
        // Front face
        glColor3f(0.6f, 0.6f, 0.6f);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(-arenaHalfWidth - wallThickness, 0, -arenaHalfLength);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-arenaHalfWidth, 0, -arenaHalfLength);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(-arenaHalfWidth, wallHeight, -arenaHalfLength);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-arenaHalfWidth - wallThickness, wallHeight, -arenaHalfLength);

        // Back face
        glColor3f(0.5f, 0.5f, 0.5f);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(-arenaHalfWidth - wallThickness, 0, arenaHalfLength);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-arenaHalfWidth, 0, arenaHalfLength);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(-arenaHalfWidth, wallHeight, arenaHalfLength);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-arenaHalfWidth - wallThickness, wallHeight, arenaHalfLength);

        // Top face
        glColor3f(0.7f, 0.7f, 0.7f);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(-arenaHalfWidth - wallThickness, wallHeight, -arenaHalfLength);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(-arenaHalfWidth, wallHeight, -arenaHalfLength);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(-arenaHalfWidth, wallHeight, arenaHalfLength);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(-arenaHalfWidth - wallThickness, wallHeight, arenaHalfLength);

        // left side
        glColor3f(0.3f, 0.3f, 0.3f);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(-arenaHalfWidth - wallThickness, 0, -arenaHalfLength);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(-arenaHalfWidth - wallThickness, 0, arenaHalfLength);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(-arenaHalfWidth - wallThickness, wallHeight, arenaHalfLength);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(-arenaHalfWidth - wallThickness, wallHeight, -arenaHalfLength);

        // right side
        glColor3f(0.2f, 0.2f, 0.2f);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(-arenaHalfWidth, 0, -arenaHalfLength);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(-arenaHalfWidth, 0, arenaHalfLength);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(-arenaHalfWidth, wallHeight, arenaHalfLength);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(-arenaHalfWidth, wallHeight, -arenaHalfLength);
    glEnd();

    // RIGHT wall box
    glBegin(GL_QUADS);
        glColor3f(0.6f, 0.6f, 0.6f); // Front
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(arenaHalfWidth, 0, -arenaHalfLength);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(arenaHalfWidth + wallThickness, 0, -arenaHalfLength);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(arenaHalfWidth + wallThickness, wallHeight, -arenaHalfLength);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(arenaHalfWidth, wallHeight, -arenaHalfLength);

        glColor3f(0.5f, 0.5f, 0.5f); // Back
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(arenaHalfWidth, 0, arenaHalfLength);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(arenaHalfWidth + wallThickness, 0, arenaHalfLength);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(arenaHalfWidth + wallThickness, wallHeight, arenaHalfLength);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(arenaHalfWidth, wallHeight, arenaHalfLength);

        glColor3f(0.7f, 0.7f, 0.7f); // Top
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(arenaHalfWidth, wallHeight, -arenaHalfLength);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(arenaHalfWidth + wallThickness, wallHeight, -arenaHalfLength);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(arenaHalfWidth + wallThickness, wallHeight, arenaHalfLength);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(arenaHalfWidth, wallHeight, arenaHalfLength);

        glColor3f(0.2f, 0.2f, 0.2f); // Inner side
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(arenaHalfWidth, 0, -arenaHalfLength);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(arenaHalfWidth, 0, arenaHalfLength);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(arenaHalfWidth, wallHeight, arenaHalfLength);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(arenaHalfWidth, wallHeight, -arenaHalfLength);

        glColor3f(0.3f, 0.3f, 0.3f); // Outer side
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(arenaHalfWidth + wallThickness, 0, -arenaHalfLength);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(arenaHalfWidth + wallThickness, 0, arenaHalfLength);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(arenaHalfWidth + wallThickness, wallHeight, arenaHalfLength);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(arenaHalfWidth + wallThickness, wallHeight, -arenaHalfLength);
    glEnd();

    // FRONT wall
    glBegin(GL_QUADS);
        glColor3f(0.5f, 0.5f, 0.5f); // Front face
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(-arenaHalfWidth, 0, arenaHalfLength);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(arenaHalfWidth, 0, arenaHalfLength);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(arenaHalfWidth, wallHeight, arenaHalfLength);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(-arenaHalfWidth, wallHeight, arenaHalfLength);

        glColor3f(0.65f, 0.65f, 0.65f); // Top
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(-arenaHalfWidth, wallHeight, arenaHalfLength);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(arenaHalfWidth, wallHeight, arenaHalfLength);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(arenaHalfWidth, wallHeight, arenaHalfLength + wallThickness);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(-arenaHalfWidth, wallHeight, arenaHalfLength + wallThickness);

        glColor3f(0.25f, 0.25f, 0.25f); // Left side
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(-arenaHalfWidth, 0, arenaHalfLength);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(-arenaHalfWidth, 0, arenaHalfLength + wallThickness);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(-arenaHalfWidth, wallHeight, arenaHalfLength + wallThickness);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(-arenaHalfWidth, wallHeight, arenaHalfLength);

        glColor3f(0.25f, 0.25f, 0.25f); // Right side
        glNormal3f(1.0f, 0.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(arenaHalfWidth, 0, arenaHalfLength);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(arenaHalfWidth, 0, arenaHalfLength + wallThickness);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(arenaHalfWidth, wallHeight, arenaHalfLength + wallThickness);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(arenaHalfWidth, wallHeight, arenaHalfLength);
    glEnd();

    // BACK wall
    glBegin(GL_QUADS);
        glColor3f(0.5f, 0.5f, 0.5f); // Back face
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(-arenaHalfWidth, 0, -arenaHalfLength);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(arenaHalfWidth, 0, -arenaHalfLength);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(arenaHalfWidth, wallHeight, -arenaHalfLength);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(-arenaHalfWidth, wallHeight, -arenaHalfLength);

        glColor3f(0.65f, 0.65f, 0.65f); // Top
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(-arenaHalfWidth, wallHeight, -arenaHalfLength);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(arenaHalfWidth, wallHeight, -arenaHalfLength);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(arenaHalfWidth, wallHeight, -arenaHalfLength - wallThickness);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(-arenaHalfWidth, wallHeight, -arenaHalfLength - wallThickness);

        glColor3f(0.25f, 0.25f, 0.25f); // Left side
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(-arenaHalfWidth, 0, -arenaHalfLength);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(-arenaHalfWidth, 0, -arenaHalfLength - wallThickness);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(-arenaHalfWidth, wallHeight, -arenaHalfLength - wallThickness);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(-arenaHalfWidth, wallHeight, -arenaHalfLength);

        glColor3f(0.25f, 0.25f, 0.25f); // Right side
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(arenaHalfWidth, 0, -arenaHalfLength);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(arenaHalfWidth, 0, -arenaHalfLength - wallThickness);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(arenaHalfWidth, wallHeight, -arenaHalfLength - wallThickness);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(arenaHalfWidth, wallHeight, -arenaHalfLength);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
}//end of drawArenaWalls

void Battlefield::drawHUDTextScreen(const char* line1, const char* line2) {
    // function to show battle text in the middle of window
    // Save current projection and modelview
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 650);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Draw background rectangle
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
        glVertex3f(1.0f, 1.0f,1.0f);
        glVertex3f(1.0f, 1.0f,1.0f);
        glVertex3f(1.0f, 1.0f,1.0f);
        glVertex3f(1.0f, 1.0f,1.0f);
    glEnd();

    // Draw text
    glColor3f(1.0f, 1.0f, 1.0f);

    glRasterPos2f(350, 520);  // Position line 1
    for (const char* c = line1; *c != '\0'; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    glRasterPos2f(350, 480);  // Position line 2
    for (const char* c = line2; *c != '\0'; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Restore projection
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}// end of drawHUDTextScreen

void Battlefield::drawbattlearena()
{
    // Function to combine all parts of battlearena
    drawfloor();
    drawArenaWalls();
    drawPokeBall();
    drawHUDTextScreen("BATTLE START!", "Blue VS Red");
}//end of drawbattlearena

void MyVirtualWorld::finalassemble(){
    //Function to comvine all parts from all classes
    glDisable(GL_CULL_FACE);
    battlefield.drawbattlearena();
    background.drawgrasses(1000,150.0f);
    background.drawHouseArc();
    background.drawforest(200,90.0f);
    drawSpotLights();

    glPushMatrix();
        glTranslatef(5.0f,20.0f,-20.0f);
        glScalef(0.7f,0.7f,0.7f);
        battlefield.drawWeedleWord();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(5.0f,20.0f,20.0f);
        glRotatef(-180, 0.0f, 1.0f, 0.0f);
        glScalef(0.7f,0.7f,0.7f);
        battlefield.drawMinunWord();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.0f, 0.0f, minunAttackOffset);
        glRotatef(-180, 0.0f, 1.0f, 0.0f);
        glScalef(0.5f,0.5f,0.5f);
        glTranslatef(0.0f,0.5f,-40.0f);
        minun.assemble();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.0f, 0.0f, weedleAttackOffset);
        glScalef(0.4f,0.4f,0.4f);
        glTranslatef(0.0f,10.0f,-30.0f);
        weedle.drawweedle();
    glPopMatrix();

    // When minun skill was activated, display effect
    if (minunAttackEffectActive) {
        glColor3f(0.0f, 0.0f, 1.0f);
        for (int i = 0; i < MAX_PROJECTILES; ++i) {
            if (minunProjectiles[i].active) {
                glPushMatrix();
                glScalef(1.5f,1.5f,1.5f);
                glTranslatef(minunProjectiles[i].x, minunProjectiles[i].y, minunProjectiles[i].z);
                drawMinus3D();
                glPopMatrix();
            }
        }
    }

    // When weedle skill was fiactivatedred, display effect
    if (weedleAttackEffectActive) {
        glColor3f(0.75f, 0.5f, 1.0f);
        for (int i = 0; i < MAX_PROJECTILES; ++i) {
            if (weedleProjectiles[i].active) {
                glPushMatrix();
                glScalef(1.5f,1.5f,1.5f);
                glTranslatef(weedleProjectiles[i].x, weedleProjectiles[i].y, weedleProjectiles[i].z);
                drawWaterDrop3D();
                glPopMatrix();
            }
        }
    }
}//end of finalassemble

void MyVirtualWorld::startMinunAttackEffect() {
    minunAttackEffectActive = true;
    for (int i = 0; i < MAX_PROJECTILES; ++i) {
        minunProjectiles[i] = {
            0.0f, 0.0f, 40.0f + (float)(rand() % 10), // start near Minun at z=40+
            0.0f, // negative speed to move backward toward z=-30
            ((rand() % 10) - 5) * 0.005f, // slight random y speed
            -0.2f - (rand() % 10) * 0.01f,
            true
        };
    }
}//end of startMinunAttackEffect

void MyVirtualWorld::startWeedleAttackEffect() {
    weedleAttackEffectActive = true;
    for (int i = 0; i < MAX_PROJECTILES; ++i) {
        weedleProjectiles[i] = {
            0.0f, 0.0f, -30.0f - (float)(rand() % 10), // start near Weedle at z=-30-
            0.0f, // positive speed to move forward toward z=40
            ((rand() % 10) - 5) * 0.005f, // slight random y speed
            0.15f + (rand() % 10) * 0.01f,
            true
        };
    }
}//end ofstartWeedleAttackEffect

void MyVirtualWorld::updateProjectiles(Projectile* projectiles, bool &effectActive) {
    bool anyActive = false;
    for (int i = 0; i < MAX_PROJECTILES; ++i) {
        if (projectiles[i].active) {
            projectiles[i].x += projectiles[i].speedX * elapseTime;
            projectiles[i].y += projectiles[i].speedY * elapseTime;
            projectiles[i].z += projectiles[i].speedZ * elapseTime;

            // Deactivate based on direction:
            if (projectiles[i].speedZ < 0 && projectiles[i].z < -30.0f) {
                // Moving backward, stop after z < -30
                projectiles[i].active = false;
            } else if (projectiles[i].speedZ > 0 && projectiles[i].z > 40.0f) {
                // Moving forward, stop after z > 40
                projectiles[i].active = false;
            } else {
                anyActive = true;
            }
        }
    }
    effectActive = anyActive;
}//end of updateProjectiles

void MyVirtualWorld::drawMinus3D() {
    // Draw a 3D minus sign for minun skill animation
    glDisable(GL_CULL_FACE);
    glBegin(GL_QUADS);
        // front face
        glVertex3f(-1.0f, -0.2f, 0.1f);
        glVertex3f(1.0f, -0.2f, 0.1f);
        glVertex3f(1.0f, 0.2f, 0.1f);
        glVertex3f(-1.0f, 0.2f, 0.1f);
        // back face
        glVertex3f(-1.0f, -0.2f, -0.1f);
        glVertex3f(1.0f, -0.2f, -0.1f);
        glVertex3f(1.0f, 0.2f, -0.1f);
        glVertex3f(-1.0f, 0.2f, -0.1f);
    glEnd();
    glEnable(GL_CULL_FACE);
}//end of drawMinus3D

void MyVirtualWorld::drawWaterDrop3D() {
    // Draw a simple teardrop shape for weedle skill animation
    glDisable(GL_CULL_FACE);
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0.0f, 0.0f, 0.0f);
        for (int i = 0; i <= 20; ++i) {
            float angle = i * 2.0f * 3.14159f / 20;
            float x = 0.3f * cos(angle);
            float y = 0.5f * sin(angle);
            glVertex3f(x, y, 0.0f);
        }
    glEnd();
    glEnable(GL_CULL_FACE);
}//end of drawWaterDrop3D

void MyVirtualWorld::setupLights()
{
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    myspotlights.setupLights();

    // for light selection
    for (int i=0;i<3;++i){
        lighton[i] = true;
    }
}//end of setupLights

void MyVirtualWorld::toggleLight(int lightno)
{
    if (lightno==0) //for white light
    {
        lighton[0] = !lighton[0];
        if (lighton[0])
            glEnable(GL_LIGHT0);
        else
            glDisable(GL_LIGHT0);
        }
    else if (lightno>=1 && lightno<=2) // for lamppost light
    {
        myspotlights.toggleLight(lightno-1);
    }
}//end of toggleLight

void MyVirtualWorld::drawSpotLights()
{
    myspotlights.drawlamppost();
}//end of drawSpotLights

MySpotLights::MySpotLights()
{
    pObj = gluNewQuadric();
    gluQuadricNormals(pObj, GLU_SMOOTH);
}

MySpotLights::~MySpotLights()
{
    //empty
}

void MySpotLights::setupLights()
{
    GLfloat noAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // No ambient light

    glLightfv(GL_LIGHT1, GL_AMBIENT, noAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, myred);
    glLightfv(GL_LIGHT1, GL_SPECULAR, myred);
    glLightf (GL_LIGHT1, GL_SPOT_CUTOFF, 50);
    glLightf (GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
    glLightf (GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);
    glLightf (GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.02);
    glLightfv(GL_LIGHT2, GL_AMBIENT, noAmbient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, myblue);
    glLightfv(GL_LIGHT2, GL_SPECULAR, myblue);
    glLightf (GL_LIGHT2, GL_SPOT_CUTOFF, 50);
    glLightf (GL_LIGHT2, GL_SPOT_EXPONENT, 2.0);
    glLightf (GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0);
    glLightf (GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.02);

}//end of setupLights

void MySpotLights::toggleLight(int lightno)
{
    static GLenum tag[] = {GL_LIGHT1, GL_LIGHT2};
    lighton[lightno] = !lighton[lightno];
    if (lighton[lightno])
        glEnable( tag[lightno] );
    else
        glDisable( tag[lightno] );
}//end of toggleLight

void MySpotLights::draw()
{
    GLboolean cullingIsOn;
    glGetBooleanv(GL_CULL_FACE, &cullingIsOn);
    glDisable(GL_CULL_FACE);

    glPushMatrix();
        // Draw the stick
        glPushMatrix();
            glColor3f(0.2f, 0.2f, 0.2f);
            glTranslatef(0.0f, 1.5f, 0.0f);
            glScalef(0.2f, 4.0f, 0.2f);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Draw the lamp holder
        glPushMatrix();
            //cube holder
            glColor3f(0.2f, 0.2f, 0.2f);
            glTranslatef(0.0f, 3.2f, 0.0f);
            glutSolidCube(0.4f);

            //stick to hold the lamp
            glTranslatef(0.0f, 0.0f, 0.6f);
            glScalef(0.2f, 0.2f, 1.0f);
            glutSolidCube(1.0f);
        glPopMatrix();

        // Draw the cone (lamp)
        glPushMatrix();

            glTranslatef(0.0f, 2.7f, 1.2f);
            glRotatef(-115.0f, 1.0f, 0.0f, 0.0f);

            // Draw bottom cap
            glColor3f(0.9f, 0.9f, 0.5f);
            gluDisk(pObj, 0.0f, 0.4f, 30, 1);
            // Draw the lamp cylinder
            gluCylinder(pObj, 0.4f, 0.2f, 0.5f, 30, 5);

            // Draw top cap
            glPushMatrix();
                glColor3f(0.2f, 0.2f, 0.2f);
                glTranslatef(0.0f, 0.0f, 0.5f);
                gluDisk(pObj, 0.0f, 0.2f, 30, 1);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();

    if (cullingIsOn==GL_TRUE)
        glEnable(GL_CULL_FACE);
}//end of draw

void MySpotLights::drawlamppost() {
    // Lamppost 1 with GL_LIGHT1(red)
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, -30.0f);
        glScalef(5.5f, 6.5f, 5.5f);

        GLfloat position1[] = {0.0f, 2.7f, 0.5f, 1.0f};
        GLfloat direction1[] = {0.0f, -1.0f, 0.7f};
        glLightfv(GL_LIGHT1, GL_POSITION, position1);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction1);

        draw(); // Draw the first lamppost
    glPopMatrix();

    // Lamppost 2 with GL_LIGHT2 (blue)
    glPushMatrix();
        glRotatef(-180.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, -30.0f);
        glScalef(5.5f, 6.5f, 5.5f);

        GLfloat position2[] = {0.0f, 2.7f, 0.5f, 1.0f};
        GLfloat direction2[] = {0.0f, -1.0f, 0.7f};
        glLightfv(GL_LIGHT2, GL_POSITION, position2);
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, direction2);

        draw(); // Draw the second lamppost
    glPopMatrix();
}//end of drawlamppost

void Battlefield::drawWeedleWord()
{
    // Draw capital W
    glDisable(GL_CULL_FACE);
    glColor3f(0.85f, 0.0f, 0.0f); // red
    glBegin(GL_QUAD_STRIP);
        // Left leg
        glVertex3f(-35.0f, 8.0f, 0.0f);
        glVertex3f(-35.0f, 8.0f, 3.0f);
        glVertex3f(-32.0f, 0.0f, 0.0f);
        glVertex3f(-32.0f, 0.0f, 3.0f);
        // Left middle leg
        glVertex3f(-28.0f, 5.0f, 0.0f);
        glVertex3f(-28.0f, 5.0f, 3.0f);
        // Right middle leg
        glVertex3f(-24.0f, 0.0f, 0.0f);
        glVertex3f(-24.0f, 0.0f, 3.0f);
        // Right leg
        glVertex3f(-21.0f, 8.0f, 0.0f);
        glVertex3f(-21.0f, 8.0f, 3.0f);
    glEnd();

    // draw E1
    // Vertical bar (main stem of E)
    glBegin(GL_QUAD_STRIP);
        glVertex3f(-19.0f, 0.0f, 0.0f);
        //glVertex3f(-10.0f, 0.0f, depth);
        glVertex3f(-19.0f, 0.0f, 3.0f);
        //glVertex3f(-10.0f, 10.0f, depth);
        glVertex3f(-19.0f, 8.0f, 0.0f);
        //glVertex3f(-8.0f, 10.0f, depth);
        glVertex3f(-19.0f, 8.0f, 3.0f);
    glEnd();

    // Top horizontal bar
    glBegin(GL_QUAD_STRIP);
        glVertex3f(-19.0f, 8.0f, 0.0f);
        //glVertex3f(-8.0f, 10.0f, depth);
        glVertex3f( -19.0f, 8.0f, 3.0f);
        //glVertex3f( 0.0f, 10.0f, depth);
        glVertex3f( -13.0f, 8.0f, 0.0f);
        //glVertex3f( 0.0f, 8.0f, depth);
        glVertex3f(-13.0f, 8.0f, 3.0f);
    glEnd();

    // Middle horizontal bar
    glBegin(GL_QUAD_STRIP);
        glVertex3f(-19.0f, 4.0f, 0.0f);
        //glVertex3f(-8.0f, 10.0f, depth);
        glVertex3f( -19.0f, 4.0f, 3.0f);
        //glVertex3f( 0.0f, 10.0f, depth);
        glVertex3f( -14.0f, 4.0f, 0.0f);
        //glVertex3f( 0.0f, 8.0f, depth);
        glVertex3f(-14.0f, 4.0f, 3.0f);
    glEnd();

    // Bottom horizontal bar
    glBegin(GL_QUAD_STRIP);
        glVertex3f(-19.0f, 0.0f, 0.0f);
        //glVertex3f(-8.0f, 10.0f, depth);
        glVertex3f( -19.0f, 0.0f, 3.0f);
        //glVertex3f( 0.0f, 10.0f, depth);
        glVertex3f( -12.5f, 0.0f, 0.0f);
        //glVertex3f( 0.0f, 8.0f, depth);
        glVertex3f(-12.5f, 0.0f, 3.0f);
    glEnd();

    // draw E2
    // Vertical bar (main stem of E)
    glBegin(GL_QUAD_STRIP);
        glVertex3f(-10.0f, 0.0f, 0.0f);
        //glVertex3f(-10.0f, 0.0f, depth);
        glVertex3f(-10.0f, 0.0f, 3.0f);
        //glVertex3f(-10.0f, 10.0f, depth);
        glVertex3f(-10.0f, 8.0f, 0.0f);
        //glVertex3f(-8.0f, 10.0f, depth);
        glVertex3f(-10.0f, 8.0f, 3.0f);
    glEnd();

    // Top horizontal bar
    glBegin(GL_QUAD_STRIP);
        glVertex3f(-10.0f, 8.0f, 0.0f);
        //glVertex3f(-8.0f, 10.0f, depth);
        glVertex3f( -10.0f, 8.0f, 3.0f);
        //glVertex3f( 0.0f, 10.0f, depth);
        glVertex3f( -4.0f, 8.0f, 0.0f);
        //glVertex3f( 0.0f, 8.0f, depth);
        glVertex3f(-4.0f, 8.0f, 3.0f);
    glEnd();

    // Middle horizontal bar
    glBegin(GL_QUAD_STRIP);
        glVertex3f(-10.0f, 4.0f, 0.0f);
        //glVertex3f(-8.0f, 10.0f, depth);
        glVertex3f( -10.0f, 4.0f, 3.0f);
        //glVertex3f( 0.0f, 10.0f, depth);
        glVertex3f( -5.0f, 4.0f, 0.0f);
        //glVertex3f( 0.0f, 8.0f, depth);
        glVertex3f(-5.0f, 4.0f, 3.0f);
    glEnd();

    // Bottom horizontal bar
    glBegin(GL_QUAD_STRIP);
        glVertex3f(-10.0f, 0.0f, 0.0f);
        //glVertex3f(-8.0f, 10.0f, depth);
        glVertex3f( -10.0f, 0.0f, 3.0f);
        //glVertex3f( 0.0f, 10.0f, depth);
        glVertex3f( -3.5f, 0.0f, 0.0f);
        //glVertex3f( 0.0f, 8.0f, depth);
        glVertex3f(-3.5f, 0.0f, 3.0f);
    glEnd();

    // Draw D
    glBegin(GL_QUAD_STRIP);
        glVertex3f(-2.0f, 0.0f, 0.0f);
        glVertex3f(-2.0f, 0.0f, 3.0f);
        glVertex3f(-2.0f, 8.0f, 0.0f);
        glVertex3f(-2.0f, 8.0f, 3.0f);
        glVertex3f(1.0f, 8.0f, 0.0f);
        glVertex3f(1.0f, 8.0f, 3.0f);
        glVertex3f(3.0f,6.0f, 0.0f);
        glVertex3f(3.0f,6.0f, 3.0f);
        glVertex3f(3.0f,3.0f, 0.0f);
        glVertex3f(3.0f,3.0f, 3.0f);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, 0.0f, 3.0f);
        glVertex3f(-2.0f, 0.0f, 0.0f);
        glVertex3f(-2.0f, 0.0f, 3.0f);
    glEnd();

    // Draw letter L
    glBegin(GL_QUAD_STRIP);
        glVertex3f(5.0f, 0.0f, 0.0f);
        glVertex3f(5.0f, 0.0f, 3.0f);
        glVertex3f(5.0f, 8.0f, 0.0f);
        glVertex3f(5.0f, 8.0f, 3.0f);
        glVertex3f(5.0f, 0.0f, 0.0f);
        glVertex3f(5.0f, 0.0f, 3.0f);
        glVertex3f(10.0f, 0.0f, 0.0f);
        glVertex3f(10.0f, 0.0f, 3.0f);
    glEnd();

    // draw E3
    // Vertical bar (main stem of E)
    glBegin(GL_QUAD_STRIP);
        glVertex3f(12.0f, 0.0f, 0.0f);
        //glVertex3f(-10.0f, 0.0f, depth);
        glVertex3f(12.0f, 0.0f, 3.0f);
        //glVertex3f(-10.0f, 10.0f, depth);
        glVertex3f(12.0f, 8.0f, 0.0f);
        //glVertex3f(-8.0f, 10.0f, depth);
        glVertex3f(12.0f, 8.0f, 3.0f);
    glEnd();

    // Top horizontal bar
    glBegin(GL_QUAD_STRIP);
        glVertex3f(12.0f, 8.0f, 0.0f);
        //glVertex3f(-8.0f, 10.0f, depth);
        glVertex3f( 12.0f, 8.0f, 3.0f);
        //glVertex3f( 0.0f, 10.0f, depth);
        glVertex3f( 18.0f, 8.0f, 0.0f);
        //glVertex3f( 0.0f, 8.0f, depth);
        glVertex3f(18.0f, 8.0f, 3.0f);
    glEnd();

    // Middle horizontal bar
    glBegin(GL_QUAD_STRIP);
        glVertex3f(12.0f, 4.0f, 0.0f);
        //glVertex3f(-8.0f, 10.0f, depth);
        glVertex3f( 12.0f, 4.0f, 3.0f);
        //glVertex3f( 0.0f, 10.0f, depth);
        glVertex3f( 17.0f, 4.0f, 0.0f);
        //glVertex3f( 0.0f, 8.0f, depth);
        glVertex3f(17.0f, 4.0f, 3.0f);
    glEnd();

    // Bottom horizontal bar
    glBegin(GL_QUAD_STRIP);
        glVertex3f(12.0f, 0.0f, 0.0f);
        //glVertex3f(-8.0f, 10.0f, depth);
        glVertex3f( 12.0f, 0.0f, 3.0f);
        //glVertex3f( 0.0f, 10.0f, depth);
        glVertex3f( 18.5f, 0.0f, 0.0f);
        //glVertex3f( 0.0f, 8.0f, depth);
        glVertex3f(18.5f, 0.0f, 3.0f);
    glEnd();

    glEnable(GL_CULL_FACE);
}//end of drawWeedleWord

void Battlefield::drawMinunWord(){
    glDisable(GL_CULL_FACE);

    // Draw M
    glBegin(GL_QUAD_STRIP);
        glColor3f(0.0f, 0.0f, 0.85f );
        glVertex3f(-15.0f, 0.0f, 0.0f);
        glVertex3f(-15.0f, 0.0f, 3.0f);
        glVertex3f(-15.0f, 8.0f, 0.0f);
        glVertex3f(-15.0f, 8.0f, 3.0f);
        glVertex3f(-10.0f, 5.0f, 0.0f);
        glVertex3f(-10.0f, 5.0f, 3.0f);
        glVertex3f(-5.0f, 8.0f, 0.0f);
        glVertex3f(-5.0f, 8.0f, 3.0f);
        glVertex3f(-5.0f, 0.0f, 0.0f);
        glVertex3f(-5.0f, 0.0f, 3.0f);
    glEnd();

    // Capital letter "I"
    // Top bar
    glBegin(GL_QUAD_STRIP);
        glVertex3f(-3.0f, 8.0f, 0.0f);
        glVertex3f( -3.0f, 8.0f, 3.0f);
        glVertex3f( 3.0f, 8.0f, 0.0f);
        glVertex3f(3.0f, 8.0f, 3.0f);
    glEnd();

    // Middle stem
    glBegin(GL_QUAD_STRIP);
        glVertex3f(-1.0f, 0.0f, 1.5f);
        glVertex3f(-1.0f, 8.0f, 1.5f);
        glVertex3f( 1.0f, 0.0f, 1.5f);
        glVertex3f( 1.0f, 8.0f, 1.5f);
    glEnd();

    // Bottom bar
    glBegin(GL_QUAD_STRIP);
        glVertex3f(-3.0f, 0.0f, 0.0f);
        glVertex3f( -3.0f, 0.0f, 3.0f);
        glVertex3f( 3.0f, 0.0f, 0.0f);
        glVertex3f(3.0f, 0.0f, 3.0f);
    glEnd();

    // draw N1
    glBegin(GL_QUAD_STRIP);
        glVertex3f(5.0f, 0.0f, 0.0f);
        glVertex3f(5.0f, 0.0f, 3.0f);
        glVertex3f(5.0f, 8.0f, 0.0f);
        glVertex3f(5.0f, 8.0f, 3.0f);
        glVertex3f(12.0f, 0.0f, 0.0f);
        glVertex3f(12.0f, 0.0f, 3.0f);
        glVertex3f(12.0f, 8.0f, 0.0f);
        glVertex3f(12.0f, 8.0f, 3.0f);
        glVertex3f(12.0f, 0.0f, 0.0f);
        glVertex3f(12.0f, 0.0f, 3.0f);
    glEnd();

    //draw U
    glBegin(GL_QUAD_STRIP);
        glVertex3f(15.0f, 8.0f, 0.0f);
        glVertex3f(15.0f, 8.0f, 3.0f);
        glVertex3f(15.0f, 4.0f, 0.0f);
        glVertex3f(15.0f, 4.0f, 3.0f);
        glVertex3f(17.0f, 0.0f, 0.0f);
        glVertex3f(17.0f, 0.0f, 3.0f);
        glVertex3f(20.0f, 0.0f, 0.0f);
        glVertex3f(20.0f, 0.0f, 3.0f);
        glVertex3f(22.0f, 4.0f, 0.0f);
        glVertex3f(22.0f, 4.0f, 3.0f);
        glVertex3f(22.0f, 8.0f, 0.0f);
        glVertex3f(22.0f, 8.0f, 3.0f);
    glEnd();

   //draw N2
    glBegin(GL_QUAD_STRIP);
        glVertex3f(25.0f, 0.0f, 0.0f);
        glVertex3f(25.0f, 0.0f, 3.0f);
        glVertex3f(25.0f, 8.0f, 0.0f);
        glVertex3f(25.0f, 8.0f, 3.0f);
        glVertex3f(32.0f, 0.0f, 0.0f);
        glVertex3f(32.0f, 0.0f, 3.0f);
        glVertex3f(32.0f, 8.0f, 0.0f);
        glVertex3f(32.0f, 8.0f, 3.0f);
        glVertex3f(32.0f, 0.0f, 0.0f);
        glVertex3f(32.0f, 0.0f, 3.0f);
    glEnd();

    glEnable(GL_CULL_FACE);
}//end of drawMinunWord
