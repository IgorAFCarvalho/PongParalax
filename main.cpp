

#include <windows.h>
#include <gl/glut.h>
#include <math.h>
#include <stdio.h>


GLfloat xstep = 1.0f;
GLfloat ystep = 1.0f;

GLfloat windowWidth = 1080.0f;
GLfloat windowHeight = 720.0f;

float ball_x = 0.0f;
float ball_y = 0.0f;

// Position and size of the bars
GLfloat barWidth = 12.0f;
GLfloat barHeight = 120.0f;
GLfloat bar1_x = 50.0f;
GLfloat bar1_y = 350.0f;
GLfloat bar2_x = windowWidth - barWidth - 50.0f;
GLfloat bar2_y = 350.0f;


float x1_ = 50.0f;
float y1_ = 120.0f;
float rsize = 28.0f;


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


int collided_with_bar(float x_ball, float y_ball) {

    if (y_ball >= bar1_y && y_ball <= (bar1_y + barHeight) && x_ball <= bar1_x + rsize + 5) {
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

     for (int i = 0; i < 360; i++)
     {
         float theta = i * 3.14159 / 180;
         float x = x1_ + rsize/2 * cos(theta);
         float y = y1_ + rsize/2 * sin(theta);

         ball_x = x;
         ball_y = y;
         glVertex2f(x, y);
     }

     glEnd();
}

void DrawBackground(void) {
    // Draw a circle
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

     glBegin(GL_QUADS);
               glVertex2i(20, 20);
               glVertex2i(50, 520);
               glVertex2i(1030, 520);

               glColor3f(0.0f, 0.0f, 1.0f);
               glVertex2i(1060, 20);

     glEnd();
}



void Desenha(void)
{

     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();

     glClear(GL_COLOR_BUFFER_BIT);

     glColor3f(1.0f, 0.0f, 0.0f);

     DrawBackground();
     DrawField();
     DrawBall();

     DrawBar1();
     DrawBar2();

     glutSwapBuffers();
}


void Timer(int value)
{

    GLfloat collision = 0.0f;

    if(x1_> windowWidth-rsize-15 || x1_< 40)
            xstep = -xstep;

    if(y1_ > windowHeight-rsize- (0.25*windowHeight) || y1_ < 30)
          ystep = -ystep;


    if(x1_> windowWidth-rsize)
         x1_= windowWidth-rsize-1;

    if(y1_ > windowHeight-rsize)
         y1_ = windowHeight-rsize-1;

    collision = collided_with_bar(ball_x, ball_y);

    if (collision == 1) {
        printf("left\n");
        xstep = -xstep;
    }

    else if (collision == 2) {
        printf("right\n");
        xstep = -xstep;
    }

    x1_ += 7*xstep;
    y1_ +=7*ystep;

    glutPostRedisplay();
    glutTimerFunc(1, Timer, 1);
}

void Inicializa (void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
