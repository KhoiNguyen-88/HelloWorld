/*<RBHead>
 *************************************************************************
 *                                                                       *
 *                      ROBERT BOSCH GMBH                                *
 *                          STUTTGART                                    *
 *                                                                       *
 *          Alle Rechte vorbehalten - All rights reserved                *
 *                                                                       *
 *************************************************************************

 *************************************************************************
 *    Administrative Information (automatically filled in by eASEE)      *
 *************************************************************************
 *
 * $Filename__:$
 *
 * $Author____:$
 *
 * $Function__:$
 *
 *************************************************************************
 * $Domain____:$
 * $User______:$
 * $Date______:$
 * $Class_____:$
 * $Name______:$
 * $Variant___:$
 * $Revision__:$
 * $Type______:$
 * $State_____:$
 * $Generated_:$
 *************************************************************************
 *
 * $UniqueName:$
 * $Component_:$
 *
 *
 *************************************************************************
</RBHead>*/

/*-------------------------------------------------------------------------------------------------------------------*
 * Includes
 *-------------------------------------------------------------------------------------------------------------------*/
/*!
 * \rba_comp rba_I2C
 *  @{
 */
#include "rba_I2C.h"

#if(RBA_I2C_CFG_MODULE_ACTIVE == STD_ON)

#if (RBA_I2C_CFG_CHECK_AUTOSAR_VERSION == STD_ON)
    #if (!defined(STD_TYPES_AR_RELEASE_MAJOR_VERSION) || !defined(STD_TYPES_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR version numbers of Std_Types.h undefined."
    #endif
    #if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != 4) \
        || ((STD_TYPES_AR_RELEASE_MINOR_VERSION != 0) && (STD_TYPES_AR_RELEASE_MINOR_VERSION != 2)))
        #error "AUTOSAR version numbers of this header and Std_Types.h are different."
    #endif
#endif

#include "rba_I2C_Prv.h"

#include "rba_I2C_Cfg_SchM.h"

#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif

#ifndef RBA_I2C_CFG_MAX_CONFIGURED_CHANNELS
#error "RBA_I2C_CFG_MAX_CONFIGURED_CHANNELS undefined"
#endif

#ifndef RBA_I2C_CFG_QUEUE_LENGTH
#error "RBA_I2C_CFG_QUEUE_LENGTH undefined"
#endif
#define  RBA_I2C_START_SEC_VAR_CLEARED_UNSPECIFIED
#include RBA_I2C_CFG_MEMMAP_H



/* I2C-FIFO variables------------------------------------------------*/

/* I2C sequence storage FIFO Queue for low priority */
rba_I2c_SeqDef_tst            *rba_I2c_Prv_adrSequenceQueue_ast[ RBA_I2C_CFG_MAX_CONFIGURED_CHANNELS ][ RBA_I2C_CFG_QUEUE_LENGTH ];

/* I2C sequence storage FIFO Queue for High Priority */
rba_I2c_SeqDef_tst            *rba_I2c_Prv_adrSequenceHiPriQueue_ast[ RBA_I2C_CFG_MAX_CONFIGURED_CHANNELS ][ RBA_I2C_CFG_QUEUE_LENGTH ];


/* I2C Low priority FIFO Queue execution status counter array  */
rba_I2c_QueueDynamic_tst      rba_I2c_Prv_posnQueueDynamic_ast[ RBA_I2C_CFG_MAX_CONFIGURED_CHANNELS ];

/* I2C High priority FIFO Queue execution status counter array  */
rba_I2c_QueueDynamic_tst      rba_I2c_Prv_posnQueueHiPriDynamic_ast[ RBA_I2C_CFG_MAX_CONFIGURED_CHANNELS ];

/* I2C FIFO execution status variable */
rba_I2c_QueueStatusType_ten   rba_I2c_Prv_stI2c_ChanRunStatus_en[ RBA_I2C_CFG_MAX_CONFIGURED_CHANNELS ] ;

/* I2C FIFO Queue status (Active, Disable) */
uint8                         rba_I2c_Prv_stQueueState_au8[ RBA_I2C_CFG_MAX_CONFIGURED_CHANNELS ];

/* I2C Number of ticks per clock*/
uint32                        rba_I2c_Clk = 0uL;

/*I2C-Sequence varialbe------------------------------------------------*/

/* I2C Sequece current status varialbe (idle or busy) */
rba_I2c_xQueueSeqStatus_ten   rba_I2c_Prv_stSequenceStatus_au8[ RBA_I2C_CFG_QUEUE_LENGTH ];

/*I2C-Postbuild configuration loacal pointer varialbe-------------------*/

/* brief Hold pointer to current configuration here.*/
const rba_I2c_ConfigType      *rba_I2c_prv_cfgConfigType_pcst = NULL_PTR;

#define  RBA_I2C_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include RBA_I2C_CFG_MEMMAP_H

#define  RBA_I2C_START_SEC_VAR_INIT_8
#include RBA_I2C_CFG_MEMMAP_H


/*I2C-interface status varialbe------------------------------------------*/

/* I2c operation mode */
boolean                    rba_I2c_Prv_stInterruptMode_b = FALSE;

/* I2C interrutp Flag*/
boolean                    rba_I2c_stInitialized_b = FALSE;    /* I2C Interrupt status variable*/

#define  RBA_I2C_STOP_SEC_VAR_INIT_8
#include RBA_I2C_CFG_MEMMAP_H

//- Functions -----------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
//! \brief Initialize driver specific stuff.
//! Initializes and handles the pre-condition settings for I2C communication. I2C protocol can support
//! more than one I2C module (hardware), hence init process shall vary based on the number of Configured I2C modules.
//! This function interface initialize the software lever setting, for the hardware level initialize call to the
//! controller depended function ex(IFX or JDP).
//!
//! <b>Sync/Async:</b> Synchronous
//!
//! <b>Reentrancy:</b> Non-reentrant
//!
//! \param configuration_pcst: Pointer to post-build configuration.
//!
//! \return Void
//!
//! \pre none
//!
///////////////////////////////////////////////////////////////////////////////

#define  RBA_I2C_START_SEC_CODE
#include RBA_I2C_CFG_MEMMAP_H

void rba_I2c_Init(const rba_I2c_ConfigType *configuration_pcst)
{
    /* Local variable*/
    uint8 unitId_Prv_u8 = 0u;        /* channel counter */
    uint8 seqId_Prv_u8 = 0u;         /* sequence ID */

    /* Check whether the I2C channel already initialized */
    if (rba_I2c_stInitialized_b == TRUE)
    {
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
        /* Reinitialize the I2C module condition not allowed - call to DET error */
        (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_INIT, RBA_I2C_DET_ERR_ALREADY_INITIALIZED_STATUS);
#endif

    }
    /* Check whether the I2C configuration pointer is NULL pointer */
    else if (configuration_pcst == NULL_PTR)
    {
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
        /* Invalid pointer to configuration. */
        (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_INIT, RBA_I2C_DET_ERR_INVALID_CONF_POINTER);
#endif

    }

    else
    {

        /* Load the configuration pointer to Private variable */
        rba_I2c_prv_cfgConfigType_pcst = configuration_pcst;

        /* Initialize queue and make alive. */
        for (unitId_Prv_u8 = 0u;  unitId_Prv_u8 < RBA_I2C_CFG_MAX_CONFIGURED_CHANNELS; unitId_Prv_u8++)
        {
            /* Set queue to inactive state. */
            rba_I2c_Prv_stQueueState_au8[ unitId_Prv_u8 ] = RBA_I2C_PRV_QUEUE_INACTIVE;

            /* Initialization of queue. */
            rba_I2c_Prv_posnQueueDynamic_ast[ unitId_Prv_u8 ].idxFreePos_u8 = 0u;         /* Initial frees positin set to zero. */
            rba_I2c_Prv_posnQueueDynamic_ast[ unitId_Prv_u8 ].idxSendPos_u8 = 0u;         /* Initial send positin set to zero. */
            rba_I2c_Prv_posnQueueDynamic_ast[ unitId_Prv_u8 ].numEntries_u8 = 0u;         /* Initial number of entries set to zero. */

            rba_I2c_Prv_posnQueueHiPriDynamic_ast[ unitId_Prv_u8 ].idxFreePos_u8 = 0u;         /* Initial frees positin set to zero. */
            rba_I2c_Prv_posnQueueHiPriDynamic_ast[ unitId_Prv_u8 ].idxSendPos_u8 = 0u;         /* Initial send positin set to zero. */
            rba_I2c_Prv_posnQueueHiPriDynamic_ast[ unitId_Prv_u8 ].numEntries_u8 = 0u;         /* Initial number of entries set to zero. */

             /* set all the I2C FIFO exectuion status to not running */
            rba_I2c_Prv_stI2c_ChanRunStatus_en[unitId_Prv_u8] = RBA_I2C_CHANNEL_NOT_RUNNING;

            /* Set queue to Active state. */
            rba_I2c_Prv_stQueueState_au8[ unitId_Prv_u8 ]  = RBA_I2C_PRV_QUEUE_ACTIVE;

        }

        /* Check whether the I2C haraware channel Zero available and configured */
#ifdef RBA_I2C_CFG_CHANNEL_CONF_AVAILABLE_0
        /*MR12 RULE 14.3 VIOLATION: The warning is uncritical and non-removable since the value of conditional operator depends on configuration or calibration parameters which contain unchanged value. */
        if(RBA_I2C_CFG_CHANNEL_CONF_AVAILABLE_0 == 1u)
        {
            /* initialize the hardware I2C channel Zero*/
            rba_I2c_Prv_Hw_Init( RBA_I2C_CHANNEL_ZERO );
        }

#endif

        /* Check whether the I2C haraware channel One available and configured */
#ifdef RBA_I2C_CFG_CHANNEL_CONF_AVAILABLE_1
        /*MR12 RULE 14.3 VIOLATION: The warning is uncritical and non-removable since the value of conditional operator depends on configuration or calibration parameters which contain unchanged value. */
        if(RBA_I2C_CFG_CHANNEL_CONF_AVAILABLE_1 == 1u )
        {
            /* initialize the hardware I2C channel One*/
            rba_I2c_Prv_Hw_Init( RBA_I2C_CHANNEL_ONE );
        }

#endif

        /* Initial condition set to the all the available Sequence execution status to IDLE*/
        for(seqId_Prv_u8 = 0u;  seqId_Prv_u8 < rba_I2c_MaxSequenceCount_s; seqId_Prv_u8++)
        {
            /* set all the I2C configured sequence status to IDLE */
            rba_I2c_Prv_stSequenceStatus_au8[ seqId_Prv_u8 ] = rba_I2c_QueueSequence_Idle_e;
        }

        /* Set the I2C-Initialize status variable to TRUE */
        rba_I2c_stInitialized_b = TRUE;
    }
}

// ***********************************************************************************************************************
// * \brief  part of the I2C initialization.
// *
// * This part prepares the usage of the interrupt driven command queue handler and enables it.
// * It shall be called after interrupts are enabled.
//!
//! <b>Sync/Async:</b> Synchronous
//!
//! <b>Reentrancy:</b> Non-reentrant
//!
//! \param void.
//!
//! \return Void
//!
//! \pre none
//!
// /////////////////////////////////////////////////////////////////////////////
void rba_I2c_End_Init(void)
{
    /* Set the Interrupt mode status variable to Activated */
    rba_I2c_Prv_stInterruptMode_b = TRUE;

}

///////////////////////////////////////////////////////////////////////////////
//! \brief Function interface
//!This interface will receive the Read or  Write request from the I2C interface request module. And sends to the I2C slave hardware.
//!Every request will be first stored in to the Queue and  with Each sequence Queue count will be incremented,
//!Depending on the configured module the corresponding message sequence will be loaded in to the appropriate
//!queue in a Circular queue manner.
//!
//!
//! \param configuration_pcst: Pointer to I2C sequence definition.
//!
//! \return E_OK: Sequence definition has been entered successfully into the queue.
//!         E_NOT_OK: for the three state error will be generate
//!                         1.I2C_QUEUE_INACTIVE
//!                         2.I2C_SEQ_NOT_IDLE
//!                         3.I2C_QUEUE_SET_QUEUE_FULL
//!
//! \pre none
//!
//! \post Enter a new I2C sequence into the queue.
//!
///////////////////////////////////////////////////////////////////////////////
Std_ReturnType  rba_I2c_TxRx_Sequence( rba_I2c_SeqDef_tst * rba_I2c_SeqDef_st )
{
    /* Local Variable */
    uint8                      xSequenceId_u8 = 0u;                                  /* Sequence ID. */
    uint8                      xSlaveId_u8 = 0u;                                     /* Slave ID. */
    uint8                      xHwId_Prv_u8 = 0u;                                    /* sequence interface Hardware ID. */
    uint8                      xLogicChannelId_Prv_u8 = 0u;                          /* Hardware index. */
    Std_ReturnType             stRet = E_NOT_OK;                                     /* Return variable. */
    rba_I2c_xComType_ten       xComType_en;                                          /* Sequence Communication type */
    rba_I2c_PriorityType_ten   xSeqPrio_en = RBA_I2C_SEQ_LOW_PRIORITY;               /* Sequence Priority. */

    /* Check whether the I2C channel initialized */
    if(rba_I2c_stInitialized_b != TRUE)
    {
        /* signal validation is failure set the sequence to Failed */
        rba_I2c_SeqDef_st->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;

        /*status variable update to signal initialization is failure */
        rba_I2c_SeqDef_st->rba_I2c_xComErr_u8 = RBA_I2C_ERR_SEQ_NOT_INITIALIZED_STATUS;

        /* Generate the DET error for the I2C-init is not completed */
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError( RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_TXRX_SEQUENCE, RBA_I2C_DET_ERR_NOT_INITIALIZED_STATUS );
#endif
        /* function return with status set to failure */
        stRet = E_NOT_OK;
    }
    else if( rba_I2c_SeqDef_st == NULL_PTR )// check whether the input parameter is valid
    {
        /* Generate the DET error for the invalid sequence address */
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError( RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_TXRX_SEQUENCE, RBA_I2C_DET_ERR_INVALID_SEQ_POINTER );
#endif
        /* function return with status set to failure */
        stRet = E_NOT_OK;
    }
    else if( rba_I2c_SeqDef_st->rba_I2c_xSequenceCfgId_u8 >= rba_I2c_MaxSequenceCount_s )// check whether the input parameter is valid
    {

        /* set the sequence execution status to Failed */
        rba_I2c_SeqDef_st->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;

        /* Set error status to the sequence as Signal not configured */
        rba_I2c_SeqDef_st->rba_I2c_xComErr_u8 = RBA_I2C_ERR_SEQ_SIGNAL_NOT_CONFIGURED;

        /* Generate the DET error for signal is not valid*/
#if ( RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON )
        (void)Det_ReportError( RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_TXRX_SEQUENCE, RBA_I2C_DET_ERR_SIGNAL_NOT_CONFIGURED );
#endif
        /* function retun with status set to failure */
        stRet = E_NOT_OK;

    }
    else if( rba_I2c_SeqDef_st->rba_I2c_xComErr_u8 != 0u )
    {

        /* signal Communication type is failure set the sequence to Fail */
        rba_I2c_SeqDef_st->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;

        /*Set the sequence status to failed */
        rba_I2c_SeqDef_st->rba_I2c_xComErr_u8 = RBA_I2C_ERR_SEQ_COMM_ERROR;

        /* Generate the DET error for Comm_error is not valid*/
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError( RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_TXRX_SEQUENCE, RBA_I2C_DET_ERR_COMM_ERROR );
#endif

        /* function return with status set to failure */
        stRet = E_NOT_OK;

    }
    else if(  rba_I2c_SeqDef_st->rba_I2c_xSeqStatus_en  != rba_I2c_SeqIdle_e  )
    {

        /* signal validation is failure set the sequence to Fail */
        rba_I2c_SeqDef_st->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;

        /*status variable update to signal validation is failure */
        rba_I2c_SeqDef_st->rba_I2c_xComErr_u8 = (uint8)RBA_I2C_ERR_SEQ_NOT_IDLE;

        /* Generate the DET error for sequence is not new*/
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_TXRX_SEQUENCE, RBA_I2C_DET_ERR_SEQ_NOT_IDLE);
#endif

        /* function return with status set to failure */
        stRet = E_NOT_OK;

    }
    else
    {

        /* Read the Signal ID from the input sequence */
        xSequenceId_u8 = rba_I2c_SeqDef_st->rba_I2c_xSequenceCfgId_u8;

        /* Read the Communication of the sequence input sequence */
        xComType_en = rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[xSequenceId_u8]->seqComType_en;

            /* Check the Transmit data pointer */
            if( ( xComType_en == rba_I2c_Tx_e ) && ( rba_I2c_SeqDef_st->rba_I2c_dataTx_pu8 == NULL_PTR ) )
            {
                    /* signal validation is failure set the sequence to Fail */
                    rba_I2c_SeqDef_st->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;

                    /*status variable update to signal validation is failure */
                    rba_I2c_SeqDef_st->rba_I2c_xComErr_u8 = RBA_I2C_ERR_SEQ_TX_POINTER_ERROR;

                    /* Generate the DET error for sequence Transmit data pointer is not valid */
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                    (void)Det_ReportError( RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_TXRX_SEQUENCE, RBA_I2C_DET_ERR_TX_DATA_POINTER );
#endif

                    /* function return with status set to failure */
                    stRet = E_NOT_OK;

            }

            /* validate the Receive data pointer in the sequence */
            else if( ( xComType_en == rba_I2c_Rx_e ) && ( rba_I2c_SeqDef_st->rba_I2c_dataRx_pu8 == NULL_PTR) )
            {
                    /* signal validation is failure set the sequence to Fail */
                    rba_I2c_SeqDef_st->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;

                    /*status variable update to signal validation is failure */
                    rba_I2c_SeqDef_st->rba_I2c_xComErr_u8 = RBA_I2C_ERR_SEQ_RX_POINTER_ERROR;

                    /* Generate the DET for Sequence Receive data pointer is error */
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                    (void)Det_ReportError( RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_TXRX_SEQUENCE, RBA_I2C_DET_ERR_RX_DATA_POINTER );
#endif
                    /* function return with status set to failure */
                    stRet = E_NOT_OK;



            }
            else if( ( xComType_en == rba_I2c_CombinedTxRx_e ) && ( ( rba_I2c_SeqDef_st->rba_I2c_dataTx_pu8 == NULL_PTR ) || ( rba_I2c_SeqDef_st->rba_I2c_dataRx_pu8 == NULL_PTR) ) )
            {

                    /* signal validation is failure set the sequence to Fail */
                    rba_I2c_SeqDef_st->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;


                    if( rba_I2c_SeqDef_st->rba_I2c_dataTx_pu8 == NULL_PTR )
                    {
                        /*status variable update to signal validation is failure */
                        rba_I2c_SeqDef_st->rba_I2c_xComErr_u8 = RBA_I2C_ERR_SEQ_TX_POINTER_ERROR;

                        /* Generate the DET error for sequence Transmit data pointer is not valid */
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                        (void)Det_ReportError( RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_TXRX_SEQUENCE, RBA_I2C_DET_ERR_TX_DATA_POINTER );
#endif

                    }
                    else
                    {
                        /*status variable update to signal validation is failure */
                        rba_I2c_SeqDef_st->rba_I2c_xComErr_u8 = RBA_I2C_ERR_SEQ_RX_POINTER_ERROR;

                        /* Generate the DET for Sequence Receive data pointer is error */
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                        (void)Det_ReportError( RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_TXRX_SEQUENCE, RBA_I2C_DET_ERR_RX_DATA_POINTER );
#endif

                    }

                    /* function return with status set to failure */
                    stRet = E_NOT_OK;


            }
            else
            {
                /* Read the sequence priority info from sequence configured dataset */
                xSeqPrio_en = rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[xSequenceId_u8]->sequPrio_en;

                /* Read the Slave ID info from sequence configured dataset*/
                xSlaveId_u8 = rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[xSequenceId_u8]->SlaveId;

                /* get the I2C Hardware channel number info from sequence configured dataset */
                xHwId_Prv_u8 = rba_I2c_prv_cfgConfigType_pcst->slaveConfig_pcst[ xSlaveId_u8 ]->hwConnectID_u8;

                /* convert the Hardware-channel number to logical-ID in configuration dataset. */
                xLogicChannelId_Prv_u8 = rba_I2c_prv_cfgConfigType_pcst->hwUnitConfig_pcst[ xHwId_Prv_u8 ]->LogicHwId_u8;

                /* check whether the same sequence is running the Queue */
                if(  rba_I2c_Prv_stSequenceStatus_au8[xSequenceId_u8] != rba_I2c_QueueSequence_Idle_e  )
                {

                    /* signal validation is failure set the sequence to Fail */
                    rba_I2c_SeqDef_st->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;

                    /*status variable update to signal validation is failure */
                    rba_I2c_SeqDef_st->rba_I2c_xComErr_u8 = RBA_I2C_ERR_SEQ_BUSY;

                    /* Generate the DET error for the I2C-init is not completed */
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                    (void)Det_ReportError( RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_TXRX_SEQUENCE, RBA_I2C_DET_ERR_QUEUE_SEQ_NOT_IDLE );
#endif

                    /* function return with status set to failure */
                    stRet = E_NOT_OK;


                }
                else
                {


                        /* Check whether the sequence is LOW priority*/
                        if(xSeqPrio_en == RBA_I2C_SEQ_LOW_PRIORITY)
                        {

                           /* check if the number of entries is below the maximum size of queue length */
                            if( ( rba_I2c_Prv_posnQueueDynamic_ast[ xLogicChannelId_Prv_u8 ].numEntries_u8 ) < (RBA_I2C_CFG_QUEUE_LENGTH) )
                            {

                                SchM_Enter_rba_I2c_Driver();

                                /* Change the sequence state to Start condition */
                                rba_I2c_SeqDef_st ->rba_I2c_xSeqStatus_en = rba_I2c_Start_e;

                                /* Set the sequence state to Busy*/
                                rba_I2c_Prv_stSequenceStatus_au8[xSequenceId_u8] = rba_I2c_QueueSequence_Busy_e;

                                /* Add the sequence to the queue */
                                /* queue handling is active -> try to add a new job to the queue    */
                                rba_I2c_Prv_adrSequenceQueue_ast[ xLogicChannelId_Prv_u8 ] [ rba_I2c_Prv_posnQueueDynamic_ast[ xLogicChannelId_Prv_u8 ].idxFreePos_u8 ] = rba_I2c_SeqDef_st ;

                                /* increment the number of sequences inside the queue */
                                rba_I2c_Prv_posnQueueDynamic_ast[ xLogicChannelId_Prv_u8 ].numEntries_u8++;

                                /* increment the index of the next free input at the queue */
                                rba_I2c_Prv_posnQueueDynamic_ast[ xLogicChannelId_Prv_u8 ].idxFreePos_u8++;

                                /* when free position is at end of queue step to the begin of ring buffer */
                                rba_I2c_Prv_posnQueueDynamic_ast[ xLogicChannelId_Prv_u8 ].idxFreePos_u8 %= RBA_I2C_CFG_QUEUE_LENGTH;

                                /* Check the sequence running */
                                if(rba_I2c_Prv_stI2c_ChanRunStatus_en[ xLogicChannelId_Prv_u8 ] == RBA_I2C_CHANNEL_NOT_RUNNING )
                                {
                                    /* Set the Handler running state to TRUE */
                                    rba_I2c_Prv_stI2c_ChanRunStatus_en[ xLogicChannelId_Prv_u8 ] = RBA_I2C_CHANNEL_RUNNING;

                                    SchM_Exit_rba_I2c_Driver();

                                    /* call the sequence handler */
                                    rba_I2c_Prv_Handler( xLogicChannelId_Prv_u8 );
                                }
                                else
                                {
                                    SchM_Exit_rba_I2c_Driver();

                                }

                                /* sequence successfully added to queue */
                                stRet = E_OK;
                            }
                            else
                            {
                                /* Set the sequence status to fail */
                                rba_I2c_SeqDef_st->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;

                                /* Set the sequence Error status to Queue is full */
                                rba_I2c_SeqDef_st->rba_I2c_xComErr_u8 = RBA_I2C_ERR_SEQ_QUEUE_FULL;

                                /* Call the DET error for QUEUE full */
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                                (void)Det_ReportError( RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_TXRX_SEQUENCE, RBA_I2C_DET_ERR_QUEUE_FULL_HIGH_PRIO );
#endif
                                /* set error flag */
                                stRet = E_NOT_OK;

                                /* call the sequence handler */
                                rba_I2c_Prv_Handler( xLogicChannelId_Prv_u8 );
                            }
                        }
                        else
                        {
                            /* check if the number of entries is below the maximum size of queue length */
                            if( ( rba_I2c_Prv_posnQueueHiPriDynamic_ast[ xLogicChannelId_Prv_u8 ].numEntries_u8 ) < (RBA_I2C_CFG_QUEUE_LENGTH) )
                            {
                                SchM_Enter_rba_I2c_Driver();

                                /* Change the sequence state to Start condition */
                                rba_I2c_SeqDef_st ->rba_I2c_xSeqStatus_en = rba_I2c_Start_e;

                                /* Queue sequence state to Start condition */
                                rba_I2c_Prv_stSequenceStatus_au8 [xSequenceId_u8] = rba_I2c_QueueSequence_Busy_e;

                                /* Add the sequence to the queue */
                                /* queue handling is active -> try to add a new job to the queue    */
                                rba_I2c_Prv_adrSequenceHiPriQueue_ast[ xLogicChannelId_Prv_u8 ] [ rba_I2c_Prv_posnQueueHiPriDynamic_ast[ xLogicChannelId_Prv_u8 ].idxFreePos_u8 ]= rba_I2c_SeqDef_st ;

                                 /* increment the number of sequences inside the queue */
                                rba_I2c_Prv_posnQueueHiPriDynamic_ast[ xLogicChannelId_Prv_u8 ].numEntries_u8++;

                                 /* increment the index of the next free input at the queue */
                                rba_I2c_Prv_posnQueueHiPriDynamic_ast[ xLogicChannelId_Prv_u8 ].idxFreePos_u8++;

                                 /* when free position is at end of queue step to the begin of ring buffer */
                                rba_I2c_Prv_posnQueueHiPriDynamic_ast[ xLogicChannelId_Prv_u8 ].idxFreePos_u8 %= RBA_I2C_CFG_QUEUE_LENGTH;

                                 /* Check the sequence running */
                                if(rba_I2c_Prv_stI2c_ChanRunStatus_en[ xLogicChannelId_Prv_u8 ] == RBA_I2C_CHANNEL_NOT_RUNNING )
                                {
                                    /* Set the Handler running state to TRUE */
                                    rba_I2c_Prv_stI2c_ChanRunStatus_en[ xLogicChannelId_Prv_u8 ] = RBA_I2C_CHANNEL_RUNNING;

                                    SchM_Exit_rba_I2c_Driver();

                                    /* call the sequence handler */
                                    rba_I2c_Prv_Handler( xLogicChannelId_Prv_u8 );
                                }
                                else
                                {
                                    SchM_Exit_rba_I2c_Driver();

                                    // Misra ++  WARNING ++: <=4=(2004) No concluding 'else' exists in this 'if'-'else'-'if' statement. MISRA-C:2004 Rule 14.10
                                }

                                 /* sequence successfully added to queue */
                                stRet = E_OK;
                           }
                            else
                            {
                                /* Set the sequence status to fail */
                                rba_I2c_SeqDef_st->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;

                                /* Set the sequence Error status to Queue is full */
                                rba_I2c_SeqDef_st->rba_I2c_xComErr_u8 = RBA_I2C_ERR_SEQ_QUEUE_FULL;

                                 /* Call the DET error for QUEUE full */
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                                (void)Det_ReportError( RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_TXRX_SEQUENCE, RBA_I2C_DET_ERR_QUEUE_FULL_LOW_PRIO );
#endif
                                 /* set error flag */
                                 stRet = E_NOT_OK;

                                 /* call the sequence handler */
                                 rba_I2c_Prv_Handler( xLogicChannelId_Prv_u8 );
                             }
                       }


                }



            }


    }

    return  stRet;

}

///////////////////////////////////////////////////////////////////////////////
//! \brief Function interface
//! This process Will help to execute the  Queue sequence. Application request
//! are updated in to  the queue and  Handler will Read the data from the queue .
//!
//!
//! \param Logic ID of I2C channel
//!
//! \return Void
//!
//! \pre none
//!
//! \post Enter a new I2C sequence into the queue.
//!
///////////////////////////////////////////////////////////////////////////////
void  rba_I2c_Prv_Handler(uint8 xlogic_Id_u8)
{

    /* Local variables */
    uint8        xNextSeqAtQueue_u8 = 0u;  /* Next sequence id. */
    uint8        xSequenceId_u8 = 0u;      /* Sequence ID. */

    if( rba_I2c_Prv_posnQueueHiPriDynamic_ast[ xlogic_Id_u8 ].numEntries_u8 > 0 )
    {
        /* Get the index of the next sequence which should be processed */
        xNextSeqAtQueue_u8 = rba_I2c_Prv_posnQueueHiPriDynamic_ast[ xlogic_Id_u8 ].idxSendPos_u8;

        /* state-machine for processing the sequence */
        if( rba_I2c_Prv_adrSequenceHiPriQueue_ast[ xlogic_Id_u8 ] [ xNextSeqAtQueue_u8 ]->rba_I2c_xSeqStatus_en == rba_I2c_Start_e )
        {
            /* Execute the Sequence request*/
            rba_I2c_Prv_Hw_Handler(rba_I2c_Prv_adrSequenceHiPriQueue_ast[ xlogic_Id_u8 ] [ xNextSeqAtQueue_u8 ]);
        }
        else
        {
            /* Sequence status set to failed */
            rba_I2c_Prv_adrSequenceHiPriQueue_ast[ xlogic_Id_u8 ] [ xNextSeqAtQueue_u8 ]->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;

            /* Set the BUS is not free error  */
            rba_I2c_Prv_adrSequenceHiPriQueue_ast[ xlogic_Id_u8 ] [ xNextSeqAtQueue_u8 ]->rba_I2c_xComErr_u8 = RBA_I2C_ERR_SEQ_REPEAT_SEQ;

            /* Increment the Queue counter*/
            rba_I2c_Prv_IncSendPosition_Queue( xlogic_Id_u8, RBA_I2C_SEQ_HIGH_PRIORITY );

            /* Sequence Handler status updates as Process Completed*/
            rba_I2c_Prv_stI2c_ChanRunStatus_en[ xlogic_Id_u8 ] = RBA_I2C_CHANNEL_NOT_RUNNING;

            /* Generate the DET report for the repeat sequence entry in queue */
#if ( RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON )
           (void)Det_ReportError( RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_PRV_HANDLER, RBA_I2C_DET_ERR_REPEAT_SEQ_HIGH_PRIO );
#endif

           /* Read the sequence ID*/
           xSequenceId_u8 = rba_I2c_Prv_adrSequenceHiPriQueue_ast[ xlogic_Id_u8 ] [ xNextSeqAtQueue_u8 ]->rba_I2c_xSequenceCfgId_u8;

            /* After complete the sequence handling execute the callback notification if available */
            if(rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[xSequenceId_u8]->xSeqEndNotif_pfct != NULL_PTR )
            {
                (*rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[ xSequenceId_u8 ]->xSeqEndNotif_pfct)();
            }

            /* after complete the sequence execution set the Sequence status to IDLE */
            rba_I2c_Prv_stSequenceStatus_au8[ xSequenceId_u8 ] = rba_I2c_QueueSequence_Idle_e;

        }
    }
    else
    {
        if( rba_I2c_Prv_posnQueueDynamic_ast[ xlogic_Id_u8 ].numEntries_u8 > 0 )
        {
        /* Get the index of the next sequence which should be processed */
        xNextSeqAtQueue_u8 = rba_I2c_Prv_posnQueueDynamic_ast[ xlogic_Id_u8 ].idxSendPos_u8;

        /* state-machine for processing the sequence */
        if( rba_I2c_Prv_adrSequenceQueue_ast[ xlogic_Id_u8 ] [ xNextSeqAtQueue_u8 ]->rba_I2c_xSeqStatus_en == rba_I2c_Start_e )
        {
            /* Execute the Sequence request*/
            rba_I2c_Prv_Hw_Handler(rba_I2c_Prv_adrSequenceQueue_ast[ xlogic_Id_u8 ] [ xNextSeqAtQueue_u8 ]);
        }
        else
        {
            /* Sequence status set to failed */
            rba_I2c_Prv_adrSequenceQueue_ast[ xlogic_Id_u8 ] [ xNextSeqAtQueue_u8 ]->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;

            /* Set the BUS is not free error  */
            rba_I2c_Prv_adrSequenceQueue_ast[ xlogic_Id_u8 ] [ xNextSeqAtQueue_u8 ]->rba_I2c_xComErr_u8 = RBA_I2C_ERR_SEQ_REPEAT_SEQ;

            /* Increment the Queue counter*/
            rba_I2c_Prv_IncSendPosition_Queue( xlogic_Id_u8, RBA_I2C_SEQ_LOW_PRIORITY );

            /* Sequence Handler status updates as Process Completed*/
            rba_I2c_Prv_stI2c_ChanRunStatus_en[ xlogic_Id_u8 ] = RBA_I2C_CHANNEL_NOT_RUNNING;

            /* Generate the DET report for the repeat sequence entry in queue */
#if ( RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON )
            (void)Det_ReportError( RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_PRV_HANDLER, RBA_I2C_DET_ERR_REPEAT_SEQ_LOW_PRIO );
#endif

            /* Read the sequence ID */
            xSequenceId_u8 = rba_I2c_Prv_adrSequenceQueue_ast[ xlogic_Id_u8 ] [ xNextSeqAtQueue_u8 ]->rba_I2c_xSequenceCfgId_u8;

             /* After complete the sequence handling execute the callback notification if available */
             if(rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[xSequenceId_u8]->xSeqEndNotif_pfct != NULL_PTR )
             {
                 (*rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[ xSequenceId_u8 ]->xSeqEndNotif_pfct)();
             }

             /* after complete the sequence execution set the Sequence status to IDLE */
             rba_I2c_Prv_stSequenceStatus_au8[ xSequenceId_u8 ] = rba_I2c_QueueSequence_Idle_e;
        }
        }
    }
}

#ifdef RBA_I2C_ENABLE_DEINIT
///////////////////////////////////////////////////////////////////////////////
//! \brief Function interface
//! This process will support the shutdown process, during Os shutdown all the configured
//! I2C channels are  disabled.
//!
//! This interface sets the I2C module hardware setting to the IDLE mode, and resets to  the default state.
//! After De-Init if required initialization sequence should be called for using I2C module again.
//!
//!
//! \param Void
//!
//! \return Void
//!
//! \pre none
//!
//! \post Enter a new I2C sequence into the queue.
//!
///////////////////////////////////////////////////////////////////////////////
void  rba_I2c_DeInit(void)
{
     uint8  idxLogicUnit_qu8 = 0u;
     uint8  idxHwId_u8 = 0u;
     uint8  idxLogicId_u8 = 0u;
    // Check whether the module is already in deinitialized state
  if(rba_I2c_prv_cfgConfigType_pcst == NULL_PTR)
  {
    #if( RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON )
     (void)Det_ReportError( RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_DEINIT, RBA_I2C_DET_ERR_DEINITIALIZED );
    #endif
  }
  else
   {
     for(idxLogicUnit_qu8 = 0u; idxLogicUnit_qu8 < RBA_I2C_CFG_MAX_CONFIGURED_CHANNELS; idxLogicUnit_qu8++)
       {
        idxHwId_u8 = idxLogicUnit_qu8;
        idxLogicId_u8 = idxLogicUnit_qu8;

#if( (MCU_RB_MACHINE_DEV == MCU_RB_JDP_UC1_DEV4_ALL) || (MCU_RB_MACHINE_DEV == MCU_RB_IFX_UC1_DEV4_ALL) || (MCU_RB_MACHINE_DEV == MCU_RB_IFX_UC1_DEV5_40NM_ALL) || (MCU_RB_MACHINE_DEV == MCU_RB_IFX_UC1_DEV4_40NM_ALL) )

          #if((RBA_I2C_CFG_MAX_CONFIGURED_CHANNELS == RBA_I2C_CHANNEL_ONE) && (RBA_I2C_CFG_CHANNEL_CONF_AVAILABLE_1 == RBA_I2C_CHANNEL_ONE))

           idxHwId_u8 = RBA_I2C_CHANNEL_ONE;

          #endif
#endif
                    /* For complete the Queue entry sequence*/
          rba_I2c_Prv_Handler( idxLogicId_u8 );

                   /* Set the Hardware register to Disable the I2C module*/
          rba_I2c_Prv_Hw_DeInit( idxHwId_u8 );
         }


    /* Set the init status */
    rba_I2c_stInitialized_b = FALSE;
    rba_I2c_Prv_stInterruptMode_b = FALSE;
    rba_I2c_prv_cfgConfigType_pcst = NULL_PTR;
    }
}
#endif /* #ifdef RBA_I2C_ENABLE_DEINIT */

// ---------------------------------------
//! \brief rba_I2c Driver init Check Function
//!
//! This function checks the pointer given to the initialization of the module against
//! the pointer given as parameter to this function.
//!
//! Sync/Async: Synchronous
//!
//! Reentrancy: Reentrant
//!
//! \param configuration_pcst [in]: Pointer to selected rba_I2c driver configuration structure
//!
//! \return E_OK if pointer matches used configuration, otherwise E_NOT_OK
//!
//! \pre  None
//!
//! \post None
//!
// ---------------------------------------

Std_ReturnType rba_I2C_MoInitCheck(const rba_I2c_ConfigType *configuration_pcst)
{
    return (Std_ReturnType)((configuration_pcst == rba_I2c_prv_cfgConfigType_pcst) ? E_OK : E_NOT_OK);
}


#define  RBA_I2C_STOP_SEC_CODE
#include RBA_I2C_CFG_MEMMAP_H

/* end of #ifdef RBA_I2C_CFG_MODULE_ACTIVE macro */
#endif


/*! @} */
/*<RBHead>
 *************************************************************************
 * List Of Changes
 *
 * $History$
 *
 *************************************************************************
</RBHead>*/

