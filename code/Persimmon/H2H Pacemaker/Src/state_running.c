#include "state_running.h"

/* Possible transition to the following states */
#include "state_goodbye.h"

/* Child states */
#include "state_h2h_wait.h"
#include "state_h2h_connected.h"
#include "state_h2h_keygen.h"
#include "state_h2h_waitkey.h"
#include "state_h2h_auth.h"

/* State includes */
#include "cmsis_os.h"
#include "menu.h"
#include "buzzer.h"

/* Queues */
extern osMailQId queue_periph_buzzerHandle;

static void running_action_button_short(state_ptr state)
{
  buzzer_note_t beep;

  /* Do transition actions */

  /* Beep */
  beep.note = A6;
  beep.ms = 100;
  while(osMailPut(queue_periph_buzzerHandle, (void *) &beep) != osOK)
  {
    osDelay(1);
  }
}

static void running_to_goodbye(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_goodbye(state);
}

/* State behaviour */
void behaviour_running(state_ptr state)
{
  /* Set events to react to */
  state->button_short = running_action_button_short;
  state->button_long = running_to_goodbye;

  /* Do state actions */
}

/* Entry point to the state */
void entry_to_running(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "running");

  /* Go to child default state */
  entry_to_h2h_wait(state);
}