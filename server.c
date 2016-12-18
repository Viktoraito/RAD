#include "common.h"

#define QUEUE 3

int rad_fd,proc_fd,obst_fd,move_fd;

void parseHello(char *xmlfile, int fd) {
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
  else
    if(result->nodesetval[0].nodeTab[0]->content != NULL) {
      char *tmp = calloc(sizeof(result->nodesetval[0].nodeTab[0]->content),
			 sizeof(char));
      tmp = (char *) result->nodesetval[0].nodeTab[0]->content;
      if(!strcmp(tmp,"RAD")) rad_fd=fd;
      if(!strcmp(tmp,"PROC")) proc_fd=fd;
      if(!strcmp(tmp,"OBST")) obst_fd=fd;
      if(!strcmp(tmp,"MOVE")) move_fd=fd;
    }
}

void askRad(){
  printf("TODO: write ask to RAD\n");
}

void askObst(){
  printf("TODO: write ask to OBST\n");
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
  else
    if(result->nodesetval[0].nodeTab[0]->content != NULL) {
      char *tmp = calloc(sizeof(result->nodesetval[0].nodeTab[0]->content),
			 sizeof(char));
      tmp = (char *) result->nodesetval[0].nodeTab[0]->content;
      if(!strcmp(tmp,"RAD")) 
	askRad();
      if(!strcmp(tmp,"OBST"))
	askObst();
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

int removeClient(int fd, fd_set *readfds) {
  close(fd);
  FD_CLR(fd, readfds);
  printf("Removing client...\n");
}

int retriveData(int fd, int nread) {
  char *buf = calloc(nread, sizeof(char));
  read(fd, buf, nread);
  int xmlfd;
  mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
  xmlfd = open("reader1.xml", O_WRONLY | O_CREAT, mode);
  write(xmlfd,buf,nread);
  close(xmlfd); free(buf);
  if(rad_fd==-1 || proc_fd==-1 || obst_fd==-1 || move_fd==-1)
    parseHello("reader1.xml",fd);
  if(fd==proc_fd)
    parseAsk("reader1.xml");
  else if(!(fd==rad_fd || fd==obst_fd || fd==move_fd))
    printf("Unregistered file descryptor\n");
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
      ioctl(fd, FIONREAD, &nread);
      if(nread) {
	retriveData(fd, nread);
      }
      else{
	removeClient(fd,&readfds);}
    }
  }
}

int main( int argc, char **argv) {
  rad_fd=-1;proc_fd=-1;obst_fd=-1;move_fd=-1;
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
}
