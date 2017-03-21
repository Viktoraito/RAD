#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/queue.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <libxml/xpath.h>
#include <malloc.h>

#define DEFPORT 7777
#define NIHIL 000
#define L_STEP 444
#define F_STEP 888
#define R_STEP 666
#define STOP 555
