/* mouse1.c 
Laura Toma

Example using the mouse in OpenGL.  First the mouse is registered via
a callback function. Once registered, this function will be called on
any mouse event in the window.  The user can use this function to
respond to mouse events. This code will print the coordinates of the
point where the mouse is clicked, and will draw a small blue disk at
the point where the mouse is pressed.

*/
#include "geom.h" 

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#include <vector> 

using namespace std; 



GLfloat red[3] = {1.0, 0.0, 0.0};
GLfloat green[3] = {0.0, 1.0, 0.0};
GLfloat blue[3] = {0.0, 0.0, 1.0};
GLfloat black[3] = {0.0, 0.0, 0.0};
GLfloat white[3] = {1.0, 1.0, 1.0};
GLfloat gray[3] = {0.5, 0.5, 0.5};
GLfloat yellow[3] = {1.0, 1.0, 0.0};
GLfloat magenta[3] = {1.0, 0.0, 1.0};
GLfloat cyan[3] = {0.0, 1.0, 1.0};



/* global variables */
const int WINDOWSIZE = 750; 

//the current polygon 
vector<point2d>  poly;

//coordinates of the last mouse click
double mouse_x=-10, mouse_y=-10;  //initialized to a point outside the window







/* forward declarations of functions */
void display(void);
void keypress(unsigned char key, int x, int y);
void mousepress(int button, int state, int x, int y);
void timerfunc(); 

void initialize_polygon(); 
void print_polygon(vector<point2d>& poly); 






/* ****************************** */
int main(int argc, char** argv) {

  initialize_polygon();
  print_polygon(poly);


  /* initialize GLUT  */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(WINDOWSIZE, WINDOWSIZE);
  glutInitWindowPosition(100,100);
  glutCreateWindow(argv[0]);

  /* register callback functions */
  glutDisplayFunc(display); 
  glutKeyboardFunc(keypress);
  glutMouseFunc(mousepress); 
  //glutIdleFunc(timerfunc); //register this if you want it called aat every fraame

  /* init GL */
  /* set background color black*/
  glClearColor(0, 0, 0, 0);   
  
  /* give control to event handler */
  glutMainLoop();
  return 0;
}




/* ****************************** */
/* initialize  polygon stored in global variable poly  */
//Note: use our local coordinate system (0,0) to (WINSIZE,WINSIZE),
//with the origin in the lower left corner.
void initialize_polygon() {
  
  //clear the vector, in case something was there 
  poly.clear(); 

  double rad = 100; 
  point2d p; 
  for (double a=0; a<2*M_PI; a+=.5) {
    p.x = WINDOWSIZE/2 + rad * cos (a); 
    p.y = WINDOWSIZE/2 + rad * sin (a); 
    poly.push_back (p); 
  } 
}


/* ****************************** */
/* draw the polygon */
void draw_polygon(vector<point2d>& poly){
   if (poly.size() == 0) return; 

  //set color  and polygon mode 
  glColor3fv(yellow);   
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
 
  int i;
  for (i=0; i<poly.size()-1; i++) {
    //draw a line from point i to i+1
    glBegin(GL_LINES);
    glVertex2f(poly[i].x, poly[i].y); 
    glVertex2f(poly[i+1].x, poly[i+1].y);
    glEnd();
  }
  //draw a line from the last point to the first  
  int last=poly.size()-1; 
    glBegin(GL_LINES);
    glVertex2f(poly[last].x, poly[last].y); 
    glVertex2f(poly[0].x, poly[0].y);
    glEnd();
}





/* our coordinate system is (0,0) x (WINDOWSIZE,WINDOWSIZE) with the
   origin in the lower left corner 
*/
//draw a circle with center = (x,y)
void draw_circle(double x, double y){

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glColor3fv(blue);   

  double r = 20;
  glBegin(GL_POLYGON);
  for(double theta = 0; theta < 2*M_PI; theta+=.3){
   glVertex2f(x + r*cos(theta), y + r*sin(theta));
  }
  glEnd();
}



/* ******************************** */
void print_polygon(vector<point2d>& poly) {
  printf("polygon:"); 
  for (unsigned int i=0; i<poly.size()-1; i++) {
    printf("\tedge %d: [(%d,%d), (%d,%d)]\n",
	   i, poly[i].x, poly[i].y, poly[i+1].x, poly[i+1].y);
  }
  //print last edge from last point to first point 
  int last = poly.size()-1; 
    printf("\tedge %d: [(%d,%d), (%d,%d)]\n",
	   last, poly[last].x, poly[last].y, poly[0].x, poly[0].y);

}


/* ****************************** */
/* This is the main render function. We registered this function to be
   called by GL to render the window. 
 */
void display(void) {

  glClear(GL_COLOR_BUFFER_BIT);
  //clear all modeling transformations 
  glMatrixMode(GL_MODELVIEW); 
  glLoadIdentity();


  /* The default GL window is [-1,1]x[-1,1] with the origin in the
     center.  The camera is at (0,0,0) looking down negative
     z-axis.  

     The points are in the range (0,0) to (WINSIZE,WINSIZE), so they
     need to be mapped to [-1,1]x [-1,1] */
  
  //First we scale down to [0,2] x [0,2] */ 
  glScalef(2.0/WINDOWSIZE, 2.0/WINDOWSIZE, 1.0);  
  /* Then we translate so the local origin goes in the middle of teh
     window to (-WINDOWSIZE/2, -WINDOWSIZE/2) */
  glTranslatef(-WINDOWSIZE/2, -WINDOWSIZE/2, 0); 
  
  //now we draw in our local coordinate system (0,0) to
  //(WINSIZE,WINSIZE), with the origin in the lower left corner.

  draw_polygon(poly); 

  //draw a circle where the mouse was last clicked. Note that this
  //point is stored as a global variable and is modified by the mouse handler function 
  draw_circle(mouse_x, mouse_y); 

  /* execute the drawing commands */
  glFlush();
}



/* ****************************** */
void keypress(unsigned char key, int x, int y) {
  switch(key) {
  case 'q':	
    exit(0);
    break;

  }
}


/* 
void glutMouseFunc(void (*func)(int button, int state, int x, int y));

glutMouseFunc sets the mouse callback for the current window. When a
user presses and releases mouse buttons in the window, each press and
each release generates a mouse callback. The button parameter is one
of GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON. For
systems with only two mouse buttons, it may not be possible to
generate GLUT_MIDDLE_BUTTON callback. For systems with a single mouse
button, it may be possible to generate only a GLUT_LEFT_BUTTON
callback. The state parameter is either GLUT_UP or GLUT_DOWN
indicating whether the callback was due to a release or press
respectively. The x and y callback parameters indicate the window
relative coordinates when the mouse button state changed. If a
GLUT_DOWN callback for a specific button is triggered, the program can
assume a GLUT_UP callback for the same button will be generated
(assuming the window still has a mouse callback registered) when the
mouse button is released even if the mouse has moved outside the
window.
*/
void mousepress(int button, int state, int x, int y) {

  if(state == GLUT_DOWN) { //mouse click detected 

    //(x,y) are in window coordinates, where the origin is in the upper
    //left corner; our reference system has the origin in lower left
    //corner, this means we have to reflect y
    mouse_x = (double)x;
    mouse_y = (double)(WINDOWSIZE - y); 
    printf("mouse pressed at (%.1f,%.1f)\n", mouse_x, mouse_y); 
  }
  
  glutPostRedisplay();
}





//this function is called every frame. Use for animations 
void timerfunc() {
  
  printf("timerfunc: \n"); 

  //if you want the window to be re-drawn, call this 
  // glutPostRedisplay(); 

}




/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
     
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   glMatrixMode(GL_PROJECTION);  // operate on the Projection matrix
   glLoadIdentity();             // reset
   gluOrtho2D(0.0, (GLdouble) width, 0.0, (GLdouble) height); 
}


