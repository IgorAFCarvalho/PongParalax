#include <windows.h>
#include <gl/glut.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


GLfloat xstep = 1.0f;
GLfloat ystep = 1.0f;

GLfloat windowWidth = 1080.0f;
GLfloat windowHeight = 720.0f;

float ball_speed = 0.1;

float ball_x = 0.0f;
float ball_y = 0.0f;

float score_player_left = 0.0f;
float score_player_right = 0.0f;

float goals_center_y = 250.0f;
float goal_top_y = 380;
float goal_bottom_y = 100;

GLfloat barWidth = 12.0f;
GLfloat barHeight = 140.0f;
GLfloat bar1_x = 50.0f;
GLfloat bar1_y = 350.0f;
GLfloat bar2_x = windowWidth - barWidth - 50.0f;
GLfloat bar2_y = 350.0f;

float x1_ = 50.0f;
float y1_ = 120.0f;
float rsize = 28.0f;

GLuint fieldTexture;
GLuint torcidaTexture1;
GLuint torcidaTexture2;
GLuint placar0Texture;
GLuint placar1Texture;
GLuint placar2Texture;
GLuint placar3Texture;
GLuint bolaTexture;

bool loadTexture = true;

float lightX, lightY = 0.0;
float lightZ = 40;
GLfloat luzAmbiente[4] = {0.5,0.5,0.5,1.0};
GLfloat luzDifusa[4] = {0.7,0.7,0.7,1.0};
GLfloat luzEspecular[4] = {1.0,1.0,1.0,1.0};
GLfloat posicaoLuz[4] = {lightX, lightY, lightZ, 1.0};

std::string relativePath;

bool torcida;
int timer = 0;

float corBolaR = 1.0f;
float corBolaG = 1.0f;
float corBolaB = 1.0f;

void caminhosDinamicos(){
    const char* sourceFilePath = __FILE__;
    std::string fullPath(sourceFilePath);

    // Extract the directory of the source file
    size_t lastSlash = fullPath.rfind('/');
    relativePath = fullPath.substr(0, lastSlash);

    // Remove the main.cpp part from the current path
    size_t mainPos = relativePath.rfind("\main.cpp");
    if (mainPos != std::string::npos) {
        relativePath = relativePath.substr(0, mainPos);
    }

    size_t pos = 0;
    while ((pos = relativePath.find("\\", pos)) != std::string::npos) {
        relativePath.replace(pos, 1, "\\\\");
        pos += 2;  // Move past the replaced double backslash
    }
}

GLuint loadImage(const char *imagepath){
    GLuint textureID;
    int width, height, nrComponents;
    unsigned char *data = stbi_load(imagepath, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << imagepath << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void loadTextures(){
    if(loadTexture){
    std::string  fieldTexturePath = relativePath + "Campo.png";
    fieldTexture = loadImage(fieldTexturePath.c_str());

    std::string torcidaTexture1Path = relativePath + "torcida1.png";
    torcidaTexture1 = loadImage(torcidaTexture1Path.c_str());

    std::string torcidaTexture2Path = relativePath + "torcida2.png";
    torcidaTexture2 = loadImage(torcidaTexture2Path.c_str());

    std::string placar0TexturePath = relativePath + "zero64x64.png";
    placar0Texture = loadImage(placar0TexturePath.c_str());

    std::string placar1TexturePath = relativePath + "um64x64.png";
    placar1Texture = loadImage(placar1TexturePath.c_str());

    std::string placar2TexturePath = relativePath + "dois64x64.png";
    placar2Texture = loadImage(placar2TexturePath.c_str());

    std::string placar3TexturePath = relativePath + "tres64x64.png";
    placar3Texture = loadImage(placar3TexturePath.c_str());

    std::string bolaTexturePath = relativePath + "bola64x64.png";
    bolaTexture = loadImage(bolaTexturePath.c_str());

    loadTexture = false;
    }
}

void delay(int v)
{
	glColor3f(1.0f, 1.0f, 0.0f);
	glRectf(-90.0f, 90.0f, -10.0f, 10.0f);
	glutPostRedisplay();
	glutTimerFunc(1000, delay, v);
}

void reset_ball() {
    delay(2000);

    float x = (float)rand()/(float)(RAND_MAX/100);

    corBolaR = 1.0f;
    corBolaG = 1.0f;
    corBolaB = 1.0f;

    x1_ = windowWidth/2 - x;
    y1_ = 120.0f;
}

void DrawBar1(void) {
   glColor3f(0.0f, 0.0f, 1.0f);
   glBegin(GL_QUADS);
   glVertex2f(bar1_x, bar1_y);
   glVertex2f(bar1_x + barWidth, bar1_y);
   glVertex2f(bar1_x + barWidth, bar1_y + barHeight);
   glVertex2f(bar1_x, bar1_y + barHeight);
   glEnd();
}

void DrawBar2(void) {
   // Draw bar 2
   glColor3f(1.0f, 0.0f, 0.0f);
   glBegin(GL_QUADS);
   glVertex2f(bar2_x, bar2_y);
   glVertex2f(bar2_x + barWidth, bar2_y);
   glVertex2f(bar2_x + barWidth, bar2_y + barHeight);
   glVertex2f(bar2_x, bar2_y + barHeight);
   glEnd();
}

void catchKey(int key, int x, int y)
{

    if(key == GLUT_KEY_UP){
        bar1_y += 15.0f;
    }

    else if(key == GLUT_KEY_DOWN)
        bar1_y -= 15.0f;

   if (bar1_y < 20) bar1_y = 20;
   if (bar1_y > windowHeight-rsize- (0.24*windowHeight) - barHeight) bar1_y = windowHeight-rsize- (0.24*windowHeight) - barHeight;
}

void KeyboardHandler(unsigned char key, int x, int y)
{

   if (key == 'w' || key == 'W') {
       bar2_y += 15.0f;
   } else if (key == 's' || key == 'S') {
       bar2_y -= 15.0f;
   } else if (key == 'i' || key == 'I') {
       bar1_y += 15.0f;
   } else if (key == 'k' || key == 'K') {
       bar1_y -= 15.0f;
   }

   if (bar2_y < 20) bar2_y = 20;
   if (bar2_y > windowHeight-rsize- (0.24*windowHeight) - barHeight) bar2_y = windowHeight-rsize- (0.24*windowHeight) - barHeight;
}

int check_for_goal(float x_ball, float y_ball) {

    if ((y_ball > 100000 ) && y_ball < (goal_top_y) && x_ball < bar1_x + rsize - barWidth) {
        ball_speed = 0.1;
        return 1;
    }

    else if ((y_ball > 100000 ) && y_ball < (goal_top_y) && x_ball > bar2_x + barWidth) {
        ball_speed = 0.1;
        return 2;
    }

    return 0;

}

int collided_with_bar(float x_ball, float y_ball) {

    if (y_ball > bar1_y && y_ball < (bar1_y + barHeight) && x_ball < bar1_x + 15) {
        return 1;
    }

    else if (y_ball > bar2_y && y_ball < (bar2_y + barHeight) && x_ball > bar2_x) {
        return 2;
    }

    return 0;
}

void DrawBall(void) {

    // Draw a circle
     glColor3f(corBolaR, corBolaG, corBolaB);
     glBindTexture(GL_TEXTURE_2D,bolaTexture);
     glBegin(GL_POLYGON);

     ball_x = x1_;
     ball_y = y1_;

     for (int i = 0; i < 360; i++)
     {
        float theta = i * 3.14159 / 180;
        float x = x1_ + rsize/2 * cos(theta);
        float y = y1_ + rsize/2 * sin(theta);

        float u = 0.5 + cos(theta)/2;
        float v = 0.5 - sin(theta)/2;

        glTexCoord2f(u, v);
        glVertex2f(x, y);
     }

     glEnd();
     glBindTexture(GL_TEXTURE_2D,0);
}

void DrawBackground(void) {
    glColor3f(1.0f, 1.0f, 1.0f);

    if(torcida)
    glBindTexture(GL_TEXTURE_2D,torcidaTexture1);
    else
    glBindTexture(GL_TEXTURE_2D,torcidaTexture2);
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 1.0f);glVertex2i(0,520);
        glTexCoord2f(0.0f, 1.0f);glVertex2i(windowWidth,520);
        glTexCoord2f(0.0f, 0.0f);glVertex2i(windowWidth,windowHeight);
        glTexCoord2f(1.0f, 0.0f);glVertex2i(0, windowHeight);
    glEnd();
    glBindTexture(GL_TEXTURE_2D,0);
}

void DrawField(void) {
    glColor3f(1.0f, 1.0f, 1.0f);
    GLfloat especularidade[4]={1.0,1.0,1.0,1.0};
    GLint especMaterial = 60;

    glBindTexture(GL_TEXTURE_2D,fieldTexture);

    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(0, 0);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(0, 520);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(1080, 520);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(1080, 0);

    // Define a reflet�ncia do material
    glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
    // Define a concentra��o do brilho
    glMateriali(GL_FRONT,GL_SHININESS,especMaterial);
    glEnd();
    glBindTexture(GL_TEXTURE_2D,0);
}



void Desenha(void)
{
     glEnable(GL_TEXTURE_2D);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();

     glClear(GL_COLOR_BUFFER_BIT);

     glColor3f(1.0f, 0.0f, 0.0f);

     DrawBackground();

     DrawField();

     DrawBall();

     DrawBar1();
     DrawBar2();

     loadTextures();

     glDisable(GL_TEXTURE_2D);

     GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("OpenGL error: %s\n", gluErrorString(error));
    }

     glutSwapBuffers();
}


void Timer(int value)
{

    GLfloat collision = 0.0f;
    GLfloat goal = 0.0f;

    collision = collided_with_bar(ball_x, ball_y);
    goal = check_for_goal(ball_x, ball_y);

    if(x1_> windowWidth-rsize-15 || x1_< 40)
            xstep = -xstep;

    if(y1_ > windowHeight-rsize- (0.25*windowHeight) || y1_ < 30)
          ystep = -ystep;

    if(x1_> windowWidth-rsize)
         x1_= windowWidth-rsize-1;

    if(y1_ > windowHeight-rsize)
         y1_ = windowHeight-rsize-1;



    if (goal == 1) {
        printf("Gol da direita");
        reset_ball();
    }

    if (goal == 2) {
        printf("Gol da esquerda");
        reset_ball();
    }

    if (collision == 1) {
        xstep = -xstep;
    }

    if (collision == 2) {
        xstep = -xstep;
    }

    x1_ += 7 * ball_speed * xstep;
    y1_ += 7 * ball_speed * ystep;

    corBolaG = corBolaG - (ball_speed * abs(xstep)*0.0008);
    corBolaB = corBolaB - (ball_speed * abs(ystep)*0.0008);

    printf("(%f,%f)",corBolaG,corBolaB);

    glutPostRedisplay();
    glutTimerFunc(1, Timer, 1);

    timer = timer + 1;
    if (timer % 5 == 0) {
        torcida = !torcida;
    }
    ball_speed += 0.0005;

}

void Inicializa (void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Define os par�metros da luz de n�mero 0
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
    //glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );

    // Habilita a defini��o da cor do material a partir da cor corrente
    glEnable(GL_COLOR_MATERIAL);
    //Habilita o uso de ilumina��o
    glEnable(GL_LIGHTING);
    // Habilita a luz de n�mero 0
    glEnable(GL_LIGHT0);
}

void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
     if(h == 0) h = 1;

     glViewport(0, 0, w, h);

     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();

     if (w <= h)  {
		windowHeight = 720.0f*h/w;
		windowWidth = 1080.0f;
     }
     else  {
		windowWidth = 1080.0f;
		windowHeight = 720.0f;
     }

     gluOrtho2D(0.0f, windowWidth, 0.0f, windowHeight);
}

int main(void)
{
     caminhosDinamicos();
     glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
     glutInitWindowSize(1080, 720);
     glutInitWindowPosition(0.0,0.0);
     glutCreateWindow("PongParallax");
     glutDisplayFunc(Desenha);

     glutKeyboardFunc(KeyboardHandler);
     glutSpecialFunc(catchKey);

     glutReshapeFunc(AlteraTamanhoJanela);
     glutTimerFunc(1, Timer, 1);
     Inicializa();
     glutMainLoop();
}
