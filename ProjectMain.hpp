// Project by - Section 1E (Group Rational)
// Character - Weedle & Minun from Pokemon
// 242UT2449P SEE CHWAN KAI
// 242UT24490 TEO JING AN
// 242UT2449Z KHO WEI CONG
// 242UT244B2 TEE KIAN HAO

#ifndef KSST_PROJECTMAIN_HPP
#define KSST_PROJECTMAIN_HPP

#include "RationalMain.hpp"

#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "CGimageloader.hpp"

namespace ProjectMain {
    const float PI = 3.14159265358979323846f;

    class MyVirtualWorld;

    class Background {
        private:
            MyVirtualWorld * world;
        public:
            Background(MyVirtualWorld * w): world(w) {}
            void drawhousebody();
            void drawdoor();
            void drawcirclewindow();
            void drawsquarewindow();
            void drawbalcony();
            void drawroof();
            void drawhouse();
            void drawtree();
            void drawgrass();
            void drawgrasses(int count, float spread);
            void drawHouseArc();
            void drawforest(int count, float spread);
    };

    class Weedle {
        public:
            void drawhead();
            void drawbody();
            void drawtail();
            void drawweedle();
    };

    class Minun {
        private:
            GLUquadricObj * pObj;
            const GLfloat * profilepoints;
            GLint numofprofilepoints;
            GLfloat * surfacepoints;
            GLint numofsurfacepoints;
            GLint numofverticallines;
            //animation
            GLfloat velx, vely, velz; //unit per sec

        public:
            Minun();
            ~Minun();
            //note: size of profilePoints = 3*numOfProfilePoints
            void setup(const GLfloat * profilePoints,
                GLint numOfProfilePoints,
                GLfloat degreeStart,
                GLfloat degreeEnd,
                GLfloat degreeStep);
            void leg();
            void body();
            void hand();
            void head();
            void ear();
            void face();
            void tail();
            void eyes();
            void cheeks();
            void mouth();
            // combine for the face
            void expression();
            void assemble();
    };

    class Battlefield {
        // battle field
        private:
            MyVirtualWorld * world;
        public:
            Battlefield(MyVirtualWorld * w): world(w) {}
            long int timeold,
            timenew,
            elapseTime;
            float gridSize = 1.0f;
            float arenaHalfWidth = 20.0f; // X-axis
            float arenaHalfLength = 25.0f;
            float wallHeight = 5.0f;
            float wallThickness = 0.5f;
            float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
            float screenX = arenaHalfWidth + 2.0f;
            float screenY = 1.0f;
            float screenZ = 0.0f;
            float screenWidth = 3.0f;
            float screenHeight = 1.5f;
            void drawbattlearena();
            void drawfloor();
            void drawArenaWalls();
            void drawPokeBall();
            void drawHUDTextScreen(const char * line1, const char * line2);
            void drawWeedleWord();
            void drawMinunWord();
    };

    struct Projectile {
        float x, y, z; // Position
        float speedX, speedY, speedZ; // Velocity
        bool active;
    };

    class MySpotLights {
        public:
            MySpotLights();
            ~MySpotLights();
            void setupLights();
            void toggleLight(int lightno);
            void draw();
            void drawlamppost();
            void tickTime(long int elapseTime); //elapsetime in milisec
            private: GLUquadricObj * pObj;
            GLfloat rotateangle, rotatespeed;
            bool lighton[2];
    };

    class MyVirtualWorld {
        public:
            MyVirtualWorld();
            ~MyVirtualWorld();
            void tickTime();
            void init();

            Background background;
            Weedle weedle;
            Minun minun;
            Battlefield battlefield;

            MySpotLights myspotlights;
            bool lighton[3];
            long int timeold,
            timenew,
            elapseTime;
            bool minunAttacking = false;
            float minunAttackOffset = 0.0f;
            bool minunGoingForward = true;
            bool weedleAttacking = false;
            float weedleAttackOffset = 0.0f;
            bool weedleGoingForward = true;
            bool autoRotatefunc = false;

            static const int MAX_PROJECTILES = 10;
            Projectile minunProjectiles[MAX_PROJECTILES];
            Projectile weedleProjectiles[MAX_PROJECTILES];
            bool minunAttackEffectActive = false;
            bool weedleAttackEffectActive = false;

            void finalassemble();

            // pokemon's effect
            void startMinunAttackEffect();
            void startWeedleAttackEffect();
            void updateProjectiles(Projectile * projectiles, bool & effectActive);
            void drawMinus3D();
            void drawWaterDrop3D();

            // lighting
            void setupLights();
            void toggleLight(int lightno);
            void drawSpotLights();

            // texture
            void setupTextures();
            void toggleTextures();
            int numberOfTexture;
            GLuint * textureNumbers;
            bool textureison;

        void draw() {
            //background.drawhousebody();
            //background.drawdoor();
            //background.drawcirclewindow();
            //background.drawsquarewindow();
            //background.drawbalcony();
            //background.drawroof();
            //background.drawhouse();
            //background.drawgrass();
            //background.drawtree();
            //background.drawgrasses(1000,150.0f);
            //background.drawHouseArc();
            //background.drawforest(200,90.0f);
            //weedle.drawhead();
            //weedle.drawbody();
            //weedle.drawtail();
            //weedle.drawweedle();
            //minun.leg();
            //minun.body();
            //minun.hand();
            //minun.head();
            //minun.ear();
            //minun.tail();
            //minun.eyes();
            //minun.cheeks();
            //minun.mouth();
            //minun.assemble();
            //battlefield.drawfloor();
            //battlefield.drawArenaWalls();
            //battlefield.drawPokeBall();
            //battlefield.drawHUDTextScreen("BATTLE START!", "Blue VS Red");
            //battlefield.drawbattlearena();
            //myspotlights.draw();
            //drawSpotLights();
            //battlefield.drawWeedleWord();
            //battlefield.drawMinunWord();
            //drawMinus3D();
            //drawWaterDrop3D();

            finalassemble();
        }
    };
}
#endif // KSST_PROJECTMAIN_HPP
