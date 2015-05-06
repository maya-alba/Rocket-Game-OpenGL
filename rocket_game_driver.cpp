#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include <string>
#include <ctime>
#include <cmath>

#include "SOIL.h"
#include "position.h"
#include "rocket.h"
#include "asteroid.h"

using namespace std;

// Global variables for window's size
int width = 1000;
int height = 700;
// Global variables for moving camera
int mouseButton = 0;
bool mouseRotate = false;
bool mouseZoom = false;
bool mousePan = false;
int xClick = 0;
int yClick = 0;
int xAux = 0;
int yAux = 0;
double rotX = 0.;
double rotY = 0.;
double panX = 0.;
double panY = 0.;
double zoomZ = -0.;
static char label[100];   

// Global variable for texture
std::string texFile;
std::string const texFileNames[14]={"sun.jpg", "mercury.jpg", "venus.jpg", "earth.bmp", "moon.jpg", "mars.jpg", "jupiter.jpg", "saturn.jpg",
                                    "saturnring.jpg", "uranus.jpg", "neptune.jpg", "pluto.jpg", "rocky.jpg", "DeathStar.jpg"};
std::string const textFileSpace[2]={"space1.jpg", "space.jpg"};
GLuint texID[14];
GLuint texSpaceID[2];
GLuint texControlsID;
GLuint texGameOverID, texGoalID;
GLUquadric *quad[14];
GLUquadric *quadCont;
float rot = 0.;
float rotMoon = 0.;
float rotEarth=0.;


Rocket *rocket;
float rocketSize = 3.;
float rocketX=120;
float rocketY=0.;
float rocketZ=100.;

Asteroid *asteroid;
int const numAst=100;
Asteroid *asteroids[numAst];
int ranX, ranY, ranZ, ranDisp;
float velocities[6] = {0.05, 0.009, 0.006, 0.008, 0.03, 0.01};
float ranDispX, ranDispY, ranDispZ;

float d2, radii2;

float signX, signY, signZ;

int mainWindow;
int gameSubWindow;
int controlSubWindow;

Asteroid *deathStar;

int gameOver=0;

void init()
{
    glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, zoomZ); // Camera inicialization  
}

void initTextures(){
    std::cout << "Loading Textures..." << std::endl;
    // Texture Initialization
    for(int i=0; i<14; i++){
        texFile = texFileNames[i];
        
        texID[i] = SOIL_load_OGL_texture(
		texFile.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_POWER_OF_TWO
		);
        
        if (texID[i] > 0){
		    glEnable(GL_DEPTH_TEST);
		    glEnable(GL_TEXTURE_2D);
		    glBindTexture(GL_TEXTURE_2D, texID[i]);
            //funcion cuádrica
		    quad[i] = gluNewQuadric();
		    gluQuadricTexture(quad[i], 1);
	    }
	    else{
		    std::cout << "Texture File: " << texFile << " not found!" << std::endl;
	    }
    }

    for(int j=0; j<2; j++){
        texFile = textFileSpace[j];
	    texSpaceID[j] = SOIL_load_OGL_texture(
		    texFile.c_str(),
		    SOIL_LOAD_AUTO,
		    SOIL_CREATE_NEW_ID,
		    SOIL_FLAG_POWER_OF_TWO
		    );
	    if (texSpaceID[j] > 0)
	    {
		    glEnable(GL_TEXTURE_2D);
		    glBindTexture(GL_TEXTURE_2D, texSpaceID[j]);
            glDisable(GL_TEXTURE_2D);
	    }
	    else
	    {
		    std::cout << "Texture File: " << texFile << " not found!" << std::endl;
	    }
    }

    texFile = "gameover.png";
	texGameOverID = SOIL_load_OGL_texture(
		texFile.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_POWER_OF_TWO
		);
	if (texGameOverID > 0)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texGameOverID);
        glDisable(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Texture File: " << texFile << " not found!" << std::endl;
	}

    texFile = "goal.jpg";
	texGoalID = SOIL_load_OGL_texture(
		texFile.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_POWER_OF_TWO
		);
	if (texGoalID > 0)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texGoalID);
        glDisable(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Texture File: " << texFile << " not found!" << std::endl;
	}

     std::cout << "All textures loaded!" << std::endl;
     std::cout  << std::endl;
}

void initDeathStar(){
    deathStar = new Asteroid();
    deathStar->size=9.0;
    ranX=rand()%250;
    if(rand()%2 == 0){
        ranX = -ranX;
    }
           
    ranY=rand()%100;
    if(rand()%2 == 0){
        ranY = -ranY;
    }
            
    ranZ=rand()%250;
    if(rand()%2 == 0){
        ranZ = -ranZ;
    }
    deathStar->setRandomInitPosition(ranX, ranY, ranZ);
    for(int k=0; k<3; k++){
        ranDisp = rand()%6;
        if(k==0){
            ranDispX=velocities[ranDisp];
            if(asteroid->pos->x > 150){
                ranDispX= -ranDispX;
            }
        }
        else if(k==1){
            ranDispY=velocities[ranDisp];
            if(asteroid->pos->y > 50){
                ranDispY= -ranDispY;
            }
        }
        else if(k==2){
            ranDispZ=velocities[ranDisp];
            if(asteroid->pos->z > 150){
                ranDispZ= -ranDispZ;
            }
        }
    }
    deathStar->setDisplacementVelocity(ranDispX, ranDispY, ranDispZ);
    std::cout << "Death Star X position " << ranX << std::endl;
    std::cout << "Death Star Y position " << ranY << std::endl;
    std::cout << "Death Star Z position " << ranZ << std::endl;
}

void initGame(){
    
    initTextures();
    
    rocket = new Rocket(rocketSize);
    rocket->setInitPosition();

    srand(time(NULL));
    for(int i=0; i<numAst; i++){
        asteroid = new Asteroid();
        //set random positions
        for(int j=0; j<3; j++){
            if(j==0){
                ranX=rand()%250;
                if(rand()%2 == 0){
                    ranX = -ranX;
                }
            }
            else if(j==1){
                ranY=rand()%100;
                if(rand()%2 == 0){
                    ranY = -ranY;
                }
            }
            else if(j==2){
                ranZ=rand()%250;
                if(rand()%2 == 0){
                    ranZ = -ranZ;
                }
            }
        }
        asteroid->setRandomInitPosition(ranX, ranY, ranZ);
        //set displacements
        for(int k=0; k<3; k++){
            ranDisp = rand()%6;
           if(k==0){
               ranDispX=velocities[ranDisp];
               if(asteroid->pos->x > 150){
                   ranDispX= -ranDispX;
               }
           }
           else if(k==1){
               ranDispY=velocities[ranDisp];
               if(asteroid->pos->y > 50){
                   ranDispY= -ranDispY;
               }
           }
           else if(k==2){
               ranDispZ=velocities[ranDisp];
               if(asteroid->pos->z > 150){
                   ranDispZ= -ranDispZ;
               }
           }
        }
        asteroid->setDisplacementVelocity(ranDispX, ranDispY, ranDispZ);
        asteroids[i]=asteroid;
    }
    initDeathStar();
}

/*
void initControll(){
    texFile = "controls.png";
    texControlsID = SOIL_load_OGL_texture(
	texFile.c_str(),
	SOIL_LOAD_AUTO,
	SOIL_CREATE_NEW_ID,
	SOIL_FLAG_POWER_OF_TWO
	);
        
    if (texControlsID > 0){
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texControlsID);
        glDisable(GL_TEXTURE_2D);
	}
	else{
		std::cout << "Texture File: " << texFile << " not found!" << std::endl;
	}
}*/

void drawString (char *s){ 
  unsigned int i; 
  for (i = 0; i < strlen (s); i++) 
    glutBitmapCharacter (GLUT_BITMAP_9_BY_15, s[i]); 
}; 

void reshapeViewportGameSub(int w, int h)
{
    glutSetWindow(gameSubWindow);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (height == 0)
	{
		height = 1;
	}
	gluPerspective(25.0, (double)width / height, 0.1, 1000.0);	//proyección en perspectiva
	glMatrixMode(GL_MODELVIEW);
}

/*
void reshapeViewportControlSub(int w, int h)
{
    glutSetWindow(controlSubWindow);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (height == 0)
	{
		height = 1;
	}
	gluPerspective(25.0, (double)width / height, 0.1, 1000.0);	//proyección en perspectiva
	glMatrixMode(GL_MODELVIEW);
}
*/

void reshapeViewport(int w, int h){
    //medidas para la ventana actual
    width=w;
    height=h;
    glutSetWindow(gameSubWindow);
    glutPositionWindow(0,0);
    reshapeViewportGameSub(w, h);
    /*glutSetWindow(controlSubWindow);
    glutPositionWindow(0,(3*h)/4);
    reshapeViewportControlSub(w, h/4);*/
}

void drawSpaceWalls(){
    //BACK WALL
    glBindTexture(GL_TEXTURE_2D,texSpaceID[0]);
	    glBegin(GL_QUADS);
	    // vertex 1
	    glTexCoord2f(0., 0.);
	    glVertex3f(-250., -100., -250.);
	    // vertex 2
	    glTexCoord2f(1., 0.);
	    glVertex3f(250., -100., -250.);
	    // vertex 3
	    glTexCoord2f(1., 1.);
	    glVertex3f(250., 100., -250.);
	    // vertex 4
	    glTexCoord2f(0., 1.);
	    glVertex3f(-250., 100., -250.);
	glEnd();
    
    //RIGHT WALL
    glBindTexture(GL_TEXTURE_2D,texSpaceID[1]);
	    glBegin(GL_QUADS);
	    // vertex 1
	    glTexCoord2f(0., 0.);
	    glVertex3f(250., -100., -250.);
	    // vertex 2
	    glTexCoord2f(1., 0.);
	    glVertex3f(250., -100., 250.);
	    // vertex 3
	    glTexCoord2f(1., 1.);
	    glVertex3f(250., 100., 250.);
	    // vertex 4
	    glTexCoord2f(0., 1.);
	    glVertex3f(250., 100., -250.);
	glEnd();

    //LEFT WALL
    glBindTexture(GL_TEXTURE_2D,texSpaceID[0]);
	    glBegin(GL_QUADS);
	    // vertex 1
	    glTexCoord2f(0., 0.);
	    glVertex3f(-250., -100., 250.);
	    // vertex 2
	    glTexCoord2f(1., 0.);
	    glVertex3f(-250., -100., -250.);
	    // vertex 3
	    glTexCoord2f(1., 1.);
	    glVertex3f(-250., 100., -250.);
	    // vertex 4
	    glTexCoord2f(0., 1.);
	    glVertex3f(-250., 100., 250.);
	glEnd();

    //FRONT WALL
    glBindTexture(GL_TEXTURE_2D,texSpaceID[1]);
	    glBegin(GL_QUADS);
	    // vertex 1
	    glTexCoord2f(0., 0.);
	    glVertex3f(-250., -100., 250.);
	    // vertex 2
	    glTexCoord2f(1., 0.);
	    glVertex3f(250., -100., 250.);
	    // vertex 3
	    glTexCoord2f(1., 1.);
	    glVertex3f(250., 100., 250.);
	    // vertex 4
	    glTexCoord2f(0., 1.);
	    glVertex3f(-250., 100., 250.);
	glEnd();

    //UP WALL
    glBindTexture(GL_TEXTURE_2D,texSpaceID[1]);
	    glBegin(GL_QUADS);
	    // vertex 1
	    glTexCoord2f(0., 0.);
	    glVertex3f(-250., 100., -250.);
	    // vertex 2
	    glTexCoord2f(1., 0.);
	    glVertex3f(250., 100., -250.);
	    // vertex 3
	    glTexCoord2f(1., 1.);
	    glVertex3f(250., 100., 250.);
	    // vertex 4
	    glTexCoord2f(0., 1.);
	    glVertex3f(-250., 100., 250.);
	glEnd();

    //BOTTOM WALL
    glBindTexture(GL_TEXTURE_2D,texSpaceID[0]);
	    glBegin(GL_QUADS);
	    // vertex 1
	    glTexCoord2f(0., 0.);
	    glVertex3f(-250., -100., 250.);
	    // vertex 2
	    glTexCoord2f(1., 0.);
	    glVertex3f(250., -100., 250.);
	    // vertex 3
	    glTexCoord2f(1., 1.);
	    glVertex3f(250., -100., -250.);
	    // vertex 4
	    glTexCoord2f(0., 1.);
	    glVertex3f(-250., -100., -250.);
	glEnd();
}

void drawPlanets(){
    //SUN
    glBindTexture(GL_TEXTURE_2D,texID[0]);
    gluSphere(quad[0],25,20,20);
    {
        //MERCURY
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D,texID[1]);
	    glRotatef(rot, 0., 1., 0.);
        glTranslatef(30.,0.,30.);
	    gluSphere(quad[1], 4, 20, 20);
        glPopMatrix();
    }
    {
        //VENUS
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D,texID[2]);
	    glRotatef(rot, 0., 1., 0.);
        glTranslatef(45.,0.,-20.);
	    gluSphere(quad[2], 6, 20, 20);
        glPopMatrix();
    }
    {
        //EARTH
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D,texID[3]);
	    glRotatef(rot, 0., 1., 0.);
        glRotatef(-90,1.0,0.,0.);
        glTranslatef(65.,0.,10.);
	    gluSphere(quad[3], 6, 20, 20);
            {
                //MOON
                glPushMatrix();
                glBindTexture(GL_TEXTURE_2D,texID[4]);
                glRotatef(rotMoon, 0., 0., 1.);
                glTranslatef(4.,0.,0.);
	            gluSphere(quad[4], 2, 20, 20);
                glPopMatrix();
            }
        glPopMatrix();
    }
    {
        //MARS
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D,texID[5]);
	    glRotatef(rot, 0., 1., 0.);
        glTranslatef(75.,0.,-30.);
	    gluSphere(quad[5], 4, 20, 20);
        glPopMatrix();
    }
    {
        //JUPITER
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D,texID[6]);
	    glRotatef(rot, 0., 1., 0.);
        glRotatef(-90,1.0,0.,0.);
        glRotatef(rot, 1.0,0.,0.);
        glTranslatef(105.,35.,20.);
	    gluSphere(quad[6], 12, 20, 20);
        glPopMatrix();
    }
    {
        //SATURN
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D,texID[7]);
	    glRotatef(rot, 0., 1., 0.);
        glRotatef(90,1.0,0.,0.);
        glTranslatef(140.,0.,10.);
	    gluSphere(quad[7], 9, 20, 20);
            {
                //SATURN RING
                glPushMatrix();
                glBindTexture(GL_TEXTURE_2D,texID[8]);
                glRotatef(90,1.,0.,0.);
                glRotatef(-20, 0.,0.,1.);
                glScalef(1.0,0.1,1.);
	            gluSphere(quad[8], 15, 20, 20);
                glPopMatrix();
            }
            glPopMatrix();
    }
    {
        //URANUS
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D,texID[9]);
	    glRotatef(rot, 0., 1., 0.);
        glRotatef(90,1.,0.,0.);
        glTranslatef(170.,0.,-10.);
	    gluSphere(quad[9], 9, 20, 20);
        glPopMatrix();
    }
    {
        //NEPTUNE
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D,texID[10]);
	    glRotatef(rot, 0., 1., 0.);
        glRotatef(90,1.,0.,0.);
        glTranslatef(195.,0.,0.);
	    gluSphere(quad[10], 8, 20, 20);
        glPopMatrix();
    }
    {
        //PLUTO
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D,texID[11]);
	    glRotatef(rot, 0., 1., 0.);
        glRotatef(90,1.,0.,0.);
        glTranslatef(210.,0.,-8.);
	    gluSphere(quad[11], 1.5, 20, 20);
        glPopMatrix();
    }
}

void processForwardBackward(unsigned char key, int x, int y){
    if(key == 'w'){ 
        //std::cout << "FORWARD"<< std::endl;
        rocket->moveOnZ(-0.7);
    }
    if(key == 's'){ 
        //std::cout << "BACKWARD"<< std::endl;
        rocket->moveOnZ(0.7);
    }
}

void processMovementKeys(int key, int x, int y){
    if(key == GLUT_KEY_UP){ 
        //std::cout << "UP"<< std::endl;
        rocket->moveOnY(0.7);
    }
    if(key == GLUT_KEY_RIGHT){ 
        //std::cout << "RIGHT"<< std::endl;
        rocket->moveOnX(0.7);
    }
    if(key == GLUT_KEY_DOWN){ 
        //std::cout << "DOWN"<< std::endl;
        rocket->moveOnY(-0.7);
    }
    if(key == GLUT_KEY_LEFT){ 
        //std::cout << "LEFT"<< std::endl;
        rocket->moveOnX(-0.7);
    }
}

float calculateDistance(int ast){
    //d2 = (x1-x2)2 + (y1-y2)2 + (z1-z2)2
    return pow((rocket->pos->x - asteroids[ast]->pos->x),2) + pow((rocket->pos->y - asteroids[ast]->pos->y),2) + pow((rocket->pos->z - asteroids[ast]->pos->z),2);
}

void checkBoundaries(){
    for(int i=0; i<numAst; i++){
        signX=asteroids[i]->dispX;
        signY=asteroids[i]->dispY;
        signZ=asteroids[i]->dispZ;
        if( asteroids[i]->pos->x >= 250 || asteroids[i]->pos->x <= -250){
            signX=-signX;
        }
        if( asteroids[i]->pos->y >= 100 || asteroids[i]->pos->y <= -100){
            signY=-signY;
        }
        if( asteroids[i]->pos->z >= 250 || asteroids[i]->pos->z <= -250){
            signZ=-signZ;
        }
        asteroids[i]->setDisplacementVelocity(signX, signY, signZ);
    }
    //death star boundaries
    signX=deathStar->dispX;
    signY=deathStar->dispY;
    signZ=deathStar->dispZ;
    if( deathStar->pos->x >= 250 || deathStar->pos->x <= -250){
        signX=-signX;
    }
    if( deathStar->pos->y >= 100 || deathStar->pos->y <= -100){
        signY=-signY;
    }
    if( deathStar->pos->z >= 250 || deathStar->pos->z <= -250){
        signZ=-signZ;
    }
    deathStar->setDisplacementVelocity(signX, signY, signZ);

}

void checkAsteroidCollisions(){
    for(int i=0; i<numAst; i++){
        d2 = calculateDistance(i);
        radii2 = pow( (rocket->size + asteroids[i]->size) ,2);
        if(d2 <= radii2){
            std::cout << "Collided with an asteroid!! :(" << std::endl;
            gameOver=1;
        }
    }
}

void checkDeathStarArrival(){
    d2 = pow((rocket->pos->x - deathStar->pos->x),2) + pow((rocket->pos->y - deathStar->pos->y),2) + pow((rocket->pos->z - deathStar->pos->z),2);
    radii2 = pow( (rocket->size + deathStar->size) ,2);
    if(d2 <= radii2){
        std::cout << "Arrived to Death Star!! :)" << std::endl;
        gameOver=2;
    }
}

void display(){
    glutSetWindow(gameSubWindow);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Moving camera
	glPushMatrix();
	glTranslatef(panX, panY, zoomZ);
	glRotatef(rotY, 1.0, 0.0, 0.0);
	glRotatef(rotX, 0.0, 1.0, 0.0);

    

    if(gameOver == 0){

        //eyeX, eyeY, eyeZ
        //centerX,centerY,centerZ
        //upX, upY, upZ
        gluLookAt(rocket->pos->x, rocket->pos->y, rocket->pos->z, rocket->pos->x , rocket->pos->y, rocket->pos->z-1,0.,1.,0.);
        //gluLookAt(rocketX, rocketY, rocketZ, rocketX , rocketY, rocketZ-1,0.,1.,0.);
        //gluLookAt(rocketX+0.5, rocketY+0.5, rocketZ+0.5, rocketX , rocketY, rocketZ,0.,1.,0.);
        //gluLookAt(rocketX+0.5, rocketY, rocketZ+0.5, rocketX+1+(toX*1) , rocketY+0+(toY*1), rocketZ+0+(toZ*1),0.,1.,0.);

	    // Scene Model
        glEnable(GL_TEXTURE_2D);
    
        drawSpaceWalls();
        drawPlanets();

        //Move and draw asteroids
        glBindTexture(GL_TEXTURE_2D,texID[12]);
        for(int i=0; i<numAst; i++){
            asteroids[i]->moveOnX();
            asteroids[i]->moveOnY();
            asteroids[i]->moveOnZ();
            asteroids[i]->drawAsteroid(quad[12]);
        }
        
        //Move and draw Death Star
        glBindTexture(GL_TEXTURE_2D,texID[13]);
        deathStar->moveOnX();
        deathStar->moveOnY();
        deathStar->moveOnZ();
        deathStar->drawAsteroid(quad[13]);
        glDisable(GL_TEXTURE_2D);

        rocket->drawRocket();

        //check collisions
        checkAsteroidCollisions();

        //check Asteroid Boundaries
        checkBoundaries();

        //check if arrived to Death Star
        checkDeathStarArrival();

	    //End Scene Model
	
	    rot = rot + 0.003;
        rotMoon = rotMoon + 0.01;
    }
    //User crashed with asteroid
    else if (gameOver == 1){
        zoomZ=-8;
        /*glColor3f (1.0, 1.0, 1.0); 
        sprintf (label, "GAME OVER"); 
        glRasterPos2f (-0.0, 0.0); 
        drawString (label);*/
        //Draw Game Over Sign
        glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,texGameOverID);
	        glBegin(GL_QUADS);
	        // vertex 1
	        glTexCoord2f(0., 0.);
	        glVertex3f(-1., -0.5, 0.);
	        // vertex 2
	        glTexCoord2f(1., 0.);
	        glVertex3f(1., -0.5, 0.);
	        // vertex 3
	        glTexCoord2f(1., 1.);
	        glVertex3f(1., 0.5, 0.);
	        // vertex 4
	        glTexCoord2f(0., 1.);
	        glVertex3f(-1., 0.5, 0.);
	    glEnd();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
    //USer won
    else if(gameOver == 2){

        zoomZ=-5;
        glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,texGoalID);
	        glBegin(GL_QUADS);
	        // vertex 1
	        glTexCoord2f(0., 0.);
	        glVertex3f(-0.8, -0.5, 0.);
	        // vertex 2
	        glTexCoord2f(1., 0.);
	        glVertex3f(0.8, -0.5, 0.);
	        // vertex 3
	        glTexCoord2f(1., 1.);
	        glVertex3f(0.8, 0.5, 0.);
	        // vertex 4
	        glTexCoord2f(0., 1.);
	        glVertex3f(-0.8, 0.5, 0.);
	    glEnd();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }

	glPopMatrix();
	glutSwapBuffers();	// Swap buffers
}

/*
void drawSceneControl(){

    glutSetWindow(controlSubWindow);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Moving camera
	glPushMatrix();
	glTranslatef(panX, panY, zoomZ);
	glRotatef(rotY, 1.0, 0.0, 0.0);
	glRotatef(rotX, 0.0, 1.0, 0.0);

    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texControlsID);
	    glBegin(GL_QUADS);
	    // vertex 1
	    glTexCoord2f(0., 0.);
	    glVertex3f(-1., 0, 0.);
	    // vertex 2
	    glTexCoord2f(1., 0.);
	    glVertex3f(2., 0., 0.);
	    // vertex 3
	    glTexCoord2f(1., 1.);
	    glVertex3f(2., 2., 0.);
	    // vertex 4
	    glTexCoord2f(0., 1.);
	    glVertex3f(-1., 2., 0.);
	glEnd();
    glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glutSwapBuffers();	// Swap buffers
}*/

void drawScene(){
    display();
    //drawSceneControl();
}

void idle()
{
	glutPostRedisplay();    // call display
}

void mouse(int button, int state, int x, int y)
{
	mouseButton = button;
	mouseRotate = false;
	mouseZoom = false;
	mousePan = false;
	xClick = x;
	yClick = y;
	if (button == GLUT_LEFT_BUTTON)
	{
		mouseRotate = true;
		xAux = rotX;
		yAux = rotY;
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		mouseZoom = true;
		xAux = zoomZ;
	}
	else if (button == GLUT_MIDDLE_BUTTON)
	{
		mousePan = true;
		xAux = panX;
		yAux = panY;
	}
}

void mouseMotion(int x, int y)
{
	if (mouseRotate == true)
	{
		if (mouseButton == GLUT_LEFT_BUTTON)
		{
			if ((x - xClick + xAux) > 360.0)
			{
				xAux = xAux - 360.0;
			}
			if ((x - xClick + xAux) < 0.0)
			{
				xAux = xAux + 360.0;
			}
			if ((y - yClick + yAux) > 360.0)
			{
				yAux = yAux - 360.0;
			}
			if ((y - yClick + yAux) < 0.0)
			{
				yAux = yAux + 360.0;
			}
			rotX = x - xClick + xAux;
			rotY = y - yClick + yAux;
		}
	}
	else if (mouseZoom == true)
	{
		if (mouseButton == GLUT_RIGHT_BUTTON)
		{
			zoomZ = ((x - xClick) / 10.0) + xAux;
		}
	}
	else if (mousePan == true)
	{
		if (mouseButton == GLUT_MIDDLE_BUTTON)
		{
			panX = ((x - xClick) / 63.0) + xAux;
			panY = ((y - yClick) / (-63.0)) + yAux;
		}
	}
}

int main(int argc, char** argv)
{
	// Init Window (GLUT)
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	 mainWindow = glutCreateWindow("Rocket Game");

     gameSubWindow = glutCreateSubWindow(mainWindow,0,0,width,height);
     //controlSubWindow = glutCreateSubWindow(mainWindow,0,(3*height)/4, width, height/4);

     glutSetWindow(gameSubWindow);
     init();
     initGame();
     //glutSetWindow(controlSubWindow);

	// Callback functions registration (GLUT)
    glutSetWindow(mainWindow);
	glutReshapeFunc(reshapeViewport);
	glutDisplayFunc(drawScene);
	glutIdleFunc(drawScene);


    glutSetWindow(gameSubWindow);
	glutReshapeFunc(reshapeViewportGameSub);
	glutDisplayFunc(display);		// display scene function
    glutKeyboardFunc(processForwardBackward);
    glutSpecialFunc(processMovementKeys);
	glutIdleFunc(idle);				// idle function
	glutMouseFunc(mouse);			// processing mouse events function
	glutMotionFunc(mouseMotion);	// processing mouse motion function

    /*
    glutSetWindow(controlSubWindow);
	glutReshapeFunc(reshapeViewportControlSub);
    glutDisplayFunc(drawSceneControl);
    glutMouseFunc(mouse);			// processing mouse events function
	glutMotionFunc(mouseMotion);
    initControll();*/

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "GLEW error");
		return 1;
	}
	glutMainLoop();					// Main loop waiting for events!
	return 0;
}
