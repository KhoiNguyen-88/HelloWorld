/*<BASDKey>
**********************************************************************************************************************
*
* COPYRIGHT RESERVED, 2010 Robert Bosch GmbH. All rights reserved.
* The reproduction, distribution and utilization of this document as well as the communication of its contents to
* others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
* All rights reserved in the event of the grant of a patent, utility model or design.
*
**********************************************************************************************************************
* Administrative Information (automatically filled in)
* $Domain____:$
* $Namespace_:$
* $Class_____:$
* $Name______:$
* $Variant___:$
* $Revision__:$
**********************************************************************************************************************
</BASDKey>*/

#ifndef RBA_DMA_H
#define RBA_DMA_H

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */
#include "Std_Types.h"
// this re-export is not needed for rba_Dma public interfaces, but DMA users will need it -> don't remove
#include "rba_Reg.h"
#include RBA_REG_DMA_H
#include "rba_Dma_PBcfg.h"
#include "Mcu.h"
/* Vendor and module identification */
#define RBA_DMA_VENDOR_ID   (6)
#define RBA_DMA_MODULE_ID   (254)
#define RBA_DMA_INSTANCE_ID (34)

/* Module version information */
#define RBA_DMA_SW_MAJOR_VERSION (4)
#define RBA_DMA_SW_MINOR_VERSION (36)
#define RBA_DMA_SW_PATCH_VERSION (0)

// Det macros
#define RBA_DMA_PRV_APIID_INIT        (0)
#define RBA_DMA_PRV_APIID_ERR_ISR     (1)
#define RBA_DMA_PRV_ERRID_PENDING_ERR (0)


// JDP DMA control and status register, initialise group priorities
#define RBA_DMA_PRV_JDP_CR_INIT  ( (3uL << RBA_REG_DMA_CR_GRP3PRI_POS)     \
                                 | (2uL << RBA_REG_DMA_CR_GRP2PRI_POS)     \
                                 | (1uL << RBA_REG_DMA_CR_GRP1PRI_POS)     \
                                 | (0uL << RBA_REG_DMA_CR_GRP0PRI_POS)     \
                                 )

#define RBA_DMA_GETCHSTS_ID_ERR_POS         0uL
#define RBA_DMA_GETCHSTS_RSLV_ERR_POS       1uL
#define RBA_DMA_GETCHSTS_CHACTV_ERR_POS     4uL
#define RBA_DMA_GETCHSTS_REQLOST_ERR_POS    8uL
#define RBA_DMA_GETCHSTS_SER_ERR_POS       16uL
#define RBA_DMA_GETCHSTS_DER_ERR_POS       17uL
#define RBA_DMA_GETCHSTS_FPIER_ERR_POS     20uL
#define RBA_DMA_GETCHSTS_SRIER_ERR_POS     21uL
#define RBA_DMA_GETCHSTS_PERER_ERR_POS     22uL

// error bts for function rba_Dma_SetUpCh()
#define RBA_DMA_SETUPCH_ID_ERR_POS           0uL
#define RBA_DMA_SETUPCH_RSLV_ERR_POS         1uL
#define RBA_DMA_SETUPCH_SWTSIZE_ERR_POS      2uL
#define RBA_DMA_SETUPCH_SWTCHTRIG_ERR_POS    3uL
#define RBA_DMA_SETUPCH_NUMMOVES_ERR_POS     4uL
#define RBA_DMA_SETUPCH_LINKCH_ERR_POS       5uL
#define RBA_DMA_SETUPCH_NUMTRIG_ERR_POS      6uL
#define RBA_DMA_SETUPCH_CHNOTREADY_ERR_POS  16uL
#define RBA_DMA_SETUPCH_SRCBUFSIZE_ERR_POS  17uL
#define RBA_DMA_SETUPCH_DESTBUFSIZE_ERR_POS 18uL

// bit field definitions for swtChTrig_u8 (function rba_Dma_SetUpCh())
#define RBA_DMA_SETUPCH_NO_TRG              0u
#define RBA_DMA_SETUPCH_ENBL_HW_TRG_POS     0u
#define RBA_DMA_SETUPCH_ENBL_HW_TRG_MSK     1u
#define RBA_DMA_SETUPCH_IMDT_TRG_POS        1u
#define RBA_DMA_SETUPCH_IMDT_TRG_MSK        2u
#define RBA_DMA_SETUPCH_TRG_LINK_INNER_POS  2u
#define RBA_DMA_SETUPCH_TRG_LINK_INNER_MSK  0x4u
#define RBA_DMA_SETUPCH_TRG_LINK_OUTER_POS  3u
#define RBA_DMA_SETUPCH_TRG_LINK_OUTER_MSK  0x8u
#define RBA_DMA_SETUPCH_LINK_TARGET_POS     4u
#define RBA_DMA_SETUPCH_LINK_TARGET_MSK     0x10u
#define RBA_DMA_SETUPCH_KEEP_HWTRIGGER_POS  5u
#define RBA_DMA_SETUPCH_KEEP_HWTRIGGER_MSK  (1u << RBA_DMA_SETUPCH_KEEP_HWTRIGGER_POS)
#define RBA_DMA_SETUPCH_ENA_RING_BUF_POS    6u
#define RBA_DMA_SETUPCH_ENA_RING_BUF_MSK    (1u << RBA_DMA_SETUPCH_ENA_RING_BUF_POS)
#define RBA_DMA_SETUPCH_ENA_OPTIONS2_POS    7u
#define RBA_DMA_SETUPCH_ENA_OPTIONS2_MSK    (1u << RBA_DMA_SETUPCH_ENA_OPTIONS2_POS)


#define RBA_DMA_SETUPCH_TRG_LINK_ANY_MSK   (RBA_DMA_SETUPCH_TRG_LINK_INNER_MSK | RBA_DMA_SETUPCH_TRG_LINK_OUTER_MSK)

// bit field definitions for swtOptions2_u32 (function rba_Dma_SetUpCh())
#define RBA_DMA_SETUPCH_ENA_RING_BUFF_SRC_POS   0u
#define RBA_DMA_SETUPCH_ENA_RING_BUFF_SRC_MSK   1u
#define RBA_DMA_SETUPCH_ENA_RING_BUFF_DST_POS   1u
#define RBA_DMA_SETUPCH_ENA_RING_BUFF_DST_MSK   2u
#define RBA_DMA_SETUPCH_DECREMENT_SRC_POS       2u
#define RBA_DMA_SETUPCH_DECREMENT_SRC_MSK       4u
#define RBA_DMA_SETUPCH_DECREMENT_DST_POS       3u
#define RBA_DMA_SETUPCH_DECREMENT_DST_MSK       8u
#define RBA_DMA_SETUPCH_SRC_OFFSET_POS          4u
#define RBA_DMA_SETUPCH_SRC_OFFSET_MSK          0x70u
#define RBA_DMA_SETUPCH_DST_OFFSET_POS          7u
#define RBA_DMA_SETUPCH_DST_OFFSET_MSK          0x380u

#if ((MCU_RB_MACHINE_DEV == MCU_RB_IFX_UC1_DEV3_40NM_ALL ) || (MCU_RB_MACHINE_DEV == MCU_RB_IFX_UC1_DEV4_40NM_ALL ) || (MCU_RB_MACHINE_TYPE == MCU_RB_IFX_UC1_DEV5_40NM_B ))
#define RBA_DMA_ERR_REQ                         4u         // Number of DMA Error Service Requests
#endif

// macros to read source and destination register of a DMA channel
#if ( MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1 )
#define RBA_DMA_GET_SRC_REG(idxUnit, idxCh) DMA.CH[idxCh].SADR
#endif
#if ( MCU_RB_MACHINE_FAM == MCU_RB_JDP_UC1 )
#define RBA_DMA_GET_SRC_REG(idxUnit, idxCh) eDMA_PTR(idxUnit)->TCD[idxCh].SADDR
#endif
#if ( MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1 )
#define RBA_DMA_GET_DST_REG(idxUnit, idxCh) DMA.CH[idxCh].DADR
#endif
#if ( MCU_RB_MACHINE_FAM == MCU_RB_JDP_UC1 )
#define RBA_DMA_GET_DST_REG(idxUnit, idxCh) eDMA_PTR(idxUnit)->TCD[idxCh].DADDR
#endif


/**
 ***************************************************************************************************
 * \brief Macro to initialise a SRC register for DMA triggering
 *
 * This macro is used to initialise a SRC control register for DMA triggering purposes. It must be used
 * on the SRC register of the trigger source and will connect the source to the DMA channel.
 * Additional actions:
 * - clear any pending request
 * - clear interrupt overflow bit
 * - clear SW sticky bit
 * - enable the service request node
 *
 * \param[in]   uint8 dmaUnit_u8     DMA unit/module number (currently unused, was required for compatibility to IFX Dev3A)
 * \param[in]   uint8 dmaChannel_u8  DMA channel number
 *
 * \return      void
 *
 * \rb_example [Optional remove tag if not needed]
 * \code
 * void AnyFunction(void)
 * {
 *      uint8 idxUnit_u8;
 *      uint8 idxCh_u8;
 *
 *      if ( E_OK = rba_Dma_GetIdx(<myChannelId>, &idxUnit_u8, &idxCh_u8) )
 *      {
 *          -- init trigger source SRN (TIM)
 * #if (MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1)
 *           SRC.GTM.TIM[numTimMod_u8][numTimChan_u8] = RBA_DMA_CREATE_DMA_SRC(idxUnit_u8, idxCh_u8);
 * #endif
 *      }
 * }
 * \endcode
 *
 * \note If the variable idxUnit_u8 is not used anywhere else in the code it will most likely lead to a compiler warning
 * of type <unused variable>. To avoid this it can be cast to void like <(void)idxUnit_u8;>
 ***************************************************************************************************
 */
#if (MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1)
#define RBA_DMA_CREATE_DMA_SRC(idxUnit_u8, idxChannel_u8)                                                                        \
                               ( ((uint32)(idxChannel_u8)         << RBA_REG_SRC_SRPN_POS)    /* service request priority nr. */ \
                               | ((uint32)RBA_REG_SRC_TOS_VAL_DMA << RBA_REG_SRC_TOS_POS)     /* type of service */              \
                               | (1uL                             << RBA_REG_SRC_CLRR_POS)    /* clear pending request */        \
                               | (1uL                             << RBA_REG_SRC_IOVCLR_POS)  /* clear interrupt overflow bit */ \
                               | (1uL                             << RBA_REG_SRC_SWSCLR_POS)  /* clear SW sticky bit */          \
                               | (1uL                             << RBA_REG_SRC_SRE_POS))    /* enable service */
#else
#define RBA_DMA_CREATE_DMA_SRC(dmaChannel_u8, dmaUnit_u8)
#endif


/**
 ***************************************************************************************************
 * \brief Function to trigger a DMA transfer
 *
 * This function triggers a DMA transaction at the selected DMA channel. The function parameters are not checked
 * for valid contents so the user has to take care that rba_Dma_TrgCh() is only called with properly initialised
 * module and channel indices.
 *
 * \param[in]   uint8 idxUnit_u8  DMA unit number
 * \param[in]   uint8 idxCh_u8    DMA channel number
 *
 * \return      void
 *
 * \pre [Optional remove tag if not needed] Here you can describe the preconditions for the function
 *
 * \rb_example [Optional remove tag if not needed]
 * \code
 * void AnyFunction(void)
 * {
 *      uint8 idxUnit_u8;
 *      uint8 idxCh_u8;
 *
 *      if ( E_OK = rba_Dma_GetIdx(<myChannelId>, &idxUnit_u8, &idxCh_u8) )
 *      {
 *          -- trigger DMA
 *          rba_Dma_TrgCh(idxUnit_u8, idxCh_u8);
 *      }
 * }
 * \endcode
 *
 * \attention [Optional remove tag if not needed] Will be marked with a red marker in the output
 *
 * \note [Optional remove tag if not needed] Will be marked with a yellow marker in the output
 ***************************************************************************************************
 */
LOCAL_INLINE void rba_Dma_TrgCh(uint8 idxUnit_u8, uint8 idxCh_u8)
{
#if ( MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1 )
    DMA.CH[idxCh_u8].CHCSR |= 1uL << RBA_REG_DMA_CHCSRx_SCH_POS;
    (void) idxUnit_u8;
#endif
#if ( MCU_RB_MACHINE_FAM == MCU_RB_JDP_UC1 )
    eDMA_PTR(idxUnit_u8)->SSRT = (uint8)idxCh_u8;
#endif
}

/**
 ***************************************************************************************************
 * \brief Function to set source and destination address of a DMA channel
 *
 * This function sets the source and destination address registers of a selected DMA channel to the values passed
 * via the two interface parameters adrSrc_pv and adrDest_pv. In case a NULL_PTR is given the address
 * register is not changed.
 *
 * \param[in]   uint8 idxUnit_u8  DMA unit number
 * \param[in]   uint8 idxCh_u8    DMA channel number
 * \param[in]   void* adrSrc_pv   source address
 * \param[in]   void* adrDest_pv  destination address
 *
 * \return      void
 *
 * \rb_example
 * \code
 * void AnyFunction(void)
 * {
 *      uint8 idxUnit_u8;
 *      uint8 idxCh_u8;
 *
 *      if ( E_OK = rba_Dma_GetIdx(<myChannelId>, &idxUnit_u8, &idxCh_u8) )
 *      {
 *          -- set DMA channel source and destination address
 *          rba_Dma_SetAdr(idxUnit_u8, idxCh_u8, mySrcAddress, myDestAddress);
 *      }
 * }
 * \endcode
 *
 * \note For pointers pointing to any volatile memory the corresponding macro RBA_DMA_SET_ADR() can be used
 *       because the usage of the function will lead to compiler warnings in this case.
 ***************************************************************************************************
 */
#if ( MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1 )
#define RBA_DMA_SET_ADR(idxUnit_u8, idxCh_u8, adrSrc_cpcv, adrDest_cpcv)                           \
    {                                                                                              \
        if ((adrSrc_cpcv) != NULL_PTR)                                                             \
        {                                                                                          \
            DMA.CH[idxCh_u8].SADR = (uint32)(adrSrc_cpcv);                                         \
        }                                                                                          \
        if ((adrDest_cpcv) != NULL_PTR)                                                            \
        {                                                                                          \
            DMA.CH[idxCh_u8].DADR = (uint32)(adrDest_cpcv);                                        \
        }                                                                                          \
        (void)(idxUnit_u8);                                                                        \
    }
#endif
#if ( MCU_RB_MACHINE_FAM == MCU_RB_JDP_UC1 )
#define RBA_DMA_SET_ADR(idxUnit_u8, idxCh_u8, adrSrc_cpcv, adrDest_cpcv)                           \
    {                                                                                              \
        if ((adrSrc_cpcv) != NULL_PTR)                                                             \
        {                                                                                          \
            eDMA_PTR(idxUnit_u8)->TCD[idxCh_u8].SADDR = (uint32)(adrSrc_cpcv);                     \
        }                                                                                          \
        if ((adrDest_cpcv) != NULL_PTR)                                                            \
        {                                                                                          \
            eDMA_PTR(idxUnit_u8)->TCD[idxCh_u8].DADDR = (uint32)(adrDest_cpcv);                    \
        }                                                                                          \
    }
#endif

LOCAL_INLINE void rba_Dma_SetAdr(uint8 idxUnit_u8, uint8 idxCh_u8, const void* const adrSrc_cpcv, const void* const adrDest_cpcv)
{
    /* MR12 RULE 11.4, 11.6 VIOLATION: The warning is uncritical and non-removable since cast is required to
     * initialize hardware registers with an address - Approved by MISRA2012_SUPPORT_TEAM. */
    RBA_DMA_SET_ADR(idxUnit_u8, idxCh_u8, adrSrc_cpcv, adrDest_cpcv);
    return;
}

/**
 ***************************************************************************************************
 * \brief Function to get the source address of a DMA channel based upon the unit and channel
 *
 * This function returns the source of a selected DMA channel
 *
 * \param[in]   uint8 idxUnit_u8  DMA unit number
 * \param[in]   uint8 idxCh_u8    DMA channel number
 * \param[out]  adrSrc_u32        source address of the DMA channel
 *
 *
 * \rb_example
 * \code
 * void AnyFunction(void)
 * {
 *      uint8 idxUnit_u8;
 *      uint8 idxCh_u8;
 *      uint32 adrDst_u32;
 *
 *      adrDst_u32 = rba_Dma_GetSrcAdrByIdx(idxUnit_u8, idxCh_u8);
 * }
 * \endcode
 ***************************************************************************************************
 */
LOCAL_INLINE uint32 rba_Dma_GetSrcAdrByIdx(uint8 idxUnit_u8, uint8 idxCh_u8)
{
    uint32 adrSrc_u32 = 0;

    // check if channel is configured in current Pb variant
    if ( (idxUnit_u8 < RBA_DMA_NUM_DMA_MODULES) && (idxCh_u8 < RBA_DMA_NUM_DMA_CHANS) )
    {
        // return contents of SADR register
        adrSrc_u32 = RBA_DMA_GET_SRC_REG(idxUnit_u8, idxCh_u8);
        // to avoid compiler warnings (IFX)
        (void)idxUnit_u8;
    }

    return adrSrc_u32;
}

/**
 ***************************************************************************************************
 * \brief Function to get the destination address of a DMA channel based upon the unit and channel
 *
 * This function returns the destination of a selected DMA channel
 *
 * \param[in]   uint8 idxUnit_u8  DMA unit number
 * \param[in]   uint8 idxCh_u8    DMA channel number
 * \param[out]  adrSrc_u32        destination address of the DMA channel
 *
 *
 * \rb_example
 * \code
 * void AnyFunction(void)
 * {
 *      uint8  idxUnit_u8;
 *      uint8  idxCh_u8;
 *      uint32 adrDst_u32;
 *
 *      adrDst_u32 = rba_Dma_GetDstAdrByIdx(idxUnit_u8, idxCh_u8);
 * }
 * \endcode
 ***************************************************************************************************
 */
LOCAL_INLINE uint32 rba_Dma_GetDstAdrByIdx(uint8 idxUnit_u8, uint8 idxCh_u8)
{
    uint32 adrDst_u32 = 0;

    // check if channel is configured in current Pb variant
    if ( (idxUnit_u8 < RBA_DMA_NUM_DMA_MODULES) && (idxCh_u8 < RBA_DMA_NUM_DMA_CHANS) )
    {
        // return contents of DADR register
        adrDst_u32 = RBA_DMA_GET_DST_REG(idxUnit_u8, idxCh_u8);
        // to avoid compiler warnings (IFX)
        (void)idxUnit_u8;
    }

    return adrDst_u32;
}

typedef struct
{
    void*   adrSrc_pv;                 // source address
    void*   adrDest_pv;                // destination address
    uint32  numTrig_u32;               // number of triggers
    uint32  numMoves_u32;              // number of data moves per trigger
    uint8   swtSize_u8;                // data transfer size?
    boolean swtIncSrc_b;               // increment source address?
    boolean swtIncDest_b;              // increment destination address?
    boolean swtEnaReq_b;               // enable service request trigger after last trigger
    uint8   swtChTrig_u8;              // transfer options
    uint16  numSrcBuf_u16;             // source buffer size for wrap-around buffer
    uint16  numDestBuf_u16;            // target buffer size for wrap-around buffer
    uint32  swtOptions2_u32;           // second set of transfer options
} rba_Dma_SetUpCh_tst;


#define RBA_DMA_START_SEC_CODE_NORMAL
#include "rba_Dma_MemMap.h"
extern Std_ReturnType rba_Dma_GetDstAdr  (rba_Dma_Id_ten const idChan_cen,
                                          uint32*        const adrDst_cpu32);
extern Std_ReturnType rba_Dma_GetIdx     (rba_Dma_Id_ten const idChan_cen,
                                          uint8*         const ptrUnitVal_cpu8,
                                          uint8*         const ptrChVal_cpu8);
extern Std_ReturnType rba_Dma_GetUnitNr  (rba_Dma_Id_ten const idChan_cen,
                                          uint8*         const ptrIdxVal_cpu8);
extern Std_ReturnType rba_Dma_GetChNr    (rba_Dma_Id_ten const idChan_cen,
                                          uint8*         const ptrIdxVal_cpu8);
extern Std_ReturnType rba_Dma_EnblCh     (rba_Dma_Id_ten const idChan_cen);
extern Std_ReturnType rba_Dma_DsblCh     (rba_Dma_Id_ten const idChan_cen);
extern Std_ReturnType rba_Dma_RstCh      (rba_Dma_Id_ten const idChan_cen);
extern uint8_least    rba_Dma_GetUnitNum (rba_Dma_Id_ten const idChan_cen);
extern uint8_least    rba_Dma_GetChanNum (rba_Dma_Id_ten const idChan_cen);
extern Std_ReturnType rba_Dma_GetChSts   (rba_Dma_Id_ten const idChan_cen,
                                          uint32 *       const stErr_pu32);
extern Std_ReturnType rba_Dma_SetUpCh    (rba_Dma_Id_ten              const idChan_cen,
                                          rba_Dma_SetUpCh_tst const * const adrConfig_cpcst,
                                          uint32                    * const stErr_cpu32);
extern Std_ReturnType rba_Dma_SwtMux     (rba_Dma_Id_ten const idChan_cen,
                                          boolean        const swtOnOff_cb);
extern Std_ReturnType rba_Dma_MoInitCheck(const rba_Dma_ConfigType* ConfigPtr_pc);
#if ( (defined RBA_DMA_SHUTDOWN_API) && (RBA_DMA_SHUTDOWN_API == STD_ON) )
extern void           rba_Dma_Shutdown   (void);
#endif
#define RBA_DMA_STOP_SEC_CODE_NORMAL
#include "rba_Dma_MemMap.h"

#define RBA_DMA_START_SEC_CODE_FAST
#include "rba_Dma_MemMap.h"
extern Std_ReturnType rba_Dma_AckIrq  (rba_Dma_Id_ten const idChan_cen);
extern void           rba_Dma_Error_Isr (void);
#define RBA_DMA_STOP_SEC_CODE_FAST
#include "rba_Dma_MemMap.h"

#define RBA_DMA_START_SEC_CODE_SLOW
#include "rba_Dma_MemMap.h"
extern void           rba_Dma_Init       (rba_Dma_ConfigType  const * const argPbVar_cpc);
#define RBA_DMA_STOP_SEC_CODE_SLOW
#include "rba_Dma_MemMap.h"

#define RBA_DMA_START_SEC_VAR_FAST_CLEARED_ENUM
#include "rba_Dma_MemMap.h"
extern rba_Dma_ConfigType rba_Dma_idxPbVar;
#define RBA_DMA_STOP_SEC_VAR_FAST_CLEARED_ENUM
#include "rba_Dma_MemMap.h"

/* RBA_DMA_H */
#endif

/* <BASDKey>
*********************************************************************************************************
* $History__:$
*********************************************************************************************************
</BASDKey>*/
