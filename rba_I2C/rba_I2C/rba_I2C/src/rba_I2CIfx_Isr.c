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
 * $Filename__:rba_I2CIfx_Isr.c$
 *
 * $Author____:MOR2ABT$
 *
 * $Function__:Transferred from BASD:
 *             COMP : rba_I2CIfx \ AR40.0.7.0; 0$
 *
 *************************************************************************
 * $Domain____:SDOM$
 * $User______:MTR1COB$
 * $Date______:17.10.2018$
 * $Class_____:SWSRC$
 * $Name______:rba_I2CIfx_Isr$
 * $Variant___:1.35.0$
 * $Revision__:0$
 * $Type______:C$
 * $State_____:AVAILABLE$
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
#include  "rba_I2CIfx.h"

#if (MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1)

#if(RBA_I2C_CFG_MODULE_ACTIVE == STD_ON)

#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif

#include   "rba_I2C_Cfg_SchM.h"
///////////////////////////////////////////////////////////////////////////////
//! \brief Function interface
//! This Isr routine will generated from the cpu, this request is created from
//! Transmit and Receive I2C request complete ISR for the I2C chalnnel-0 .
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

void rba_I2c_HwUnit0_Protocol_ISR_Proc(void)
{
#if (RBA_I2C_CFG_CHANNEL_CONF_AVAILABLE_0 == 1u)
    /* Local variables */

    uint8                     xSequenceId_u8 = 0u;                      /* Sequence ID. */
    uint8                     xSlaveId_u8 = 0u;                         /* Slave ID. */
    uint8                     *rxDataAdd_pu8;                           /* Sequence receive data address. */
    uint8                     xLogicId_u8=0u;                           /* Logical ID. */
    uint32                    cntrFifoRegData_Prv_u32;                  /* counter variable. */
    uint32                    xSlaveUniqueId_u8;                        /* Sequence configured Unique slave address. */
    uint32                    rba_I2C_Prv_IfxTimeout_Max_u32;           /* counter variable */
    rba_I2c_PriorityType_ten  xSeqPrio_en = RBA_I2C_SEQ_LOW_PRIORITY;   /* Sequence Priority status. */
    boolean                   xEndtransmit_u8 = TRUE;                   /* Status of last byte exectuion */
    uint32                    current_Ticks_u32 = 0uL;                  /* Current System ticks*/
    uint32                    initial_Ticks_u32 = 0uL;                  /* Initial System ticks*/

    /* Read the logical hardware Id from the slave configuration dataset.*/
    xLogicId_u8 = rba_I2c_prv_cfgConfigType_pcst->hwUnitConfig_pcst[RBA_I2C_CHANNEL_ZERO]->LogicHwId_u8;

    /* Read the Sequence ID */
    xSequenceId_u8 = rba_I2c_Prv_IfxSeq_Run_st[xLogicId_u8].runSequenceCfgId_u8;

    /* Read the Slave ID */
    xSlaveId_u8 = rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[xSequenceId_u8]->SlaveId;

    /* Read the slave address */
    xSlaveUniqueId_u8 = rba_I2c_prv_cfgConfigType_pcst->slaveConfig_pcst[xSlaveId_u8]->slaveAddress_u8;

    /* Read the sequence priority */
    xSeqPrio_en = rba_I2c_Prv_IfxSeq_Run_st[xLogicId_u8].runSeqPriority_u8;

    /* Check the communication bus as Transmit Mode. --->M3 and M4*/
    if( ( ( rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->BUSSTAT ) & ( 1uL << RBA_REG_I2C_BUSSTAT_RNW_POS ) ) == 0uL )
    {
        if( ( (rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->PIRQSS) & ( 1uL << RBA_REG_I2C_PIRQSS_NACK_POS ) ) != 0uL)
        {
            /*   Disable the I2C interface and write access enable to the I2C configuration registers -spec T1 process*/
            rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->RUNCTRL &= ~( RBA_I2C_IFX_RUNCTRL_RUN_ENABLE << RBA_REG_I2C_RUNCTRL_RUN_POS );  //0x01

            /* after complete the sequence execution set the Sequence status to IDLE */
            rba_I2c_Prv_stSequenceStatus_au8[ xSequenceId_u8 ] = rba_I2c_QueueSequence_Idle_e;

            /* Sequence status set to failed */
            rba_I2c_Prv_IfxSeq_Run_st[xLogicId_u8].currentSeq->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;

            /* Generate the DET error for the Acknowledge not received*/
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
            (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_IFX_HW_PRV_ISR_CORE_0, RBA_I2C_DET_ERR_ISR_ACK_NOT_RECEIVED);
#endif
        }
        else
        {
            if ( rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runSeqComType_en == rba_I2c_Tx_e )
            {
                /*   Disable the I2C interface and write access enable to the I2C configuration registers -spec T1 process*/
                rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->RUNCTRL &= ~( RBA_I2C_IFX_RUNCTRL_RUN_ENABLE << RBA_REG_I2C_RUNCTRL_RUN_POS );  //0x01

                /* after complete the sequence execution set the Sequence status to IDLE */
                rba_I2c_Prv_stSequenceStatus_au8[ xSequenceId_u8 ] = rba_I2c_QueueSequence_Idle_e;

                /* Get the  data belonging to the index */
                rba_I2c_Prv_IfxSeq_Run_st[xLogicId_u8].currentSeq->rba_I2c_xSeqStatus_en = rba_I2c_SeqCompleted_e;

            }
            else //combined Receive
            {
                /* Clear protocol interrupt request source */
                rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->PIRQSC = (( 1uL << RBA_REG_I2C_PIRQSC_NACK_POS )
                                                                          | ( 1uL << RBA_REG_I2C_PIRQSC_TX_END_POS )
                                                                          | ( 1uL << RBA_REG_I2C_PIRQSC_RX_POS ));

                /* Clear interrupt request source */
                rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->ICR = (( 1uL << RBA_REG_I2C_ICR_LSREQ_INT_POS )
                                                                       | ( 1uL << RBA_REG_I2C_ICR_SREQ_INT_POS )
                                                                       | ( 1uL << RBA_REG_I2C_ICR_LBREQ_INT_POS )
                                                                       | ( 1uL << RBA_REG_I2C_ICR_BREQ_INT_POS ));

                /*   Generate the Restart condition */
                rba_I2c_Prv_stI2c_ChanRunStatus_en[ xLogicId_u8 ] = RBA_I2C_CHANNEL_RUNNING;

                /* multicore lock */
			    SchM_Enter_rba_I2c_Driver();

                /* Receive Data Length*/
                rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->MRPSCTRL = rba_I2c_Prv_IfxSeq_Run_st[xLogicId_u8].runSeqRxlength_u8;

                /* written the number of bytes which have to be transmitted -------->M1*/
                rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->TPSCTRL = 0x01uL; // size of packet to transmit

                /* Push the every 32 bit data to FIFO */
                rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->TXD = ( 0uL << 24uL ) + ( 0uL << 16uL ) + ( 0uL << 8uL ) + ( xSlaveUniqueId_u8 + RBA_I2C_RECEIVE_MODE );

                /* Clear interrupt request source */
                 rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->ICR = (( 1uL << RBA_REG_I2C_ICR_LSREQ_INT_POS )
                                                                        | ( 1uL << RBA_REG_I2C_ICR_SREQ_INT_POS )
                                                                        | ( 1uL << RBA_REG_I2C_ICR_LBREQ_INT_POS )
                                                                        | ( 1uL << RBA_REG_I2C_ICR_BREQ_INT_POS ));
                /* multicore release */
			    SchM_Exit_rba_I2c_Driver();

                xEndtransmit_u8 = FALSE;//return;
            }
        }
    }
    else
    {
        if ( ( rba_I2c_Prv_IfxSeq_Run_st[xLogicId_u8].runSeqComType_en == rba_I2c_Rx_e ) || ( ( ( rba_I2c_Prv_Address_apcst[RBA_I2C_CHANNEL_ZERO]->PIRQSS ) & ( 1uL << RBA_REG_I2C_PIRQSS_TX_END_POS ) ) != 0uL ))
        {
            /* Read the Receive data address from the Sequence */
            rxDataAdd_pu8 = rba_I2c_Prv_IfxSeq_Run_st[xLogicId_u8].runDataRx_pu8;

            /* Timeout checking for Transmission completed*/
            cntrFifoRegData_Prv_u32 = 0uL;
            initial_Ticks_u32 = Mcu_Rb_GetSysTicks();
            rba_I2C_Prv_IfxTimeout_Max_u32 = 3*rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runSeqRxlength_u8*RBA_I2C_CLK_LENGTH*rba_I2c_Clk;
            while((current_Ticks_u32 < rba_I2C_Prv_IfxTimeout_Max_u32)&&( cntrFifoRegData_Prv_u32 < rba_I2c_Prv_IfxSeq_Run_st[xLogicId_u8].runSeqRxlength_u8))
            {
                if( rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->FFSSTAT !=  0uL )
                {
                    while(( rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ZERO ]->FFSSTAT !=  0uL ) && ( cntrFifoRegData_Prv_u32 < rba_I2c_Prv_IfxSeq_Run_st[xLogicId_u8].runSeqRxlength_u8))
                    {
                        /* Read the Data from the Data Register (FIFO) */
                        rxDataAdd_pu8[ cntrFifoRegData_Prv_u32 ] = ( (uint8)( rba_I2c_Prv_Address_apcst[RBA_I2C_CHANNEL_ZERO]->RXD ) );

                        /* Increment the data length receive counter */
                        cntrFifoRegData_Prv_u32++;
                    }
                }
                else
                {
                     /* Update the current system Tick value */
                     current_Ticks_u32 = Mcu_Rb_GetSysTicks() - initial_Ticks_u32;
                }
           }
            /*   write access enable to the I2C configuration registers and Disable the I2C bus */
            /* after complete the sequence execution set the Sequence status to IDLE */
            rba_I2c_Prv_stSequenceStatus_au8[ xSequenceId_u8 ] = rba_I2c_QueueSequence_Idle_e;

            /* Get the  data belonging to the index */
            rba_I2c_Prv_IfxSeq_Run_st[xLogicId_u8].currentSeq->rba_I2c_xSeqStatus_en = rba_I2c_SeqCompleted_e;
        }
        else
        {
             /* Clear protocol interrupt request source */
             rba_I2c_Prv_Address_apcst[RBA_I2C_CHANNEL_ZERO]->PIRQSC = (( 1uL << RBA_REG_I2C_PIRQSC_NACK_POS )
                       //| ( 1uL << RBA_REG_I2C_PIRQSC_TX_END_POS )
                         | ( 1uL << RBA_REG_I2C_PIRQSC_RX_POS ));

             /* Clear interrupt request source */
             rba_I2c_Prv_Address_apcst[RBA_I2C_CHANNEL_ZERO]->ICR = (( 1uL << RBA_REG_I2C_ICR_LSREQ_INT_POS )
                         | ( 1uL << RBA_REG_I2C_ICR_SREQ_INT_POS )
                         | ( 1uL << RBA_REG_I2C_ICR_LBREQ_INT_POS )
                         | ( 1uL << RBA_REG_I2C_ICR_BREQ_INT_POS ));

            /* Change the communication after complete the transmission */
            rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runSeqComType_en = rba_I2c_Rx_e;

            xEndtransmit_u8 = FALSE;
        }
    }

    if(xEndtransmit_u8 == TRUE)
    {
        /* Clear the All Protocol Interrupt*/
        rba_I2c_ClearInterrupt(RBA_I2C_CHANNEL_ZERO);

        /* Increment the Send Position in the queue */
        rba_I2c_Prv_IncSendPosition_Queue( xLogicId_u8, xSeqPrio_en );

        /* call end notification if present */
        if (rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[ xSequenceId_u8 ]->xSeqEndNotif_pfct != NULL_PTR)
        {
            (*rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[ xSequenceId_u8 ]->xSeqEndNotif_pfct)();
        }

        /* Check the Entries in Queue */
        if ( ( rba_I2c_Prv_posnQueueDynamic_ast[ xLogicId_u8 ].numEntries_u8 > 0u )
             ||  ( rba_I2c_Prv_posnQueueHiPriDynamic_ast[ xLogicId_u8 ].numEntries_u8 > 0u ) )
        {
            /* If entries available call to the Sequence handler function continuously */
            rba_I2c_Prv_Handler(xLogicId_u8);
        }
        else
        {
            /* Sequence Handler status updates as Process Completed*/
            rba_I2c_Prv_stI2c_ChanRunStatus_en[ xLogicId_u8 ] = RBA_I2C_CHANNEL_NOT_RUNNING;
        }
    }
#endif
}


///////////////////////////////////////////////////////////////////////////////
//! \brief Function interface
//! This Isr routine will generated from the cpu, this request is created from
//! Transmit and Receive I2C request complete ISR for the I2C chalnnel-1.
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
#if( (MCU_RB_MACHINE_DEV == MCU_RB_IFX_UC1_DEV4_ALL) || (MCU_RB_MACHINE_DEV == MCU_RB_IFX_UC1_DEV5_40NM_ALL) || (MCU_RB_MACHINE_DEV == MCU_RB_IFX_UC1_DEV4_40NM_ALL) )
void rba_I2c_HwUnit1_Protocol_ISR_Proc(void)
{

#if( RBA_I2C_CFG_CHANNEL_CONF_AVAILABLE_1 == 1u )
    /* Local variables */

    uint8                     xSequenceId_u8 = 0u;                      /* Sequence ID. */
    uint8                     xSlaveId_u8 = 0u;                         /* Slave ID. */
    uint8                     *rxDataAdd_pu8;                           /* Sequence receive data address. */
    uint8                     xLogicId_u8=0u;                           /* Logical ID. */
    uint32                    cntrFifoRegData_Prv_u32;                  /* counter variable. */
    uint32                    xSlaveUniqueId_u8;                        /* Sequence configured Unique slave address. */
    uint32                    rba_I2C_Prv_IfxTimeout_Max_u32;           /* counter variable */
    rba_I2c_PriorityType_ten  xSeqPrio_en = RBA_I2C_SEQ_LOW_PRIORITY;   /* Sequence Priority status. */
    boolean                   xEndtransmit_u8 = TRUE;                   /* Status of last byte exectuion */
    uint32                    current_Ticks_u32 = 0uL;                  /* Current System ticks*/
    uint32                    initial_Ticks_u32 = 0uL;                  /* Initial System ticks*/

    /* Read the logical hardware Id from the slave configuration dataset.*/
    xLogicId_u8 = rba_I2c_prv_cfgConfigType_pcst->hwUnitConfig_pcst[RBA_I2C_CHANNEL_ONE]->LogicHwId_u8;

    /* Read the Sequence ID */
    xSequenceId_u8 = rba_I2c_Prv_IfxSeq_Run_st[xLogicId_u8].runSequenceCfgId_u8;

    /* Read the Slave ID */
    xSlaveId_u8 = rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[xSequenceId_u8]->SlaveId;

    /* Read the slave address */
    xSlaveUniqueId_u8 = rba_I2c_prv_cfgConfigType_pcst->slaveConfig_pcst[xSlaveId_u8]->slaveAddress_u8;

    /* Read the sequence priority */
    xSeqPrio_en = rba_I2c_Prv_IfxSeq_Run_st[xLogicId_u8].runSeqPriority_u8;

    /* Check the communication bus as Transmit Mode. --->M3 and M4*/
    if( ( ( rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->BUSSTAT ) & ( 1uL << RBA_REG_I2C_BUSSTAT_RNW_POS ) ) == 0uL )
    {
        if( ( (rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->PIRQSS) & ( 1uL << RBA_REG_I2C_PIRQSS_NACK_POS ) ) != 0uL)
        {
            /*   Disable the I2C interface and write access enable to the I2C configuration registers -spec T1 process*/
            rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->RUNCTRL &= ~( RBA_I2C_IFX_RUNCTRL_RUN_ENABLE << RBA_REG_I2C_RUNCTRL_RUN_POS );  //0x01

            /* after complete the sequence execution set the Sequence status to IDLE */
            rba_I2c_Prv_stSequenceStatus_au8[ xSequenceId_u8 ] = rba_I2c_QueueSequence_Idle_e;

            /* Sequence status set to failed */
            rba_I2c_Prv_IfxSeq_Run_st[xLogicId_u8].currentSeq->rba_I2c_xSeqStatus_en = rba_I2c_SeqFailed_e;

            /* Generate the DET error for the Acknowledge not received*/
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
            (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_IFX_HW_PRV_ISR_CORE_1, RBA_I2C_DET_ERR_ISR_ACK_NOT_RECEIVED);
#endif
        }
        else
        {

            if ( rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runSeqComType_en == rba_I2c_Tx_e )
            {
                /*   Disable the I2C interface and write access enable to the I2C configuration registers -spec T1 process*/
                rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->RUNCTRL &= ~( RBA_I2C_IFX_RUNCTRL_RUN_ENABLE << RBA_REG_I2C_RUNCTRL_RUN_POS );  //0x01

                /* after complete the sequence execution set the Sequence status to IDLE */
                rba_I2c_Prv_stSequenceStatus_au8[ xSequenceId_u8 ] = rba_I2c_QueueSequence_Idle_e;

                /* Get the  data belonging to the index */
                rba_I2c_Prv_IfxSeq_Run_st[xLogicId_u8].currentSeq->rba_I2c_xSeqStatus_en = rba_I2c_SeqCompleted_e;
            }
            else //combined Receive
            {
                /* Clear protocol interrupt request source */
                rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->PIRQSC = (( 1uL << RBA_REG_I2C_PIRQSC_NACK_POS )
                                                                          | ( 1uL << RBA_REG_I2C_PIRQSC_TX_END_POS )
                                                                          | ( 1uL << RBA_REG_I2C_PIRQSC_RX_POS ));

                /* Clear interrupt request source */
                rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->ICR = (( 1uL << RBA_REG_I2C_ICR_LSREQ_INT_POS )
                                                                       | ( 1uL << RBA_REG_I2C_ICR_SREQ_INT_POS )
                                                                       | ( 1uL << RBA_REG_I2C_ICR_LBREQ_INT_POS )
                                                                       | ( 1uL << RBA_REG_I2C_ICR_BREQ_INT_POS ));

                /*   Generate the Restart condition */
                rba_I2c_Prv_stI2c_ChanRunStatus_en[ xLogicId_u8 ] = RBA_I2C_CHANNEL_RUNNING;

                /* multicore lock */
                SchM_Enter_rba_I2c_Driver();

                /* Receive Data Length*/
                rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->MRPSCTRL = rba_I2c_Prv_IfxSeq_Run_st[xLogicId_u8].runSeqRxlength_u8;

                /* written the number of bytes which have to be transmitted -------->M1*/
                rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->TPSCTRL = 0x01uL; // size of packet to transmit

                /* Push the every 32 bit data to FIFO */
                rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->TXD = ( 0uL << 24uL ) + ( 0uL << 16uL ) + ( 0uL << 8uL ) + ( xSlaveUniqueId_u8 + RBA_I2C_RECEIVE_MODE );

                /* Clear interrupt request source */
                 rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->ICR = (( 1uL << RBA_REG_I2C_ICR_LSREQ_INT_POS )
                                                                        | ( 1uL << RBA_REG_I2C_ICR_SREQ_INT_POS )
                                                                        | ( 1uL << RBA_REG_I2C_ICR_LBREQ_INT_POS )
                                                                        | ( 1uL << RBA_REG_I2C_ICR_BREQ_INT_POS ));

                /* multicore release */
                SchM_Exit_rba_I2c_Driver();

                xEndtransmit_u8 = FALSE;//return;
            }
        }
    }
    else
    {
        if ( ( rba_I2c_Prv_IfxSeq_Run_st[xLogicId_u8].runSeqComType_en == rba_I2c_Rx_e ) || ( ( ( rba_I2c_Prv_Address_apcst[RBA_I2C_CHANNEL_ONE]->PIRQSS ) & ( 1uL << RBA_REG_I2C_PIRQSS_TX_END_POS ) ) != 0uL ))
        {
            /* Read the Receive data address from the Sequence */
            rxDataAdd_pu8 = rba_I2c_Prv_IfxSeq_Run_st[xLogicId_u8].runDataRx_pu8;

            /* Timeout checking for Transmission completed*/
            cntrFifoRegData_Prv_u32 = 0uL;
            initial_Ticks_u32 = Mcu_Rb_GetSysTicks();
            rba_I2C_Prv_IfxTimeout_Max_u32 = 3*rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runSeqRxlength_u8*RBA_I2C_CLK_LENGTH*rba_I2c_Clk;
            while((current_Ticks_u32 < rba_I2C_Prv_IfxTimeout_Max_u32)&&( cntrFifoRegData_Prv_u32 < rba_I2c_Prv_IfxSeq_Run_st[xLogicId_u8].runSeqRxlength_u8))
            {
                if( rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->FFSSTAT !=  0uL )
                {
                    while(( rba_I2c_Prv_Address_apcst[ RBA_I2C_CHANNEL_ONE ]->FFSSTAT !=  0uL ) && ( cntrFifoRegData_Prv_u32 < rba_I2c_Prv_IfxSeq_Run_st[xLogicId_u8].runSeqRxlength_u8))
                    {
                                                                /* Read the Data from the Data Register (FIFO) */
                        rxDataAdd_pu8[ cntrFifoRegData_Prv_u32 ] = ( (uint8)( rba_I2c_Prv_Address_apcst[RBA_I2C_CHANNEL_ONE]->RXD ) );

                        /* Increment the data length receive counter */
                        cntrFifoRegData_Prv_u32++;
                    }
                }
                else
                {
                     /* Update the current system Tick value */
                     current_Ticks_u32 = Mcu_Rb_GetSysTicks() - initial_Ticks_u32;
                }
            }

            /*   write access enable to the I2C configuration registers and Disable the I2C bus */
            /* after complete the sequence execution set the Sequence status to IDLE */
            rba_I2c_Prv_stSequenceStatus_au8[ xSequenceId_u8 ] = rba_I2c_QueueSequence_Idle_e;

            /* Get the  data belonging to the index */
            rba_I2c_Prv_IfxSeq_Run_st[xLogicId_u8].currentSeq->rba_I2c_xSeqStatus_en = rba_I2c_SeqCompleted_e;
        }
        else
        {
             /* Clear protocol interrupt request source */
             rba_I2c_Prv_Address_apcst[RBA_I2C_CHANNEL_ONE]->PIRQSC = (( 1uL << RBA_REG_I2C_PIRQSC_NACK_POS )
                       //| ( 1uL << RBA_REG_I2C_PIRQSC_TX_END_POS )
                         | ( 1uL << RBA_REG_I2C_PIRQSC_RX_POS ));

             /* Clear interrupt request source */
             rba_I2c_Prv_Address_apcst[RBA_I2C_CHANNEL_ONE]->ICR = (( 1uL << RBA_REG_I2C_ICR_LSREQ_INT_POS )
                         | ( 1uL << RBA_REG_I2C_ICR_SREQ_INT_POS )
                         | ( 1uL << RBA_REG_I2C_ICR_LBREQ_INT_POS )
                         | ( 1uL << RBA_REG_I2C_ICR_BREQ_INT_POS ));

            /* Change the communication after complete the transmission */
            rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runSeqComType_en = rba_I2c_Rx_e;

            xEndtransmit_u8 = FALSE;
        }
    }

    if(xEndtransmit_u8 == TRUE)
    {
        /* Clear the All Protocol Interrupt*/
        rba_I2c_ClearInterrupt(RBA_I2C_CHANNEL_ONE);

        /* Increment the Send Position in the queue */
        rba_I2c_Prv_IncSendPosition_Queue( xLogicId_u8, xSeqPrio_en );

        /* call end notification if present */
        if (rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[ xSequenceId_u8 ]->xSeqEndNotif_pfct != NULL_PTR)
        {
            (*rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[ xSequenceId_u8 ]->xSeqEndNotif_pfct)();
        }

        /* Check the Entries in Queue */
        if ( ( rba_I2c_Prv_posnQueueDynamic_ast[ xLogicId_u8 ].numEntries_u8 > 0u )
             ||  ( rba_I2c_Prv_posnQueueHiPriDynamic_ast[ xLogicId_u8 ].numEntries_u8 > 0u ) )
        {
            /* If entries available call to the Sequence handler function continuously */
            rba_I2c_Prv_Handler(xLogicId_u8);
        }
        else
        {
            /* Sequence Handler status updates as Process Completed*/
            rba_I2c_Prv_stI2c_ChanRunStatus_en[ xLogicId_u8 ] = RBA_I2C_CHANNEL_NOT_RUNNING;
        }
    }
#endif
}
#endif
///////////////////////////////////////////////////////////////////////////////
//! \brief Function interface
//! This Isr routine will generated from the cpu,
//!
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
void rba_I2c_HwUnit0_ErrorStatus_ISR_Proc(void)
{

#if ( (RBA_I2C_CFG_CHANNEL_CONF_AVAILABLE_0 == 1u) && (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON) )

    /* FIFO Receive under flow error  */
    if( ( rba_I2c_Prv_Address_apcst[RBA_I2C_CHANNEL_ZERO]->ERRIRQSS ) != 0uL)
    {
        /* Call DET error for Timeout slave interface */
        (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_IFX_HW_PRV_ERR_ISR_0, RBA_I2C_DET_ERR_IFX_ISR_ERROR);

    }

#endif

}

///////////////////////////////////////////////////////////////////////////////
//! \brief Function interface
//! This Isr routine will generated from the cpu,
//!
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
#if( (MCU_RB_MACHINE_DEV == MCU_RB_IFX_UC1_DEV4_ALL) || (MCU_RB_MACHINE_DEV == MCU_RB_IFX_UC1_DEV5_40NM_ALL) || (MCU_RB_MACHINE_DEV == MCU_RB_IFX_UC1_DEV4_40NM_ALL) )
void rba_I2c_HwUnit1_ErrorStatus_ISR_Proc(void)
{

#if ( (RBA_I2C_CFG_CHANNEL_CONF_AVAILABLE_0 == 1u) && (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON) )

    /* FIFO Receive under flow error  */
    if( ( rba_I2c_Prv_Address_apcst[RBA_I2C_CHANNEL_ONE]->ERRIRQSS ) != 0uL)
    {
        /* Call DET error for Timeout slave interface */
        (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_IFX_HW_PRV_ERR_ISR_1, RBA_I2C_DET_ERR_IFX_ISR_ERROR);
    }

#endif
}
#endif

#define  RBA_I2C_STOP_SEC_CODE
#include RBA_I2C_CFG_MEMMAP_H



// #ifdef RBA_I2C_CFG_MODULE_ACTIVE
#endif

/* end of MCU_RB_MACHINE_FAM*/
#endif


/*! @} */
/*<RBHead>
 *************************************************************************
 * List Of Changes
 *
 * $History
 * 
 * 1.35.0; 0     17.10.2018 MTR1COB
 *   rba_I2C : high prio interrupt support.
 * 
 * 1.31.0; 0     01.11.2017 MTR1COB
 *   RQONE01255487 : IFX_DEV4 40nm controller support.
 * 
 * 1.29.1; 0     21.06.2017 MTR1COB
 *   rba_I2C bug fix for waiting maxtimeout error in polling mode.
 * 
 * 1.28.0; 2     20.01.2017 KMU8COB
 *   RQONE00956202 - FC-ARB : rba_I2c 1.28.0
 *   RQONE00990226 - [Mcal][rba_I2c]: Implementation of dynamic waiting time in 
 *   polling mode
 * 
 * 1.28.0; 1     11.01.2017 KMU8COB
 *   RQONE00956202 - FC-ARB : rba_I2c 1.28.0
 *   RQONE00990226 - [Mcal][rba_I2c]: Implementation of dynamic waiting time in 
 *   polling mode
 * 
 * 1.28.0; 0     04.01.2017 KMU8COB
 *   RQONE00956202 - FC-ARB : rba_I2c 1.28.0
 *   RQONE00990226 - [Mcal][rba_I2c]: Implementation of dynamic waiting time in 
 *   polling mode
 * 
 * 1.27.0; 0     05.10.2016 KMU8COB
 *   RQONE00933393 - FC-ARB : rba_I2c 1.27.0
 *   RQONE00953691 - [Mcal][rba_I2c]: Improvement of code coverage to maximum 
 *   percentage for IFX variant
 * 
 * 1.26.0; 1     19.07.2016 CJA8COB
 *   -
 * 
 * 1.26.0; 0     19.05.2016 CJA8COB
 *   RQONE00839710 - rba_i2c internal review point fixed
 * 
 * 1.24.0; 0     13.01.2016 MTR1COB
 *   unrequired return statements removed.
 * 
 * $
 *
 *************************************************************************
</RBHead>*/

