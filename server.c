#include "common.h"

#define QUEUE 3

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
