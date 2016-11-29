#include <stdio.h>
#include <string.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#if defined(LIBXML_WRITER_ENABLED) && defined(LIBXML_OUTPUT_ENABLED)

#define MY_ENCODING "ISO-8859-1"

void testXmlwriterFilename(const char *uri);

int main(void)
{
  LIBXML_TEST_VERSION

  testXmlwriterFilename("writer1.xml");

  xmlCleanupParser();

  xmlMemoryDump();
  return 0;
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

#else
int main(void) {
  fprintf(stderr, "Writer or output support not compiled in\n");
  exit(1);
}
#endif

