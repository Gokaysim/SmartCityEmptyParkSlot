
#include "base.h"
#include "dataLayer.h"
#include "helpers.h"

#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/rpl/rpl.h"
#include "sys/node-id.h"

#include "net/netstack.h"
/* #include "dev/leds.h" */
#include <stdio.h>
#include <ctype.h>
#include <random.h>

/* from er-rest-example/er-rexample-server.c */
#include "rest-engine.h"

/**************************************************************************/
/* from er-rest-example/er-rexample-client.c */

#include "er-coap-engine.h"
#include "er-coap.h"
#include "er-coap-observe-client.h"
/* #include "dev/button-sensor.h" */




#define REMOTE_PORT     UIP_HTONS(COAP_DEFAULT_PORT)
#define GET_INTERVAL 10

#if DEBUG
#define PRINT6ADDR(addr) PRINTF("[%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x]", ((uint8_t *)addr)[0], ((uint8_t *)addr)[1], ((uint8_t *)addr)[2], ((uint8_t *)addr)[3], ((uint8_t *)addr)[4], ((uint8_t *)addr)[5], ((uint8_t *)addr)[6], ((uint8_t *)addr)[7], ((uint8_t *)addr)[8], ((uint8_t *)addr)[9], ((uint8_t *)addr)[10], ((uint8_t *)addr)[11], ((uint8_t *)addr)[12], ((uint8_t *)addr)[13], ((uint8_t *)addr)[14], ((uint8_t *)addr)[15])
#define PRINTLLADDR(lladdr) PRINTF("[%02x:%02x:%02x:%02x:%02x:%02x]", (lladdr)->addr[0], (lladdr)->addr[1], (lladdr)->addr[2], (lladdr)->addr[3], (lladdr)->addr[4], (lladdr)->addr[5])
#else
#define PRINT6ADDR(addr)
#define PRINTLLADDR(addr)
#endif

static uip_ipaddr_t leftServer_ipaddr;
static uip_ipaddr_t rightServer_ipaddr;
static struct etimer et;
extern resource_t parkevent;

struct uip_ds6_addr *root_if;


/*---------------------------------Methods For client---------------------------------*/
static void set_global_address(void)
{
  uip_ipaddr_t ipaddr;

  uip_ip6addr(&ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, node_id);
  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
  uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);

  int nextNode = node_id +1;
  int previousNode = node_id+1;
  uip_ip6addr(&leftServer_ipaddr, 0xfe80, 0, 0, 0, 0x0200, 0, 0, nextNode);
  uip_ip6addr(&rightServer_ipaddr, 0xfe80, 0, 0, 0, 0x0200, 0, 0, previousNode);
  // uip_ip6addr(&server_ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0x00ff, 0xfe00, 1);


}
static void tcpip_handler(void)
{
  char *str;

  if(uip_newdata()) {
    str = uip_appdata;
    str[uip_datalen()] = '\0';
    printf("DATA recv '%s'\n", str);
  }
}
void client_chunk_handler(void *response)
{
  const uint8_t *chunk;

  coap_get_payload(response, &chunk);

  PRINTF("Response: \n");
  PRINTF("RX: %d\n", chunk[0]);
}

static void print_local_addresses(void)
{
  int i;
  uint8_t state;

  PRINTF("Client IPv6 addresses: ");
  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
    state = uip_ds6_if.addr_list[i].state;
    if(uip_ds6_if.addr_list[i].isused &&
       (state == ADDR_TENTATIVE || state == ADDR_PREFERRED)) {
      PRINT6ADDR(&uip_ds6_if.addr_list[i].ipaddr);
      PRINTF("\n");
      /* hack to make address "final" */
      if(state == ADDR_TENTATIVE) {
        uip_ds6_if.addr_list[i].state = ADDR_PREFERRED;
      }
    }
  }
}

PROCESS(park_server, "Park Server");
PROCESS(park_client, "Park Client");
AUTOSTART_PROCESSES(&park_server,&park_client);

PROCESS_THREAD(park_server, ev, data)
{
  uip_ipaddr_t ipaddr;
  struct uip_ds6_addr *root_if;

  PROCESS_BEGIN();

  clock_init();
  PRINTF("Park mote server process started\n");

  PROCESS_PAUSE();
#ifdef RF_CHANNEL
  PRINTF("RF channel: %u\n", RF_CHANNEL);
#endif
#ifdef IEEE802154_PANID
  PRINTF("PAN ID: 0x%04X\n", IEEE802154_PANID);
#endif

  PRINTF("uIP buffer: %u\n", UIP_BUFSIZE);
  PRINTF("LL header: %u\n", UIP_LLH_LEN);
  PRINTF("IP+UDP header: %u\n", UIP_IPUDPH_LEN);
  PRINTF("REST max chunk: %u\n", REST_MAX_CHUNK_SIZE);

  /* Initialize the REST engine. */
  rest_init_engine();

  rest_activate_resource(&parkevent, "parkevent");


  #if 0
  /* Mode 1 - 64 bits inline */
    uip_ip6addr(&ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 1);
  #elif 1
  /* Mode 2 - 16 bits inline */
    uip_ip6addr(&ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0x00ff, 0xfe00, 1);
  #else
  /* Mode 3 - derived from link local (MAC) address */
    uip_ip6addr(&ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 0);
    uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
  #endif

    uip_ds6_addr_add(&ipaddr, 0, ADDR_MANUAL);
    root_if = uip_ds6_addr_lookup(&ipaddr);
    if(root_if != NULL) {
      rpl_dag_t *dag;
      dag = rpl_set_root(RPL_DEFAULT_INSTANCE, (uip_ip6addr_t *)&ipaddr);
      uip_ip6addr(&ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 0);
      rpl_set_prefix(dag, &ipaddr, 64);
      PRINTF("created a new RPL dag\n");
    } else {
      PRINTF("failed to create a new RPL DAG\n");
    }

  print_local_addresses();
  /* The data sink runs with a 100% duty cycle in order to ensure high
     packet reception rates. */
  NETSTACK_MAC.off(1);

  while(1) {
    PROCESS_WAIT_EVENT();
  }

  PROCESS_END();
}

PROCESS_THREAD(park_client, ev, data)
{
  PROCESS_BEGIN();
  PROCESS_PAUSE();
  DataLayer_initStorage();
  set_global_address();

  PRINTF("Park mote client process started\n");

  print_local_addresses();


  /* receives all CoAP messages */
  coap_init_engine();

  etimer_set(&et, GET_INTERVAL * CLOCK_SECOND);

  while(1) {
    PROCESS_YIELD();

    if(ev == tcpip_event) {
      printf("TCPIP_HANDLER\n");
      tcpip_handler();
    }

    if(etimer_expired(&et)) {

      if(rand()%100<25)
      {
        DataLayer_addOwnRecord(rand()%2);

        char* message = DataLayer_getDataToSend();

        coap_packet_t * request = (coap_packet_t *)malloc(sizeof(coap_packet_t));
        coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
        size_t lenght = strlen(message);
        coap_set_payload(request,message,lenght);

        coap_set_header_uri_path(request, "/parkevent");

        COAP_BLOCKING_REQUEST(&leftServer_ipaddr, REMOTE_PORT, request,
                          client_chunk_handler);

        coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);

        coap_set_payload(request,message,lenght);

        coap_set_header_uri_path(request, "/parkevent");

        COAP_BLOCKING_REQUEST(&rightServer_ipaddr, REMOTE_PORT, request,
                          client_chunk_handler);
        free(message);
      }
      else if (DataLayer_isDataNeedToBeSend()){
        char* message = DataLayer_getDataToSend();

        coap_packet_t * request = (coap_packet_t *)malloc(sizeof(coap_packet_t));
        coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
        size_t lenght = strlen(message);
        coap_set_payload(request,message,lenght);

        coap_set_header_uri_path(request, "/parkevent");

        COAP_BLOCKING_REQUEST(&leftServer_ipaddr, REMOTE_PORT, request,
                          client_chunk_handler);

        coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);

        coap_set_payload(request,message,lenght);

        coap_set_header_uri_path(request, "/parkevent");

        COAP_BLOCKING_REQUEST(&rightServer_ipaddr, REMOTE_PORT, request,
                          client_chunk_handler);
        free(message);
      }
      etimer_reset(&et);
    }
  } /* END_WHILE(1) */

  PROCESS_END();
}
