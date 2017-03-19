#include "common.h"

#define MY_ENCODING "ISO-8859-1"

/*Global parameters. If equal to 1, answer was received.*/
int AnsRad=0, AnsObst=0;

/*dx, dy - global parameters, delta of robot's coordinates. transfered from coordinates from client xml-message*/
float dx=0, dy=0;

/*global parameters for RAD values*/
float rad_l, rad_f, rad_r;

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

void askXmlwriterFilename(const char *uri, char *ask)
{
  int rc;
  float test=1.0;
  xmlTextWriterPtr writer;
  writer = xmlNewTextWriterFilename(uri, 0);
  if (writer == NULL) {
      printf("Error creating the xml writer\n");
      return;
  }
  rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
  if (rc < 0) {
      printf("Error at xmlTextWriterStartDocument\n");
      return;
  }
    rc = xmlTextWriterStartElement(writer, BAD_CAST "DATA");
    if (rc < 0) {
        printf("Error at xmlTextWriterStartElement\n");
        return;
    } 
      rc = xmlTextWriterWriteElement(writer, BAD_CAST "TYPE", BAD_CAST "PROC");
      if (rc < 0) {
        printf("Error at xmlTextWriterWriteAttribute\n");
        return;
      }
      rc = xmlTextWriterWriteElement(writer, BAD_CAST "ASK", BAD_CAST ask);
      if (rc < 0) {
        printf("Error at xmlTextWriterWriteAttribute\n");
        return;
      }
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
      printf("Error at xmlTextWriterEndElement\n");
      return;
    }
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterEndDocument\n");
        return;
    }
    xmlFreeTextWriter(writer);
}

void ask(void *argument, char *ask, char *file) {
  int serv_sock = *(int *) argument;
  int xmlsize;
  FILE *xmlfd;
  LIBXML_TEST_VERSION
  askXmlwriterFilename(file, ask);
  xmlCleanupParser();
  xmlMemoryDump();
  xmlfd = fopen(file, "rb");
  fseek(xmlfd, 0L, SEEK_END);
  xmlsize = ftell(xmlfd);
  fseek(xmlfd, 0L, SEEK_SET);
  char *buf = calloc(xmlsize, sizeof(char));
  fread(buf,xmlsize,sizeof(char),xmlfd);
  write(serv_sock, buf, xmlsize);
  free(xmlfd); free(buf); 
}

void ansRad(char *xmlfile){
  xmlXPathContextPtr context;
  xmlXPathObjectPtr result;
  xmlChar *xpath1 = "/DATA/LEFT/text()";
  xmlChar *xpath2 = "/DATA/FRONT/text()";
  xmlChar *xpath3 = "/DATA/RIGHT/text()";
  xmlInitParser ();
  LIBXML_TEST_VERSION
  xmlDoc *doc = xmlParseFile (xmlfile);
  context = xmlXPathNewContext(doc);
  result = xmlXPathEvalExpression(xpath1, context);
  if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
    xmlXPathFreeObject(result);
    printf("No result for parsing xml package\n");
  }
  else {
    if(result->nodesetval[0].nodeTab[0]->content != NULL) {
      char *tmp = calloc(sizeof(result->nodesetval[0].nodeTab[0]->content),
			 sizeof(char));
      tmp = (char *) result->nodesetval[0].nodeTab[0]->content;
      rad_l = strtod(tmp, NULL);
    }
  }
  result = xmlXPathEvalExpression(xpath2, context);
  if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
    xmlXPathFreeObject(result);
    printf("No result for parsing xml package\n");
  }
  else {
    if(result->nodesetval[0].nodeTab[0]->content != NULL) {
      char *tmp = calloc(sizeof(result->nodesetval[0].nodeTab[0]->content),
			 sizeof(char));
      tmp = (char *) result->nodesetval[0].nodeTab[0]->content;
      rad_f = strtod(tmp, NULL);
    }
  }
result = xmlXPathEvalExpression(xpath3, context);
  if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
    xmlXPathFreeObject(result);
    printf("No result for parsing xml package\n");
  }
  else {
    if(result->nodesetval[0].nodeTab[0]->content != NULL) {
      char *tmp = calloc(sizeof(result->nodesetval[0].nodeTab[0]->content),
			 sizeof(char));
      tmp = (char *) result->nodesetval[0].nodeTab[0]->content;
      rad_r = strtod(tmp, NULL);
    }
  }
  AnsRad=1;
}

void parseAns(char *xmlfile) {
  xmlXPathContextPtr context;
  xmlXPathObjectPtr result;
  xmlChar *xpath1 = "/DATA/TYPE/text()";
  xmlInitParser ();
  LIBXML_TEST_VERSION
  xmlDoc *doc = xmlParseFile (xmlfile);
  context = xmlXPathNewContext(doc);
  result = xmlXPathEvalExpression(xpath1, context);
  if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
    xmlXPathFreeObject(result);
    printf("No result for parsing xml package\n");
  }
  else {
    if(result->nodesetval[0].nodeTab[0]->content != NULL) {
      char *tmp = calloc(sizeof(result->nodesetval[0].nodeTab[0]->content),
			 sizeof(char));
      tmp = (char *) result->nodesetval[0].nodeTab[0]->content;
      if(!strcmp(tmp,"RAD")) 
        ansRad(xmlfile);
    }
  }    
}

void readData(int serv_sock, int nread) {
  char *buf = calloc(nread, sizeof(char));
  read(serv_sock, buf, nread);
  int xmlfd;
  mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
  remove("readerP.xml");
  xmlfd = open("readerP.xml", O_WRONLY | O_CREAT, mode);
  write(xmlfd,buf,nread);
  close(xmlfd); free(buf);
    parseAns("readerP.xml");
}

void *hear(void *argument) {
  int serv_sock = *(int *) argument;
  int nread;
  while(1) {
    ioctl(serv_sock, FIONREAD, &nread);
      if(nread) {
       readData(serv_sock, nread);
    }
  }  
}

void *speak(void *argument) {
  ask(argument, "RAD", "askrad.xml");
  printf("Asking client_rad for data\n");
  while(!AnsRad) {}
  AnsRad=0;
  ask(argument, "OBST", "askobst.xml");
  printf("Asking client_obst for data\n");
  while(!AnsObst) {}
  AnsObst=0;
}

int conn_serv(int serv_sock) {
  int len,result;
  struct sockaddr_in s_address;
  s_address.sin_family = AF_INET;
  s_address.sin_addr.s_addr = inet_addr("127.0.0.1");
  s_address.sin_port = htons(DEFPORT);
  printf("Trying to connect with server\n");
  len = sizeof(s_address);
  result = connect(serv_sock, (struct sockaddr *)&s_address, len);
  if(result == -1) {
    perror("Connection with server failed");
    return 0;
  }
  return 1;
}

int main(int argc, char **argv)
{
  FILE *fmap;
  fmap = fopen("map", "r");
  int x,y;
  fscanf(fmap, "%d", &x);
  fscanf(fmap, "%d", &y);
  float map[x][y][2];
  for(int i=0; i<x; i++) {
    for(int j=0; j<y; j++){
      fscanf(fmap, "%f", &map[i][j][0]); /*obstacles*/
      fscanf(fmap, "%f", &map[i][j][1]); /*radiation*/
    }
  }

  int serv_sock;
  serv_sock = socket(AF_INET, SOCK_STREAM, 0);
  if(!conn_serv(serv_sock)) {
    printf("Can't connect with server\n");
    exit(EXIT_FAILURE);
  }
  printf("Successfully connected to server\n");
  pthread_t hear_t, speak_t;
  pthread_create(&hear_t, NULL, hear, &serv_sock);
  pthread_create(&speak_t, NULL, speak, &serv_sock);
  pthread_join(hear_t, NULL);
  pthread_join(speak_t,NULL);
  exit(EXIT_SUCCESS);

/*   glutInit( &argc, argv );
   glutInitDisplayMode( GLUT_DEPTH| GLUT_DOUBLE | GLUT_RGBA);
   glutInitWindowSize( 500, 500 );
   glutInitWindowPosition( 100, 100 );
   glutCreateWindow( "RAD DEMO" );
   glutTimerFunc(1, timer, 0);
   glutMainLoop();*/
}



