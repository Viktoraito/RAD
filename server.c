#include <GL/gl.h>

#include <GL/glut.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <sys/queue.h>

#define DEFPORT 7777
#define QUEUE 3

/*dx, dy - global parameters, delta of robot's coordinates. transfered from coordinates from client xml-message*/
float dx=0, dy=0;

void Draw() {
	float x=-0.5, y=-0.5;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_QUADS);
		glVertex3f(0.05-dx,-0.05,0.0);
		glVertex3f(0.05-dx,0.05,0.0);
		glVertex3f(-0.05-dx,0.05,0.0);
		glVertex3f(-0.05-dx,-0.05,0.0);
	glEnd();
/*	switch (c) {
	case 'a':
		glTranslatef(-0.05,0,0);
		c='\0';
		break;
	case 'w':
		glTranslatef(0,0.05,0);
		c='\0';
		break;
	case 's':
		glTranslatef(0,-0.05,0);
		c='\0';
		break;
	case 'd':
		glTranslatef(0.05,0,0);
		c='\0';
		break;
	default:
		break;
	}*/
	glBegin(GL_QUADS);
		glVertex3f(0.05+x+dx,-0.05+y,0.0);
		glVertex3f(0.05+x+dx,0.05+y,0.0);
		glVertex3f(-0.05+x+dx,0.05+y,0.0);
		glVertex3f(-0.05+x+dx,-0.05+y,0.0);
	glEnd();
	glutSwapBuffers();
}

void timer (int t) {
   dx+=0.01;
   glutDisplayFunc( Draw );
   glutPostRedisplay();
   glutTimerFunc(100, timer, 0); 	
}

int main( int argc, char **argv) {
  unsigned port=DEFPORT;
  int s_sockfd;
  struct sockaddr_in s_address;
  s_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  s_address.sin_family = AF_INET;
  s_address.sin_addr.s_addr = htonl(INADDR_ANY);
  s_address.sin_port = htons(port);

  if(bind(s_sockfd, (struct sockaddr *)&s_address, sizeof(s_address))) {
    printf("Can't bind socket\n");
    return EXIT_FAILURE;
  }
  else printf("SUCCESS\n");

/*   glutInit( &argc, argv );
   glutInitDisplayMode( GLUT_DEPTH| GLUT_DOUBLE | GLUT_RGBA);
   glutInitWindowSize( 500, 500 );
   glutInitWindowPosition( 100, 100 );
   glutCreateWindow( "RAD DEMO" );
   glutTimerFunc(1, timer, 0);
   glutMainLoop();*/
}
