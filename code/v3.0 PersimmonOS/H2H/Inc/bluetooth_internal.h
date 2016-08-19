/*
 * bluetooth_internal.h
 *
 *  Created on: 22/6/2015
 *      Author: Smau
 */

#ifndef BLUETOOTH_INTERNAL_H_
#define BLUETOOTH_INTERNAL_H_

#include "HCITRANS.h"
#include "BTVSAPI.h"             /* Vendror Specific header.                  */
#include "SS1BTPS.h"             /* Main SS1 Bluetooth Stack Header.          */
#include "BTPSKRNL.h"            /* BTPS Kernel Header.                       */

#include "bt_const_and_struct.h"
#include "command_interpreter.h"

/* Functions non accessable by user                              */

int InitializeApplication(HCI_DriverInformation_t *HCI_DriverInformation, BTPS_Initialization_t *BTPS_Initialization);
int OpenStack(HCI_DriverInformation_t *HCI_DriverInformation, BTPS_Initialization_t *BTPS_Initialization);
int CloseStack(void);
void BD_ADDRToStr(BD_ADDR_t Board_Address, BoardStr_t BoardStr);
int SendData(uint16_t length, unsigned char *buff);

/* Functions accessable by user menu                        */

int DeleteLinkKey(BD_ADDR_t BD_ADDR);
int Inquiry(ParameterList_t *TempParam);



int ChangeSimplePairingParameters(ParameterList_t *TempParam);
int Pair(ParameterList_t *TempParam);
int EndPairing(ParameterList_t *TempParam);
int PINCodeResponse(ParameterList_t *TempParam);
int PassKeyResponse(ParameterList_t *TempParam);
int UserConfirmationResponse(ParameterList_t *TempParam);
int GetLocalAddress(ParameterList_t *TempParam);
int SetLocalName(ParameterList_t *TempParam);
int GetLocalName(ParameterList_t *TempParam);
int SetClassOfDevice(ParameterList_t *TempParam);
int GetClassOfDevice(ParameterList_t *TempParam);
int GetRemoteName(ParameterList_t *TempParam);
int SniffMode(ParameterList_t *TempParam);
int ExitSniffMode(ParameterList_t *TempParam);

int Read(ParameterList_t *TempParam);
int Write(ParameterList_t *TempParam);
int GetConfigParams(ParameterList_t *TempParam);
int SetConfigParams(ParameterList_t *TempParam);
int GetQueueParams(ParameterList_t *TempParam);
int SetQueueParams(ParameterList_t *TempParam);
int Loopback(ParameterList_t *TempParam);
int DisplayRawModeData(ParameterList_t *TempParam);
int SetBaudRate(ParameterList_t *TempParam);

/* Setters w/o parameters                                        */

int SetDiscoverabilityMode(GAP_Discoverability_Mode_t DiscoverabilityMode);
int SetConnectabilityMode(GAP_Connectability_Mode_t ConnectableMode);
int SetPairabilityMode(GAP_Pairability_Mode_t  PairabilityMode);
int AutomaticReadMode(void);
int OpenServer(void);
int CloseServer(ParameterList_t *TempParam);
int OpenRemoteServer(ParameterList_t *TempParam);
int CloseRemoteServer(ParameterList_t *TempParam);

/* Callbacks for bt events                                   */
void BTPSAPI GAP_Event_Callback(unsigned int BluetoothStackID, GAP_Event_Data_t *GAP_Event_Data, unsigned long CallbackParameter);
void BTPSAPI SPP_Event_Callback(unsigned int BluetoothStackID, SPP_Event_Data_t *SPP_Event_Data, unsigned long CallbackParameter);
void BTPSAPI HCI_Event_Callback(unsigned int BluetoothStackID, HCI_Event_Data_t *HCI_Event_Data, unsigned long CallbackParameter);


#endif /* BLUETOOTH_INTERNAL_H_ */