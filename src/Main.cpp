/////////////////////////////////////////////
// This source is licensed under MIT license
/////////////////////////////////////////////
#include <iostream> 
#include <vector> 
#include <string> 
#include <stdio.h>
#include <glew.h>
#include <wglew.h>
#include <GL/glut.h>// Header File For The GLUT Library 
using namespace std;
#include "glsl.h"
///////////////////////////////////////////
void DrawScene()
{
	glClearDepth(1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static bool init=true;
	static Shader quad_shader("Quad Shader");
	static Shader  tri_shader("Triangle Shader");

	static int VBOvert=0;
	const int num_floats=4*4;

	if(init)
	{
		/*+++++++++++++++++++++++++++++++++++++*/
		std::cout << "GL_VERSION: " << glGetString(GL_VERSION) << std::endl;			//std::cout << "GL_EXTENSIONS: " << glGetString(GL_EXTENSIONS) << std::endl;
		std::cout << "GL_RENDERER: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "GL_VENDOR: " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "GLU_VERSION: " << gluGetString(GLU_VERSION) << std::endl;			//std::cout << "GLU_EXTENSIONS: " << gluGetString(GLU_EXTENSIONS) << std::endl;
		std::cout << "GLUT_API_VERSION: " << GLUT_API_VERSION << std::endl;
		/*+++++++++++++++++++++++++++++++++++++*/
		quad_shader.attach(GL_VERTEX_SHADER,"../shader/tess_vs.txt");
		quad_shader.attach(GL_FRAGMENT_SHADER,"../shader/tess_frag.txt");
		quad_shader.attach(GL_GEOMETRY_SHADER,"../shader/tess_geo.txt");
		quad_shader.attach(GL_TESS_CONTROL_SHADER,"../shader/tess_quad_tcs.txt");
		quad_shader.attach(GL_TESS_EVALUATION_SHADER,"../shader/tess_quad_tes.txt");
		quad_shader.link();
		/*+++++++++++++++++++++++++++++++++++++*/
		tri_shader.attach(GL_VERTEX_SHADER,"../shader/tess_vs.txt");
		tri_shader.attach(GL_FRAGMENT_SHADER,"../shader/tess_frag.txt");
		tri_shader.attach(GL_GEOMETRY_SHADER,"../shader/tess_geo.txt");
		tri_shader.attach(GL_TESS_CONTROL_SHADER,"../shader/tess_tri_tcs.txt");
		tri_shader.attach(GL_TESS_EVALUATION_SHADER,"../shader/tess_tri_tes.txt");
		tri_shader.link();
		/*+++++++++++++++++++++++++++++++++++++*/
		float vert[num_floats] = {
			- 0.3 , -0.3 , 0 , 0.3,	// upleft
			  0.3 , -0.3 , 0 , 0.6,	// upright
			  0.3 ,  0.3 , 0 , 0.4,	// downright
			- 0.3 ,  0.3 , 0 , 0.1	// downleft
		};
		glGenBuffers(1, (GLuint *)(&VBOvert));
		glBindBuffer(GL_ARRAY_BUFFER, VBOvert);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_floats,vert, GL_DYNAMIC_DRAW_ARB );
		glPointSize(10);
		/*+++++++++++++++++++++++++++++++++++++*/
		init=false;
		/*+++++++++++++++++++++++++++++++++++++*/
	}
	glMatrixMode( GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode( GL_MODELVIEW);
	glLoadIdentity();

	float Projection[16];
	float Modelview[16];
	glGetFloatv(GL_PROJECTION_MATRIX, Projection);			CHECK_GL_ERROR();
	glTranslatef(-0.5,0,0);
	glGetFloatv(GL_MODELVIEW_MATRIX, Modelview);		

	// Enable VBO
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, VBOvert);			CHECK_GL_ERROR();
	glEnableClientState(GL_VERTEX_ARRAY);					CHECK_GL_ERROR();
	glVertexPointer  ( 4, GL_FLOAT,0, (char *) 0);			CHECK_GL_ERROR();

	// Quad Mesh	
	quad_shader.begin();
	quad_shader.setUniformMatrix4fv("projectionMatrix", 1, 0, Projection);	CHECK_GL_ERROR();
	quad_shader.setUniformMatrix4fv("modelViewMatrix", 1, 0, Modelview);	CHECK_GL_ERROR();
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawArrays( GL_PATCHES, 0, 4);										CHECK_GL_ERROR();
	quad_shader.end();

	glTranslatef(1,0,0);
	glGetFloatv(GL_MODELVIEW_MATRIX, Modelview);		

	// Triangle Mesh
	tri_shader.begin();
	tri_shader.setUniformMatrix4fv("projectionMatrix", 1, 0, Projection);	CHECK_GL_ERROR();
	tri_shader.setUniformMatrix4fv("modelViewMatrix", 1, 0, Modelview);	CHECK_GL_ERROR();
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glDrawArrays( GL_PATCHES, 0, 3);										CHECK_GL_ERROR();
	tri_shader.end();

	// Disable VBO
	glDisableClientState(GL_VERTEX_ARRAY);									CHECK_GL_ERROR();
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);								CHECK_GL_ERROR();

	glutSwapBuffers();
}
///////////////////////////////////////////
int main(int argc, char **argv) 
{ 
  glutInit(&argc, argv);  
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  
  glutInitWindowSize(800, 600);  
  glutInitWindowPosition(0, 0);  
  glutCreateWindow("Tesselation Example (c) Sven Forstmann 2011");
  glutDisplayFunc(DrawScene);
  glutIdleFunc(DrawScene);
  glewInit();
  wglSwapIntervalEXT(0);
  glutMainLoop();  
  return 0;
}
///////////////////////////////////////////
