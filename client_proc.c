#include "common.h"

#define MY_ENCODING "ISO-8859-1"

/*Global parameters. If equal to 1, answer was received.*/
int AnsRad=0, AnsObst=0, AnsMove=0;

/*dx, dy - global parameters, delta of robot's coordinates. transfered from coordinates from client xml-message*/
float dx=0, dy=0;

/*global parameters for values*/
float rad_l, rad_f, rad_r, obst_l, obst_f, obst_r;

void askXmlwriterFilename(const char *uri, char *ask, int code)
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
      if(code!=NIHIL) {
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "DEC", "%d", code);
        if (rc < 0) {
          printf("Error at xmlTextWriterWriteAttribute\n");
          return;
        }
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

void ask(void *argument, char *ask, char *file, int code) {
  int serv_sock = *(int *) argument;
  int xmlsize;
  FILE *xmlfd;
  LIBXML_TEST_VERSION
  askXmlwriterFilename(file, ask, code);
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

void ansObst(char *xmlfile){
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
      obst_l = strtod(tmp, NULL);
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
      obst_f = strtod(tmp, NULL);
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
      obst_r = strtod(tmp, NULL);
    }
  }
  AnsObst=1;
}

void ansMove(char *xmlfile){
  xmlXPathContextPtr context;
  xmlXPathObjectPtr result;
  xmlChar *xpath1 = "/DATA/ANS/text()";
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
      if(!strcmp(tmp,"OK"))
        AnsMove=1;
    }
  }
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
      if(!strcmp(tmp,"OBST"))
        ansObst(xmlfile);
      if(!strcmp(tmp,"MOVE"))
        ansMove(xmlfile);
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

int Decision() {
  if(rad_l==1.0 || rad_f==1.0 || rad_r==1.0)
    return STOP;
  int F=0, L=0, R=0;
  if(rad_f>=rad_l && rad_f>=rad_r) {
    F+=40; L+=20; R+=20;
  }
  if(rad_l>=rad_f && rad_l>=rad_r) {
    F+=20; L+=40; R+=20;
  }
  if(rad_r>=rad_l && rad_r>=rad_f) {
    F+=20; L+=20; R+=40;
  }
  if(obst_f==0.0) F+=60;
  if(obst_l==0.0) L+=40;
  if(obst_r==0.0) R+=20;
  if(obst_f==1.0) {
    L+=40; R+=40;
  }
  if(obst_l==1.0) {
    F+=20; R+=40;
  }
  if(obst_r==1.0) {
    F+=20; L+=20;
  }
  
  if(F>=L && F>=R)
    return F_STEP;
  if(L>=F && L>=R)
    return L_STEP;
  if(R>=L && R>=F)
    return R_STEP;
  return STOP;
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
    ask(argument, "RAD", "askrad.xml", NIHIL);
    printf("Asking client_rad for data\n");
    while(!AnsRad) {}
    AnsRad=0;
    printf("rad_l=%f\trad_f=%f\trad_r=%f\n",rad_l,rad_f,rad_r);
    ask(argument, "OBST", "askobst.xml", NIHIL);
    printf("Asking client_obst for data\n");
    while(!AnsObst) {}
    AnsObst=0;
    printf("obst_l=%f\tobst_f=%f\tobst_r=%f\n",obst_l,obst_f,obst_r);
    int dec;
    dec=Decision();
    printf("dec=%d\n",dec);
    if(dec==STOP) {
      ask(argument, "MOVE", "askmove.xml", dec);
      break;
    }
    ask(argument, "MOVE", "askmove.xml", dec);
    printf("Asking client_move to move\n");
    while(!AnsMove) {}
    AnsMove=0;
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
  return 1;
}

int main(int argc, char **argv)
{
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
}



