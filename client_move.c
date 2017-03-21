#include "common.h"

#define MY_ENCODING "ISO-8859-1"

int procIsAsking;
int x,y;
float **map_o; float **map_r;

void testXmlwriterFilename(const char *uri);

void Draw() {
  int dx, dy;
  FILE *fcoord;
  fcoord = fopen("coord", "r");
  fscanf(fcoord, "%d", &dx);
  fscanf(fcoord, "%d", &dy);
  fclose(fcoord);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /*clear screen*/
  glLoadIdentity(); /*move (0,0) to center of screen*/
  glRotatef(180.0, 1.0, 0.0, 0.0); /*rotate screen over X axis*/
  glTranslatef(-1.0f,-1.0f,0.0f); /*move (0,0) to left upper corner*/

  float dtx=2.0/x, dty=2.0/y;

	glBegin(GL_QUADS);
		glVertex3f(dx*dtx-dtx,dy*dty-dty,0.01f); //upper left
		glVertex3f(dx*dtx,    dy*dty-dty,0.01f); //upper right
		glVertex3f(dx*dtx,    dy*dty,    0.01f); //bottom right
		glVertex3f(dx*dtx-dtx,dy*dty,    0.01f); //bottom left
	glEnd();
	glutSwapBuffers();
}

void timer (int t) {
   glutDisplayFunc(Draw);
   glutPostRedisplay();
   glutTimerFunc(100, timer, 0); 	
}

void sendXmlwriterFilename(const char *uri)
{
  int rc;
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
      rc = xmlTextWriterWriteElement(writer, BAD_CAST "TYPE", BAD_CAST "MOVE");
      if (rc < 0) {
        printf("Error at xmlTextWriterWriteAttribute\n");
        return;
      }
      rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "ANS", BAD_CAST "OK");
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

void sendData(void *argument, char *file){
  int serv_sock = *(int *) argument;
  int result, xmlsize;
  FILE *xmlfd;
  LIBXML_TEST_VERSION
  sendXmlwriterFilename(file);
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

void move(int dec) {
  int dx, dy;
  FILE *fcoord;
  fcoord = fopen("coord", "r");
  fscanf(fcoord, "%d", &dx);
  fscanf(fcoord, "%d", &dy);
  fclose(fcoord);
  fcoord = fopen("coord", "w");
  switch(dec) {
    case L_STEP:
      dx--;
      break;
    case F_STEP:
      dy--;
      break;
    case R_STEP:
      dx++;
      break;
  }
  fprintf(fcoord,"%d %d",dx,dy);
  fclose(fcoord);
}

void parseAsk(char *xmlfile) {
  int dec=NIHIL;
  xmlXPathContextPtr context;
  xmlXPathObjectPtr result;
  xmlChar *xpath1 = "/DATA/DEC/text()";
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
      dec = strtod(tmp, NULL);
    }
  }
  if(dec!=NIHIL) {
    move(dec);
    sleep(5);
    procIsAsking=1;
  }
}

void readData(int serv_sock, int nread) {
  char *buf = calloc(nread, sizeof(char));
  read(serv_sock, buf, nread);
  int xmlfd;
  mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
  remove("readerM.xml");
  xmlfd = open("readerM.xml", O_WRONLY | O_CREAT, mode);
  write(xmlfd,buf,nread);
  close(xmlfd); free(buf);
    parseAsk("readerM.xml");
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
  while(1) {
    if(procIsAsking){
      sendData(argument, "moveData.xml");
      printf("Sending client_proc a data\n");
      procIsAsking=0;
    }
  }
}

void *paint(void *argument) {
   glutInitDisplayMode(GLUT_DEPTH| GLUT_DOUBLE | GLUT_RGBA);
   glutInitWindowSize(100*x, 100*y);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("RAD DEMO");
   glutTimerFunc(1, timer, 0);
   glutMainLoop();
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
  sendData(&serv_sock, "moveData.xml");
  return 1;
}

int main(int argc, char **argv) {
  FILE *fmap;
  fmap = fopen("map", "r");
  fscanf(fmap, "%d", &x);
  fscanf(fmap, "%d", &y);
  glutInit(&argc, argv);
  map_o = (float**)malloc(x * sizeof(float*));
  map_r = (float**)malloc(x * sizeof(float*));
  for(int i=0; i<x; i++) {
    map_o[i] = (float*)malloc(y * sizeof(float));
    map_r[i] = (float*)malloc(y * sizeof(float));
    for(int j=0; j<y; j++){
      fscanf(fmap, "%f", &map_o[i][j]); /*obstacles*/
      fscanf(fmap, "%f", &map_r[i][j]); /*radiation*/
    }
  }
  fclose(fmap);

  procIsAsking=0;
  int serv_sock;
  serv_sock = socket(AF_INET, SOCK_STREAM, 0);
  if(!conn_serv(serv_sock)) {
    printf("Can't connect with server\n");
    exit(EXIT_FAILURE);
  }
  printf("Successfully connected to server\n");
  pthread_t hear_t, speak_t, paint_t;
  pthread_create(&hear_t, NULL, hear, &serv_sock);
  pthread_create(&speak_t, NULL, speak, &serv_sock);
  pthread_create(&paint_t, NULL, paint, NULL);
  pthread_join(hear_t, NULL);
  pthread_join(speak_t,NULL);
  pthread_join(paint_t, NULL);

  for (int i=0; i<x; i++) {
    free(map_o[i]);
    free(map_r[i]);
  }
  free(map_o); free(map_r);

  exit(EXIT_SUCCESS);
}

