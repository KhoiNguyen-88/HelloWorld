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
#if ( MCU_RB_MACHINE_FAM == MCU_RB_JDP_UC1 )
#include "rba_Reg.h"
#include RBA_REG_DMAMUX_H
#endif
#if (MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1)
#include "rba_Reg.h"
#include RBA_REG_SRC_H
#endif

#if (MCU_RB_MACHINE_DEV == MCU_RB_ST_UC1_DEV3_40NM_ALL)
/* table for ST DEV3PLUS DMACHMUX register access  */
#define RBA_DMA_START_SEC_VAR_INIT_32
#include "rba_Dma_MemMap.h"
static rba_Reg_DMAMUX_tst* rba_Dma_Prv_adrDmaMux_apst[RBA_DMA_NUM_MUX] = {&DMAMUX_0, &DMAMUX_1, &DMAMUX_2, &DMAMUX_3, &DMAMUX_4, &DMAMUX_5};
#define RBA_DMA_STOP_SEC_VAR_INIT_32
#include "rba_Dma_MemMap.h"

#endif

#define RBA_DMA_START_SEC_CODE_FAST
#include "rba_Dma_MemMap.h"
/**
 ***************************************************************************************************
 * \brief Function to acknowledge DMA interrupt requests
 *
 * This function acknowledges any DMA triggered interrupt request on JDP controllers, it does
 * nothing on IFX controllers.
 *
 * \param[in]   rba_Dma_Id_ten const idChan_cen  DMA channel identifier
 *
 * \return      Std_ReturnType  returns E_OK or E_NOT_OK
 *
 * When used on IFX controllers the function will do nothing and returns with E_OK.
 *
 * \pre [Optional remove tag if not needed] Here you can describe the preconditions for the function
 *
 * \rb_example [Optional remove tag if not needed]
 * \code
 * void AnyDmaIsr(void)
 * {
 *      rba_Dma_AckIrq( rba_Dma_Id_ten ChannelId )
 * }
 * \endcode
 *
 * \attention [Optional remove tag if not needed] Will be marked with a red marker in the output
 *
 * \note [Optional remove tag if not needed] Will be marked with a yellow marker in the output
 ***************************************************************************************************
 */
Std_ReturnType rba_Dma_AckIrq(rba_Dma_Id_ten const idChan_cen)
{
#if ( MCU_RB_MACHINE_FAM == MCU_RB_JDP_UC1 )
    Std_ReturnType stRet = E_NOT_OK;
    uint8_least    idxUnit_qu8 = 0;
    uint8_least    idxCh_qu8   = 0;
    // check for valid channel ID
    if ( idChan_cen < RBA_DMA_NUM_IDS_E )
    {
        // read unit and channel number
        idxUnit_qu8 = rba_Dma_Cfg_Rslvr_acst[idChan_cen][rba_Dma_idxPbVar].idxUnit_u8;
        idxCh_qu8   = rba_Dma_Cfg_Rslvr_acst[idChan_cen][rba_Dma_idxPbVar].idxChan_u8;
        // check both numbers
        if  ( (idxUnit_qu8 < RBA_DMA_NUM_DMA_MODULES) || (idxCh_qu8 < RBA_DMA_NUM_DMA_CHANS) )
        {
            // acknowledge interrupt
            eDMA_PTR(idxUnit_qu8)->CINT = (uint8)idxCh_qu8;
            stRet = E_OK;
        }
    }
    return stRet;
#else
    // interrupts are automatically acknowledged in IFX controllers
    (void)idChan_cen;
    return E_OK;
#endif
}
#define RBA_DMA_STOP_SEC_CODE_FAST
#include "rba_Dma_MemMap.h"

#define RBA_DMA_START_SEC_CODE_NORMAL
#include "rba_Dma_MemMap.h"

/**
 ***************************************************************************************************
 * \brief Function to en/disable JDP DMA trigger MUX channel
 *
 * This function can be used to enable or disable a DMA mUX input on JDP controllers, it does
 * nothing on IFX controllers.
 *
 * \param[in]   rba_Dma_Id_ten const idChan_cen   DMA channel identifier
 * \param[in]   boolean        const swtOnOff_cb  switch direction
 *
 * \return      Std_ReturnType  returns E_OK or E_NOT_OK
 *
 * swtOnOff_cb = TRUE enables the MUX input, FALSE disables the input.
 *
 * \pre [Optional remove tag if not needed] Here you can describe the preconditions for the function
 *
 * \rb_example [Optional remove tag if not needed]
 * \code
 * void AnyDmaIsr(void)
 * {
 *      if (E_OK = rba_Dma_SwtMux( rba_Dma_Id_ten ChannelId, TRUE ))
 *      {
 *          -- ...
 *      }
 * }
 * \endcode
 *
 * \attention [Optional remove tag if not needed] Will be marked with a red marker in the output
 *
 * \note [Optional remove tag if not needed] Will be marked with a yellow marker in the output
 ***************************************************************************************************
 */
Std_ReturnType rba_Dma_SwtMux(rba_Dma_Id_ten const idChan_cen, boolean const swtOnOff_cb)
{
#if ( MCU_RB_MACHINE_FAM == MCU_RB_JDP_UC1 )
    Std_ReturnType stRet = E_NOT_OK;
    uint8_least    idxUnit_qu8 = 0;
    uint8_least    idxCh_qu8   = 0;
    uint8_least    idxMux_qu8;
    uint8_least    idxMuxCh_qu8;
    // check for valid channel ID
    if ( idChan_cen < RBA_DMA_NUM_IDS_E )
    {
        // read unit and channel number
        idxUnit_qu8 = rba_Dma_Cfg_Rslvr_acst[idChan_cen][rba_Dma_idxPbVar].idxUnit_u8;
        idxCh_qu8   = rba_Dma_Cfg_Rslvr_acst[idChan_cen][rba_Dma_idxPbVar].idxChan_u8;
        // check both numbers
        if  ( (idxUnit_qu8 < RBA_DMA_NUM_DMA_MODULES) || (idxCh_qu8 < RBA_DMA_NUM_DMA_CHANS) )
        {
#if (MCU_RB_MACHINE_DEV == MCU_RB_ST_UC1_DEV3_40NM_ALL)
            // calculate MUX and MUX channel number based on DMA channel / move engine
            // DMAMUX0: ME1.CH 0-15   DEV3+
            // DMAMUX1: ME1.CH16-31   DEV3+
            // DMAMUX2: ME1.CH32-47   DEV3+
            // DMAMUX3: ME1.CH48-63   DEV3+
            // DMAMUX4: ME0.CH 0-15   DEV3+
            // DMAMUX5: ME0.CH16-31   DEV3+
            if ( idxUnit_qu8 == 0u )
            {
                idxMux_qu8   = (idxCh_qu8 / 16u) + 4u;
                idxMuxCh_qu8 = idxCh_qu8 % 16u;
            }
            else // ( idxUnit_qu8 == 1 )
            {
                idxMux_qu8   = idxCh_qu8 / 16u;
                idxMuxCh_qu8 = idxCh_qu8 % 16u;
            }
#elif ( MCU_RB_MACHINE_FAM == MCU_RB_JDP_UC1 )
            // calculate MUX and MUX channel number based on DMA channel / move engine
            // DMAMUX0: ME0.CH 0- 7   DEV1, DEV2, DEV3, DEV4
            // DMAMUX1: ME0.CH 8-15   DEV1, DEV2, DEV3, DEV4
            // DMAMUX2: ME0.CH16-23         DEV2, DEV3, DEV4
            // DMAMUX3: ME0.CH24-31         DEV2, DEV3, DEV4
            // DMAMUX4: ME0.CH32-47               DEV3, DEV4
            // DMAMUX5: ME0.CH48-63               DEV3, DEV4
            // DMAMUX6: ME1.CH 0-15                     DEV4
            // DMAMUX7: ME1.CH16-31                     DEV4
            // DMAMUX8: ME1.CH32-47                     DEV4
            // DMAMUX9: ME1.CH48-63                     DEV4
            if ( idxUnit_qu8 == 0u )
            {
                if ( idxCh_qu8 >= 48u )
                {
                    idxMux_qu8   = 5;
                    idxMuxCh_qu8 = idxCh_qu8 % 16u;
                }
                else if ( idxCh_qu8 >= 32u )
                {
                    idxMux_qu8   = 4;
                    idxMuxCh_qu8 = idxCh_qu8 % 16u;
                }
                else
                {
                    idxMux_qu8   = idxCh_qu8 / 8u;
                    idxMuxCh_qu8 = idxCh_qu8 % 8u;
                }
            }
            else // ( idxUnit_qu8 == 1 )
            {
                idxMux_qu8   = (idxCh_qu8 / 16u) + 6u;
                idxMuxCh_qu8 = idxCh_qu8 % 16u;
            }
#endif
            // enable / disable MUX channel
            if ( swtOnOff_cb != FALSE )
            {
#if (MCU_RB_MACHINE_DEV == MCU_RB_ST_UC1_DEV3_40NM_ALL)
                // set CHCONFIG.ENBL and CHCONFIG.SOURCE
                rba_Dma_Prv_adrDmaMux_apst[idxMux_qu8]->CHCONFIG[idxMuxCh_qu8] = rba_Dma_Mux_acu8[rba_Dma_idxPbVar][idxMux_qu8][idxMuxCh_qu8];
#else
                DMAMUX[idxMux_qu8].CHCONFIG[idxMuxCh_qu8] = rba_Dma_Mux_acu8[rba_Dma_idxPbVar][idxMux_qu8][idxMuxCh_qu8];
#endif
            }
            else
            {
#if (MCU_RB_MACHINE_DEV == MCU_RB_ST_UC1_DEV3_40NM_ALL)
                // clear CHCONFIG.ENBL and CHCONFIG.SOURCE
                rba_Dma_Prv_adrDmaMux_apst[idxMux_qu8]->CHCONFIG[idxMuxCh_qu8] = 0;
#else
                DMAMUX[idxMux_qu8].CHCONFIG[idxMuxCh_qu8] = 0;
#endif

            }
            stRet = E_OK;
        }
    }
    return stRet;
#else
    // IFX controllers do not have a DMA input MUX
    (void)idChan_cen;
    (void)swtOnOff_cb;
    return E_OK;
#endif
}


/**
 ***************************************************************************************************
 * \brief Function to check DMA channel status
 *
 * This function does a simple DMA channel check
 *
 * \param[in]   rba_Dma_Id_ten const idChan_cen  DMA channel identifier
 * \param[out]  uint32 * const stErr_pu32        DMA channel status information
 *
 * \return          Std_ReturnType  returns E_OK or E_NOT_OK
 *
 * stErr_pu32 provides following error information:
 *    RBA_DMA_GETCHSTS_CHACTV_ERR_POS:     this bit is set if the channel is active when the function is called
 *    RBA_DMA_GETCHSTS_REQLOST_ERR_POS:    this bit is set if a request lost event was detected (IFX only)
 *
 * The function will return E_NOT_OK if the DMA channel identifier resolving fails, no valid error
 * information is returned in this case. If the resolving is successful the function will return E_OK
 * regardless of the actual channel status (i.e. it will return E_OK even if there are channel errors
 * reported through stErr_pu32)
 * The function does not check the move engine status registers, the monitoring of such kind of errors is done
 * in a different way (error interrupt or rba_Emm actions).
 * IFX only: If there is a request lost event detected the function will reset this event to prevent
 * a false positve detection in the next function call.
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
Std_ReturnType rba_Dma_GetChSts(rba_Dma_Id_ten const idChan_cen,
                                uint32 *       const stErr_pu32)
{
    Std_ReturnType stRet;
    uint8_least    idxUnit_qu8 = 0;
    uint8_least    idxCh_qu8   = 0;

    // clean status bitfield as some bits might be set from previous call
    *stErr_pu32 = 0;
    // do error checks
    if ( idChan_cen >= RBA_DMA_NUM_IDS_E )
    {
        stRet = E_NOT_OK;
        *stErr_pu32 |= 1uL << RBA_DMA_GETCHSTS_ID_ERR_POS;
    }
    else
    {
        // read unit and channel number
        idxUnit_qu8 = rba_Dma_Cfg_Rslvr_acst[idChan_cen][rba_Dma_idxPbVar].idxUnit_u8;
        idxCh_qu8   = rba_Dma_Cfg_Rslvr_acst[idChan_cen][rba_Dma_idxPbVar].idxChan_u8;
        stRet = E_OK;
        // check both numbers
        if  ( (idxUnit_qu8 >= RBA_DMA_NUM_DMA_MODULES) || (idxCh_qu8 >= RBA_DMA_NUM_DMA_CHANS) )
        {
            stRet = E_NOT_OK;
            *stErr_pu32 |= 1uL << RBA_DMA_GETCHSTS_RSLV_ERR_POS;
        }
    }
    // read channel status if checks were OK
    if ( stRet == E_OK )
    {
        // further action is machine type dependent
#if ( MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1 )
        uint32 regTsr_u32;
        // check if channel is currently active
        regTsr_u32 = DMA.TSR[idxCh_qu8];
        if ( (regTsr_u32 & (1uL << RBA_REG_DMA_TSRx_CH_POS)) != 0u )
        {
            *stErr_pu32 |= 1uL << RBA_DMA_GETCHSTS_CHACTV_ERR_POS;
        }
        // check request lost bit
        if ( (regTsr_u32 & (1uL << RBA_REG_DMA_TSRx_TRL_POS)) != 0u )
        {
            *stErr_pu32 |= 1uL << RBA_DMA_GETCHSTS_REQLOST_ERR_POS;
            // clear TRL bit to avoid false positives in further calls
            DMA.TSR[idxCh_qu8] |= 1uL << RBA_REG_DMA_TSRx_CTL_POS;
        }
#endif
#if ( MCU_RB_MACHINE_FAM == MCU_RB_JDP_UC1 )
        // check if channel is currently running
        if ((eDMA_PTR(idxUnit_qu8)->TCD[idxCh_qu8].CSR & (uint16)((uint16)1u << RBA_REG_DMA_TCDx_CSR_ACTIVE_POS)) != 0u)
        {
            *stErr_pu32 |= 1uL << RBA_DMA_GETCHSTS_CHACTV_ERR_POS;
        }
        // check for a lost request cannot be performed on JDP controllers
#endif
    }
    return stRet;
}


/**
 ***************************************************************************************************
 * \brief Simple DMA initialisation function
 *
 * This function provides abstraction of a simple DMA channel set-up.
 *
 * \param[in]       rba_Dma_Id_ten    DMA channel identifier
 * \param[in]       adrConfig_cpcst   pointer to configuration structure, see below.
 * \param[out]      stErr_pu32        pointer to error channel/function error information. This parameter
 *                                    is updated on every function call, zero means successful DMA channel programming
 *
 * \return          Std_ReturnType  returns E_OK or E_NOT_OK
 *
 * The configuration structure of type rba_Dma_SetUpCh_tst provides following parameters
 *    void*   adrSrc_pv   :                source address pointer, may point to uint8, uint16 or uint32
 *    void*   adrDest_pv  :                destination address pointer, may point to uint8, uint16 or uint32
 *    uint32  numTrig_u32 :                number of triggers, outer loop counter, how many SW or HW triggers
 *                                         shall be processed until DMA sequence ends
 *    uint32  numMoves_u32:                number of data moves per trigger, number is DMA HW dependent!
 *    uint8   swtSize_u8  :                data transfer size, valid values are
 *                                         0: uint8
 *                                         1: uint16
 *                                         2: uint32
 *    boolean swtIncSrc_b :                increment source address, valid values are
 *                                         TRUE:  source address is in/de-cremented based on transfer size
 *                                         FLASE: source address is not incremented
 *    boolean swtIncDest_b:                increment destination address, valid values are
 *                                         TRUE:  destination address is in/de-cremented based on transfer size
 *                                         FLASE: destination address is not incremented
 *    boolean swtEnaReq_b :                enable service request trigger after end of outer loop
 *                                         TRUE:  enable service request trigger
 *                                         FLASE: no service request trigger is generated
 *    uint8   swtChTrig_u8:                start channel / enable HW request, valid values are:
 *                                           0: do nothing
 *                                           1: enable HW trigger for DMA channel
 *                                           2: do a SW trigger after channel initialisation
 *                                           4: enable channel linking after inner loop
 *                                           8: enable channel linking after outer loop
 *                                          16: channel is trigger target in a linked-channel configuration
 *                                          32: keep HW trigger enabled after outer loop
 *                                          64: enable circular buffer configuration
 *                                         128: enable extended option bits in swtOptions2_u32
 *    uint32  swtOptions2_u32:             This parameter is only evaluated if bit 7 in swtChTrig_u8 is set.
 *                                           0: enable circular buffer configuration for source
 *                                           1: enable circular buffer configuration for destination
 *                                           2: decrement source address (only if swtIncSrc_b is TRUE)
 *                                           4: decrement destination address (only if swtIncDest_b is TRUE)
 *
 *    uint16  numSrcBuf_u16:               source buffer size for wrap-around buffer
 *    uint16  numDestBuf_u16:              target buffer size for wrap-around buffer
 *
 * If there is an ongoing transaction (i.e. the DMA channel is active) the function waits for the transaction
 * to be finished. If the previously programmed channel setup is not completely finished the function performes
 * a channel reset and sets the bit RBA_DMA_SETUPCH_CHNOTREADY_ERR_POS in *stErr_cpu32. Afterwards the new channel
 * settings are programmed normally and the function will return E_OK.
 * Following conditions will prevent the function from programming a DMA channel
 * - resolving of channel ID fails, bit RBA_DMA_SETUPCH_ID_ERR_POS is set in *stErr_cpu32
 * - resolved channel or move engine numbers are out of range, bit RBA_DMA_SETUPCH_RSLV_ERR_POS is set in
 *   *stErr_cpu32. This normally happens in post-build environments when the channel is not configured in the
 *   currently active post build variant.
 * - function parameter swtSize_u8 is out of range, bit RBA_DMA_SETUPCH_SWTSIZE_ERR_POS is set in *stErr_cpu32
 * - function parameter swtChTrig_u8 is out of range, bit RBA_DMA_SETUPCH_SWTCHTRIG_ERR_POS is set in *stErr_cpu32
 * - function parameter numMoves_u32 is out of range, bit RBA_DMA_SETUPCH_NUMMOVES_ERR_POS is set in *stErr_cpu32
 * In all of the listed cases the function will return E_NOT_OK and won't do a DMA channel configuration.
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
Std_ReturnType rba_Dma_SetUpCh(rba_Dma_Id_ten              const idChan_cen,
                               rba_Dma_SetUpCh_tst const * const adrConfig_cpcst,
                               uint32                    * const stErr_cpu32)
{
    Std_ReturnType xRet;
    uint8_least idxUnit_qu8  = 0;
    uint8_least idxCh_qu8    = 0;
#if ( MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1 )
    uint32      regChcfgr_u32 = 0;
    uint8       idxLinkMod_u8 = 0;
    uint32      numMoves_u32;
#endif
    uint8       idxLinkCh_u8  = 0;
    uint32      numTrig_u32;
    uint32      swtOptions2_u32 = 0;
    uint8       xSrcOffset_u8 = 0;
    uint8       xDstOffset_u8 = 0;

    // clean status bitfield as some bits might be set from previous call
    *stErr_cpu32 = 0;
    // do error checks,
    if ( idChan_cen >= RBA_DMA_NUM_IDS_E )
    {
        xRet         = E_NOT_OK;
        *stErr_cpu32 |= 1uL << RBA_DMA_SETUPCH_ID_ERR_POS;
    }
    else
    {
        // read unit and channel number
        idxUnit_qu8 = rba_Dma_Cfg_Rslvr_acst[idChan_cen][rba_Dma_idxPbVar].idxUnit_u8;
        idxCh_qu8   = rba_Dma_Cfg_Rslvr_acst[idChan_cen][rba_Dma_idxPbVar].idxChan_u8;
        xRet = E_OK;
        // check both numbers
        if  ( (idxUnit_qu8 >= RBA_DMA_NUM_DMA_MODULES) || (idxCh_qu8 >= RBA_DMA_NUM_DMA_CHANS) )
        {
            xRet          = E_NOT_OK;
            *stErr_cpu32 |= 1uL << RBA_DMA_SETUPCH_RSLV_ERR_POS;
        }
    }
    // check parameter swtSize_u8
    if ( adrConfig_cpcst->swtSize_u8 > 2 )
    {
        xRet         = E_NOT_OK;
        *stErr_cpu32 |= 1uL << RBA_DMA_SETUPCH_SWTSIZE_ERR_POS;
    }

    // check parameter swtOptions2_u32 if enabled by swtChTrig_u8
    if ( (adrConfig_cpcst->swtChTrig_u8 > 0x7F) && (adrConfig_cpcst->swtOptions2_u32 > 0x3FFuL) )
    {
        xRet         = E_NOT_OK;
        *stErr_cpu32 |= 1uL << RBA_DMA_SETUPCH_SWTCHTRIG_ERR_POS;
    }
    // use a local copy of swtOptions2_u32 to have it reliably cleared (initialised to '0') when not
    // activated by swtChTrig_u8 (it might be uninitialised and thus != zero in this case!)
    // therefore: overwrite the local swtOptions2_u32 only if it is activated in swtChTrig_u8
    else if ( adrConfig_cpcst->swtChTrig_u8 > 0x7F )
    {
        swtOptions2_u32 = adrConfig_cpcst->swtOptions2_u32;
        // Copy the Source and Destination Address Offsets to a local variable
        xSrcOffset_u8 = (uint8)( (swtOptions2_u32 & RBA_DMA_SETUPCH_SRC_OFFSET_MSK)  >> RBA_DMA_SETUPCH_SRC_OFFSET_POS );
        xDstOffset_u8 = (uint8)( (swtOptions2_u32 & RBA_DMA_SETUPCH_DST_OFFSET_MSK)  >> RBA_DMA_SETUPCH_DST_OFFSET_POS );
    }
    else
    {
        // nothing to do here
    }
    // to keep its original meaning and to be able to check only one variable:
    // if RBA_DMA_SETUPCH_ENA_RING_BUF_POS is set also set the two new bits in swtOptions2_u32
    if ( (adrConfig_cpcst->swtChTrig_u8 & (uint8)(RBA_DMA_SETUPCH_ENA_RING_BUF_MSK)) != 0u )
    {
        // if yes, set both circular/ring buffer bits in swtOptions2_u32
        swtOptions2_u32 |= (  (1uL << RBA_DMA_SETUPCH_ENA_RING_BUFF_SRC_POS)
                            | (1uL << RBA_DMA_SETUPCH_ENA_RING_BUFF_DST_POS)
                           );
    }

    // check parameter numMoves_u32, this is machine type dependent
    // take a copy of numTrig_u32 as it might be modified here (only for IFX)
    numTrig_u32 = adrConfig_cpcst->numTrig_u32;
#if ( MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1 )
    // take a local copy because it might be modified here (only for IFX)
    numMoves_u32 = adrConfig_cpcst->numMoves_u32;
    if (!(    (numMoves_u32 ==  1u)
           || (numMoves_u32 ==  2u)
           || (numMoves_u32 ==  3u)
           || (numMoves_u32 ==  4u)
           || (numMoves_u32 ==  5u)
           || (numMoves_u32 ==  8u)
           || (numMoves_u32 ==  9u)
           || (numMoves_u32 == 16u)
       ) )
    {
        // other than the listed number of moves are only possible if numTrig_u32 == 1
        // use the RROAT feature for this kind of configuraion
        if (numTrig_u32 == 1u)
        {
            // directly set RROAT bit
            regChcfgr_u32 |= 1uL << RBA_REG_DMA_CHCFGRx_RROAT_POS;
            // set numTrig_u32 = numMoves_u32 because numTrig_u32 is copied to TREL
            numTrig_u32   = numMoves_u32;
            // set numMoves_u32 = 1 because BLKM must be set to '0' in this case
            numMoves_u32  = 1u;
        }
        else
        {
            xRet         = E_NOT_OK;
            *stErr_cpu32 |= 1uL << RBA_DMA_SETUPCH_NUMMOVES_ERR_POS;
        }
    }
#endif
#if ( MCU_RB_MACHINE_FAM == MCU_RB_JDP_UC1 )
    // for JDP controllers the size check is data width dependent as transfer size is always configured as byte
    // no check is done for 8-bit transfer as the maximum number (numMoves_u32) of uint32 is possible there
    // the size would be reduced if minor loop or minor loop offset is enabled (which is not in this case)
    if (    ((adrConfig_cpcst->swtSize_u8 == 1u) && (adrConfig_cpcst->numMoves_u32 > 0x7FFFFFFFuL))      // uint16 check
         || ((adrConfig_cpcst->swtSize_u8 == 2u) && (adrConfig_cpcst->numMoves_u32 > 0x3FFFFFFFuL))      // uint32 check
       )
    {
        xRet         = E_NOT_OK;
        *stErr_cpu32 |= 1uL << RBA_DMA_SETUPCH_NUMMOVES_ERR_POS;
    }
#endif

    // check parameter numTrig_u32
#if ( MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1 )
    if ( numTrig_u32 > 0x3FFFu )
#else
    if (  ( numTrig_u32 > 0x7FFFu )
       || ( (  (adrConfig_cpcst->swtChTrig_u8 & RBA_DMA_SETUPCH_TRG_LINK_ANY_MSK) != 0u
          ) && (numTrig_u32 > 0x1FFu) )
       )
#endif
    {
        xRet         = E_NOT_OK;
        *stErr_cpu32 |= 1uL << RBA_DMA_SETUPCH_NUMTRIG_ERR_POS;
    }

    // channel linking: if a linked trigger is selected check if the corresponding configuration is present
    if ( (adrConfig_cpcst->swtChTrig_u8 & RBA_DMA_SETUPCH_TRG_LINK_ANY_MSK) != 0u )
    {
        // get linked channel ID
        rba_Dma_Id_ten idLinkCh_en = rba_Dma_Cfg_Rslvr_acst[idChan_cen][rba_Dma_idxPbVar].idLinkCh_en;
        // now check if a valid entry is present
        if (idLinkCh_en < RBA_DMA_NUM_IDS_E)
        {
            // get linked chennel parameters
#if ( MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1 )
            idxLinkMod_u8 = rba_Dma_Cfg_Rslvr_acst[idLinkCh_en][rba_Dma_idxPbVar].idxUnit_u8;
#endif
            idxLinkCh_u8  = rba_Dma_Cfg_Rslvr_acst[idLinkCh_en][rba_Dma_idxPbVar].idxChan_u8;
        }
        else
        {
            xRet         = E_NOT_OK;
            *stErr_cpu32 |= 1uL << RBA_DMA_SETUPCH_LINKCH_ERR_POS;
        }
    }

    // do circular buffer checks in case circular buffer is enabled (circular buffer == ring buffer)
    // check source side
    if ( (swtOptions2_u32 & RBA_DMA_SETUPCH_ENA_RING_BUFF_SRC_MSK) != 0u )
    {
        // get source & destination buffer size
        uint32 numSrcBuf_u32  = (uint32)adrConfig_cpcst->numSrcBuf_u16;
        // check if buffer size is  a power of 2
        // and check if buffer size is != 0
        // and check if buffer size (in bytes!) is not larger than 64k bytes
        if (  ((numSrcBuf_u32 & (numSrcBuf_u32 - 1u)) != 0u)
           || (numSrcBuf_u32 == 0u)
           || ((numSrcBuf_u32 * (1uL << adrConfig_cpcst->swtSize_u8)) > 0x10000uL)
           )
        {
            xRet         = E_NOT_OK;
            *stErr_cpu32 |= 1uL << RBA_DMA_SETUPCH_SRCBUFSIZE_ERR_POS;
        }
    }
    // check destination side
    if ( (swtOptions2_u32 & RBA_DMA_SETUPCH_ENA_RING_BUFF_DST_MSK) != 0u )
    {
        // get destination buffer size
        uint32 numDestBuf_u32 = (uint32)adrConfig_cpcst->numDestBuf_u16;
        // check if buffer size is  a power of 2
        // and check if buffer size is != 0
        // and check if buffer size (in bytes!) is not larger than 64k bytes
        if (  ((numDestBuf_u32 & (numDestBuf_u32 - 1u)) != 0u)
           || (numDestBuf_u32 == 0u)
           || ((numDestBuf_u32 * (1uL << adrConfig_cpcst->swtSize_u8)) > 0x10000uL)
           )
        {
            xRet         = E_NOT_OK;
            *stErr_cpu32 |= 1uL << RBA_DMA_SETUPCH_DESTBUFSIZE_ERR_POS;
        }
    }
    // do compatibility check to tolerate numTrig_u32 = 0 without interrupts in circular buffer mode
    // this is only done if the old bit RBA_DMA_SETUPCH_ENA_RING_BUF_MSK is used
    if ( (adrConfig_cpcst->swtChTrig_u8 & (uint8)(RBA_DMA_SETUPCH_ENA_RING_BUF_MSK)) != 0u )
    {
        // if no interrupt is required set numTrig_u32 to MAX(numSrcBuf_u16, numDestBuf_u16)
        if (adrConfig_cpcst->swtEnaReq_b == FALSE)
        {
            // first take adrConfig_cpcst->numSrctBuf_u16
            numTrig_u32 = adrConfig_cpcst->numSrcBuf_u16;
            // then check adrConfig_cpcst->numDestBuf_u16
            if (adrConfig_cpcst->numDestBuf_u16 > numTrig_u32)
            {
                numTrig_u32 = adrConfig_cpcst->numDestBuf_u16;
            }
        }
        else
        {
            // in circular buffer mode
            // AND
            // with activated interrupt triggers numTrig_u32 must have a feasible value > 0
            if (numTrig_u32 == 0u)
            {
                xRet         = E_NOT_OK;
                *stErr_cpu32 |= 1uL << RBA_DMA_SETUPCH_NUMTRIG_ERR_POS;
            }
        }
    }
    else
    {
        // check if numTrig_u32 is zero in case no circular buffer is selected
        if (numTrig_u32 == 0u)
        {
            xRet         = E_NOT_OK;
            *stErr_cpu32 |= 1uL << RBA_DMA_SETUPCH_NUMTRIG_ERR_POS;
        }
    }

    // start channel init if checks were OK
    if ( xRet == E_OK )
    {
        // further action is machine type dependent
#if ( MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1 )

        uint32 regTsr_u32    = 0;
        uint32 regAdicr_u32  = 0;
        // first, disable HW request to avoid disturbance while re-programming
        // set DCH = 1 (overwrite other register contents)
        DMA.TSR[idxCh_qu8] = 1uL << RBA_REG_DMA_TSRx_DCH_POS;
        // check for an ongoing request, wait until it is finished
        while ( (DMA.TSR[idxCh_qu8] & (1uL << RBA_REG_DMA_TSRx_CH_POS)) != 0u )
        {
            Mcu_Rb_WaitNop(8);                      // do not overload the system bus
        }
        // check if channel is ready to be configured (TCOUNT must be zero)
        if ( (DMA.CH[idxCh_qu8].CHCSR & rba_Reg_Mask(DMA, CHCSR, CHCSRx_TCOUNT)) > 0u )
        {
            // if not, execute a channel reset, follow the sequence described in manual
            // 1.) set DCH = 1 (overwrite other register contents)
            // this was already done above
            // 2.) Writing a 1 to TSRx.RST
            DMA.TSR[idxCh_qu8] = 1uL << RBA_REG_DMA_TSRx_RST_POS;
            // 3.) Waiting (polling) until TSRx.RST = 0.
            while ( (DMA.TSR[idxCh_qu8] & (1uL << RBA_REG_DMA_TSRx_RST_POS)) != 0u )
            {
            }
            // indicate that a channel reset was necessary
            *stErr_cpu32 |= 1uL << RBA_DMA_SETUPCH_CHNOTREADY_ERR_POS;
        }
        /* MR12 RULE 11.4, 11.6 VIOLATION: The warning is uncritical and non-removable since cast is required to
         * initialize hardware registers with an address - Approved by MISRA2012_SUPPORT_TEAM. */
        // set source address (skip if it is a NULL_PTR)
        if (adrConfig_cpcst->adrSrc_pv != NULL_PTR)
        {
            DMA.CH[idxCh_qu8].SADR = (uint32)adrConfig_cpcst->adrSrc_pv;
        }
        // set destination address (skip if it is a NULL_PTR)
        if (adrConfig_cpcst->adrDest_pv != NULL_PTR)
        {
            DMA.CH[idxCh_qu8].DADR = (uint32)adrConfig_cpcst->adrDest_pv;
        }
        // check if circular buffer is enabled for source
        if ( (swtOptions2_u32 & RBA_DMA_SETUPCH_ENA_RING_BUFF_SRC_MSK) != 0u )
        {
            // get source buffer size
            uint32 numSrcBuf_u32 = (uint32)adrConfig_cpcst->numSrcBuf_u16;
            uint8  xCbls_u8      = 0;
            // calculate buffer size in bytes, this depends on the configured data width
            // multiply by 1 for uint8 transfer size (swtSize_u8 = 0)
            // multiply by 2 for uint16 transfer size (swtSize_u8 = 1)
            // multiply by 4 for uint32 transfer size (swtSize_u8 = 2)
            numSrcBuf_u32  *= 1uL << adrConfig_cpcst->swtSize_u8;
            // enable circular buffer for source in ADICR
            regAdicr_u32 |= 1uL << RBA_REG_DMA_ADICRx_SCBE_POS;
            // check for source buffer wrap around
            if ( adrConfig_cpcst->numSrcBuf_u16 > 1 )
            {
                // calculate and set CBLS bitfield value
                while ((numSrcBuf_u32 & 1u) == 0u)
                {
                    xCbls_u8++;
                    numSrcBuf_u32 >>= 1u;
                }
                regAdicr_u32 |= ((uint32)xCbls_u8 << RBA_REG_DMA_ADICRx_CBLS_POS);
                // set INCS if DECREMENT_SRC is _not_ set
                if ( (swtOptions2_u32 & RBA_DMA_SETUPCH_DECREMENT_SRC_MSK) == 0u )
                {
                    regAdicr_u32 |= 1uL << RBA_REG_DMA_ADICRx_INCS_POS;
                }
                // prepare regAdicr_u32 with the source Modification Factor
                regAdicr_u32 |= (xSrcOffset_u8 << RBA_REG_DMA_ADICRx_SMF_POS);
            }
        }
        // configure linaear mode for source
        else
        {
            // check if source address modification is configured
            if (adrConfig_cpcst->swtIncSrc_b != FALSE)
            {
                // check if address shall be incremented: If yes, set INCS = 1
                if ( (swtOptions2_u32 & RBA_DMA_SETUPCH_DECREMENT_SRC_MSK) == 0u )
                {
                    // INCS = 1
                    regAdicr_u32 |= (1uL << RBA_REG_DMA_ADICRx_INCS_POS);
                }
                // prepare regAdicr_u32 with the source Modification Factor
                regAdicr_u32 |= (xSrcOffset_u8 << RBA_REG_DMA_ADICRx_SMF_POS);
            }
            else
            {
                // enable circular buffer in this case to prevent modification
                // additionally, circular buffer size must be 0 (ADICR.CBLS = 0)!
                regAdicr_u32 |= (1uL << RBA_REG_DMA_ADICRx_SCBE_POS);
            }
        }
        // check if circular buffer is enabled for destination
        if ( (swtOptions2_u32 & RBA_DMA_SETUPCH_ENA_RING_BUFF_DST_MSK) != 0u )
        {
            // get source buffer size
            uint32 numDestBuf_u32 = (uint32)adrConfig_cpcst->numDestBuf_u16;
            uint8  xCbld_u8 = 0;
            // calculate buffer size in bytes, this depends on the configured data width
            // multiply by 1 for uint8 transfer size (swtSize_u8 = 0)
            // multiply by 2 for uint16 transfer size (swtSize_u8 = 1)
            // multiply by 4 for uint32 transfer size (swtSize_u8 = 2)
            numDestBuf_u32 *= 1uL << adrConfig_cpcst->swtSize_u8;
            // enable circular buffer for destination in ADICR
            regAdicr_u32 |= 1uL << RBA_REG_DMA_ADICRx_DCBE_POS;
            // check for destination buffer wrap around
            if ( adrConfig_cpcst->numDestBuf_u16 > 1 )
            {
                // calculate and set CBLD bitfield value
                while ((numDestBuf_u32 & 1u) == 0u)
                {
                    xCbld_u8++;
                    numDestBuf_u32 >>= 1u;
                }
                regAdicr_u32 |= ((uint32)xCbld_u8 << RBA_REG_DMA_ADICRx_CBLD_POS);
                // set INCD if DECREMENT_DST is _NOT_ set
                if ( (swtOptions2_u32 & RBA_DMA_SETUPCH_DECREMENT_DST_MSK) == 0u )
                {
                    regAdicr_u32 |= 1uL << RBA_REG_DMA_ADICRx_INCD_POS;
                }
                // prepare regAdicr_u32 with the destination Modification Factor
                regAdicr_u32 |= (xDstOffset_u8 << RBA_REG_DMA_ADICRx_DMF_POS);
            }
        }
        // configure linaear mode for destination
        else
        {
            // check if destination address modification is configured
            if (adrConfig_cpcst->swtIncDest_b != FALSE)
            {
                // check if address shall be incremented: If yes, set INCD = 1
                if ( (swtOptions2_u32 & RBA_DMA_SETUPCH_DECREMENT_DST_MSK) == 0u )
                {
                    // INCD = 1
                    regAdicr_u32 |= (1uL << RBA_REG_DMA_ADICRx_INCD_POS);
                }
                // prepare regAdicr_u32 with the destination Modification Factor
                regAdicr_u32 |= (xDstOffset_u8 << RBA_REG_DMA_ADICRx_DMF_POS);

            }
            else
            {
                // enable circular buffer in this case to prevent modification
                // additionally, circular buffer size must be 0 (ADICR.CBLD = 0)!
                regAdicr_u32 |= (1uL << RBA_REG_DMA_ADICRx_DCBE_POS);
            }
        }
        // enable continuous mode for full circular buffer operation
        if (   ((swtOptions2_u32 & RBA_DMA_SETUPCH_ENA_RING_BUFF_SRC_MSK) != 0u)
            && ((swtOptions2_u32 & RBA_DMA_SETUPCH_ENA_RING_BUFF_DST_MSK) != 0u)
           )
        {
            regChcfgr_u32 |= 1uL << RBA_REG_DMA_CHCFGRx_CHMODE_POS;
        }

        // check if a (interrupt) service request trigger is configured
        if ( adrConfig_cpcst->swtEnaReq_b != FALSE )
        {
            regAdicr_u32 |= 2uL << RBA_REG_DMA_ADICRx_INTCT_POS;
        }
        else
        {
            // INTCT is zero in this case
        }
        // check if HW trigger is configured
        // also enable HW trigger if this channel is the target in a channel-linking setup
        if ( (adrConfig_cpcst->swtChTrig_u8 & ((RBA_DMA_SETUPCH_ENBL_HW_TRG_MSK) | (RBA_DMA_SETUPCH_LINK_TARGET_MSK)))
             != 0u
           )
        {
            regTsr_u32 |= 1uL << RBA_REG_DMA_TSRx_ECH_POS;
            // check if HW trigger shall be left enabled after completion of the outer loop
            if ( (adrConfig_cpcst->swtChTrig_u8 & (uint8)RBA_DMA_SETUPCH_KEEP_HWTRIGGER_MSK) != 0u )
            {
                regChcfgr_u32 |= 1uL << RBA_REG_DMA_CHCFGRx_CHMODE_POS;
            }
        }
        else
        {
            // disable HW trigger for the cases (0 or 2), is already done in the beginning
        }
        // set outer loop counter
        regChcfgr_u32 |= (numTrig_u32) << RBA_REG_DMA_CHCFGRx_TREL_POS;
        // set inner loop counter
        switch (numMoves_u32)
        {
            case  1uL: regChcfgr_u32 |= 0uL << RBA_REG_DMA_CHCFGRx_BLKM_POS;
            break;
            case  2uL: regChcfgr_u32 |= 1uL << RBA_REG_DMA_CHCFGRx_BLKM_POS;
            break;
            case  3uL: regChcfgr_u32 |= 5uL << RBA_REG_DMA_CHCFGRx_BLKM_POS;
            break;
            case  4uL: regChcfgr_u32 |= 2uL << RBA_REG_DMA_CHCFGRx_BLKM_POS;
            break;
            case  5uL: regChcfgr_u32 |= 6uL << RBA_REG_DMA_CHCFGRx_BLKM_POS;
            break;
            case  8uL: regChcfgr_u32 |= 3uL << RBA_REG_DMA_CHCFGRx_BLKM_POS;
            break;
            case  9uL: regChcfgr_u32 |= 7uL << RBA_REG_DMA_CHCFGRx_BLKM_POS;
            break;
            case 16uL: regChcfgr_u32 |= 4uL << RBA_REG_DMA_CHCFGRx_BLKM_POS;
            break;
            default: // numMoves_u32 is checked at the beginning
            break;
        }
        // set data width
        regChcfgr_u32 |= (uint32)(adrConfig_cpcst->swtSize_u8) << RBA_REG_DMA_CHCFGRx_CHDW_POS;
        // optionally configure channel linking via SRN
        if ( (adrConfig_cpcst->swtChTrig_u8 & RBA_DMA_SETUPCH_TRG_LINK_OUTER_MSK) != 0u )
        {
            // enable interrupt generation
            regAdicr_u32 |= 2uL << RBA_REG_DMA_ADICRx_INTCT_POS;
            // configure SRC of triggering channel with data for trigger target
            SRC.DMACH[idxCh_qu8] = RBA_DMA_CREATE_DMA_SRC(idxLinkMod_u8, idxLinkCh_u8);
            (void)idxLinkMod_u8;
        }
        if ( (adrConfig_cpcst->swtChTrig_u8 & RBA_DMA_SETUPCH_TRG_LINK_INNER_MSK) != 0u )
        {
            // enable interrupt generation
            regAdicr_u32 |= 3uL << RBA_REG_DMA_ADICRx_INTCT_POS;
            // configure SRC of triggering channel with data for trigger target
            SRC.DMACH[idxCh_qu8] = RBA_DMA_CREATE_DMA_SRC(idxLinkMod_u8, idxLinkCh_u8);
            (void)idxLinkMod_u8;
        }
        // write register ADICR
        DMA.CH[idxCh_qu8].ADICR = regAdicr_u32;
        // write value to CHCFGR & TSR
        DMA.CH[idxCh_qu8].CHCFGR = regChcfgr_u32;
        DMA.TSR[idxCh_qu8]       = regTsr_u32;
        // finally check if SW trigger is configured
        if ( (adrConfig_cpcst->swtChTrig_u8 & RBA_DMA_SETUPCH_IMDT_TRG_MSK) != 0u )
        {
            DMA.CH[idxCh_qu8].CHCSR |= 1uL << RBA_REG_DMA_CHCSRx_SCH_POS;
        }
#endif

#if ( MCU_RB_MACHINE_FAM == MCU_RB_JDP_UC1 )
        uint16 regAttr_u16     = 0;
        uint16 regCsr_u16      = 0;
        uint16 regXiter_u16    = 0;
        sint16 regSoffInit_s16 = 1;
        sint16 regDoffInit_s16 = 1;
        uint16 xSoff_u16;
        uint16 xDoff_u16;

        // prepare regSoffInit_u16 for later usage
        if ( (swtOptions2_u32 & RBA_DMA_SETUPCH_DECREMENT_SRC_MSK) != 0u )
        {
            regSoffInit_s16 = -1;
        }
        // prepare regDoffInit_u16 for later usage
        if ( (swtOptions2_u32 & RBA_DMA_SETUPCH_DECREMENT_DST_MSK) != 0u )
        {
            regDoffInit_s16 = -1;
        }

        /* Calculating the Source and Destination offsets from SRC_OFFSET and DST_OFFSET.
         * xSoff_u16 and xDoff_u16 are values to be written in SOFF and DOFF registers which are calculated from
         * 2^xSrcOffset_u8 and 2^xDstOffset_u8 respectively. */

        xSoff_u16 = 1u << xSrcOffset_u8;
        xDoff_u16 = 1u << xDstOffset_u8;

        // prepare regSoffInit_s16 and regDoffInit_s16 with the source and destination offset address
        regSoffInit_s16 = regSoffInit_s16 * (sint16)xSoff_u16;
        regDoffInit_s16 = regDoffInit_s16 * (sint16)xDoff_u16;

        // first, disable HW request to avoid disturbance while re-programming
        eDMA_PTR(idxUnit_qu8)->CERQ = (uint8)idxCh_qu8;
        // check for an ongoing request, wait until it is finished
        while ( (eDMA_PTR(idxUnit_qu8)->TCD[idxCh_qu8].CSR & (uint16)((uint16)1u << RBA_REG_DMA_TCDx_CSR_ACTIVE_POS)) != 0u )
        {
            Mcu_Rb_WaitNop(8);                      // do not overload the system bus
        }
        /* MR12 RULE 11.4, 11.6 VIOLATION: The warning is uncritical and non-removable since cast is required to
         * initialize hardware registers with an address - Approved by MISRA2012_SUPPORT_TEAM. */
        // set source address (skip if it is a NULL_PTR)
        if (adrConfig_cpcst->adrSrc_pv != NULL_PTR)
        {
            eDMA_PTR(idxUnit_qu8)->TCD[idxCh_qu8].SADDR = (uint32)adrConfig_cpcst->adrSrc_pv;
        }
        // set destination address (skip if it is a NULL_PTR)
        if (adrConfig_cpcst->adrDest_pv != NULL_PTR)
        {
            eDMA_PTR(idxUnit_qu8)->TCD[idxCh_qu8].DADDR = (uint32)adrConfig_cpcst->adrDest_pv;
        }
        // configure inner loop counter in number of bytes, so this number depends on the configured data width
        eDMA_PTR(idxUnit_qu8)->TCD[idxCh_qu8].NBYTES_MLX = (adrConfig_cpcst->numMoves_u32) * (1uL << adrConfig_cpcst->swtSize_u8);

        // check if circular buffer is enabled for source
        if ( (swtOptions2_u32 & RBA_DMA_SETUPCH_ENA_RING_BUFF_SRC_MSK) != 0u )
        {
            // get source & destination buffer size
            uint32 numSrcBuf_u32 = (uint32)adrConfig_cpcst->numSrcBuf_u16;
            uint8  xSmod_u8      = 0;

            // calculate buffer size in bytes, this depends on the configured data width
            // multiply by 1 for uint8 transfer size (swtSize_u8 = 0)
            // multiply by 2 for uint16 transfer size (swtSize_u8 = 1)
            // multiply by 4 for uint32 transfer size (swtSize_u8 = 2)
            numSrcBuf_u32  *= 1uL << adrConfig_cpcst->swtSize_u8;

            // check if source buffer wrap around required
            if ( adrConfig_cpcst->numSrcBuf_u16 > 1u )
            {
                while ((numSrcBuf_u32 & 1u) == 0uL)
                {
                    xSmod_u8++;
                    numSrcBuf_u32 >>= 1u;
                }
                regAttr_u16 |= (uint16)((uint16)xSmod_u8 << RBA_REG_DMA_TCDx_ATTR_SMOD_POS);
                // set SOFF according to configuration (data width / address modification)
                // increment:  1 for 8 bit,  2 for 16 bit and  4 for 32 bit transfer size
                // decrement: -1 for 8 bit, -2 for 16 bit and -4 for 32 bit transfer size
                eDMA_PTR(idxUnit_qu8)->TCD[idxCh_qu8].SOFF = (uint16)regSoffInit_s16 << adrConfig_cpcst->swtSize_u8;
            }
        }
        // configure linaear mode for source
        else
        {
            // check if source address modification is configured
            if (adrConfig_cpcst->swtIncSrc_b != FALSE)
            {
                // increment:  1 for 8 bit,  2 for 16 bit and  4 for 32 bit transfer size
                // decrement: -1 for 8 bit, -2 for 16 bit and -4 for 32 bit transfer size
                eDMA_PTR(idxUnit_qu8)->TCD[idxCh_qu8].SOFF = (uint16)regSoffInit_s16 << adrConfig_cpcst->swtSize_u8;
            }
            else
            {
                // no modification: SOFF = 0
                eDMA_PTR(idxUnit_qu8)->TCD[idxCh_qu8].SOFF = 0;
            }
        }
        // check if circular buffer is enabled for destination
        if ( (swtOptions2_u32 & RBA_DMA_SETUPCH_ENA_RING_BUFF_DST_MSK) != 0u )
        {
            // get source & destination buffer size
            uint32 numDestBuf_u32 = (uint32)adrConfig_cpcst->numDestBuf_u16;
            uint8  xDmod_u8       = 0;

            // calculate buffer size in bytes, this depends on the configured data width
            // multiply by 1 for uint8 transfer size (swtSize_u8 = 0)
            // multiply by 2 for uint16 transfer size (swtSize_u8 = 1)
            // multiply by 4 for uint32 transfer size (swtSize_u8 = 2)
            numDestBuf_u32 *= 1uL << adrConfig_cpcst->swtSize_u8;

            // check for destination buffer wrap around
            if ( adrConfig_cpcst->numDestBuf_u16 > 1 )
            {
                while ((numDestBuf_u32 & 1u) == 0uL)
                {
                    xDmod_u8++;
                    numDestBuf_u32 >>= 1uL;
                }
                regAttr_u16 |= (uint16)((uint16)xDmod_u8 << RBA_REG_DMA_TCDx_ATTR_DMOD_POS);
                // set DOFF according to configuration (data width / address modification)
                // increment:  1 for 8 bit,  2 for 16 bit and  4 for 32 bit transfer size
                // decrement: -1 for 8 bit, -2 for 16 bit and -4 for 32 bit transfer size
                eDMA_PTR(idxUnit_qu8)->TCD[idxCh_qu8].DOFF = (uint16)regDoffInit_s16 << adrConfig_cpcst->swtSize_u8;
            }
        }
        // configure linaear mode for destination
        else
        {
            // check if destination address modification is configured
            if (adrConfig_cpcst->swtIncDest_b != FALSE)
            {
                // increment:  1 for 8 bit,  2 for 16 bit and  4 for 32 bit transfer size
                // decrement: -1 for 8 bit, -2 for 16 bit and -4 for 32 bit transfer size
                eDMA_PTR(idxUnit_qu8)->TCD[idxCh_qu8].DOFF = (uint16)regDoffInit_s16 << adrConfig_cpcst->swtSize_u8;
            }
            else
            {
                 // no modification: DOFF = 0
                 eDMA_PTR(idxUnit_qu8)->TCD[idxCh_qu8].DOFF = 0;
            }
        }
        // enable continuous mode for full circular buffer operation
        if (   ((swtOptions2_u32 & RBA_DMA_SETUPCH_ENA_RING_BUFF_SRC_MSK) != 0u)
            && ((swtOptions2_u32 & RBA_DMA_SETUPCH_ENA_RING_BUFF_DST_MSK) != 0u)
           )
        {
            // nothing to be done here, this is the default behaviour on JDP controllers
        }
        // else in mixed mode or in full linear mode disable HW trigger input after completion of the major (outer) loop
        else
        {
            // disable HW trigger input after completion of the outer loop (default case for IFX DMA controller)
            regCsr_u16 |= (uint16)((uint16)1u << RBA_REG_DMA_TCDx_CSR_DREQ_POS);
        }

        // configure transfer data width
        regAttr_u16 |= (uint16)((uint16)adrConfig_cpcst->swtSize_u8 << RBA_REG_DMA_TCDx_ATTR_DSIZE_POS);
        regAttr_u16 |= (uint16)((uint16)adrConfig_cpcst->swtSize_u8 << RBA_REG_DMA_TCDx_ATTR_SSIZE_POS);
        // init ATTR register
        eDMA_PTR(idxUnit_qu8)->TCD[idxCh_qu8].ATTR = regAttr_u16;
        // configure outer loop counter(s) in BITER and CITER
        regXiter_u16 = (uint16)numTrig_u32;
        // check if a service request is configured
        if (adrConfig_cpcst->swtEnaReq_b != FALSE)
        {
            regCsr_u16 |= (uint16)((uint16)1u << RBA_REG_DMA_TCDx_CSR_INTMAJOR_POS);
        }
        // optionally configure channel linking via SRN
        if ( (adrConfig_cpcst->swtChTrig_u8 & RBA_DMA_SETUPCH_TRG_LINK_OUTER_MSK) != 0u )
        {
            // enable major-loop link in CSR
            regCsr_u16 |= (uint16)((uint16)1u << RBA_REG_DMA_TCDx_CSR_MAJORELINK_POS);
            // enter linked channel number in CSR
            regCsr_u16 |= (uint16)((uint16)idxLinkCh_u8 << RBA_REG_DMA_TCDx_CSR_MAJORLINKCH_POS);
        }
        if ( (adrConfig_cpcst->swtChTrig_u8 & RBA_DMA_SETUPCH_TRG_LINK_INNER_MSK) != 0u )
        {
            // enable minor-loop link
            regXiter_u16 |= (uint16)((uint16)1u << RBA_REG_DMA_TCDx_BITER_ELINKYES_ELINK_POS);
            // enter linked channel number
            regXiter_u16 |= (uint16)((uint16)idxLinkCh_u8 << RBA_REG_DMA_TCDx_CITER_ELINKYES_LINKCH_POS);
            // additionally configure the major-loop link as the JDP DMA controller does not execute the
            // minor-loop link after the last minor loop.
            // enable major-loop link in CSR
            regCsr_u16 |= (uint16)((uint16)1u << RBA_REG_DMA_TCDx_CSR_MAJORELINK_POS);
            // enter linked channel number in CSR
            regCsr_u16 |= (uint16)((uint16)idxLinkCh_u8 << RBA_REG_DMA_TCDx_CSR_MAJORLINKCH_POS);
        }
        // write BITER and CITER contents
        eDMA_PTR(idxUnit_qu8)->TCD[idxCh_qu8].CITER_ELINKX = (uint16)regXiter_u16;
        eDMA_PTR(idxUnit_qu8)->TCD[idxCh_qu8].BITER_ELINKX = (uint16)regXiter_u16;
        // check if HW trigger is configured
        // also enable HW trigger if this channel is the target in a channel-linking setup
        if ( (adrConfig_cpcst->swtChTrig_u8 & ((RBA_DMA_SETUPCH_ENBL_HW_TRG_MSK) | (RBA_DMA_SETUPCH_LINK_TARGET_MSK)))
             != 0u
           )
        {
            // check if HW trigger shall be left enabled after completion of the outer loop
            // always keep HW trigger enabled in circular/ring buffer mode!
            if (  ((adrConfig_cpcst->swtChTrig_u8 & (uint8)RBA_DMA_SETUPCH_KEEP_HWTRIGGER_MSK) != 0u)
               || ((adrConfig_cpcst->swtChTrig_u8 & (uint8)RBA_DMA_SETUPCH_ENA_RING_BUF_MSK)   != 0u)
               )
            {
                // nothing to be done here, this is the default behaviour on JDP controllers
            }
            else
            {
                // automatically disable HW trigger input after completion of the major (outer) loop
                // this is also done in IFX DMA automatically.
                regCsr_u16 |= (uint16)((uint16)1u << RBA_REG_DMA_TCDx_CSR_DREQ_POS);
            }
            // write CSR bitfield, first clear DONE bit (to be able to write bit MAJORLINK, see manual)
            eDMA_PTR(idxUnit_qu8)->TCD[idxCh_qu8].CSR = 0;
            eDMA_PTR(idxUnit_qu8)->TCD[idxCh_qu8].CSR = regCsr_u16;
            // enable HW trigger input (should be last action besides the SW trigger check)
            eDMA_PTR(idxUnit_qu8)->SERQ = (uint8)idxCh_qu8;
        }
        else
        {
            // disable HW trigger for the other two cases (0 or 2), is already done in the beginning
            // write CSR bitfield, first clear DONE bit (to be able to write bit MAJORLINK, see manual)
            eDMA_PTR(idxUnit_qu8)->TCD[idxCh_qu8].CSR = 0;
            eDMA_PTR(idxUnit_qu8)->TCD[idxCh_qu8].CSR = regCsr_u16;
        }
        // finally check for SW trigger
        if ( (adrConfig_cpcst->swtChTrig_u8 & RBA_DMA_SETUPCH_IMDT_TRG_MSK) != 0u )
        {
            eDMA_PTR(idxUnit_qu8)->SSRT = (uint8)idxCh_qu8;
        }
#endif
    }
    return xRet;
}
#define RBA_DMA_STOP_SEC_CODE_NORMAL
#include "rba_Dma_MemMap.h"

/** @} */

/* <BASDKey>
*********************************************************************************************************
* $History__:$
*********************************************************************************************************
</BASDKey>*/
