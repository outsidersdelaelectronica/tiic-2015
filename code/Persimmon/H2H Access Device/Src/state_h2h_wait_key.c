#include "state_h2h_wait_key.h"

/* Possible transition to the following states */
#include "state_main.h"


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

/* State behaviour */
void behaviour_h2h_wait_key(state_ptr state)
{
  bt_packet_t fsm_send_packet = {.packet_content = {0}};
  /* Set events to react to */
//  state->h2h_pass_ready       = default_h2h_pass_ready;
  
  /* Do state actions */
  sprintf(&fsm_send_packet.packet_content[8],"%s",gen_ack);
  
  osSemaphoreRelease(sem_ecg_keygenHandle);
  osMailPut(queue_bt_packet_sendHandle, (void *) &fsm_send_packet);
}

/* Entry point to the state */
void entry_to_h2h_wait_key(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "h2h_start_gen");
  
  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_h2h_wait_key(state);
  behaviour_h2h(state);
  behaviour_running(state);
}