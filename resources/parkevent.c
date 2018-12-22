#include <stdio.h>
#include <string.h>
#include "rest-engine.h"
#include "er-coap.h"
#include "er-dc-test.h"
#include "sys/node-id.h"
#include "../dataLayer.h"



static void res_post_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);


RESOURCE(parkevent, "title=\"PARKEVENTS\"", NULL, res_post_handler , NULL, NULL);


static void res_post_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset)
{
  coap_packet_t * coap_req = (coap_packet_t *)request;

  uint8_t *incoming = NULL;
  size_t len = REST.get_request_payload(request, (const uint8_t **)&incoming);
  PRINTF("Request: \n");

  char * incomingMessage = (char *)incoming;
  PRINTF("RX: %d\n%s\n",len ,incomingMessage);


  DataLayer_addOrReplaceRecords(incomingMessage);


  REST.set_response_status(response, REST.status.CHANGED);

}
