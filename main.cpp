#include <windows.h>
#include <gl/glut.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


GLfloat xstep = 1.0f;
GLfloat ystep = 1.0f;

GLfloat windowWidth = 1080.0f;
GLfloat windowHeight = 720.0f;

float ball_x = 0.0f;
float ball_y = 0.0f;

float score_player_left = 0.0f;
float score_player_right = 0.0f;

float goals_center_y = 250.0f;
float goal_size = 120.0f;


// Position and size of the bars
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

bool loadTexture = true;

float lightX, lightY = 0.0;
float lightZ = 40;
    GLfloat luzAmbiente[4] = {0.5,0.5,0.5,1.0};
    GLfloat luzDifusa[4] = {0.7,0.7,0.7,1.0};
    GLfloat luzEspecular[4] = {1.0,1.0,1.0,1.0};
    GLfloat posicaoLuz[4] = {lightX, lightY, lightZ, 1.0};

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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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
    fieldTexture = loadImage("C:\\Workspace\\PongParalax\\Campo.png");
    printf("1");
    loadTexture = false;
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
    x1_ = windowWidth/2;
    y1_ = 120.0f;
}

void DrawBar1(void) {
   // Draw bar 1
   glColor3f(0.0f, 0.0f, 0.0f);
   glBegin(GL_QUADS);
   glVertex2f(bar1_x, bar1_y);
   glVertex2f(bar1_x + barWidth, bar1_y);
   glVertex2f(bar1_x + barWidth, bar1_y + barHeight);
   glVertex2f(bar1_x, bar1_y + barHeight);
   glEnd();
}

void DrawBar2(void) {
   // Draw bar 2
   glColor3f(0.0f, 0.0f, 0.0f);
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
        bar1_y += 10.0f;
    }

    else if(key == GLUT_KEY_DOWN)
        bar1_y -= 10.0f;

   if (bar1_y < 20) bar1_y = 20;
   if (bar1_y > windowHeight-rsize- (0.24*windowHeight) - barHeight) bar1_y = windowHeight-rsize- (0.24*windowHeight) - barHeight;
}

void KeyboardHandler(unsigned char key, int x, int y)
{

   if (key == 'w' || key == 'W') {
       // Move the right bar up
       bar2_y += 10.0f;
   } else if (key == 's' || key == 'S') {
       // Move the right bar down
       bar2_y -= 10.0f;
   } else if (key == 'i' || key == 'I') {
       // Move the left bar up
       bar1_y += 10.0f;
   } else if (key == 'k' || key == 'K') {
       // Move the left bar down
       bar1_y -= 10.0f;
   }

   if (bar2_y < 20) bar2_y = 20;
   if (bar2_y > windowHeight-rsize- (0.24*windowHeight) - barHeight) bar2_y = windowHeight-rsize- (0.24*windowHeight) - barHeight;
}

int check_for_goal(float x_ball, float y_ball) {

    if ((y_ball >= goals_center_y - goal_size ) && y_ball <= (goals_center_y + goal_size) && x_ball < bar1_x + rsize - 12) {
        return 1;
    }

    else if ((y_ball >= goals_center_y - goal_size ) && y_ball <= (goals_center_y + goal_size) && x_ball > bar2_x + 12) {
        return 2;
    }

    return 0;

}

int collided_with_bar(float x_ball, float y_ball) {

    if (y_ball >= bar1_y && y_ball <= (bar1_y + barHeight) && x_ball <= bar1_x + rsize) {
        return 1;
    }

    else if (y_ball >= bar2_y && y_ball <= (bar2_y + barHeight) && x_ball >= bar2_x) {
        return 2;
    }

    return 0;
}

void DrawBall(void) {

    // Draw a circle
     glColor3f(0.0f, 1.0f, 0.0f);
     glBegin(GL_POLYGON);

     float coordx = 1.0f;
     float coordy = 1.0f;

     ball_x = x1_;
     ball_y = y1_;

     for (int i = 0; i < 360; i++)
     {
         float theta = i * 3.14159 / 180;
         float x = x1_ + rsize/2 * cos(theta);
         float y = y1_ + rsize/2 * sin(theta);


         glVertex2f(x, y);
     }

     glEnd();
}

void DrawBackground(void) {
     glColor3f(1.0f, 0.0f, 0.0f);

     glBegin(GL_QUADS);
               glVertex2i(0,0);
               glVertex2i(windowWidth,0);
               glVertex2i(windowWidth,windowHeight);

               glColor3f(0.0f, 0.0f, 1.0f);
               glVertex2i(0, windowHeight);

     glEnd();
}

void DrawField(void) {
    glColor3f(1.0f, 1.0f, 1.0f);
    GLfloat especularidade[4]={1.0,1.0,1.0,1.0};
    GLint especMaterial = 60;
    glBindTexture(GL_TEXTURE_2D,fieldTexture);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(20, 20);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(50, 520);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(1030, 520);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(1060, 20);

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

     glDisable(GL_TEXTURE_2D);

     GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("OpenGL error: %s\n", gluErrorString(error));
    }

    loadTextures();

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

    x1_ += 7*xstep;
    y1_ += 7*ystep;

    glutPostRedisplay();
    glutTimerFunc(1, Timer, 1);

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
