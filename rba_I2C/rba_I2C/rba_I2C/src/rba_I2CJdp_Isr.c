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

#include "rba_I2CJdp.h"

#if((MCU_RB_MACHINE_FAM == MCU_RB_JDP_UC1) && (MCU_RB_MACHINE_DEV != MCU_RB_JDP_UC1_DEV1_ALL))

#if(RBA_I2C_CFG_MODULE_ACTIVE == STD_ON)

#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif

///////////////////////////////////////////////////////////////////////////////
//! \brief Function interface
//! This Isr routine will generated from the cpu, this request is created from
//! Transmit and Receive request complete ISR for I2C channel-0.
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
#define  RBA_I2C_START_SEC_CODE
#include RBA_I2C_CFG_MEMMAP_H

void rba_I2c_HwUnit0_Isr_Proc(void)
{
#if(RBA_I2C_CFG_CHANNEL_CONF_AVAILABLE_0 == 1u)
    /* Local Variable */
    uint8                       xSequenceId_u8 = 0u;                            /* Sequence ID for the running sequence. */
    uint8                       idxLogicId_u8 = 0u;                             /* Logical ID for the I2C Hardware. */
    uint8                       xSeqCurrentPosition_u8 = 0u;                    /* Next sequence in the queue. */
    rba_I2c_PriorityType_ten    xSeqPrio_en = RBA_I2C_SEQ_LOW_PRIORITY;         /* Priority for the running sequence. */
    Std_ReturnType              stRet = E_NOT_OK;                               /* stauts value*/
    boolean                     xEndtransmit_u8 = TRUE;                         /* Status of last byte exectuion */


    //SchM_Enter_rba_I2c_Driver_ISR();

    /* Clear the Bus Interrupt Flag. */
    rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->IBSR |= ((uint8)(1uL << RBA_REG_I2C_IBSR_IBIF_POS));

    // Read the logical hardware Id from the slave configuration dataset.
    idxLogicId_u8 = rba_I2c_prv_cfgConfigType_pcst->hwUnitConfig_pcst[ RBA_I2C_CHANNEL_ZERO ]->LogicHwId_u8;

    /* Read the signal ID from the current running sequence*/
    xSequenceId_u8 = rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].currentSeq->rba_I2c_xSequenceCfgId_u8;

    /* Read the sequence priority */
    xSeqPrio_en = rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[xSequenceId_u8]->sequPrio_en;

    if(xSeqPrio_en == RBA_I2C_SEQ_LOW_PRIORITY)
    {
        /* Get the index of the next sequence which should be processed */
        xSeqCurrentPosition_u8 = rba_I2c_Prv_posnQueueDynamic_ast[ idxLogicId_u8 ].idxSendPos_u8;
    }
    else
    {
        /* Get the index of the next sequence which should be processed */
        xSeqCurrentPosition_u8 = rba_I2c_Prv_posnQueueHiPriDynamic_ast[ idxLogicId_u8 ].idxSendPos_u8;
    }

    /* Check the communication mode as Transmit  */
    if( ( (rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->IBCR) & ((uint8)(1u << RBA_REG_I2C_IBCR_TXRX_POS) ) ) != 0u )
    {

        /* Check whether the last byte of the transmission */
        if( (rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runSeqComType_en == rba_I2c_Tx_e)  && (rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataTxPosition_u8 >= rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runSeqTxlength_u8) )
        {
            /* after complete the sequence execution set the Sequence status to IDLE */
            rba_I2c_Prv_stSequenceStatus_au8[ xSequenceId_u8 ] = rba_I2c_QueueSequence_Idle_e;

            if(xSeqPrio_en == RBA_I2C_SEQ_LOW_PRIORITY)
            {
                /* Get the  data belonging to the index */
                rba_I2c_Prv_adrSequenceQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xSeqStatus_en = rba_I2c_SeqCompleted_e;
            }
            else
            {
                /* Get the  data belonging to the index  from high priority queue*/
                rba_I2c_Prv_adrSequenceHiPriQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xSeqStatus_en = rba_I2c_SeqCompleted_e;
            }

            /* Send Stop command to the Slave device*/
            rba_I2c_JdpStop( RBA_I2C_CHANNEL_ZERO );

        }
        else
        {
            /* Check whether the Acknowledge is not recived */
            if ( ( (rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->IBSR) & ((uint8)(1uL <<RBA_REG_I2C_IBSR_RXAK_POS) ) ) != 0u )
            {
                /* after complete the sequence execution set the Sequence status to IDLE */
                rba_I2c_Prv_stSequenceStatus_au8[ xSequenceId_u8 ] = rba_I2c_QueueSequence_Idle_e;

                if(xSeqPrio_en == RBA_I2C_SEQ_LOW_PRIORITY)
                {
                    /* Sequence status set to failed */
                    rba_I2c_Prv_adrSequenceQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;

                    /* Set the Sequence Error status to BUS is not free  */
                    rba_I2c_Prv_adrSequenceQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xComErr_u8 = RBA_I2C_ERR_SEQ_JDP_ISR_TX_ACK_NOT_RECEIVED;
                }
                else
                {
                    /* Sequence status set to failed */
                    rba_I2c_Prv_adrSequenceHiPriQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;

                    /* Set the Sequence Error status to BUS is not free  */
                    rba_I2c_Prv_adrSequenceHiPriQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xComErr_u8 = RBA_I2C_ERR_SEQ_JDP_ISR_TX_ACK_NOT_RECEIVED;
                }

                /* Call the DET error for the Acknowledge failure TX mode*/
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_JDP_HW_PRV_ISR_CORE_0, RBA_I2C_DET_ERR_JDP_ISR_TX_ACK_NOT_RECEIVED);
#endif

                /* Send Stop command to the Slave device*/
                rba_I2c_JdpStop( RBA_I2C_CHANNEL_ZERO );

            }
            else
            {
                if(rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runSeqComType_en == rba_I2c_Tx_e)
                {
                    /* Send the data to I2C Bus */
                    rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->IBDR = rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runDataTx_pu8[ rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataTxPosition_u8 ];

                    /* Increment the Transmit data position */
                    rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataTxPosition_u8++;

                    //SchM_Exit_rba_I2c_Driver_ISR();

                    xEndtransmit_u8 = FALSE;

                }
                else if(rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runSeqComType_en == rba_I2c_CombinedTxRx_e )
                {
                    /* check whether the last byte transmission is completed */
                    if( (rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runSeqComType_en == rba_I2c_CombinedTxRx_e ) && (rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataTxPosition_u8 >= rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runSeqTxlength_u8 ) )
                    {
                        /* In the combined TX-RX mode after complete the transmission enable Receive mode, for Send the Repeat Start command to the I2C Channel*/
                        rba_I2c_JdpReStart( RBA_I2C_CHANNEL_ZERO );

                        /* After the RE-START command wait till I2C bus go to busy state*/
                        stRet =  rba_I2c_Prv_waitTill_BusBusy( RBA_I2C_CHANNEL_ZERO);

                        /* Check whether the bus enter in to busy state in previous while condition */
                        if(stRet ==  E_NOT_OK )
                        {
                            /* after complete the sequence execution set the Sequence status to IDLE */
                            rba_I2c_Prv_stSequenceStatus_au8[ xSequenceId_u8 ] = rba_I2c_QueueSequence_Idle_e;

                            if(xSeqPrio_en == RBA_I2C_SEQ_LOW_PRIORITY)
                            {
                                /* Sequence status set to failed */
                                rba_I2c_Prv_adrSequenceQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;

                                /* Set the Sequence Error status to BUS is not Busy  */
                                rba_I2c_Prv_adrSequenceQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xComErr_u8 = RBA_I2C_ERR_SEQ_JDP_ISR_COMB_RX_TX_BUS_NOT_BUSY;
                            }
                            else
                            {
                                /* Sequence status set to failed */
                                rba_I2c_Prv_adrSequenceHiPriQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;

                                /* Set the Sequence Error status to BUS is not Busy  */
                                rba_I2c_Prv_adrSequenceHiPriQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xComErr_u8 = RBA_I2C_ERR_SEQ_JDP_ISR_COMB_RX_TX_BUS_NOT_BUSY;
                            }

                            /* Generate the DET error for the BUS is not enter in to BUSY state in combined TX mode*/
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                            (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_JDP_HW_PRV_ISR_CORE_0, RBA_I2C_DET_ERR_JDP_ISR_COMB_RX_TX_BUS_NOT_BUSY);
#endif

                            /* Send Stop command to the Slave device*/
                            rba_I2c_JdpStop( RBA_I2C_CHANNEL_ZERO );
                        }
                        else
                        {
                            /* Change the Communication mode form Transmit to receive */
                            rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runSeqComType_en = rba_I2c_Rx_e;

                            /* Send Slave ID with Readmode in I2C channel*/
                            rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->IBDR = rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].slaveID_u8 + 1u;

                            xEndtransmit_u8 = FALSE;
                        }
                    }
                    else
                    {
                        /* Send the data to I2C Bus */
                        rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->IBDR = rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runDataTx_pu8[ rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataTxPosition_u8 ];

                        /* Increment the Transmit data position */
                        rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataTxPosition_u8++;

                        xEndtransmit_u8 = FALSE;
                    }
                }
                else
                {
                    /* Set to I2C Read Mode*/
                    rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->IBCR &= ((uint8)~(1uL << RBA_REG_I2C_IBCR_TXRX_POS));

                    /* Dummy Read required for as mentioned the JDP User Manuel */
                    *(rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runDataRx_pu8) = (rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->IBDR);

                    if(rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runSeqRxlength_u8> 1uL)
                    {
                        /* Enable  the ACK for Transmission */
                        rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->IBCR &= ((uint8)~(1uL << RBA_REG_I2C_IBCR_NOACK_POS) );
                    }
                    else
                    {
                        /* Disable the ACK for Transmission */
                        rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->IBCR |= ((uint8)(1uL << RBA_REG_I2C_IBCR_NOACK_POS));
                    }

                    //SchM_Exit_rba_I2c_Driver_ISR();
                    xEndtransmit_u8 = FALSE;
                }
            }
        }
    }
    else
    {
        if(rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runSeqComType_en == rba_I2c_Rx_e)
        {
            /* check whether the last byte going to Receive */
            if( ( rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataRxPosition_u8 + 1uL) >= rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runSeqRxlength_u8 )
            {
                /* Set to I2C Write Mode*/
                rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->IBCR |= ((uint8)( 1uL << RBA_REG_I2C_IBCR_TXRX_POS ) );

                /* Send Stop command to the Slave device*/
                rba_I2c_JdpStop( RBA_I2C_CHANNEL_ZERO );

                /* Read the Data from the Data Register */
                *(rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runDataRx_pu8 + (rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataRxPosition_u8) ) = (rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->IBDR);

                /* after complete the sequence execution set the Sequence status to IDLE */
                rba_I2c_Prv_stSequenceStatus_au8[ xSequenceId_u8 ] = rba_I2c_QueueSequence_Idle_e;

                if(xSeqPrio_en == RBA_I2C_SEQ_LOW_PRIORITY)
                {
                    /* Set sequence status to completed */
                    rba_I2c_Prv_adrSequenceQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xSeqStatus_en = rba_I2c_SeqCompleted_e;
                }
                else
                {
                    /* Set sequence status to completed */
                    rba_I2c_Prv_adrSequenceHiPriQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xSeqStatus_en = rba_I2c_SeqCompleted_e;
                }
            }
            else if( ( (rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataRxPosition_u8) + 2u) == rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runSeqRxlength_u8  )
            {
                /* Disable the ACK for Transmission */
                rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->IBCR |= ( (uint8)(1uL << RBA_REG_I2C_IBCR_NOACK_POS) );

                /* Read the Data from the Data Register */
                *(rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runDataRx_pu8 + (rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataRxPosition_u8) )   = (rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->IBDR);

                /* Read Counter position increment */
                rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataRxPosition_u8++;


                xEndtransmit_u8 = FALSE;
            }
            else
            {
                /* Read the Data from the Data Register */
                *(rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runDataRx_pu8 + (rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataRxPosition_u8) )   = (rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->IBDR);

                /* Read Counter position increment */
                rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataRxPosition_u8++;


                xEndtransmit_u8 = FALSE;
            }
        }
        else
        {
            /* after complete the sequence execution set the Sequence status to IDLE */
            rba_I2c_Prv_stSequenceStatus_au8[ xSequenceId_u8 ] = rba_I2c_QueueSequence_Idle_e;

            if(xSeqPrio_en == RBA_I2C_SEQ_LOW_PRIORITY)
            {
                /* Sequence status set to failed */
                rba_I2c_Prv_adrSequenceQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;

                /* Set the BUS is not free error  */
                rba_I2c_Prv_adrSequenceQueue_ast[ idxLogicId_u8 ][ xSeqCurrentPosition_u8 ]->rba_I2c_xComErr_u8 = RBA_I2C_ERR_SEQ_JDP_ISR_COMTYPE_ERROR;
            }
            else
            {
                /* Sequence status set to failed */
                rba_I2c_Prv_adrSequenceHiPriQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;

                /* Set the BUS is not free error  */
                rba_I2c_Prv_adrSequenceHiPriQueue_ast[ idxLogicId_u8 ][ xSeqCurrentPosition_u8 ]->rba_I2c_xComErr_u8 = RBA_I2C_ERR_SEQ_JDP_ISR_COMTYPE_ERROR;
            }

#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
            (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_JDP_HW_PRV_ISR_CORE_0, RBA_I2C_DET_ERR_JDP_ISR_RX_COMM_TYPE);
#endif

            /* Send Stop command to the Slave device*/
            rba_I2c_JdpStop( RBA_I2C_CHANNEL_ZERO );
        }
    }


    if( xEndtransmit_u8 == TRUE )
    {
        /* Disable the I2C hardware */
        rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->IBCR = ((uint8)(1uL << RBA_REG_I2C_IBCR_MDIS_POS) );

        /* Enable the I2C bus and default setting */
        rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->IBCR = ((uint8)(1uL << RBA_REG_I2C_IBCR_NOACK_POS) );

        /* Increment the Queue Send position counter */
        rba_I2c_Prv_IncSendPosition_Queue( idxLogicId_u8, xSeqPrio_en );

        /* call end notification if present */
        if (rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[ xSequenceId_u8 ]->xSeqEndNotif_pfct != NULL_PTR)
        {
            (*rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[ xSequenceId_u8 ]->xSeqEndNotif_pfct)();
        }

        /* Check the Entries in Queue */
        if ( ( rba_I2c_Prv_posnQueueDynamic_ast[ idxLogicId_u8 ].numEntries_u8 > 0u )
             ||  ( rba_I2c_Prv_posnQueueHiPriDynamic_ast[ idxLogicId_u8 ].numEntries_u8 > 0u ) )
        {
            /* If entries available call to the Sequence handler function continouly */
            rba_I2c_Prv_Handler(idxLogicId_u8);
        }
        else
        {
            /* Sequence Handler status updates as Process Completed*/
            rba_I2c_Prv_stI2c_ChanRunStatus_en[ idxLogicId_u8 ] = RBA_I2C_CHANNEL_NOT_RUNNING;

        }

    }

#endif /* end of #if(RBA_I2C_CFG_CHANNEL_CONF_AVAILABLE_0 == 1u) validation */
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \brief Function interface
//! This Isr routine will generated from the cpu, this request is created from
//! Transmit and Receive request complete ISR for I2C channel-0.
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if(MCU_RB_MACHINE_DEV == MCU_RB_JDP_UC1_DEV4_ALL)
void rba_I2c_HwUnit1_Isr_Proc(void)
{
#if(RBA_I2C_CFG_CHANNEL_CONF_AVAILABLE_1 == 1u )
    /* Local Variable */
    uint8                       xSequenceId_u8 = 0u;                         /* Sequence ID for the running sequence. */
    uint8                       idxLogicId_u8 = 0u;                          /* Logical ID for the I2C Hardware. */
    uint8                       xSeqCurrentPosition_u8 = 0u;               /* Next sequence in the queue. */
    rba_I2c_PriorityType_ten    xSeqPrio_en=RBA_I2C_SEQ_LOW_PRIORITY;   /* Priority for the running sequence. */
    Std_ReturnType              stRet  = E_NOT_OK;
    boolean                     xEndtransmit_u8 = TRUE;

    /* Clear the Bus Interrupt Flag. */
    rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->IBSR |= ((uint8)(1uL << RBA_REG_I2C_IBSR_IBIF_POS) );

    // Read the logical hardware Id from the slave configuration dataset.
    idxLogicId_u8 = rba_I2c_prv_cfgConfigType_pcst->hwUnitConfig_pcst[ RBA_I2C_CHANNEL_ONE ]->LogicHwId_u8;

    /* Read the signal ID from the current running sequence*/
    xSequenceId_u8 = rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].currentSeq->rba_I2c_xSequenceCfgId_u8;

    /* Read the sequence priority */
    xSeqPrio_en = rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[xSequenceId_u8]->sequPrio_en;

    if(xSeqPrio_en == RBA_I2C_SEQ_LOW_PRIORITY)
    {
        /* Get the index of the next sequence which should be processed */
        xSeqCurrentPosition_u8 = rba_I2c_Prv_posnQueueDynamic_ast[ idxLogicId_u8 ].idxSendPos_u8;
    }
    else
    {
        /* Get the index of the next sequence which should be processed for the HIGH priority */
        xSeqCurrentPosition_u8 = rba_I2c_Prv_posnQueueHiPriDynamic_ast[ idxLogicId_u8 ].idxSendPos_u8;
    }

    /* Check the communication mode as Transmit  */
    if (((rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->IBCR) & ( (uint8)(1uL << RBA_REG_I2C_IBCR_TXRX_POS ) ) ) != 0u)
    {

        /* Check the Transmission has Completed. */
        if( (rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runSeqComType_en == rba_I2c_Tx_e)  && (rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataTxPosition_u8 >= rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runSeqTxlength_u8) )
        {
            /* after complete the sequence execution set the Sequence status to IDLE */
            rba_I2c_Prv_stSequenceStatus_au8[ xSequenceId_u8 ] = rba_I2c_QueueSequence_Idle_e;

            if(xSeqPrio_en == RBA_I2C_SEQ_LOW_PRIORITY)
            {
            /* Get the  data belonging to the index */
            rba_I2c_Prv_adrSequenceQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xSeqStatus_en = rba_I2c_SeqCompleted_e;
            }
            else
            {
                /* Get the  data belonging to the index of HIGH priority queue */
                rba_I2c_Prv_adrSequenceHiPriQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xSeqStatus_en = rba_I2c_SeqCompleted_e;
            }

            /* Generate Stop signal */
            rba_I2c_JdpStop( RBA_I2C_CHANNEL_ONE );

        }
        else
        {
            /* Check whether the Acknowledge is received or not */
            if( ( (rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->IBSR) & ((uint8)(1uL << RBA_REG_I2C_IBSR_RXAK_POS) ) ) != 0u)
            {
                /* after complete the sequence execution set the Sequence status to IDLE */
                rba_I2c_Prv_stSequenceStatus_au8[ xSequenceId_u8 ] = rba_I2c_QueueSequence_Idle_e;

                if(xSeqPrio_en == RBA_I2C_SEQ_LOW_PRIORITY)
                {
                    /* Sequence status set to failed */
                    rba_I2c_Prv_adrSequenceQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;

                    /* Set the BUS is not free error  */
                    rba_I2c_Prv_adrSequenceQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xComErr_u8 = RBA_I2C_ERR_SEQ_JDP_ISR_TX_ACK_NOT_RECEIVED;
                }
                else
                {
                    /* Sequence status set to failed */
                    rba_I2c_Prv_adrSequenceHiPriQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;

                    /* Set the BUS is not free error  */
                    rba_I2c_Prv_adrSequenceHiPriQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xComErr_u8 = RBA_I2C_ERR_SEQ_JDP_ISR_TX_ACK_NOT_RECEIVED;
                }

                /* Call the DET error for the Acknowledge failure TX mode*/
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_JDP_HW_PRV_ISR_CORE_1, RBA_I2C_DET_ERR_JDP_ISR_TX_ACK_NOT_RECEIVED);
#endif

                /* Generate the stop signal */
                rba_I2c_JdpStop( RBA_I2C_CHANNEL_ONE );

            }
            else
            {
                if(rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runSeqComType_en == rba_I2c_Tx_e)
                {
                    /* Send the data to I2C Bus */
                    rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->IBDR = rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runDataTx_pu8[ rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataTxPosition_u8 ];

                    /* Increment the Transmit data position */
                    rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataTxPosition_u8++;

                    //SchM_Exit_rba_I2c_Driver_ISR();

                    xEndtransmit_u8 = FALSE;

                }
                else if(rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runSeqComType_en == rba_I2c_CombinedTxRx_e )
                {
                    /* check whether the last byte transmission is completed */
                    if( (rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runSeqComType_en == rba_I2c_CombinedTxRx_e ) && (rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataTxPosition_u8 >= rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runSeqTxlength_u8 ) )
                    {
                        /* In the combined TX-RX mode after complete the transmission enable Receive mode, for Send the Repeat Start command to the I2C Channel*/
                        rba_I2c_JdpReStart( RBA_I2C_CHANNEL_ONE );

                        /* After the RE-START command wait till I2C bus go to busy state*/
                        stRet =  rba_I2c_Prv_waitTill_BusBusy( RBA_I2C_CHANNEL_ONE);

                        /* Check whether the bus enter in to busy state in previous while condition */
                        if(stRet == E_NOT_OK )
                        {
                            /* after complete the sequence execution set the Sequence status to IDLE */
                            rba_I2c_Prv_stSequenceStatus_au8[ xSequenceId_u8 ] = rba_I2c_QueueSequence_Idle_e;

                            if(xSeqPrio_en == RBA_I2C_SEQ_LOW_PRIORITY)
                            {
                                /* Sequence status set to failed */
                                rba_I2c_Prv_adrSequenceQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;

                                /* Set the BUS is not free error  */
                                rba_I2c_Prv_adrSequenceQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xComErr_u8 = RBA_I2C_ERR_SEQ_JDP_ISR_COMB_RX_TX_BUS_NOT_BUSY;
                            }
                            else
                            {
                                /* Sequence status set to failed */
                                rba_I2c_Prv_adrSequenceHiPriQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;

                                /* Set the Sequence Error status to BUS is not Busy  */
                                rba_I2c_Prv_adrSequenceHiPriQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xComErr_u8 = RBA_I2C_ERR_SEQ_JDP_ISR_COMB_RX_TX_BUS_NOT_BUSY;
                            }

                            /* Call the DET error for the BUS is not enter in to BUSY state in combined TX mode*/
            #if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                                (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_JDP_HW_PRV_ISR_CORE_1, RBA_I2C_DET_ERR_JDP_ISR_COMB_RX_TX_BUS_NOT_BUSY);
            #endif
                                    rba_I2c_JdpStop( RBA_I2C_CHANNEL_ONE );
                        }
                        else
                        {
                            /* Change the Communication mode form Transmit to receive */
                            rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runSeqComType_en = rba_I2c_Rx_e;

                            /* Send Slave ID in I2C channel*/
                            rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->IBDR = rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].slaveID_u8 + 1u;

                            xEndtransmit_u8 = FALSE;
                        }
                    }
                    else
                    {
                        /* Send the data to I2C Bus */
                        rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->IBDR = rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runDataTx_pu8[ rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataTxPosition_u8 ];

                        /* Increment the Transmit data position */
                        rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataTxPosition_u8++;

                        xEndtransmit_u8 = FALSE;
                    }
                }
                else
                {
                    /* Set to I2C Read Mode*/
                    rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->IBCR &=  ((uint8)~(1uL << RBA_REG_I2C_IBCR_TXRX_POS) );

                    /* Dummy Read required for as mentioned the JDP User Manuel */
                    *(rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runDataRx_pu8) = (rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->IBDR);

                    if(rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runSeqRxlength_u8 > 1uL)
                    {
                        /* Enable  the ACK for Transmission */
                        rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->IBCR &=  ((uint8)~(1uL << RBA_REG_I2C_IBCR_NOACK_POS));
                    }
                    else
                    {
                        /* Disable the ACK for Transmission */
                        rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->IBCR |= ((uint8)(1uL << RBA_REG_I2C_IBCR_NOACK_POS));
                    }

                    //SchM_Exit_rba_I2c_Driver_ISR();
                    xEndtransmit_u8 = FALSE;
                }
            }
        }
    }
    else
    {
        if(rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runSeqComType_en == rba_I2c_Rx_e)
        {
            /* check whether the last byte going to Receive */
            if( ( rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataRxPosition_u8 + 1uL) >= rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runSeqRxlength_u8 )
            {
                /* Set to I2C Write Mode*/
                rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->IBCR |= ((uint8)( 1uL << RBA_REG_I2C_IBCR_TXRX_POS ));

                /* Send Stop command to the Slave device*/
                rba_I2c_JdpStop( RBA_I2C_CHANNEL_ONE );

                /* Disable the I2C hardware */
                *(rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runDataRx_pu8 + (rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataRxPosition_u8) ) = (rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->IBDR);

                /* after complete the sequence execution set the Sequence status to IDLE */
                rba_I2c_Prv_stSequenceStatus_au8[ xSequenceId_u8 ] = rba_I2c_QueueSequence_Idle_e;

                if(xSeqPrio_en == RBA_I2C_SEQ_LOW_PRIORITY)
                {
                    /* Set sequence status to completed */
                    rba_I2c_Prv_adrSequenceQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xSeqStatus_en = rba_I2c_SeqCompleted_e;
                }
                else
                {
                    /* Set sequence status to completed */
                    rba_I2c_Prv_adrSequenceHiPriQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xSeqStatus_en = rba_I2c_SeqCompleted_e;
                }
            }
            else if( ( (rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataRxPosition_u8) + 2u) == rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runSeqRxlength_u8  )
            {
                /* Disable the ACK for Transmission */
                rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->IBCR |= ((uint8)(1uL << RBA_REG_I2C_IBCR_NOACK_POS));

                /* If entries available call to the Sequence handler function continouly */
                *(rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runDataRx_pu8 + (rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataRxPosition_u8) )   = (rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->IBDR);

                /* Sequence Handler status updates as Process Completed*/
                rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataRxPosition_u8++;


                xEndtransmit_u8 = FALSE;
            }
            else
            {
                /* Read the Data from the Data Register */
                *(rba_I2c_Prv_JdpSeq_Run_st[ idxLogicId_u8 ].runDataRx_pu8 + (rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataRxPosition_u8) )   = (rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->IBDR);

                /* Read Counter position increment */
                rba_I2c_Prv_SeqCount_st[ idxLogicId_u8 ].dataRxPosition_u8++;


                xEndtransmit_u8 = FALSE;
            }
        }
        else
        {
            /* after complete the sequence execution set the Sequence status to IDLE */
            rba_I2c_Prv_stSequenceStatus_au8[ xSequenceId_u8 ] = rba_I2c_QueueSequence_Idle_e;

            if(xSeqPrio_en == RBA_I2C_SEQ_LOW_PRIORITY)
            {
                /* Sequence status set to failed */
                rba_I2c_Prv_adrSequenceQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;

                /* Set the BUS is not free error  */
                rba_I2c_Prv_adrSequenceQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xComErr_u8 = RBA_I2C_ERR_SEQ_JDP_ISR_COMTYPE_ERROR;
            }
            else
            {

                rba_I2c_Prv_adrSequenceHiPriQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;

                /* Set the BUS is not free error  */
                rba_I2c_Prv_adrSequenceHiPriQueue_ast[ idxLogicId_u8 ] [ xSeqCurrentPosition_u8 ]->rba_I2c_xComErr_u8 = RBA_I2C_ERR_SEQ_JDP_ISR_COMTYPE_ERROR;
            }

#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
            (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_JDP_HW_PRV_ISR_CORE_1, RBA_I2C_DET_ERR_JDP_ISR_RX_COMM_TYPE);
#endif
            rba_I2c_JdpStop( RBA_I2C_CHANNEL_ONE );
        }
    }

    /* Send Stop command to the Slave device*/

    if( xEndtransmit_u8 == TRUE )
    {
	    /* Disable the I2C hardware */
	    rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->IBCR = ((uint8)(1uL << RBA_REG_I2C_IBCR_MDIS_POS));

	    /* Enable the I2C bus and default setting */
	    rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->IBCR = ((uint8)(1uL << RBA_REG_I2C_IBCR_NOACK_POS));

	    /* Increment the Queue Send position counter */
	    rba_I2c_Prv_IncSendPosition_Queue( idxLogicId_u8, xSeqPrio_en );

	    /* call end notification if present */
	    if (rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[ xSequenceId_u8 ]->xSeqEndNotif_pfct != NULL_PTR)
	    {
	        (*rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[ xSequenceId_u8 ]->xSeqEndNotif_pfct)();
	    }
	    /* Check the Entries in Queue */
	    if ( ( rba_I2c_Prv_posnQueueDynamic_ast[ idxLogicId_u8 ].numEntries_u8 > 0u )
	         ||  ( rba_I2c_Prv_posnQueueHiPriDynamic_ast[ idxLogicId_u8 ].numEntries_u8 > 0u ) )
	    {
	        /* If entries available call to the Sequence handler function continuously */
	        rba_I2c_Prv_Handler(idxLogicId_u8);
	    }
	    else
	    {
	        /* Sequence Handler status updates as Process Completed*/
	        rba_I2c_Prv_stI2c_ChanRunStatus_en[ idxLogicId_u8 ] = RBA_I2C_CHANNEL_NOT_RUNNING;

	    }
    }

#endif  //end of #if(RBA_I2C_CFG_CHANNEL_CONF_AVAILABLE_1 == 1u )
}
#endif

#define  RBA_I2C_STOP_SEC_CODE
#include RBA_I2C_CFG_MEMMAP_H

/* end of  "RBA_I2C_CFG_MODULE_ACTIVE" */
#endif

/* end of #if(MCU_RB_MACHINE_FAM == MCU_RB_JDP_UC1) */
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
