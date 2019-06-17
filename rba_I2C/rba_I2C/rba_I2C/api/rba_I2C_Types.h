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

#ifndef RBA_I2C_TYPES_H
#define RBA_I2C_TYPES_H

/*!
 * \rba_comp rba_I2C
 *  @{
 */
/*-------------------------------------------------------------------------------------------------------------------*
 * Includes
 *-------------------------------------------------------------------------------------------------------------------*/

#include "Mcu.h"

/*
 ***********************************************************************************************************************
 * Type definitions


 ***********************************************************************************************************************
*/
//- Macros ------------------------------------

/*-------------------------------------------------------------------------------------------------------------------*
 * Type definition for struct for I2c sequence
 *-------------------------------------------------------------------------------------------------------------------*/

/* Sequence communication Type */
typedef enum
{
    rba_I2c_Tx_e=0,                                      /* Sequence communication type Transmit */
    rba_I2c_Rx_e,                                        /* Sequence communication type Receive */
    rba_I2c_CombinedTxRx_e                               /* Sequence communication type Transmit and Receive combined communication */
} rba_I2c_xComType_ten;

/* IFX controller I2C Core to Port interconnect Multiplexer selection value */
#if ( MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1 )
typedef enum
{
    RBA_I2C_IFX_PORTSELECT_A = 0,                        /* IFX I2C core port interface Multiplexer selection A */
    RBA_I2C_IFX_PORTSELECT_B,                            /* IFX I2C core port interface Multiplexer selection B */
    RBA_I2C_IFX_PORTSELECT_C,                            /* IFX I2C core port interface Multiplexer selection C */
    RBA_I2C_IFX_PORTSELECT_D,                            /* IFX I2C core port interface Multiplexer selection D */
    RBA_I2C_IFX_PORTSELECT_E,                            /* IFX I2C core port interface Multiplexer selection E */
    RBA_I2C_IFX_PORTSELECT_F,                            /* IFX I2C core port interface Multiplexer selection F */
    RBA_I2C_IFX_PORTSELECT_G,                            /* IFX I2C core port interface Multiplexer selection G */
    RBA_I2C_IFX_PORTSELECT_H                             /* IFX I2C core port interface Multiplexer selection H */
} rba_I2c_Port_ten;
#endif

/* Type that describes the possible I2C sequence  Status */
typedef enum
{
    rba_I2c_SeqIdle_e=0,                                  /* Sequence newly created */
    rba_I2c_Start_e,                                      /* Sequence updated in to queue*/
    rba_I2c_SeqRunning_e,                                 /* Sequence in  transmission */
    rba_I2c_SeqCompleted_e,                               /* Sequence completed */
    rba_I2c_SeqFailed_e                                   /* Sequence Failed */
} rba_I2c_xSeqStatus_ten ;

/* Type that describes the possible I2C sequence  Status */
typedef enum
{
    rba_I2c_QueueSequence_Idle_e=0,                       /* Sequence newly created */
    rba_I2c_QueueSequence_Busy_e                          /* Sequence updated in to queue*/
} rba_I2c_xQueueSeqStatus_ten ;

//! \brief The baud rate parameters of slave devices that are connected to I2c.
typedef enum
{
    RBA_I2C_SLAVE_NORMAL_MODE=0,                          /* sequence communication Speed Normal Mode */
    RBA_I2C_SLAVE_FAST_MODE                               /* sequence Communication Speed Fast Mode */
}
rba_I2c_SlaveBaudType_ten;

//! \brief The priorty type.
typedef enum
{
    RBA_I2C_SEQ_LOW_PRIORITY=0,                           /* sequence Communication LOW priority Mode */
    RBA_I2C_SEQ_HIGH_PRIORITY                             /* sequence Communication HIGH priority Mode */
}rba_I2c_PriorityType_ten;

//! \brief I2C channel status.
typedef enum
{
    RBA_I2C_CHANNEL_NOT_RUNNING=0,                       /* sequence Current status Running */
    RBA_I2C_CHANNEL_RUNNING                              /* sequence Current status Not Running */
}rba_I2c_QueueStatusType_ten;

//! \brief The configuration parameters of the Slave devices that are connected to I2c.
typedef struct
{
   const  uint8                     hwConnectID_u8;       /* physical Hardware unit ID. */
   const  uint8                     slaveAddress_u8;      /* Slave address. */
   const  rba_I2c_SlaveBaudType_ten     baudRate_en;      /* I2C Baudrate. */
} rba_I2c_Cfg_Slave_tst;

typedef struct
{
   void   (* xSeqEndNotif_pfct)(void);                    //!< Pointer to sequence end notification function.
   const  rba_I2c_PriorityType_ten    sequPrio_en;        /* Sequence priority status. */
   const rba_I2c_xComType_ten         seqComType_en;      /* Sequence Communication Type */
   const  uint8                       seqTxLen_u8;        /* Sequence Transmit length */
   const  uint8                       seqRxLen_u8;        /* Sequence Receive length. */
   const  uint8                       SlaveId;            /* Slave ID. */
} rba_I2c_Cfg_Sequence_tst;

//! \brief The configuration parameters of the  I2C hardware devices.
typedef struct
{
    uint8            LogicHwId_u8;                        /* Logical HW unit ID. */
#if ( MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1 )
    rba_I2c_Port_ten hwPort_u8;                           /* Physical HW Port ID. */
#endif
} rba_I2c_Cfg_HwUnit_tst;

//! \brief The configuration parameters of the I2C.
typedef struct
{
    const uint8                              numberOfUnits_u8;        /* Number of units that are used.*/
    const rba_I2c_Cfg_Slave_tst* const*      slaveConfig_pcst;        /* Pointer to slave configurations.*/
    const rba_I2c_Cfg_HwUnit_tst* const*     hwUnitConfig_pcst;       /* Pointer to Hw unit configuraitons. */
    const rba_I2c_Cfg_Sequence_tst* const*   sequenceConfig_pcst;     /* Sequence configurations*/
} rba_I2c_ConfigType;

/* slave interface sequence */
typedef struct
{
    uint8                    rba_I2c_xSequenceCfgId_u8;   /* Sequence configured signal ID */
    uint8*                   rba_I2c_dataRx_pu8;          /* sequence Receive data pointer */
    uint8*                   rba_I2c_dataTx_pu8;          /* sequence Transmit data pointer */
    uint8                    rba_I2c_xComErr_u8;          /* Sequence process Error  Status */
    rba_I2c_xSeqStatus_ten   rba_I2c_xSeqStatus_en ;      /* sequence process status */
} rba_I2c_SeqDef_tst;

/* end of #ifndef RBA_I2C_TYPES_H macro*/
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
