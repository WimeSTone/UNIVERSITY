#include <stdlib.h>
#include <gl/glut.h>

#define _USE_MATH_DEFINES // for m_pi
#include <cmath>

//next defines clear screen in colors according to their names:
#define clsWhite glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
#define clsULightGray glClearColor(0.85f, 0.85f, 0.85f, 1.0f);
#define clsBlack glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

#define ESCAPE '\033'

// lerp_speed[0.0f..1.0f] - defines the linear interpolation speed
// lower provides "smooth effect", 0.1 is the best looking one
#define lerp_speed 0.1
// almost smooth linear interpolation with fma (fused multiply-add)
// to boost calculations. Thanks to nVidia proTips.
// fma(x,y,z) <- (x*y) +z
inline float lerp(float v0, float v1, float t) {           
	return fma(t, v1, fma(-t, v0, v0));
}

// Client width and height
GLint Width = 1024, Height = 1024;
// dx/dy - our current shift
// destination x/y - our destination shift
GLfloat dx = 0, dy = 0,
		dest_x = 0, dest_y = 0;

void RenderScene(void)

{	
	// before anything is drawn, calculate our current position with lerp
	dx = lerp(dx, dest_x, lerp_speed);
	dy = lerp(dy, dest_y, lerp_speed);

	// initialize lines coordinates, as we need to provide immediates
	// to vertex functions
	int x1, x2, y1, y2;
	
	// clear screen in ultra light gray to not destroy our eyes with white
	clsULightGray;
	// clear buffers currently enabled for color writing
	// GL_COLOR_CLEAR_VALUE (r,g,b,a) is used to clear the buffers
	// with corresponding color
	glClear(GL_COLOR_BUFFER_BIT);

	// we use matrix transformations, thus, we save the one we have
	// in a stack
	glPushMatrix();

	// translate our coordinates to center of client plus shift
	glTranslatef((Width / 2) + dx, (Height / 2) + dy, 0);
	// this rotate is needed to finish the drawing
	// particularly to turn octagon from this position "\/" to this "\_/"/
	glRotatef(22.5, 0, 0, 1);
	
	// octagon drawing routine
	for (int i = 0; i < 8; i++) {
		
		// here we calculate line's points coordinates
		x1 = 200 * cos((i+1) * 45 * M_PI / 180);
		y1 = 200 * sin((i+1) * 45 * M_PI / 180);
		x2 = 200 * cos((i)* 45 * M_PI / 180);
		y2 = 200 * sin((i)* 45 * M_PI / 180);
		
		// set color to black
		glColor3ub(0, 0, 0);
		
		// draw N-1 lines, when N is number of vertices.
		// why not GL_LINES? I like strips more :D
		glBegin(GL_LINE_STRIP);		
			glVertex2f(x1, y1);
			glVertex2f(x2, y2);
		glEnd();			

	}
	
	// turn our matrix again to initial state
	glRotatef(-22.5, 0, 0, 1);

	// this is Sauron eye graphical artifact :D
	// polygon gradient is weird, must do some research

	/*glBegin(GL_POLYGON);
		glColor3ub(0, 0, 255);
		glVertex2f(0, 150);
		glColor3ub(255, 0, 255);
		glVertex2f(-50, 0);
		glColor3ub(255, 0, 0);
		glVertex2f(0, -150);
		glColor3ub(255, 0, 255);
		glVertex2f(50, 0);
	glEnd();*/

	// here we draw the inner part of our picture
	// two triangles with different vertices colors
	// colors interpolate automacally into a gradient
	glBegin(GL_TRIANGLES);
		glColor3ub(0, 0, 255);
		glVertex2f(0, 150);
		glColor3ub(255, 0, 255);
		glVertex2f(-50, 0);
		glVertex2f(50, 0);
		glVertex2f(-50, 0);
		glVertex2f(50, 0);
		glColor3ub(255, 0, 0);
		glVertex2f(0, -150);
	glEnd();

	// set color to black
	glColor3ub(0, 0, 0);

	// draw the contour with line loop
	// we specify four points and the last vertex
	// is connected with the first one
	glBegin(GL_LINE_LOOP);
		glVertex2f(0, 150);
		glVertex2f(-50, 0);
		glVertex2f(0, -150);
		glVertex2f(50, 0);
	glEnd();

	// restore the matrix from the stack
	glPopMatrix();

	// I'm using double buffering to reduce stuttering
	// and choppiness. This swaps the background buffer, which contains
	// our graphics, with the current visible buffer. After this action
	// the contents of background buffer become undefined and an implicit
	// call of glFlush() occurs, hence, we dont need to call any sort of
	// that function afterwards
	glutSwapBuffers();	

	// we call this to update screen
	glutPostRedisplay();
}


void Resize(GLint w, GLint h)

{

	// change client size to corresponding values
	Width = w;
	Height = h;

	// remap viewport coordinates
	glViewport(0, 0, w, h);
	
	// set matrix mode to projection and clear transform matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// update our parallel projection 
	// where  T - top, L- left, B - bottom, R - right
	// Z planes are clipping planes, z axis points to the viewer.
	//
	//	   Z near plane      ___       Z far plane             
	//	      /\  \         /\  \         /\  \                
	//	     /::\  \       /::\  \       /::\  \               
	//	    /:/\:\  \     /L/\:\TT\     /:/\:\  \              
	//	   /:/  \:\  \   /L/  \:\  \   /:/  \:\  \             
	//	  /:/__/ \:\__\ /:/__/ \:\__\ /:/__/ \:\__\ 
	//	  \:\  \ /:/  / \:\  \ /:/  / \:\  \ /:/  /            
	//	   \:\  /:/  /   \B\  /:/  /   \:\  /:/  /              
	//	    \:\/:/  /     \B\/:/RR/     \:\/:/  /                
	//	     \::/  /       \::/  /       \::/  /                  
	//	      \/__/         \/__/         \/__/   
	//	                 View plane
	//	
	glOrtho(0, w, 0, h, -1.0, 1.0);

	// change back to modelview and clear transform matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}


void KeyboardHandle(unsigned char key, int x, int y)

{
	// here we process the keyboard input
	switch (key)
	{
	default: 
		if (key == ESCAPE)
			exit(0);
		break;
	}

	// must redisplay after a keyboard event occurs
	glutPostRedisplay();
}


void initGL()
{
	// some general and primitive antialiasing
	// to make things look not ugly
	glLineWidth(2);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	   
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

void IdleHandle()
{
	// nothing drawn when idle, just redisplay
	glutPostRedisplay();
}

void SpecialKeysHandle(int key, int x, int y)
{
	// move our graphics with arrow keys
	// we can also track different special keys like F1,
	// modifier keys etc. here, yet ESC, Backspace and Delete are still
	// generated as ASCII characters
	switch (key)
	{
	case GLUT_KEY_UP:
		dest_y += 64;
		break;
	case GLUT_KEY_DOWN:
		dest_y -= 64;
		break;
	case GLUT_KEY_LEFT:
		dest_x -= 64;
		break;
	case GLUT_KEY_RIGHT:
		dest_x += 64;
		break;
	default:
		break;
	}

	// redisaplay after event occurs
	glutPostRedisplay();
}

void MouseHandle(int button, int state, int x, int y)
{
	// we track here mouse buttons, left, right and middle
	// there are two states GLUT_UP and GLUT_DOWN, which represent
	// mousebutton release and press states
	// in our case we set destination x/y of our graphics to
	// the coordinates of left mousebutton click
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		// this if is mandatory, otherwise we will proc this twice: 
		//both on up and down states
		if (state == GLUT_DOWN)
		{
			dest_x = -(Width / 2) + x;
			dest_y = (Height / 2) - y;
		}
		break;
	default:
		break;
	}
	
	// redisaplay after event occurs
	glutPostRedisplay();
}


void main(int argc, char *argv[])

{

	// pass command line arguments to glutInit
	// e.g. -sync for synchronous X protocol transactions
	// or -gldebug to track error messages with glGetError and other options
	glutInit(&argc, argv);

	// Set display mode to double buffering and RGBA color scheme
	// There are other bits, like GLUT_SINGLE for single buffering etc.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	// Initialize the client with corresponding size.
	glutInitWindowSize(Width, Height);
	// Create window with caption.
	glutCreateWindow("OpenGL+GLUT. Lab1. Leshyk A.V., DA-21");

	// Set main loop functions
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(Resize);
	glutKeyboardFunc(KeyboardHandle);
	glutIdleFunc(IdleHandle);
	glutSpecialFunc(SpecialKeysHandle);
	glutMouseFunc(MouseHandle);

	// Here I do some preparations, like enabling antialiasing.
	// Not really antialiasing with multisampling, nor per-fragment alpha blending + textures
	// simple GL_SMOOTH group
	initGL();

	// this starts the main GLUT routine, where all calls chain happens
	glutMainLoop();

}