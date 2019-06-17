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
# JDP Device 1/2/3/4 DMAMUX look-up table
#
# Author: CDG-SMT/ESA1 - Geß
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
    if ( $numChan_s >= 48 )
    {
      $numMux_s    = 5;
      $idxMuxReg_s = $numChan_s % 16;
    }
    elsif ( $numChan_s >= 32 )
    {
      $numMux_s    = 4;
      $idxMuxReg_s = $numChan_s % 16;
    }
    else
    {
      $numMux_s    = int($numChan_s / 8);
      $idxMuxReg_s = $numChan_s % 8;
    }
  }
  if ( $numMoveEngine_s eq "SB1" )
  {
    $numMux_s    = int($numChan_s / 16) + 6;
    $idxMuxReg_s = $numChan_s % 16;
  }

  # do the initialisation
  %xDmaMux_h = ( SB0 => {
                         # DMAMUX0 is present in Dev1, Dev2, Dev3 & Dev4
                         DMAMUX0 => { ADC_SAR_0_EOC  => { Val =>  1 },
                                      ADC_SAR_1_EOC  => { Val =>  2 },
                                      ADC_SAR_B_EOC  => { Val =>  3 },
                                      ADC_SD_0_EOC   => { Val =>  4 },
                                      DSPI_0_RX      => { Val =>  5 },
                                      DSPI_0_TX      => { Val =>  6 },
                                      DSPI_4_RX      => { Val =>  7 },
                                      DSPI_4_TX      => { Val =>  8 },
#                                     not used                    9
                                      ADC_SAR_4_EOC  => { Val => 10 },
                                      ADC_SD_3_EOC   => { Val => 11 },
                                      MCAN_1         => { Val => 12 },
                                      MCAN_2         => { Val => 13 },
                                      SENT_0_RX_FAST => { Val => 14 },
                                      SENT_0_RX_SLOW => { Val => 15 },
                                      LINFlex_0_RX   => { Val => 16 }, # duplicate for backward compatibility
                                      LINFlex_0_TX   => { Val => 17 }, # duplicate for backward compatibility
                                      LINFlex_14_RX  => { Val => 18 }, # duplicate for backward compatibility
                                      LINFlexD_0_RX  => { Val => 16 },
                                      LINFlexD_0_TX  => { Val => 17 },
                                      LINFlexD_14_RX => { Val => 18 },
                                      DSPI_0_CMD     => { Val => 19 },
                                      DSPI_4_CMD     => { Val => 20 },
                                      Always_on      => { Val => 63 },
                                    },
                         # DMAMUX1 is present in Dev1, Dev2, Dev3 & Dev4
                         DMAMUX1 => { DSPI_12_RX       => { Val =>  1 },
                                      DSPI_12_TX       => { Val =>  2 },
                                      LINFlex_0_RX     => { Val =>  3 }, # duplicate for backward compatibility
                                      LINFlex_0_TX     => { Val =>  4 }, # duplicate for backward compatibility
                                      LINFlex_1_RX     => { Val =>  5 }, # duplicate for backward compatibility
                                      LINFlex_1_TX     => { Val =>  6 }, # duplicate for backward compatibility
                                      LINFlex_14_RX    => { Val =>  7 }, # duplicate for backward compatibility
                                      LINFlex_14_TX    => { Val =>  8 }, # duplicate for backward compatibility
                                      LINFlexD_0_RX    => { Val =>  3 },
                                      LINFlexD_0_TX    => { Val =>  4 },
                                      LINFlexD_1_RX    => { Val =>  5 },
                                      LINFlexD_1_TX    => { Val =>  6 },
                                      LINFlexD_14_RX   => { Val =>  7 },
                                      LINFlexD_14_TX   => { Val =>  8 },
                                      SENT_0_RX_FAST   => { Val =>  9 },
                                      SENT_0_RX_SLOW   => { Val => 10 },
                                      SIPI_CH0         => { Val => 11 },
                                      SIPI_CH1         => { Val => 12 },
                                      SIPI_CH2         => { Val => 13 },
                                      SIPI_CH3         => { Val => 14 },
                                      SIUL2_REQ0       => { Val => 15 },
                                      SIUL2_REQ1       => { Val => 16 },
                                      GTM_TIM0_IRQ0    => { Val => 17 },
                                      GTM_TIM0_IRQ1    => { Val => 18 },
                                      GTM_TIM0_IRQ2    => { Val => 19 },
                                      GTM_TIM0_IRQ3    => { Val => 20 },
                                      GTM_TIM0_IRQ4    => { Val => 21 },
                                      GTM_TIM0_IRQ5    => { Val => 22 },
                                      GTM_TIM0_IRQ6    => { Val => 23 },
                                      GTM_TIM0_IRQ7    => { Val => 24 },
                                      GTM_TOM0_IRQ0    => { Val => 25 },
                                      GTM_TOM0_IRQ1    => { Val => 26 },
                                      GTM_TOM0_IRQ2    => { Val => 27 },
                                      GTM_TOM0_IRQ3    => { Val => 28 },
                                      GTM_TOM0_IRQ4    => { Val => 29 },
                                      GTM_TOM0_IRQ5    => { Val => 30 },
                                      GTM_TOM0_IRQ6    => { Val => 31 },
                                      GTM_TOM0_IRQ7    => { Val => 32 },
                                      GTM_ATOM0_IRQ0   => { Val => 33 },
                                      GTM_ATOM0_IRQ1   => { Val => 34 },
                                      GTM_ATOM0_IRQ2   => { Val => 35 },
                                      GTM_ATOM0_IRQ3   => { Val => 36 },
                                      GTM_MCS0_IRQ0    => { Val => 37 },
                                      GTM_MCS0_IRQ1    => { Val => 38 },
                                      GTM_MCS0_IRQ2    => { Val => 39 },
                                      GTM_MCS0_IRQ3    => { Val => 40 },
                                      GTM_MCS0_IRQ4    => { Val => 41 },
                                      GTM_MCS0_IRQ5    => { Val => 42 },
                                      GTM_MCS0_IRQ6    => { Val => 43 },
                                      GTM_MCS0_IRQ7    => { Val => 44 },
                                      LINFlex_15_RX    => { Val => 45 }, # duplicate for backward compatibility
                                      LINFlex_15_TX    => { Val => 46 }, # duplicate for backward compatibility
                                      LINFlexD_15_RX   => { Val => 45 },
                                      LINFlexD_15_TX   => { Val => 46 },
                                      DSPI_5_RX        => { Val => 47 },
                                      DSPI_5_TX        => { Val => 48 },
                                      DSPI_5_CMD       => { Val => 49 },
                                      DSPI_12_CMD      => { Val => 50 },
                                      DECFIL_FILL_BUF  => { Val => 51 }, # Dev1 specific
                                      DECFIL_DRAIN_BUF => { Val => 52 }, # Dev1 specific
                                      DSPI_0_RX        => { Val => 53 },
                                      DSPI_0_TX        => { Val => 54 },
                                      ADC_SAR_0_EOC    => { Val => 55 },
                                      ADC_SAR_4_EOC    => { Val => 56 },
                                      ADC_SD_3_EOC     => { Val => 57 },
#                                     not used                     58
                                      Always_on        => { Val => 63 },
                                    },
                         # DMAMUX2 is present in Dev2, Dev3 & Dev4
                         DMAMUX2 => { ADC_SAR_2_EOC      => { Val =>  1 },
                                      ADC_SD_1_EOC       => { Val =>  2 },
                                      DSPI_1_RX          => { Val =>  3 },
                                      DSPI_1_TX          => { Val =>  4 },
                                      SENT_RX_1_FAST     => { Val =>  5 }, # duplicate for backward compatibility
                                      SENT_RX_1_SLOW     => { Val =>  6 }, # duplicate for backward compatibility
                                      SENT_1_RX_FAST     => { Val =>  5 },
                                      SENT_1_RX_SLOW     => { Val =>  6 },
                                      PSI5_0_CH0_RX_PSI5 => { Val =>  7 },
                                      PSI5_0_CH0_RX_SENT => { Val =>  8 },
                                      SIUL2_REQ2         => { Val =>  9 },
                                      SIUL2_REQ4         => { Val => 10 },
                                      GTM_PSM0_IRQ0      => { Val => 11 },
                                      GTM_PSM0_IRQ1      => { Val => 12 },
                                      GTM_PSM0_IRQ2      => { Val => 13 },
                                      GTM_PSM0_IRQ3      => { Val => 14 },
                                      GTM_TIM1_IRQ0      => { Val => 15 },
                                      GTM_TIM1_IRQ1      => { Val => 16 },
                                      GTM_TIM1_IRQ2      => { Val => 17 },
                                      GTM_TIM1_IRQ3      => { Val => 18 },
                                      GTM_TOM1_IRQ0      => { Val => 19 },
                                      GTM_TOM1_IRQ1      => { Val => 20 },
                                      GTM_TOM1_IRQ2      => { Val => 21 },
                                      GTM_TOM1_IRQ3      => { Val => 22 },
                                      GTM_ATOM1_IRQ0     => { Val => 23 },
                                      GTM_ATOM1_IRQ1     => { Val => 24 },
                                      GTM_MCS1_IRQ0      => { Val => 25 },
                                      GTM_MCS1_IRQ1      => { Val => 26 },
                                      GTM_MCS1_IRQ2      => { Val => 27 },
                                      GTM_MCS1_IRQ3      => { Val => 28 },
                                      GTM_TIM2_IRQ0      => { Val => 29 },
                                      GTM_TIM2_IRQ1      => { Val => 30 },
                                      GTM_TIM2_IRQ2      => { Val => 31 },
                                      GTM_TIM2_IRQ3      => { Val => 32 },
                                      GTM_ATOM2_IRQ0     => { Val => 33 },
                                      GTM_ATOM2_IRQ1     => { Val => 34 },
                                      GTM_MCS2_IRQ0      => { Val => 35 },
                                      GTM_MCS2_IRQ1      => { Val => 36 },
                                      GTM_MCS2_IRQ2      => { Val => 37 },
                                      GTM_MCS2_IRQ3      => { Val => 38 },
                                      GTM_ATOM3_IRQ0     => { Val => 39 },
                                      GTM_ATOM3_IRQ1     => { Val => 40 },
                                      ADC_SD_2_EOC       => { Val => 41 },
                                      DSPI_1_CMD         => { Val => 42 },
                                      DSPI_2_RX          => { Val => 43 },
                                      DSPI_2_TX          => { Val => 44 },
                                      LINFlex_2_RX       => { Val => 45 }, # duplicate for backward compatibility
                                      LINFlex_2_TX       => { Val => 46 }, # duplicate for backward compatibility
                                      LINFlexD_2_RX      => { Val => 45 },
                                      LINFlexD_2_TX      => { Val => 46 },
                                      GTM_SPE0           => { Val => 47 },
                                      GTM_SPE1           => { Val => 48 },
                                      PSI5_S             => { Val => 49 },
                                      PSI5_S_PS          => { Val => 49 }, # renamed in Dev4 Cut2
                                      PSI5_S_RX          => { Val => 50 }, # Dev4 Cut2
                                      Always_on          => { Val => 63 },
                                    },
                         # DMAMUX3 is present in Dev2, Dev3 & Dev4
                         DMAMUX3 => { ADC_SAR_3_EOC      => { Val =>  1 },
                                      DSPI_2_RX          => { Val =>  2 },
                                      DSPI_2_TX          => { Val =>  3 },
                                      LINFlex_2_RX       => { Val =>  4 }, # duplicate for backward compatibility
                                      LINFlex_2_TX       => { Val =>  5 }, # duplicate for backward compatibility
                                      LINFlexD_2_RX      => { Val =>  4 },
                                      LINFlexD_2_TX      => { Val =>  5 },
                                      I2C_0_RX           => { Val =>  6 },
                                      I2C_0_TX           => { Val =>  7 },
                                      PSI5_1_CH0_RX_PSI5 => { Val =>  8 },
                                      PSI5_1_CH0_RX_SENT => { Val =>  9 },
                                      SIUL2_REQ5         => { Val => 10 },
                                      GTM_PSM0_IRQ4      => { Val => 11 },
                                      GTM_PSM0_IRQ5      => { Val => 12 },
                                      GTM_PSM0_IRQ6      => { Val => 13 },
                                      GTM_PSM0_IRQ7      => { Val => 14 },
                                      GTM_TIM1_IRQ4      => { Val => 15 },
                                      GTM_TIM1_IRQ5      => { Val => 16 },
                                      GTM_TIM1_IRQ6      => { Val => 17 },
                                      GTM_TIM1_IRQ7      => { Val => 18 },
                                      GTM_TOM1_IRQ4      => { Val => 19 },
                                      GTM_TOM1_IRQ5      => { Val => 20 },
                                      GTM_TOM1_IRQ6      => { Val => 21 },
                                      GTM_TOM1_IRQ7      => { Val => 22 },
                                      GTM_ATOM1_IRQ2     => { Val => 23 },
                                      GTM_ATOM1_IRQ3     => { Val => 24 },
                                      GTM_MCS1_IRQ4      => { Val => 25 },
                                      GTM_MCS1_IRQ5      => { Val => 26 },
                                      GTM_MCS1_IRQ6      => { Val => 27 },
                                      GTM_MCS1_IRQ7      => { Val => 28 },
                                      GTM_TIM2_IRQ4      => { Val => 29 },
                                      GTM_TIM2_IRQ5      => { Val => 30 },
                                      GTM_TIM2_IRQ6      => { Val => 31 },
                                      GTM_TIM2_IRQ7      => { Val => 32 },
                                      GTM_ATOM2_IRQ2     => { Val => 33 },
                                      GTM_ATOM2_IRQ3     => { Val => 34 },
                                      GTM_MCS2_IRQ4      => { Val => 35 },
                                      GTM_MCS2_IRQ5      => { Val => 36 },
                                      GTM_MCS2_IRQ6      => { Val => 37 },
                                      GTM_MCS2_IRQ7      => { Val => 38 },
                                      GTM_ATOM3_IRQ2     => { Val => 39 },
                                      GTM_ATOM3_IRQ3     => { Val => 40 },
                                      SIUL2_REQ8         => { Val => 41 },
                                      SIUL2_REQ3         => { Val => 42 }, # Dev4 Cut2
                                      ADC_SD_3_EOC       => { Val => 43 },
                                      ADC_SAR_6_EOC      => { Val => 44 },
                                      DSPI_2_CMD         => { Val => 45 },
                                      DSPI_1_RX          => { Val => 46 },
                                      DSPI_1_TX          => { Val => 47 },
                                      ADC_SAR_2_EOC      => { Val => 48 },
                                      ADC_SD_2_EOC       => { Val => 49 },
                                      PSI5_S             => { Val => 50 },
                                      PSI5_S_TX          => { Val => 50 }, # renamed in Dev4 Cut2
                                      Always_on          => { Val => 63 },
                                    },
                         # DMAMUX4 is present in Dev3 & Dev4
                         DMAMUX4 => { ADC_SAR_4_EOC      => { Val =>  1 },
                                      ADC_SAR_6_EOC      => { Val =>  2 },
                                      ADC_SD_2_EOC       => { Val =>  3 },
                                      ADC_SD_3_EOC       => { Val =>  4 },
                                      DSPI_3_RX          => { Val =>  5 },
                                      DSPI_3_TX          => { Val =>  6 },
                                      LINFlex_0_RX       => { Val =>  7 }, # duplicate for backward compatibility
                                      LINFlex_0_TX       => { Val =>  8 }, # duplicate for backward compatibility
                                      LINFlex_14_RX      => { Val =>  9 }, # duplicate for backward compatibility
                                      LINFlex_14_TX      => { Val => 10 }, # duplicate for backward compatibility
                                      LINFlexD_0_RX      => { Val =>  7 },
                                      LINFlexD_0_TX      => { Val =>  8 },
                                      LINFlexD_14_RX     => { Val =>  9 },
                                      LINFlexD_14_TX     => { Val => 10 },
                                      PSI5_0_CH1_RX_PSI5 => { Val => 11 },
                                      PSI5_0_CH1_RX_SENT => { Val => 12 },
                                      SIPI_CH0           => { Val => 13 },
                                      SIPI_CH1           => { Val => 14 },
                                      SIUL2_REQ9         => { Val => 15 },
                                      GTM_TIM0_IRQ0      => { Val => 16 },
                                      GTM_TIM0_IRQ1      => { Val => 17 },
                                      GTM_TOM0_IRQ0      => { Val => 18 },
                                      GTM_TOM0_IRQ1      => { Val => 19 },
                                      GTM_PSM0_IRQ0      => { Val => 20 },
                                      GTM_PSM0_IRQ1      => { Val => 21 },
                                      GTM_PSM0_IRQ2      => { Val => 22 },
                                      GTM_PSM0_IRQ3      => { Val => 23 },
                                      GTM_TOM1_IRQ0      => { Val => 24 },
                                      GTM_TOM1_IRQ1      => { Val => 25 },
                                      GTM_TIM3_IRQ0      => { Val => 26 },
                                      GTM_TIM3_IRQ1      => { Val => 27 },
                                      GTM_TIM3_IRQ2      => { Val => 28 },
                                      GTM_TIM3_IRQ3      => { Val => 29 },
                                      GTM_MCS3_IRQ0      => { Val => 30 },
                                      GTM_MCS3_IRQ1      => { Val => 31 },
                                      GTM_MCS3_IRQ2      => { Val => 32 },
                                      GTM_MCS3_IRQ3      => { Val => 33 },
                                      DSPI_3_CMD         => { Val => 34 },
                                      ADC_SD_1_EOC       => { Val => 35 },
                                      ADC_SD_4_EOC       => { Val => 36 },
                                      ADC_SD_5_EOC       => { Val => 37 },
                                      ADC_SAR_0_EOC      => { Val => 38 },
                                      DSPI_0_CMD         => { Val => 39 },
                                      DSPI_0_RX          => { Val => 40 },
                                      DSPI_0_TX          => { Val => 41 },
                                      AMU0               => { Val => 42 }, # Dev4 Cut2
                                      Always_on          => { Val => 63 },
                                    },
                         # DMAMUX5 is present in Dev3 & Dev4
                         DMAMUX5 => { ADC_SAR_7_EOC  => { Val =>  1 },
                                      ADC_SD_4_EOC   => { Val =>  2 },
                                      ADC_SD_5_EOC   => { Val =>  3 },
                                      DSPI_5_RX      => { Val =>  4 },
                                      DSPI_5_TX      => { Val =>  5 },
                                      LINFlex_1_RX   => { Val =>  6 }, # duplicate for backward compatibility
                                      LINFlex_1_TX   => { Val =>  7 }, # duplicate for backward compatibility
                                      LINFlex_15_RX  => { Val =>  8 }, # duplicate for backward compatibility
                                      LINFlex_15_TX  => { Val =>  9 }, # duplicate for backward compatibility
                                      LINFlexD_1_RX  => { Val =>  6 },
                                      LINFlexD_1_TX  => { Val =>  7 },
                                      LINFlexD_15_RX => { Val =>  8 },
                                      LINFlexD_15_TX => { Val =>  9 },
                                      SENT_0_RX_FAST => { Val => 10 },
                                      SENT_0_RX_SLOW => { Val => 11 },
                                      SIPI_CH2       => { Val => 12 },
                                      SIPI_CH3       => { Val => 13 },
                                      SIUL_REQ10     => { Val => 14 },
                                      GTM_TIM0_IRQ2  => { Val => 15 },
                                      GTM_TIM0_IRQ3  => { Val => 16 },
                                      GTM_TOM0_IRQ4  => { Val => 17 },
                                      GTM_TOM0_IRQ5  => { Val => 18 },
                                      GTM_PSM0_IRQ4  => { Val => 19 },
                                      GTM_PSM0_IRQ5  => { Val => 20 },
                                      GTM_PSM0_IRQ6  => { Val => 21 },
                                      GTM_PSM0_IRQ7  => { Val => 22 },
                                      GTM_TOM1_IRQ4  => { Val => 23 },
                                      GTM_TOM1_IRQ5  => { Val => 24 },
                                      GTM_TIM3_IRQ4  => { Val => 25 },
                                      GTM_TIM3_IRQ5  => { Val => 26 },
                                      GTM_TIM3_IRQ6  => { Val => 27 },
                                      GTM_TIM3_IRQ7  => { Val => 28 },
                                      GTM_MCS3_IRQ4  => { Val => 29 },
                                      GTM_MCS3_IRQ5  => { Val => 30 },
                                      GTM_MCS3_IRQ6  => { Val => 31 },
                                      GTM_MCS3_IRQ7  => { Val => 32 },
                                      DSPI_5_CMD     => { Val => 33 },
                                      MCAN_1         => { Val => 34 },
                                      MCAN_2         => { Val => 35 },
                                      DSPI_3_RX      => { Val => 36 },
                                      DSPI_3_TX      => { Val => 37 },
                                      ADC_SAR_3_EOC  => { Val => 38 },
                                      LINFlex_2_RX   => { Val => 39 }, # duplicate for backward compatibility
                                      LINFlex_2_TX   => { Val => 40 }, # duplicate for backward compatibility
                                      LINFlexD_2_RX  => { Val => 39 },
                                      LINFlexD_2_TX  => { Val => 40 },
                                      ADC_SAR_1_EOC  => { Val => 41 },
                                      AMU1           => { Val => 42 }, # Dev4 Cut2
                                      Always_on      => { Val => 63 },
                                    },
                        },
                 SB1 => {
                         # DMAMUX6 is present in Dev4 only
                         DMAMUX6 => { ADC_SAR_8_EOC      => { Val =>  1 },
                                      ADC_SAR_9_EOC      => { Val =>  2 },
                                      ADC_SAR_10_EOC     => { Val =>  3 },
                                      ADC_SAR_11_EOC     => { Val =>  4 }, # Dev4 Cut 1 devices
                                      ADC_SAR_5_EOC      => { Val =>  4 }, # Dev4 Cut 2 devices
                                      ADC_SD_6_EOC       => { Val =>  5 },
                                      ADC_SD_7_EOC       => { Val =>  6 },
                                      ADC_SD_8_EOC       => { Val =>  7 },
                                      ADC_SD_9_EOC       => { Val =>  8 },
                                      PSI_0_CH2_RX_PSI5  => { Val =>  9 }, # duplicate for backward compatibility
                                      PSI_0_CH2_RX_SENT  => { Val => 10 }, # duplicate for backward compatibility
                                      PSI_1_CH1_RX_PSI5  => { Val => 11 }, # duplicate for backward compatibility
                                      PSI_1_CH1_RX_SENT  => { Val => 12 }, # duplicate for backward compatibility
                                      PSI5_0_CH2_RX_PSI5 => { Val =>  9 },
                                      PSI5_0_CH2_RX_SENT => { Val => 10 },
                                      PSI5_1_CH1_RX_PSI5 => { Val => 11 },
                                      PSI5_1_CH1_RX_SENT => { Val => 12 },
                                      DSPI_6_RX          => { Val => 13 },
                                      DSPI_6_TX          => { Val => 14 },
                                      DSPI_6_CMD         => { Val => 15 },
                                      LINFlex_16_RX      => { Val => 16 }, # duplicate for backward compatibility
                                      LINFlex_16_TX      => { Val => 17 }, # duplicate for backward compatibility
                                      LINFlexD_16_RX     => { Val => 16 },
                                      LINFlexD_16_TX     => { Val => 17 },
                                      I2C_1_RX           => { Val => 18 },
                                      I2C_1_TX           => { Val => 19 },
                                      GTM_PSM1_IRQ0      => { Val => 20 },
                                      GTM_PSM1_IRQ1      => { Val => 21 },
                                      GTM_PSM1_IRQ2      => { Val => 22 },
                                      GTM_PSM1_IRQ3      => { Val => 23 },
                                      DSPI_1_CMD         => { Val => 24 },
                                      DSPI_1_RX          => { Val => 25 },
                                      DSPI_1_TX          => { Val => 26 },
                                      DSPI_12_CMD        => { Val => 27 },
                                      DSPI_12_RX         => { Val => 28 },
                                      DSPI_12_TX         => { Val => 29 },
                                      SENT_1_RX_FAST     => { Val => 30 },
                                      SENT_1_RX_SLOW     => { Val => 31 },
                                      GTM_TOM1_IRQ0      => { Val => 32 },
                                      GTM_TOM1_IRQ1      => { Val => 33 },
                                      GTM_TOM1_IRQ2      => { Val => 34 },
                                      GTM_TOM1_IRQ3      => { Val => 35 },
                                      GTM_TOM1_IRQ4      => { Val => 36 },
                                      GTM_TOM1_IRQ5      => { Val => 37 },
                                      GTM_TOM1_IRQ6      => { Val => 38 },
                                      GTM_TOM1_IRQ7      => { Val => 39 },
                                      GTM_TOM2_IRQ0      => { Val => 40 },
                                      GTM_TOM2_IRQ1      => { Val => 41 },
                                      GTM_TOM2_IRQ2      => { Val => 42 },
                                      GTM_TOM2_IRQ3      => { Val => 43 },
                                      GTM_TOM2_IRQ4      => { Val => 44 },
                                      GTM_TOM2_IRQ5      => { Val => 45 },
                                      GTM_TOM2_IRQ6      => { Val => 46 },
                                      GTM_TOM2_IRQ7      => { Val => 47 },
                                      Always_on          => { Val => 63 },
                                    },
                         # DMAMUX7 is present in Dev4 only
                         DMAMUX7 => { GTM_MCS4_IRQ0      => { Val =>  1 },
                                      GTM_MCS4_IRQ1      => { Val =>  2 },
                                      GTM_MCS4_IRQ2      => { Val =>  3 },
                                      GTM_MCS4_IRQ3      => { Val =>  4 },
                                      GTM_MCS4_IRQ4      => { Val =>  5 },
                                      GTM_MCS4_IRQ5      => { Val =>  6 },
                                      GTM_MCS4_IRQ6      => { Val =>  7 },
                                      GTM_MCS4_IRQ7      => { Val =>  8 },
                                      GTM_MCS5_IRQ0      => { Val =>  9 },
                                      GTM_MCS5_IRQ1      => { Val => 10 },
                                      GTM_MCS5_IRQ2      => { Val => 11 },
                                      GTM_MCS5_IRQ3      => { Val => 12 },
                                      GTM_MCS5_IRQ4      => { Val => 13 },
                                      GTM_MCS5_IRQ5      => { Val => 14 },
                                      GTM_MCS5_IRQ6      => { Val => 15 },
                                      GTM_MCS5_IRQ7      => { Val => 16 },
                                      GTM_ATOM5_IRQ0     => { Val => 17 },
                                      GTM_ATOM5_IRQ1     => { Val => 18 },
                                      GTM_ATOM5_IRQ2     => { Val => 19 },
                                      GTM_ATOM5_IRQ3     => { Val => 20 },
                                      GTM_ATOM6_IRQ0     => { Val => 21 },
                                      GTM_ATOM6_IRQ1     => { Val => 22 },
                                      GTM_ATOM6_IRQ2     => { Val => 23 },
                                      GTM_ATOM6_IRQ3     => { Val => 24 },
                                      GTM_TOM3_IRQ0      => { Val => 25 },
                                      GTM_TOM3_IRQ1      => { Val => 26 },
                                      GTM_TOM3_IRQ2      => { Val => 27 },
                                      GTM_TOM3_IRQ3      => { Val => 28 },
                                      GTM_TOM3_IRQ4      => { Val => 29 },
                                      GTM_TOM3_IRQ5      => { Val => 30 },
                                      GTM_TOM3_IRQ6      => { Val => 31 },
                                      GTM_TOM3_IRQ7      => { Val => 32 },
                                      ADC_SD_6_EOC       => { Val => 33 },
                                      ADC_SD_7_EOC       => { Val => 34 },
                                      PSI5_0_CH0_RX_PSI5 => { Val => 35 },
                                      PSI5_0_CH0_RX_SENT => { Val => 36 },
                                      PSI5_0_CH1_RX_PSI5 => { Val => 37 },
                                      PSI5_0_CH1_RX_SENT => { Val => 38 },
                                      PSI5_0_CH2_RX_PSI5 => { Val => 39 },
                                      PSI5_0_CH2_RX_SENT => { Val => 40 },
                                      ADC_SD_0_EOC       => { Val => 41 },
                                      ADC_SD_1_EOC       => { Val => 42 },
                                      ADC_SD_2_EOC       => { Val => 43 },
                                      ADC_SD_3_EOC       => { Val => 44 },
                                      DSPI_3_CMD         => { Val => 45 },
                                      DSPI_3_RX          => { Val => 46 },
                                      DSPI_3_TX          => { Val => 47 },
                                      DSPI_5_CMD         => { Val => 48 },
                                      DSPI_5_RX          => { Val => 49 },
                                      DSPI_5_TX          => { Val => 50 },
                                      Always_on          => { Val => 63 },
                                    },
                         # DMAMUX8 is present in Dev4 only
                         DMAMUX8 => { GTM_TIM4_IRQ0      => { Val =>  1 },
                                      GTM_TIM4_IRQ1      => { Val =>  2 },
                                      GTM_TIM4_IRQ2      => { Val =>  3 },
                                      GTM_TIM4_IRQ3      => { Val =>  4 },
                                      GTM_TIM4_IRQ4      => { Val =>  5 },
                                      GTM_TIM4_IRQ5      => { Val =>  6 },
                                      GTM_TIM4_IRQ6      => { Val =>  7 },
                                      GTM_TIM4_IRQ7      => { Val =>  8 },
                                      GTM_TIM5_IRQ0      => { Val =>  9 },
                                      GTM_TIM5_IRQ1      => { Val => 10 },
                                      GTM_TIM5_IRQ2      => { Val => 11 },
                                      GTM_TIM5_IRQ3      => { Val => 12 },
                                      GTM_TIM5_IRQ4      => { Val => 13 },
                                      GTM_TIM5_IRQ5      => { Val => 14 },
                                      GTM_TIM5_IRQ6      => { Val => 15 },
                                      GTM_TIM5_IRQ7      => { Val => 16 },
                                      GTM_ATOM7_IRQ0     => { Val => 17 },
                                      GTM_ATOM7_IRQ1     => { Val => 18 },
                                      GTM_ATOM7_IRQ2     => { Val => 19 },
                                      GTM_ATOM7_IRQ3     => { Val => 20 },
                                      GTM_ATOM8_IRQ0     => { Val => 21 },
                                      GTM_ATOM8_IRQ1     => { Val => 22 },
                                      GTM_ATOM8_IRQ2     => { Val => 23 },
                                      GTM_ATOM8_IRQ3     => { Val => 24 },
                                      GTM_TOM4_IRQ0      => { Val => 25 },
                                      GTM_TOM4_IRQ1      => { Val => 26 },
                                      GTM_TOM4_IRQ2      => { Val => 27 },
                                      GTM_TOM4_IRQ3      => { Val => 28 },
                                      GTM_TOM4_IRQ4      => { Val => 29 },
                                      GTM_TOM4_IRQ5      => { Val => 30 },
                                      GTM_TOM4_IRQ6      => { Val => 31 },
                                      GTM_TOM4_IRQ7      => { Val => 32 },
                                      ADC_SD_8_EOC       => { Val => 33 },
                                      PSI5_1_CH0_RX_PSI5 => { Val => 34 },
                                      PSI5_1_CH0_RX_SENT => { Val => 35 },
                                      PSI5_1_CH1_RX_PSI5 => { Val => 36 },
                                      PSI5_1_CH1_RX_SENT => { Val => 37 },
                                      ADC_SAR_0_EOC      => { Val => 38 },
                                      ADC_SAR_1_EOC      => { Val => 39 },
                                      ADC_SAR_2_EOC      => { Val => 40 },
                                      ADC_SAR_B_EOC      => { Val => 41 },
                                      ADC_SD_4_EOC       => { Val => 42 },
                                      DSPI_4_CMD         => { Val => 43 },
                                      DSPI_4_RX          => { Val => 44 },
                                      DSPI_4_TX          => { Val => 55 },
                                      Always_on          => { Val => 63 },
                                    },
                         # DMAMUX9 is present in Dev4 only
                         DMAMUX9 => { GTM_PSM1_IRQ0  => { Val =>  1 },
                                      GTM_PSM1_IRQ1  => { Val =>  2 },
                                      GTM_PSM1_IRQ2  => { Val =>  3 },
                                      GTM_PSM1_IRQ3  => { Val =>  4 },
                                      GTM_PSM1_IRQ4  => { Val =>  5 },
                                      GTM_PSM1_IRQ5  => { Val =>  6 },
                                      GTM_PSM1_IRQ6  => { Val =>  7 },
                                      GTM_PSM1_IRQ7  => { Val =>  8 },
                                      GTM_SPE2       => { Val =>  9 },
                                      GTM_SPE3       => { Val => 10 },
                                      I2C_0_RX       => { Val => 11 },
                                      I2C_0_TX       => { Val => 12 },
                                      GTM_ATOM5_IRQ0 => { Val => 13 },
                                      GTM_ATOM5_IRQ1 => { Val => 14 },
                                      GTM_ATOM7_IRQ0 => { Val => 15 },
                                      GTM_ATOM7_IRQ1 => { Val => 16 },
                                      GTM_TOM3_IRQ0  => { Val => 17 },
                                      GTM_TOM3_IRQ1  => { Val => 18 },
                                      GTM_TOM3_IRQ2  => { Val => 19 },
                                      GTM_TOM3_IRQ3  => { Val => 20 },
                                      GTM_TOM3_IRQ4  => { Val => 21 },
                                      GTM_TOM3_IRQ5  => { Val => 22 },
                                      GTM_TOM3_IRQ6  => { Val => 23 },
                                      GTM_TOM3_IRQ7  => { Val => 24 },
                                      GTM_TOM4_IRQ0  => { Val => 25 },
                                      GTM_TOM4_IRQ1  => { Val => 26 },
                                      GTM_TOM4_IRQ2  => { Val => 27 },
                                      GTM_TOM4_IRQ3  => { Val => 28 },
                                      GTM_TOM4_IRQ4  => { Val => 29 },
                                      GTM_TOM4_IRQ5  => { Val => 30 },
                                      GTM_TOM4_IRQ6  => { Val => 31 },
                                      GTM_TOM4_IRQ7  => { Val => 32 },
                                      ADC_SD_9_EOC   => { Val => 33 },
                                      MCAN_1         => { Val => 34 },
                                      MCAN_2         => { Val => 35 },
                                      ADC_SAR_3_EOC  => { Val => 36 },
                                      ADC_SAR_4_EOC  => { Val => 37 },
                                      ADC_SAR_6_EOC  => { Val => 38 },
                                      ADC_SAR_7_EOC  => { Val => 39 },
                                      ADC_SD_5_EOC   => { Val => 40 },
                                      DSPI_2_CMD     => { Val => 41 },
                                      DSPI_2_RX      => { Val => 42 },
                                      DSPI_2_TX      => { Val => 43 },
                                      SIUL2_REQ11    => { Val => 44 },
                                      Always_on      => { Val => 63 },
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
