#! /usr/bin/perl -w
#
####################################################################################################
# COPYRIGHT RESERVED, 2010 Robert Bosch GmbH. All rights reserved.
# The reproduction, distribution and utilization of this document as well as the communication
# of its contents to others without explicit authorization is prohibited. Offenders will be held
# liable for the payment of damages. All rights reserved in the event of the grant of a patent,
# utility model or design.
####################################################################################################
#
####################################################################################################
# Script used to generate configuration files for CORE HWE DMA module:
# - rba_Dma_PBcfg.h
# - rba_Dma_PBcfg.c
# - rba_Dma_Report.txt
#
#
# The script checks if the configuration is valid and if resources are used multiple times.
#
#
# Author: CDG-SMT/ESA1 - Geß
####################################################################################################
package rba_Dma_EcucProc;
require Exporter;

use conf_process;

use strict;
use warnings;
use Data::Dumper;

####################################################################################################
# WhoAmI
####################################################################################################
sub WhoAmI
{
  return "rba_Dma";
}

####################################################################################################
# Handle generation process of specified array of files.
#
# Function is used to handle generation process of files which have to be generated for this module.
#
# \param   none
# \return  void
# \retval  none
####################################################################################################
sub Generate
{
  my $path_Sources_s = $_[1][0];
  my $path_Headers_s = $_[1][1];
  my $path_Scripts_s = $_[2];
  my $path_Reports_s = $_[1][5];

  my %dataInp_h;          # data input / main hash
  my @xPbVar_a;           # list of all Pb variants
  my $dataDma_ph;         # main input data pointer
  my %rebuiltDma_h;
  my %xDmaLut_h;          # global Pb look-up table
  my %xDmaOut_h;          # global configuration output hash
  my %xCpuType_ch;        # machine typeinformation

  my $xContentHt_s    = "";
  my $xContentPrvHt_s = "";
  my $xContentCt_s    = "";
  my $xReport_s       = "\n";

  conf_process::Log("Begin: rba_Dma Generate phase\n");
  
  # set Mip and multiple configuration container to be used for current build action
  conf_process::SetMip("rba_Dma");
  conf_process::SetAr40MultipleConfigurationContainer("rba_Dma_Cfg");
  # Call this API at the beginning of each build action which has input data
  conf_process::AnalyzeInputData("rba_Dma_Cfg");

  # AR42, re-build the original contents returned from conf_process::Pop("rba_Dma");
  {
    my $invariantData_ph = conf_process::GetInvariantInputData("rba_Dma");
    # add invariant data
    %rebuiltDma_h = %{$invariantData_ph};
    # create list of all Pb variants: @xPbVars
    foreach my $PbVariant_s (conf_process::GetVariants())
    {
      # get config data for this variant and place it in %rebuiltDma_h
      my $variantData_ph = conf_process::GetInputData("rba_Dma", $PbVariant_s);
      # in AR42 the 'rba_Dma_Cfg__KEY' is not filled with the current Pb variant, do this manually
      $variantData_ph->{rba_Dma_Cfg}{rba_Dma_Cfg__KEY}{content} = $PbVariant_s;
      # add post-build specicif data to %rebuiltDma_h
      push @{$rebuiltDma_h{rba_Dma_Cfg}}, $variantData_ph->{rba_Dma_Cfg};
    }
    $dataDma_ph = \%rebuiltDma_h;
  }
  
  # For test purposes write the generated hash as text to a file
  conf_process::CreateDumpFile("$path_Reports_s/rba_Dma_Dump.txt", WhoAmI());

  # get machine type information
  %xCpuType_ch = GetCpuType();
  
  # require extra loop-up files for JDP/ST controllers
  if ( $xCpuType_ch{Device} eq "MCU_RB_ST_UC1_DEV3_40NM_ALL" )
  {
    require "$path_Scripts_s/rba_Dma_StDev3_40nm.pm";
  }
  elsif ( $xCpuType_ch{Family} eq "MCU_RB_JDP_UC1" )
  {
    require "$path_Scripts_s/rba_Dma_Jdp.pm";
  }
  else
  {
      # no extra file required for IFX controllers
  }

  # init num check hash to check machine type
  {
    my %locDma_ch;
    %locDma_ch = InitNumCheckHash(\%xCpuType_ch);
    # exit if machine type is not known/supported
    if (keys %locDma_ch == 0)
    {
      conf_process::Exit("Error in DMA configuration:\n" .
                         "The configured machine type " . $xCpuType_ch{Type} . " is unknown!\n" .
                         "Pointless to continue, check integrity of Mcal package."
                        );
    }
  }

  # read configuration data, build up main hash
  %dataInp_h = CreateMainHash($dataDma_ph, \%xDmaLut_h, \%xCpuType_ch);
  @xPbVar_a  = sort {$a cmp $b} keys (%{$dataInp_h{Pb}});
  # dump main hash into report file
  conf_process::GenerateFile($path_Reports_s."rba_Dma_MainHash.txt",
                             "\n%dataInp_h:\n" . Dumper( \%dataInp_h ));

  # CheckForErrors in \%dataInp_h, exit if there are any
  {
    my @Error_a = ();
    my $Error_s = "";
    my $i = 0;
    foreach my $xPbVar_s (keys %{$dataInp_h{Pb}})
    {
      foreach my $xChan_s (keys %{$dataInp_h{Pb}{$xPbVar_s}{Ch}})
      {
        my $hashref_s = $dataInp_h{Pb}{$xPbVar_s}{Ch}{$xChan_s};
        if ( $hashref_s->{Status} ne 'clean' )
        {
          # exteract all error messages
          for (my $numErr_s = scalar @{$hashref_s->{Error}};
               $numErr_s > 0;
               $numErr_s--)
          {
            $Error_s .= "---------\n";
            $Error_s .= pop @{$hashref_s->{Error}};
            $Error_s .= "---------\n";
          }
        }
      }
    }
    if (scalar $Error_s ne "")
    {
      conf_process::Exit("There were errors in DMA configuration:\n" .
                         "---------------------------------------\n" .
                         $Error_s
                        );
    }
  }

  # create output hash and Pb look-up table based on input hash
  CreateOutputHash(\%dataInp_h, \%xDmaLut_h, \%xDmaOut_h, \%xCpuType_ch);

  conf_process::GenerateFile($path_Reports_s."rba_Dma_Out.txt",
                             "%xDmaLut_h:\n" . Dumper( \%xDmaLut_h ) .
                             "%xDmaOut_h:\n" . Dumper( \%xDmaOut_h ));

  # create all PBcfg files
  CreatePBcfg(\%xDmaOut_h,
              \%xDmaLut_h,
              \@xPbVar_a,
              $path_Sources_s,
              $path_Headers_s,
              $path_Scripts_s,
              \%xCpuType_ch);
}


####################################################################################################
# Create all make-relevant output files.
#
# \param   see local variables below
# \return  void
# \retval  none
####################################################################################################
sub CreatePBcfg()
{
  my $ptrOut_ph    = $_[0];               # pointer to output data hash
  my $ptrDmaLut_ph = $_[1];               # pointer to look-up table (hash)
  my $ptrPbVar_pa  = $_[2];               # pointer to list of all Pb variant names
  my $SrcPath_s    = $_[3];               # path for generated C files
  my $HdrPath_s    = $_[4];               # path for generated h files
  my $ScrptPath_s  = $_[5];               # script file path
  my $ptrCpuType_ph = $_[6];              # machine type info

  my $TmpltFileC_s = "";                  # contents of c-file temp
  my $TmpltFileH_s = "";                  # contents of h-file
  my %locDma_ch;                          # num check hash

  %locDma_ch = InitNumCheckHash($ptrCpuType_ph);

#  print Dumper (\%locDma_ch);

  # open template files
  $TmpltFileH_s = conf_process::GetFileContent(WhoAmI(), "${ScrptPath_s}\\rba_Dma_PBcfg.ht");
  $TmpltFileC_s = conf_process::GetFileContent(WhoAmI(), "${ScrptPath_s}\\rba_Dma_PBcfg.ct");

  # fill look-up table as some channels might be only present in one Pb variant
  # generate DMA loop-up table to symbolically access DMA channels
  {
    my $xEnums_s  = "";                     # enumeration list of all DMA signal Ids
    my $xSigDef_s = "\n";                   # presence macro for each configured signal
    my $xLut_s    = "";                     # DMA look-up table

    $xLut_s .= "struct DmaRslvr_st const rba_Dma_Cfg_Rslvr_acst[RBA_DMA_NUM_IDS_E][RBA_DMA_NUM_PB_VRNT_E] =\n";
    $xLut_s .= "{\n";
    $xEnums_s .= "typedef enum\n".
                 "{\n";
    # loop through all Dma Ids
    foreach my $xChId_s (sort {$a cmp $b} keys %{$ptrDmaLut_ph})
    {
      my @innerList_a;
      $xLut_s   .= "    { ";
      # add enum entry and presence macro
      $xEnums_s .= "    rba_Dma_$xChId_s,\n";
      $xSigDef_s .= "#define RBA_DMA_".uc($xChId_s)."_CONF\n";
      # loop through all existing Pb varaints
      foreach my $xPbVar_s (sort {$a cmp $b} @{$ptrPbVar_pa})
      {
        # if no entry exists for current Pb variant enter invalid 0xFF to indicate that
        if ( !exists($ptrDmaLut_ph->{$xChId_s}{$xPbVar_s}) )
        {
#          $ptrDmaLut_ph->{$xChId_s}{$xPbVar_s} = "NULL_PTR" ;
          $xLut_s .= "{ 0xFF, 0xFF, RBA_DMA_NUM_IDS_E }, ";
        }
        else
        {
          my $xDmaCh_s = $ptrDmaLut_ph->{$xChId_s}{$xPbVar_s};
          my $Sb;
          my $Ch;
          my $LinkCh = "RBA_DMA_NUM_IDS_E";
          $xDmaCh_s =~ m/^SB(\d)_CH(\d+)$/;
          $Sb = $1;
          $Ch = $2;
          # check if there linked channel configuration
 #         my $ref = $ptrOut_ph->{$xPbVar_s}{"SB$Sb"}{Mainref}
          if ( $ptrOut_ph->{$xPbVar_s}{"SB$Sb"}{MainRef}{"CH$Ch"}{LinkCh} ne "none" )
          {
            $LinkCh = "rba_Dma_".$ptrOut_ph->{$xPbVar_s}{"SB$Sb"}{MainRef}->{"CH$Ch"}{LinkCh};
          }
          # generate line
          $xLut_s .= sprintf("{ %1s, %2s, %3s },      ", $Sb, $Ch, $LinkCh);
        }
      }
      $xLut_s  .= " },     // $xChId_s\n";
    }
    $xLut_s .= "};\n";
    $xEnums_s .= "    RBA_DMA_NUM_IDS_E\n";
    $xEnums_s .= "} rba_Dma_Id_ten;\n";
    # replace placeholders with content
    $TmpltFileH_s =~ s/<\/RBA_DMA_PB_DMA_ENUMS\/>/$xEnums_s$xSigDef_s/;
    $TmpltFileC_s =~ s/<\/RBA_DMA_PB_CHM_LOOKUP\/>/$xLut_s/;
  }

  # JDP-only: create DMAMUX typedef for input MUX table
  {
    my $xTypedef_s = "";
    if ($ptrCpuType_ph->{Family} eq 'MCU_RB_JDP_UC1')
    {
      $xTypedef_s .= "typedef struct\n{\n";
      # loop through all DMAMUX instances
      for (my $DmaMux = 0; $DmaMux < $locDma_ch{SB}{DmaMux}{total}; $DmaMux++)
      {
        # extract number of outputs of the current DMAMUX
        my $Num = ${@{$locDma_ch{SB}{DmaMuxChan}}[$DmaMux]}{Num};
        # create size macro
        $xTypedef_s .= "    uint8  CHCONFIG${DmaMux}[RBA_DMA_MUX${DmaMux}_SIZE];\n";
      }
      $xTypedef_s .= "} rba_Dma_Mux_tst;\n";
    }
    # replace placeholders with content
    $TmpltFileH_s =~ s/<\/RBA_DMA_DMAMUX_TYPE\/>/$xTypedef_s/;
  }
  # JDP-only: buld up input MUX table
  if ($ptrCpuType_ph->{Family} eq 'MCU_RB_JDP_UC1')
  {
    my $xDmaMux_s = "const uint8 rba_Dma_Mux_acu8[RBA_DMA_NUM_PB_VRNT_E][RBA_DMA_NUM_MUX][16] =\n{\n";
    # build up data structures, outer PbVar loop
    foreach my $xPbVar_s (sort {$a cmp $b} @{$ptrPbVar_pa})
    {
      $xDmaMux_s .= "    // $xPbVar_s\n";
      $xDmaMux_s .= "    {\n";
      # loop through all DMAMUX instances
      for (my $i = 0; $i < $locDma_ch{SB}{DmaMux}{total}; $i++)
      {
        # at first extract move engine number associated with the current DMAMUX
        my $Sb = ${@{$locDma_ch{SB}{DmaMuxChan}}[$i]}{Sb};
        # and number of outputs of the current DMAMUX
        my $Num = ${@{$locDma_ch{SB}{DmaMuxChan}}[$i]}{Num};

        $xDmaMux_s .= " " x 8 . "{";
        # loop through the attached DMA channels
        for ( my $j = 0; $j < $Num; $j++ )
        {
          my $val_s = $ptrOut_ph->{$xPbVar_s}{"SB$Sb"}{"CHCONFIG$i"}[$j];
#print "val_s ($Sb, $i, $j): $val_s\n";
          # set ENBL bit if a connection is configured
          $val_s |= 0x80 if ( $val_s != 0x0 );
          $xDmaMux_s .= " $val_s, ";
        }
        # stuff the array for the lower four DMAMUX instances
        if ($Num == 8)
        {
          $xDmaMux_s .= "0, 0, 0, 0, 0, 0, 0, 0 ";
        }
        $xDmaMux_s .= "}, // DMAMUX_$i\n";
      }
      $xDmaMux_s .= "    },\n";
    }
    $xDmaMux_s .= "};\n\n";

    $TmpltFileC_s =~ s/<\/RBA_DMA_MUX\/>/$xDmaMux_s/;
  }
  else
  {
    $TmpltFileC_s =~ s/<\/RBA_DMA_MUX\/>//;
  }


  ######################################
  #          Pb Lookup:output          #
  ######################################
  {
    my $xPbEnum_s  = "";
    my $xPbVars_s  = "";
    my $xPbExt_s   = "";
    my $xPbReDef_s = "";

    # generate enum list of all configured Pb variants
    $xPbEnum_s   .= "typedef enum\n" .
                    "{\n";

    # simple loop over all Pb variants to generate rba_Dma_ConfigType
    foreach my $xPbVar_s (@{$ptrPbVar_pa})
    {
      my $locPbVar_s = conf_process::GetConfigName($xPbVar_s);
      $xPbEnum_s    .= "    ${locPbVar_s}_E,\n";
      $xPbVars_s    .= "rba_Dma_ConfigType const ${locPbVar_s} = ${locPbVar_s}_E;\n";
      $xPbExt_s     .= "extern rba_Dma_ConfigType const ${locPbVar_s};\n";
    }
    $xPbEnum_s   .= "    RBA_DMA_NUM_PB_VRNT_E\n" .
                    "} rba_Dma_ConfigType;\n";

    # In AR42 context the set of variants returned by GetVariants() may be a reduced set, because for variants with
    # identical input data only the 1st variant (master variant) is returned while the others are considered to be
    # matching variants to the master variant. Since it must be possible to initialize a component for all top level
    # predefined variants (TLPVs), config name macros must be generated for each matching variant which map to the
    # config name of the master variant.
    # The list returned by GetConfigMappingMacros() may be empty in which case the configuration processor shall
    # generate no marco. In AR40 context special AR42 compatibility macros are generated using the same mechanism.
    $xPbReDef_s .= "// macros generated by conf_process::GetConfigMappingMacros()\n";
    foreach my $xConfigMappingMacro_s (conf_process::GetConfigMappingMacros())
    {
      $xPbReDef_s .= CreateCDefine($xConfigMappingMacro_s, conf_process::GetConfigMappingMacroValue($xConfigMappingMacro_s));
    }

    # substitue placeholders in $xContentHt_s
    $TmpltFileH_s =~ s/<\/RBA_DMA_PB_PBENUMS\/>/$xPbEnum_s/;
    $TmpltFileH_s =~ s/<\/RBA_DMA_PB_REDEFS\/>/$xPbReDef_s/;
    $TmpltFileH_s =~ s/<\/RBA_DMA_PB_EXTERNALS\/>/$xPbExt_s/;
    # substitue placeholders in $xContentCt_s
    $TmpltFileC_s =~ s/<\/RBA_DMA_PB_VARS\/>/$xPbVars_s/;
  }

  # generate global DMA macros & JDP DMAMUX look-up table
  {
    my $xMacros_s  = "";
    my $xMuxSize_s = "";
    $xMacros_s .= "#define RBA_DMA_DEV_ERROR_DETECT  " . $ptrOut_ph->{General}{ActErrDet} . "\n";
    $xMacros_s .= "#define RBA_DMA_VERSION_INFO_API  " . $ptrOut_ph->{General}{VrsnInfoApi} . "\n";

    if ($ptrCpuType_ph->{Family} eq 'MCU_RB_IFX_UC1')
    {
      $xMacros_s .= "#define RBA_DMA_NUM_DMA_MODULES    $locDma_ch{SB}{Modules}u\n";
      $xMacros_s .= "#define RBA_DMA_NUM_DMA_CHANS     $locDma_ch{SB}{Channels}u\n";
    }
    if ($ptrCpuType_ph->{Family} eq 'MCU_RB_JDP_UC1')
    {
      $xMacros_s .= "#define RBA_DMA_NUM_DMA_MODULES     $locDma_ch{SB}{Modules}u\n";
      $xMacros_s .= "#define RBA_DMA_NUM_DMA_CHANS      $locDma_ch{SB}{Channels}u\n";
      $xMacros_s .= "#define RBA_DMA_NUM_MUX            $locDma_ch{SB}{DmaMux}{total}u\n";
      # start DMAMUX size table
      $xMuxSize_s .= "const uint8 rba_Dma_MuxSize_acu8[RBA_DMA_NUM_MUX] =\n{\n";
      # create DMAMUX size macros
      for (my $DmaMux = 0; $DmaMux < $locDma_ch{SB}{DmaMux}{total}; $DmaMux++)
      {
        # and number of outputs of the current DMAMUX
        my $Num = ${@{$locDma_ch{SB}{DmaMuxChan}}[$DmaMux]}{Num};
        # create size macro
        $xMacros_s .= "#define RBA_DMA_MUX${DmaMux}_SIZE          $Num\n";
        # fill DMAMUX size table
        $xMuxSize_s .= "    RBA_DMA_MUX${DmaMux}_SIZE,\n";
      }
      # close DMAMUX size table
      $xMuxSize_s .= "};\n\n";
    }
    # generate error reaction macro
    $xMacros_s .= "#define RBA_DMA_ERROR_ACTION   " . $ptrOut_ph->{General}{ErrReaction} . "\n";

    # Get Autosar Version info based on ECUC values
    {
      my $AR_Version_ph = GetAutosarVersion();
      $xMacros_s .= "/* Autosar version information */\n";
      $xMacros_s .= CreateCDefine('RBA_DMA_AR_RELEASE_MAJOR_VERSION'    ,$AR_Version_ph->{MAJOR});
      $xMacros_s .= CreateCDefine('RBA_DMA_AR_RELEASE_MINOR_VERSION'    ,$AR_Version_ph->{MINOR});
      $xMacros_s .= CreateCDefine('RBA_DMA_AR_RELEASE_REVISION_VERSION' ,$AR_Version_ph->{REVISION});
    }

    # substitue placeholders in $xContentHt_s
    $TmpltFileH_s =~ s/<\/RBA_DMA_MACROS\/>/$xMacros_s/;
    $TmpltFileC_s =~ s/<\/RBA_DMA_MUXSIZE\/>/$xMuxSize_s/;
  }

  # generate file(s)
  conf_process::GenerateFile($SrcPath_s."rba_Dma_PBcfg.h", $TmpltFileH_s);
  conf_process::GenerateFile($SrcPath_s."rba_Dma_PBcfg.c", $TmpltFileC_s);
}



sub CreateOutputHash(\%\%\%\%)
{
  my $ptrCfg_ph     = $_[0];
  my $ptrLut_ph     = $_[1];
  my $ptrOut_ph     = $_[2];
  my $ptrCpuType_ph = $_[3];
  my %locDma_ch;

  %locDma_ch = InitNumCheckHash($ptrCpuType_ph);

  # global stuff
  my $ErrReaction_s = $ptrCfg_ph->{General}{ErrReaction};
  my $ActErrDet_s   = $ptrCfg_ph->{General}{ActErrDet};
  my $VrsnInfoApi_s = $ptrCfg_ph->{General}{VrsnInfoApi};
  $ptrOut_ph->{General}{ErrReaction} = "RBA_DMA_ERROR_ACTION_NONE"  if ($ErrReaction_s eq "None");
  $ptrOut_ph->{General}{ErrReaction} = "RBA_DMA_ERROR_ACTION_DET"   if ($ErrReaction_s eq "Det");
  $ptrOut_ph->{General}{ErrReaction} = "RBA_DMA_ERROR_ACTION_RESET" if ($ErrReaction_s eq "Reset");
  $ptrOut_ph->{General}{ActErrDet}   = "STD_OFF";
  $ptrOut_ph->{General}{VrsnInfoApi} = "STD_OFF";
  $ptrOut_ph->{General}{ActErrDet}   = "STD_ON" if ($ActErrDet_s   == 1);
  $ptrOut_ph->{General}{VrsnInfoApi} = "STD_ON" if ($VrsnInfoApi_s == 1);

  # loop over configured DMA channels in all Pb variants in global hash
  # fill DMA output hash through $ptrOut_ph
  # outer loop through all Pb variants
  foreach my $xPbVar_s (keys %{$ptrCfg_ph->{Pb}})
  {
    # create trigger source registers for JDP devices
    if ( $ptrCpuType_ph->{Family} eq 'MCU_RB_JDP_UC1' )
    {
      # loop through all DMAMUX instances
      for (my $DmaMux = 0; $DmaMux < $locDma_ch{SB}{DmaMux}{total}; $DmaMux++)
      {
        # at first extract move engine number associated with the current DMAMUX
        my $Sb = ${@{$locDma_ch{SB}{DmaMuxChan}}[$DmaMux]}{Sb};
        # then create DMAMUX (CHCONFIG) array
        @{$ptrOut_ph->{$xPbVar_s}{"SB$Sb"}{"CHCONFIG$DmaMux"}} = ();    # = ( 0, 0, 0, 0, 0, 0, 0, 0 );
        # now fill array with default '0'
        for (my $i = 0; $i < ${@{$locDma_ch{SB}{DmaMuxChan}}[$DmaMux]}{Num}; $i++)
        {
          @{$ptrOut_ph->{$xPbVar_s}{"SB$Sb"}{"CHCONFIG$DmaMux"}}[$i] = 0;
        }
#        @{$ptrOut_ph->{$xPbVar_s}{"SB$Sb"}{CHCONFIG1}} = ( 0, 0, 0, 0, 0, 0, 0, 0 );
#        @{$ptrOut_ph->{$xPbVar_s}{"SB$Sb"}{CHCONFIG2}} = ( 0, 0, 0, 0, 0, 0, 0, 0 );
#        @{$ptrOut_ph->{$xPbVar_s}{"SB$Sb"}{CHCONFIG3}} = ( 0, 0, 0, 0, 0, 0, 0, 0 );
#        @{$ptrOut_ph->{$xPbVar_s}{"SB$Sb"}{CHCONFIG4}} = ( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
#        @{$ptrOut_ph->{$xPbVar_s}{"SB$Sb"}{CHCONFIG5}} = ( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
      }
    }
    # inner loop through all configured channel requests in current Pb variant,
    # sorted by channel name
    foreach my $xChan_s ( sort {$a cmp $b} keys %{$ptrCfg_ph->{Pb}{$xPbVar_s}{Ch}} )
    {
      # get reference for shorter access
      my $hashref_s = $ptrCfg_ph->{Pb}{$xPbVar_s}{Ch}{$xChan_s};
      my $locUnit_s = $hashref_s->{Unit};
      my $locChan_s = $hashref_s->{Channel};
      my $locIrq_s  = $hashref_s->{Irq};
      my $locTrg_s  = $hashref_s->{TrgSrc};

      # enter list into %xOut_h
      $ptrOut_ph->{$xPbVar_s}{$locUnit_s}{User}{$locChan_s}    = $xChan_s;
      $ptrOut_ph->{$xPbVar_s}{$locUnit_s}{MainRef}{$locChan_s} = $hashref_s;


      # do the trigger source configuration (JDP only)
      if ( $ptrCpuType_ph->{Family} eq 'MCU_RB_JDP_UC1' )
      {
        # check if a trigger is configured
        if ( $locTrg_s ne 'none' )
        {
          my %xMuxReg_h;
          %xMuxReg_h = rba_Dma_ActMachineType::GetTriggerRegs($locUnit_s,
                                                              $locChan_s,
                                                              $locTrg_s,
                                                              $ptrCpuType_ph);
          # enter value into hash (the resolving will not fail as the connection is checked in CreateMainHash()
          @{$ptrOut_ph->{$xPbVar_s}{$locUnit_s}{"CHCONFIG$xMuxReg_h{MuxIdx}"}}[$xMuxReg_h{RegIdx}] = $xMuxReg_h{Val};
        }
      }

      # take care for a configured IRQ demand
      if ( $locIrq_s ne "none" )
      {
        # enter into out hash under common 'Irq' key
        $ptrOut_ph->{$xPbVar_s}{$locUnit_s}{Irq}{$locChan_s} = $locIrq_s;
      }
    }
  }
}



#---------------------------------------------------------------------------------------------------
# CreateMainHash
# $_[0]: complete configuration hash of rba_Dma
# $_[1]: pointer to DMA look-up tab le (to be filled)
# $_[2]: CPU / machine type information
#
# Creats main hash containing all rba_Dma configuration data.
#
# return: hash of all configuration data
#---------------------------------------------------------------------------------------------------
sub CreateMainHash ($\%)
{
  my $xEcuc_ph      = $_[0];
  my $xDmaLut_ph    = $_[1];
  my $ptrCpuType_ph = $_[2];              # machine type info
  my %xResult_h;
  my %xNames_h;
  # Get general information from ECUC values
  my $rba_DmaConfig_ph  = undef;
  my $rba_DmaGeneral_ph = undef;
  my %xPbVariant_h = ();
  my %locDma_ch;
  %locDma_ch = InitNumCheckHash($ptrCpuType_ph);

  $rba_DmaConfig_ph   = $xEcuc_ph->{'rba_Dma_Cfg'};
  $rba_DmaGeneral_ph  = $xEcuc_ph->{'rba_Dma_General'};

  ##################################
  #        rba_Dma_General         #
  ##################################
  {
    my $swtActErrDet_s   = 0;
    my $swtErrReaction_s = "None";
    my $swtVrsnInfoApi_s = 0;

    # read rba_Dma_ActErrDet parameter if defined
    if ( defined($rba_DmaGeneral_ph->{rba_Dma_ActErrDet}{content}) )
    {
      $swtActErrDet_s = conf_process::GetBoolean($rba_DmaGeneral_ph->{rba_Dma_ActErrDet}{content});
    }
    # read rba_Dma_ErrReaction parameter if defined
    if ( defined($rba_DmaGeneral_ph->{rba_Dma_ErrReaction}{content}) )
    {
      $swtErrReaction_s = $rba_DmaGeneral_ph->{rba_Dma_ErrReaction}{content};
    }
    # read rba_Dma_VersionInfoApi parameter if defined
    if ( defined($rba_DmaGeneral_ph->{rba_Dma_VersionInfoApi}{content}) )
    {
      $swtVrsnInfoApi_s = conf_process::GetBoolean($rba_DmaGeneral_ph->{rba_Dma_VrsnInfoApi}{content});
    }
    # write to result hash
    $xResult_h{General}{ActErrDet}   = $swtActErrDet_s;
    $xResult_h{General}{ErrReaction} = $swtErrReaction_s;
    $xResult_h{General}{VrsnInfoApi} = $swtVrsnInfoApi_s;
  }

  ##################################
  #          rba_Dma_Cfg           #
  ##################################
  # Loop over all available PostBuild configurations
  foreach my $outerKey_s (@{$rba_DmaConfig_ph})
  {
    my $xPbVar_s          = $outerKey_s->{'rba_Dma_Cfg__KEY'}{'content'};
    my $xConfig_Module_s  = $outerKey_s->{'Module'};
    my $xConfig_FileRef_s = $outerKey_s->{'FileRef'};
    my %xChanCheck_h;     # check for doubled configured resources

    conf_process::Log("Pbs variant name: $xPbVar_s");

    # Loop over all available Dma channel configuration sets
    foreach my $key (@{$outerKey_s->{'rba_Dma_Ch'}})
    {
      # read configuration data and remove whitespaces where not needed
      my $xShortName_s = RmWs($key->{'rba_Dma_Ch__KEY'}{'content'});
      my $xModRef_s    = $key->{'Module'};
      my $xFileRef_s   = $key->{'FileRef'};
      my $xUnit_s      = RmWs($key->{'rba_Dma_1_Unit'}{'content'});
      my $xChannel_s   = RmWs($key->{'rba_Dma_2_Ch'}{'content'});
      my $xTrgSrc_s    = "none";
      my $xIrq_s       = "none";
      my $xLinkCh_s    = "none";

      # default is 'clean'
      $xResult_h{Pb}{$xPbVar_s}{Ch}{$xShortName_s}{Status} = "clean";

      # check move engine and channel number (for all machine types)
      if ( GetNum($xUnit_s) >= $locDma_ch{SB}{Modules} )
      {
        # mark channel as faulty
        $xResult_h{Pb}{$xPbVar_s}{Ch}{$xShortName_s}{Status} = "faulty";
        push @{$xResult_h{Pb}{$xPbVar_s}{Ch}{$xShortName_s}{Error}}, (
               " Error in DMA channel configuration (PbVar: $xPbVar_s).\n" .
               " The configured move engine number: $xUnit_s is invalid.\n" .
               " in configuration of channel: $xShortName_s ($xUnit_s.$xChannel_s)\n"
              );
      }
      if ( GetNum($xChannel_s) >= $locDma_ch{SB}{Channels} )
      {
        # mark channel as faulty
        $xResult_h{Pb}{$xPbVar_s}{Ch}{$xShortName_s}{Status} = "faulty";
        push @{$xResult_h{Pb}{$xPbVar_s}{Ch}{$xShortName_s}{Error}}, (
               " Error in DMA channel configuration (PbVar: $xPbVar_s).\n" .
               " The configured channel number: $xChannel_s is invalid\n" .
               " in configuration of channel: $xShortName_s ($xUnit_s.$xChannel_s)\n" .
               " Maximum channel number is ".($locDma_ch{SB}{Channels}-1)." in $ptrCpuType_ph->{Type}.\n"
              );
      }

      # check if DMA channel number is already configured
      unless ( exists ($xChanCheck_h{$xUnit_s}{$xChannel_s}) )
      {
        # enter channel info to hash
        $xChanCheck_h{$xUnit_s}{$xChannel_s} = $xShortName_s;
      }
      else
      {
        # mark channel as faulty
        $xResult_h{Pb}{$xPbVar_s}{Ch}{$xShortName_s}{Status} = "faulty";
        push @{$xResult_h{Pb}{$xPbVar_s}{Ch}{$xShortName_s}{Error}}, (
               " Error in DMA channel configuration (PbVar: $xPbVar_s).\n" .
               " The configured move engine / channel number of DMA channel $xShortName_s (DMA$xUnit_s.$xChannel_s)\n" .
               " is already occupied by: $xChanCheck_h{$xUnit_s}{$xChannel_s}\n"
              );
      }

      # read trigger source if configured (JDP only, is ignored in case of IFX)
      if ( $ptrCpuType_ph->{Family} eq 'MCU_RB_JDP_UC1' )
      {
        if ( exists($key->{'rba_Dma_3_TrgSrc'}) )
        {
          my %xMuxReg_h;
          # read configuration item
          $xTrgSrc_s = RmWs($key->{'rba_Dma_3_TrgSrc'}{'content'});
          # get settings
          %xMuxReg_h = rba_Dma_ActMachineType::GetTriggerRegs($xUnit_s,
                                                              $xChannel_s,
                                                              $xTrgSrc_s,
                                                              $ptrCpuType_ph);

          # check if connection is possible
          if ( $xMuxReg_h{Val} == 0 )
          {
            # mark channel as faulty
            $xResult_h{Pb}{$xPbVar_s}{Ch}{$xShortName_s}{Status} = "faulty";
            # enter error message
            push @{$xResult_h{Pb}{$xPbVar_s}{Ch}{$xShortName_s}{Error}}, (
                   " Error in DMA channel configuration (PbVar: $xPbVar_s):\n" .
                   " The configured trigger source: $xTrgSrc_s\n" .
                   " cannot be connected to: $xShortName_s ($xUnit_s.$xChannel_s)\n"
                  );
          }
        }
      }

      # check if rba_Dma_4_Irq is configured
      $xIrq_s = RmWs($key->{'rba_Dma_4_Irq'}{'content'}) if ( exists($key->{'rba_Dma_4_Irq'}) );

      # check if rba_Dma_5_LinkCh is configured
      if ( exists($key->{'rba_Dma_5_LinkCh'}) )
      {
        # read value, checking of this value is only possible after all channels have been processed
        # this is done in the separate loop below
        $xLinkCh_s = RmWs($key->{'rba_Dma_5_LinkCh'}{'content'});
      }

      # Add current Tim to result hash
      $xResult_h{Pb}{$xPbVar_s}{Ch}{$xShortName_s}{Unit}          = $xUnit_s;
      $xResult_h{Pb}{$xPbVar_s}{Ch}{$xShortName_s}{Channel}       = $xChannel_s;
      $xResult_h{Pb}{$xPbVar_s}{Ch}{$xShortName_s}{TrgSrc}        = $xTrgSrc_s;
      $xResult_h{Pb}{$xPbVar_s}{Ch}{$xShortName_s}{Irq}           = $xIrq_s;
      $xResult_h{Pb}{$xPbVar_s}{Ch}{$xShortName_s}{LinkCh}        = $xLinkCh_s;
      $xResult_h{Pb}{$xPbVar_s}{Ch}{$xShortName_s}{PbVariant}     = $xPbVar_s;
      $xResult_h{Pb}{$xPbVar_s}{Ch}{$xShortName_s}{PbCfg_Module}  = $xConfig_Module_s;
      $xResult_h{Pb}{$xPbVar_s}{Ch}{$xShortName_s}{PbCfg_FileRef} = $xConfig_FileRef_s;
      $xResult_h{Pb}{$xPbVar_s}{Ch}{$xShortName_s}{FileRef}       = $xFileRef_s;
      $xResult_h{Pb}{$xPbVar_s}{Ch}{$xShortName_s}{ModRef}        = $xModRef_s;

      # fill DMA channel Pb look-up table
      $xDmaLut_ph->{$xShortName_s}{$xPbVar_s} = $xUnit_s."_".$xChannel_s;
    }

    # loop again to check contents of rba_Dma_5_LinkCh
    foreach my $idSig_s (keys %{$xResult_h{Pb}{$xPbVar_s}{Ch}})
    {
      if ($xResult_h{Pb}{$xPbVar_s}{Ch}{$idSig_s}{LinkCh} ne "none")
      {
        my $xLinkCh_s = $xResult_h{Pb}{$xPbVar_s}{Ch}{$idSig_s}{LinkCh};
        # check if configured link target is configured in this Pb variant
        unless ( exists($xResult_h{Pb}{$xPbVar_s}{Ch}{$xLinkCh_s}) )
        {
          # mark channel as faulty
          $xResult_h{Pb}{$xPbVar_s}{Ch}{$idSig_s}{Status} = "faulty";
          push @{$xResult_h{Pb}{$xPbVar_s}{Ch}{$idSig_s}{Error}}, (
                 " Error in DMA channel configuration (PbVar: $xPbVar_s)\n" .
                 " of channel: $idSig_s\n" .
                 " The configured link target: $xLinkCh_s \n" .
                 " is not existing in the current Pb variant. \n"
                );
        }
        else
        {
          # on JDP devices channel linking is only possible within the same move engine
          if ( $ptrCpuType_ph->{Family} eq 'MCU_RB_JDP_UC1' )
          {
            if (   $xResult_h{Pb}{$xPbVar_s}{Ch}{$xLinkCh_s}{Unit}
                ne $xResult_h{Pb}{$xPbVar_s}{Ch}{$idSig_s}{Unit}
               )
            {
              # mark channel as faulty
              $xResult_h{Pb}{$xPbVar_s}{Ch}{$idSig_s}{Status} = "faulty";
              push @{$xResult_h{Pb}{$xPbVar_s}{Ch}{$idSig_s}{Error}}, (
                     " Error in DMA channel configuration (PbVar: $xPbVar_s)\n" .
                     " of channel: $idSig_s\n" .
                     " The configured link target: $xLinkCh_s \n" .
                     " is not located on the same move engine. This is\n" .
                     " required on JDP devices for channel linking. \n"
                    );
            }
          }
        }
      }
    }
  }
  return %xResult_h;
}


#---------------------------------------------------------------------------------------------------
# GetTimInpData: Machine type dependent routing function to initialise a hash containing all
#                output connection registers.
#
# Parameters:    $_[0]: pointer to CPU information garthered through GetCpuType()
#
# return: Hash containing all output registers pre-filled with 0x0.
#---------------------------------------------------------------------------------------------------
sub InitNumCheckHash ($)
{
#  my $xPbVar_pa   = $_[0];
  my $xCpuType_ph = $_[0];
  my %xNumCheck_h;

  if ( $xCpuType_ph->{Device}  eq "MCU_RB_JDP_UC1_DEV1_ALL" )
  {
    $xNumCheck_h{SB}{Modules}       =  1;            # number of move engines
    $xNumCheck_h{SB}{Channels}      = 16;            # number of channels per move engine
    $xNumCheck_h{SB}{DmaMux}{total} =  2;            # total number of DMAMUX instances
    $xNumCheck_h{SB}{DmaMux}{SB0}   =  2;            # number of DMAMUX instances, SB0
    @{$xNumCheck_h{SB}{DmaMuxChan}} =  ({Num=>8, Sb=>0},
                                        {Num=>8, Sb=>0});
  }
  if ( $xCpuType_ph->{Device}  eq "MCU_RB_JDP_UC1_DEV2_ALL" )
  {
    $xNumCheck_h{SB}{Modules}       =  1;            # number of move engines
    $xNumCheck_h{SB}{Channels}      = 32;            # number of channels per move engine
    $xNumCheck_h{SB}{DmaMux}{total} =  4;            # total number of DMAMUX instances
    $xNumCheck_h{SB}{DmaMux}{SB0}   =  4;            # number of DMAMUX instances, SB0
    # input numbers of each DMAMUX and attached move engine number; array index is DMAMUX instance
    @{$xNumCheck_h{SB}{DmaMuxChan}} =  ({Num=>8, Sb=>0},
                                        {Num=>8, Sb=>0},
                                        {Num=>8, Sb=>0},
                                        {Num=>8, Sb=>0});
  }
  if ( $xCpuType_ph->{Device} eq "MCU_RB_ST_UC1_DEV3_40NM_ALL" )
  {
    $xNumCheck_h{SB}{Modules}       =  2;             # number of move engines
    $xNumCheck_h{SB}{Channels}      = 64;             # number of channels per move engine
    $xNumCheck_h{SB}{SB0}{Channels} = 32;             # number of channels in move engine 0
    $xNumCheck_h{SB}{SB1}{Channels} = 64;             # number of channels in move engine 1
    $xNumCheck_h{SB}{DmaMux}{total} =  6;             # total number of DMAMUX instances
    $xNumCheck_h{SB}{DmaMux}{SB0}   =  2;             # number of DMAMUX instances, SB0
    $xNumCheck_h{SB}{DmaMux}{SB1}   =  4;             # number of DMAMUX instances, SB1
    # input numbers of each DMAMUX (Num) and attached move engine number (Sb); array index is DMAMUX instance
    @{$xNumCheck_h{SB}{DmaMuxChan}} =  ({Num=>16, Sb=>1},
                                        {Num=>16, Sb=>1},
                                        {Num=>16, Sb=>1},
                                        {Num=>16, Sb=>1},
                                        {Num=>16, Sb=>0},
                                        {Num=>16, Sb=>0});
  }
  if ( $xCpuType_ph->{Device} eq "MCU_RB_JDP_UC1_DEV4_ALL" )
  {
    $xNumCheck_h{SB}{Modules}       =  2;               # number of move engines
    $xNumCheck_h{SB}{Channels}      = 64;               # number of channels per move engine
    $xNumCheck_h{SB}{DmaMux}{total} = 10;               # total number of DMAMUX instances
    $xNumCheck_h{SB}{DmaMux}{SB0}   =  6;               # number of DMAMUX instances, SB0
    $xNumCheck_h{SB}{DmaMux}{SB1}   =  4;               # number of DMAMUX instances, SB1
    # input numbers of each DMAMUX (Num) and attached move engine number (Sb); array index is DMAMUX instance
    @{$xNumCheck_h{SB}{DmaMuxChan}} =  ({Num=>8, Sb=>0},
                                        {Num=>8, Sb=>0},
                                        {Num=>8, Sb=>0},
                                        {Num=>8, Sb=>0},
                                        {Num=>16, Sb=>0},
                                        {Num=>16, Sb=>0},
                                        {Num=>16, Sb=>1},
                                        {Num=>16, Sb=>1},
                                        {Num=>16, Sb=>1},
                                        {Num=>16, Sb=>1});
  }
  if ( $xCpuType_ph->{Device} eq "MCU_RB_IFX_UC1_DEV2_ALL" )
  {
    $xNumCheck_h{SB}{Modules}   =  1;                    # number of virtual move engines (it has actually two)
    $xNumCheck_h{SB}{SubBlocks} =  2;                    # number of sub-blocks
    $xNumCheck_h{SB}{Channels}  = 48;                    # number of channels per virtual move engine
  }
  if ( $xCpuType_ph->{Device} eq "MCU_RB_IFX_UC1_DEV3_ALL" )
  {
    $xNumCheck_h{SB}{Modules}   =  1;                    # number of virtual move engines (it has actually two)
    $xNumCheck_h{SB}{SubBlocks} =  2;                    # number of sub-blocks
    $xNumCheck_h{SB}{Channels}  = 64;                    # number of channels per virtual move engine
  }
  if ( $xCpuType_ph->{Device} eq "MCU_RB_IFX_UC1_DEV4_ALL" )
  {
    $xNumCheck_h{SB}{Modules}   =   1;                   # number of virtual move engines (it has actually two)
    $xNumCheck_h{SB}{SubBlocks} =   2;                   # number of sub-blocks
    $xNumCheck_h{SB}{Channels}  = 128;                   # number of channels per virtual move engine
  }
  if (( $xCpuType_ph->{Device} eq "MCU_RB_IFX_UC1_DEV4_40NM_ALL" ) || ( $xCpuType_ph->{Device} eq "MCU_RB_IFX_UC1_DEV3_40NM_ALL" ))
  {
    $xNumCheck_h{SB}{Modules}   =   1;                   # number of virtual move engines (it has actually two)
    $xNumCheck_h{SB}{SubBlocks} =   2;                   # number of sub-blocks
    $xNumCheck_h{SB}{Channels}  = 128;                   # number of channels per virtual move engine
  }
  if ( $xCpuType_ph->{Device} eq "MCU_RB_IFX_UC1_DEV5_40NM_ALL" )
  {
    $xNumCheck_h{SB}{Modules}   =   1;                   # number of virtual move engines (it has actually two)
    $xNumCheck_h{SB}{SubBlocks} =   2;                   # number of sub-blocks
    $xNumCheck_h{SB}{Channels}  = 128;                   # number of channels per virtual move engine
  }

  return %xNumCheck_h;
}


# GetCpuType() returns the MCU provided CPU type information
sub GetCpuType
{
  my %xCpuType_h;
  my $xMcu_ph = conf_process::GetInvariantInputData("Mcu");

  # read Mcu information and fill hash
  #define MCU_RB_MACHINE_FAM             MCU_RB_IFX_UC1
  #define MCU_RB_MACHINE_TYPE            MCU_RB_IFX_UC1_DEV3
  #define MCU_RB_MACHINE_SUBTYPE         MCU_RB_IFX_UC1_DEV3_PRE
  $xCpuType_h{FullString} = $xMcu_ph->{McuRbCpuInformation}{McuRbMachineSubType}{content};
  $xCpuType_h{Family}     = $xMcu_ph->{McuRbCpuInformation}{McuRbMachineFam}{content};
  $xCpuType_h{Device}     = $xMcu_ph->{McuRbCpuInformation}{McuRbMachineDev}{content};
  $xCpuType_h{Type}       = $xMcu_ph->{McuRbCpuInformation}{McuRbMachineType}{content};
  $xCpuType_h{SubType}    = $xMcu_ph->{McuRbCpuInformation}{McuRbMachineSubType}{content};

  # return complete hash
  return %xCpuType_h;
}

# RmWs: returns given string after removing whitespaces from it
sub RmWs ($)
{
  my $xString_s = $_[0];
  $xString_s  =~ s/\s+//g;
  return $xString_s;
}

# GetNum is used to extract a number from a string (i.e removes all non-number characters)
sub GetNum ($)
{
  my $xString_s = $_[0];
  $xString_s =~ s/\D+//g;
  return $xString_s;
}

#----------------------------------------------------------------------------------------------------------------------
# ExtractPostBuildName: Extracts the individual name from the postbuild configuration set's name.
#
# Parameters:
# $_[0]: post build dataset name [IN]
#
# Example:
# Input:   "rba_Dma_Cfg_4cyl" or "rba_Dma_Cfg"
# Returns: The name suffix or "" if name is correct, otherwise conf process exits.
#----------------------------------------------------------------------------------------------------------------------
#sub ExtractPostBuildName
#{
#  my $DataSetName_s  = $_[0];
#  my $PBName_s;
#
#  if ($DataSetName_s =~ /^rba_Dma_Cfg$/)
#  {
#      $PBName_s = "";
#  }
#  elsif ($DataSetName_s =~ /^rba_Dma_Cfg(_.+)$/)
#  {
#      $PBName_s = $1;
#  }
#  else
#  {
#      conf_process::Exit( "ERROR: $DataSetName_s is not a valid PostBuild configuration name!\n".
#                          "Valid names are: rba_Dma_Cfg or rba_Dma_Cfg_<PostbuildName>");
#  }
#
#  return $PBName_s;
#}

#----------------------------------------------------------------------------------------------------------------------
# Subroutine to generate a #define
#
# Parameters:
# $_[0]: Name of the define to be generated
# $_[1]: Value which should be assigned to the define
#----------------------------------------------------------------------------------------------------------------------
sub CreateCDefine 
{
    my $name_s  = shift();
    my $value_s = shift();

    return sprintf( "#define %-51s %s\n", $name_s, $value_s );
}

#----------------------------------------------------------------------------------------------------------------------
# Subroutine to get AutosarVersion from EcuC
#
# Preconditions:
# Needs input artifact /AUTOSAR_EcuC/EcucModuleDefs/EcuC/EcucRbSettings/EcucRbArRelease specified in BAMF
#
# Returns:
# HASH reference with
# {MAJOR => 4, MINOR => 0,  REVISION => 2}
# or
# {MAJOR => 4, MINOR => 2,  REVISION => 2}
#----------------------------------------------------------------------------------------------------------------------
sub GetAutosarVersion
{
  my $EcuC_ph = conf_process::GetInvariantInputData("EcuC");

  # if EcuCRbArRelease does not exist fall back to AR40
  if (exists $EcuC_ph->{'EcuCRbSettings'}{'EcuCRbArRelease'}{'content'})
  {
    if ($EcuC_ph->{'EcuCRbSettings'}{'EcuCRbArRelease'}{'content'} eq 'AR40_Rev2')
    {
      return {MAJOR => 4, MINOR => 0,  REVISION => 2};
    }
    elsif ($EcuC_ph->{'EcuCRbSettings'}{'EcuCRbArRelease'}{'content'} eq 'AR42_Rev2')
    {
      return {MAJOR => 4, MINOR => 2,  REVISION => 2};
    }
    else
    {
      # No need to collect this warning since it has such a big impact that further processing is not usefull
      conf_process::Exit("ERROR: Content of EcuCRbArRelease is Unknown: " . $EcuC_ph->{'EcuCRbSettings'}{'EcuCRbArRelease'}{'content'});
    }
  }
  else
  {
    return {MAJOR => 4, MINOR => 0,  REVISION => 2};
  }
}

####################################################################################################
# Anonymous function
####################################################################################################
{
  1;
}

__END__
