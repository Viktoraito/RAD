#include "common.h"

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

int listenSocket(int s_sockfd, struct sockaddr_in s_address, int queue) {
  int result, fd, nread;
  fd_set readfds, testfds;

  listen(s_sockfd, queue);
  FD_ZERO(&readfds);
  FD_SET(s_sockfd, &readfds);

  while(1){
    testfds = readfds;
    result = select(FD_SETSIZE, &testfds, (fd_set *)0, (fd_set *)0, (struct timeval *)0);
    if (result <1) {
      perror("Server");
      exit(EXIT_FAILURE);
    }
    
    for(fd=0; fd<FD_SETSIZE; fd++) {
      if(!FD_ISSET(fd,&testfds))
	continue;
      if(fd == s_sockfd) {
	addClient(s_sockfd, &readfds);
        continue;
      }
//      ioctl(fd, FIONREAD, &nread);
//      if(nread)
//	retriveData(fd, nread);
//      else
//	removeclient(fd,&readfds);
    }
  }
}

int addClient(int s_sockfd, fd_set *readfds) {
  int c_len, c_sockfd;
  struct sockaddr_in c_address;
  c_len = sizeof(c_address);
  c_sockfd = accept(s_sockfd, (struct sockaddr *)&c_address, &c_len);
  FD_SET(c_sockfd, readfds);
  printf("Adding client...\n");
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
  listenSocket(s_sockfd, s_address, QUEUE);

/*   glutInit( &argc, argv );
   glutInitDisplayMode( GLUT_DEPTH| GLUT_DOUBLE | GLUT_RGBA);
   glutInitWindowSize( 500, 500 );
   glutInitWindowPosition( 100, 100 );
   glutCreateWindow( "RAD DEMO" );
   glutTimerFunc(1, timer, 0);
   glutMainLoop();*/
}
