/*
 * bluetooth_functions.c
 *
 *  Created on: 22/6/2015
 *      Author: Tomas Funk
 */

#include "bluetooth_internal.h"

#include "lcd.h"
#include "stm32l1xx_hal.h"

extern unsigned int        BluetoothStackID;

int                        UI_Mode;                 /* Holds the UI Mode.              */

static int                 SerialPortID;            /* Variable which contains the     */
                                                    /* Handle of the most recent       */
                                                    /* SPP Port that was opened.       */

static int                 ServerPortID;            /* Variable which contains the     */
                                                    /* Handle of the SPP Server Port   */
                                                    /* that was opened.                */

static Word_t              Connection_Handle;       /* Holds the Connection Handle of  */
                                                    /* the most recent SPP Connection. */

static Boolean_t           Connected;               /* Variable which flags whether or */
                                                    /* not there is currently an active*/
                                                    /* connection.                     */

static DWord_t             SPPServerSDPHandle;      /* Variable used to hold the Serial*/
                                                    /* Port Service Record of the      */
                                                    /* Serial Port Server SDP Service  */
                                                    /* Record.                         */

static BD_ADDR_t           InquiryResultList[MAX_INQUIRY_RESULTS]; /* Variable which   */
                                                    /* contains the inquiry result     */
                                                    /* received from the most recently */
                                                    /* preformed inquiry.              */

static unsigned int        NumberofValidResponses;  /* Variable which holds the number */
                                                    /* of valid inquiry results within */
                                                    /* the inquiry results array.      */

static LinkKeyInfo_t       LinkKeyInfo[MAX_SUPPORTED_LINK_KEYS]; /* Variable holds     */
                                                    /* BD_ADDR <-> Link Keys for       */
                                                    /* pairing.                        */

static BD_ADDR_t           CurrentRemoteBD_ADDR;    /* Variable which holds the        */
                                                    /* current BD_ADDR of the device   */
                                                    /* which is currently pairing or   */
                                                    /* authenticating.                 */

static GAP_IO_Capability_t IOCapability;            /* Variable which holds the        */
                                                    /* current I/O Capabilities that   */
                                                    /* are to be used for Secure Simple*/
                                                    /* Pairing.                        */

static Boolean_t           OOBSupport;              /* Variable which flags whether    */
                                                    /* or not Out of Band Secure Simple*/
                                                    /* Pairing exchange is supported.  */

static Boolean_t           MITMProtection;          /* Variable which flags whether or */
                                                    /* not Man in the Middle (MITM)    */
                                                    /* protection is to be requested   */
                                                    /* during a Secure Simple Pairing  */
                                                    /* procedure.                      */

static BD_ADDR_t           SelectedBD_ADDR;         /* Holds address of selected Device*/

static BD_ADDR_t           NullADDR;                /* Holds a NULL BD_ADDR for comp.  */
                                                    /* purposes.                       */

static Boolean_t           LoopbackActive;          /* Variable which flags whether or */
                                                    /* not the application is currently*/
                                                    /* operating in Loopback Mode      */
                                                    /* (TRUE) or not (FALSE).          */

static Boolean_t           DisplayRawData;          /* Variable which flags whether or */
                                                    /* not the application is to       */
                                                    /* simply display the Raw Data     */
                                                    /* when it is received (when not   */
                                                    /* operating in Loopback Mode).    */

static Boolean_t           AutomaticReadActive;     /* Variable which flags whether or */
                                                    /* not the application is to       */
                                                    /* automatically read all data     */
                                                    /* as it is received.              */

static BoardStr_t          Callback_BoardStr;       /* Holds a BD_ADDR string in the   */
                                                    /* Callbacks.                      */

static BoardStr_t          Function_BoardStr;       /* Holds a BD_ADDR string in the   */
                                                    /* various functions.              */

static Send_Info_t         SendInfo;                /* Variable that contains          */
                                                    /* information about a data        */
                                                    /* transfer process.               */

   /* Variables which contain information used by the loopback          */
   /* functionality of this test application.                           */
static unsigned int        BufferLength;

static unsigned char       Buffer[64];

static Boolean_t           BufferFull;

   /* The following string table is used to map HCI Version information */
   /* to an easily displayable version string.                          */
static BTPSCONST char *HCIVersionStrings[] =
{
   "1.0b",
   "1.1",
   "1.2",
   "2.0",
   "2.1",
   "3.0",
   "4.0",
   "4.1",
   "Unknown (greater 4.1)"
} ;

#define NUM_SUPPORTED_HCI_VERSIONS              (sizeof(HCIVersionStrings)/sizeof(char *) - 1)

   /* The following string table is used to map the API I/O Capabilities*/
   /* values to an easily displayable string.                           */
static BTPSCONST char *IOCapabilitiesStrings[] =
{
   "Display Only",
   "Display Yes/No",
   "Keyboard Only",
   "No Input/Output"
} ;

 /* The following function is used to initialize the application      */
   /* instance.  This function should open the stack and prepare to     */
   /* execute commands based on user input.  The first parameter passed */
   /* to this function is the HCI Driver Information that will be used  */
   /* when opening the stack and the second parameter is used to pass   */
   /* parameters to BTPS_Init.  This function returns the               */
   /* BluetoothStackID returned from BSC_Initialize on success or a     */
   /* negative error code (of the form APPLICATION_ERROR_XXX).          */
int InitializeApplication(HCI_DriverInformation_t *HCI_DriverInformation, BTPS_Initialization_t *BTPS_Initialization)
{
   int ret_val = APPLICATION_ERROR_UNABLE_TO_OPEN_STACK;

   /* Initiailize some defaults.                                        */
   SerialPortID           = 0;
   UI_Mode                = UI_MODE_SELECT;
   LoopbackActive         = FALSE;
   DisplayRawData         = FALSE;
   AutomaticReadActive    = FALSE;
   NumberofValidResponses = 0;

   /* Next, makes sure that the Driver Information passed appears to be */
   /* semi-valid.                                                       */
   if((HCI_DriverInformation) && (BTPS_Initialization))
   {
      /* Try to Open the stack and check if it was successful.          */
      if(!OpenStack(HCI_DriverInformation, BTPS_Initialization))
      {
         /* The stack was opened successfully.  Now set some defaults.  */

         /* First, attempt to set the Device to be Connectable.         */
         ret_val = SetConnect();

         /* Next, check to see if the Device was successfully made      */
         /* Connectable.                                                */
         if(!ret_val)
         {
            /* Now that the device is Connectable attempt to make it    */
            /* Discoverable.                                            */
            ret_val = SetDiscoverabilityMode(dmGeneralDiscoverableMode);

            /* Next, check to see if the Device was successfully made   */
            /* Discoverable.                                            */
            if(!ret_val)
            {
               /* Now that the device is discoverable attempt to make it*/
               /* pairable.                                             */
               ret_val = SetPairable();
               if(!ret_val)
               {
                  /* Attempt to register a HCI Event Callback.          */
                  ret_val = HCI_Register_Event_Callback(BluetoothStackID, HCI_Event_Callback, (unsigned long)NULL);
                  if(ret_val > 0)
                  {
                     /* Assign a NULL BD_ADDR for comparison.           */
                     ASSIGN_BD_ADDR(NullADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
                     /* Return success to the caller.                   */
                     ret_val = (int)BluetoothStackID;
                     if(ret_val > 0)
                     {
                       // BOOOOOOOOOOOOOM HEADSHOT
                       OpenServer((ParameterList_t *) NULL);
                     }
                  }
               }
            }
         }
         /* In some error occurred then close the stack.                */
         if(ret_val < 0)
         {
            /* Close the Bluetooth Stack.                               */
            CloseStack();
         }
      }
      else
      {
         /* There was an error while attempting to open the Stack.      */
      }
   }
   else
      ret_val = APPLICATION_ERROR_INVALID_PARAMETERS;

   return(ret_val);
}

   /* The following function is responsible for opening the SS1         */
   /* Bluetooth Protocol Stack.  This function accepts a pre-populated  */
   /* HCI Driver Information structure that contains the HCI Driver     */
   /* Transport Information.  This function returns zero on successful  */
   /* execution and a negative value on all errors.                     */

int OpenStack(HCI_DriverInformation_t *HCI_DriverInformation, BTPS_Initialization_t *BTPS_Initialization)
{
   int                        Result;
   int                        ret_val = 0;
   char                       BluetoothAddress[16];
   Byte_t                     Status;
   BD_ADDR_t                  BD_ADDR;
   L2CA_Link_Connect_Params_t L2CA_Link_Connect_Params;

   /* First check to see if the Stack has already been opened.          */
   if(!BluetoothStackID)
   {
      /* Next, makes sure that the Driver Information passed appears to */
      /* be semi-valid.                                                 */
      if((HCI_DriverInformation) && (BTPS_Initialization))
      {

         /* Initialize BTPSKNRl.                                        */
         BTPS_Init(BTPS_Initialization);

         /* Initialize the Stack                                        */
         Result = BSC_Initialize(HCI_DriverInformation, 0);

         /* Next, check the return value of the initialization to see if*/
         /* it was successful.                                          */
         if(Result > 0)
         {
           HAL_GPIO_WritePin(GPIOC, UI_LED_R_Pin,GPIO_PIN_SET);
           HAL_Delay(1000);
           HAL_GPIO_WritePin(GPIOC, UI_LED_R_Pin,GPIO_PIN_RESET);
            /* The Stack was initialized successfully, inform the user  */
            /* and set the return value of the initialization function  */
            /* to the Bluetooth Stack ID.                               */
            BluetoothStackID = Result;

            /* Initialize the default Secure Simple Pairing parameters. */
            IOCapability     = DEFAULT_IO_CAPABILITY;
            OOBSupport       = FALSE;
            MITMProtection   = DEFAULT_MITM_PROTECTION;

            /* Let's output the Bluetooth Device Address so that the    */
            /* user knows what the Device Address is.                   */
            if(!GAP_Query_Local_BD_ADDR(BluetoothStackID, &BD_ADDR))
            {
               BD_ADDRToStr(BD_ADDR, BluetoothAddress);
            }
            /* Set the Name for the initial use.              */
            GAP_Set_Local_Device_Name (BluetoothStackID,APP_DEMO_NAME);

            /* Go ahead and allow Master/Slave Role Switch.             */
            L2CA_Link_Connect_Params.L2CA_Link_Connect_Request_Config  = cqAllowRoleSwitch;
            L2CA_Link_Connect_Params.L2CA_Link_Connect_Response_Config = csMaintainCurrentRole;

            L2CA_Set_Link_Connection_Configuration(BluetoothStackID, &L2CA_Link_Connect_Params);

            if(HCI_Command_Supported(BluetoothStackID, HCI_SUPPORTED_COMMAND_WRITE_DEFAULT_LINK_POLICY_BIT_NUMBER) > 0)
            {
               HCI_Write_Default_Link_Policy_Settings(BluetoothStackID,
                                                      (HCI_LINK_POLICY_SETTINGS_ENABLE_MASTER_SLAVE_SWITCH|
                                                       HCI_LINK_POLICY_SETTINGS_ENABLE_SNIFF_MODE), &Status);
            }
            /* Delete all Stored Link Keys.                             */
            ASSIGN_BD_ADDR(BD_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

            DeleteLinkKey(BD_ADDR);
         }
         else
         {
            /* The Stack was NOT initialized successfully, inform the   */
            /* user and set the return value of the initialization      */
            /* function to an error.                                    */
            BluetoothStackID = 0;
            ret_val          = UNABLE_TO_INITIALIZE_STACK;
         }
      }
      else
      {
         /* One or more of the necessary parameters are invalid.        */
         ret_val = INVALID_PARAMETERS_ERROR;
      }
   }

   return(ret_val);
}

   /* The following function is responsible for closing the SS1         */
   /* Bluetooth Protocol Stack.  This function requires that the        */
   /* Bluetooth Protocol stack previously have been initialized via the */
   /* OpenStack() function.  This function returns zero on successful   */
   /* execution and a negative value on all errors.                     */
int CloseStack(void)
{
   int ret_val = 0;

   /* First check to see if the Stack has been opened.                  */
   if(BluetoothStackID)
   {
      /* Simply close the Stack                                         */
      BSC_Shutdown(BluetoothStackID);

      /* Free BTPSKRNL allocated memory.                                */
      BTPS_DeInit();

      /* Flag that the Stack is no longer initialized.                  */
      BluetoothStackID = 0;

      /* Flag success to the caller.                                    */
      ret_val          = 0;
   }
   else
   {
      /* A valid Stack ID does not exist, inform to user.               */
      ret_val = UNABLE_TO_INITIALIZE_STACK;
   }

   return(ret_val);
}

   /* The following function is responsible for converting data of type */
   /* BD_ADDR to a string.  The first parameter of this function is the */
   /* BD_ADDR to be converted to a string.  The second parameter of this*/
   /* function is a pointer to the string in which the converted BD_ADDR*/
   /* is to be stored.                                                  */
void BD_ADDRToStr(BD_ADDR_t Board_Address, BoardStr_t BoardStr)
{
   BTPS_SprintF((char *)BoardStr, "0x%02X%02X%02X%02X%02X%02X", Board_Address.BD_ADDR5,
                Board_Address.BD_ADDR4, Board_Address.BD_ADDR3, Board_Address.BD_ADDR2,
                Board_Address.BD_ADDR1, Board_Address.BD_ADDR0);
}


   /* The following function is responsible for sending a number of     */
   /* characters to a remote device to which a connection exists.  The  */
   /* function receives a parameter that indicates the number of byte to*/
   /* be transferred.  This function will return zero on successful     */
   /* execution and a negative value on errors.                         */
int SendData(uint16_t length, unsigned char *buff)
{
  int ret_val = -1; /* -1 means the transmision couldn't be done */

      /* Verify that there is a connection that is established.         */
  if(SerialPortID)
  {
    ret_val = SPP_Data_Write(BluetoothStackID, SerialPortID, length, buff);
  }

  return ret_val;
}

//   /* The following function is responsible for placing the Local       */
//   /* Bluetooth Device into General Discoverablity Mode.  Once in this  */
//   /* mode the Device will respond to Inquiry Scans from other Bluetooth*/
//   /* Devices.  This function requires that a valid Bluetooth Stack ID  */
//   /* exists before running.  This function returns zero on successful  */
//   /* execution and a negative value if an error occurred.              */
//int SetDisc(void)
//{
//   int ret_val = 0;
//
//   /* First, check that a valid Bluetooth Stack ID exists.              */
//   if(BluetoothStackID)
//   {
//      /* A semi-valid Bluetooth Stack ID exists, now attempt to set the */
//      /* attached Devices Discoverablity Mode to General.               */
//      ret_val = GAP_Set_Discoverability_Mode(BluetoothStackID, dmGeneralDiscoverableMode, 0);
//
//      /* Next, check the return value of the GAP Set Discoverability    */
//      /* Mode command for successful execution.                         */
//      if(ret_val)
//      {
//         /* An error occurred while trying to set the Discoverability   */
//         /* Mode of the Device.                                         */
//      }
//   }
//   else
//   {
//      /* No valid Bluetooth Stack ID exists.                            */
//      ret_val = INVALID_STACK_ID_ERROR;
//   }
//
//   return(ret_val);
//}

   /* The following function is responsible for setting the             */
   /* Discoverability Mode of the local device.  This function returns  */
   /* zero on successful execution and a negative value on all errors.  */
//static int SetDiscoverabilityMode(ParameterList_t *TempParam)
//{
//   int                        Result;
//   int                        ret_val;
//   GAP_Discoverability_Mode_t DiscoverabilityMode;
//
//   /* First, check that valid Bluetooth Stack ID exists.                */
//   if(BluetoothStackID)
//   {
//      /* Make sure that all of the parameters required for this function*/
//      /* appear to be at least semi-valid.                              */
//      if((TempParam) && (TempParam->NumberofParameters > 0)
//         && (TempParam->Params[0].intParam >= 0) && (TempParam->Params[0].intParam <= 2))
//      {
//         /* Parameters appear to be valid, map the specified parameters */
//         /* into the API specific parameters.                           */
//         if(TempParam->Params[0].intParam == 1)
//            DiscoverabilityMode = dmLimitedDiscoverableMode;
//         else
//         {
//            if(TempParam->Params[0].intParam == 2)
//               DiscoverabilityMode = dmGeneralDiscoverableMode;
//            else
//               DiscoverabilityMode = dmNonDiscoverableMode;
//         }
//
//         /* Parameters mapped, now set the Discoverability Mode.        */
//         Result = GAP_Set_Discoverability_Mode(BluetoothStackID, DiscoverabilityMode,
//                                               (DiscoverabilityMode == dmLimitedDiscoverableMode)?60:0);
//
//         /* Next, check the return value to see if the command was      */
//         /* issued successfully.                                        */
//         if(Result >= 0)
//         {
//            /* The Mode was changed successfully.                       */
//
//            /* Flag success to the caller.                              */
//            ret_val = 0;
//         }
//         else
//         {
//            /* There was an error setting the Mode.                     */
//
//            /* Flag that an error occurred while submitting the command.*/
//            ret_val = FUNCTION_ERROR;
//         }
//      }
//      else
//      {
//         /* One or more of the necessary parameters is/are invalid.     */
//
//         ret_val = INVALID_PARAMETERS_ERROR;
//      }
//   }
//   else
//   {
//      /* No valid Bluetooth Stack ID exists.                            */
//      ret_val = INVALID_STACK_ID_ERROR;
//   }
//
//   return(ret_val);
//}

int SetDiscoverabilityMode(GAP_Discoverability_Mode_t DiscoverabilityMode)
{
   int Result;
   int ret_val = INVALID_STACK_ID_ERROR;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
         Result = GAP_Set_Discoverability_Mode(BluetoothStackID, DiscoverabilityMode,0);
         ret_val = (Result >= 0)?0:FUNCTION_ERROR;
   }
   return ret_val;
}

   /* The following function is responsible for placing the Local       */
   /* Bluetooth Device into Connectable Mode.  Once in this mode the    */
   /* Device will respond to Page Scans from other Bluetooth Devices.   */
   /* This function requires that a valid Bluetooth Stack ID exists     */
   /* before running.  This function returns zero on success and a      */
   /* negative value if an error occurred.                              */
int SetConnect(void)
{
   int ret_val = 0;

   /* First, check that a valid Bluetooth Stack ID exists.              */
   if(BluetoothStackID)
   {
      /* Attempt to set the attached Device to be Connectable.          */
      ret_val = GAP_Set_Connectability_Mode(BluetoothStackID, cmConnectableMode);

      /* Next, check the return value of the                            */
      /* GAP_Set_Connectability_Mode() function for successful          */
      /* execution.                                                     */
      if(ret_val)
      {
         /* An error occurred while trying to make the Device           */
         /* Connectable.                                                */
      }
   }
   else
   {
      /* No valid Bluetooth Stack ID exists.                            */
      ret_val = INVALID_STACK_ID_ERROR;
   }

   return(ret_val);
}

   /* The following function is responsible for placing the local       */
   /* Bluetooth device into Pairable mode.  Once in this mode the device*/
   /* will response to pairing requests from other Bluetooth devices.   */
   /* This function returns zero on successful execution and a negative */
   /* value on all errors.                                              */
int SetPairable(void)
{
   int Result;
   int ret_val = 0;

   /* First, check that a valid Bluetooth Stack ID exists.              */
   if(BluetoothStackID)
   {
      /* Attempt to set the attached device to be pairable.             */
      Result = GAP_Set_Pairability_Mode(BluetoothStackID, pmPairableMode_EnableSecureSimplePairing);

      /* Next, check the return value of the GAP Set Pairability mode   */
      /* command for successful execution.                              */
      if(!Result)
      {
         /* The device has been set to pairable mode, now register an   */
         /* Authentication Callback to handle the Authentication events */
         /* if required.                                                */
         Result = GAP_Register_Remote_Authentication(BluetoothStackID, GAP_Event_Callback, (unsigned long)0);

         /* Next, check the return value of the GAP Register Remote     */
         /* Authentication command for successful execution.            */
         if(Result)
         {
            /* An error occurred while trying to execute this function. */
            ret_val = Result;
         }
      }
      else
      {
         /* An error occurred while trying to make the device pairable. */
         ret_val = Result;
      }
   }
   else
   {
      /* No valid Bluetooth Stack ID exists.                            */
      ret_val = INVALID_STACK_ID_ERROR;
   }

   return(ret_val);
}

   /* The following function is a utility function that exists to delete*/
   /* the specified Link Key from the Local Bluetooth Device.  If a NULL*/
   /* Bluetooth Device Address is specified, then all Link Keys will be */
   /* deleted.                                                          */
int DeleteLinkKey(BD_ADDR_t BD_ADDR)
{
   int       Result;
   Byte_t    Status_Result;
   Word_t    Num_Keys_Deleted = 0;
   BD_ADDR_t NULL_BD_ADDR;

   Result = HCI_Delete_Stored_Link_Key(BluetoothStackID, BD_ADDR, TRUE, &Status_Result, &Num_Keys_Deleted);

   /* Any stored link keys for the specified address (or all) have been */
   /* deleted from the chip.  Now, let's make sure that our stored Link */
   /* Key Array is in sync with these changes.                          */

   /* First check to see all Link Keys were deleted.                    */
   ASSIGN_BD_ADDR(NULL_BD_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

   if(COMPARE_BD_ADDR(BD_ADDR, NULL_BD_ADDR))
      BTPS_MemInitialize(LinkKeyInfo, 0, sizeof(LinkKeyInfo));
   else
   {
      /* Individual Link Key.  Go ahead and see if know about the entry */
      /* in the list.                                                   */
      for(Result=0;(Result<sizeof(LinkKeyInfo)/sizeof(LinkKeyInfo_t));Result++)
      {
         if(COMPARE_BD_ADDR(BD_ADDR, LinkKeyInfo[Result].BD_ADDR))
         {
            LinkKeyInfo[Result].BD_ADDR = NULL_BD_ADDR;

            break;
         }
      }
   }

   return(Result);
}

   /* The following function is responsible for performing a General    */
   /* Inquiry for discovering Bluetooth Devices.  This function requires*/
   /* that a valid Bluetooth Stack ID exists before running.  This      */
   /* function returns zero is successful or a negative value if there  */
   /* was an error.                                                     */
int Inquiry(ParameterList_t *TempParam)
{
   int Result;
   int ret_val = 0;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
      /* Use the GAP_Perform_Inquiry() function to perform an Inquiry.  */
      /* The Inquiry will last 10 seconds or until MAX_INQUIRY_RESULTS  */
      /* Bluetooth Devices are found.  When the Inquiry Results become  */
      /* available the GAP_Event_Callback is called.                    */
      Result = GAP_Perform_Inquiry(BluetoothStackID, itGeneralInquiry, 0, 0, 10,
                                   MAX_INQUIRY_RESULTS, GAP_Event_Callback, (unsigned long)NULL);

      /* Next, check to see if the GAP_Perform_Inquiry() function was   */
      /* successful.                                                    */
      if(!Result)
      {
         /* The Inquiry appears to have been sent successfully.         */
         /* Processing of the results returned from this command occurs */
         /* within the GAP_Event_Callback() function.                   */

         /* Flag that we have found NO Bluetooth Devices.               */
         NumberofValidResponses = 0;

         ret_val                = 0;
      }
      else
      {
         /* A error occurred while performing the Inquiry.              */

         ret_val = Result;
      }
   }
   else
   {
      /* No valid Bluetooth Stack ID exists.                            */
      ret_val = INVALID_STACK_ID_ERROR;
   }

   return(ret_val);
}

   /* The following function is responsible for setting the             */
   /* Connectability Mode of the local device.  This function returns   */
   /* zero on successful execution and a negative value on all errors.  */
static int SetConnectabilityMode(ParameterList_t *TempParam)
{
   int                       Result;
   int                       ret_val;
   GAP_Connectability_Mode_t ConnectableMode;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
      /* Make sure that all of the parameters required for this function*/
      /* appear to be at least semi-valid.                              */
      if((TempParam) && (TempParam->NumberofParameters > 0) && (TempParam->Params[0].intParam >= 0) && (TempParam->Params[0].intParam <= 1))
      {
         /* Parameters appear to be valid, map the specified parameters */
         /* into the API specific parameters.                           */
         if(TempParam->Params[0].intParam == 0)
            ConnectableMode = cmNonConnectableMode;
         else
            ConnectableMode = cmConnectableMode;

         /* Parameters mapped, now set the Connectabilty Mode.          */
         Result = GAP_Set_Connectability_Mode(BluetoothStackID, ConnectableMode);

         /* Next, check the return value to see if the command was      */
         /* issued successfully.                                        */
         if(Result >= 0)
         {
            /* The Mode was changed successfully.                       */

            /* Flag success to the caller.                              */
            ret_val = 0;
         }
         else
         {
            /* There was an error setting the Mode.                     */

            /* Flag that an error occurred while submitting the command.*/
            ret_val = FUNCTION_ERROR;
         }
      }
      else
      {
         /* One or more of the necessary parameters is/are invalid.     */

         ret_val = INVALID_PARAMETERS_ERROR;
      }
   }
   else
   {
      /* No valid Bluetooth Stack ID exists.                            */
      ret_val = INVALID_STACK_ID_ERROR;
   }

   return(ret_val);
}

   /* The following function is responsible for setting the Pairability */
   /* Mode of the local device.  This function returns zero on          */
   /* successful execution and a negative value on all errors.          */
static int SetPairabilityMode(ParameterList_t *TempParam)
{
   int                     Result;
   int                     ret_val;
   char                   *Mode;
   GAP_Pairability_Mode_t  PairabilityMode;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
      /* Make sure that all of the parameters required for this function*/
      /* appear to be at least semi-valid.                              */
      if((TempParam) && (TempParam->NumberofParameters > 0)
         && (TempParam->Params[0].intParam >= 0) && (TempParam->Params[0].intParam <= 2))
      {
         /* Parameters appear to be valid, map the specified parameters */
         /* into the API specific parameters.                           */
         if(TempParam->Params[0].intParam == 0)
         {
            PairabilityMode = pmNonPairableMode;
            Mode            = "pmNonPairableMode";
         }
         else
         {
            if(TempParam->Params[0].intParam == 1)
            {
               PairabilityMode = pmPairableMode;
               Mode            = "pmPairableMode";
            }
            else
            {
               PairabilityMode = pmPairableMode_EnableSecureSimplePairing;
               Mode            = "pmPairableMode_EnableSecureSimplePairing";
            }
         }

         /* Parameters mapped, now set the Pairability Mode.            */
         Result = GAP_Set_Pairability_Mode(BluetoothStackID, PairabilityMode);

         /* Next, check the return value to see if the command was      */
         /* issued successfully.                                        */
         if(Result >= 0)
         {
            /* The Mode was changed successfully.                       */

            /* If Secure Simple Pairing has been enabled, inform the    */
            /* user of the current Secure Simple Pairing parameters.    */
            if(PairabilityMode == pmPairableMode_EnableSecureSimplePairing)

            /* Flag success to the caller.                              */
            ret_val = 0;
         }
         else
         {
            /* There was an error setting the Mode.                     */

            /* Flag that an error occurred while submitting the command.*/
            ret_val = FUNCTION_ERROR;
         }
      }
      else
      {
         /* One or more of the necessary parameters is/are invalid.     */

         ret_val = INVALID_PARAMETERS_ERROR;
      }
   }
   else
   {
      /* No valid Bluetooth Stack ID exists.                            */
      ret_val = INVALID_STACK_ID_ERROR;
   }

   return(ret_val);
}

   /* The following function is responsible for changing the Secure     */
   /* Simple Pairing Parameters that are exchanged during the Pairing   */
   /* procedure when Secure Simple Pairing (Security Level 4) is used.  */
   /* This function returns zero on successful execution and a negative */
   /* value on all errors.                                              */
int ChangeSimplePairingParameters(ParameterList_t *TempParam)
{
   int ret_val = 0;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
      /* Make sure that all of the parameters required for this function*/
      /* appear to be at least semi-valid.                              */
      if((TempParam) && (TempParam->NumberofParameters >= 2)
         && (TempParam->Params[0].intParam >= 0) && (TempParam->Params[0].intParam <= 3))
      {
         /* Parameters appear to be valid, map the specified parameters */
         /* into the API specific parameters.                           */
         if(TempParam->Params[0].intParam == 0)
            IOCapability = icDisplayOnly;
         else
         {
            if(TempParam->Params[0].intParam == 1)
               IOCapability = icDisplayYesNo;
            else
            {
               if(TempParam->Params[0].intParam == 2)
                  IOCapability = icKeyboardOnly;
               else
                  IOCapability = icNoInputNoOutput;
            }
         }

         /* Finally map the Man in the Middle (MITM) Protection valid.  */
         MITMProtection = (Boolean_t)(TempParam->Params[1].intParam?TRUE:FALSE);

         /* Inform the user of the New I/O Capabilities.                */

         /* Flag success to the caller.                                 */
         ret_val = 0;
      }
      else
      {
         /* One or more of the necessary parameters is/are invalid.     */

         ret_val = INVALID_PARAMETERS_ERROR;
      }
   }
   else
   {
      /* No valid Bluetooth Stack ID exists.                            */
      ret_val = INVALID_STACK_ID_ERROR;
   }

   return(ret_val);
}

   /* The following function is responsible for initiating bonding with */
   /* a remote device.  This function returns zero on successful        */
   /* execution and a negative value on all errors.                     */
int Pair(ParameterList_t *TempParam)
{
   int                Result;
   int                ret_val;
   GAP_Bonding_Type_t BondingType;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
      /* Next, make sure that we are not already connected.             */
      if(!Connected)
      {
         /* There are currently no active connections, make sure that   */
         /* all of the parameters required for this function appear to  */
         /* be at least semi-valid.                                     */
         if((TempParam) && (TempParam->NumberofParameters > 0) && (TempParam->Params[0].intParam) &&
            (NumberofValidResponses) && (TempParam->Params[0].intParam <= NumberofValidResponses) &&
              (!COMPARE_BD_ADDR(InquiryResultList[(TempParam->Params[0].intParam - 1)], NullADDR)))
         {
            /* Check to see if General Bonding was specified.           */
            if(TempParam->NumberofParameters > 1)
               BondingType = TempParam->Params[1].intParam?btGeneral:btDedicated;
            else
               BondingType = btDedicated;

            /* Before we submit the command to the stack, we need to    */
            /* make sure that we clear out any Link Key we have stored  */
            /* for the specified device.                                */
            DeleteLinkKey(InquiryResultList[(TempParam->Params[0].intParam - 1)]);

            /* Attempt to submit the command.                           */
            Result = GAP_Initiate_Bonding(BluetoothStackID,
                                          InquiryResultList[(TempParam->Params[0].intParam - 1)],
                                          BondingType, GAP_Event_Callback, (unsigned long)0);

            /* Check the return value of the submitted command for      */
            /* success.                                                 */
            if(!Result)
            {
               /* Display a message indicating that Bonding was         */
               /* initiated successfully.                               */

               /* Flag success to the caller.                           */
               ret_val = 0;
            }
            else
            {
               /* Display a message indicating that an error occurred   */
               /* while initiating bonding.                             */

               ret_val = FUNCTION_ERROR;
            }
         }
         else
         {
            /* One or more of the necessary parameters is/are invalid.  */

            ret_val = INVALID_PARAMETERS_ERROR;
         }
      }
      else
      {
         /* Display an error to the user describing that Pairing can    */
         /* only occur when we are not connected.                       */

         ret_val = FUNCTION_ERROR;
      }
   }
   else
   {
      /* No valid Bluetooth Stack ID exists.                            */
      ret_val = INVALID_STACK_ID_ERROR;
   }

   return(ret_val);
}

   /* The following function is responsible for ending a previously     */
   /* initiated bonding session with a remote device.  This function    */
   /* returns zero on successful execution and a negative value on all  */
   /* errors.                                                           */
int EndPairing(ParameterList_t *TempParam)
{
   int Result;
   int ret_val;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
      /* Make sure that all of the parameters required for this function*/
      /* appear to be at least semi-valid.                              */
      if((TempParam) && (TempParam->NumberofParameters > 0) && (TempParam->Params[0].intParam)
         && (NumberofValidResponses) && (TempParam->Params[0].intParam <= NumberofValidResponses)
           && (!COMPARE_BD_ADDR(InquiryResultList[(TempParam->Params[0].intParam - 1)], NullADDR)))
      {
         /* Attempt to submit the command.                              */
         Result = GAP_End_Bonding(BluetoothStackID, InquiryResultList[(TempParam->Params[0].intParam - 1)]);

         /* Check the return value of the submitted command for success.*/
         if(!Result)
         {
            /* Flag success to the caller.                              */
            ret_val = 0;

            /* Flag that there is no longer a current Authentication    */
            /* procedure in progress.                                   */
            ASSIGN_BD_ADDR(CurrentRemoteBD_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
         }
         else
         {
            /* Display a message indicating that an error occurred while*/
            /* ending bonding.                                          */

            ret_val = FUNCTION_ERROR;
         }
      }
      else
      {
         ret_val = INVALID_PARAMETERS_ERROR;
      }
   }
   else
   {
      /* No valid Bluetooth Stack ID exists.                            */
      ret_val = INVALID_STACK_ID_ERROR;
   }

   return(ret_val);
}

   /* The following function is responsible for issuing a GAP           */
   /* Authentication Response with a PIN Code value specified via the   */
   /* input parameter.  This function returns zero on successful        */
   /* execution and a negative value on all errors.                     */
int PINCodeResponse(ParameterList_t *TempParam)
{
   int                              Result;
   int                              ret_val;
   PIN_Code_t                       PINCode;
   GAP_Authentication_Information_t GAP_Authentication_Information;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
      /* First, check to see if there is an on-going Pairing operation  */
      /* active.                                                        */
      if(!COMPARE_BD_ADDR(CurrentRemoteBD_ADDR, NullADDR))
      {
         /* Make sure that all of the parameters required for this      */
         /* function appear to be at least semi-valid.                  */
         if((TempParam) && (TempParam->NumberofParameters > 0) &&
            (TempParam->Params[0].strParam) &&
              (BTPS_StringLength(TempParam->Params[0].strParam) > 0) &&
                (BTPS_StringLength(TempParam->Params[0].strParam) <= sizeof(PIN_Code_t)))
         {
            /* Parameters appear to be valid, go ahead and convert the  */
            /* input parameter into a PIN Code.                         */

            /* Initialize the PIN code.                                 */
            ASSIGN_PIN_CODE(PINCode, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

            BTPS_MemCopy(&PINCode, TempParam->Params[0].strParam, BTPS_StringLength(TempParam->Params[0].strParam));

            /* Populate the response structure.                         */
            GAP_Authentication_Information.GAP_Authentication_Type      = atPINCode;
            GAP_Authentication_Information.Authentication_Data_Length   = (Byte_t)(BTPS_StringLength(TempParam->Params[0].strParam));
            GAP_Authentication_Information.Authentication_Data.PIN_Code = PINCode;

            /* Submit the Authentication Response.                      */
            Result = GAP_Authentication_Response(BluetoothStackID, CurrentRemoteBD_ADDR, &GAP_Authentication_Information);

            /* Check the return value for the submitted command for     */
            /* success.                                                 */
            if(!Result)
            {
               /* Operation was successful, inform the user.            */

               /* Flag success to the caller.                           */
               ret_val = 0;
            }
            else
            {
               /* Inform the user that the Authentication Response was  */
               /* not successful.                                       */

               ret_val = FUNCTION_ERROR;
            }

            /* Flag that there is no longer a current Authentication    */
            /* procedure in progress.                                   */
            ASSIGN_BD_ADDR(CurrentRemoteBD_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
         }
         else
         {
            /* One or more of the necessary parameters is/are invalid.  */

            ret_val = INVALID_PARAMETERS_ERROR;
         }
      }
      else
      {
         /* There is not currently an on-going authentication operation,*/
         /* inform the user of this error condition.                    */

         ret_val = FUNCTION_ERROR;
      }
   }
   else
   {
      /* No valid Bluetooth Stack ID exists.                            */
      ret_val = INVALID_STACK_ID_ERROR;
   }

   return(ret_val);
}

   /* The following function is responsible for issuing a GAP           */
   /* Authentication Response with a Pass Key value specified via the   */
   /* input parameter.  This function returns zero on successful        */
   /* execution and a negative value on all errors.                     */
int PassKeyResponse(ParameterList_t *TempParam)
{
   int                              Result;
   int                              ret_val;
   GAP_Authentication_Information_t GAP_Authentication_Information;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
      /* First, check to see if there is an on-going Pairing operation  */
      /* active.                                                        */
      if(!COMPARE_BD_ADDR(CurrentRemoteBD_ADDR, NullADDR))
      {
         /* Make sure that all of the parameters required for this      */
         /* function appear to be at least semi-valid.                  */
         if((TempParam) && (TempParam->NumberofParameters > 0) && (BTPS_StringLength(TempParam->Params[0].strParam) <= GAP_PASSKEY_MAXIMUM_NUMBER_OF_DIGITS))
         {
            /* Parameters appear to be valid, go ahead and populate the */
            /* response structure.                                      */
            GAP_Authentication_Information.GAP_Authentication_Type     = atPassKey;
            GAP_Authentication_Information.Authentication_Data_Length  = (Byte_t)(sizeof(DWord_t));
            GAP_Authentication_Information.Authentication_Data.Passkey = (DWord_t)(TempParam->Params[0].intParam);

            /* Submit the Authentication Response.                      */
            Result = GAP_Authentication_Response(BluetoothStackID, CurrentRemoteBD_ADDR, &GAP_Authentication_Information);

            /* Check the return value for the submitted command for     */
            /* success.                                                 */
            if(!Result)
            {
               /* Operation was successful, inform the user.            */

               /* Flag success to the caller.                           */
               ret_val = 0;
            }
            else
            {
               /* Inform the user that the Authentication Response was  */
               /* not successful.                                       */

               ret_val = FUNCTION_ERROR;
            }

            /* Flag that there is no longer a current Authentication    */
            /* procedure in progress.                                   */
            ASSIGN_BD_ADDR(CurrentRemoteBD_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
         }
         else
         {
            /* One or more of the necessary parameters is/are invalid.  */

            ret_val = INVALID_PARAMETERS_ERROR;
         }
      }
      else
      {
         /* There is not currently an on-going authentication operation,*/
         /* inform the user of this error condition.                    */

         ret_val = FUNCTION_ERROR;
      }
   }
   else
   {
      /* No valid Bluetooth Stack ID exists.                            */
      ret_val = INVALID_STACK_ID_ERROR;
   }

   return(ret_val);
}

   /* The following function is responsible for issuing a GAP           */
   /* Authentication Response with a User Confirmation value specified  */
   /* via the input parameter.  This function returns zero on successful*/
   /* execution and a negative value on all errors.                     */
int UserConfirmationResponse(ParameterList_t *TempParam)
{
   int                              Result;
   int                              ret_val;
   GAP_Authentication_Information_t GAP_Authentication_Information;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
      /* First, check to see if there is an on-going Pairing operation  */
      /* active.                                                        */
      if(!COMPARE_BD_ADDR(CurrentRemoteBD_ADDR, NullADDR))
      {
         /* Make sure that all of the parameters required for this      */
         /* function appear to be at least semi-valid.                  */
         if((TempParam) && (TempParam->NumberofParameters > 0))
         {
            /* Parameters appear to be valid, go ahead and populate the */
            /* response structure.                                      */
            GAP_Authentication_Information.GAP_Authentication_Type          = atUserConfirmation;
            GAP_Authentication_Information.Authentication_Data_Length       = (Byte_t)(sizeof(Byte_t));
            GAP_Authentication_Information.Authentication_Data.Confirmation = (Boolean_t)(TempParam->Params[0].intParam?TRUE:FALSE);

            /* Submit the Authentication Response.                      */
            Result = GAP_Authentication_Response(BluetoothStackID, CurrentRemoteBD_ADDR, &GAP_Authentication_Information);

            /* Check the return value for the submitted command for     */
            /* success.                                                 */
            if(!Result)
            {
               /* Flag success to the caller.                           */
               ret_val = 0;
            }
            else
            {
               /* Inform the user that the Authentication Response was  */
               /* not successful.                                       */

               ret_val = FUNCTION_ERROR;
            }

            /* Flag that there is no longer a current Authentication    */
            /* procedure in progress.                                   */
            ASSIGN_BD_ADDR(CurrentRemoteBD_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
         }
         else
         {
            /* One or more of the necessary parameters is/are invalid.  */
            ret_val = INVALID_PARAMETERS_ERROR;
         }
      }
      else
      {
         /* There is not currently an on-going authentication operation,*/
         /* inform the user of this error condition.                    */

         ret_val = FUNCTION_ERROR;
      }
   }
   else
   {
      /* No valid Bluetooth Stack ID exists.                            */
      ret_val = INVALID_STACK_ID_ERROR;
   }

   return(ret_val);
}

   /* The following function is responsible for querying the Bluetooth  */
   /* Device Address of the local Bluetooth Device.  This function      */
   /* returns zero on successful execution and a negative value on all  */
   /* errors.                                                           */
int GetLocalAddress(ParameterList_t *TempParam)
{
   int       Result;
   int       ret_val;
   BD_ADDR_t BD_ADDR;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
      /* Attempt to submit the command.                                 */
      Result = GAP_Query_Local_BD_ADDR(BluetoothStackID, &BD_ADDR);

      /* Check the return value of the submitted command for success.   */
      if(!Result)
      {
         BD_ADDRToStr(BD_ADDR, Function_BoardStr);
         /* Flag success to the caller.                                 */
         ret_val = 0;
      }
      else
      {
         /* Display a message indicating that an error occurred while   */
         /* attempting to query the Local Device Address.               */

         ret_val = FUNCTION_ERROR;
      }
   }
   else
   {
      /* No valid Bluetooth Stack ID exists.                            */
      ret_val = INVALID_STACK_ID_ERROR;
   }

   return(ret_val);
}

   /* The following function is responsible for setting the name of the */
   /* local Bluetooth Device to a specified name.  This function returns*/
   /* zero on successful execution and a negative value on all errors.  */
int SetLocalName(ParameterList_t *TempParam)
{
   int Result;
   int ret_val = 0;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
      /* Make sure that all of the parameters required for this function*/
      /* appear to be at least semi-valid.                              */
      if((TempParam) && (TempParam->NumberofParameters > 0) && (TempParam->Params[0].strParam))
      {
         /* Attempt to submit the command.                              */
         Result = GAP_Set_Local_Device_Name(BluetoothStackID, TempParam->Params[0].strParam);

         /* Check the return value of the submitted command for success.*/
         if(!Result)
         {
            /* Flag success to the caller.                              */
            ret_val = 0;
         }
         else
         {
            /* Display a message indicating that an error occurred while*/
            /* attempting to set the local Device Name.                 */

            ret_val = FUNCTION_ERROR;
         }
      }
      else
      {
         /* One or more of the necessary parameters is/are invalid.     */
         ret_val = INVALID_PARAMETERS_ERROR;
      }
   }
   else
   {
      /* No valid Bluetooth Stack ID exists.                            */
      ret_val = INVALID_STACK_ID_ERROR;
   }

   return(ret_val);
}

   /* The following function is responsible for querying the Bluetooth  */
   /* Device Name of the local Bluetooth Device.  This function returns */
   /* zero on successful execution and a negative value on all errors.  */
int GetLocalName(ParameterList_t *TempParam)
{
   int   Result;
   int   ret_val;
   char *LocalName;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
      /* Allocate a Buffer to hold the Local Name.                      */
      if((LocalName = BTPS_AllocateMemory(257)) != NULL)
      {
         /* Attempt to submit the command.                              */
         Result = GAP_Query_Local_Device_Name(BluetoothStackID, 257, (char *)LocalName);

         /* Check the return value of the submitted command for success.*/
         if(!Result)
         {
            /* Flag success to the caller.                              */
            ret_val = 0;
         }
         else
         {
            /* Display a message indicating that an error occurred while*/
            /* attempting to query the Local Device Name.               */

            ret_val = FUNCTION_ERROR;
         }

         BTPS_FreeMemory(LocalName);
      }
      else
      {
         ret_val = FUNCTION_ERROR;
      }
   }
   else
   {
      /* No valid Bluetooth Stack ID exists.                            */
      ret_val = INVALID_STACK_ID_ERROR;
   }

   return(ret_val);
}

   /* The following function is responsible for setting the Class of    */
   /* Device of the local Bluetooth Device to a Class Of Device value.  */
   /* This function returns zero on successful execution and a negative */
   /* value on all errors.                                              */
int SetClassOfDevice(ParameterList_t *TempParam)
{
   int               Result;
   int               ret_val;
   Class_of_Device_t Class_of_Device;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
      /* Make sure that all of the parameters required for this function*/
      /* appear to be at least semi-valid.                              */
      if((TempParam) && (TempParam->NumberofParameters > 0))
      {
         /* Attempt to submit the command.                              */
         ASSIGN_CLASS_OF_DEVICE(Class_of_Device, (Byte_t)((TempParam->Params[0].intParam) & 0xFF),
                                (Byte_t)(((TempParam->Params[0].intParam) >> 8) & 0xFF),
                                (Byte_t)(((TempParam->Params[0].intParam) >> 16) & 0xFF));

         Result = GAP_Set_Class_Of_Device(BluetoothStackID, Class_of_Device);

         /* Check the return value of the submitted command for success.*/
         if(!Result)
         {
            /* Flag success to the caller.                              */
            ret_val = 0;
         }
         else
         {
            /* Display a message indicating that an error occurred while*/
            /* attempting to set the local Class of Device.             */
            ret_val = FUNCTION_ERROR;
         }
      }
      else
      {
         /* One or more of the necessary parameters is/are invalid.     */
         ret_val = INVALID_PARAMETERS_ERROR;
      }
   }
   else
   {
      /* No valid Bluetooth Stack ID exists.                            */
      ret_val = INVALID_STACK_ID_ERROR;
   }

   return(ret_val);
}

   /* The following function is responsible for querying the Bluetooth  */
   /* Class of Device of the local Bluetooth Device.  This function     */
   /* returns zero on successful execution and a negative value on all  */
   /* errors.                                                           */
int GetClassOfDevice(ParameterList_t *TempParam)
{
   int               Result;
   int               ret_val;
   Class_of_Device_t Class_of_Device;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
      /* Attempt to submit the command.                                 */
      Result = GAP_Query_Class_Of_Device(BluetoothStackID, &Class_of_Device);

      /* Check the return value of the submitted command for success.   */
      if(!Result)
      {
         /* Flag success to the caller.                                 */
         ret_val = 0;
      }
      else
      {
         /* Display a message indicating that an error occurred while   */
         /* attempting to query the Local Class of Device.              */
         ret_val = FUNCTION_ERROR;
      }
   }
   else
   {
      /* No valid Bluetooth Stack ID exists.                            */
      ret_val = INVALID_STACK_ID_ERROR;
   }

   return(ret_val);
}

   /* The following function is responsible for querying the Bluetooth  */
   /* Device Name of the specified remote Bluetooth Device.  This       */
   /* function returns zero on successful execution and a negative value*/
   /* on all errors.                                                    */
int GetRemoteName(ParameterList_t *TempParam)
{
   int Result;
   int ret_val;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
      /* Make sure that all of the parameters required for this function*/
      /* appear to be at least semi-valid.                              */
      if((TempParam) && (TempParam->NumberofParameters > 0)
         && (TempParam->Params[0].intParam) && (NumberofValidResponses)
           && (TempParam->Params[0].intParam <= NumberofValidResponses)
             && (!COMPARE_BD_ADDR(InquiryResultList[(TempParam->Params[0].intParam - 1)], NullADDR)))
      {
         /* Attempt to submit the command.                              */
         Result = GAP_Query_Remote_Device_Name(BluetoothStackID, InquiryResultList[(TempParam->Params[0].intParam - 1)], GAP_Event_Callback, (unsigned long)0);

         /* Check the return value of the submitted command for success.*/
         if(!Result)
         {
            /* Flag success to the caller.                              */
            ret_val = 0;
         }
         else
         {
            /* Display a message indicating that an error occurred while*/
            /* initiating the Remote Name request.                      */
            ret_val = FUNCTION_ERROR;
         }
      }
      else
      {
         /* One or more of the necessary parameters is/are invalid.     */
         ret_val = INVALID_PARAMETERS_ERROR;
      }
   }
   else
   {
      /* No valid Bluetooth Stack ID exists.                            */
      ret_val = INVALID_STACK_ID_ERROR;
   }

   return(ret_val);
}

   /* The following function is responsible for putting a specified     */
   /* connection into HCI Sniff Mode with passed in parameters.         */
int SniffMode(ParameterList_t *TempParam)
{
   Word_t Sniff_Max_Interval;
   Word_t Sniff_Min_Interval;
   Word_t Sniff_Attempt;
   Word_t Sniff_Timeout;
   Byte_t Status;
   int       Result;
   int       ret_val;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
      /* Make sure that all of the parameters required for this function*/
      /* appear to be at least semi-valid.                              */
      if((TempParam) && (TempParam->NumberofParameters >= 4))
      {
         Sniff_Max_Interval   = (Word_t)MILLISECONDS_TO_BASEBAND_SLOTS(TempParam->Params[0].intParam);
         Sniff_Min_Interval   = (Word_t)MILLISECONDS_TO_BASEBAND_SLOTS(TempParam->Params[1].intParam);
         Sniff_Attempt        = TempParam->Params[2].intParam;
         Sniff_Timeout        = TempParam->Params[3].intParam;

         /* Make sure the Sniff Mode parameters seem semi valid.        */
         if((Sniff_Attempt) && (Sniff_Max_Interval) && (Sniff_Min_Interval) && (Sniff_Min_Interval < Sniff_Max_Interval))
         {
            /* Make sure the connection handle is valid.                */
            if(Connection_Handle)
            {
               /* Now that we have the connection try and go to Sniff.  */
               Result = HCI_Sniff_Mode(BluetoothStackID, Connection_Handle, Sniff_Max_Interval, Sniff_Min_Interval, Sniff_Attempt, Sniff_Timeout, &Status);
               if(!Result)
               {
                  /* Check the Command Status.                          */
                  if(!Status)
                  {
                     /* Return success to the caller.                   */
                     ret_val = 0;
                  }
                  else
                  {
                     ret_val = FUNCTION_ERROR;
                  }
               }
               else
               {
                  ret_val = FUNCTION_ERROR;
               }
            }
            else
            {
               ret_val = FUNCTION_ERROR;
            }
         }
         else
         {
            /* One or more of the necessary parameters is/are invalid.  */
            ret_val = INVALID_PARAMETERS_ERROR;
         }
      }
      else
      {
         /* One or more of the necessary parameters is/are invalid.     */
         ret_val = INVALID_PARAMETERS_ERROR;
      }
   }
   else
   {
      /* No valid Bluetooth Stack ID exists.                            */
      ret_val = INVALID_STACK_ID_ERROR;
   }

   return(ret_val);
}

   /* The following function is responsible for attempting to Exit      */
   /* Sniff Mode for a specified connection.                            */
int ExitSniffMode(ParameterList_t *TempParam)
{
   Byte_t    Status;
   int    Result;
   int    ret_val;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
      /* Make sure that all of the parameters required for this function*/
      /* appear to be at least semi-valid.                              */
      if((Connection_Handle) && (TempParam))
      {
         /* Attempt to Exit Sniff Mode for the Specified Device.        */
         Result = HCI_Exit_Sniff_Mode(BluetoothStackID, Connection_Handle, &Status);
         if(!Result)
         {
            if(!Status)
            {
               /* Flag that HCI_Exit_Sniff_Mode was successfull.        */

               ret_val = 0;
            }
            else
            {
               /* We received a failure status in the Command Status    */
               ret_val = FUNCTION_ERROR;
            }
         }
         else
         {
            /* Failed to get exit sniff mode.                           */

            /* Return function error to the caller.                     */
            ret_val = FUNCTION_ERROR;
         }
      }
      else
      {
         /* One or more of the necessary parameters is/are invalid.     */

         ret_val = INVALID_PARAMETERS_ERROR;
      }
   }
   else
   {
      /* No valid Bluetooth Stack ID exists.                            */
      ret_val = INVALID_STACK_ID_ERROR;
   }

   return(ret_val);
}

   /* The following function is responsible for opening a Serial Port   */
   /* Server on the Local Device.  This function opens the Serial Port  */
   /* Server on the specified RFCOMM Channel.  This function returns    */
   /* zero if successful, or a negative return value if an error        */
   /* occurred.                                                         */
int OpenServer(ParameterList_t *TempParam)
{
  int  ret_val;
  char *ServiceName;

  /* First check to see if a valid Bluetooth Stack ID exists and there is no */
  /* other port opened.          */
  if((BluetoothStackID) || (ServerPortID > 0 ))
  {
    /* Simply attempt to open an Serial Server, on RFCOMM Server Port 1. */
    ret_val = SPP_Open_Server_Port(BluetoothStackID, 1, SPP_Event_Callback, (unsigned long)0);
    /* If the Open was successful, then note the Serial Port Server ID.  */
    if(ret_val > 0)
    {
      /* Note the Serial Port Server ID of the opened Serial Port Server.*/
      ServerPortID = ret_val;
      /* Create a Buffer to hold the Service Name.             */
      if((ServiceName = BTPS_AllocateMemory(64)) != NULL)
      {
        /* The Server was opened successfully, now register a */
        /* SDP Record indicating that an Serial Port Server   */
        /* exists. Do this by first creating a Service Name.  */
        BTPS_SprintF(ServiceName, "Serial Port Server Port %d", (int)(ServerPortID));

        /* Now that a Service Name has been created try to    */
        /* Register the SDP Record.                           */
        ret_val = SPP_Register_Generic_SDP_Record(BluetoothStackID, ServerPortID, ServiceName, &SPPServerSDPHandle);

        /* If there was an error creating the Serial Port     */
        /* Server's SDP Service Record then go ahead an close */
        /* down the server an flag an error.                  */
        if(ret_val < 0)
        {
          SPP_Close_Server_Port(BluetoothStackID, ServerPortID);
          /* Flag that there is no longer an Serial Port     */
          /* Server Open.                                    */
          ServerPortID = 0;
          /* Flag that we are no longer connected.           */
          Connected    = FALSE;
          ret_val      = UNABLE_TO_REGISTER_SERVER;
        }
        /* Free the Service Name buffer.                      */
        BTPS_FreeMemory(ServiceName);
      }
    }else{
      ret_val = UNABLE_TO_REGISTER_SERVER;
    }
  }
  else
  {
    /* A Server is already open, or stack isn't    */
    ret_val = FUNCTION_ERROR;
  }
  return(ret_val);
}

   /* The following function is responsible for closing a Serial Port   */
   /* Server that was previously opened via a successful call to the    */
   /* OpenServer() function.  This function returns zero if successful  */
   /* or a negative return error code if there was an error.            */
int CloseServer(ParameterList_t *TempParam)
{
   int ret_val = 0;

  /* First check to see if a valid Bluetooth Stack ID exists and there is an */
  /* open port .          */
  if((BluetoothStackID) || (ServerPortID == 0 ))
  {
    /* If there is an SDP Service Record associated with the Serial*/
    /* Port Server then we need to remove it from the SDP Database.*/
    if(SPPServerSDPHandle)
    {
      SPP_Un_Register_SDP_Record(BluetoothStackID, SerialPortID, SPPServerSDPHandle);
      /* Flag that there is no longer an SDP Serial Port Server   */
      /* Record.                                                  */
      SPPServerSDPHandle = 0;
    }

    /* Finally close the Serial Port Server.                       */
    ret_val = SPP_Close_Server_Port(BluetoothStackID, ServerPortID);

    if(ret_val < 0)
    {
      ret_val = FUNCTION_ERROR;
    }

    /* Flag that there is no Serial Port Server currently open.    */
    ServerPortID = 0;
    SerialPortID = 0;

    /* Flag that we are no longer connected.                       */
    Connected    = FALSE;
  }
  else
  {
    /* Server or stack isn't opened    */
    ret_val = FUNCTION_ERROR;
  }
   return(ret_val);
}

   /* The following function is responsible for initiating a connection */
   /* with a Remote Serial Port Server.  This function returns zero if  */
   /* successful and a negative value if an error occurred.             */
int OpenRemoteServer(ParameterList_t *TempParam)
{
   int ret_val;
   int Result;

  /* First, check that valid Bluetooth Stack ID exists.                */
  if(BluetoothStackID)
  {
    /* Next, check that we are in client mode.                        */
    if(UI_Mode == UI_MODE_IS_CLIENT)
    {
      /* Next, let's make sure that there is not an Serial Port      */
      /* Client already open.                                        */
      if(!SerialPortID)
      {
        /* Next, let's make sure that the user has specified a      */
        /* Remote Bluetooth Device to open.                         */
        if((TempParam) && (TempParam->NumberofParameters > 1) &&
            (TempParam->Params[0].intParam) && (NumberofValidResponses)
              && (TempParam->Params[0].intParam <= NumberofValidResponses)
                && (!COMPARE_BD_ADDR(InquiryResultList[(TempParam->Params[0].intParam - 1)], NullADDR))
                  && (TempParam->Params[1].intParam))
        {
          /* Now let's attempt to open the Remote Serial Port      */
          /* Server.                                               */
          Result = SPP_Open_Remote_Port(BluetoothStackID,
                    InquiryResultList[(TempParam->Params[0].intParam - 1)],
                      TempParam->Params[1].intParam, SPP_Event_Callback, (unsigned long)0);

          if(Result > 0)
          {
            /* Note the Serial Port Client ID.                    */
            SerialPortID = Result;
            /* Flag success to the caller.                        */
            ret_val = 0;
            /* Save the BD_ADDR so we can Query the Connection    */
            /* handle when receive Connection Confirmation Event. */
            SelectedBD_ADDR = InquiryResultList[(TempParam->Params[0].intParam - 1)];
          }
          else
          {
            /* One or more of the necessary parameters is/are     */
            /* invalid.                                           */
            ret_val = INVALID_PARAMETERS_ERROR;
          }
        }
        else
        {
          /* One or more of the necessary parameters is/are        */
          /* invalid.                                              */
          ret_val = INVALID_PARAMETERS_ERROR;
        }
      }
      else
      {
        ret_val = INVALID_PARAMETERS_ERROR;
      }
    }
    else
    {
      /* One or more of the necessary parameters is/are invalid.     */
      ret_val = INVALID_PARAMETERS_ERROR;
    }
  }
  else
  {
    /* No valid Bluetooth Stack ID exists.                            */
    ret_val = INVALID_STACK_ID_ERROR;
  }

   return(ret_val);
}

   /* The following function is responsible for terminating a connection*/
   /* with a Remote Serial Port Server.  This function returns zero if  */
   /* successful and a negative value if an error occurred.             */
int CloseRemoteServer(ParameterList_t *TempParam)
{
   int ret_val = 0;

   /* First, check that valid Bluetooth Stack ID exists.                */
   if(BluetoothStackID)
   {
      /* Next, check that we are in client mode.                        */
      if(UI_Mode == UI_MODE_IS_CLIENT)
      {
         /* Next, let's make sure that a Remote Serial Port Server is   */
         /* indeed Open.                                                */
         if(SerialPortID)
         {
            /* Simply close the Serial Port Client.                     */
            SPP_Close_Port(BluetoothStackID, SerialPortID);

            /* Flag that there is no longer a Serial Port Client        */
            /* connected.                                               */
            SerialPortID = 0;
            SendInfo.BytesToSend = 0;

            /* Flag that we are no longer connected.                    */
            Connected    = FALSE;

            /* Flag success to the caller.                              */
            ret_val      = 0;
         }
         else
         {
            /* Display an error to the user informing them that no      */
            /* Serial Port Client is open.                              */

            ret_val = INVALID_PARAMETERS_ERROR;
         }
      }
      else
      {
         /* One or more of the necessary parameters is/are invalid.     */
         ret_val = INVALID_PARAMETERS_ERROR;
      }
   }
   else
   {
      /* No valid Bluetooth Stack ID exists.                            */
      ret_val = INVALID_STACK_ID_ERROR;
   }

   return(ret_val);
}

//   /* The following function is responsible for reading data that was   */
//   /* received via an Open SPP port.  The function reads a fixed number */
//   /* of bytes at a time from the SPP Port and displays it. If the call */
//   /* to the SPP_Data_Read() function is successful but no data is      */
//   /* available to read the function displays "No data to read.".  This */
//   /* function requires that a valid Bluetooth Stack ID and Serial Port */
//   /* ID exist before running.  This function returns zero if successful*/
//   /* and a negative value if an error occurred.                        */
//int Read(ParameterList_t *TempParam)
//{
//   int  ret_val;
//   int  Result;
//   char Buffer[32];
//
//   /* First check to see if the parameters required for the execution of*/
//   /* this function appear to be semi-valid.                            */
//   if((BluetoothStackID) && (SerialPortID))
//   {
//      /* Only allow the Read Command if we are not in Loopback or       */
//      /* Display Raw Data Mode.                                         */
//      if((!LoopbackActive) && (!DisplayRawData))
//      {
//         /* The required parameters appear to be semi-valid, send the   */
//         /* command to Read Data from SPP.                              */
//         do
//         {
//            Result = SPP_Data_Read(BluetoothStackID, SerialPortID, (Word_t)(sizeof(Buffer)-1), (Byte_t*)&Buffer);
//
//            /* Next, check the return value to see if the command was   */
//            /* successfully.                                            */
//            if(Result >= 0)
//            {
//               /* Null terminate the read data.                         */
//               Buffer[Result] = 0;
//
//               /* Data was read successfully, the result indicates the  */
//               /* of bytes that were successfully Read.                 */
//
//               if(Result > 0){
//                ret_val = 0;
//               }
//            }
//            else
//            {
//               /* An error occurred while reading from SPP.             */
//
//               ret_val = Result;
//            }
//         } while(Result > 0);
//      }
//      else
//      {
//         /* Simply inform the user that this command is not available in*/
//         /* this mode.                                                  */
//
//         ret_val = INVALID_PARAMETERS_ERROR;
//      }
//   }
//   else
//   {
//      /* One or more of the necessary parameters are invalid.           */
//      ret_val = INVALID_PARAMETERS_ERROR;
//   }
//
//   return(ret_val);
//}

   /* The following function is responsible for querying the current    */
   /* configuration parameters that are used by SPP.  This function will*/
   /* return zero on successful execution and a negative value on       */
   /* errors.                                                           */
int GetConfigParams(ParameterList_t *TempParam)
{
   int                        ret_val;
   SPP_Configuration_Params_t SPPConfigurationParams;

   /* First check to see if the parameters required for the execution of*/
   /* this function appear to be semi-valid.                            */
   if(BluetoothStackID)
   {
      /* Simply query the configuration parameters.                     */
      ret_val = SPP_Get_Configuration_Parameters(BluetoothStackID, &SPPConfigurationParams);

      if(ret_val >= 0)
      {
         /* Parameters have been queried successfully, go ahead and     */
         /* notify the user.                                            */

         /* Flag success.                                               */
         ret_val = 0;
      }
      else
      {
         /* Error querying the current parameters.                      */

         ret_val = FUNCTION_ERROR;
      }
   }
   else
   {
      /* One or more of the necessary parameters are invalid.           */
      ret_val = INVALID_PARAMETERS_ERROR;
   }

   return(ret_val);
}

   /* The following function is responsible for setting the current     */
   /* configuration parameters that are used by SPP.  This function will*/
   /* return zero on successful execution and a negative value on       */
   /* errors.                                                           */
int SetConfigParams(ParameterList_t *TempParam)
{
   int                        ret_val;
   SPP_Configuration_Params_t SPPConfigurationParams;

   /* First check to see if the parameters required for the execution of*/
   /* this function appear to be semi-valid.                            */
   if(BluetoothStackID)
   {
      /* Next check to see if the parameters required for the execution */
      /* of this function appear to be semi-valid.                      */
      if((TempParam) && (TempParam->NumberofParameters > 2))
      {
         /* Parameters have been specified, go ahead and write them to  */
         /* the stack.                                                  */
         SPPConfigurationParams.MaximumFrameSize   = (unsigned int)(TempParam->Params[0].intParam);
         SPPConfigurationParams.TransmitBufferSize = (unsigned int)(TempParam->Params[1].intParam);
         SPPConfigurationParams.ReceiveBufferSize  = (unsigned int)(TempParam->Params[2].intParam);

         ret_val = SPP_Set_Configuration_Parameters(BluetoothStackID, &SPPConfigurationParams);

         if(ret_val >= 0)
         {
            /* Flag success.                                            */
            ret_val = 0;
         }
         else
         {
            /* Error setting the current parameters.                    */
            ret_val = FUNCTION_ERROR;
         }
      }
      else
      {
         ret_val = INVALID_PARAMETERS_ERROR;
      }
   }
   else
   {
      /* One or more of the necessary parameters are invalid.           */
      ret_val = INVALID_PARAMETERS_ERROR;
   }

   return(ret_val);
}

   /* The following function is responsible for querying the current    */
   /* queuing parameters that are used by SPP/RFCOMM (into L2CAP).  This*/
   /* function will return zero on successful execution and a negative  */
   /* value on errors.                                                  */
int GetQueueParams(ParameterList_t *TempParam)
{
   int          ret_val;
   unsigned int MaximumNumberDataPackets;
   unsigned int QueuedDataPacketsThreshold;

   /* First check to see if the parameters required for the execution of*/
   /* this function appear to be semi-valid.                            */
   if(BluetoothStackID)
   {
      /* Simply query the queuing parameters.                           */
      ret_val = SPP_Get_Queuing_Parameters(BluetoothStackID, &MaximumNumberDataPackets, &QueuedDataPacketsThreshold);

      if(ret_val >= 0)
      {
         /* Parameters have been queried successfully, go ahead and     */
         /* notify the user.                                            */
         /* Flag success.                                               */
         ret_val = 0;
      }
      else
      {
         /* Error querying the current parameters.                      */
         ret_val = FUNCTION_ERROR;
      }
   }
   else
   {
      /* One or more of the necessary parameters are invalid.           */
      ret_val = INVALID_PARAMETERS_ERROR;
   }

   return(ret_val);
}

   /* The following function is responsible for setting the current     */
   /* queuing parameters that are used by SPP/RFCOMM (into L2CAP).  This*/
   /* function will return zero on successful execution and a negative  */
   /* value on errors.                                                  */
int SetQueueParams(ParameterList_t *TempParam)
{
   int ret_val;

   /* First check to see if the parameters required for the execution of*/
   /* this function appear to be semi-valid.                            */
   if(BluetoothStackID)
   {
      /* Next check to see if the parameters required for the execution */
      /* of this function appear to be semi-valid.                      */
      if((TempParam) && (TempParam->NumberofParameters > 1))
      {
         /* Parameters have been specified, go ahead and write them to  */
         /* the stack.                                                  */
         ret_val = SPP_Set_Queuing_Parameters(BluetoothStackID,
                                              (unsigned int)(TempParam->Params[0].intParam),
                                              (unsigned int)(TempParam->Params[1].intParam));

         if(ret_val >= 0)
         {
            /* Flag success.                                            */
            ret_val = 0;
         }
         else
         {
            /* Error setting the current parameters.                    */
            ret_val = FUNCTION_ERROR;
         }
      }
      else
      {
         ret_val = INVALID_PARAMETERS_ERROR;
      }
   }
   else
   {
      /* One or more of the necessary parameters are invalid.           */
      ret_val = INVALID_PARAMETERS_ERROR;
   }

   return(ret_val);
}

   /* The following function is responsible for setting the application */
   /* state to support loopback mode.  This function will return zero on*/
   /* successful execution and a negative value on errors.              */
int Loopback(ParameterList_t *TempParam)
{
  int ret_val;

  /* First check to see if the parameters required for the execution of*/
  /* this function appear to be semi-valid.                            */
  if(BluetoothStackID)
  {
    /* Next check to see if the parameters required for the execution */
    /* of this function appear to be semi-valid.                      */
    if((TempParam) && (TempParam->NumberofParameters >= 1) &&
    ((TempParam->Params[0].intParam == 0) ||(TempParam->Params[0].intParam == 1)))
    {
      if(TempParam->Params[0].intParam)
      {
      LoopbackActive = TRUE;
      }
      else
      {
      LoopbackActive = FALSE;
      }
    }
    else
    {
      LoopbackActive = (LoopbackActive?FALSE:TRUE);
    }
    /* Finally output the current Loopback state.                     */
    /* Flag success.                                                  */
    ret_val = 0;
  }
  else
  {
    /* One or more of the necessary parameters are invalid.           */
    ret_val = INVALID_PARAMETERS_ERROR;
  }

  return(ret_val);
}

   /* The following function is responsible for setting the application */
   /* state to support displaying Raw Data.  This function will return  */
   /* zero on successful execution and a negative value on errors.      */
int DisplayRawModeData(ParameterList_t *TempParam)
{
   int ret_val;

   /* First check to see if the parameters required for the execution of*/
   /* this function appear to be semi-valid.                            */
   if(BluetoothStackID)
   {
      /* Check to see if Loopback is active.  If it is then we will not */
      /* process this command (and we will inform the user).            */
      if(!LoopbackActive)
      {
         /* Next check to see if the parameters required for the        */
         /* execution of this function appear to be semi-valid.         */
         if((TempParam) && (TempParam->NumberofParameters > 0))
         {
            if(TempParam->Params->intParam)
               DisplayRawData = TRUE;
            else
               DisplayRawData = FALSE;
         }
         else
         {
            DisplayRawData = (DisplayRawData?FALSE:TRUE);
         }
         /* Flag that the function was successful.                      */
         ret_val = 0;
      }
      else
      {
         ret_val = INVALID_PARAMETERS_ERROR;
      }
   }
   else
   {
      /* One or more of the necessary parameters are invalid.           */
      ret_val = INVALID_PARAMETERS_ERROR;
   }

   return(ret_val);
}

   /* The following function is responsible for setting the application */
   /* state to support Automatically reading all data that is received  */
   /* through SPP.  This function will return zero on successful        */
   /* execution and a negative value on errors.                         */
int AutomaticReadMode(void)
{
   int ret_val = INVALID_PARAMETERS_ERROR;/* Initialized to this value to avoid*/
                                          /* the "else"                        */
   /* Check if */
      /* Check to see if Loopback is active.  If it is then we will not */
      /* process this command (and we will inform the user).            */
   if((BluetoothStackID) && (!LoopbackActive))
   {
     /* Change the automatic reading to opposite state */
     AutomaticReadActive = (AutomaticReadActive?FALSE:TRUE);
     ret_val = 0;
   }
   return ret_val;
}

   /* The following thread is responsible for checking changing the     */
   /* current Baud Rate used to talk to the Radio.                      */
   /* * NOTE * This function ONLY configures the Baud Rate for a TI     */
   /*          Bluetooth chipset.                                       */
int SetBaudRate(ParameterList_t *TempParam)
{
  int ret_val = INVALID_STACK_ID_ERROR; /* Initialized to this value to avoid */
                                         /* the "else"                        */

  /* First check to see if the parameters required for the execution of*/
  /* this function appear to be semi-valid.                            */
  if(BluetoothStackID)
  {
    /* Next, write the command to the device.                      */
    ret_val = VS_Update_UART_Baud_Rate(BluetoothStackID, (DWord_t)TempParam->Params[0].intParam);
  }

  return ret_val;
}


   /*********************************************************************/
   /*                         Event Callbacks                           */
   /*********************************************************************/

   /* The following function is for the GAP Event Receive Data Callback.*/
   /* This function will be called whenever a Callback has been         */
   /* registered for the specified GAP Action that is associated with   */
   /* the Bluetooth Stack.  This function passes to the caller the GAP  */
   /* Event Data of the specified Event and the GAP Event Callback      */
   /* Parameter that was specified when this Callback was installed.    */
   /* The caller is free to use the contents of the GAP Event Data ONLY */
   /* in the context of this callback.  If the caller requires the Data */
   /* for a longer period of time, then the callback function MUST copy */
   /* the data into another Data Buffer.  This function is guaranteed   */
   /* NOT to be invoked more than once simultaneously for the specified */
   /* installed callback (i.e.  this function DOES NOT have be          */
   /* reentrant).  It Needs to be noted however, that if the same       */
   /* Callback is installed more than once, then the callbacks will be  */
   /* called serially.  Because of this, the processing in this function*/
   /* should be as efficient as possible.  It should also be noted that */
   /* this function is called in the Thread Context of a Thread that the*/
   /* User does NOT own.  Therefore, processing in this function should */
   /* be as efficient as possible (this argument holds anyway because   */
   /* other GAP Events will not be processed while this function call is*/
   /* outstanding).                                                     */
   /* * NOTE * This function MUST NOT Block and wait for events that    */
   /*          can only be satisfied by Receiving other GAP Events.  A  */
   /*          Deadlock WILL occur because NO GAP Event Callbacks will  */
   /*          be issued while this function is currently outstanding.  */
void BTPSAPI GAP_Event_Callback(unsigned int BluetoothStackID, GAP_Event_Data_t *GAP_Event_Data, unsigned long CallbackParameter)
{
   int                               Result;
   int                               Index;
   BD_ADDR_t                         NULL_BD_ADDR;
   Boolean_t                         OOB_Data;
   Boolean_t                         MITM;
   GAP_IO_Capability_t               RemoteIOCapability;
   GAP_Inquiry_Event_Data_t         *GAP_Inquiry_Event_Data;
   GAP_Remote_Name_Event_Data_t     *GAP_Remote_Name_Event_Data;
   GAP_Authentication_Information_t  GAP_Authentication_Information;
   ParameterList_t Paramlist;

   /* First, check to see if the required parameters appear to be       */
   /* semi-valid.                                                       */
   if((BluetoothStackID) && (GAP_Event_Data))
   {
      /* The parameters appear to be semi-valid, now check to see what  */
      /* type the incoming event is.                                    */
      switch(GAP_Event_Data->Event_Data_Type)
      {
         case etInquiry_Result:
            /* The GAP event received was of type Inquiry_Result.       */
            GAP_Inquiry_Event_Data = GAP_Event_Data->Event_Data.GAP_Inquiry_Event_Data;

            /* Next, Check to see if the inquiry event data received    */
            /* appears to be semi-valid.                                */
            if(GAP_Inquiry_Event_Data)
            {
               /* Now, check to see if the gap inquiry event data's     */
               /* inquiry data appears to be semi-valid.                */
               if(GAP_Inquiry_Event_Data->GAP_Inquiry_Data)
               {

                  /* Display a list of all the devices found from       */
                  /* performing the inquiry.                            */
                  for(Index=0;(Index<GAP_Inquiry_Event_Data->Number_Devices) && (Index<MAX_INQUIRY_RESULTS);Index++)
                  {
                     InquiryResultList[Index] = GAP_Inquiry_Event_Data->GAP_Inquiry_Data[Index].BD_ADDR;
                     BD_ADDRToStr(GAP_Inquiry_Event_Data->GAP_Inquiry_Data[Index].BD_ADDR, Callback_BoardStr);

                  }

                  NumberofValidResponses = GAP_Inquiry_Event_Data->Number_Devices;
               }
            }
            break;
         case etInquiry_Entry_Result:
            /* Next convert the BD_ADDR to a string.                    */
            BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Inquiry_Entry_Event_Data->BD_ADDR, Callback_BoardStr);

            /* Display this GAP Inquiry Entry Result.                   */
            break;
         case etAuthentication:
            /* An authentication event occurred, determine which type of*/
            /* authentication event occurred.                           */
            switch(GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->GAP_Authentication_Event_Type)
            {
               case atLinkKeyRequest:
                  BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, Callback_BoardStr);

                  /* Setup the authentication information response      */
                  /* structure.                                         */
                  GAP_Authentication_Information.GAP_Authentication_Type    = atLinkKey;
                  GAP_Authentication_Information.Authentication_Data_Length = 0;

                  /* See if we have stored a Link Key for the specified */
                  /* device.                                            */
                  for(Index=0;Index<(sizeof(LinkKeyInfo)/sizeof(LinkKeyInfo_t));Index++)
                  {
                     if(COMPARE_BD_ADDR(LinkKeyInfo[Index].BD_ADDR, GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device))
                     {
                        /* Link Key information stored, go ahead and    */
                        /* respond with the stored Link Key.            */
                        GAP_Authentication_Information.Authentication_Data_Length   = sizeof(Link_Key_t);
                        GAP_Authentication_Information.Authentication_Data.Link_Key = LinkKeyInfo[Index].LinkKey;

                        break;
                     }
                  }

                  /* Submit the authentication response.                */
                  Result = GAP_Authentication_Response(BluetoothStackID, GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, &GAP_Authentication_Information);

                  /* Check the result of the submitted command.         */

                  break;
               case atPINCodeRequest:
                  /* A pin code request event occurred, first display   */
                  /* the BD_ADD of the remote device requesting the pin.*/
                  BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, Callback_BoardStr);

                  /* Note the current Remote BD_ADDR that is requesting */
                  /* the PIN Code.                                      */
                  CurrentRemoteBD_ADDR = GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device;

                  /* Inform the user that they will need to respond with*/
                  /* a PIN Code Response.                               */

                  break;
               case atAuthenticationStatus:
                  /* An authentication status event occurred, display   */
                  /* all relevant information.                          */
                  BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, Callback_BoardStr);

                  /* Flag that there is no longer a current             */
                  /* Authentication procedure in progress.              */
                  ASSIGN_BD_ADDR(CurrentRemoteBD_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
                  break;
               case atLinkKeyCreation:
                  /* A link key creation event occurred, first display  */
                  /* the remote device that caused this event.          */
                  BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, Callback_BoardStr);

                  /* Now store the link Key in either a free location OR*/
                  /* over the old key location.                         */
                  ASSIGN_BD_ADDR(NULL_BD_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

                  for(Index=0,Result=-1;Index<(sizeof(LinkKeyInfo)/sizeof(LinkKeyInfo_t));Index++)
                  {
                     if(COMPARE_BD_ADDR(LinkKeyInfo[Index].BD_ADDR,
                                        GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device))
                        break;
                     else
                     {
                        if((Result == (-1)) && (COMPARE_BD_ADDR(LinkKeyInfo[Index].BD_ADDR, NULL_BD_ADDR)))
                           Result = Index;
                     }
                  }

                  /* If we didn't find a match, see if we found an empty*/
                  /* location.                                          */
                  if(Index == (sizeof(LinkKeyInfo)/sizeof(LinkKeyInfo_t)))
                     Index = Result;

                  /* Check to see if we found a location to store the   */
                  /* Link Key information into.                         */
                  if(Index != (-1))
                  {
                     LinkKeyInfo[Index].BD_ADDR = GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device;
                     LinkKeyInfo[Index].LinkKey = GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Authentication_Event_Data.Link_Key_Info.Link_Key;

                  }else{

                  }

                  break;
               case atIOCapabilityRequest:
                  BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, Callback_BoardStr);

                  /* Setup the Authentication Information Response      */
                  /* structure.                                         */
                  GAP_Authentication_Information.GAP_Authentication_Type                                      = atIOCapabilities;
                  GAP_Authentication_Information.Authentication_Data_Length                                   = sizeof(GAP_IO_Capabilities_t);
                  GAP_Authentication_Information.Authentication_Data.IO_Capabilities.IO_Capability            = (GAP_IO_Capability_t)IOCapability;
                  GAP_Authentication_Information.Authentication_Data.IO_Capabilities.MITM_Protection_Required = MITMProtection;
                  GAP_Authentication_Information.Authentication_Data.IO_Capabilities.OOB_Data_Present         = OOBSupport;

                  /* Submit the Authentication Response.                */
                  Result = GAP_Authentication_Response(BluetoothStackID, GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, &GAP_Authentication_Information);

                  /* Check the result of the submitted command.         */
                  break;
               case atIOCapabilityResponse:
                  BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, Callback_BoardStr);

                  RemoteIOCapability = GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Authentication_Event_Data.IO_Capabilities.IO_Capability;
                  MITM               = (Boolean_t)GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Authentication_Event_Data.IO_Capabilities.MITM_Protection_Required;
                  OOB_Data           = (Boolean_t)GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Authentication_Event_Data.IO_Capabilities.OOB_Data_Present;

                  break;
               case atUserConfirmationRequest:
                  BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, Callback_BoardStr);

                  CurrentRemoteBD_ADDR = GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device;

                  if(IOCapability != icDisplayYesNo)
                  {
                     /* Invoke JUST Works Process...                    */
                     GAP_Authentication_Information.GAP_Authentication_Type          = atUserConfirmation;
                     GAP_Authentication_Information.Authentication_Data_Length       = (Byte_t)sizeof(Byte_t);
                     GAP_Authentication_Information.Authentication_Data.Confirmation = TRUE;

                     /* Submit the Authentication Response.             */

                     Result = GAP_Authentication_Response(BluetoothStackID, GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, &GAP_Authentication_Information);

                     /* Flag that there is no longer a current          */
                     /* Authentication procedure in progress.           */
                     ASSIGN_BD_ADDR(CurrentRemoteBD_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
                  }
                  else
                  {

                     /* Inform the user that they will need to respond  */
                     /* with a PIN Code Response.                       */
                  }
                  break;
               case atPasskeyRequest:
                  BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, Callback_BoardStr);

                  /* Note the current Remote BD_ADDR that is requesting */
                  /* the Passkey.                                       */
                  CurrentRemoteBD_ADDR = GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device;

                  /* Inform the user that they will need to respond with*/
                  /* a Passkey Response.                                */
                  break;
               case atRemoteOutOfBandDataRequest:
                  BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, Callback_BoardStr);

                  /* This application does not support OOB data so      */
                  /* respond with a data length of Zero to force a      */
                  /* negative reply.                                    */
                  GAP_Authentication_Information.GAP_Authentication_Type    = atOutOfBandData;
                  GAP_Authentication_Information.Authentication_Data_Length = 0;

                  Result = GAP_Authentication_Response(BluetoothStackID, GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, &GAP_Authentication_Information);

                  break;
               case atPasskeyNotification:
                  BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, Callback_BoardStr);

                  break;
               case atKeypressNotification:
                  BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, Callback_BoardStr);
                  break;
               default:
                  break;
            }
            break;
         case etRemote_Name_Result:
            /* Bluetooth Stack has responded to a previously issued     */
            /* Remote Name Request that was issued.                     */
            GAP_Remote_Name_Event_Data = GAP_Event_Data->Event_Data.GAP_Remote_Name_Event_Data;
            if(GAP_Remote_Name_Event_Data)
            {
               /* Inform the user of the Result.                        */
               BD_ADDRToStr(GAP_Remote_Name_Event_Data->Remote_Device, Callback_BoardStr);

            }
            break;
         case etEncryption_Change_Result:
            BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Encryption_Mode_Event_Data->Remote_Device, Callback_BoardStr);
            break;
         default:
            /* An unknown/unexpected GAP event was received.            */
            break;
      }
   }
   else
   {
      /* There was an error with one or more of the input parameters.   */
   }

}

   /* The following function is for an SPP Event Callback.  This        */
   /* function will be called whenever a SPP Event occurs that is       */
   /* associated with the Bluetooth Stack.  This function passes to the */
   /* caller the SPP Event Data that occurred and the SPP Event Callback*/
   /* Parameter that was specified when this Callback was installed.    */
   /* The caller is free to use the contents of the SPP Event Data ONLY */
   /* in the context of this callback.  If the caller requires the Data */
   /* for a longer period of time, then the callback function MUST copy */
   /* the data into another Data Buffer.  This function is guaranteed   */
   /* NOT to be invoked more than once simultaneously for the specified */
   /* installed callback (i.e.  this function DOES NOT have be          */
   /* reentrant).  It Needs to be noted however, that if the same       */
   /* Callback is installed more than once, then the callbacks will be  */
   /* called serially.  Because of this, the processing in this function*/
   /* should be as efficient as possible.  It should also be noted that */
   /* this function is called in the Thread Context of a Thread that the*/
   /* User does NOT own.  Therefore, processing in this function should */
   /* be as efficient as possible (this argument holds anyway because   */
   /* another SPP Event will not be processed while this function call  */
   /* is outstanding).                                                  */
   /* * NOTE * This function MUST NOT Block and wait for Events that can*/
   /*          only be satisfied by Receiving SPP Event Packets.  A     */
   /*          Deadlock WILL occur because NO SPP Event Callbacks will  */
   /*          be issued while this function is currently outstanding.  */
void BTPSAPI SPP_Event_Callback(unsigned int BluetoothStackID, SPP_Event_Data_t *SPP_Event_Data, unsigned long CallbackParameter)
{
   int       ret_val = 0;
   int       Index;
   int       Index1;
   int       TempLength;
   Boolean_t _DisplayPrompt = TRUE;
   Boolean_t Done;

   /* **** SEE SPPAPI.H for a list of all possible event types.  This   */
   /* program only services its required events.                   **** */

   /* First, check to see if the required parameters appear to be       */
   /* semi-valid.                                                       */
   if((SPP_Event_Data) && (BluetoothStackID))
   {
      /* The parameters appear to be semi-valid, now check to see what  */
      /* type the incoming event is.                                    */
      switch(SPP_Event_Data->Event_Data_Type)
      {
         case etPort_Open_Request_Indication:
            /* A remote port is requesting a connection.                */
            BD_ADDRToStr(SPP_Event_Data->Event_Data.SPP_Open_Port_Request_Indication_Data->BD_ADDR, Callback_BoardStr);

            /* Accept the connection always.                            */
            SPP_Open_Port_Request_Response(BluetoothStackID, SPP_Event_Data->Event_Data.SPP_Open_Port_Request_Indication_Data->SerialPortID, TRUE);
            break;
         case etPort_Open_Indication:
            /* A remote port is requesting a connection.                */
            BD_ADDRToStr(SPP_Event_Data->Event_Data.SPP_Open_Port_Indication_Data->BD_ADDR, Callback_BoardStr);

            /* Save the Serial Port ID for later use.                   */
            SerialPortID = SPP_Event_Data->Event_Data.SPP_Open_Port_Indication_Data->SerialPortID;

            /* Flag that we are now connected.                          */
            Connected  = TRUE;

            /* Query the connection handle.                             */
            ret_val = GAP_Query_Connection_Handle(BluetoothStackID, SPP_Event_Data->Event_Data.SPP_Open_Port_Indication_Data->BD_ADDR, &Connection_Handle);
            if(ret_val)
            {
               /* Failed to Query the Connection Handle.                */

               ret_val           = 0;
               Connection_Handle = 0;
            }else{
            }

            break;
         case etPort_Open_Confirmation:
            /* A Client Port was opened.  The Status indicates the      */
            /* Status of the Open.                                      */

            /* Check the Status to make sure that an error did not      */
            /* occur.                                                   */
            if(SPP_Event_Data->Event_Data.SPP_Open_Port_Confirmation_Data->PortOpenStatus)
            {
               /* An error occurred while opening the Serial Port so    */
               /* invalidate the Serial Port ID.                        */
               SerialPortID      = 0;
               Connection_Handle = 0;

               /* Flag that we are no longer connected.                 */
               Connected         = FALSE;
            }
            else
            {
               /* Flag that we are now connected.                       */
               Connected  = TRUE;

               /* Query the connection Handle.                          */
               ret_val = GAP_Query_Connection_Handle(BluetoothStackID, SelectedBD_ADDR, &Connection_Handle);
               if(ret_val)
               {
                  /* Failed to Query the Connection Handle.             */

                  ret_val           = 0;
                  Connection_Handle = 0;
               }
            }
            break;
         case etPort_Close_Port_Indication:
            /* The Remote Port was Disconnected.                        */

            /* Invalidate the Serial Port ID.                           */
            if(UI_Mode == UI_MODE_IS_CLIENT)
               SerialPortID = 0;

            Connection_Handle = 0;
            SendInfo.BytesToSend = 0;

            /* Flag that we are no longer connected.                    */
            Connected         = FALSE;
            break;
         case etPort_Status_Indication:
            /* Display Information about the new Port Status.           */
            break;
         case etPort_Data_Indication:
            /* Data was received.  Process it differently based upon the*/
            /* current state of the Loopback Mode.                      */
            if(LoopbackActive)
            {
               /* Initialize Done to false.                             */
               Done = FALSE;

               /* Loop until the write buffer is full or there is not   */
               /* more data to read.                                    */
               while((Done == FALSE) && (BufferFull == FALSE))
               {
                  /* The application state is currently in the loop back*/
                  /* state.  Read as much data as we can read.          */
                  if((TempLength = SPP_Data_Read(BluetoothStackID, SerialPortID, (Word_t)sizeof(Buffer), (Byte_t *)Buffer)) > 0)
                  {
                     /* Adjust the Current Buffer Length by the number  */
                     /* of bytes which were successfully read.          */
                     BufferLength = TempLength;

                     /* Next attempt to write all of the data which is  */
                     /* currently in the buffer.                        */
                     if((TempLength = SPP_Data_Write(BluetoothStackID, SerialPortID, (Word_t)BufferLength, (Byte_t *)Buffer)) < (int)BufferLength)
                     {
                        /* Not all of the data was successfully written */
                        /* or an error occurred, first check to see if  */
                        /* an error occurred.                           */
                        if(TempLength >= 0)
                        {
                           /* An error did not occur therefore the      */
                           /* Transmit Buffer must be full.  Adjust the */
                           /* Buffer and Buffer Length by the amount    */
                           /* which as successfully written.            */
                           if(TempLength)
                           {
                              for(Index=0,Index1=TempLength;Index1<BufferLength;Index++,Index1++)
                                 Buffer[Index] = Buffer[Index1];

                              BufferLength -= TempLength;
                           }

                           /* Set the flag indicating that the SPP Write*/
                           /* Buffer is full.                           */
                           BufferFull = TRUE;
                        }
                        else
                           Done = TRUE;
                     }
                  }
                  else
                     Done = TRUE;
               }

               _DisplayPrompt = FALSE;
            }
            else
            {
               /* If we are operating in Raw Data Display Mode then     */
               /* simply display the data that was give to use.         */
               if((DisplayRawData) || (AutomaticReadActive))
               {
                  /* Initialize Done to false.                          */
                  Done = FALSE;

                  /* Loop through and read all data that is present in  */
                  /* the buffer.                                        */
                  while(!Done)
                  {
                     /* Read as much data as possible.                  */
                     if((TempLength = SPP_Data_Read(BluetoothStackID, SerialPortID, (Word_t)sizeof(Buffer)-1, (Byte_t *)Buffer)) > 0)
                     {
                        /* Now simply display each character that we    */
                        /* have just read.                              */
                        if(DisplayRawData)
                        {
                           Buffer[TempLength] = '\0';
                        }
                     }
                     else
                     {
                        /* Either an error occurred or there is no more */
                        /* data to be read.                             */
                        if(TempLength < 0)
                        {
                           /* Error occurred.                           */
                        }

                        /* Regardless if an error occurred, we are      */
                        /* finished with the current loop.              */
                        Done = TRUE;
                     }
                  }
                  _DisplayPrompt = FALSE;
               }
               else
               {
                  /* Simply inform the user that data has arrived.      */
                  TempLength = SPP_Data_Read(BluetoothStackID, SerialPortID, (Word_t)sizeof(Buffer)-1, (Byte_t *)Buffer);
                  Buffer[TempLength] = 0;
               }
            }
            break;
         case etPort_Send_Port_Information_Indication:
            /* Simply Respond with the information that was sent to us. */
            ret_val = SPP_Respond_Port_Information(BluetoothStackID, SPP_Event_Data->Event_Data.SPP_Send_Port_Information_Indication_Data->SerialPortID, &SPP_Event_Data->Event_Data.SPP_Send_Port_Information_Indication_Data->SPPPortInformation);
            break;
         case etPort_Transmit_Buffer_Empty_Indication:
            /* The transmit buffer is now empty after being full.  Next */
            /* check the current application state.                     */
            if(SendInfo.BytesToSend)
            {
               /* Send the remainder of the last attempt.               */
//               TempLength            = (DataStrLen-SendInfo.BytesSent);
//               SendInfo.BytesSent    = SPP_Data_Write(BluetoothStackID, SerialPortID, TempLength, (unsigned char *)&(DataStr[SendInfo.BytesSent]));
//               if((int)(SendInfo.BytesSent) >= 0)
//               {
//                  if(SendInfo.BytesSent <= SendInfo.BytesToSend)
//                     SendInfo.BytesToSend -= SendInfo.BytesSent;
//                  else
//                     SendInfo.BytesToSend = 0;
//
//                  while(SendInfo.BytesToSend)
//                  {
//                     /* Set the Number of bytes to send in the next     */
//                     /* packet.                                         */
//                     if(SendInfo.BytesToSend > DataStrLen)
//                        TempLength = DataStrLen;
//                     else
//                        TempLength = SendInfo.BytesToSend;
//
//                     SendInfo.BytesSent = SPP_Data_Write(BluetoothStackID, SerialPortID, TempLength, (unsigned char *)DataStr);
//                     if((int)(SendInfo.BytesSent) >= 0)
//                     {
//                        SendInfo.BytesToSend -= SendInfo.BytesSent;
//                        if(SendInfo.BytesSent < TempLength)
//                           break;
//                     }
//                     else
//                     {
//                        SendInfo.BytesToSend = 0;
//                     }
//                  }
//               }
//               else
//               {
//                  SendInfo.BytesToSend = 0;
//               }
            }
            else
            {
               if(LoopbackActive)
            {
               /* Initialize Done to false.                             */
               Done = FALSE;

               /* Loop until the write buffer is full or there is not   */
               /* more data to read.                                    */
               while(Done == FALSE)
               {
                     /* The application state is currently in the loop  */
                     /* back state.  Read as much data as we can read.  */
                  if(((TempLength = SPP_Data_Read(BluetoothStackID, SerialPortID, (Word_t)(sizeof(Buffer)-BufferLength), (Byte_t *)&(Buffer[BufferLength]))) > 0) || (BufferLength > 0))
                  {
                        /* Adjust the Current Buffer Length by the      */
                        /* number of bytes which were successfully read.*/
                     if(TempLength > 0)
                        {
                        BufferLength += TempLength;
                        }

                        /* Next attempt to write all of the data which  */
                        /* is currently in the buffer.                  */
                     if((TempLength = SPP_Data_Write(BluetoothStackID, SerialPortID, (Word_t)BufferLength, (Byte_t *)Buffer)) < (int)BufferLength)
                     {
                           /* Not all of the data was successfully      */
                           /* written or an error occurred, first check */
                           /* to see if an error occurred.              */
                        if(TempLength >= 0)
                        {
                           /* An error did not occur therefore the      */
                              /* Transmit Buffer must be full.  Adjust  */
                              /* the Buffer and Buffer Length by the    */
                              /* amount which was successfully written. */
                           if(TempLength)
                           {
                              for(Index=0,Index1=TempLength;Index1<BufferLength;Index++,Index1++)
                                 Buffer[Index] = Buffer[Index1];

                              BufferLength -= TempLength;
                           }
                           else
                              Done = TRUE;

                              /* Set the flag indicating that the SPP   */
                              /* Write Buffer is full.                  */
                           BufferFull = TRUE;
                        }
                        else
                           Done = TRUE;
                     }
                     else
                     {
                        BufferLength = 0;

                        BufferFull   = FALSE;
                     }
                  }
                  else
                     Done = TRUE;
               }
               }
               else
               {
                  /* Only print the event indication to the user if we  */
                  /* are NOT operating in Raw Data Display Mode.        */
               }
            }

            _DisplayPrompt = FALSE;
            break;
         default:
            /* An unknown/unexpected SPP event was received.            */
            break;
      }

      /* Check the return value of any function that might have been    */
      /* executed in the callback.                                      */
      if(ret_val)
      {
         /* An error occurred, so output an error message.              */
      }
   }
   else
   {
      /* There was an error with one or more of the input parameters.   */
   }
}

   /* The following function is responsible for processing HCI Mode     */
   /* change events.                                                    */
void BTPSAPI HCI_Event_Callback(unsigned int BluetoothStackID, HCI_Event_Data_t *HCI_Event_Data, unsigned long CallbackParameter)
{
   char *Mode;

   /* Make sure that the input parameters that were passed to us are    */
   /* semi-valid.                                                       */
   if((BluetoothStackID) && (HCI_Event_Data))
   {
      /* Process the Event Data.                                        */
      switch(HCI_Event_Data->Event_Data_Type)
      {
         case etMode_Change_Event:
            if(HCI_Event_Data->Event_Data.HCI_Mode_Change_Event_Data)
            {
               switch(HCI_Event_Data->Event_Data.HCI_Mode_Change_Event_Data->Current_Mode)
               {
                  case HCI_CURRENT_MODE_HOLD_MODE:
                     Mode = "Hold";
                     break;
                  case HCI_CURRENT_MODE_SNIFF_MODE:
                     Mode = "Sniff";
                     break;
                  case HCI_CURRENT_MODE_PARK_MODE:
                     Mode = "Park";
                     break;
                  case HCI_CURRENT_MODE_ACTIVE_MODE:
                  default:
                     Mode = "Active";
                     break;
               }
            }
            break;
         default:
            break;
      }
   }
}

