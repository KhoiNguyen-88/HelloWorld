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
#if ( RBA_DMA_DEV_ERROR_DETECT == STD_ON )
#include "Det.h"
#endif
#if (RBA_DMA_ERROR_ACTION == RBA_DMA_ERROR_ACTION_RESET)
#include "rba_Reset.h"
#endif

#if ( MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1 )
// create mask for all error bits of ERRSR register
#define IFX_ERROR_BIT_MSK    ( (1uL << RBA_REG_DMA_ERRSR0_DLLER_POS)    \
                             | (1uL << RBA_REG_DMA_ERRSR0_SLLER_POS)    \
                             | (1uL << RBA_REG_DMA_ERRSR0_RAMER_POS)    \
                             | (1uL << RBA_REG_DMA_ERRSR0_SRIER_POS)    \
                             | (1uL << RBA_REG_DMA_ERRSR0_SPBER_POS)    \
                             | (1uL << RBA_REG_DMA_ERRSR0_DER_POS)      \
                             | (1uL << RBA_REG_DMA_ERRSR0_SER_POS)      \
                             )
// create mask for LEC bitfield
#define ERRSR_LEC_MSK                rba_Reg_Mask(DMA,DMA_ERRSR0,ERRSR0_LEC)
#define ERRSR_DLLER_SLLER_RAMER_MSK  ( (1uL << RBA_REG_DMA_ERRSR0_DLLER_POS) \
                                     | (1uL << RBA_REG_DMA_ERRSR0_SLLER_POS) \
                                     | (1uL << RBA_REG_DMA_ERRSR0_RAMER_POS) \
                                     )
#define ERRSR_SRIER_SPBER_MSK        ( (1uL << RBA_REG_DMA_ERRSR0_SRIER_POS) \
                                     | (1uL << RBA_REG_DMA_ERRSR0_SPBER_POS) \
                                     )
#define ERRSR_DER_SER_MSK            ( (1uL << RBA_REG_DMA_ERRSR0_DER_POS) \
                                     | (1uL << RBA_REG_DMA_ERRSR0_SER_POS) \
                                     )

#endif

#if ( MCU_RB_MACHINE_FAM == MCU_RB_JDP_UC1 )

// create mask for all error bits of ES register
// mask to detect a source error
#define ES_SBE_SOE_SAE_MSK   ( (1uL << RBA_REG_DMA_ES_SBE_POS)    \
                             | (1uL << RBA_REG_DMA_ES_SOE_POS)    \
                             | (1uL << RBA_REG_DMA_ES_SAE_POS)    \
                             )
// mask to detect a destination error
#define ES_DBE_DOE_DAE_MSK   ( (1uL << RBA_REG_DMA_ES_DBE_POS)    \
                             | (1uL << RBA_REG_DMA_ES_DOE_POS)    \
                             | (1uL << RBA_REG_DMA_ES_DAE_POS)    \
                             )
// mask for the neighbouring SGE and NCE bits
#define ES_SGE_NCE_MSK       ( (1uL << RBA_REG_DMA_ES_SGE_POS)    \
                             | (1uL << RBA_REG_DMA_ES_NCE_POS)    \
                             )
// mask for the neighbouring CPE, GPE, ECX and UCE bits
#define ES_CPE_GPE_ECX_UCE_MSK ( (1uL << RBA_REG_DMA_ES_CPE_POS)    \
                               | (1uL << RBA_REG_DMA_ES_GPE_POS)    \
                               | (1uL << RBA_REG_DMA_ES_ECX_POS)    \
                               | (1uL << RBA_REG_DMA_ES_UCE_POS)    \
                               )
// create mask for ERRCHN bitfield
#define ES_ERRCHN_MSK  rba_Reg_Mask(DMA,DMA_ES,ES_ERRCHN)
#endif


#if (RBA_DMA_ERROR_ACTION != RBA_DMA_ERROR_ACTION_NONE)

#define RBA_DMA_START_SEC_VAR_FAST_CLEARED_32
#include "rba_Dma_MemMap.h"
static uint32 rba_Dma_cntrErrIsr_u32;
#define RBA_DMA_STOP_SEC_VAR_FAST_CLEARED_32
#include "rba_Dma_MemMap.h"

#define RBA_DMA_START_SEC_CODE_FAST
#include "rba_Dma_MemMap.h"

void rba_Dma_Error_Isr (void)
{
    /********************************************/
    /*                IFX part                  */
    /********************************************/
#if ( MCU_RB_MACHINE_FAM == MCU_RB_IFX_UC1 )
    // no support for IFX Dev3 A
    uint32 regDmaErr0_u32;
    uint32 regDmaErr1_u32;
    regDmaErr0_u32 = DMA.ERRSR0;
    regDmaErr1_u32 = DMA.ERRSR1;
    // increment cinvocation counter
    rba_Dma_cntrErrIsr_u32++;
#if ( (RBA_DMA_ERROR_ACTION == RBA_DMA_ERROR_ACTION_DET) && (RBA_DMA_DEV_ERROR_DETECT == STD_ON) )
    if ((regDmaErr0_u32 & (IFX_ERROR_BIT_MSK)) != 0uL)
    {
        // extract channel number and report to Det
        (void)Det_ReportError (RBA_DMA_MODULE_ID,
                               RBA_DMA_INSTANCE_ID,
                               RBA_DMA_PRV_APIID_ERR_ISR,
                               (uint8)(regDmaErr0_u32 & ERRSR_LEC_MSK));
        // clear error bits
        DMA.CLRE0 = regDmaErr0_u32 & (IFX_ERROR_BIT_MSK);
    }
    if ((regDmaErr1_u32 & (IFX_ERROR_BIT_MSK)) != 0uL)
    {
        // extract channel number and report to Det
        (void)Det_ReportError (RBA_DMA_MODULE_ID,
                               RBA_DMA_INSTANCE_ID,
                               RBA_DMA_PRV_APIID_ERR_ISR,
                               (uint8)(regDmaErr1_u32 & ERRSR_LEC_MSK)
                              );
        // clear error bits
        DMA.CLRE1 = regDmaErr1_u32 & (IFX_ERROR_BIT_MSK);
    }
#endif

#if (RBA_DMA_ERROR_ACTION == RBA_DMA_ERROR_ACTION_RESET)
    // check if there is an error reported
    if (  ((regDmaErr0_u32 & IFX_ERROR_BIT_MSK) != 0)
       || ((regDmaErr1_u32 & IFX_ERROR_BIT_MSK) != 0)
       )
    {
        uint32 dataUserDefined_u32;
        // layout of dataUserDefined_u32 which is passed to rba_Reset_Request()
        // it contains all bitfields of the two ERRSE0/1 registers
        //
        //  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16|15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
        // ------------------------------------------------|-----------------------------------------------
        //   0  D  S  R  S  S  D  S  0|------ LEC1 --------| 0  D  S  R  S  S  D  S  0 |------ LEC0 -------|
        //      L  L  A  R  P  E  E                        |    L  L  A  R  P  E  E
        //      L  L  M  I  B  R  R                        |    L  L  M  I  B  R  R
        //      E  E  E  E  E  1  1                        |    E  E  E  E  E  0  0
        //      R  R  R  R  R                              |    R  R  R  R  R
        //      1  1  1  1  1                              |    0  0  0  0  0

        // shift contents of regDmaErr1_u32 into upper 16 bit
        dataUserDefined_u32  = (regDmaErr1_u32 & ERRSR_LEC_MSK) << 16;
        dataUserDefined_u32 |= (regDmaErr1_u32 & ERRSR_DER_SER_MSK) <<  8;
        dataUserDefined_u32 |= (regDmaErr1_u32 & ERRSR_SRIER_SPBER_MSK) <<  6;
        dataUserDefined_u32 |= (regDmaErr1_u32 & ERRSR_DLLER_SLLER_RAMER_MSK) <<  4;
        // shift contents of regDmaErr0_u32 into lower 16 bit
        dataUserDefined_u32 |= (regDmaErr0_u32 & ERRSR_LEC_MSK);
        dataUserDefined_u32 |= (regDmaErr0_u32 & ERRSR_DER_SER_MSK) >>  8;
        dataUserDefined_u32 |= (regDmaErr0_u32 & ERRSR_SRIER_SPBER_MSK) >> 10;
        dataUserDefined_u32 |= (regDmaErr0_u32 & ERRSR_DLLER_SLLER_RAMER_MSK) >> 12;
        // finally trigger reset
        rba_Reset_Request(rba_Reset_Dma, rba_Reset_DmaErr, dataUserDefined_u32);
    }
#endif
#endif
    /********************************************/
    /*                JDP part                  */
    /********************************************/
#if ( MCU_RB_MACHINE_FAM == MCU_RB_JDP_UC1 )
    uint32 regDmaEs0_u32;
#if (RBA_DMA_NUM_DMA_MODULES > 1)
    uint32 regDmaEs1_u32;
#endif
    // increment invocation counter
    rba_Dma_cntrErrIsr_u32++;
    // read ES register(s)
    regDmaEs0_u32 = eDMA_0.ES;
#if (RBA_DMA_NUM_DMA_MODULES > 1)
    regDmaEs1_u32 = eDMA_1.ES;
#endif
    // note: there is a chance for another/additional DMA error entry here between reading the ES register
    //       and clearing the error bit inside the ERRx register. In this case the second error is kept inside ES
    //       register but as all ERRs bits will be cleared it will not lead to an additional error interrupt!
    // fixing this is not possible without losing one of the two events as the ES register has no history.

    // acknowlege IRQ
    // do not rely on the data in ES register(s) because multiple errors are not displayed there
    // keep in mind: this will clear the valid bit inside the ES register!
    if (eDMA_0.ERRL != 0u)
    {
        eDMA_0.ERRL = eDMA_0.ERRL;
    }
    // in case of ST Dev3 Cut3 move engine 0 only has 32 channels
    // do not use the 'H' registers in this case!
#if ( (RBA_DMA_NUM_DMA_CHANS > 32) && (MCU_RB_MACHINE_DEV != MCU_RB_ST_UC1_DEV3_40NM_ALL)  )
    if (eDMA_0.ERRH != 0u)
    {
        eDMA_0.ERRH = eDMA_0.ERRH;
    }
#endif
#if (RBA_DMA_NUM_DMA_MODULES > 1)
    if (eDMA_1.ERRL != 0u)
    {
        eDMA_1.ERRL = eDMA_1.ERRL;
    }
    if (eDMA_1.ERRH != 0u)
    {
        eDMA_1.ERRH = eDMA_1.ERRH;
    }
#endif
#if ( (RBA_DMA_ERROR_ACTION == RBA_DMA_ERROR_ACTION_DET) && (RBA_DMA_DEV_ERROR_DETECT == STD_ON) )
    // check eDMA0.ES.VLD for a valid error entry
    if ( (regDmaEs0_u32 & (1uL << RBA_REG_DMA_ES_VLD_POS)) != 0u )
    {
        // extract channel number and report to Det
        (void)Det_ReportError(RBA_DMA_MODULE_ID,
                              RBA_DMA_INSTANCE_ID,
                              RBA_DMA_PRV_APIID_ERR_ISR,
                              (uint8)((regDmaEs0_u32 & ES_ERRCHN_MSK) >> RBA_REG_DMA_ES_ERRCHN_POS)
                             );
    }
#if (RBA_DMA_NUM_DMA_MODULES > 1)
    // check eDMA1.ES.VLD for a valid error entry
    if ( (regDmaEs1_u32 & (1uL << RBA_REG_DMA_ES_VLD_POS)) != 0u )
    {
        // extract channel number and report to Det, set leftmost bit in user info to insicate DMA move engine 1
        (void)Det_ReportError(RBA_DMA_MODULE_ID,
                              RBA_DMA_INSTANCE_ID,
                              RBA_DMA_PRV_APIID_ERR_ISR,
                              (uint8)(128u + ((regDmaEs1_u32 & ES_ERRCHN_MSK) >> RBA_REG_DMA_ES_ERRCHN_POS))
                             );
    }
#endif
#endif
#if (RBA_DMA_ERROR_ACTION == RBA_DMA_ERROR_ACTION_RESET)
    // check if there is an error reported
    if (  ((regDmaEs0_u32 & (1uL << RBA_REG_DMA_ES_VLD_POS)) != 0u)
#if (RBA_DMA_NUM_DMA_MODULES > 1)
       || ((regDmaEs1_u32 & (1uL << RBA_REG_DMA_ES_VLD_POS)) != 0u)
#endif
       )
    {
        uint32 dataUserDefined_u32 = 0;
        // layout of dataUserDefined_u32 which is passed to rba_Reset_Request()
        // it contains all bitfields of the two ERRSE0/1 registers
        //
        //  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16|15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
        // ------------------------------------------------|------------------------------------------------
        //   U  E  G  C  N  S  D  S  0  0 |---- ERRCHN1 ---| U  E  G  C  N  S  D  S  0  0 |---- ERRCHN0 ---|
        //   C  C  P  P  C  G  E  E                        | C  C  P  P  C  G  E  E
        //   E  X  E  E  E  E  R  R                        | E  X  E  E  E  E  R  R
        //   1  1  1  1  1  1  R  R                        | 0  0  0  0  0  0  R  R
        //                     1  1                        |                   0  0
        //
        // with SERRx = SAEx | SOEx | SBEx
        //  and DERRx = DAEx | DOEx | DBEx

        // check if any source or destination error bit is set
        // if yes, set the corresponding bit in dataUserDefined_u32
        if ( (regDmaEs0_u32 & ES_SBE_SOE_SAE_MSK) != 0u )
        {
            dataUserDefined_u32 |= 1uL <<  8u;
        }
        if ( (regDmaEs0_u32 & ES_DBE_DOE_DAE_MSK) != 0u )
        {
            dataUserDefined_u32 |= 1uL <<  9u;
        }
#if (RBA_DMA_NUM_DMA_MODULES > 1)
        if ( (regDmaEs1_u32 & ES_SBE_SOE_SAE_MSK) != 0u )
        {
            dataUserDefined_u32 |= 1uL << 24u;
        }
        if ( (regDmaEs1_u32 & ES_DBE_DOE_DAE_MSK) != 0u )
        {
            dataUserDefined_u32 |= 1uL << 25u;
        }
        // shift remaining contents of regDmaErr1_u32 into upper 16 bit
        dataUserDefined_u32 |= (regDmaEs1_u32 & ES_SGE_NCE_MSK)         << 24u;    // shift NCE,SGE         to pos 26
        dataUserDefined_u32 |= (regDmaEs1_u32 & ES_ERRCHN_MSK)          <<  8u;    // shift ERRCHN          to pos 16
        dataUserDefined_u32 |= (regDmaEs1_u32 & ES_CPE_GPE_ECX_UCE_MSK) << 14u;    // shift UCE,ECX,GPE,CPE to pos 28
#endif
        // shift remaining contents of regDmaErr0_u32 into lower 16 bit
        dataUserDefined_u32 |= (regDmaEs0_u32 & ES_SGE_NCE_MSK)         <<  8u;    // shift NCE,SGE         to pos 26
        dataUserDefined_u32 |= (regDmaEs0_u32 & ES_ERRCHN_MSK)          >>  8u;    // shift ERRCHN          to pos 10
        dataUserDefined_u32 |= (regDmaEs0_u32 & ES_CPE_GPE_ECX_UCE_MSK) >>  2u;    // shift UCE,ECX,GPE,CPE to pos 12
        // finally trigger reset
        rba_Reset_Request(rba_Reset_Dma, rba_Reset_DmaErr, dataUserDefined_u32);
    }
#endif

#endif

}

#define RBA_DMA_STOP_SEC_CODE_FAST
#include "rba_Dma_MemMap.h"

#endif


/** @} */

/* <BASDKey>
*********************************************************************************************************
* $History__:$
*********************************************************************************************************
</BASDKey>*/
