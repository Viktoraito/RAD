#include "common.h"

#define MY_ENCODING "ISO-8859-1"

void testXmlwriterFilename(const char *uri);

int main(int argc, char **argv)
{
  int serv_sock;
  serv_sock = socket(AF_INET, SOCK_STREAM, 0);
  if(!conn_serv(serv_sock)) {
    printf("Can;t connect with server\n");
    exit(EXIT_FAILURE);
  }
  printf("Successfully connected to server\n");
/*  pthread_t hear_t, speak_t;
  pthread_create(&hear_t, NULL, hear, &serv_sock);
  pthread_create(&speak_t, NULL, speak, &serv_sock);
  pthread_join(hear_t, NULL);
  pthread_join(speak_t,NULL);*/
  exit(EXIT_SUCCESS);

/*  LIBXML_TEST_VERSION
  testXmlwriterFilename("writer1.xml");
  xmlCleanupParser();
  xmlMemoryDump();
  return 0;*/
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

void testXmlwriterFilename(const char *uri)
{
  int rc;
  xmlTextWriterPtr writer;

  /*The range of sensor values:*/
  /*angle: 0 to 6.283/
  /*hamma: 0.05 to 3.0*/
  /*beta: 0.05 to 3.5*/
  /*dose: 0.001 to 999*/
  /*dose_rate: 0.1 to 999*/
  /*flux_density: 6 to 999*/

  float angle=2.1, hamma=0.09, beta=0.12, dose=100, dose_rate=0.26, flux_density=11;

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

      rc = xmlTextWriterStartElement(writer, BAD_CAST "RADIATION_DATA");
      if (rc < 0) {
          printf("Error at xmlTextWriterStartElement\n");
          return;
      }

        rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "Radiation_expressed_in", BAD_CAST "MeV");
        if (rc < 0) {
          printf("Error at xmlTextWriterWriteAttribute\n");
          return;
        }

        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "HAMMA", "%f", hamma);
        if (rc < 0) {
          printf("Error at xmlTextWriterWriteFormatElement\n");
          return;
        }

        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "BETA", "%f", beta);
        if (rc < 0) {
          printf("Error at xmlTextWriterWriteFormatElement\n");
          return;
        }

      rc = xmlTextWriterEndElement(writer);
      if (rc < 0) {
        printf("Error at xmlTextWriterEndElement\n");
        return;
      }

      rc = xmlTextWriterStartElement(writer, BAD_CAST "DOSE_DATA");
      if (rc < 0) {
        printf("Error at xmlTextWriterStartElement\n");
        return;
      }

        rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "Dose_expressed_in", BAD_CAST "mSv");
        if (rc < 0) {
          printf("Error at xmlTextWriterWriteAttribute\n");
          return;
        }

        rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "Dose_rate_expressed_in", BAD_CAST "microSv/h");
        if (rc < 0) {
          printf("Error at xmlTextWriterWriteAttribute\n");
          return;
        }

        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "DOSE", "%f", dose);
        if (rc < 0) {
          printf("Error at xmlTextWriterWriteFormatElement\n");
          return;
        }

        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "DOSE_RATE", "%f", dose_rate);
        if (rc < 0) {
          printf("Error at xmlTextWriterWriteFormatElement\n");
          return;
        }

      rc = xmlTextWriterEndElement(writer);
      if (rc < 0) {
        printf("Error at xmlTextWriterEndElement\n");
        return;
      }

      rc = xmlTextWriterStartElement(writer, BAD_CAST "FLUX_DATA");
      if (rc < 0) {
        printf("Error at xmlTextWriterStartElement\n");
        return;
      }

        rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "Flux_density_expressed_in", BAD_CAST "1/(cm2*min)");
        if (rc < 0) {
          printf("Error at xmlTextWriterWriteAttribute\n");
          return;
        }

        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "FLUX_DENSITY", "%f", flux_density);
        if (rc < 0) {
          printf("Error at xmlTextWriterWriteFormatElement\n");
          return;
        }

      rc = xmlTextWriterEndElement(writer);
      if (rc < 0) {
        printf("Error at xmlTextWriterEndElement\n");
        return;
      }

      rc = xmlTextWriterStartElement(writer, BAD_CAST "DIRECTION_DATA");
      if (rc < 0) {
        printf("Error at xmlTextWriterStartElement\n");
        return;
      }

        rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "Angles_expressed_in", BAD_CAST "radians");
        if (rc < 0) {
          printf("Error at xmlTextWriterWriteAttribute\n");
          return;
        }

        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "ANGLE", "%f", angle);
        if (rc < 0) {
          printf("Error at xmlTextWriterWriteFormatElement\n");
          return;
        }

    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterEndElement\n");
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

