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

#include   "rba_I2CJdp.h"

#if((MCU_RB_MACHINE_FAM == MCU_RB_JDP_UC1) && (MCU_RB_MACHINE_DEV != MCU_RB_JDP_UC1_DEV1_ALL))


#if(RBA_I2C_CFG_MODULE_ACTIVE == STD_ON)

#include   "rba_BswSrv.h"
#include   "rba_I2C_Cfg_SchM.h"

#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#if (!defined(DET_AR_RELEASE_MAJOR_VERSION) || (DET_AR_RELEASE_MAJOR_VERSION != RBA_I2C_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(DET_AR_RELEASE_MINOR_VERSION) || ((DET_AR_RELEASE_MINOR_VERSION != 0) && \
                                                (DET_AR_RELEASE_MINOR_VERSION != 2)))
#error "AUTOSAR minor version undefined or mismatched"
#endif
#endif

#define  RBA_I2C_START_SEC_VAR_CLEARED_UNSPECIFIED
#include RBA_I2C_CFG_MEMMAP_H
    /* I2C module JDP global variable */
    rba_I2c_Seq_Run_tst           rba_I2c_Prv_JdpSeq_Run_st[RBA_I2C_CFG_MAX_CONFIGURED_CHANNELS];    /* I2C Sequence status st */

    /* I2C Sequence transmit/receive status counter */
    rba_I2c_Prv_SeqCount_tst    rba_I2c_Prv_SeqCount_st[RBA_I2C_CFG_MAX_CONFIGURED_CHANNELS];   /* I2C Sequence status st */

#define  RBA_I2C_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include RBA_I2C_CFG_MEMMAP_H

#define  RBA_I2C_START_SEC_VAR_INIT_UNSPECIFIED
#include RBA_I2C_CFG_MEMMAP_H
    /* Port Address Array */
    rba_Reg_I2C_tst* rba_I2c_Prv_Address_apcst[] =
    {   /*  Register Address  */
        (rba_Reg_I2C_tst*)   &I2C0,                  // Index  0  PORT_00
        #if(MCU_RB_MACHINE_DEV == MCU_RB_JDP_UC1_DEV4_ALL)
        (rba_Reg_I2C_tst*)   &I2C1,                  // Index  1  PORT_02
        #endif
    };
#define  RBA_I2C_STOP_SEC_VAR_INIT_UNSPECIFIED
#include RBA_I2C_CFG_MEMMAP_H

//- Functions -----------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
//! \brief Initialize driver specific stuff.
//! Initializes and handles the pre-condition Hardware settings for I2C communication. I2C protocol can support
//! more than one I2C module (hardware), hence init process shall vary based on the number of Configured I2C modules.
//!
//! <b>Sync/Async:</b> Synchronous
//! <b>Reentrancy:</b> Non-reentrant
//!
//! \param idxHwUnit_qu8: I2C Hardware unit ID.
//!
//! \return Void
//!
//! \pre none
//!
///////////////////////////////////////////////////////////////////////////////

#define  RBA_I2C_START_SEC_CODE
#include RBA_I2C_CFG_MEMMAP_H
void rba_I2c_Prv_Hw_Init(uint8 idxHwUnit_qu8)
{
    /* Enable the I2C module setting in the IBCR register */
    rba_I2c_Prv_Address_apcst[ idxHwUnit_qu8 ]->IBCR = (rba_I2c_Prv_Address_apcst[ idxHwUnit_qu8 ]->IBCR) & ( (uint8)(~(1uL << RBA_REG_I2C_IBCR_MDIS_POS)) );

    /* Set the I2C bus to IDLE condition in the IBCR register */
    rba_I2c_Prv_Address_apcst[ idxHwUnit_qu8 ]->IBCR &= (uint8)(~(1uL << RBA_REG_I2C_IBCR_MSSL_POS));

    /* Set to I2C Write Mode in the IBCR register*/
    rba_I2c_Prv_Address_apcst[ idxHwUnit_qu8 ]->IBCR |= (uint8)(1uL << RBA_REG_I2C_IBCR_TXRX_POS);

    /* set ID for Master in the IBAD register */
    rba_I2c_Prv_Address_apcst[ idxHwUnit_qu8 ]->IBAD = RBA_I2C_JDP_SLAVEID;

    /* Set the I2C Clock Divider(default 100KHz) in the IBFD register */
    rba_I2c_Prv_Address_apcst[ idxHwUnit_qu8 ]->IBFD = RBA_I2C_JDP_CLOCK_SPEED_100K;
}

//- Functions -----------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \brief Initialize driver specific stuff.
//! Initializes and handles the pre-condition settings for I2C communication. I2C protocol can support
//! more than one I2C module (hardware), hence init process shall vary based on the number of Configured I2C modules.
//!
//! <b>Sync/Async:</b> Synchronous
//!
//! <b>Reentrancy:</b> Non-reentrant
//!
//! \param rba_I2c_SeqData_st: interface sequence.
//!
//! \return Void
//!
//! \pre none
//!
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void rba_I2c_Prv_Hw_Handler(rba_I2c_SeqDef_tst *rba_I2c_SeqData_st)
{
    /* Private Variable */
    uint8                     xLogicId_u8 = RBA_I2C_ZERO;               /* Generated Logical ID for the Hardware */
    uint8                     xSequenceId_u8 = RBA_I2C_ZERO;            /* Configured Signal ID */
    uint8                     xSlaveId_u8 = RBA_I2C_ZERO;               /* Configured Signal ID */
    uint8                     xHwId_u8 = RBA_I2C_ZERO;                  /* Configured Hardware ID for Signal*/
    Std_ReturnType            stRet = E_NOT_OK;                         /* function Retun value */
    uint8                     xValidateStatus_u8 = 0u;                  /* function Retun value */
    rba_I2c_PriorityType_ten  xSeqPrio_en = RBA_I2C_SEQ_LOW_PRIORITY;   /* Sequence Priority status*/
    uint8                     countVar_u8 = 0u;                         /* Local counter */
    boolean                   xCommunication_status_u8 = TRUE;          /* Local communication status variable */

    /* Read the Sequence Signal ID */
    xSequenceId_u8 = rba_I2c_SeqData_st->rba_I2c_xSequenceCfgId_u8;

    /* Read the Slave ID */
    xSlaveId_u8 = rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[xSequenceId_u8]->SlaveId;

    /* Read the hardware (channel info) Id from the slave configuration dataset.  */
    xHwId_u8 = rba_I2c_prv_cfgConfigType_pcst->slaveConfig_pcst[ xSlaveId_u8 ]->hwConnectID_u8;

    /* Read the logic ID*/
    xLogicId_u8 = rba_I2c_prv_cfgConfigType_pcst->hwUnitConfig_pcst[ xHwId_u8 ]->LogicHwId_u8;

    /* Read the sequence priority */
    xSeqPrio_en = rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[xSequenceId_u8]->sequPrio_en;

    /* common function is start of this place( Polling and interrupt)*/

    /* Check the interrupt status from the initial function */
    /* MISRA warring: Rule CCode_Control_002: Value Test Expression*/
    if ( rba_I2c_Prv_stInterruptMode_b )
    {
        /* enable the Hardware interrupt in the IBCR register  */
        rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBCR |= (uint8)(1uL << RBA_REG_I2C_IBCR_IBIE_POS);
    }
    else
    {
        /*  Disable the Hardware interrupt in the IBCR register */
        rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBCR &= (uint8)(~(1uL << RBA_REG_I2C_IBCR_IBIE_POS));
    }

    if(rba_I2c_prv_cfgConfigType_pcst->slaveConfig_pcst[ xSlaveId_u8 ]->baudRate_en == RBA_I2C_SLAVE_NORMAL_MODE )
    {
        /*MR12 RULE 14.3 VIOLATION: Warning is non-removable and uncritical since the conditional operator is used to make the return value is not overflow - Approved by MISRA2012_SUPPORT_TEAM*/
        rba_I2c_Clk = MCU_RB_US_TO_TICKS(RBA_I2C_CLK_100K); /* One clock cycle for 89KHz -> 1/89KHZ = 11.2359 microseconds */
    }
    else
    {
        /*MR12 RULE 14.3 VIOLATION: Warning is non-removable and uncritical since the conditional operator is used to make the return value is not overflow - Approved by MISRA2012_SUPPORT_TEAM*/
        rba_I2c_Clk = MCU_RB_US_TO_TICKS(RBA_I2C_CLK_400K); /* One clock cycle for 384KHz -> 1/384KHZ = 2.6041 microseconds */
    }

    /* Copy the sequence to internal variable */
    rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].currentSeq          =  rba_I2c_SeqData_st;
    rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runDataRx_pu8       =  rba_I2c_SeqData_st->rba_I2c_dataRx_pu8;
    rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runDataTx_pu8       = rba_I2c_SeqData_st->rba_I2c_dataTx_pu8;
    rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runSequenceCfgId_u8 = rba_I2c_SeqData_st->rba_I2c_xSequenceCfgId_u8;
    rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runSeqTxlength_u8   = rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[xSequenceId_u8]->seqTxLen_u8;
    rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runSeqRxlength_u8   = rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[xSequenceId_u8]->seqRxLen_u8;
    rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runSeqComType_en    = rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[xSequenceId_u8]->seqComType_en;
    rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runSeqComType_en    = rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[xSequenceId_u8]->seqComType_en;

    /* Reset the Transmit position counter */
    rba_I2c_Prv_SeqCount_st[ xLogicId_u8 ].dataTxPosition_u8 = RBA_I2C_ZERO;

    /* Reset the Receive position counter */
    rba_I2c_Prv_SeqCount_st[ xLogicId_u8 ].dataRxPosition_u8 = RBA_I2C_ZERO;

    /*wait till bus go to IDLE before start condition (wait till previous sequence send the STOP condition complete  )*/
    /* 100Khz speed each byte transmission time 120us or 400khz for 30us*/
    stRet = rba_I2c_Prv_waitTill_BusIdle(xHwId_u8);

    /* Check the I2C communication bus is idle in the IBSR register*/
    if( (stRet == E_OK ) &&  ( ( ( rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBCR ) & ( (uint8)(1uL << RBA_REG_I2C_IBCR_MDIS_POS )) ) == 0u )  )
    {
        /* Disable the ACK for Transmission setting in the in the IBCR register and set as Write mode*/
        rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBCR &= (uint8)(~( 1uL << RBA_REG_I2C_IBCR_NOACK_POS));

        /* Set to I2C Write Mode setting in the IBCR register*/
        rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBCR |= (uint8)( 1uL << RBA_REG_I2C_IBCR_TXRX_POS );

        /* Frequency setting */
        if(rba_I2c_prv_cfgConfigType_pcst->slaveConfig_pcst[ xSlaveId_u8 ]->baudRate_en == RBA_I2C_SLAVE_NORMAL_MODE )
        {
            /* Set the I2C Clock Divider */
            rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBFD =  RBA_I2C_JDP_CLOCK_SPEED_100K; /* clock setting 89Khz */
        }
        else
        {
            /* Set the I2C Clock Divider */
            rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBFD =  RBA_I2C_JDP_CLOCK_SPEED_400K; /* clock setting 384Khz */
        }

        /* Read the configure 7bit salve unique address from the config dataset*/
        rba_I2c_Prv_SeqCount_st[ xLogicId_u8 ].slaveID_u8 = rba_I2c_prv_cfgConfigType_pcst->slaveConfig_pcst[ xSlaveId_u8 ]->slaveAddress_u8;

        /* Send Start command to the Slave device*/
        rba_I2c_JdpStart( xHwId_u8 );

        /* Wait till I2C bus go to busy state */
        stRet =  rba_I2c_Prv_waitTill_BusBusy( xHwId_u8);

        if(stRet == E_NOT_OK )
        {
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
            /* Call DET error for Timeout slave interface */
            (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_JDP_HW_PRV_HANDLER,
                                  RBA_I2C_DET_ERR_JDP_I2C_START_FAILURE);
#endif
            /* Execute the Sequence End transmission process */
            rba_I2c_Prv_SeqEndTransmission(xSequenceId_u8, xHwId_u8, xLogicId_u8,
                    xSeqPrio_en, rba_I2c_SeqFailed_e, RBA_I2C_ERR_SEQ_JDP_I2C_START_FAILURE);

        }

        else
        {
            /* Clear the I2C module interrupt*/
            rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBSR |= (uint8)( 1uL << RBA_REG_I2C_IBSR_IBIF_POS );

            /* Check whether the communication type is Receive */
            if(rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runSeqComType_en == rba_I2c_Rx_e)
            {
                /* Send the Slave id in I2C bus for the Receive mode*/
                rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBDR = rba_I2c_Prv_SeqCount_st[ xLogicId_u8 ].slaveID_u8 + RBA_I2C_RECEIVE_MODE;
            }
            else
            {
                /* Send the Slave id in I2C bus for the Transmit mode */
                rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBDR = rba_I2c_Prv_SeqCount_st[ xLogicId_u8 ].slaveID_u8 + RBA_I2C_TRANSMIT_MODE;
            }

            /* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ Polling Mode Interface start ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
            /* Check whether the interrupt is enable or not */
            if(rba_I2c_Prv_stInterruptMode_b == FALSE)
            {
                /* Validate the last byte(Slave ID) transmission */
                xValidateStatus_u8 = rba_I2c_Prv_ValidateTransmissionStatus( xHwId_u8, RBA_I2C_SLAVEADD_LEN);

                /* check the return status of "Validate Transmission Status" function */
                if(xValidateStatus_u8 == RBA_I2C_JDP_TX_COMPLETE_CHK_FAILED )
                {

#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                    /* Call DET error for Timeout slave interface */
                    (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_JDP_HW_PRV_HANDLER,
                                          RBA_I2C_DET_ERR_JDP_POL_TX_ID_NOT_COMPLETED);
#endif

                    /* Execute the Sequence End transmission process */
                    rba_I2c_Prv_SeqEndTransmission(xSequenceId_u8, xHwId_u8, xLogicId_u8,
                                        xSeqPrio_en, rba_I2c_SeqFailed_e, RBA_I2C_ERR_SEQ_JDP_POL_TX_ID_NOT_COMPLETED);


                }
                else if(xValidateStatus_u8 == RBA_I2C_JDP_TX_ACK_NOT_RECEIVED_FAILED)
                {
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                    /* Call DET error for Timeout slave interface */
                    (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_JDP_HW_PRV_HANDLER,
                            RBA_I2C_DET_ERR_JDP_POL_TX_ID_ACK_NOT_RECEIVED);
#endif

                    /* Execute the Sequence End transmission process */
                    rba_I2c_Prv_SeqEndTransmission(xSequenceId_u8, xHwId_u8, xLogicId_u8,
                            xSeqPrio_en, rba_I2c_SeqFailed_e, RBA_I2C_ERR_SEQ_JDP_POL_TX_ID_ACK_NOT_RECEIVED);
                }
                else
                {
                    /* Check the communication Type Transmit or Combined Transmit or Receive */
                    if(rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runSeqComType_en == rba_I2c_Tx_e)
                    {
                        /* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ Polling-Transmit sequence execution start ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */


                        /* Send the all Transmit data in the sequence */
                        for(countVar_u8 = 0u; countVar_u8 < rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runSeqTxlength_u8; countVar_u8++)
                        {
                            rba_I2c_Prv_SeqCount_st[ xLogicId_u8 ].dataTxPosition_u8 = countVar_u8 ;

                            /* before transmit the next byte Clear the I2C module interrupt status Flag*/
                            rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBSR |=  ((uint8)( 1uL << RBA_REG_I2C_IBSR_IBIF_POS ));

                            /* Send the Write data to the Slave device*/
                            rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBDR = rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runDataTx_pu8[ rba_I2c_Prv_SeqCount_st[ xLogicId_u8 ].dataTxPosition_u8 ];

                            /* Validate the each byte transmission */
                            xValidateStatus_u8 = rba_I2c_Prv_ValidateTransmissionStatus(xHwId_u8, rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runSeqTxlength_u8);

                            /* check the return status of "Validate Transmission Status" function */
                            if(xValidateStatus_u8 == RBA_I2C_JDP_TX_COMPLETE_CHK_FAILED )
                            {
                                /* set communication status to failure */
                                xCommunication_status_u8 = FALSE;

#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                                /* Call DET error for Timeout slave interface */
                                (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_JDP_HW_PRV_HANDLER,
                                                      RBA_I2C_DET_ERR_JDP_POL_TX_NOT_COMPLETED);
#endif

                                /* Execute the Sequence End transmission process */
                                rba_I2c_Prv_SeqEndTransmission(xSequenceId_u8, xHwId_u8, xLogicId_u8, xSeqPrio_en,
                                                        rba_I2c_SeqFailed_e,RBA_I2C_ERR_SEQ_JDP_POL_TX_NOT_COMPLETED );

                            }
                            else
                            {
							    if(xValidateStatus_u8 == RBA_I2C_JDP_TX_ACK_NOT_RECEIVED_FAILED)
								{
                                /* set communication status to failure */
                                xCommunication_status_u8 = FALSE;

#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                                /* Call DET error for Timeout slave interface */
                                (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_JDP_HW_PRV_HANDLER,
                                                      RBA_I2C_DET_ERR_JDP_POL_TX_ACK_NOT_RECEIVED);
#endif

                                /* Execute the Sequence End transmission process */
                                rba_I2c_Prv_SeqEndTransmission(xSequenceId_u8, xHwId_u8, xLogicId_u8,
                                        xSeqPrio_en, rba_I2c_SeqFailed_e, RBA_I2C_ERR_SEQ_JDP_POL_TX_ACK_NOT_RECEIVED);
                                }
                            }
                            /* for the error communication stop the I2C interface */
                            if(xCommunication_status_u8 == FALSE)
                            {
                                break;
                            }
                        }
                        if( xCommunication_status_u8 == TRUE )
                        {

                            /* Send Stop command to the Slave device*/
                            rba_I2c_JdpStop(xHwId_u8);

                            /* Execute the Sequence End transmission process */
                            rba_I2c_Prv_SeqEndTransmission(xSequenceId_u8, xHwId_u8, xLogicId_u8, xSeqPrio_en,
                               rba_I2c_SeqCompleted_e, E_OK);
                        }

                           /* ^^^^^^^^^^^^^^^^ Polling-Transmit sequence execution End ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
                    }
                    else if(rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runSeqComType_en == rba_I2c_CombinedTxRx_e)
                    {
                        /* ^^^^^^^^^^ Polling-Transmit combined Receive sequence execution start ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

                        /* Send the all Transmit data */
                        for(countVar_u8 = 0u; countVar_u8 <= rba_I2c_Prv_JdpSeq_Run_st[xLogicId_u8].runSeqTxlength_u8; countVar_u8++)
                        {

                            rba_I2c_Prv_SeqCount_st[ xLogicId_u8 ].dataTxPosition_u8 = countVar_u8;

                            /* check whether the tranfering the laster byte in I2C interface */
                            if( rba_I2c_Prv_SeqCount_st[ xLogicId_u8 ].dataTxPosition_u8 >= rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runSeqTxlength_u8)
                            {
                                /* Clear the I2C module interrupt*/
                                rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBSR |=  (uint8)( 1uL << RBA_REG_I2C_IBSR_IBIF_POS );

                                /* Send Repeat Start command to the I2C Channel */
                                rba_I2c_JdpReStart(xHwId_u8);

                                /* create a delay for bus go to busy state */
                                stRet = rba_I2c_Prv_waitTill_BusBusy(xHwId_u8);

                                /* check the return status of "rba_I2c_Prv_waitTill_BusBusy" function */
                                if( stRet != E_OK)
                                {
                                    /* failure condition break the for loop */
                                    /* set communication status to failure */
                                    xCommunication_status_u8 = FALSE;

#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                                    /* Call DET error for Timeout slave interface */
                                    (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_JDP_HW_PRV_HANDLER,
                                           RBA_I2C_DET_ERR_JDP_POL_COMB_TX_RX_BUS_NOT_BUSY);
#endif

                                    /* Execute the Sequence End transmission process */
                                    rba_I2c_Prv_SeqEndTransmission(xSequenceId_u8, xHwId_u8, xLogicId_u8,
                                           xSeqPrio_en, rba_I2c_SeqFailed_e,RBA_I2C_ERR_SEQ_JDP_POL_COMB_TX_RX_BUS_NOT_BUSY );


                                }
                                else
                                {
                                    /* Change I2C bus to receive Mode Send Slave ID in I2C channel*/
                                    rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBDR = rba_I2c_Prv_SeqCount_st[ xLogicId_u8 ].slaveID_u8 + RBA_I2C_RECEIVE_MODE;

                                    /* Validate the last byte transmission status*/
                                    xValidateStatus_u8 = rba_I2c_Prv_ValidateTransmissionStatus( xHwId_u8, RBA_I2C_SLAVEADD_LEN);

                                    /* check the return status of "rba_I2c_Prv_ValidateTransmissionStatus" function */
                                    if(xValidateStatus_u8 == RBA_I2C_JDP_TX_COMPLETE_CHK_FAILED )
                                    {
                                        /* failure condition break the for loop */
                                        /* set communication status to failure */
                                        xCommunication_status_u8 = FALSE;

#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                                        /* Call DET error for Timeout slave interface */
                                        (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_JDP_HW_PRV_HANDLER,
                                                              RBA_I2C_DET_ERR_JDP_POL_COMB_TX_LAST_BYTE_NOT_COMPLETED);
#endif

                                        /* Execute the Sequence End transmission process */
                                        rba_I2c_Prv_SeqEndTransmission(xSequenceId_u8, xHwId_u8, xLogicId_u8,
                                                                       xSeqPrio_en,rba_I2c_SeqFailed_e, RBA_I2C_ERR_SEQ_JDP_POL_COMB_TX_LAST_BYTE_NOT_COMPLETED);

                                    }
                                    else
                                    {
									if(xValidateStatus_u8 == RBA_I2C_JDP_TX_ACK_NOT_RECEIVED_FAILED)
									{
                                        /* failure condition break the for loop */
                                        /* set communication status to failure */
                                        xCommunication_status_u8 = FALSE;

#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                                        /* Call DET error for Timeout slave interface */
                                        (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_JDP_HW_PRV_HANDLER,
                                                RBA_I2C_DET_ERR_JDP_POL_COMB_TX_LAST_BYTE_ACK_NOT_RECEIVED);
#endif

                                        /* Execute the Sequence End transmission process */
                                        rba_I2c_Prv_SeqEndTransmission(xSequenceId_u8, xHwId_u8, xLogicId_u8,
                                               xSeqPrio_en, rba_I2c_SeqFailed_e, RBA_I2C_ERR_SEQ_JDP_POL_COMB_TX_LAST_BYTE_ACK_NOT_RECEIVED);

                                    }
                                    }
                                }
                            }
                            else
                            {
                                /* Clear the I2C module interrupt*/
                                rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBSR |=  ((uint8)( 1uL << RBA_REG_I2C_IBSR_IBIF_POS ));

                                /* Send the data to I2C Bus */
                                rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBDR = rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runDataTx_pu8[ rba_I2c_Prv_SeqCount_st[ xLogicId_u8 ].dataTxPosition_u8 ];

                                /* Validate the Transmission */
                                xValidateStatus_u8 = rba_I2c_Prv_ValidateTransmissionStatus( xHwId_u8, rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runSeqTxlength_u8);

                                /* check the return status of "rba_I2c_Prv_ValidateTransmissionStatus" function */
                                if(xValidateStatus_u8 == RBA_I2C_JDP_TX_COMPLETE_CHK_FAILED)
                                {

                                    /* failure condition break the for loop */
                                    /* set communication status to failure */
                                    xCommunication_status_u8 = FALSE;


#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                                    /* Call DET error for Transmit not completed */
                                    (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_JDP_HW_PRV_HANDLER,
                                           RBA_I2C_DET_ERR_JDP_POL_COMB_TX_NOT_COMPLETED);
#endif

                                    /* Execute the Sequence End transmission process */
                                    rba_I2c_Prv_SeqEndTransmission(xSequenceId_u8, xHwId_u8, xLogicId_u8,
                                           xSeqPrio_en, rba_I2c_SeqFailed_e,  RBA_I2C_ERR_SEQ_JDP_POL_COMB_TX_NOT_COMPLETED);


                                }
                                else
                                {
								if(xValidateStatus_u8 == RBA_I2C_JDP_TX_ACK_NOT_RECEIVED_FAILED)
								{

                                    /* failure condition break the for loop */
                                    /* set communication status to failure */
                                    xCommunication_status_u8 = FALSE;

#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                                    /* Call DET error for Timeout slave interface */
                                    (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_JDP_HW_PRV_HANDLER,
                                           RBA_I2C_DET_ERR_JDP_POL_COMB_TX_ACK_NOT_RECEIVED);
#endif

                                    /* Execute the Sequence End transmission process */
                                    rba_I2c_Prv_SeqEndTransmission(xSequenceId_u8, xHwId_u8, xLogicId_u8,
                                           xSeqPrio_en, rba_I2c_SeqFailed_e, RBA_I2C_ERR_SEQ_JDP_POL_COMB_TX_ACK_NOT_RECEIVED);

                                }
                                }
                            }

                            /* for the error communication stop the I2C interface */
                            if(xCommunication_status_u8 == FALSE)
                            {
                                break;
                            }

                        }

                        /* check whether transmission is sucess */
                        if(xCommunication_status_u8 == TRUE)
                        {
                            if(rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runSeqRxlength_u8 > 1u)
                            {
                                /* Enable  the ACK for Transmission */
                                rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBCR &= ((uint8)~(1uL << RBA_REG_I2C_IBCR_NOACK_POS));
                            }
                            else
                            {
                                /* Disable the ACK for Transmission */
                                rba_I2c_Prv_Address_apcst[xHwId_u8]->IBCR |= (uint8)(1uL << RBA_REG_I2C_IBCR_NOACK_POS);
                            }

                            /* Set to I2C Read Mode*/
                            rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBCR &= ((uint8)~(1uL << RBA_REG_I2C_IBCR_TXRX_POS));

                            /* Dummy Read required for as mentioned the JDP User Manuel */
                            *(rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runDataRx_pu8) = (rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBDR);

                            /* Wait till complete the receive condition */
                            stRet = rba_I2c_Prv_RxComplete_Chk(xHwId_u8, rba_I2c_Prv_JdpSeq_Run_st[xLogicId_u8].runSeqRxlength_u8);

                            /* check the first byte transmission status failure*/
                            if( stRet == E_NOT_OK)
                            {
                                /* set communication status to failure */
                                xCommunication_status_u8 = FALSE;

#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                                /* Call DET error for Timeout slave interface */
                                (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_JDP_HW_PRV_HANDLER,
                                                      RBA_I2C_DET_ERR_JDP_POL_COMB_RX_DUMMY_NOT_COMPLETED);
#endif
                                /* Execute the Sequence End transmission process */
                                rba_I2c_Prv_SeqEndTransmission(xSequenceId_u8, xHwId_u8, xLogicId_u8,
                                       xSeqPrio_en, rba_I2c_SeqFailed_e, RBA_I2C_ERR_SEQ_JDP_POL_COMB_RX_DUMMY_NOT_COMPLETED);


                            }
                            else
                            {

                                for( countVar_u8 = 0u; countVar_u8 < rba_I2c_Prv_JdpSeq_Run_st[xLogicId_u8].runSeqRxlength_u8; countVar_u8++ )
                                {
                                    rba_I2c_Prv_SeqCount_st[xLogicId_u8].dataRxPosition_u8 = countVar_u8;

                                    if( ( rba_I2c_Prv_SeqCount_st[ xLogicId_u8 ].dataRxPosition_u8 + 1u) == rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runSeqRxlength_u8 )
                                    {

                                        /* Clear the Bus Interrupt Flag. */
                                        rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBSR |= (uint8)(1uL << RBA_REG_I2C_IBSR_IBIF_POS);

                                        /* Send Stop command to the Slave device*/
                                        rba_I2c_JdpStop(xHwId_u8);

                                        /* Read the Data from the Data Register */
                                        *(rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runDataRx_pu8) = ( rba_I2c_Prv_Address_apcst[xHwId_u8]->IBDR );

                                        /* Set to I2C Write Mode setting in the IBCR register*/
                                        rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBCR |= (uint8)( 1uL << RBA_REG_I2C_IBCR_TXRX_POS );

                                        /* Execute the Sequence End transmission process */
                                        rba_I2c_Prv_SeqEndTransmission(xSequenceId_u8, xHwId_u8, xLogicId_u8, xSeqPrio_en,
                                               rba_I2c_SeqCompleted_e, E_OK);

                                    }
                                    else if( ( (rba_I2c_Prv_SeqCount_st[ xLogicId_u8 ].dataRxPosition_u8) + 2u) == rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runSeqRxlength_u8 )
                                    {
                                        /* Disable the ACK before 2nd byte */
                                        rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBCR |= (uint8)( 1uL << RBA_REG_I2C_IBCR_NOACK_POS);

                                        /* Clear the I2C module interrupt*/
                                        rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBSR |= (uint8)( 1uL << RBA_REG_I2C_IBSR_IBIF_POS );

                                        /* Read the Data from the Data Register */
                                        *(rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runDataRx_pu8)  =
                                               ( rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBDR );

                                        /* Pointer increment Misra warring need place */
                                        rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runDataRx_pu8++;

                                        /* Wait till complete the receive condition */
                                        stRet = rba_I2c_Prv_RxComplete_Chk(xHwId_u8, rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runSeqRxlength_u8);

                                        /* check the first byte transmission status failure*/
                                        if( stRet == E_NOT_OK)
                                        {

                                            /* set communication status to failure */
                                            xCommunication_status_u8 = FALSE;


#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                                            /* Call DET errror for Timeout slave interface */
                                            (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_JDP_HW_PRV_HANDLER,
                                                                  RBA_I2C_DET_ERR_JDP_POL_COMB_RX_LAST_BYTE_NOT_COMPLETED);
#endif

                                            /* Execute the Sequence End transmission process */
                                            rba_I2c_Prv_SeqEndTransmission(xSequenceId_u8, xHwId_u8, xLogicId_u8,
                                                   xSeqPrio_en, rba_I2c_SeqFailed_e, RBA_I2C_ERR_SEQ_JDP_POL_COMB_RX_LAST_BYTE_NOT_COMPLETED);


                                        }
                                    }
                                    else
                                    {
                                        /* Clear the I2C module interrupt*/
                                        rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBSR |=  (uint8)( 1uL << RBA_REG_I2C_IBSR_IBIF_POS );

                                        /* Read the Data from the Data Register */
                                        *(rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runDataRx_pu8)  =
                                               ( rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBDR );

                                        /* Misra warring removal */
                                        rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runDataRx_pu8++;

                                        /* Wait till complete the receive condition */
                                        stRet = rba_I2c_Prv_RxComplete_Chk(xHwId_u8, rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runSeqRxlength_u8);

                                        /* check the first byte transmission status failure*/
                                        if( stRet == E_NOT_OK)
                                        {

                                            /* set communication status to failure */
                                            xCommunication_status_u8 = FALSE;

#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                                            /* Call DET error for Timeout slave interface */
                                            (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_JDP_HW_PRV_HANDLER,
                                                   RBA_I2C_DET_ERR_JDP_POL_COMB_RX_NOT_COMPLETED);
#endif

                                            /* Execute the Sequence End transmission process */
                                            rba_I2c_Prv_SeqEndTransmission(xSequenceId_u8, xHwId_u8, xLogicId_u8,
                                                                           xSeqPrio_en, rba_I2c_SeqFailed_e, RBA_I2C_ERR_SEQ_JDP_POL_COMB_RX_NOT_COMPLETED);


                                        }
                                    }


                                    /* for the error communication stop the I2C interface- aand break the "for" loop */
                                    if(xCommunication_status_u8 == FALSE)
                                    {
                                        break;
                                    }
                                }
                            }
                        }
                       /* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ Polling-Transmit combined Receive sequence execution End ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */
                    }
                   else
                   {
                       /* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ Polling-Receive sequence execution start ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */
                       if (rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runSeqComType_en == rba_I2c_Rx_e)
					   {
                       if(rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runSeqRxlength_u8 > 1u)
                       {
                           /* Enable  the ACK for Transmission */
                           rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBCR &= ((uint8)~(1uL << RBA_REG_I2C_IBCR_NOACK_POS));
                       }
                       else
                       {
                           /* Disable the ACK for Transmission */
                           rba_I2c_Prv_Address_apcst[xHwId_u8]->IBCR |= ((uint8)(1uL << RBA_REG_I2C_IBCR_NOACK_POS));
                       }

                       /* Set to I2C Read Mode*/
                       rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBCR &= ((uint8)~(1uL << RBA_REG_I2C_IBCR_TXRX_POS));

                       /* Dummy Read required for as mentioned the JDP User Manuel */
                       *(rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runDataRx_pu8) = (rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBDR);

                       /* Wait till complete the receive condition */
                       stRet = rba_I2c_Prv_RxComplete_Chk(xHwId_u8, rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runSeqRxlength_u8);

                       /* check the first byte transmission status failure*/
                       if( stRet == E_NOT_OK)
                       {
                           /* set communication status to failure */
                           xCommunication_status_u8 = FALSE;

#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                           /* Call DET error for Timeout slave interface */
                           (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_JDP_HW_PRV_HANDLER,
                                   RBA_I2C_DET_ERR_JDP_POL_RX_DUMMY_NOT_COMPLETED);
#endif
                           /* Execute the Sequence End transmission process */
                           rba_I2c_Prv_SeqEndTransmission(xSequenceId_u8, xHwId_u8, xLogicId_u8,
                                   xSeqPrio_en, rba_I2c_SeqFailed_e,
                                   RBA_I2C_ERR_SEQ_JDP_POL_RX_DUMMY_NOT_COMPLETED);


                       }
                       else
                       {

                           for( countVar_u8 = 0u; countVar_u8 < rba_I2c_Prv_JdpSeq_Run_st[xLogicId_u8].runSeqRxlength_u8; countVar_u8++ )
                           {

                               rba_I2c_Prv_SeqCount_st[xLogicId_u8].dataRxPosition_u8 = countVar_u8;

                               if( ( rba_I2c_Prv_SeqCount_st[ xLogicId_u8 ].dataRxPosition_u8 + 1u) == rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runSeqRxlength_u8  )
                               {
                                   /* Clear the Bus Interrupt Flag. */
                                   rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBSR |= (uint8)(1uL << RBA_REG_I2C_IBSR_IBIF_POS);

                                   /* Send Stop command to the Slave device*/
                                   rba_I2c_JdpStop(xHwId_u8);

                                   /* Read the Data from the Data Register */
                                   *(rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runDataRx_pu8) = ( rba_I2c_Prv_Address_apcst[xHwId_u8]->IBDR );

                                   /* Set to I2C Write Mode setting in the IBCR register*/
                                   rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBCR |= (uint8)( 1uL << RBA_REG_I2C_IBCR_TXRX_POS );

                                   /* Execute the Sequence End transmission process */
                                   rba_I2c_Prv_SeqEndTransmission(xSequenceId_u8, xHwId_u8, xLogicId_u8, xSeqPrio_en,
                                           rba_I2c_SeqCompleted_e, E_OK);


                               }
                               else if( ( (rba_I2c_Prv_SeqCount_st[ xLogicId_u8 ].dataRxPosition_u8) + 2u) == rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runSeqRxlength_u8  )
                               {
                                   /* Disable the ACK before 2nd byte */
                                   rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBCR |= (uint8)(1uL << RBA_REG_I2C_IBCR_NOACK_POS);

                                   /* Clear the I2C module interrupt*/
                                   rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBSR |= (uint8)( 1uL << RBA_REG_I2C_IBSR_IBIF_POS );

                                   /* Read the Data from the Data Register */
                                   *(rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runDataRx_pu8)  =
                                           ( rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBDR );

                                   /* Pointer increment Misra warring need place */
                                   rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runDataRx_pu8++;

                                   /* Wait till complete the receive condition */
                                   stRet = rba_I2c_Prv_RxComplete_Chk(xHwId_u8, rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runSeqRxlength_u8);

                                   /* check the first byte transmission status failure*/
                                   if( stRet == E_NOT_OK)
                                   {
                                       /* set communication status to failure */
                                       xCommunication_status_u8 = FALSE;

#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                                       /* Call DET error for Timeout slave interface */
                                       (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_JDP_HW_PRV_HANDLER,
                                               RBA_I2C_DET_ERR_JDP_POL_RX_LAST_BYTE_NOT_COMPLETED);
#endif

                                       /* Execute the Sequence End transmission process */
                                       rba_I2c_Prv_SeqEndTransmission(xSequenceId_u8, xHwId_u8, xLogicId_u8,
                                               xSeqPrio_en, rba_I2c_SeqFailed_e,
                                               RBA_I2C_ERR_SEQ_JDP_POL_RX_LAST_BYTE_NOT_COMPLETED);

                                   }
                               }
                               else
                               {
                                   /* Clear the I2C module interrupt*/
                                   rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBSR |=  (uint8)( 1uL << (RBA_REG_I2C_IBSR_IBIF_POS ));

                                   /* Read the Data from the Data Register */
                                   *(rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runDataRx_pu8)  =
                                           ( rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBDR );

                                   /* Misra warring removal */
                                   rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runDataRx_pu8++;

                                   /* Wait till complete the receive condition */
                                   stRet = rba_I2c_Prv_RxComplete_Chk(xHwId_u8, rba_I2c_Prv_JdpSeq_Run_st[ xLogicId_u8 ].runSeqRxlength_u8);

                                   /* check the first byte transmission status failure*/
                                   if( stRet == E_NOT_OK)
                                   {

                                       /* set communication status to failure */
                                       xCommunication_status_u8 = FALSE;

#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                                       /* Call DET error for Timeout slave interface */
                                       (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_JDP_HW_PRV_HANDLER,
                                               RBA_I2C_DET_ERR_JDP_POL_RX_NOT_COMPLETED);
#endif

                                       /* Execute the Sequence End transmission process */
                                       rba_I2c_Prv_SeqEndTransmission(xSequenceId_u8, xHwId_u8, xLogicId_u8,
                                               xSeqPrio_en, rba_I2c_SeqFailed_e, RBA_I2C_ERR_SEQ_JDP_POL_RX_NOT_COMPLETED);


                                   }
                               }

                               /* for the error communication stop the I2C interface */
                               if(xCommunication_status_u8 == FALSE)
                               {
                                   break;
                               }

                           }

                       }



                       /* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ Polling-Receive sequence execution End ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */
                   }
                   }
                }
            }   /*          if(rba_I2c_Prv_stInterruptMode_b == FALSE)*/
            /* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ Polling Mode Interface End ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */
        }
    }
    else
    {
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
        /* Call DET error for Timeout slave interface */
        (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_JDP_HW_PRV_HANDLER,
                RBA_I2C_DET_ERR_JDP_I2C_BUS_NOT_IDLE);
#endif


        /* Execute the Sequence End transmission process */
        rba_I2c_Prv_SeqEndTransmission(xSequenceId_u8, xHwId_u8, xLogicId_u8,
                xSeqPrio_en, rba_I2c_SeqFailed_e, RBA_I2C_ERR_SEQ_JDP_I2C_BUS_NOT_IDLE);
    }
}

#ifdef RBA_I2C_ENABLE_DEINIT
//- Functions -----------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
//! \brief Disable initialize driver specific stuff.
//! Uninitialized and handles the pre-condition settings for I2C communication. I2C protocol can support
//! more than one I2C module (hardware), hence init process shall vary based on the number of Configured I2C modules.
//!
//! <b>Sync/Async:</b> Synchronous
//!
//! <b>Reentrancy:</b> Non-reentrant
//!
//! \param idxHwUnit_qu8: I2C hardware unit ID .
//!
//! \return Void
//!
//! \pre none
//!
///////////////////////////////////////////////////////////////////////////////
void rba_I2c_Prv_Hw_DeInit(uint8 idxHwUnit_qu8)
{
    /* Clear the Present interrupt */
    rba_I2c_Prv_Address_apcst[ idxHwUnit_qu8 ]->IBSR |= (uint8)(1uL << RBA_REG_I2C_IBSR_IBIF_POS);

    /* Set to I2C Read Mode*/
    rba_I2c_Prv_Address_apcst[ idxHwUnit_qu8 ]->IBCR |= (uint8)(1uL << RBA_REG_I2C_IBCR_TXRX_POS);

    /* Disable the ACK for Transmission */
    rba_I2c_Prv_Address_apcst[ idxHwUnit_qu8 ]->IBCR |= (uint8)(1uL << RBA_REG_I2C_IBCR_NOACK_POS);

    /* Send Stop command to the Slave device*/
    rba_I2c_JdpStop(idxHwUnit_qu8);
}
#endif /* #ifdef RBA_I2C_ENABLE_DEINIT */

//- Functions -----------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
//! \brief
//! Validate the Transmit complete sequence.
//!
//!
//! <b>Sync/Async:</b> Synchronous
//!
//! <b>Reentrancy:</b> reentrant
//!
//! \param xHwId_u8:                 I2C Hardware ID
//!
//! \return \return E_OK is success, E_NOT_OK is failure execution
//!
//! \pre None
//!
///////////////////////////////////////////////////////////////////////////////
Std_ReturnType rba_I2c_Prv_TxComplete_Chk(uint8 xHwId_u8, uint8 runSeqTxlength_u8)
{
    /* Local variable */
    Std_ReturnType   stRet = E_NOT_OK;                /* Return status variable */
    uint32 current_Ticks_u32 = 0uL;
    uint32 initial_Ticks_u32 = Mcu_Rb_GetSysTicks();
    /*The waiting time is made for two times the Transmission length.
     * This buffer time is provided by taking into consideration,
     * the time needed to update the registers, stop bit, etc., */
    uint32 rba_I2C_Prv_JdpTimeout_Max_u32 = 2*runSeqTxlength_u8*RBA_I2C_CLK_LENGTH*rba_I2c_Clk;

    /* Timeout checking for First byte Transmission completed*/
    while(current_Ticks_u32 < rba_I2C_Prv_JdpTimeout_Max_u32)
    {

        if( ( ( (rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBSR)  & ( (uint8)(1uL << RBA_REG_I2C_IBSR_IBIF_POS) ) ) != 0 )
         && ( ( (rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBSR)  & ( (uint8)(1uL << RBA_REG_I2C_IBSR_TCF_POS) ) ) != 0 ) )
        {
            /* break counter value because Transmit completed successfully */
            current_Ticks_u32 = rba_I2C_Prv_JdpTimeout_Max_u32;

            /* Set the return value to TX completed success */
            stRet = E_OK;
        }
        else
        {
            /* Update the current system Tick value */
            current_Ticks_u32 = Mcu_Rb_GetSysTicks() - initial_Ticks_u32;
        }
    }

    /* Return the function status */
    return  stRet;
}

//- Functions -----------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
//! \brief
//! Validate the Receive sequence.
//!
//!
//! <b>Sync/Async:</b> Synchronous
//!
//! <b>Reentrancy:</b> reentrant
//!
//! \param xHwId_u8:                 I2C Hardware ID
//!
//! \return \return E_OK is success, E_NOT_OK is failure execution
//!
//! \pre None
//!
///////////////////////////////////////////////////////////////////////////////
Std_ReturnType rba_I2c_Prv_RxComplete_Chk(uint8 xHwId_u8, uint8 runSeqRxlength_u8)
{
    /* Local variable */
    Std_ReturnType   stRet = E_NOT_OK;   /* Return status variable */
    uint32 current_Ticks_u32 = 0uL;
    uint32 initial_Ticks_u32 = Mcu_Rb_GetSysTicks();
    /*The waiting time is made for two times the Receive sequence length.
     * This buffer time is provided by taking into consideration,
     * the time needed to update the registers, stop bit, etc., */
    uint32 rba_I2C_Prv_JdpTimeout_Max_u32 = 2*runSeqRxlength_u8*RBA_I2C_CLK_LENGTH*rba_I2c_Clk;

    /* Timeout checking for First byte Transmission completed*/
    while(current_Ticks_u32 < rba_I2C_Prv_JdpTimeout_Max_u32)
    {

        if ( ( ( (rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBSR)  & ( (uint8)(1uL << RBA_REG_I2C_IBSR_IBIF_POS) ) ) != 0 )
            && ( ( (rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBSR)  & ( (uint8)(1uL << RBA_REG_I2C_IBSR_TCF_POS) ) ) != 0 ) )
        {
            /* break counter value because Receive completed successfully */
            current_Ticks_u32 = rba_I2C_Prv_JdpTimeout_Max_u32;

            /* Set the return value to TX completed success */
            stRet = E_OK;
        }
        else
        {
            /* Update the current system Tick value */
            current_Ticks_u32 = Mcu_Rb_GetSysTicks() - initial_Ticks_u32;
        }
    }

/* Return the function status */
    return  stRet;
}
//- Functions -----------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
//! \brief
//! Validate the Acknowledge received for the sequence.
//!
//!
//! <b>Sync/Async:</b> Synchronous
//!
//! <b>Reentrancy:</b> reentrant
//!
//! \param xHwId_u8:                 I2C Hardware ID
//!
//! \return \return E_OK is success, E_NOT_OK is failure execution
//!
//! \pre None
//!
///////////////////////////////////////////////////////////////////////////////
Std_ReturnType rba_I2c_Prv_AckRecive_Chk(uint8 xHwId_u8)
{
    /* Local variable */
    Std_ReturnType    stRet = E_NOT_OK;   /* Return status variable */

    /* Check the Acknowledge is received */
    if( ( (rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBSR) & ( (uint8)(1uL << RBA_REG_I2C_IBSR_RXAK_POS) ) )  == 0 )
    {
        /* Set the return value to Acknowledge received */
        stRet = E_OK;
    }
    else
    {
        /* Set the return value to Acknowledge Failure */
        stRet = E_NOT_OK;
    }
    return stRet;
}

//- Functions -----------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
//! \brief
//! Last byte transmit data Validation. This function will check the Transfer complete status form the register and
//! validate the Receive acknowledge. if any one condition is failure return the failure status.
//!
//! <b>Sync/Async:</b> Synchronous
//!
//! <b>Reentrancy:</b> reentrant
//!
//! \param xHwId_u8:                 I2C Hardware ID
//!
//! \return \return E_OK is success, E_NOT_OK is failure execution
//!
//! \pre None
//!
///////////////////////////////////////////////////////////////////////////////
uint8 rba_I2c_Prv_ValidateTransmissionStatus(uint8 xHwId_u8, uint8 runSeqTxlength_u8)
{
    /* Local variable */
    Std_ReturnType       stVal = E_NOT_OK;
    uint8   rba_I2C_Prv_TxComplete_Chk_Status_u8 = RBA_I2C_JDP_TX_COMPLETE_CHK_PASS;
    /* ***************************get the first byte transmission stauts************************************************************** */
    stVal = rba_I2c_Prv_TxComplete_Chk(xHwId_u8, runSeqTxlength_u8);

    /* check the first byte transmission status failure*/
    if( stVal == E_NOT_OK)
    {
        rba_I2C_Prv_TxComplete_Chk_Status_u8 = RBA_I2C_JDP_TX_COMPLETE_CHK_FAILED;
    }
    else
    {
    /* Check the ACK */
         stVal = rba_I2c_Prv_AckRecive_Chk(xHwId_u8);

    /* check the first byte transmission status failure*/
         if( stVal == E_NOT_OK)
         {
             rba_I2C_Prv_TxComplete_Chk_Status_u8 = RBA_I2C_JDP_TX_ACK_NOT_RECEIVED_FAILED;
         }

    }
    return rba_I2C_Prv_TxComplete_Chk_Status_u8;

}

//- Functions -----------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
//! \brief
//! This function create delay to till I2C go to BUSY state.
//!
//!
//! <b>Sync/Async:</b> Synchronous
//!
//! <b>Reentrancy:</b> reentrant
//!
//! \param xHwId_u8:                 I2C Hardware ID
//!
//! \return : E_OK is sucess, E_NOT_OK is failure execution
//!
//! \pre None
//!
///////////////////////////////////////////////////////////////////////////////
Std_ReturnType rba_I2c_Prv_waitTill_BusBusy(uint8 xHwId_u8)
{
    /*wait till bus go to IDLE before start condition (wait till previous sequence send the STOP condition complete  )*/
    Std_ReturnType   stRet = E_NOT_OK;           /* Return status variable */
    uint32 current_Ticks_u32 = 0uL;
    uint32 initial_Ticks_u32 = Mcu_Rb_GetSysTicks();

    /* Timeout checking for First byte Transmission completed*/
    /* The waiting time is made for four times the I2c clock pulse.
     * This buffer time is provided by taking into consideration,
     * the time needed to update the registers, time needed by I2c to
     * switch , etc., */
    while(current_Ticks_u32 < (4u*rba_I2c_Clk))
    {
         /* check the bus enters into busy state*/
         if( ( (rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBSR)  &  ((uint8)(1uL << RBA_REG_I2C_IBSR_IBB_POS) ) ) != 0uL )
         {
             current_Ticks_u32 = 4u*rba_I2c_Clk;
             /* Set the return value to TX completed success */
             stRet  = E_OK;
         }
         else
         {
              /* Update the current system Tick value */
              current_Ticks_u32 = Mcu_Rb_GetSysTicks() - initial_Ticks_u32;
         }
    }
    /* Return the function status */
    return  stRet;
}

//- Functions -----------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
//! \brief
//! This function create delay to till I2C go to IDLE state.
//!
//!
//! <b>Sync/Async:</b> Synchronous
//! <b>Reentrancy:</b> reentrant
//!
//! \param xHwId_u8:                 I2C Hardware ID
//!
//! \return \return E_OK is success, E_NOT_OK is failure execution
//!
//! \pre None
//!
///////////////////////////////////////////////////////////////////////////////
Std_ReturnType rba_I2c_Prv_waitTill_BusIdle(uint8 xHwId_u8)
{
        Std_ReturnType   stRet = E_NOT_OK;           /* Return status variable */
        uint32 current_Ticks_u32 = 0uL;
        uint32 initial_Ticks_u32 = Mcu_Rb_GetSysTicks();

        /* Timeout checking for First byte Transmission completed*/
        /* The waiting time is made for four times the I2c clock pulse.
         * This buffer time is provided by taking into consideration,
         * the time needed to update the registers, time needed by I2c to
         * switch , etc., */
        while(current_Ticks_u32 < (4u*rba_I2c_Clk))
        {
             /* check the bus enters into busy state*/
            if( ( (rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBSR)  & ((uint8)(1uL << RBA_REG_I2C_IBSR_IBB_POS) ) ) == RBA_I2C_ZERO )
             {
			 /* Reset the counter */
                 current_Ticks_u32 = 4u*rba_I2c_Clk;
                 /* Set the return value to TX completed success */
                 stRet  = E_OK;
             }
             else
             {
                  /* Update the current system Tick value */
                  current_Ticks_u32 = Mcu_Rb_GetSysTicks() - initial_Ticks_u32;
             }
        }

    /* Return the function status */
    return  stRet;

}

//- Functions -----------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
//! \brief
//! for every byte transmission complte reset the I2C hardware registers and reset the I2C software variable.
//! <b>Sync/Async:</b> Synchronous
//!
//! <b>Reentrancy:</b> reentrant
//!
//! \param xSequenceId_u8:           request sequence
//! \param xHwId_u8:                 I2C Hardware ID
//! \param xLogicId_u8:              I2C Logical ID
//! \param xSeqPrio_en:              I2C sequence priorty
//! \param xSeqStatus:               I2C Logical ID
//! \param xErrorID_u8:              I2C Logical ID
//!
//! \return  void
//!
//! \pre None
//!
///////////////////////////////////////////////////////////////////////////////
void rba_I2c_Prv_SeqEndTransmission(uint8 xSequenceId_u8, uint8 xHwId_u8, uint8 xLogicId_u8,
        rba_I2c_PriorityType_ten xSeqPrio_en, rba_I2c_xSeqStatus_ten xSeqStatus, uint8 xErrorID_u8 )
{
    /* Enable the I2C bus and default setting */
    rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBCR |= (uint8)(1uL << RBA_REG_I2C_IBCR_NOACK_POS);

    /* Clear the Bus Interrupt Flag. */
    rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IBSR |= (uint8)(1uL << RBA_REG_I2C_IBSR_IBIF_POS);

    /* After complete the sequence increment the queue position*/
    rba_I2c_Prv_IncSendPosition_Queue( xLogicId_u8, xSeqPrio_en );

    /* Call end notification if present */
    if (rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[ xSequenceId_u8 ]->xSeqEndNotif_pfct != NULL_PTR)
    {
        (*rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[ xSequenceId_u8 ]->xSeqEndNotif_pfct)();
    }

    /* Set the BUS is not free error  */
    rba_I2c_Prv_JdpSeq_Run_st[xLogicId_u8].currentSeq->rba_I2c_xComErr_u8 = xErrorID_u8;

    /* Get the  data belonging to the index */
    rba_I2c_Prv_JdpSeq_Run_st[xLogicId_u8].currentSeq->rba_I2c_xSeqStatus_en = xSeqStatus;

    /* after complete the sequence execution set the Sequence status to IDLE */
    rba_I2c_Prv_stSequenceStatus_au8 [ xSequenceId_u8 ] = rba_I2c_QueueSequence_Idle_e;

    /* Sequence Handler status updates as Process Completed*/
    rba_I2c_Prv_stI2c_ChanRunStatus_en[ xLogicId_u8 ] = RBA_I2C_CHANNEL_NOT_RUNNING;
}

#define  RBA_I2C_STOP_SEC_CODE
#include RBA_I2C_CFG_MEMMAP_H

/* End of #ifdef RBA_I2C_CFG_MODULE_ACTIVE validation */
#endif

/* Enf of #if(MCU_RB_MACHINE_FAM == MCU_RB_JDP_UC1) validation */
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

