#
####################################################################################################
#
# (C) All rights reserved by ROBERT BOSCH GMBH, STUTTGART
#
####################################################################################################
#
#    __   __   ___  __
#   /_ / /  / /__  /    /__/
#  /__/ /__/ __ / /__  /  /
#
#  C O R E
#
####################################################################################################

####################################################################################################
# ST Device 3 Cut3 DMAMUX look-up table
#
# Author: CDG-SMT/ESS2 - Geß
####################################################################################################

package rba_Dma_ActMachineType;

use strict;
use warnings;

####################################################################################################
# InitOutputMux()
#
# Initialise output hash
#
# \retval  DMAMUX register contents based on given connection
####################################################################################################
sub GetTriggerRegs ()
{
  my $numMoveEngine_s = $_[0];
  my $numChan_s       = $_[1];
  my $xTrgSrc_s       = $_[2];
  my $ptrCpuType_ph   = $_[3];
  my %xDmaMux_h;
  my $numMux_s;
  my $idxMuxReg_s;
  my $locVal_s        = 0;

  # determine MUX depending on channel number
  # additionally calculate MUX register index
  $numChan_s       =~ s/\D//g;        # remove letters
  if ( $numMoveEngine_s eq "SB0" )
  {
    $numMux_s    = int($numChan_s / 16) + 4;
    $idxMuxReg_s = $numChan_s % 16;
  }
  if ( $numMoveEngine_s eq "SB1" )
  {
    $numMux_s    = int($numChan_s / 16);
    $idxMuxReg_s = $numChan_s % 16;
  }

  # also accept LINFlex_, replace by LINFlexD_
  $xTrgSrc_s =~ s/LINFlex_(.+)/LINFlexD_$1/;

  # do the initialisation
  %xDmaMux_h = ( SB1 => {
                         # DMAMUX0 is connected to DMA1, channels 0 .. 15
                         DMAMUX0 => { ADC_SAR_B_EOC       => { Val =>  1 }, # renamed SUPERVISOR to B for compatibility
                                      ADC_SAR_0_EOC       => { Val =>  2 },
                                      DSPI_0_RX           => { Val =>  3 },
                                      DSPI_0_TX           => { Val =>  4 },
                                      DSPI_0_CMD          => { Val =>  5 },
                                      DSPI_1_RX           => { Val =>  6 },
                                      DSPI_1_TX           => { Val =>  7 },
                                      DSPI_1_CMD          => { Val =>  8 },
                                      DSPI_4_RX           => { Val =>  9 },
                                      DSPI_4_TX           => { Val => 10 },
                                      DSPI_4_CMD          => { Val => 11 },
                                      DSPI_5_RX           => { Val => 12 },
                                      DSPI_5_TX           => { Val => 13 },
                                      DSPI_5_CMD          => { Val => 14 },
                                      LINFlexD_0_RX       => { Val => 15 },
                                      LINFlexD_0_TX       => { Val => 16 },
                                      LINFlexD_1_RX       => { Val => 17 },
                                      LINFlexD_1_TX       => { Val => 18 },
                                      LINFlexD_2_RX       => { Val => 19 },
                                      LINFlexD_2_TX       => { Val => 20 },
                                      LINFlexD_3_RX       => { Val => 21 },
                                      LINFlexD_3_TX       => { Val => 22 },
                                      LINFlexD_4_RX       => { Val => 23 },
                                      LINFlexD_4_TX       => { Val => 24 },
                                      ADC_SAR_B_Q0_IN     => { Val => 25 },
                                      ADC_SAR_B_Q0_OUT    => { Val => 26 },
                                      I2C_0_RX            => { Val => 27 },
                                      I2C_0_TX            => { Val => 28 },
                                      CAN_SUB_0_M_CAN_1   => { Val => 29 }, # ST original
                                      MCAN_1              => { Val => 29 }, # for JDP compatibility
                                      SIUL2_REQ0          => { Val => 30 },
                                      SIUL2_REQ1          => { Val => 31 },
                                      ADC_SAR_B_Q1_IN     => { Val => 32 },
                                      ADC_SAR_B_Q1_OUT    => { Val => 33 },
                                      ADC_SAR_0_Q0_IN     => { Val => 32 },
                                      ADC_SAR_0_Q0_OUT    => { Val => 33 },
                                      ADC_SAR_0_Q1_IN     => { Val => 36 },
                                      ADC_SAR_0_Q1_OUT    => { Val => 37 },
#                                     not used                        38
                                      GTM_MCS0_IRQ0       => { Val => 39 },
                                      GTM_MCS0_IRQ1       => { Val => 40 },
                                      GTM_MCS0_IRQ2       => { Val => 41 },
                                      GTM_MCS0_IRQ3       => { Val => 42 },
                                      GTM_MCS0_IRQ4       => { Val => 43 },
                                      GTM_MCS0_IRQ5       => { Val => 44 },
                                      GTM_MCS0_IRQ6       => { Val => 45 },
                                      GTM_MCS0_IRQ7       => { Val => 46 },
                                      GTM_ATOM0_IRQ0      => { Val => 47 },
                                      GTM_ATOM0_IRQ1      => { Val => 48 },
                                      GTM_ATOM0_IRQ2      => { Val => 49 },
                                      GTM_ATOM0_IRQ3      => { Val => 50 },
                                      GTM_TIM2_IRQ0       => { Val => 51 },
                                      GTM_TIM2_IRQ1       => { Val => 52 },
                                      GTM_TIM2_IRQ2       => { Val => 53 },
                                      GTM_TIM2_IRQ3       => { Val => 54 },
                                      GTM_TIM2_IRQ4       => { Val => 55 },
                                      GTM_TIM2_IRQ5       => { Val => 56 },
                                      GTM_TIM2_IRQ6       => { Val => 57 },
                                      GTM_TIM2_IRQ7       => { Val => 58 },
                                      GTM_TOM2_IRQ0       => { Val => 59 },
                                      GTM_TOM2_IRQ1       => { Val => 60 },
                                      GTM_TOM2_IRQ2       => { Val => 61 },
#                                     Always_on                       62
                                      Always_on           => { Val => 63 },
                                    },
                         # DMAMUX1 is connected to DMA1, channels 16 .. 31
                         DMAMUX1 => { ADC_SAR_1_EOC       => { Val =>  1 },
                                      ADC_SD_0_EOC        => { Val =>  2 },
                                      ADC_SD_3_EOC        => { Val =>  3 },
                                      DSPI_2_RX           => { Val =>  4 },
                                      DSPI_2_TX           => { Val =>  5 },
                                      DSPI_2_CMD          => { Val =>  6 },
                                      DSPI_3_RX           => { Val =>  7 },
                                      DSPI_3_TX           => { Val =>  8 },
                                      DSPI_3_CMD          => { Val =>  9 },
                                      DSPI_6_RX           => { Val => 10 },
                                      DSPI_6_TX           => { Val => 11 },
                                      DSPI_6_CMD          => { Val => 12 },
                                      ADC_SAR_1_Q0_IN     => { Val => 13 },
                                      ADC_SAR_1_Q0_OUT    => { Val => 14 },
                                      ADC_SAR_1_Q1_IN     => { Val => 15 },
                                      ADC_SAR_1_Q1_OUT    => { Val => 16 },
                                      ADC_SAR_0_Q0_IN     => { Val => 17 },
                                      ADC_SAR_0_Q0_OUT    => { Val => 18 },
                                      ADC_SAR_0_Q1_IN     => { Val => 19 },
                                      ADC_SAR_1_Q1_OUT    => { Val => 20 },
#                                     not used                        21
#                                     not used                        22
#                                     not used                        23
#                                     not used                        24
#                                     not used                        25
#                                     not used                        26
#                                     not used                        27
                                      CAN_SUB_0_M_CAN_2   => { Val => 28 }, # ST original
                                      MCAN_2              => { Val => 28 }, # for JDP compatibility
                                      SIUL2_REQ2          => { Val => 29 },
                                      SIUL2_REQ4          => { Val => 30 },
#                                     not used                        31
#                                     not used                        32
#                                     not used                        33
#                                     not used                        34
#                                     not used                        35
#                                     not used                        36
                                      GTM_SPE0            => { Val => 37 },
                                      GTM_SPE1            => { Val => 38 },
                                      GTM_MCS1_IRQ0       => { Val => 39 },
                                      GTM_MCS1_IRQ1       => { Val => 40 },
                                      GTM_MCS1_IRQ2       => { Val => 41 },
                                      GTM_MCS1_IRQ3       => { Val => 42 },
                                      GTM_MCS1_IRQ4       => { Val => 43 },
                                      GTM_MCS1_IRQ5       => { Val => 44 },
                                      GTM_MCS1_IRQ6       => { Val => 45 },
                                      GTM_MCS1_IRQ7       => { Val => 46 },
                                      GTM_ATOM1_IRQ0      => { Val => 47 },
                                      GTM_ATOM1_IRQ1      => { Val => 48 },
                                      GTM_ATOM1_IRQ2      => { Val => 49 },
                                      GTM_ATOM1_IRQ3      => { Val => 50 },
                                      GTM_TIM3_IRQ0       => { Val => 51 },
                                      GTM_TIM3_IRQ1       => { Val => 52 },
                                      GTM_TIM3_IRQ2       => { Val => 53 },
                                      GTM_TIM3_IRQ3       => { Val => 54 },
                                      GTM_TIM3_IRQ4       => { Val => 55 },
                                      GTM_TIM3_IRQ5       => { Val => 56 },
                                      GTM_TIM3_IRQ6       => { Val => 57 },
                                      GTM_TIM3_IRQ7       => { Val => 58 },
                                      GTM_TOM2_IRQ3       => { Val => 59 },
                                      GTM_TOM2_IRQ4       => { Val => 60 },
                                      GTM_TOM2_IRQ5       => { Val => 61 },
#                                     Always_on                       62
                                      Always_on           => { Val => 63 },
                                    },
                         # DMAMUX2 is connected to DMA1, channels 32 .. 47
                         DMAMUX2 => { ADC_SAR_2_EOC       => { Val =>  1 },
                                      ADC_SD_1_EOC        => { Val =>  2 },
                                      ADC_SD_4_EOC        => { Val =>  3 },
                                      ADC_SAR_2_Q0_IN     => { Val =>  4 },
                                      ADC_SAR_2_Q0_OUT    => { Val =>  5 },
                                      ADC_SAR_2_Q1_IN     => { Val =>  6 },
                                      ADC_SAR_2_Q1_OUT    => { Val =>  7 },
#                                     not used                         8 },
                                      LINFlexD_14_RX      => { Val =>  9 },
                                      LINFlexD_14_TX      => { Val => 10 },
                                      SIUL2_REQ5          => { Val => 11 },
                                      ADC_SAR10_0_EOC     => { Val => 12 },
                                      DSPI_4_RX           => { Val => 13 },
                                      DSPI_4_TX           => { Val => 14 },
                                      DSPI_4_CMD          => { Val => 15 },
                                      GTM_MCS2_IRQ0       => { Val => 16 },
                                      GTM_MCS2_IRQ1       => { Val => 17 },
                                      GTM_MCS2_IRQ2       => { Val => 18 },
                                      GTM_MCS2_IRQ3       => { Val => 19 },
                                      GTM_MCS2_IRQ4       => { Val => 20 },
                                      GTM_MCS2_IRQ5       => { Val => 21 },
                                      GTM_MCS2_IRQ6       => { Val => 22 },
                                      GTM_MCS2_IRQ7       => { Val => 23 },
                                      GTM_TIM0_IRQ0       => { Val => 24 },
                                      GTM_TIM0_IRQ1       => { Val => 25 },
                                      GTM_TIM0_IRQ2       => { Val => 26 },
                                      GTM_TIM0_IRQ3       => { Val => 27 },
                                      GTM_TIM0_IRQ4       => { Val => 28 },
                                      GTM_TIM0_IRQ5       => { Val => 29 },
                                      GTM_TIM0_IRQ6       => { Val => 30 },
                                      GTM_TIM0_IRQ7       => { Val => 31 },
                                      GTM_TOM0_IRQ0       => { Val => 32 },
                                      GTM_TOM0_IRQ1       => { Val => 33 },
                                      GTM_TOM0_IRQ2       => { Val => 34 },
                                      GTM_TOM0_IRQ3       => { Val => 35 },
                                      GTM_TOM0_IRQ4       => { Val => 36 },
                                      GTM_TOM0_IRQ5       => { Val => 37 },
                                      GTM_TOM0_IRQ6       => { Val => 38 },
                                      GTM_TOM0_IRQ7       => { Val => 39 },
                                      GTM_PSM0_IRQ0       => { Val => 40 },
                                      GTM_PSM0_IRQ1       => { Val => 41 },
                                      GTM_PSM0_IRQ2       => { Val => 42 },
                                      GTM_PSM0_IRQ3       => { Val => 43 },
                                      GTM_ATOM2_IRQ0      => { Val => 44 },
                                      GTM_ATOM2_IRQ1      => { Val => 45 },
                                      GTM_ATOM2_IRQ2      => { Val => 46 },
                                      GTM_ATOM2_IRQ3      => { Val => 47 },
                                      GTM_TIM4_IRQ0       => { Val => 48 },
                                      GTM_TIM4_IRQ1       => { Val => 49 },
                                      GTM_TIM4_IRQ2       => { Val => 50 },
                                      GTM_TIM4_IRQ3       => { Val => 51 },
                                      GTM_TOM3_IRQ0       => { Val => 52 },
                                      GTM_TOM3_IRQ1       => { Val => 53 },
                                      GTM_TOM3_IRQ2       => { Val => 54 },
                                      GTM_TOM3_IRQ3       => { Val => 55 },
                                      GTM_TOM3_IRQ4       => { Val => 56 },
                                      GTM_TOM3_IRQ5       => { Val => 57 },
                                      GTM_TOM3_IRQ6       => { Val => 58 },
                                      GTM_TOM3_IRQ7       => { Val => 59 },
                                      GTM_TOM2_IRQ6       => { Val => 60 },
                                      GTM_TOM2_IRQ7       => { Val => 61 },
#                                     Always_on                       62
                                      Always_on           => { Val => 63 },
                                    },
                         # DMAMUX3 is connected to DMA1, channels 48 .. 63
                         DMAMUX3 => { ADC_SAR_3_EOC       => { Val =>  1 },
                                      ADC_SD_2_EOC        => { Val =>  2 },
                                      ADC_SD_5_EOC        => { Val =>  3 },
                                      DSPI_9_RX           => { Val =>  4 },
                                      DSPI_9_TX           => { Val =>  5 },
                                      DSPI_9_CMD          => { Val =>  6 },
                                      ADC_SAR_1_Q0_IN     => { Val =>  7 },
                                      ADC_SAR_1_Q0_OUT    => { Val =>  8 },
                                      LINFlexD_15_RX      => { Val =>  9 },
                                      LINFlexD_15_TX      => { Val => 10 },
                                      SIUL2_REQ8          => { Val => 11 },
                                      ADC_SAR10_1_EOC     => { Val => 12 },
                                      DSPI_5_RX           => { Val => 13 },
                                      DSPI_5_TX           => { Val => 14 },
                                      DSPI_5_CMD          => { Val => 15 },
                                      GTM_MCS3_IRQ0       => { Val => 16 },
                                      GTM_MCS3_IRQ1       => { Val => 17 },
                                      GTM_MCS3_IRQ2       => { Val => 18 },
                                      GTM_MCS3_IRQ3       => { Val => 19 },
                                      GTM_MCS3_IRQ4       => { Val => 20 },
                                      GTM_MCS3_IRQ5       => { Val => 21 },
                                      GTM_MCS3_IRQ6       => { Val => 22 },
                                      GTM_MCS3_IRQ7       => { Val => 23 },
                                      GTM_TIM1_IRQ0       => { Val => 24 },
                                      GTM_TIM1_IRQ1       => { Val => 25 },
                                      GTM_TIM1_IRQ2       => { Val => 26 },
                                      GTM_TIM1_IRQ3       => { Val => 27 },
                                      GTM_TIM1_IRQ4       => { Val => 28 },
                                      GTM_TIM1_IRQ5       => { Val => 29 },
                                      GTM_TIM1_IRQ6       => { Val => 30 },
                                      GTM_TIM1_IRQ7       => { Val => 31 },
                                      GTM_TOM1_IRQ0       => { Val => 32 },
                                      GTM_TOM1_IRQ1       => { Val => 33 },
                                      GTM_TOM1_IRQ2       => { Val => 34 },
                                      GTM_TOM1_IRQ3       => { Val => 35 },
                                      GTM_TOM1_IRQ4       => { Val => 36 },
                                      GTM_TOM1_IRQ5       => { Val => 37 },
                                      GTM_TOM1_IRQ6       => { Val => 38 },
                                      GTM_TOM1_IRQ7       => { Val => 39 },
                                      GTM_PSM0_IRQ4       => { Val => 40 },
                                      GTM_PSM0_IRQ5       => { Val => 41 },
                                      GTM_PSM0_IRQ6       => { Val => 42 },
                                      GTM_PSM0_IRQ7       => { Val => 43 },
                                      GTM_ATOM3_IRQ0      => { Val => 44 },
                                      GTM_ATOM3_IRQ1      => { Val => 45 },
                                      GTM_ATOM3_IRQ2      => { Val => 46 },
                                      GTM_ATOM3_IRQ3      => { Val => 47 },
                                      GTM_TIM4_IRQ4       => { Val => 48 },
                                      GTM_TIM4_IRQ5       => { Val => 49 },
                                      GTM_TIM4_IRQ6       => { Val => 50 },
                                      GTM_TIM4_IRQ7       => { Val => 51 },
                                      GTM_ATOM4_IRQ0      => { Val => 52 },
                                      GTM_ATOM4_IRQ1      => { Val => 53 },
                                      GTM_ATOM4_IRQ2      => { Val => 54 },
                                      GTM_ATOM4_IRQ3      => { Val => 55 },
                                      ADC_SAR_1_Q1_IN     => { Val => 56 },
                                      ADC_SAR_1_Q1_OUT    => { Val => 57 },
                                      ADC_SAR_3_Q0_IN     => { Val => 58 },
                                      ADC_SAR_3_Q0_OUT    => { Val => 59 },
                                      ADC_SAR_3_Q1_IN     => { Val => 60 },
                                      ADC_SAR_3_Q1_OUT    => { Val => 61 },
#                                     Always_on                       62
                                      Always_on           => { Val => 63 },
                                    },
                        },
                 SB0 => {
                         # DMAMUX4 is connected to DMA0, channels 0 .. 15
                         DMAMUX4 => { AMU0                => { Val =>  1 },
                                      SIPI_CH0            => { Val =>  2 },
                                      SIPI_CH2            => { Val =>  3 },
                                      DEC_FILTER_0_IN     => { Val =>  4 },
                                      DEC_FILTER_0_OUT    => { Val =>  5 },
                                      DEC_FILTER_1_IN     => { Val =>  6 },
                                      DEC_FILTER_1_OUT    => { Val =>  7 },
                                      ADC_SAR_B_Q0_IN     => { Val =>  8 },
                                      ADC_SAR_B_Q0_OUT    => { Val =>  9 },
                                      GTM_TIM0_IRQ0       => { Val => 10 },
                                      GTM_TIM0_IRQ1       => { Val => 11 },
                                      GTM_TIM0_IRQ2       => { Val => 12 },
                                      SENT_0_RX_FAST      => { Val => 13 },
                                      SENT_0_RX_SLOW      => { Val => 14 },
                                      PSI5_0_CH0_RX_PSI5  => { Val => 15 },
                                      PSI5_0_CH0_RX_SMC   => { Val => 16 },
                                      PSI5_0_CH0_RX_SENT  => { Val => 16 }, # compatibuility
                                      PSI5_1_CH0_RX_PSI5  => { Val => 17 },
                                      PSI5_1_CH0_RX_SMC   => { Val => 18 },
                                      PSI5_1_CH0_RX_SENT  => { Val => 18 }, # compatibility
                                      SIUL2_REQ9          => { Val => 19 },
                                      LINFlexD_0_RX       => { Val => 20 },
                                      LINFlexD_0_TX       => { Val => 21 },
                                      LINFlexD_1_RX       => { Val => 22 },
                                      LINFlexD_1_TX       => { Val => 23 },
                                      GTM_MCS4_IRQ0       => { Val => 24 },
                                      GTM_MCS4_IRQ1       => { Val => 25 },
                                      GTM_MCS4_IRQ2       => { Val => 26 },
                                      GTM_MCS4_IRQ3       => { Val => 27 },
                                      GTM_TIM2_IRQ0       => { Val => 28 },
                                      GTM_TIM2_IRQ1       => { Val => 29 },
                                      GTM_TIM2_IRQ2       => { Val => 30 },
                                      GTM_TIM2_IRQ3       => { Val => 31 },
                                      GTM_TIM2_IRQ4       => { Val => 32 },
                                      GTM_TIM2_IRQ5       => { Val => 33 },
                                      GTM_TIM2_IRQ6       => { Val => 34 },
                                      GTM_TIM2_IRQ7       => { Val => 35 },
                                      GTM_TOM2_IRQ0       => { Val => 36 },
                                      GTM_TOM2_IRQ1       => { Val => 37 },
                                      GTM_TOM2_IRQ2       => { Val => 38 },
                                      GTM_TOM2_IRQ3       => { Val => 39 },
                                      GTM_TOM2_IRQ4       => { Val => 40 },
                                      GTM_TOM2_IRQ5       => { Val => 41 },
                                      GTM_TOM2_IRQ6       => { Val => 42 },
                                      GTM_TOM2_IRQ7       => { Val => 43 },
                                      GTM_TIM4_IRQ0       => { Val => 44 },
                                      GTM_TIM4_IRQ1       => { Val => 45 },
                                      GTM_TIM4_IRQ2       => { Val => 46 },
                                      GTM_TIM4_IRQ3       => { Val => 47 },
                                      GTM_ATOM4_IRQ0      => { Val => 48 },
                                      GTM_ATOM4_IRQ1      => { Val => 49 },
                                      GTM_PSM0_IRQ0       => { Val => 50 },
                                      GTM_PSM0_IRQ1       => { Val => 51 },
                                      GTM_PSM0_IRQ2       => { Val => 52 },
                                      GTM_PSM0_IRQ3       => { Val => 53 },
                                      ADC_SAR10_0_EOC     => { Val => 54 },
                                      GTM_ATOM5_IRQ0      => { Val => 55 },
                                      GTM_ATOM5_IRQ1      => { Val => 56 },
                                      SIUL2_REQ3          => { Val => 57 },
                                      SIUL2_REQ7          => { Val => 58 },
                                      SIUL2_REQ13         => { Val => 59 },
                                      SIUL2_REQ15         => { Val => 60 },
                                      GTM_TIM0_IRQ6       => { Val => 61 },
#                                     Always_on                       62
                                      Always_on           => { Val => 63 },
                                    },
                         # DMAMUX5 is connected to DMA0, channels 16 .. 32
                         DMAMUX5 => { AMU1                => { Val =>  1 },
                                      SIPI_CH1            => { Val =>  2 },
                                      SIPI_CH3            => { Val =>  3 },
                                      DEC_FILTER_2_IN     => { Val =>  4 },
                                      DEC_FILTER_2_OUT    => { Val =>  5 },
                                      DEC_FILTER_3_IN     => { Val =>  6 },
                                      DEC_FILTER_3_OUT    => { Val =>  7 },
                                      ADC_SAR_B_Q1_IN     => { Val =>  8 },
                                      ADC_SAR_B_Q1_OUT    => { Val =>  9 },
                                      GTM_TIM0_IRQ3       => { Val => 10 },
                                      GTM_TIM0_IRQ4       => { Val => 11 },
                                      GTM_TIM0_IRQ5       => { Val => 12 },
                                      SENT_1_RX_FAST      => { Val => 13 },
                                      SENT_1_RX_SLOW      => { Val => 14 },
                                      PSI5_S_RX           => { Val => 15 },
                                      PSI5_S_TX           => { Val => 16 },
                                      PSI5_S_PS           => { Val => 17 },
                                      SIUL2_REQ10         => { Val => 18 },
                                      SIUL2_REQ11         => { Val => 19 },
                                      LINFlexD_2_RX       => { Val => 20 },
                                      LINFlexD_2_TX       => { Val => 21 },
                                      ADC_SAR_B_EOC       => { Val => 22 }, # renamed SUPERVISOR to B for compatibility
#                                     not used                        23
                                      GTM_MCS4_IRQ4       => { Val => 24 },
                                      GTM_MCS4_IRQ5       => { Val => 25 },
                                      GTM_MCS4_IRQ6       => { Val => 26 },
                                      GTM_MCS4_IRQ7       => { Val => 27 },
                                      GTM_TIM3_IRQ0       => { Val => 28 },
                                      GTM_TIM3_IRQ1       => { Val => 29 },
                                      GTM_TIM3_IRQ2       => { Val => 30 },
                                      GTM_TIM3_IRQ3       => { Val => 31 },
                                      GTM_TIM3_IRQ4       => { Val => 32 },
                                      GTM_TIM3_IRQ5       => { Val => 33 },
                                      GTM_TIM3_IRQ6       => { Val => 34 },
                                      GTM_TIM3_IRQ7       => { Val => 35 },
                                      GTM_TIM5_IRQ0       => { Val => 36 },
                                      GTM_TIM5_IRQ1       => { Val => 37 },
                                      GTM_TIM5_IRQ2       => { Val => 38 },
                                      GTM_TIM5_IRQ3       => { Val => 39 },
                                      GTM_TIM5_IRQ4       => { Val => 40 },
                                      GTM_TIM5_IRQ5       => { Val => 41 },
                                      GTM_TIM5_IRQ6       => { Val => 42 },
                                      GTM_TIM5_IRQ7       => { Val => 43 },
                                      GTM_TIM4_IRQ4       => { Val => 44 },
                                      GTM_TIM4_IRQ5       => { Val => 45 },
                                      GTM_TIM4_IRQ6       => { Val => 46 },
                                      GTM_TIM4_IRQ7       => { Val => 47 },
                                      GTM_ATOM4_IRQ2      => { Val => 48 },
                                      GTM_ATOM4_IRQ3      => { Val => 49 },
                                      GTM_PSM0_IRQ4       => { Val => 50 },
                                      GTM_PSM0_IRQ5       => { Val => 51 },
                                      GTM_PSM0_IRQ6       => { Val => 52 },
                                      GTM_PSM0_IRQ7       => { Val => 53 },
                                      ADC_SAR10_1_EOC     => { Val => 54 },
                                      GTM_ATOM5_IRQ2      => { Val => 55 },
                                      GTM_ATOM5_IRQ3      => { Val => 56 },
                                      SIUL2_REQ6          => { Val => 57 },
                                      SIUL2_REQ12         => { Val => 58 },
                                      SIUL2_REQ14         => { Val => 59 },
#                                     not used                        60
                                      GTM_TIM0_IRQ7       => { Val => 61 },
#                                     Always_on                       62
                                      Always_on           => { Val => 63 },
                                    },
                          },
                );
  # now check if trigger connection can be establiched
  if ( exists ($xDmaMux_h{$numMoveEngine_s}{"DMAMUX$numMux_s"}{$xTrgSrc_s}) )
  {
    $locVal_s = $xDmaMux_h{$numMoveEngine_s}{"DMAMUX$numMux_s"}{$xTrgSrc_s}{Val};
  }
  else
  {
    # if no connection can be established 'Val' is zero which indicates an error
  }
  return ('MuxIdx' => $numMux_s, 'RegIdx' => $idxMuxReg_s, 'Val' => $locVal_s);
}

# anonymous function, don't delete
{
  1;
}
