#pragma region Includes
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "src/utils.hpp"
#include "src/game.hpp"
#pragma endregion

static Game game;
static int lastTime = 0;

void setup()
{
   glEnable(GL_MULTISAMPLE);
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_LINE_SMOOTH);
   glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
   glLineWidth(Config::UI::LINE_WIDTH);
}

void timerCallback(int value)
{
   int currTime = glutGet(GLUT_ELAPSED_TIME);
   float dt = float(currTime - lastTime) / 1000.0f;
   lastTime = currTime;

   if (dt > 0.1f)
      dt = 0.1f;

   game.update(dt);
   glutPostRedisplay();
   glutTimerFunc(16, timerCallback, 0);
}

void display()
{
   game.render();
}

void reshape(int width, int height)
{
   if (height == 0)
      height = 1;

   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0, Config::Window::WIDTH, 0, Config::Window::HEIGHT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void keyDownCallback(unsigned char key, int x, int y) { game.handleKeyDown(key, x, y); }
void keyUpCallback(unsigned char key, int x, int y) { game.handleKeyUp(key, x, y); }
void arrowKeyDownCallback(int key, int x, int y) { game.handleArrowKeyDown(key, x, y); }
void arrowKeyUpCallback(int key, int x, int y) { game.handleArrowKeyUp(key, x, y); }
void mouseMotionCallback(int x, int y) { game.handleMouseMotion(x, y); }

int main(int argc, char **argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
   glutInitWindowSize(Config::Window::WIDTH, Config::Window::HEIGHT);

   int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
   int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
   int windowX = (screenWidth - Config::Window::WIDTH) / 2;
   int windowY = (screenHeight - Config::Window::HEIGHT) / 2;
   glutInitWindowPosition(windowX, windowY);

   glutCreateWindow("Catch The Eggs");

   setup();
   game.loadAssets();
   game.init();

   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyDownCallback);
   glutKeyboardUpFunc(keyUpCallback);
   glutSpecialFunc(arrowKeyDownCallback);
   glutSpecialUpFunc(arrowKeyUpCallback);
   glutPassiveMotionFunc(mouseMotionCallback);

   lastTime = glutGet(GLUT_ELAPSED_TIME);
   glutTimerFunc(16, timerCallback, 0);

   glutMainLoop();

   return 0;
}