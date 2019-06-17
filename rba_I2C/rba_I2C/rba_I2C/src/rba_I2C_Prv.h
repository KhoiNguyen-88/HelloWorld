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

#ifndef RBA_I2C_PRV_H
#define RBA_I2C_PRV_H

/*!
 * \rba_comp rba_I2C
 *  @{
 */
/*-------------------------------------------------------------------------------------------------------------------*
 * Includes
 *-------------------------------------------------------------------------------------------------------------------*/

#include "rba_I2C.h"
#if(RBA_I2C_CFG_MODULE_ACTIVE == STD_ON)




/*
 ***********************************************************************************************************************
 * Defines/Macros
 ***********************************************************************************************************************
*/

#define RBA_I2C_PRV_QUEUE_INACTIVE       0x00u     //!< Queue is inactive.
#define RBA_I2C_PRV_QUEUE_ACTIVE         0x01u     //!< Queue is active.
#define RBA_I2C_PRV_QUEUE_LOCKED         0x02u     //!< Queue is locked.this state is used to flush thequeue when switching from active to inactive queue.

#define RBA_I2C_CHANNEL_ZERO             0x00u
#define RBA_I2C_CHANNEL_ONE              0x01u
#define RBA_I2C_TRANSMIT_MODE            0x00u
#define RBA_I2C_RECEIVE_MODE             0x01u

#define RBA_I2C_ZERO                        0u
#define RBA_I2C_CLK_LENGTH                  9u    // Number of bits in a byte
#define RBA_I2C_CLK_100K                   12u    // One clock cycle for 89KHz -> 1/89KHZ = 11.2359 microseconds
#define RBA_I2C_CLK_400K                    3u    // One clock cycle for 384KHz -> 1/384KHZ = 2.6041 microseconds
#define RBA_I2C_SLAVEADD_LEN                1u    // Length of Slave address


/* Queue sequence position strutcture */
typedef struct
{
    uint8  idxFreePos_u8;                                /* index of next free position at queue       */
    uint8  idxSendPos_u8;                                /* index of actual handled sequence at queue  */
    uint8  numEntries_u8;                                /* number of sequences placed inside queue    */
} rba_I2c_QueueDynamic_tst;

/*
 ***********************************************************************************************************************
 * Extern declarations

 ***********************************************************************************************************************
*/
#define  RBA_I2C_START_SEC_VAR_CLEARED_UNSPECIFIED
#include RBA_I2C_CFG_MEMMAP_H

/* slave interface sequence */
typedef struct
{
    rba_I2c_SeqDef_tst          *currentSeq;            /* current running sequence */
    uint8                        runSequenceCfgId_u8;   /* Sequence configured signal ID */
    uint8                       *runDataRx_pu8;         /* sequence Receive data pointer */
    uint8                       *runDataTx_pu8;         /* sequence Transmit data pointer */
    uint8                        runSeqTxlength_u8;     /* sequence Transmit data Length */
    uint8                        runSeqRxlength_u8;     /* sequence Receive data Length */
    rba_I2c_xComType_ten         runSeqComType_en;      /* sequence Communication Type (ex: Read or write) */
    rba_I2c_PriorityType_ten     runSeqPriority_u8;     /* Sequence Priority. */
} rba_I2c_Seq_Run_tst;

 extern rba_I2c_xQueueSeqStatus_ten        rba_I2c_Prv_stSequenceStatus_au8[ RBA_I2C_CFG_QUEUE_LENGTH ];
 extern uint8                              rba_I2c_Prv_stQueueState_au8[ RBA_I2C_CFG_MAX_CONFIGURED_CHANNELS ];  /* Queue status */
 extern rba_I2c_QueueStatusType_ten        rba_I2c_Prv_stI2c_ChanRunStatus_en[ RBA_I2C_CFG_MAX_CONFIGURED_CHANNELS ];      /* Sequence running status */
 extern rba_I2c_QueueDynamic_tst           rba_I2c_Prv_posnQueueDynamic_ast[ RBA_I2C_CFG_MAX_CONFIGURED_CHANNELS ]; /* Queue status variable */
 extern rba_I2c_QueueDynamic_tst           rba_I2c_Prv_posnQueueHiPriDynamic_ast[ RBA_I2C_CFG_MAX_CONFIGURED_CHANNELS ]; /* Queue status variable */
 extern rba_I2c_SeqDef_tst                 *rba_I2c_Prv_adrSequenceQueue_ast[ RBA_I2C_CFG_MAX_CONFIGURED_CHANNELS ][ RBA_I2C_CFG_QUEUE_LENGTH ]; /* I2C Queue */
 extern rba_I2c_SeqDef_tst                 *rba_I2c_Prv_adrSequenceHiPriQueue_ast[ RBA_I2C_CFG_MAX_CONFIGURED_CHANNELS ][ RBA_I2C_CFG_QUEUE_LENGTH ]; /* I2C Queue */
 extern const rba_I2c_ConfigType*          rba_I2c_prv_cfgConfigType_pcst;                               /* Configuration Pointer */
 extern uint32                             rba_I2c_Clk;                                                  /* Number of ticks per clock*/
#define  RBA_I2C_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include RBA_I2C_CFG_MEMMAP_H

#define  RBA_I2C_START_SEC_VAR_INIT_8
#include RBA_I2C_CFG_MEMMAP_H
 extern boolean                            rba_I2c_stInitialized_b;                       /* I2C initialized status */
 extern boolean                            rba_I2c_Prv_stInterruptMode_b;                 /* Interrupt enable status */
#define  RBA_I2C_STOP_SEC_VAR_INIT_8
#include RBA_I2C_CFG_MEMMAP_H

#define  RBA_I2C_START_SEC_CODE
#include RBA_I2C_CFG_MEMMAP_H

 extern void  rba_I2c_Prv_Handler(uint8 xlogic_Id_u8);
 extern void  rba_I2c_Prv_Hw_Init(uint8 idxHwUnit_qu8);
 extern void  rba_I2c_Prv_Hw_Handler(rba_I2c_SeqDef_tst *rba_I2c_SeqData_st);
#ifdef RBA_I2C_ENABLE_DEINIT
 extern void  rba_I2c_Prv_Hw_DeInit(uint8 idxHwUnit_qu8);
#endif

 LOCAL_INLINE void  rba_I2c_Prv_IncSendPosition_Queue(uint8 logicId, rba_I2c_PriorityType_ten xPriority_en );

 LOCAL_INLINE void  rba_I2c_Prv_IncSendPosition_Queue(uint8 logicId, rba_I2c_PriorityType_ten xPriority_en )
 {
     if(xPriority_en == RBA_I2C_SEQ_LOW_PRIORITY)
     {
         /* decrement the number of entries at the queue */
         rba_I2c_Prv_posnQueueDynamic_ast[ logicId ].numEntries_u8--;

         /* increment the index to next transmit input of queue */
         rba_I2c_Prv_posnQueueDynamic_ast[ logicId ].idxSendPos_u8++;

         /* when tx index is at end of queue step to begin of ring buffer */
         rba_I2c_Prv_posnQueueDynamic_ast[ logicId ].idxSendPos_u8 %= RBA_I2C_CFG_QUEUE_LENGTH;
     }
     else
     {
         /* decrement the number of entries at the queue */
         rba_I2c_Prv_posnQueueHiPriDynamic_ast[ logicId ].numEntries_u8--;

         /* increment the index to next transmit input of queue */
         rba_I2c_Prv_posnQueueHiPriDynamic_ast[ logicId ].idxSendPos_u8++;

         /* when tx index is at end of queue step to begin of ring buffer */
         rba_I2c_Prv_posnQueueHiPriDynamic_ast[ logicId ].idxSendPos_u8 %= RBA_I2C_CFG_QUEUE_LENGTH;
     }
 }

#define  RBA_I2C_STOP_SEC_CODE
#include RBA_I2C_CFG_MEMMAP_H

#if (MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1)
    #include   "rba_I2CIfx.h"
#endif

#if((MCU_RB_MACHINE_FAM == MCU_RB_JDP_UC1) && (MCU_RB_MACHINE_DEV != MCU_RB_JDP_UC1_DEV1_ALL))
    #include   "rba_I2CJdp.h"
#endif


/* End of #ifdef RBA_I2C_CFG_MODULE_ACTIVE macro */
#endif

/* End of #ifndef RBA_I2C_PRV_H macro validation */
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

/*<RBHead>
**********************************************************************************************************************
* End of header file: $Name______:$
**********************************************************************************************************************
</RBHead>*/
