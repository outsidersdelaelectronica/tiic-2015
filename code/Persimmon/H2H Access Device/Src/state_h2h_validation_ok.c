#include "state_h2h_validation_ok.h"

/* Possible transition to the following states */
#include "state_main.h"
#include "state_h2h_ongoing_error.h"

/* Parent states */
#include "state_h2h.h"
#include "state_running.h"

/* State includes */
#include "cmsis_os.h"
#include "menu.h"
#include "bluetooth_internal.h"

/* Semaphores */
extern osSemaphoreId sem_ecg_keygenHandle;
/* Queues */
extern osMailQId queue_bt_packet_sendHandle;
extern osMailQId queue_lcdHandle;


/* State behaviour */
void behaviour_h2h_validation_ok(state_ptr state)
{
  bt_packet_t fsm_send_packet_1 = {.packet_content = {0}}, fsm_send_packet_2 = {.packet_content = {0}};
  /* Set events to react to */
  
  /* Do state actions */
  
  sprintf(&fsm_send_packet_1.packet_content[8],"%s",key_ready);
   /* Send Key ready */ 
  osMailPut(queue_bt_packet_sendHandle, (void *) &fsm_send_packet_1);
  
  event = osMailGet(queue_ecg_keybtHandle, osWaitForever);
  if (event.status == osEventMail)
  {
    inter_key = *((validation_key_t*) event.value.v);
    sprintf(&fsm_send_packet_2.packet_content[8],"%u",&(inter_key.token));
    fsm_send_packet_2.packet_content[0] = 1;
    /* Send Key */ 
    osMailPut(queue_bt_packet_sendHandle, (void *) &fsm_send_packet_1);  
  
}

/* Entry point to the state */
void entry_to_h2h_validation_ok(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "h2h_autentication");
  
  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_running(state);
  behaviour_h2h(state);
  behaviour_h2h_validation_ok(state);
}