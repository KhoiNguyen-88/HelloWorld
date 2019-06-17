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

#ifndef RBA_I2CJDP_H
#define RBA_I2CJDP_H

/*!
 * \rba_comp rba_I2C
 *  @{
 */
/*-------------------------------------------------------------------------------------------------------------------*
 * Includes
 *-------------------------------------------------------------------------------------------------------------------*/

//- File checks ---------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
// Module checks to avoid integration of incompatible files
///////////////////////////////////////////////////////////////////////////////

#include "Mcu.h"

#if((MCU_RB_MACHINE_FAM == MCU_RB_JDP_UC1) && (MCU_RB_MACHINE_DEV != MCU_RB_JDP_UC1_DEV1_ALL))

#include "rba_I2C.h"
#if(RBA_I2C_CFG_MODULE_ACTIVE == STD_ON)

#include  "rba_I2C_Prv.h"

#include    RBA_REG_I2C_H

#define RBA_I2C_JDP_TX_COMPLETE_CHK_PASS                             0x00u
#define RBA_I2C_JDP_TX_COMPLETE_CHK_FAILED                           0x01u
#define RBA_I2C_JDP_TX_ACK_NOT_RECEIVED_FAILED                       0x02u

/* Macro for the rba_I2CJdp.c file API */
#define RBA_I2C_APIID_JDP_HW_PRV_INIT                                0x11u
#define RBA_I2C_APIID_JDP_HW_PRV_HANDLER                             0X12u
#define RBA_I2C_APIID_JDP_HW_PRV_DE_INIT                             0x13u
#define RBA_I2C_APIID_JDP_HW_PRV_TX_COMPLETE_CHECK                   0X14u
#define RBA_I2C_APIID_JDP_HW_PRV_RX_COMPLETE_CHECK                   0X15u
#define RBA_I2C_APIID_JDP_HW_PRV_ACK_RECEIVE_CHECK                   0X16u
#define RBA_I2C_APIID_JDP_HW_PRV_RETURN_ERROR_SEQ                    0X17u
#define RBA_I2C_APIID_JDP_HW_PRV_VALIDATE_TX                         0X18u
#define RBA_I2C_APIID_JDP_HW_PRV_WAIT_TILL_BUSY                      0X19u
#define RBA_I2C_APIID_JDP_HW_PRV_WAIT_TILL_IDLE                      0X20u

/* Macro for the rba_I2CJdp_Isr.c file API */
#define RBA_I2C_APIID_JDP_HW_PRV_ISR_CORE_0                          0X41u

#if(MCU_RB_MACHINE_DEV == MCU_RB_JDP_UC1_DEV4_ALL)
#define RBA_I2C_APIID_JDP_HW_PRV_ISR_CORE_1                          0X42u
#endif

#define RBA_I2C_ERR_SEQ_JDP_POL_TX_ID_NOT_COMPLETED                  0x31u
#define RBA_I2C_ERR_SEQ_JDP_POL_TX_ID_ACK_NOT_RECEIVED               0x32u
#define RBA_I2C_ERR_SEQ_JDP_POL_TX_NOT_COMPLETED                     0x33u
#define RBA_I2C_ERR_SEQ_JDP_POL_TX_ACK_NOT_RECEIVED                  0x34u
#define RBA_I2C_ERR_SEQ_JDP_POL_COMB_TX_RX_BUS_NOT_BUSY              0x35u
#define RBA_I2C_ERR_SEQ_JDP_POL_COMB_TX_LAST_BYTE_NOT_COMPLETED      0x36u
#define RBA_I2C_ERR_SEQ_JDP_POL_COMB_TX_LAST_BYTE_ACK_NOT_RECEIVED   0x37u
#define RBA_I2C_ERR_SEQ_JDP_POL_COMB_TX_NOT_COMPLETED                0x38u
#define RBA_I2C_ERR_SEQ_JDP_POL_COMB_TX_ACK_NOT_RECEIVED             0x39u
#define RBA_I2C_ERR_SEQ_JDP_POL_COMB_RX_DUMMY_NOT_COMPLETED          0x3Au
#define RBA_I2C_ERR_SEQ_JDP_POL_COMB_RX_LAST_BYTE_NOT_COMPLETED      0x3Bu
#define RBA_I2C_ERR_SEQ_JDP_POL_COMB_RX_NOT_COMPLETED                0x3Cu
#define RBA_I2C_ERR_SEQ_JDP_POL_RX_DUMMY_NOT_COMPLETED               0x3Du
#define RBA_I2C_ERR_SEQ_JDP_POL_RX_LAST_BYTE_NOT_COMPLETED           0x3Eu
#define RBA_I2C_ERR_SEQ_JDP_POL_RX_NOT_COMPLETED                     0x40u
#define RBA_I2C_ERR_SEQ_JDP_I2C_BUS_NOT_IDLE                         0x41u
#define RBA_I2C_ERR_SEQ_JDP_I2C_START_FAILURE                        0x42u

/* JDP-ISR */
#define RBA_I2C_ERR_SEQ_JDP_ISR_TX_ACK_NOT_RECEIVED                  0x60u
#define RBA_I2C_ERR_SEQ_JDP_ISR_COMB_RX_TX_BUS_NOT_BUSY              0x61u
#define RBA_I2C_ERR_SEQ_JDP_ISR_COMB_TX_RX_ACK_NOT_RECEIVED          0x62u
#define RBA_I2C_ERR_SEQ_JDP_ISR_COMTYPE_ERROR                        0x63u
#define RBA_I2C_ERR_SEQ_JDP_ISR_RX_BUS_BUSY                          0x64u
#define RBA_I2C_ERR_SEQ_JDP_ISR_RX_COMM_TYPE                         0x65u

#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
#define RBA_I2C_DET_ERR_JDP_POL_TX_ID_NOT_COMPLETED                  0X31u
#define RBA_I2C_DET_ERR_JDP_POL_TX_ID_ACK_NOT_RECEIVED               0x32u
#define RBA_I2C_DET_ERR_JDP_POL_TX_NOT_COMPLETED                     0X33u
#define RBA_I2C_DET_ERR_JDP_POL_TX_ACK_NOT_RECEIVED                  0x34u
#define RBA_I2C_DET_ERR_JDP_POL_COMB_TX_RX_BUS_NOT_BUSY              0X35u
#define RBA_I2C_DET_ERR_JDP_POL_COMB_TX_LAST_BYTE_NOT_COMPLETED      0X36u
#define RBA_I2C_DET_ERR_JDP_POL_COMB_TX_LAST_BYTE_ACK_NOT_RECEIVED   0x37u
#define RBA_I2C_DET_ERR_JDP_POL_COMB_TX_NOT_COMPLETED                0X38u
#define RBA_I2C_DET_ERR_JDP_POL_COMB_TX_ACK_NOT_RECEIVED             0x39u
#define RBA_I2C_DET_ERR_JDP_POL_COMB_RX_DUMMY_NOT_COMPLETED          0X3Au
#define RBA_I2C_DET_ERR_JDP_POL_COMB_RX_LAST_BYTE_NOT_COMPLETED      0X3Bu
#define RBA_I2C_DET_ERR_JDP_POL_COMB_RX_NOT_COMPLETED                0X3Cu
#define RBA_I2C_DET_ERR_JDP_POL_RX_DUMMY_NOT_COMPLETED               0X3Du
#define RBA_I2C_DET_ERR_JDP_POL_RX_LAST_BYTE_NOT_COMPLETED           0X3Eu
#define RBA_I2C_DET_ERR_JDP_POL_RX_NOT_COMPLETED                     0X40u
#define RBA_I2C_DET_ERR_JDP_I2C_BUS_NOT_IDLE                         0X41u
#define RBA_I2C_DET_ERR_JDP_I2C_START_FAILURE                        0X42u

/* JDP-ISR*/
#define RBA_I2C_DET_ERR_JDP_ISR_TX_ACK_NOT_RECEIVED                  0x60u
#define RBA_I2C_DET_ERR_JDP_ISR_COMB_RX_TX_BUS_NOT_BUSY              0x61u
#define RBA_I2C_DET_ERR_JDP_ISR_COMB_TX_RX_ACK_NOT_RECEIVED          0x62u
#define RBA_I2C_DET_ERR_JDP_ISR_COMM_TYPE                            0x63u
#define RBA_I2C_DET_ERR_JDP_ISR_RX_BUS_BUSY                          0x64u
#define RBA_I2C_DET_ERR_JDP_ISR_RX_COMM_TYPE                         0x65u
#endif

/* I2C bus interface Hardeare setting Macro */
#define RBA_I2C_JDP_SLAVEID                   (0xB0u) // I2C module working only the Master for master ID is not required Temp only.
#define RBA_I2C_JDP_CLOCK_SPEED_100K          (0x32u) // Register setting for the IBFD register (as per the reference from the HW team)
#define RBA_I2C_JDP_CLOCK_SPEED_400K          (0x56u) // Register setting for the IBFD register (as per the reference from the HW team)

extern rba_Reg_I2C_tst*    rba_I2c_Prv_Address_apcst[];

/* enumeration type containing JDP-I2C sequence data status */
typedef struct
{
   uint8                 dataTxPosition_u8;
   uint8                 dataRxPosition_u8;
   uint8                 slaveID_u8;
}rba_I2c_Prv_SeqCount_tst;

/*
 ***********************************************************************************************************************
 * Extern declarations
 ***********************************************************************************************************************
*/
/* Variable */

#define  RBA_I2C_START_SEC_VAR_CLEARED_UNSPECIFIED
#include RBA_I2C_CFG_MEMMAP_H
    extern rba_I2c_Seq_Run_tst         rba_I2c_Prv_JdpSeq_Run_st[RBA_I2C_CFG_MAX_CONFIGURED_CHANNELS];        /* I2C Sequence status st */
    extern rba_I2c_Prv_SeqCount_tst    rba_I2c_Prv_SeqCount_st[RBA_I2C_CFG_MAX_CONFIGURED_CHANNELS];   /* I2C Sequence status st */
#define  RBA_I2C_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include RBA_I2C_CFG_MEMMAP_H


/* Functions */

#define  RBA_I2C_START_SEC_CODE
#include RBA_I2C_CFG_MEMMAP_H
    extern Std_ReturnType rba_I2c_Prv_TxComplete_Chk(uint8 xHwId_u8, uint8 runSeqTxlength_u8);
    extern Std_ReturnType rba_I2c_Prv_RxComplete_Chk(uint8 xHwId_u8, uint8 runSeqRxlength_u8);
    extern Std_ReturnType rba_I2c_Prv_AckRecive_Chk(uint8 xHwId_u8);
    extern uint8 rba_I2c_Prv_ValidateTransmissionStatus(uint8 xHwId_u8, uint8 runSeqTxlength_u8);
    extern Std_ReturnType rba_I2c_Prv_waitTill_BusBusy(uint8 xHwId_u8);
    extern Std_ReturnType rba_I2c_Prv_waitTill_BusIdle(uint8 xHwId_u8);
    extern void rba_I2c_Prv_SeqEndTransmission(uint8 xSequenceId_u8, uint8 xHwId_u8, uint8 xLogicId_u8,
            rba_I2c_PriorityType_ten xSeqPrio_en, rba_I2c_xSeqStatus_ten xSeqStatus, uint8 xErrorID_u8 );
    extern void rba_I2c_HwUnit0_Isr_Proc(void);
#if(MCU_RB_MACHINE_DEV == MCU_RB_JDP_UC1_DEV4_ALL)
    extern void rba_I2c_HwUnit1_Isr_Proc(void);
#endif

    LOCAL_INLINE void rba_I2c_JdpStart(uint8 Module);
    LOCAL_INLINE void rba_I2c_JdpStop(uint8 Module);
    LOCAL_INLINE void rba_I2c_JdpReStart(uint8 Module);

    LOCAL_INLINE void rba_I2c_JdpStart(uint8 Module)
    {
        /* Set the Microcontroller as Master and Write or Read mode in to the I2C Register */
        rba_I2c_Prv_Address_apcst[ Module ]->IBCR &= ((uint8)~(1uL << RBA_REG_I2C_IBCR_MSSL_POS));
        rba_I2c_Prv_Address_apcst[ Module ]->IBCR |= ((uint8)( 1uL << RBA_REG_I2C_IBCR_MSSL_POS));
    }

    LOCAL_INLINE void rba_I2c_JdpStop(uint8 Module)
    {
        /* Send the Stop command the slave device*/
        rba_I2c_Prv_Address_apcst[ Module ]->IBCR &= ((uint8)~(1uL << RBA_REG_I2C_IBCR_MSSL_POS));
    }

    LOCAL_INLINE void rba_I2c_JdpReStart(uint8 Module)
    {
        /* Send the Stop command the slave device*/
        rba_I2c_Prv_Address_apcst[ Module ]->IBCR |= ( (uint8)(1uL << RBA_REG_I2C_IBCR_RSTA_POS) );
    }

#define  RBA_I2C_STOP_SEC_CODE
#include RBA_I2C_CFG_MEMMAP_H

/* End of #if(MCU_RB_MACHINE_FAM macro validation */
#endif

/* End of  #ifdef RBA_I2C_CFG_MODULE_ACTIVE macro validation */
#endif

/* End of #ifndef RBA_I2CJDP_H macro validation */
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
