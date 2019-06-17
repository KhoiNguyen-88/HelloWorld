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

/**
 ***************************************************************************************************
 * \file
 ***************************************************************************************************
 */

/**
 * \addtogroup rba_Dma
 *  @{
 */


/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */
#include "rba_Dma.h"
#if ( MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1 )
// the #include of  "rba_Reg.h" is done in rba_Dma.h
#include RBA_REG_SRC_H
#endif
#if ( MCU_RB_MACHINE_FAM == MCU_RB_JDP_UC1 )
// the #include of  "rba_Reg.h" is done in rba_Dma.h
#include RBA_REG_DMAMUX_H
#endif
#if ( RBA_DMA_DEV_ERROR_DETECT == STD_ON )
#include "Det.h"
#endif

#define RBA_DMA_START_SEC_VAR_INIT_32
#include "rba_Dma_MemMap.h"
static const rba_Dma_ConfigType* rba_Dma_UsedConfig_pcst;
#define RBA_DMA_STOP_SEC_VAR_INIT_32
#include "rba_Dma_MemMap.h"

/* global DMA Pb variant indicator variable */
#define RBA_DMA_START_SEC_VAR_FAST_CLEARED_ENUM
#include "rba_Dma_MemMap.h"
rba_Dma_ConfigType rba_Dma_idxPbVar;
#define RBA_DMA_STOP_SEC_VAR_FAST_CLEARED_ENUM
#include "rba_Dma_MemMap.h"

#define RBA_DMA_START_SEC_CODE_NORMAL
#include "rba_Dma_MemMap.h"

/**
 ***************************************************************************************************
 * \brief Function to read the current DMA channel destination address
 *
 * This function returns the current destination address register contents of the given channel.
 *
 * \param[in]       idChan_cen   DMA channel short name
 * \param[out]      adrDst_cpu32 pointer to storage of the returned destination address, must be a pointer to uint32
 * \return          Std_ReturnType
 ***************************************************************************************************
 */
Std_ReturnType rba_Dma_GetDstAdr (rba_Dma_Id_ten const idChan_cen,
                                  uint32*        const adrDst_cpu32)
{
    Std_ReturnType xRet = E_NOT_OK;
    uint8_least    idxUnit_qu8;
    uint8_least    idxCh_qu8;
    if ( idChan_cen < RBA_DMA_NUM_IDS_E )
    {
        idxUnit_qu8 = rba_Dma_Cfg_Rslvr_acst[idChan_cen][rba_Dma_idxPbVar].idxUnit_u8;
        idxCh_qu8   = rba_Dma_Cfg_Rslvr_acst[idChan_cen][rba_Dma_idxPbVar].idxChan_u8;
        // check if channel is configured in current Pb variant
        if ( (idxUnit_qu8 < RBA_DMA_NUM_DMA_MODULES) && (idxCh_qu8 < RBA_DMA_NUM_DMA_CHANS) )
        {
            // return contents of DADR register
            *adrDst_cpu32 = RBA_DMA_GET_DST_REG(idxUnit_qu8, idxCh_qu8);
            // to avoid compiler warnings (IFX)
            (void)idxUnit_qu8;
            // set status
            xRet = E_OK;
        }
    }
    return xRet;
}

/**
 ***************************************************************************************************
 * \brief Function to resolve a DMA shaort name to the appropriate unit and channel numbers.
 *
 * Based on the given rba_Dma short name this function returns the appropriate DMA unit and channel index. If
 * idChan_cen is invalid or not configured in the currently active post build varaint the functions will return
 * E_NOT_OK, if resolving is successful the functions returns E_OK.
 *
 * \param[in]       idChan_cen       DMA channel short name
 * \param[out]      ptrUnitVal_cpu8  pointer to the location where the DMA unit number shall be stored
 * \param[out]      ptrChVal_cpu8    pointer to the location where the DMA channel number shall be stored
 *
 * \return Std_ReturnType
 *
 ***************************************************************************************************
 */
Std_ReturnType rba_Dma_GetIdx (rba_Dma_Id_ten const idChan_cen,
                               uint8*         const ptrUnitVal_cpu8,
                               uint8*         const ptrChVal_cpu8)
{
    Std_ReturnType xRet = E_NOT_OK;
    uint8_least    idxUnit_qu8;
    uint8_least    idxCh_qu8;
    if ( idChan_cen < RBA_DMA_NUM_IDS_E )
    {
        idxUnit_qu8 = rba_Dma_Cfg_Rslvr_acst[idChan_cen][rba_Dma_idxPbVar].idxUnit_u8;
        idxCh_qu8   = rba_Dma_Cfg_Rslvr_acst[idChan_cen][rba_Dma_idxPbVar].idxChan_u8;
        // check if channel is configured in current Pb variant
        if ( (idxUnit_qu8 < RBA_DMA_NUM_DMA_MODULES) && (idxCh_qu8 < RBA_DMA_NUM_DMA_CHANS) )
        {
            *ptrUnitVal_cpu8 = (uint8)idxUnit_qu8;      // write unit value
            *ptrChVal_cpu8   = (uint8)idxCh_qu8;        // write channel value
            xRet             = E_OK;                    // set status
        }
    }
    return xRet;
}



Std_ReturnType rba_Dma_GetUnitNr (rba_Dma_Id_ten const idChan_cen,
                                  uint8*         const ptrIdxVal_cpu8)
{
    Std_ReturnType xRet = E_NOT_OK;
    uint8_least    idxUnit_qu8;
    if ( idChan_cen < RBA_DMA_NUM_IDS_E )
    {
        idxUnit_qu8 = rba_Dma_Cfg_Rslvr_acst[idChan_cen][rba_Dma_idxPbVar].idxUnit_u8;
        // check if channel is configured in current Pb variant
        if ( idxUnit_qu8 < RBA_DMA_NUM_DMA_MODULES )
        {
            *ptrIdxVal_cpu8 = (uint8)idxUnit_qu8;      // write value
            xRet            = E_OK;                    // set status
        }
    }
    return xRet;
}


Std_ReturnType rba_Dma_GetChNr (rba_Dma_Id_ten const idChan_cen,
                                uint8*         const ptrIdxVal_cpu8)
{
    Std_ReturnType xRet = E_NOT_OK;
    uint8_least    idxVal_qu8;
    if ( idChan_cen < RBA_DMA_NUM_IDS_E )
    {
        idxVal_qu8 = rba_Dma_Cfg_Rslvr_acst[idChan_cen][rba_Dma_idxPbVar].idxChan_u8;
        // check if channel is configured in current Pb variant
        if ( idxVal_qu8 < RBA_DMA_NUM_DMA_CHANS )
        {
            *ptrIdxVal_cpu8 = (uint8)idxVal_qu8;       // write value
            xRet            = E_OK;                    // set status
        }
    }
    return xRet;
}

Std_ReturnType rba_Dma_EnblCh (rba_Dma_Id_ten const idChan_cen)
{
    Std_ReturnType xRet = E_NOT_OK;
    uint8_least    idxUnit_qu8;
    uint8          idxCh_u8;
    if ( idChan_cen < RBA_DMA_NUM_IDS_E )
    {
        idxUnit_qu8 = rba_Dma_Cfg_Rslvr_acst[idChan_cen][rba_Dma_idxPbVar].idxUnit_u8;
        idxCh_u8    = rba_Dma_Cfg_Rslvr_acst[idChan_cen][rba_Dma_idxPbVar].idxChan_u8;
        // check if channel is configured in current Pb variant
        if ( idxCh_u8 < RBA_DMA_NUM_DMA_CHANS )
        {
#if ( MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1 )
            // set ECH bit in TSR register
            DMA.TSR[idxCh_u8] |= 1uL << RBA_REG_DMA_TSRx_ECH_POS;
            (void)idxUnit_qu8;                       // to avoid unused variable warning
#endif
#if ( MCU_RB_MACHINE_FAM == MCU_RB_JDP_UC1 )
            eDMA_PTR(idxUnit_qu8)->SERQ = idxCh_u8;
#endif
            xRet = E_OK;                    // set status
        }
    }
    return xRet;
}

Std_ReturnType rba_Dma_DsblCh (rba_Dma_Id_ten const idChan_cen)
{
    Std_ReturnType xRet = E_NOT_OK;
    uint8_least    idxUnit_qu8;
    uint8          idxCh_u8;
    if ( idChan_cen < RBA_DMA_NUM_IDS_E )
    {
        idxUnit_qu8 = rba_Dma_Cfg_Rslvr_acst[idChan_cen][rba_Dma_idxPbVar].idxUnit_u8;
        idxCh_u8    = rba_Dma_Cfg_Rslvr_acst[idChan_cen][rba_Dma_idxPbVar].idxChan_u8;
        // check if channel is configured in current Pb variant
        if ( idxCh_u8 < RBA_DMA_NUM_DMA_CHANS )
        {
#if ( MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1 )
            // set DCH bit in TSR register
            DMA.TSR[idxCh_u8] |= 1uL << RBA_REG_DMA_TSRx_DCH_POS;
            (void)idxUnit_qu8;                       // to avoid unused variable warning
#endif
#if ( MCU_RB_MACHINE_FAM == MCU_RB_JDP_UC1 )
            eDMA_PTR(idxUnit_qu8)->CERQ = idxCh_u8;
#endif
            xRet = E_OK;                    // set status
        }
    }
    return xRet;
}

Std_ReturnType rba_Dma_RstCh (rba_Dma_Id_ten const idChan_cen)
{
    Std_ReturnType xRet = E_NOT_OK;
    uint8_least    idxUnit_qu8;
    uint8          idxCh_u8;
    if ( idChan_cen < RBA_DMA_NUM_IDS_E )
    {
        idxUnit_qu8 = rba_Dma_Cfg_Rslvr_acst[idChan_cen][rba_Dma_idxPbVar].idxUnit_u8;
        idxCh_u8    = rba_Dma_Cfg_Rslvr_acst[idChan_cen][rba_Dma_idxPbVar].idxChan_u8;
        // check if channel is configured in current Pb variant
        if ( idxCh_u8 < RBA_DMA_NUM_DMA_CHANS )
        {
#if ( MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1 )
            // set reset bit and wait until bit is actually reset to zero again
            DMA.TSR[idxCh_u8] |= 1uL << 0u;
            while ( (DMA.TSR[idxCh_u8] & (1uL << RBA_REG_DMA_TSRx_RST_POS)) != 0u )
            {
            }
            (void)idxUnit_qu8;                       // to avoid set but not used warning
#endif
#if ( MCU_RB_MACHINE_FAM == MCU_RB_JDP_UC1 )
            // disable HW trigger input
            eDMA_PTR(idxUnit_qu8)->CERQ = idxCh_u8;
            // clear channel errors
            eDMA_PTR(idxUnit_qu8)->CERR = idxCh_u8;
#endif
            xRet = E_OK;                    // set status
        }
    }
    return xRet;
}


uint8_least rba_Dma_GetUnitNum (rba_Dma_Id_ten const idChan_cen)
{
    uint8_least idxUnit_qu8 = 0xFF;
    if ( idChan_cen < RBA_DMA_NUM_IDS_E )
    {
        idxUnit_qu8 = rba_Dma_Cfg_Rslvr_acst[idChan_cen][rba_Dma_idxPbVar].idxUnit_u8;
    }
    return idxUnit_qu8;
}

uint8_least rba_Dma_GetChanNum (rba_Dma_Id_ten const idChan_cen)
{
    uint8_least idxChan_qu8 = 0xFF;
    if ( idChan_cen < RBA_DMA_NUM_IDS_E )
    {
        idxChan_qu8 = rba_Dma_Cfg_Rslvr_acst[idChan_cen][rba_Dma_idxPbVar].idxChan_u8;
    }
    return idxChan_qu8;
}

// ---------------------------------------
//! \brief rba_Dma Driver init Check Function
//!
//! This function checks the pointer given to the initialization of the module against
//! the pointer given as parameter to this function.
//!
//! Sync/Async: Synchronous
//!
//! Reentrancy: Reentrant
//!
//! \param ConfigPtr_pcst [in]: Pointer to selected rba_Dma driver configuration structure
//!
//! \return E_OK if pointer matches used configuration, otherwise E_NOT_OK
//!
//! \pre  None
//!
//! \post None
//!
// ---------------------------------------

Std_ReturnType rba_Dma_MoInitCheck(const rba_Dma_ConfigType* ConfigPtr_pc)
{
    // Note that rba_Dma implementation uses rba_Dma_UsedConfig_pcst, too.
    return (Std_ReturnType)((ConfigPtr_pc == rba_Dma_UsedConfig_pcst) ? E_OK : E_NOT_OK);
}

#define RBA_DMA_STOP_SEC_CODE_NORMAL
#include "rba_Dma_MemMap.h"


/**
 ***************************************************************************************************
 * \brief Dma initialisation function
 *
 * From here on you can describe the details of the function. You can use more than one line
 *
 * \param[in,out]   xConfigPtr_ps   [Mandatory] Describe the parameters of the function this one is a input/output.
 *                                  Can be multi line leading spaces will be removed \n \\n will force a line break
 * \param[in]       ChannelId   standard configuration switch
 * \param[out]      ChannelId   [Mandatory] Describe the parameters of the function this one is a output only
 *
 * \return void
 *
 * Long function description can be added here. So the parameter description above will stay close
 * to the function header
 *
 * \pre [Optional remove tag if not needed] Here you can describe the preconditions for the function
 *
 * \rb_example [Optional remove tag if not needed]
 * \code
 * void main(void)
 * {
 *      DoxyGenExample_Bosch_internal1( Dio_ChannelType ChannelId )
 * }
 * \endcode
 *
 * \attention [Optional remove tag if not needed] Will be marked with a red marker in the output
 *
 * \note [Optional remove tag if not needed] Will be marked with a yellow marker in the output
 ***************************************************************************************************
 */
#define RBA_DMA_START_SEC_CODE_SLOW
#include "rba_Dma_MemMap.h"
void rba_Dma_Init (rba_Dma_ConfigType const * const argPbVar_cpc)
{
    // initialise global Pb variant variable
    rba_Dma_idxPbVar = *argPbVar_cpc;

    // A valid configuration pointer marks the driver as initialized, too.
    rba_Dma_UsedConfig_pcst = argPbVar_cpc;


#if ( MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1 )
    {
        // reset all probably enabled DMA channels
        uint8 i;

#if (((MCU_RB_MACHINE_DEV == MCU_RB_IFX_UC1_DEV3_40NM_ALL )|| (MCU_RB_MACHINE_DEV == MCU_RB_IFX_UC1_DEV4_40NM_ALL ) || (MCU_RB_MACHINE_TYPE == MCU_RB_IFX_UC1_DEV5_40NM_B )) \
                                                            && (RBA_DMA_ERROR_ACTION != RBA_DMA_ERROR_ACTION_NONE))
        uint8 j;
#endif
        for (i = 0; i < RBA_DMA_NUM_DMA_CHANS; i++)
        {
            DMA.TSR[i] = (1u << RBA_REG_DMA_TSRx_RST_POS);
        }
        // clear all errors that might had happened
        DMA.CLRE0 = 0x07330000;
        DMA.CLRE1 = 0x07330000;
        // enable error interrupt if configured
#if (RBA_DMA_ERROR_ACTION != RBA_DMA_ERROR_ACTION_NONE)
#if ((MCU_RB_MACHINE_DEV != MCU_RB_IFX_UC1_DEV3_40NM_ALL )&&(MCU_RB_MACHINE_DEV != MCU_RB_IFX_UC1_DEV4_40NM_ALL ) && (MCU_RB_MACHINE_TYPE != MCU_RB_IFX_UC1_DEV5_40NM_B ))

        // check if an error IRQ is pending
        if ( ((SRC.DMAERR & (1uL << RBA_REG_SRC_SRR_POS)) != 0u) )
        {
            // clear pending interrupt
            SRC.DMAERR |= 1uL << RBA_REG_SRC_CLRR_POS;
#if (RBA_DMA_DEV_ERROR_DETECT == STD_ON)
            // dispatch a Det entry
            (void)Det_ReportError (RBA_DMA_MODULE_ID,
                                   RBA_DMA_INSTANCE_ID,
                                   RBA_DMA_PRV_APIID_INIT,
                                   RBA_DMA_PRV_ERRID_PENDING_ERR);
#endif
        }

#else
        // check if an error IRQ is pending
        for (j = 0; j < RBA_DMA_ERR_REQ; j++)
        {
        if ( ((SRC.DMAERR[j] & (1uL << RBA_REG_SRC_SRR_POS)) != 0u) )
        {
            // clear pending interrupt
            SRC.DMAERR[j] |= 1uL << RBA_REG_SRC_CLRR_POS;
#if (RBA_DMA_DEV_ERROR_DETECT == STD_ON)
            // dispatch a Det entry
            (void)Det_ReportError (RBA_DMA_MODULE_ID,
                                   RBA_DMA_INSTANCE_ID,
                                   RBA_DMA_PRV_APIID_INIT,
                                   RBA_DMA_PRV_ERRID_PENDING_ERR);
#endif
        }
        }
#endif
#endif
    }
#endif

#if ( MCU_RB_MACHINE_FAM == MCU_RB_JDP_UC1 )
    // initialise DMAMUX registers
    {
        uint8_least i;
        uint8_least j;
#if (MCU_RB_MACHINE_DEV == MCU_RB_ST_UC1_DEV3_40NM_ALL)
        rba_Reg_DMAMUX_tst* rba_Dma_Prv_adrDmaMux_apst[RBA_DMA_NUM_MUX] = {&DMAMUX_0, &DMAMUX_1, &DMAMUX_2, &DMAMUX_3, &DMAMUX_4, &DMAMUX_5};
        // loop through all DMAMUX instances
        for (i = 0; i < RBA_DMA_NUM_MUX; i++)
        {
            // now loop through all DMAMUX CHCONFIG registers within one instance
            for (j = 0; j < rba_Dma_MuxSize_acu8[i]; j++)
            {
                rba_Dma_Prv_adrDmaMux_apst[i]->CHCONFIG[j] = rba_Dma_Mux_acu8[rba_Dma_idxPbVar][i][j];
            }
        }
#else
        // loop through all DMAMUX instances
        for (i = 0; i < RBA_DMA_NUM_MUX; i++)
        {
            // now loop through all DMAMUX CHCONFIG registers within one instance
            for (j = 0; j < rba_Dma_MuxSize_acu8[i]; j++)
            {
                DMAMUX[i].CHCONFIG[j] = rba_Dma_Mux_acu8[rba_Dma_idxPbVar][i][j];
            }
        }
#endif
    }
    // set up global registers
    {
        uint8_least idxMe_qu8;
        // loop through all move engines
        for (idxMe_qu8 = 0; idxMe_qu8 < RBA_DMA_NUM_DMA_MODULES; idxMe_qu8++)
        {
            // disable all channels
            eDMA_PTR(idxMe_qu8)->CERQ = 1u << RBA_REG_DMA_CERQ_CAER_POS;
            // disable all error interrupts
            eDMA_PTR(idxMe_qu8)->CEEI = 1u << RBA_REG_DMA_CEEI_CAEE_POS;
#if ( (RBA_DMA_ERROR_ACTION != RBA_DMA_ERROR_ACTION_NONE) && (RBA_DMA_DEV_ERROR_DETECT == STD_ON) )
            // check if an error is pending before clearing all errors
#  if (MCU_RB_MACHINE_DEV == MCU_RB_ST_UC1_DEV3_40NM_ALL)
            // check register ERRH only in move engine 1 becaue it does not exist in move engine 0 (ST Dev3+)
            //                                    the order of evaluation is crucial here! (left to right)
            if ( (eDMA_PTR(idxMe_qu8)->ERRL != 0u) || ( (idxMe_qu8 == 1u) && (eDMA_PTR(idxMe_qu8)->ERRH != 0u) ) )
#  else
            if (   (eDMA_PTR(idxMe_qu8)->ERRL != 0u)
#    if (RBA_DMA_NUM_DMA_CHANS > 32)
                || (eDMA_PTR(idxMe_qu8)->ERRH != 0u)
#    endif
               )
#  endif
            {
                // dispatch a Det entry
                (void)Det_ReportError(RBA_DMA_MODULE_ID,
                                      RBA_DMA_INSTANCE_ID,
                                      RBA_DMA_PRV_APIID_INIT,
                                      RBA_DMA_PRV_ERRID_PENDING_ERR);
            }
#endif
            // clear all errors
            eDMA_PTR(idxMe_qu8)->CERR = 1u << RBA_REG_DMA_CERR_CAEI_POS;
            // clear DONE bits in all TCDs
            eDMA_PTR(idxMe_qu8)->CDNE = 1u << RBA_REG_DMA_CDNE_CADN_POS;
            // init control registers to 0x0000E400 (is reset value of controller)
            eDMA_PTR(idxMe_qu8)->CR   = RBA_DMA_PRV_JDP_CR_INIT;
        }
    }
    // enable error interrupts for different rba_Dma_ErrReaction configurations:
    // - 'trigger a Reset', reset is triggered in the ISR
    // - 'Count/Det', the Det entry is also done in the ISR
#if (RBA_DMA_ERROR_ACTION != RBA_DMA_ERROR_ACTION_NONE)
    // enable error interrupts of all configured channels
    {
        uint8_least idxCh_qu8;
        for (idxCh_qu8 = 0; idxCh_qu8 < (uint8_least)RBA_DMA_NUM_IDS_E; idxCh_qu8++)
        {
            uint8 idxMe_u8 = rba_Dma_Cfg_Rslvr_acst[idxCh_qu8][rba_Dma_idxPbVar].idxUnit_u8;
            uint8 idxCh_u8 = rba_Dma_Cfg_Rslvr_acst[idxCh_qu8][rba_Dma_idxPbVar].idxChan_u8;
            // enable error interrupt for this channel if channel is active
            if (idxMe_u8 < RBA_DMA_NUM_DMA_MODULES)
            {
                 eDMA_PTR(idxMe_u8)->SEEI = idxCh_u8;
            }
        }
    }
#endif
#endif

}
#define RBA_DMA_STOP_SEC_CODE_SLOW
#include "rba_Dma_MemMap.h"

/**
 ***************************************************************************************************
 * Dma_Shutdown
 *
 * stop all DMA activities
 *
 * \param       void
 * \return      void
 * \seealso
 ***************************************************************************************************
 */
#if ( (defined RBA_DMA_SHUTDOWN_API) && (RBA_DMA_SHUTDOWN_API == STD_ON) )
#define RBA_DMA_START_SEC_CODE_NORMAL
#include "rba_Dma_MemMap.h"
void rba_Dma_Shutdown(void)
{
#if ( MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1 )
    uint8_least i;
    // disable move engine error interrupts
    DMA.EER0 = 0;
    DMA.EER1 = 0;
    // disable HW request of all configured DMA channels
    for (i = 0u; i < (uint8_least)RBA_DMA_NUM_IDS_E; i++)
    {
        uint8 idxCh_u8 = rba_Dma_Cfg_Rslvr_acst[i][rba_Dma_idxPbVar].idxChan_u8;
        // check if channel is active in current Pb variant
        if (idxCh_u8 < RBA_DMA_NUM_DMA_CHANS)
        {
            // disable HW request for this channel
            DMA.TSR[idxCh_u8] = 1u << RBA_REG_DMA_TSRx_DCH_POS;
            // now wait until a possible transaction is finished, check TSR.CH bit
            while ( (DMA.TSR[idxCh_u8] | (uint32)(1uL << RBA_REG_DMA_TSRx_CH_POS)) == 0x0u )
            {
                // empty brackets just for Misra
            }
        }
    }
    // reset access protection registers
    Mcu_Rb_ResetEndInitSfty();
    DMA.ACCEN00 = 0uL;
    DMA.ACCEN10 = 0uL;
    DMA.ACCEN20 = 0uL;
    DMA.ACCEN30 = 0uL;
    Mcu_Rb_SetEndInitSfty();
#endif

#if ( MCU_RB_MACHINE_FAM == MCU_RB_JDP_UC1 )
    uint8_least idxMe_qu8;
    uint8_least idxDmaId_qu8;

    // loop through all move engines
    for (idxMe_qu8 = 0; idxMe_qu8 < RBA_DMA_NUM_DMA_MODULES; idxMe_qu8++)
    {
        // activate HALT
        // -> reject incoming new trigger requests, currently active channel completes as normal
        eDMA_PTR(idxMe_qu8)->CR |= 0x20uL;
        // additionally disable all hardware triggers
        eDMA_PTR(idxMe_qu8)->ERQL = 0x0;
        // clear all interrupt requests
        eDMA_PTR(idxMe_qu8)->INTL = 0xFFFFFFFFuL;
        // disable error interrupts
        eDMA_PTR(idxMe_qu8)->EEIL = 0x0;
#if (RBA_DMA_NUM_DMA_CHANS > 32)
        // in case of ST Dev3+ move engine 0 only has 32 channels
        // do not use the 'H' registers in this case!
#if (MCU_RB_MACHINE_DEV == MCU_RB_ST_UC1_DEV3_40NM_ALL)
        if (idxMe_qu8 != 0u)
#endif
        {
            // disable hardware triggers, upper 32 channels
            eDMA_PTR(idxMe_qu8)->ERQH = 0x0;
            // clear all interrupt requests, upper 32 channels
            eDMA_PTR(idxMe_qu8)->INTH = 0xFFFFFFFFuL;
            // disable error interrupts, upper 32 channels
            eDMA_PTR(idxMe_qu8)->EEIH = 0x0;
        }
#endif
    }

    // loop through all configured eDMA channels and check if anyone is still active
    // rba_Dma_Cfg_Rslvr_acst holds all configured channels
    for (idxDmaId_qu8 = 0; idxDmaId_qu8 < (uint8_least)RBA_DMA_NUM_IDS_E; idxDmaId_qu8++)
    {
        uint8 idxMe_u8;
        uint8 idxChl_u8;
        // get move engine and channel number from resolver table
        idxMe_u8  = rba_Dma_Cfg_Rslvr_acst[idxDmaId_qu8][rba_Dma_idxPbVar].idxUnit_u8;
        idxChl_u8 = rba_Dma_Cfg_Rslvr_acst[idxDmaId_qu8][rba_Dma_idxPbVar].idxChan_u8;
        // do nothing for unused array entries
        if (idxMe_u8 < 0xFF)
        {
            // now wait for the ACTIVE bit to be reset (if it was set)
            while ( (eDMA_PTR(idxMe_u8)->TCD[idxChl_u8].CSR | 0x0040u) == 0x0u )
            {
                // using blank brackets for Misra
            }
        }
    }
#endif
}
#define RBA_DMA_STOP_SEC_CODE_NORMAL
#include "rba_Dma_MemMap.h"

#endif

/** @} */

/* <BASDKey>
*********************************************************************************************************
* $History__:$
*********************************************************************************************************
</BASDKey>*/
