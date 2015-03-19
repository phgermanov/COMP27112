/*
==========================================================================
File:        ex2.c (skeleton)
Authors:     Toby Howard
==========================================================================
*/

/* The following ratios are not to scale: */
/* Moon orbit : planet orbit */
/* Orbit radius : body radius */
/* Sun radius : planet radius */

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BODIES 20
#define TOP_VIEW 1
#define ECLIPTIC_VIEW 2
#define SHIP_VIEW 3
#define EARTH_VIEW 4
#define PI 3.14159
#define DEG_TO_RAD 0.017453293
#define ORBIT_POLY_SIDES 40
#define TIME_STEP 0.05   /* days per frame */
#define SOLAR_SYSTEM_SIZE 1e12
#define NUMBER_OF_STARTS 1000
#define RESCALE_FACTOR 1e-3

typedef struct { 
  char    name[20];       /* name */
  GLfloat r, g, b;        /* colour */
  GLfloat orbital_radius; /* distance to parent body (km) */
  GLfloat orbital_tilt;   /* angle of orbit wrt ecliptic (deg) */
  GLfloat orbital_period; /* time taken to orbit (days) */
  GLfloat radius;         /* radius of body (km) */
  GLfloat axis_tilt;      /* tilt of axis wrt body's orbital plane (deg) */
  GLfloat rot_period;     /* body's period of rotation (days) */
  GLint   orbits_body;    /* identifier of parent body */
  GLfloat spin;           /* current spin value (deg) */
  GLfloat orbit;          /* current orbit value (deg) */
 } body;

body  bodies[MAX_BODIES];
int   numBodies, current_view, draw_labels, draw_orbits, draw_starfield;

float date;

/*****************************/

float myRand (void)
{
  /* return a random float in the range [0,1] */

  return (float) rand() / RAND_MAX;
}

/*****************************/

void drawStarfield (void) /*                                  GYZ                        */
{
  GLfloat x, y, z;
  glColor3f(1, 1, 1);
  glBegin(GL_POINTS);
  int i;
  for(i = 0; i < NUMBER_OF_STARTS; i++)
  {
    x = SOLAR_SYSTEM_SIZE*(2*myRand()-1);
    y = SOLAR_SYSTEM_SIZE*(2*myRand()-1);
    z = SOLAR_SYSTEM_SIZE*(2*myRand()-1);
    glVertex3f(x*RESCALE_FACTOR, y*RESCALE_FACTOR, z*RESCALE_FACTOR);
  }
  glEnd();
}

/*****************************/

void readSystem(void)
{
  /* reads in the description of the solar system */

  FILE *f;
  int i;

  f= fopen("sys", "r");
  if (f == NULL) {
     printf("ex2.c: Couldn't open the datafile 'sys'\n");
     printf("To get this file, use the following command:\n");
     printf("  cp /opt/info/courses/COMP27112/ex2/sys .\n");
     exit(0);
  }
  fscanf(f, "%d", &numBodies);
  for (i= 0; i < numBodies; i++)
  {
    fscanf(f, "%s %f %f %f %f %f %f %f %f %f %d", 
      bodies[i].name,
      &bodies[i].r, &bodies[i].g, &bodies[i].b,
      &bodies[i].orbital_radius,
      &bodies[i].orbital_tilt,
      &bodies[i].orbital_period,
      &bodies[i].radius,
      &bodies[i].axis_tilt,
      &bodies[i].rot_period,
      &bodies[i].orbits_body);

    /* Initialise the body's state */
    bodies[i].spin= 0.0;
    bodies[i].orbit= myRand() * 360.0; /* Start each bodys orbit at a
                                          random angle */
    bodies[i].radius*= 1000.0; /* Magnify the radii to make them visible */
  }
  fclose(f);
}

/*****************************/

void drawString (void *font, float x, float y, char *str) /*         GYZ                 */
{ /* Displays the string "str" at (x,y,0), using font "font" */

  /* This is for you to complete. */

}

/*****************************/

void showDate(void)
{ /* Displays the current date */
  char *str = malloc(sizeof(char)*100);
  void *font = GLUT_BITMAP_HELVETICA_18;
  sprintf(str, "Day: %.0f", date);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0.0, 1.0, 0.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glRasterPos3f(0.75, 0.02, 0.0);
  char *ch = malloc(sizeof(char)*100);
  for(*ch = *str; *ch; ch++)
    glutBitmapCharacter(font, (int)*ch);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

/*****************************/

void setView (void) { /*                                                    GYZ               */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  switch (current_view) {
  case TOP_VIEW:
    gluLookAt(0, 0, 0.5e6,
              0, 0, 0,
              0, 0.1, 1);
    /* This is for you to complete. */
    break;  
  case ECLIPTIC_VIEW:
    gluLookAt(0, 0.5e6, 0,
              0, 0, 0,
              0, 1, 0.1);
    /* This is for you to complete. */
    break;  
  case SHIP_VIEW:
    gluLookAt(0, 0.5e6, 0.5e6,
              0, 0, 0,
              0, -1.0, 0);
    break;  
  case EARTH_VIEW:
    gluLookAt(bodies[3].orbital_radius*cos(bodies[3].orbit*DEG_TO_RAD)*RESCALE_FACTOR, bodies[3].orbital_radius*sin(bodies[3].orbit*DEG_TO_RAD)*RESCALE_FACTOR, 0,
              0, 0, 0,
              0, 0, 1);
    /* This is for you to complete. */
    break;  
  }
}

/*****************************/

void menu (int menuentry) {
  switch (menuentry) {
  case 1: current_view= TOP_VIEW;
          break;
  case 2: current_view= ECLIPTIC_VIEW;
          break;
  case 3: current_view= SHIP_VIEW;
          break;
  case 4: current_view= EARTH_VIEW;
          break;
  case 5: draw_labels= !draw_labels;
          break;
  case 6: draw_orbits= !draw_orbits;
          break;
  case 7: draw_starfield= !draw_starfield;
          break;
  case 8: exit(0);
  }
}

/*****************************/

void init(void)
{
  /* Define background colour */
  glClearColor(0.0, 0.0, 0.0, 0.0);

  glutCreateMenu (menu);
  glutAddMenuEntry ("Top view", 1);
  glutAddMenuEntry ("Ecliptic view", 2);
  glutAddMenuEntry ("Spaceship view", 3);
  glutAddMenuEntry ("Earth view", 4);
  glutAddMenuEntry ("", 999);
  glutAddMenuEntry ("Toggle labels", 5);
  glutAddMenuEntry ("Toggle orbits", 6);
  glutAddMenuEntry ("Toggle starfield", 7);
  glutAddMenuEntry ("", 999);
  glutAddMenuEntry ("Quit", 8);
  glutAttachMenu (GLUT_RIGHT_BUTTON);

  current_view= TOP_VIEW;
  draw_labels= 1;
  draw_orbits= 1;
  draw_starfield= 1;
}

/*****************************/

void animate(void)
{
  int i;

    date+= TIME_STEP;

    for (i= 0; i < numBodies; i++)  {
      bodies[i].spin += 360.0 * TIME_STEP / bodies[i].rot_period;
      bodies[i].orbit += 360.0 * TIME_STEP / bodies[i].orbital_period;
      glutPostRedisplay();
    }
}

/*****************************/

void drawOrbit (int n) /*                                         GYZ                                   */
{ /* Draws a polygon to approximate the circular
     orbit of body "n" */
  glBegin(GL_LINE_LOOP);
  GLfloat angle = 0.0; // in radians
  int i;
  for(i=0;i<ORBIT_POLY_SIDES;i++)
  {
    glVertex3f(bodies[n].orbital_radius*RESCALE_FACTOR*cos(angle), bodies[n].orbital_radius*RESCALE_FACTOR*sin(angle), 0);
    angle += 2*PI/ORBIT_POLY_SIDES;
  }
  glEnd();
}

/*****************************/

void drawLabel(int n) /*                                         GYZ                                   */
{ /* Draws the name of body "n" */
  char *font = GLUT_BITMAP_HELVETICA_18;
  glRasterPos3f(0.75, 0.02, 0.0);
  char *ch = malloc(sizeof(char)*100);
  for(*ch= *bodies[n].name; *ch; ch++) 
    glutBitmapCharacter(font, (int)*ch);
    /* This is for you to complete. */
}

/*********************/

void drawBody(int n)  /*                                         GYZ                                   */
{
  glRotatef(bodies[bodies[n].orbits_body].orbital_tilt, 0, 1, 0);
  glTranslatef(bodies[bodies[n].orbits_body].orbital_radius*cos(bodies[bodies[n].orbits_body].orbit * DEG_TO_RAD)*RESCALE_FACTOR, bodies[bodies[n].orbits_body].orbital_radius*sin(bodies[bodies[n].orbits_body].orbit * DEG_TO_RAD)*RESCALE_FACTOR, 0);
  glRotatef(bodies[n].orbital_tilt, 0, 1, 0);
  glColor3f(bodies[n].r, bodies[n].g, bodies[n].b);
  if(draw_orbits)
    drawOrbit(n);
  glTranslatef(bodies[n].orbital_radius*cos(bodies[n].orbit* DEG_TO_RAD)*RESCALE_FACTOR, bodies[n].orbital_radius*sin(bodies[n].orbit* DEG_TO_RAD)*RESCALE_FACTOR, 0);
  if(draw_labels)
  {
    glTranslatef(bodies[n].radius * RESCALE_FACTOR, bodies[n].radius * RESCALE_FACTOR, bodies[n].radius * RESCALE_FACTOR);
    drawLabel(n);
    glTranslatef(0-bodies[n].radius * RESCALE_FACTOR, 0-bodies[n].radius * RESCALE_FACTOR, 0-bodies[n].radius * RESCALE_FACTOR);
  }
  glRotatef(bodies[n].axis_tilt, 0, 1, 0);
  glRotatef(bodies[n].spin, 0, 0, 1);
  glutWireSphere(bodies[n].radius * RESCALE_FACTOR, 20, 20);
    /* This is for you to complete. */
}

/*****************************/

void display(void)
{
  int i;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  showDate();

  /* set the camera */
  setView();

  if (draw_starfield)
    drawStarfield();

  for (i= 0; i < numBodies; i++)
  {
    glPushMatrix();
      drawBody (i);
    glPopMatrix();
  }
  glutSwapBuffers();
}

/*****************************/

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective (48.0, (GLfloat) w/(GLfloat) h, 10000.0, 800000000.0);
}
  
/*****************************/

void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
    case 27:  /* Escape key */
      exit(0);
  }
} 

/*****************************/

int main(int argc, char** argv)
{
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow ("COMP27112 Exercise 2");
   glEnable(GL_DEPTH_TEST);
  glutFullScreen();
  init ();
  glutDisplayFunc (display); 
  glutReshapeFunc (reshape);
  glutKeyboardFunc (keyboard);
  glutIdleFunc (animate);
  readSystem();
  glutMainLoop ();
  return 0;
}
/* end of ex2.c */

