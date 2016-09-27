#include "tasks_bt.h"

#include "fsm_client.h"
#include "authentication.h"

/* Queues */
osMailQId queue_bt_packet_recievedHandle;
osMailQId queue_bt_packet_sendHandle;
extern osMailQId queue_fsm_eventsHandle;
extern osMailQId queue_ecg_keyHandle;

/* Tasks */
osThreadId bt_txTaskHandle;
osThreadId bt_rxTaskHandle;
void Start_bt_txTask(void const * argument);
void Start_bt_rxTask(void const * argument);

/* Objects */

void tasks_bt_init()
{
  /* Queues */
  osMailQDef(queue_bt_packet_recieve, 4, bt_packet_t);
  queue_bt_packet_recievedHandle = osMailCreate(osMailQ(queue_bt_packet_recieve), NULL);

  osMailQDef(queue_bt_packet_send, 4, bt_packet_t);
  queue_bt_packet_sendHandle = osMailCreate(osMailQ(queue_bt_packet_send), NULL);
}

void tasks_bt_start()
{
  /* Tasks */
  /* bt_txTask */
  osThreadDef(bt_txTask, Start_bt_txTask, osPriorityHigh, 0, 240);
  bt_txTaskHandle = osThreadCreate(osThread(bt_txTask), NULL);

  /* bt_rxTask */
  osThreadDef(bt_rxTask, Start_bt_rxTask, osPriorityLow, 0, 240);
  bt_rxTaskHandle = osThreadCreate(osThread(bt_rxTask), NULL);
}

void Start_bt_txTask(void const * argument)
{
  osEvent event_pk_to_send;
  bt_packet_t send_packet = {.packet_content = {0}};;

  /* Infinite loop */
  for(;;)
  {
    event_pk_to_send = osMailGet(queue_bt_packet_sendHandle, osWaitForever);
    if (event_pk_to_send.status == osEventMail)
    {
      send_packet = *((bt_packet_t *) event_pk_to_send.value.p);
      SendData(PACKET_SIZE, send_packet.packet_content);
      osDelay(1);
    }
  }
}

void Start_bt_rxTask(void const * argument)
{
  osEvent event_pk_rec;
  bt_packet_t rec_packet;
  char command_str[24] = "";
  int i;
  uint64_t token;
  fsm_event_f bt_event;
  validation_key_t rec_key;
  
  init_key(&rec_key,EXTERN);
  
  /* Infinite loop */
  for(;;)
  {
    event_pk_rec = osMailGet(queue_bt_packet_recievedHandle, osWaitForever);
    if (event_pk_rec.status == osEventMail)
    {
      rec_packet = *((bt_packet_t *) event_pk_rec.value.p);
      /* Command */
      if(!rec_packet.packet_content[0])
      {
        sprintf(command_str, "%s", &rec_packet.packet_content[8]);
        bt_event =fsm_no_event;
        if(!strcmp(command_str,gen_init))
        {
          bt_event = fsm_h2h_start_gen;
        }else if(!strcmp(command_str,key_ready))
        {
          bt_event = fsm_h2h_pass_ready;
        }
        
      }
      else if(rec_packet.packet_content[0]) /* Key */
      {
        for( i = 0; i < 24; i++)
        {
          token |= (((uint64_t)rec_packet.packet_content[0]) << (4*i));
        }
        write_token_key(&rec_key,token);
        while (osMailPut(queue_ecg_keyHandle, (void *) &rec_key) != osOK)
        {
          osDelay(1);
        }
      }
      
      while(osMailPut(queue_fsm_eventsHandle, (void *) &bt_event) != osOK)
      {
        osDelay(1);
      }
    }
  }
}

