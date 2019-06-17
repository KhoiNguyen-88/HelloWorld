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

#ifndef RBA_I2CIFX_H
#define RBA_I2CIFX_H

/*!
 * \rba_comp rba_I2C
 *  @{
 */
/*-------------------------------------------------------------------------------------------------------------------*
 * Includes
 *-------------------------------------------------------------------------------------------------------------------*/

///////////////////////////////////////////////////////////////////////////////
// Module checks to avoid integration of incompatible files
///////////////////////////////////////////////////////////////////////////////

#include "Mcu.h"
#if(MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1)

#include "rba_I2C.h"
#if(RBA_I2C_CFG_MODULE_ACTIVE == STD_ON)

#include "rba_I2C_Prv.h"

#include    RBA_REG_I2C_H

#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif

/*
 ***********************************************************************************************************************
 * Defines/Macros
 ***********************************************************************************************************************
*/
/* I2C slave ID length for the Register setting*/

#define RBA_I2C_SLAVE_ID_LENGTGH                                     0x01u
#define RBA_I2C_IFX_HARDWARE_DELAY                                   120u


/* Macro for the rba_I2CIfx.c file API */
#define RBA_I2C_APIID_IFX_HW_PRV_INIT                                0x11u
#define RBA_I2C_APIID_IFX_HW_PRV_HANDLER                             0X12u
#define RBA_I2C_APIID_IFX_HW_PRV_DE_INIT                             0x13u
#define RBA_I2C_APIID_IFX_HW_PRV_CLEAR_INTERRUTP                     0X14u

/* Macro for the rba_I2CIfx_Isr.c file API */
#define RBA_I2C_APIID_IFX_HW_PRV_ISR_CORE_0                          0X41u
#define RBA_I2C_APIID_IFX_HW_PRV_ERR_ISR_0                           0X42u

#if(  (MCU_RB_MACHINE_DEV == MCU_RB_IFX_UC1_DEV4_ALL) || (MCU_RB_MACHINE_DEV == MCU_RB_IFX_UC1_DEV5_40NM_ALL) || (MCU_RB_MACHINE_DEV == MCU_RB_IFX_UC1_DEV4_40NM_ALL) )
#define RBA_I2C_APIID_IFX_HW_PRV_ISR_CORE_1                          0X43u
#define RBA_I2C_APIID_IFX_HW_PRV_ERR_ISR_1                           0X44u
#endif

#define RBA_I2C_ERR_SEQ_IFX_TIMEOUT_ERROR                            0x31u
#define RBA_I2C_ERR_IFX_TX_SEQ_TIMEOUT_ERROR                         0x32u
#define RBA_I2C_ERR_IFX_COMTX_SEQ_TIMEOUT_ERROR                      0x33u
#define RBA_I2C_ERR_IFX_SLAVE_ADDRESS_TX_SEQ_TIMEOUT_ERROR           0x34u
#define RBA_I2C_ERR_IFX_RX_FLAG_ERROR                                0x35u
#define RBA_I2C_ERR_IFX_RX_SEQ_TIMEOUT_ERROR                         0x36u

#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
/* IFX- hardware Hw_Handler function */
#define RBA_I2C_DET_ERR_IFX_BUS_NOT_IDLE                             0X31u
#define RBA_I2C_DET_ERR_IFX_TX_SEQ_TIMEOUT_ERROR                     0X32u
#define RBA_I2C_DET_ERR_IFX_COMTX_SEQ_TIMEOUT_ERROR                  0X33u
#define RBA_I2C_DET_ERR_IFX_SLAVE_ADDRESS_TX_SEQ_TIMEOUT_ERROR       0X34u
#define RBA_I2C_DET_ERR_IFX_RX_FLAG_ERROR                            0X35u
#define RBA_I2C_DET_ERR_IFX_RX_SEQ_TIMEOUT_ERROR                     0X36u
#define RBA_I2C_ERR_SEQ_IFX_COMTYPE_ERROR                            0X37u

/* ISR function*/
#define RBA_I2C_DET_ERR_ISR_ACK_NOT_RECEIVED                         0X50u

/* Error-ISR function*/
#define RBA_I2C_DET_ERR_IFX_ISR_ERROR                                0X51u

#endif

#define RBA_I2C_IFX_MODE_CLC_MODUEL_ENABLE                           0x00uL
#define RBA_I2C_IFX_MODE_CLC_MODUEL_DISABLE                          0x01uL

#define RBA_I2C_IFX_NORMAL_MODE_FDIVCFG_VALUE                        0x20025uL
#define RBA_I2C_IFX_NORMAL_MODE_TIMCFG_VAL                           0x00uL
#define RBA_I2C_IFX_NORMAL_MODE_CLC1_VALUE                           0x500uL

#define RBA_I2C_IFX_FAST_MODE_FDIVCFG_VALUE                          0x1000BuL
#define RBA_I2C_IFX_FAST_MODE_TIMCFG_VAL                             0x200C000uL
#define RBA_I2C_IFX_FAST_MODE_CLC1_VALUE                             0x200uL

#define RBA_I2C_IFX_IMSC_LSREQ_INT_ENABLE                            1uL
#define RBA_I2C_IFX_IMSC_SREQ_INT_ENABLE                             1uL
#define RBA_I2C_IFX_IMSC_LBREQ_INT_ENABLE                            1uL
#define RBA_I2C_IFX_IMSC_BREQ_INT_ENABLE                             1uL
#define RBA_I2C_IFX_IMSC_I2C_ERR_INT_ENABLE                          1uL
#define RBA_I2C_IFX_IMSC_I2C_P_INT_ENABLE                            1uL

#define RBA_I2C_IFX_IMSC_LSREQ_INT_DISABLE                           0uL
#define RBA_I2C_IFX_IMSC_SREQ_INT_DISABLE                            0uL
#define RBA_I2C_IFX_IMSC_LBREQ_INT_DISABLE                           0uL
#define RBA_I2C_IFX_IMSC_BREQ_INT_DISABLE                            0uL
#define RBA_I2C_IFX_IMSC_I2C_ERR_INT_DISABLE                         0uL
#define RBA_I2C_IFX_IMSC_I2C_P_INT_DISABLE                           0uL

#define RBA_I2C_IFX_RUNCTRL_RUN_ENABLE                               0x01uL
#define RBA_I2C_IFX_FIFOCFG_RXBS_VAL                                 0x02uL
#define RBA_I2C_IFX_FIFOCFG_TXBS_VAL                                 0x00uL
#define RBA_I2C_IFX_FIFOCFG_RXFA_VAL                                 0x02uL
#define RBA_I2C_IFX_FIFOCFG_TXFA_VAL                                 0x00uL
#define RBA_I2C_IFX_FIFOCFG_TXFC_VAL                                 0x01uL
#define RBA_I2C_IFX_FIFOCFG_RXFC_VAL                                 0x01uL

#define RBA_I2C_IFX_ADDRCFG_ADR_VAL                                  0x44uL
#define RBA_I2C_IFX_ADDRCFG_TBAM_VAL                                 0x00uL
#define RBA_I2C_IFX_ADDRCFG_MNS_VAL                                  0x01uL
#define RBA_I2C_IFX_ADDRCFG_SONA_VAL                                 0x01uL
#define RBA_I2C_IFX_ADDRCFG_SOPE_VAL                                 0x01uL

#define RBA_I2C_IFX_PIRQSM_NACK_ENABLE                               0x01uL
#define RBA_I2C_IFX_PIRQSM_TX_END_ENABLE                             0x01uL
#define RBA_REG_I2C_PIRQSM_RX_ENABLE                                 0x01uL

#define RBA_I2C_IFX_PIRQSC_NACK_CLEAR                                0x01uL
#define RBA_I2C_IFX_PIRQSC_TX_END_CLEAR                              0x01uL
#define RBA_I2C_IFX_PIRQSC_RX_CLEAR                                  0x01uL

#define RBA_I2C_IFX_ICR_LSREQ_INT_CLEAR                              0x01uL
#define RBA_I2C_IFX_ICR_SREQ_INT_CLEAR                               0x01uL
#define RBA_I2C_IFX_ICR_LBREQ_INT_CLEAR                              0x01uL
#define RBA_I2C_IFX_ICR_BREQ_INT_CLEAR                               0x01uL

#define RBA_I2C_IFX_ERRIRQSC_RXF_UFL_CLEAR                           0x01uL
#define RBA_I2C_IFX_ERRIRQSC_RXF_OFL_CLEAR                           0x01uL
#define RBA_I2C_IFX_ERRIRQSC_TXF_UFL_CLEAR                           0x01uL
#define RBA_I2C_IFX_ERRIRQSC_TXF_OFL_CLEAR                           0x01uL

#define RBA_I2C_IFX_MAX_HW_CHANNEL_DEV5_OR_DEV4                      0x02uL
#define RBA_I2C_IFX_MAX_HW_CHANNEL_DEV3_OR_DEV2                      0x01uL

/*
 ***********************************************************************************************************************
 * Type definitions

 ***********************************************************************************************************************
*/

/*
 ***********************************************************************************************************************
 * Extern declarations
 ***********************************************************************************************************************
*/

#define  RBA_I2C_START_SEC_VAR_CLEARED_UNSPECIFIED
#include RBA_I2C_CFG_MEMMAP_H
    extern rba_I2c_Seq_Run_tst rba_I2c_Prv_IfxSeq_Run_st[ RBA_I2C_CFG_MAX_CONFIGURED_CHANNELS ];
#define  RBA_I2C_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include RBA_I2C_CFG_MEMMAP_H

#define  RBA_I2C_START_SEC_VAR_INIT_UNSPECIFIED
#include RBA_I2C_CFG_MEMMAP_H
    extern rba_Reg_I2C_tst*                   rba_I2c_Prv_Address_apcst[];
#define  RBA_I2C_STOP_SEC_VAR_INIT_UNSPECIFIED
#include RBA_I2C_CFG_MEMMAP_H

#define  RBA_I2C_START_SEC_CODE
#include RBA_I2C_CFG_MEMMAP_H

    extern void rba_I2c_IfxReStart(void);
    extern void rba_I2c_HwUnit0_ErrorStatus_ISR_Proc(void);
    extern void rba_I2c_HwUnit0_Protocol_ISR_Proc(void);
    extern void rba_I2c_ClearInterrupt(uint8 logicId_u8);
    extern void rba_I2c_Prv_IfxSeqEndTransmission(uint8 xSequenceId_u8, uint8 xLogicId_u8,
            rba_I2c_xSeqStatus_ten xSeqStatus, uint8 xErrorID_u8 );
    extern Std_ReturnType rba_I2c_Prv_TxComplete_Chk(uint8 xHwId_u8, uint8 runSeqTxlength_u8);

#if( (MCU_RB_MACHINE_DEV == MCU_RB_IFX_UC1_DEV4_ALL) || (MCU_RB_MACHINE_DEV == MCU_RB_IFX_UC1_DEV5_40NM_ALL) || (MCU_RB_MACHINE_DEV == MCU_RB_IFX_UC1_DEV4_40NM_ALL) )
    extern void rba_I2c_HwUnit1_Protocol_ISR_Proc(void);
    extern void rba_I2c_HwUnit1_ErrorStatus_ISR_Proc(void);
#endif

#define  RBA_I2C_STOP_SEC_CODE
#include RBA_I2C_CFG_MEMMAP_H

/* End of #if  (MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1) validation */
#endif

/* End of RBA_I2C_CFG_MODULE_ACTIVE macro validation */
#endif

/* End of #ifndef RBA_I2CIFX_H macro validation*/
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
