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

#include   "rba_I2CIfx.h"

#if (MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1)


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

/* Gloabal variable */

#define  RBA_I2C_START_SEC_VAR_CLEARED_UNSPECIFIED
#include RBA_I2C_CFG_MEMMAP_H
/* Used sequence variable */
rba_I2c_Seq_Run_tst rba_I2c_Prv_IfxSeq_Run_st[ RBA_I2C_CFG_MAX_CONFIGURED_CHANNELS ];

#define  RBA_I2C_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include RBA_I2C_CFG_MEMMAP_H

#define  RBA_I2C_START_SEC_VAR_INIT_UNSPECIFIED
#include RBA_I2C_CFG_MEMMAP_H
/* Port Address Array */
rba_Reg_I2C_tst* rba_I2c_Prv_Address_apcst[] =
{    /*  Address                           Description  */
    (rba_Reg_I2C_tst*)   &I2C0,                  /* Index  0  PORT_00 */
#if( (MCU_RB_MACHINE_DEV == MCU_RB_IFX_UC1_DEV4_ALL) || (MCU_RB_MACHINE_DEV == MCU_RB_IFX_UC1_DEV5_40NM_ALL) || (MCU_RB_MACHINE_DEV == MCU_RB_IFX_UC1_DEV4_40NM_ALL) )
    (rba_Reg_I2C_tst*)   &I2C1,                  /* Index  1  PORT_02 */
#endif
};



#define  RBA_I2C_STOP_SEC_VAR_INIT_UNSPECIFIED
#include RBA_I2C_CFG_MEMMAP_H


//- Functions -----------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
//! \brief Initialize processor specific stuff.
//!  this function is common IFX controller , Setting the
//!      I2C control register, and clear the interrupt status.
//!
//! <b>Sync/Async:</b> Synchronous
//! <b>Reentrancy:</b> Non-reentrant
//!
//! \param configuration_pcst: Pointer to post-build configuration.
//!
//! \return E_OK if the initialization was successful, E_NOT_OK if not.
//!
//! \pre none
//!
//! \post I2cIfx is ready for use.
//!
///////////////////////////////////////////////////////////////////////////////
#define  RBA_I2C_START_SEC_CODE
#include RBA_I2C_CFG_MEMMAP_H

void rba_I2c_Prv_Hw_Init(uint8 idxHwUnit_qu8)
{
#if( (MCU_RB_MACHINE_DEV == MCU_RB_IFX_UC1_DEV4_ALL) || (MCU_RB_MACHINE_DEV == MCU_RB_IFX_UC1_DEV5_40NM_ALL) || (MCU_RB_MACHINE_DEV == MCU_RB_IFX_UC1_DEV4_40NM_ALL) )
    if( RBA_I2C_IFX_MAX_HW_CHANNEL_DEV5_OR_DEV4 > idxHwUnit_qu8 )
#else
    if( RBA_I2C_IFX_MAX_HW_CHANNEL_DEV3_OR_DEV2 > idxHwUnit_qu8 )
#endif

    {
        /* enable I2c Module0 clock -> enable I2C HW module */
        if (rba_I2c_Prv_Address_apcst[ idxHwUnit_qu8 ]->CLC != RBA_I2C_IFX_MODE_CLC_MODUEL_ENABLE )
        {
            Mcu_Rb_ResetEndInit();
            rba_I2c_Prv_Address_apcst[ idxHwUnit_qu8 ]->CLC &= ( ~( RBA_I2C_IFX_MODE_CLC_MODUEL_DISABLE << RBA_REG_I2C_CLC_DISR_POS));
            Mcu_Rb_SetEndInit();
        }
        /* Baud rate setting from the Fmax  100KHz */
        rba_I2c_Prv_Address_apcst[ idxHwUnit_qu8 ]->CLC1 = RBA_I2C_IFX_NORMAL_MODE_CLC1_VALUE;

        /* for the initial state Port interface setting SDAxA and SCLxA are selected */
        rba_I2c_Prv_Address_apcst[ idxHwUnit_qu8 ]->GPCTL = (uint32)( (uint32)RBA_I2C_IFX_PORTSELECT_A << RBA_REG_I2C_GPCTL_PISEL_POS );

        /*   write access to configuration registers enabled(Disable the communication mode) */
        rba_I2c_Prv_Address_apcst[ idxHwUnit_qu8 ]->RUNCTRL &= ( ~( RBA_I2C_IFX_RUNCTRL_RUN_ENABLE << RBA_REG_I2C_RUNCTRL_RUN_POS ) );

        /*  set the Fractional Divider for the clock - Default setting as 100KHz*/
        rba_I2c_Prv_Address_apcst[ idxHwUnit_qu8 ]->FDIVCFG = RBA_I2C_IFX_NORMAL_MODE_FDIVCFG_VALUE;

        /* Timing configuration Register initial setting as 100KHz*/
        rba_I2c_Prv_Address_apcst[ idxHwUnit_qu8 ]->TIMCFG = RBA_I2C_IFX_NORMAL_MODE_TIMCFG_VAL;

        /* Configure the FIFO register as FLOW control mode for Transmit and receive */
        rba_I2c_Prv_Address_apcst[ idxHwUnit_qu8 ]->FIFOCFG = ( RBA_I2C_IFX_FIFOCFG_RXBS_VAL << RBA_REG_I2C_FIFOCFG_RXBS_POS)
                                                            | ( RBA_I2C_IFX_FIFOCFG_TXBS_VAL << RBA_REG_I2C_FIFOCFG_TXBS_POS)
                                                            | ( RBA_I2C_IFX_FIFOCFG_RXFA_VAL << RBA_REG_I2C_FIFOCFG_RXFA_POS)
                                                            | ( RBA_I2C_IFX_FIFOCFG_TXFA_VAL << RBA_REG_I2C_FIFOCFG_TXFA_POS)
                                                            | ( RBA_I2C_IFX_FIFOCFG_TXFC_VAL << RBA_REG_I2C_FIFOCFG_TXFC_POS)
                                                            | ( RBA_I2C_IFX_FIFOCFG_RXFC_VAL << RBA_REG_I2C_FIFOCFG_RXFC_POS);

        /* Configure Address Register to set the 7 address mode and set to Master  */
        rba_I2c_Prv_Address_apcst[ idxHwUnit_qu8 ]->ADDRCFG = ( RBA_I2C_IFX_ADDRCFG_ADR_VAL << RBA_REG_I2C_ADDRCFG_ADR_POS)
                                                            | ( RBA_I2C_IFX_ADDRCFG_TBAM_VAL << RBA_REG_I2C_ADDRCFG_TBAM_POS)
                                                            | ( RBA_I2C_IFX_ADDRCFG_MNS_VAL << RBA_REG_I2C_ADDRCFG_MNS_POS)
                                                            | ( RBA_I2C_IFX_ADDRCFG_SONA_VAL << RBA_REG_I2C_ADDRCFG_SONA_POS)
                                                            | ( RBA_I2C_IFX_ADDRCFG_SOPE_VAL << RBA_REG_I2C_ADDRCFG_SOPE_POS);

        /* Enable the Protocol interrupt Mask setting */
        rba_I2c_Prv_Address_apcst[ idxHwUnit_qu8 ]->PIRQSM = ( RBA_I2C_IFX_PIRQSM_NACK_ENABLE << RBA_REG_I2C_PIRQSM_NACK_POS)
                                                           | ( RBA_I2C_IFX_PIRQSM_TX_END_ENABLE << RBA_REG_I2C_PIRQSM_TX_END_POS)
                                                           | ( RBA_REG_I2C_PIRQSM_RX_ENABLE << RBA_REG_I2C_PIRQSM_RX_POS);

        /* Enable the all Interrupt Mask register setting  ( 0x3F)*/
        rba_I2c_Prv_Address_apcst[ idxHwUnit_qu8 ]->IMSC = ( RBA_I2C_IFX_IMSC_LSREQ_INT_ENABLE << RBA_REG_I2C_IMSC_LSREQ_INT_POS )
                                                         | ( RBA_I2C_IFX_IMSC_SREQ_INT_ENABLE << RBA_REG_I2C_IMSC_SREQ_INT_POS )
                                                         | ( RBA_I2C_IFX_IMSC_LBREQ_INT_ENABLE << RBA_REG_I2C_IMSC_LBREQ_INT_POS )
                                                         | ( RBA_I2C_IFX_IMSC_BREQ_INT_ENABLE << RBA_REG_I2C_IMSC_BREQ_INT_POS )
                                                         | ( RBA_I2C_IFX_IMSC_I2C_ERR_INT_ENABLE << RBA_REG_I2C_IMSC_I2C_ERR_INT_POS )
                                                         | ( RBA_I2C_IFX_IMSC_I2C_P_INT_ENABLE << RBA_REG_I2C_IMSC_I2C_P_INT_POS );

        /* Clear the All Protocol Interrupt*/
        rba_I2c_ClearInterrupt(idxHwUnit_qu8);
    }

}

///////////////////////////////////////////////////////////////////////////////
//! \brief Runtime handler.
//! IFX runtime handler read the slave info from the slave configuration, and check the
//! interrrut status. if enable the interrupt then process the ISR handler.
//! otherwise call the Po
//!
//!
//! <b>Sync/Async:</b> Synchronous
//!
//! <b>Reentrancy:</b> Non-reentrant
//!
//! \param rba_I2c_SeqData_st: Pointer to I2c sequence definition.
//!
//! \return none.
//!
//! \pre none
//!
//! \post I2cIfx will activate the transmission of sequence.
//!
///////////////////////////////////////////////////////////////////////////////
void rba_I2c_Prv_Hw_Handler(rba_I2c_SeqDef_tst *rba_I2c_SeqData_st)
{

    /* Local variables */
    uint32                    IfxTimeout_Max_u32;                        /* counter variable */
    uint32                    dataSequence_prv_au32[4] = {0,0,0,0};      /* data read array */
    uint32                    cntrTime_Prv_u32 = 0uL;                    /* counter variable */
    uint32                    cntrTxDataLengthCount_Prv_u32;             /* Sequence Transmit Datalength */
    uint8                     cntrDataCount_Prv_u8;                      /* counter variable */
    uint8                     xLogicId_u8;                               /* Sequence Logic ID */
    uint8                     xHwId_u8;                                  /* Sequence Logic ID */
    uint8                     xSequenceId_u8 = 0u;                       /* Sequence ID. */
    uint8                     xSlaveId_u8 = 0u;                          /* Slave ID */
    uint8                    *adrTxDataAdr_Prv_pu8;                      /* Sequence Transmit address */
    uint8                    *adrRxDataAdr_Prv_pu8;                      /* Sequence receive address */
    uint8                     xSlaveUniqueId_Prv_u8 = 0u;                /* Sequence configured Unique slave address */
    uint8                     stRet = E_NOT_OK;                          /* status variable */
    rba_I2c_xComType_ten      xComType_Prv_e;                            /* Sequence communication Type */
    rba_I2c_PriorityType_ten  xSeqPrio_en = RBA_I2C_SEQ_LOW_PRIORITY;    /* Sequence Priority status*/
    boolean                   xCommunication_status_u8 = TRUE;           /* Local communication status variable */
    uint32                    current_Ticks_u32 = 0uL;                   /* Current System ticks*/
    uint8                     xReceive_Length_u8 = 0u;                 /* Receive datalength */
    uint32                    initial_Ticks_u32 = 0uL;                   /* Initial System ticks*/
    boolean                   rx_status_b = FALSE;                         /* Receive status */



/*+++++++++++++++++++++++++++++++++++++++++++++Read the info from the sequence Start+++++++++++++++++++++++++++++++++++++++*/

    /* Read the configured Sequence ID from the requesting sequence */
    xSequenceId_u8 = rba_I2c_SeqData_st->rba_I2c_xSequenceCfgId_u8;

    /* Read the Slave ID from the requesting sequence */
    xSlaveId_u8 = rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[xSequenceId_u8]->SlaveId;

    /* Read the slave interface I2C Hardware channel number form the configuration */
    xHwId_u8 = rba_I2c_prv_cfgConfigType_pcst->slaveConfig_pcst[ xSlaveId_u8 ]->hwConnectID_u8;

    /* Read the logic ID from the configuration */
    xLogicId_u8 = rba_I2c_prv_cfgConfigType_pcst->hwUnitConfig_pcst[ xHwId_u8 ]->LogicHwId_u8;

    /* Read the sequence priority from the configuration */
    xSeqPrio_en = rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[xSequenceId_u8]->sequPrio_en;

    /* Read the configured 7bit salve unique address from the configuration */
    xSlaveUniqueId_Prv_u8 = rba_I2c_prv_cfgConfigType_pcst->slaveConfig_pcst[ xSlaveId_u8 ]->slaveAddress_u8;

    /* Read the communication Type from the sequence configuration*/
    xComType_Prv_e =  rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[xSequenceId_u8]->seqComType_en;

    /* Copy the sequence to current running sequence structure internal variable */
    rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].currentSeq          = rba_I2c_SeqData_st;
    rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runDataRx_pu8       = rba_I2c_SeqData_st->rba_I2c_dataRx_pu8;
    rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runDataTx_pu8       = rba_I2c_SeqData_st->rba_I2c_dataTx_pu8;
    rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runSequenceCfgId_u8 = rba_I2c_SeqData_st->rba_I2c_xSequenceCfgId_u8;
    rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runSeqTxlength_u8   = rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[xSequenceId_u8]->seqTxLen_u8;
    rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runSeqRxlength_u8   = rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[xSequenceId_u8]->seqRxLen_u8;
    rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runSeqComType_en    = rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[xSequenceId_u8]->seqComType_en;
    rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runSeqPriority_u8   = rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[xSequenceId_u8]->sequPrio_en;

/*---------------------------------------------Read the info from the sequence Stop----------------------------------------*/

/*++++++++++++++++++++++++++++++++++++++++++++Initial Register setting for the every sequence start++++++++++++++++++++++++++++++*/

    /* I2C module connect to the Port setting via the Mux Register selection*/
    rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->GPCTL = (uint32)rba_I2c_prv_cfgConfigType_pcst->hwUnitConfig_pcst[ xHwId_u8 ]->hwPort_u8;

    /*   write access enable to the I2C configuration registers and Disable the I2C bus */
    rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->RUNCTRL &= ~( RBA_I2C_IFX_RUNCTRL_RUN_ENABLE << RBA_REG_I2C_RUNCTRL_RUN_POS );

    /* Frequency setting */
    if(rba_I2c_prv_cfgConfigType_pcst->slaveConfig_pcst[ xSlaveId_u8 ]->baudRate_en == RBA_I2C_SLAVE_NORMAL_MODE )
    {
        /*  set the Fractional Divider for the clock - Default setting as 100KHz*/
        rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->FDIVCFG = RBA_I2C_IFX_NORMAL_MODE_FDIVCFG_VALUE;

        /* Timing configuration Register initial setting as 100KHz*/
        rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->TIMCFG = RBA_I2C_IFX_NORMAL_MODE_TIMCFG_VAL;

        /* Baud rate setting from the Fmax  100KHz */
        rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->CLC1 = RBA_I2C_IFX_NORMAL_MODE_CLC1_VALUE;

        /*MR12 RULE 14.3 VIOLATION: Warning is non-removable and uncritical since the conditional operator is used to make the return value is not overflow - Approved by MISRA2012_SUPPORT_TEAM*/
        rba_I2c_Clk = MCU_RB_US_TO_TICKS(RBA_I2C_CLK_100K); /* One clock cycle for 89KHz -> 1/89KHZ = 11.2359 microseconds */
    }
    else
    {
        /*  set the Fractional Divider for the clock - Default setting as 400KHz*/
        rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->FDIVCFG = RBA_I2C_IFX_FAST_MODE_FDIVCFG_VALUE;

        /* Timing configuration Register initial setting as 400KHz*/
        rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->TIMCFG = RBA_I2C_IFX_FAST_MODE_TIMCFG_VAL;

        /* Baud rate setting from the Fmax  (Init mode for the Default clock setting 400KHz) */
        rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->CLC1 = RBA_I2C_IFX_FAST_MODE_CLC1_VALUE;

        /*MR12 RULE 14.3 VIOLATION: Warning is non-removable and uncritical since the conditional operator is used to make the return value is not overflow - Approved by MISRA2012_SUPPORT_TEAM*/
        rba_I2c_Clk = MCU_RB_US_TO_TICKS(RBA_I2C_CLK_400K); /* One clock cycle for 384KHz -> 1/384KHZ = 2.6041 microseconds */
    }


    /*   write access Disable to the I2C configuration registers and Enable the I2C bus */
    rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->RUNCTRL |= ( RBA_I2C_IFX_RUNCTRL_RUN_ENABLE << RBA_REG_I2C_RUNCTRL_RUN_POS );

    /* Disable the interrupt for the polling mode */
    if ( rba_I2c_Prv_stInterruptMode_b == FALSE )
    {
        /* Disable the I2C internal Interrupt Mask register setting  ( 0x00)*/
        rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IMSC = ( RBA_I2C_IFX_IMSC_LSREQ_INT_DISABLE << RBA_REG_I2C_IMSC_LSREQ_INT_POS )
                                                        | ( RBA_I2C_IFX_IMSC_LSREQ_INT_DISABLE << RBA_REG_I2C_IMSC_SREQ_INT_POS )
                                                        | ( RBA_I2C_IFX_IMSC_LBREQ_INT_DISABLE << RBA_REG_I2C_IMSC_LBREQ_INT_POS )
                                                        | ( RBA_I2C_IFX_IMSC_BREQ_INT_DISABLE << RBA_REG_I2C_IMSC_BREQ_INT_POS )
                                                        | ( RBA_I2C_IFX_IMSC_I2C_ERR_INT_DISABLE << RBA_REG_I2C_IMSC_I2C_ERR_INT_POS )
                                                        | ( RBA_I2C_IFX_IMSC_I2C_P_INT_DISABLE << RBA_REG_I2C_IMSC_I2C_P_INT_POS );
    }
    else
    {

        /* Enable the all Interrupt Mask register setting  ( 0x3F)*/
        rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->IMSC = ( RBA_I2C_IFX_IMSC_LSREQ_INT_ENABLE << RBA_REG_I2C_IMSC_LSREQ_INT_POS )
                                                        | ( RBA_I2C_IFX_IMSC_SREQ_INT_ENABLE << RBA_REG_I2C_IMSC_SREQ_INT_POS )
                                                        | ( RBA_I2C_IFX_IMSC_LBREQ_INT_ENABLE << RBA_REG_I2C_IMSC_LBREQ_INT_POS )
                                                        | ( RBA_I2C_IFX_IMSC_BREQ_INT_ENABLE << RBA_REG_I2C_IMSC_BREQ_INT_POS )
                                                        | ( RBA_I2C_IFX_IMSC_I2C_ERR_INT_ENABLE << RBA_REG_I2C_IMSC_I2C_ERR_INT_POS )
                                                        | ( RBA_I2C_IFX_IMSC_I2C_P_INT_ENABLE << RBA_REG_I2C_IMSC_I2C_P_INT_POS );
    }

    /* Clear the All Protocol Interrupt*/
    rba_I2c_ClearInterrupt( xHwId_u8 );

/*---------------------------Initial Register setting for the every sequence start---------------------------------------------*/


/*++++++++++++++++++++++++++++++++++++++++++++ Sequence Process start +++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /* Check the bus is Busy or Free*/
    if ( ( ( rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->BUSSTAT ) & (3uL << RBA_REG_I2C_BUSSTAT_BS_POS ) ) != 0uL )
    {
        /* Sequence status set to failed */

        /* Sequence Handler status updates as Process Completed*/
        rba_I2c_Prv_stI2c_ChanRunStatus_en[ xLogicId_u8 ] = RBA_I2C_CHANNEL_NOT_RUNNING;

#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
        /* Call DET error for Bus is not IDLE  */
        (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_IFX_HW_PRV_HANDLER, RBA_I2C_DET_ERR_IFX_BUS_NOT_IDLE);
#endif

        /* call end notification if present */
        if (rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[ xSequenceId_u8 ]->xSeqEndNotif_pfct != NULL_PTR)
        {
            (*rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[ xSequenceId_u8 ]->xSeqEndNotif_pfct)();
        }

    }
    else
    {
        /* Check the communication Type transmit or Combined receive */
        if ( ( xComType_Prv_e == rba_I2c_Tx_e ) || ( xComType_Prv_e == rba_I2c_CombinedTxRx_e ) )
        {
            /* Read the Transmit Data length from the sequence */
            cntrTxDataLengthCount_Prv_u32 = rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runSeqTxlength_u8;

            /* Read the Transmit sequence Starting Address */
            adrTxDataAdr_Prv_pu8 = rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runDataTx_pu8;

            /* Set the Transmit data length Size to the register (Sequence length + SlaveID length)*/
            rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->TPSCTRL = cntrTxDataLengthCount_Prv_u32 + RBA_I2C_SLAVE_ID_LENGTGH;

            /* Read the Data from sequence */
            cntrDataCount_Prv_u8 = 0u;
            for(cntrTime_Prv_u32 = 0uL; cntrTime_Prv_u32 < cntrTxDataLengthCount_Prv_u32; cntrTime_Prv_u32++ )
            {
                if(cntrTime_Prv_u32 == 0uL)
                {
                    /*  First byte update the slave id */
                    dataSequence_prv_au32[ cntrDataCount_Prv_u8 ] = ( (uint32)(0x000000FF) &  (xSlaveUniqueId_Prv_u8) );

                    /* Increment the data counter */
                    cntrDataCount_Prv_u8++;

                    /* Read the data from sequence */
                    dataSequence_prv_au32[ cntrDataCount_Prv_u8 ] = ( (uint32)(0x000000FF) & (adrTxDataAdr_Prv_pu8[cntrTime_Prv_u32]) );

                    /* Increment the data counter */
                    cntrDataCount_Prv_u8++;
                }
                else
                {
                    /* Read the data from sequence and load the local variable until 4 byte*/
                    dataSequence_prv_au32[ cntrDataCount_Prv_u8 ] = ( (uint32)(0x000000FF) & (adrTxDataAdr_Prv_pu8[cntrTime_Prv_u32]) );

                    /* Increment the data counter */
                    cntrDataCount_Prv_u8++;
                }

                if(cntrDataCount_Prv_u8 == 4u)
                {
                    /* Push the every 32 bit data to FIFO */
                    rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->TXD = ( dataSequence_prv_au32[3u] << 24uL ) + ( dataSequence_prv_au32[2u] << 16uL ) + ( dataSequence_prv_au32[1u] << 8uL ) + ( dataSequence_prv_au32[0u] );

                    /* Reset the data counter */
                    cntrDataCount_Prv_u8 = 0u;
                }
                else
                {
                    if( cntrTime_Prv_u32 == ( cntrTxDataLengthCount_Prv_u32 - (1uL) ) )
                     {
                    /* Push the Data to FIFO */
                    rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->TXD = ( dataSequence_prv_au32[3u] << 24uL ) + ( dataSequence_prv_au32[2u] << 16uL ) + ( dataSequence_prv_au32[1u] << 8uL ) + ( dataSequence_prv_au32[0u] );
                     }

                }
        }
    }
        else
        {
            if( xComType_Prv_e == rba_I2c_Rx_e )
            {
            /* MISRA warring: Rule CCode_Control_002: Value Test Expression*/
            if( rba_I2c_Prv_stInterruptMode_b )
            {
                rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runSeqComType_en = rba_I2c_CombinedTxRx_e;
            }

            /*   Generate the Restart condition */
            rba_I2c_Prv_stI2c_ChanRunStatus_en[ xLogicId_u8 ] = RBA_I2C_CHANNEL_RUNNING; /*need to check this state is useless */

            /* Receive Data Length*/
            rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->MRPSCTRL = rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runSeqRxlength_u8;

            /* written the number of bytes which have to be transmitted -------->M1*/
            rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->TPSCTRL = 0x01u; // Slave ID size

            /* Push the every 32 bit data to FIFO */
            rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->TXD = ( 0uL << 24uL ) + ( 0uL << 16uL ) + ( 0uL << 8uL ) + ( xSlaveUniqueId_Prv_u8 + RBA_I2C_RECEIVE_MODE );
             }
        }

       /* After interrupt disabled Polling mode process will be enable */
        if(rba_I2c_Prv_stInterruptMode_b == FALSE)
        {
            /* Check the communication Type as Transmit */
            if ( rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runSeqComType_en == rba_I2c_Tx_e )
            {
                /* Timeout checking for Transmission completed*/
                stRet = rba_I2c_Prv_TxComplete_Chk(xHwId_u8, rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runSeqTxlength_u8);

                if(stRet == E_NOT_OK)
                {
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                        /* Call DET error for Timeout slave interface */
                        (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_IFX_HW_PRV_HANDLER, RBA_I2C_DET_ERR_IFX_TX_SEQ_TIMEOUT_ERROR);
#endif
                        /* Execute the Sequence End transmission process */
                        rba_I2c_Prv_IfxSeqEndTransmission(xSequenceId_u8, xLogicId_u8,
                                rba_I2c_SeqFailed_e, RBA_I2C_ERR_IFX_TX_SEQ_TIMEOUT_ERROR);

                        xCommunication_status_u8  = FALSE;


                }

                /*   Disable the I2C interface and write access enable to the I2C configuration registers -spec T1 process*/
                rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->RUNCTRL &= ~( RBA_I2C_IFX_RUNCTRL_RUN_ENABLE << RBA_REG_I2C_RUNCTRL_RUN_POS );

            }
            else
            {
                /* Read the Receive data address from the sequence */
                adrRxDataAdr_Prv_pu8 = rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runDataRx_pu8;

                /* Check the communication Type as Combined Receive */
                if( rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runSeqComType_en == rba_I2c_CombinedTxRx_e )
                {
                    /* Timeout checking for Transmission completed*/
                    stRet = rba_I2c_Prv_TxComplete_Chk(xHwId_u8, rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runSeqTxlength_u8);//Validation for TX data

                    if(stRet == E_NOT_OK)
                    {
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                            /* Call DET error for Timeout slave interface */
                            (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_IFX_HW_PRV_HANDLER, RBA_I2C_DET_ERR_IFX_COMTX_SEQ_TIMEOUT_ERROR);
#endif
                            /* Execute the Sequence End transmission process */
                            rba_I2c_Prv_IfxSeqEndTransmission(xSequenceId_u8, xLogicId_u8,
                                    rba_I2c_SeqFailed_e, RBA_I2C_ERR_IFX_COMTX_SEQ_TIMEOUT_ERROR);

                            xCommunication_status_u8  = FALSE;
                            //return;

                    }


                    //A master transmits data to a slave and then reads back data from the same slave.
                    //The transfer direction is changed after the restart condition.

                    /*   write access enable to the I2C configuration registers and Disable the I2C bus */
                    rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->RUNCTRL &= ( ~( 1uL << RBA_REG_I2C_RUNCTRL_RUN_POS ));

                    /*   write access Disable to the I2C configuration registers and Enable the I2C bus */
                    rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->RUNCTRL |= ( 1uL << RBA_REG_I2C_RUNCTRL_RUN_POS );

                    /* Load the receive data length to register */
                    rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->MRPSCTRL = rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runSeqRxlength_u8;

                    /* Transmit data length information load to the register */
                    rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->TPSCTRL = 1u;

                    /* Transmit data push to FIFO */
                    rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->TXD =  ( 0uL << 24uL ) + ( 0uL << 16uL ) + ( 0uL << 8uL ) + ( xSlaveUniqueId_Prv_u8 + 0x01uL );

                }

                /* Clear interrupt request source */
                rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->ICR = ( RBA_I2C_IFX_ICR_LSREQ_INT_CLEAR << RBA_REG_I2C_ICR_LSREQ_INT_POS )
                                                                  | ( RBA_I2C_IFX_ICR_SREQ_INT_CLEAR << RBA_REG_I2C_ICR_SREQ_INT_POS )
                                                                  | ( RBA_I2C_IFX_ICR_LBREQ_INT_CLEAR << RBA_REG_I2C_ICR_LBREQ_INT_POS )
                                                                  | ( RBA_I2C_IFX_ICR_BREQ_INT_CLEAR << RBA_REG_I2C_ICR_BREQ_INT_POS );





                xReceive_Length_u8 = rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runSeqRxlength_u8 + RBA_I2C_SLAVEADD_LEN;
                /* Timeout checking for Transmission Receive completed*/
                stRet = rba_I2c_Prv_TxComplete_Chk(xHwId_u8, xReceive_Length_u8);

                if(stRet == E_NOT_OK)
                {
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                        /* Call DET error for Timeout slave interface */
                        (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_IFX_HW_PRV_HANDLER, RBA_I2C_DET_ERR_IFX_SLAVE_ADDRESS_TX_SEQ_TIMEOUT_ERROR);
#endif

                        /* Execute the Sequence End transmission process */
                        rba_I2c_Prv_IfxSeqEndTransmission(xSequenceId_u8, xLogicId_u8,
                                rba_I2c_SeqFailed_e, RBA_I2C_ERR_IFX_SLAVE_ADDRESS_TX_SEQ_TIMEOUT_ERROR);

                        xCommunication_status_u8  = FALSE;
                        //return;
                }

                /* Since, some time duration is need to set the register bit TX_END of the register PIRQSS,
                 * a buffer time is needed. The time needed to update the register PIRQSS is not specified
                 * in any document. Hence, 2 times the transmission length is been provided as buffer time
                 */
                /*MR12 RULE 14.3 VIOLATION: Warning is non-removable and uncritical since the conditional operator is used to make the return value is not overflow - Approved by MISRA2012_SUPPORT_TEAM*/
                IfxTimeout_Max_u32 = 2uL * ((RBA_I2C_CLK_LENGTH * rba_I2c_Clk)+(MCU_RB_US_TO_TICKS(RBA_I2C_IFX_HARDWARE_DELAY)));
                stRet = E_NOT_OK;
                initial_Ticks_u32 = Mcu_Rb_GetSysTicks();
                current_Ticks_u32 = 0uL;
                while(current_Ticks_u32 < IfxTimeout_Max_u32)
                {

                    if( ( ( rba_I2c_Prv_Address_apcst[xHwId_u8]->PIRQSS ) & ( 1uL << RBA_REG_I2C_PIRQSS_RX_POS ) ) != 0uL )
                    {

                        current_Ticks_u32 = IfxTimeout_Max_u32;
                        stRet = E_OK;
                    }
                    else
                    {
                        /* Update the current system Tick value */
                        current_Ticks_u32 = Mcu_Rb_GetSysTicks() - initial_Ticks_u32;
                    }

               }

                if(stRet == E_NOT_OK)
                {
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                        /* Call DET error for Timeout slave interface */
                        (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_IFX_HW_PRV_HANDLER, RBA_I2C_DET_ERR_IFX_RX_FLAG_ERROR);
#endif

                        /* Execute the Sequence End transmission process */
                        rba_I2c_Prv_IfxSeqEndTransmission(xSequenceId_u8, xLogicId_u8,
                                rba_I2c_SeqFailed_e, RBA_I2C_ERR_IFX_RX_FLAG_ERROR);

                        xCommunication_status_u8  = FALSE;
                        //return;
                }


                /* I2C interdate data transmission maximum time wait time is caluated double the value as per therotical value */
                /*MR12 RULE 14.3 VIOLATION: Warning is non-removable and uncritical since the conditional operator is used to make the return value is not overflow - Approved by MISRA2012_SUPPORT_TEAM*/
                IfxTimeout_Max_u32 = (2uL * RBA_I2C_CLK_LENGTH*rba_I2c_Clk * (rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runSeqRxlength_u8) ) +(MCU_RB_US_TO_TICKS(RBA_I2C_IFX_HARDWARE_DELAY));
                initial_Ticks_u32 = Mcu_Rb_GetSysTicks();
                current_Ticks_u32 = 0uL;
                xReceive_Length_u8 = 0u;

                 while(current_Ticks_u32 < IfxTimeout_Max_u32)
                 {
                     if( rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->FFSSTAT == rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runSeqRxlength_u8 )
                     {
                         rx_status_b = TRUE;

                         while(xReceive_Length_u8 < rba_I2c_Prv_IfxSeq_Run_st[ xLogicId_u8 ].runSeqRxlength_u8 )
                         {
                             /* Read the Data from the Data Register (FIFO) */
                             adrRxDataAdr_Prv_pu8[ xReceive_Length_u8 ] = (uint8)( rba_I2c_Prv_Address_apcst[ xHwId_u8 ]->RXD );

                             /* Increment the data length receive counter */
                             xReceive_Length_u8++;
                         }

                         current_Ticks_u32 = IfxTimeout_Max_u32;
                     }
                     else
                     {
                         /* Update the current system Tick value */

                      current_Ticks_u32 = Mcu_Rb_GetSysTicks() - initial_Ticks_u32;
                     }

                }

                if(rx_status_b  != TRUE)
                {
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
                    /* Call DET error for Timeout slave interface */
                    (void)Det_ReportError(RBA_I2C_MODULE_ID, RBA_I2C_INSTANCE_ID, RBA_I2C_APIID_IFX_HW_PRV_HANDLER, RBA_I2C_DET_ERR_IFX_RX_SEQ_TIMEOUT_ERROR);
#endif

                    /* Execute the Sequence End transmission process */
                    rba_I2c_Prv_IfxSeqEndTransmission(xSequenceId_u8, xLogicId_u8,
                            rba_I2c_SeqFailed_e, RBA_I2C_ERR_IFX_RX_SEQ_TIMEOUT_ERROR);

                    xCommunication_status_u8  = FALSE;
                    //return;
                }

                /*   write access enable to the I2C configuration registers and Disable the I2C bus */
                rba_I2c_Prv_Address_apcst[xHwId_u8]->RUNCTRL &= ~( 1uL << RBA_REG_I2C_RUNCTRL_RUN_POS);
            }


            /* End transmission of I2C data*/
            if(xCommunication_status_u8 == TRUE)
            {
                /* Clear the All Protocol Interrupt*/
                rba_I2c_ClearInterrupt(xHwId_u8);

                /* after complete the sequence execution set the Sequence status to IDLE */
                rba_I2c_Prv_stSequenceStatus_au8[ xSequenceId_u8 ] = rba_I2c_QueueSequence_Idle_e;

                if(rba_I2c_Prv_IfxSeq_Run_st[xLogicId_u8].currentSeq->rba_I2c_xSeqStatus_en != rba_I2c_SeqFailed_e)
                {
                    /* Get the  data belonging to the index */
                    rba_I2c_Prv_IfxSeq_Run_st[xLogicId_u8].currentSeq->rba_I2c_xSeqStatus_en = rba_I2c_SeqCompleted_e;
                }

                if(rba_I2c_Prv_stQueueState_au8[ xLogicId_u8 ] == RBA_I2C_PRV_QUEUE_ACTIVE)
                {
                    /* Increment the Send Position in the queue */
                    rba_I2c_Prv_IncSendPosition_Queue( xLogicId_u8, xSeqPrio_en );
                }

                /* call end notification if present */
                if (rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[ xSequenceId_u8 ]->xSeqEndNotif_pfct != NULL_PTR)
                {
                    (*rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[ xSequenceId_u8 ]->xSeqEndNotif_pfct)();
                }

                /* Check the Entries in Queue */
                if ( ( rba_I2c_Prv_posnQueueDynamic_ast[ xLogicId_u8 ].numEntries_u8 > 0u )
                     ||  ( rba_I2c_Prv_posnQueueHiPriDynamic_ast[ xLogicId_u8 ].numEntries_u8 > 0u ) )
                {
                    /* If entries available call to the Sequence handler function continouly */
                    rba_I2c_Prv_Handler(xLogicId_u8);
                }
                else
                {
                    /* Sequence Handler status updates as Process Completed*/
                    rba_I2c_Prv_stI2c_ChanRunStatus_en[ xLogicId_u8 ] = RBA_I2C_CHANNEL_NOT_RUNNING;

                }

            }



        }

    }
}


#ifdef RBA_I2C_ENABLE_DEINIT
///////////////////////////////////////////////////////////////////////////////
//! \brief Function interface
//! This process Will help to execute De-init the I2C interface for Hardware register
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
void rba_I2c_Prv_Hw_DeInit(uint8 idxHwUnit_qu8)
{

        /* Clear the All Protocol Interrupt*/
        rba_I2c_ClearInterrupt(idxHwUnit_qu8);

        /*   write access to configuration registers enabled(Disable the communicatio mode) */
        rba_I2c_Prv_Address_apcst[ idxHwUnit_qu8 ]->RUNCTRL &= ~( RBA_I2C_IFX_RUNCTRL_RUN_ENABLE << RBA_REG_I2C_RUNCTRL_RUN_POS );

        /* Set to LISTENING state.*/
        rba_I2c_Prv_Address_apcst[ idxHwUnit_qu8 ]->ADDRCFG |= (0x01uL <<RBA_REG_I2C_ADDRCFG_SONA_POS);

        /*   write access Disable to the I2C configuration registers and Enable the I2C bus */
        rba_I2c_Prv_Address_apcst[ idxHwUnit_qu8 ]->RUNCTRL |= ( RBA_I2C_IFX_RUNCTRL_RUN_ENABLE << RBA_REG_I2C_RUNCTRL_RUN_POS );

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
//! \param runSeqTxlength_u8         I2C Sequence Transmit length
//!
//! \return \return E_OK is success, E_NOT_OK is failure execution
//!
//! \pre None
//!
///////////////////////////////////////////////////////////////////////////////
Std_ReturnType rba_I2c_Prv_TxComplete_Chk(uint8 xHwId_u8, uint8 runSeqTxlength_u8)
{
    /* Local variables */
    uint8 stRet = E_NOT_OK;
    uint32 current_Ticks_u32 = 0uL;
    uint32 initial_Ticks_u32;
    uint32 IfxTimeout_Max_u32;            /* counter variable */
    /* Since, some time duration is need to set the register bit TX_END of the register PIRQSS,
     * a buffer time is needed. The time needed to update the register PIRQSS is not specified
     * in any document. Hence, three times the transmission length is been provided as buffer time
     */

    /* IFX controller hardware delay occur for ever I2C sequece transfer (maximum hardware delay time 120 microsecond )*/
    /*MR12 RULE 14.3 VIOLATION: Warning is non-removable and uncritical since the conditional operator is used to make the return value is not overflow - Approved by MISRA2012_SUPPORT_TEAM*/
    IfxTimeout_Max_u32 = ((2*runSeqTxlength_u8*RBA_I2C_CLK_LENGTH*rba_I2c_Clk)+(MCU_RB_US_TO_TICKS(RBA_I2C_IFX_HARDWARE_DELAY)));
    initial_Ticks_u32 = Mcu_Rb_GetSysTicks();

    while(current_Ticks_u32 < IfxTimeout_Max_u32)
    {
        if( ( ( rba_I2c_Prv_Address_apcst[xHwId_u8]->PIRQSS ) & ( 1uL << RBA_REG_I2C_PIRQSS_TX_END_POS ) ) != 0uL )
        {
            current_Ticks_u32 = IfxTimeout_Max_u32;
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

///////////////////////////////////////////////////////////////////////////////
//! \brief Function interface
//! This process will help to cleared the I2C-ISR register status register
//! sequence
//! \param logicId: logical channel number
//! \return Void
//! \pre none
//! \post Enter a new I2C sequence into the queue.
//!
///////////////////////////////////////////////////////////////////////////////

void rba_I2c_ClearInterrupt(uint8 logicId_u8)
{

    if( logicId_u8 < RBA_I2C_CFG_AVAILABLE_CHANNELS )
    {
        /* Clear all Error interrupt source */
        rba_I2c_Prv_Address_apcst[ logicId_u8 ]->ERRIRQSC = ( RBA_I2C_IFX_ERRIRQSC_RXF_UFL_CLEAR << RBA_REG_I2C_ERRIRQSC_RXF_UFL_POS )
                                                       | ( RBA_I2C_IFX_ERRIRQSC_RXF_OFL_CLEAR << RBA_REG_I2C_ERRIRQSC_RXF_OFL_POS )
                                                       | ( RBA_I2C_IFX_ERRIRQSC_TXF_UFL_CLEAR << RBA_REG_I2C_ERRIRQSC_TXF_UFL_POS )
                                                       | ( RBA_I2C_IFX_ERRIRQSC_TXF_OFL_CLEAR << RBA_REG_I2C_ERRIRQSC_TXF_OFL_POS );

        /* Clear protocol interrupt request source */
        rba_I2c_Prv_Address_apcst[ logicId_u8 ]->PIRQSC = ( RBA_I2C_IFX_PIRQSC_NACK_CLEAR << RBA_REG_I2C_PIRQSC_NACK_POS )
                                                     | ( RBA_I2C_IFX_PIRQSC_TX_END_CLEAR << RBA_REG_I2C_PIRQSC_TX_END_POS )
                                                     | ( RBA_I2C_IFX_PIRQSC_RX_CLEAR << RBA_REG_I2C_PIRQSC_RX_POS );

        /* Clear interrupt request source */
        rba_I2c_Prv_Address_apcst[ logicId_u8 ]->ICR = ( RBA_I2C_IFX_ICR_LSREQ_INT_CLEAR << RBA_REG_I2C_ICR_LSREQ_INT_POS )
                                                  | ( RBA_I2C_IFX_ICR_SREQ_INT_CLEAR << RBA_REG_I2C_ICR_SREQ_INT_POS )
                                                  | ( RBA_I2C_IFX_ICR_LBREQ_INT_CLEAR << RBA_REG_I2C_ICR_LBREQ_INT_POS )
                                                  | ( RBA_I2C_IFX_ICR_BREQ_INT_CLEAR << RBA_REG_I2C_ICR_BREQ_INT_POS );
    }

}

//- Functions -----------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
//! \brief
//! This process will help to execute the I2C complte sequence process.
//! <b>Sync/Async:</b> Synchronous
//! <b>Reentrancy:</b> reentrant
//! \param xSequenceId_u8            I2C configured sequence ID
//! \param xLogicId_u8:              I2C Logical ID
//! \param xSeqStatus:               request sequence
//! \param xErrorID_u8:              Error ID
//! \return void
//! \pre None
///////////////////////////////////////////////////////////////////////////////
void rba_I2c_Prv_IfxSeqEndTransmission(uint8 xSequenceId_u8, uint8 xLogicId_u8,
        rba_I2c_xSeqStatus_ten xSeqStatus, uint8 xErrorID_u8 )
{

    /* after complete the sequence execution set the Sequence status to IDLE */
    rba_I2c_Prv_stSequenceStatus_au8[ xSequenceId_u8 ] = rba_I2c_QueueSequence_Idle_e;

    /* Sequence status set to failed */
    rba_I2c_Prv_IfxSeq_Run_st[xLogicId_u8].currentSeq->rba_I2c_xSeqStatus_en = xSeqStatus;

    /* Set the BUS is not free error  */
    rba_I2c_Prv_IfxSeq_Run_st[xLogicId_u8].currentSeq->rba_I2c_xComErr_u8 = xErrorID_u8;

    /* Sequence Handler status updates as Process Completed*/
    rba_I2c_Prv_stI2c_ChanRunStatus_en[xLogicId_u8] = RBA_I2C_CHANNEL_NOT_RUNNING;

    /* call end notification if present */
    if (rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[ xSequenceId_u8 ]->xSeqEndNotif_pfct != NULL_PTR)
    {
        (*rba_I2c_prv_cfgConfigType_pcst->sequenceConfig_pcst[ xSequenceId_u8 ]->xSeqEndNotif_pfct)();
    }

}

#define  RBA_I2C_STOP_SEC_CODE
#include RBA_I2C_CFG_MEMMAP_H


//#if(RBA_I2C_CFG_MODULE_ACTIVE == STD_ON)
#endif

//#if (MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1)
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

