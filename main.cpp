// Anima.c -
// Um programa OpenGL simples que mostra a animação
// de quadrado em  uma janela GLUT.

#include <windows.h>
#include <gl/glut.h>
#include <math.h>
#include <stdio.h>

// Tamanho e posição inicial do quadrado


// Tamanho do incremento nas direções x e y
// (número de pixels para se mover a cada
// intervalo de tempo)
GLfloat xstep = 1.0f;
GLfloat ystep = 1.0f;

// Largura e altura da janela
GLfloat windowWidth;
GLfloat windowHeight;

float x1_ = 30.0f;
float y1_ = 120.0f;
float rsize = 32.0f;


void DrawBall(void) {

    printf(" (%.1f,  %.1f)", x1_, y1_);

    // Draw a circle
     glColor3f(0.0f, 1.0f, 0.0f);
     glBegin(GL_POLYGON);
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
    // Draw a circle
     glColor3f(1.0f, 0.0f, 0.0f);

     // Desenha um quadrado preenchido com a cor corrente
     glBegin(GL_QUADS);
               glVertex2i(0,0);
               glVertex2i(windowWidth,0);
               glVertex2i(windowWidth,windowHeight);

               // Especifica que a cor corrente é azul
               glColor3f(0.0f, 0.0f, 1.0f);
               glVertex2i(0, windowHeight);

     glEnd();
}

void DrawField(void) {
    // Draw a circle
     glColor3f(1.0f, 1.0f, 1.0f);

     // Desenha um quadrado preenchido com a cor corrente
     glBegin(GL_QUADS);
               glVertex2i(20, 20);
               glVertex2i(50, 520);
               glVertex2i(1030, 520);

               // Especifica que a cor corrente é azul
               glColor3f(0.0f, 0.0f, 1.0f);
               glVertex2i(1060, 20);

     glEnd();
}

// Função callback chamada para fazer o desenho
void Desenha(void)
{
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();

     // Limpa a janela de visualização com a cor de fundo especificada
     glClear(GL_COLOR_BUFFER_BIT);

     glColor3f(1.0f, 0.0f, 0.0f);

     DrawBackground();
     DrawField();
     DrawBall();

     // Executa os comandos OpenGL
     glutSwapBuffers();
}

// Função callback chamada pela GLUT a cada intervalo de tempo
// (a window não está sendo redimensionada ou movida)
void Timer(int value)
{
    // Muda a direção quando chega na borda esquerda ou direita
      if(x1_> windowWidth-rsize || x1_< 0)
            xstep = -xstep;

    // Muda a direção quando chega na borda superior ou inferior
    if(y1_ > windowHeight-rsize- (0.25*windowHeight) || y1_ < 0)
          ystep = -ystep;

    // Verifica as bordas.  Se a window for menor e o
    // quadrado sair do volume de visualização
   if(x1_> windowWidth-rsize)
         x1_= windowWidth-rsize-1;

   if(y1_ > windowHeight-rsize)
         y1_ = windowHeight-rsize-1;

    // Move o quadrado
    x1_+= xstep;
    y1_ += ystep;

    // Redesenha o quadrado com as novas coordenadas
    glutPostRedisplay();
    glutTimerFunc(5,Timer, 1);
}

// Inicializa parâmetros de rendering
void Inicializa (void)
{
    // Define a cor de fundo da janela de visualização como preta
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// Função callback chamada quando o tamanho da janela é alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
     // Evita a divisao por zero
     if(h == 0) h = 1;

     // Especifica as dimensões da Viewport
     glViewport(0, 0, w, h);

     // Inicializa o sistema de coordenadas
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();

     // Estabelece a janela de seleção (left, right, bottom, top)
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

// Programa Principal
int main(void)
{
     glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
     glutInitWindowSize(1080, 720);
     glutInitWindowPosition(0.0,0.0);
     glutCreateWindow("Anima");
     glutDisplayFunc(Desenha);
     glutReshapeFunc(AlteraTamanhoJanela);
     glutTimerFunc(33, Timer, 1);
     Inicializa();
     glutMainLoop();
}
