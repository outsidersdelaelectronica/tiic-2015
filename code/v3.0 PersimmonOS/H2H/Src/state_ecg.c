#include "state_ecg.h"

/* Possible transition to the following states */
#include "state_main.h"

/* Child states */
#include "state_ecg_1_lead.h"
#include "state_ecg_2_lead.h"
#include "state_ecg_6_lead.h"

static void ecg_to_main(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_main(state);
}

static void ecg_to_ecg_1_lead(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_ecg_1_lead(state);
}

static void ecg_to_ecg_2_lead(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_ecg_2_lead(state);
}

static void ecg_to_ecg_6_lead(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_ecg_6_lead(state);
}

/* State behaviour */
void behaviour_ecg(state_ptr state)
{
  /* Set events to react to */
  state->back = ecg_to_main;
  state->ecg_1_lead = ecg_to_ecg_1_lead;
  state->ecg_2_lead = ecg_to_ecg_2_lead;
  state->ecg_6_lead = ecg_to_ecg_6_lead;

  /* Do state actions */
}

/* Entry point to the state */
void entry_to_ecg(state_ptr state)
{
  /* Go to child default state */
  entry_to_ecg_1_lead(state);
}