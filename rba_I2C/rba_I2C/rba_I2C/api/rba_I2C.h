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


#ifndef RBA_I2C_H
#define RBA_I2C_H

/*!
 * \rba_comp rba_I2C
 *  @{
 */
/*-------------------------------------------------------------------------------------------------------------------*
 * Includes
 *-------------------------------------------------------------------------------------------------------------------*/

#include "Std_Types.h"
#include "rba_I2C_Cfg.h"


#define RBA_I2C_AR_RELEASE_MAJOR_VERSION      RBA_I2C_PRV_AR_VERSION_MAJOR           //!< Major version
#define RBA_I2C_AR_RELEASE_MINOR_VERSION      RBA_I2C_PRV_AR_VERSION_MINOR           //!< Minor version
#define RBA_I2C_AR_RELEASE_REVISION_VERSION   RBA_I2C_PRV_AR_VERSION_REVISION        //!< Revision version

//- File checks ---------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
// Module checks to avoid integration of incompatible files
///////////////////////////////////////////////////////////////////////////////
#if(RBA_I2C_CFG_MODULE_ACTIVE == STD_ON)

/*
 ***********************************************************************************************************************
 * Defines/Macros
 ***********************************************************************************************************************
*/
//! \brief Vendor ID
#define RBA_I2C_VENDOR_ID                     (6L)

//! \brief Module ID (use complex driver id)
#define RBA_I2C_MODULE_ID                     (254u)

#define RBA_I2C_INSTANCE_ID                   (37u)


//! \name Software version
//! Version of the vendor specific implementation of the module.
//@{
#define RBA_I2C_SW_MAJOR_VERSION              4u           //!< Software major version
#define RBA_I2C_SW_MINOR_VERSION              37u          //!< Software minor version
#define RBA_I2C_SW_PATCH_VERSION              0u           //!< Software patch version

#if (RBA_I2C_CFG_CHECK_AUTOSAR_VERSION == STD_ON)
    #if (!defined(STD_TYPES_AR_RELEASE_MAJOR_VERSION) || !defined(STD_TYPES_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR version numbers of Std_Types.h undefined."
    #endif
    #if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != 4) \
        || ((STD_TYPES_AR_RELEASE_MINOR_VERSION != 0) && (STD_TYPES_AR_RELEASE_MINOR_VERSION != 2)))
        #error "AUTOSAR version numbers of this header and Std_Types.h are different."
    #endif
#endif

//! @}

//! \name rba_I2C module API service ID's for DET
//! @{
#define RBA_I2C_APIID_INIT                                    0x00u    //!< Id - rba_I2c_Init
#define RBA_I2C_APIID_END_INIT                                0x01u    //!< Id - rba_I2c_GetVersionInfo
#define RBA_I2C_APIID_TXRX_SEQUENCE                           0x02u    //!< Id - rba_I2c_GetVersionInfo
#define RBA_I2C_APIID_PRV_HANDLER                             0x03u
#define RBA_I2C_APIID_DEINIT                                  0x04u    //!< Id - rba_I2c_DeInit
//! @}

/* I2C sequence states */
#define I2C_SEQ_IDLE                                          0x00u  /* state after startup, nothing to do             */
#define I2C_SEQ_START                                         0x01u  /* sequence placed in queue, send start condition */
#define I2C_SEQ_FINISHED                                      0x02u  /* sequence completely sent, nothing to do        */
#define I2C_SEQ_FAILED                                        0x04u   /* sequence Transmission Failed                   */
/*--------------------------------------------------------------------------------------------------------------------*/
/*---------------------*/
/* Sequence Error Info */
/*---------------------*/

/*rba_I2c_TxRx_Sequence*/
#define RBA_I2C_ERR_SEQ_NOT_INITIALIZED_STATUS                0x05u
#define RBA_I2C_ERR_SEQ_SIGNAL_NOT_CONFIGURED                 0x06u
#define RBA_I2C_ERR_SEQ_COMM_ERROR                            0x07u
#define RBA_I2C_ERR_SEQ_NOT_IDLE                              0x08u
#define RBA_I2C_ERR_SEQ_TX_POINTER_ERROR                      0x09u
#define RBA_I2C_ERR_SEQ_RX_POINTER_ERROR                      0x0Au
#define RBA_I2C_ERR_SEQ_COMTYPE_ERROR                         0x0Bu
#define RBA_I2C_ERR_SEQ_BUSY                                  0x0Cu
#define RBA_I2C_ERR_SEQ_QUEUE_FULL                            0x0Du
#define RBA_I2C_ERR_SEQ_QUEUE_INACTIVE                        0x0Eu

/*rba_I2c_Prv_Handler*/
#define RBA_I2C_ERR_SEQ_REPEAT_SEQ                            0x20u

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------*/
/* DET Error ID */
/*--------------*/
#if (RBA_I2C_CFG_DEV_ERROR_DETECT == STD_ON)
/*init*/
#define RBA_I2C_DET_ERR_ALREADY_INITIALIZED_STATUS            0x00u
#define RBA_I2C_DET_ERR_INVALID_CONF_POINTER                  0x01u
#define RBA_I2C_DET_ERR_DEINITIALIZED                         0x02u

/* rba_I2c_TxRx_Sequence */
#define RBA_I2C_DET_ERR_NOT_INITIALIZED_STATUS                0x05u
#define RBA_I2C_DET_ERR_INVALID_SEQ_POINTER                   0x06u
#define RBA_I2C_DET_ERR_SIGNAL_NOT_CONFIGURED                 0x07u
#define RBA_I2C_DET_ERR_COMM_ERROR                            0x08u
#define RBA_I2C_DET_ERR_SEQ_NOT_IDLE                          0x09u
#define RBA_I2C_DET_ERR_TX_DATA_POINTER                       0x0Au
#define RBA_I2C_DET_ERR_RX_DATA_POINTER                       0x0Bu
#define RBA_I2C_DET_ERR_COMM_TYPE                             0x0Cu
#define RBA_I2C_DET_ERR_QUEUE_SEQ_NOT_IDLE                    0x0Du
#define RBA_I2C_DET_ERR_QUEUE_FULL_HIGH_PRIO                  0x0Eu
#define RBA_I2C_DET_ERR_QUEUE_FULL_LOW_PRIO                   0x10u

/*rba_I2c_Prv_Handler*/
#define RBA_I2C_DET_ERR_REPEAT_SEQ_HIGH_PRIO                  0x20u
#define RBA_I2C_DET_ERR_REPEAT_SEQ_LOW_PRIO                   0x21u
#endif

/*
 ***********************************************************************************************************************
 * Extern declarations

 ***********************************************************************************************************************
*/

//- Interface Functions -------------------------------------------------------

#define  RBA_I2C_START_SEC_CODE
#include RBA_I2C_CFG_MEMMAP_H

extern void rba_I2c_Init(const rba_I2c_ConfigType *configuration_pcst);
extern void rba_I2c_End_Init(void);
extern Std_ReturnType rba_I2C_MoInitCheck(const rba_I2c_ConfigType *configuration_pcst);
#ifdef RBA_I2C_ENABLE_DEINIT
extern void  rba_I2c_DeInit(void);
#endif
extern Std_ReturnType  rba_I2c_TxRx_Sequence( rba_I2c_SeqDef_tst * rba_I2c_SeqDef_st );

#define  RBA_I2C_STOP_SEC_CODE
#include RBA_I2C_CFG_MEMMAP_H

/* End of #ifdef RBA_I2C_CFG_MODULE_ACTIVE macro validation */
#endif

/* End of #ifndef RBA_I2C_H macro validation */
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

