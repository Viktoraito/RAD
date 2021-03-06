#include "common.h"

#define MY_ENCODING "ISO-8859-1"

float l_obst,f_obst,r_obst;
int procIsAsking;
int x,y;
float **map;

void sendXmlwriterFilename(const char *uri)
{
  FILE *fcoord;
  fcoord = fopen("coord", "r");
  int dx,dy;
  fscanf(fcoord, "%d", &dx);
  fscanf(fcoord, "%d", &dy);
  fclose(fcoord);
  dx--; dy--;
  if(dx>0)
    l_obst=map[dy][dx-1];
  else
    l_obst=1.0;
  if(dy>0)
    f_obst=map[dy-1][dx];
  else
    f_obst=1.0;
  if(dx<y-1)
    r_obst=map[dy][dx+1];
  else
    r_obst=1.0;

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
      rc = xmlTextWriterWriteElement(writer, BAD_CAST "TYPE", BAD_CAST "OBST");
      if (rc < 0) {
        printf("Error at xmlTextWriterWriteAttribute\n");
        return;
      }
      rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "LEFT", "%f", l_obst);
      if (rc < 0) {
        printf("Error at xmlTextWriterWriteAttribute\n");
        return;
      }
      rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "FRONT", "%f", f_obst);
      if (rc < 0) {
        printf("Error at xmlTextWriterWriteAttribute\n");
        return;
      }
      rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "RIGHT", "%f", r_obst);
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

void parseAsk(char *xmlfile) {
  xmlXPathContextPtr context;
  xmlXPathObjectPtr result;
  xmlChar *xpath1 = "/DATA/ASK/text()";
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
      if(!strcmp(tmp,"OBST")) 
        procIsAsking=1;
    }
  }    
}

void readData(int serv_sock, int nread) {
  char *buf = calloc(nread, sizeof(char));
  read(serv_sock, buf, nread);
  int xmlfd;
  mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
  remove("readerO.xml");
  xmlfd = open("readerO.xml", O_WRONLY | O_CREAT, mode);
  write(xmlfd,buf,nread);
  close(xmlfd); free(buf);
    parseAsk("readerO.xml");
}

void *speak(void *argument) {
  while(1) {
    if(procIsAsking){
      sendData(argument, "obstData.xml");
      printf("Sending client_proc a data\n");
      procIsAsking=0;
    }
  }
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
  sendData(&serv_sock, "obstData.xml");
  return 1;
}

int main(int argc, char **argv)
{
  FILE *fmap;
  fmap = fopen("map", "r");
  fscanf(fmap, "%d", &x);
  fscanf(fmap, "%d", &y);
  map = (float**)malloc(x * sizeof(float*));
  float tmp[1];
  for(int i=0; i<x; i++) {
    map[i] = (float*)malloc(y * sizeof(float));
    for(int j=0; j<y; j++){
      fscanf(fmap, "%f", &map[i][j]); /*obstacles*/
      fscanf(fmap, "%f", &tmp[0]); /*radiation*/
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
  pthread_t hear_t, speak_t;
  pthread_create(&hear_t, NULL, hear, &serv_sock);
  pthread_create(&speak_t, NULL, speak, &serv_sock);
  pthread_join(hear_t, NULL);
  pthread_join(speak_t,NULL);
  for (int i=0; i<x; i++)
    free(map[i]);
  free(map);
  exit(EXIT_SUCCESS);
}



