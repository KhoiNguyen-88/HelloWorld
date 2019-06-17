#<RBHead>
#************************************************************************
#                                                                       *
#                      ROBERT BOSCH GMBH                                *
#                          STUTTGART                                    *
#                                                                       *
#          Alle Rechte vorbehalten - All rights reserved                *
#                                                                       *
#************************************************************************
#
#************************************************************************
#    Administrative Information (automatically filled in by eASEE)      *
#************************************************************************
#
# $Filename__:$
#
# $Author____:$
#
# $Function__:$
#
#************************************************************************
# $Domain____:$
# $User______:$
# $Date______:$
# $Class_____:$
# $Name______:$
# $Variant___:$
# $Revision__:$
# $Type______:$
# $State_____:$
# $Generated_:$
#************************************************************************
#
# $UniqueName:$
# $Component_:$
#
#
#************************************************************************
#</RBHead>

package rba_I2C_EcucProc;
use strict;
use warnings;
use conf_process;
use Data::Dumper;
#**********************************************************************************************************************
# Actions
#**********************************************************************************************************************

#----------------------------------------------------------------------------------------------------------------------
# Generate: Generates all Input files of this module
#
# Parameters:
# $_[1]: Destination paths for generated files [IN]
# $_[2]: Path to this ECUC processor [IN]
#----------------------------------------------------------------------------------------------------------------------

sub Generate
{
    my $path_csources_s = $_[1][0];
    my $path_headers_s  = $_[1][1];
    my $path_Reports_s  = $_[1][6];
    my $path_scripts_s  = $_[2];

    conf_process::Log("Start " . WhoAmI() . " script");

    # Set the module implementation prefix (Mip) for the current component.
    # The Mip is needed to build the name of the configuration structure generated with the help of GetConfig()
    # and its related APIs. Only neccesary for AR42 but for OneFitsAll it is called in AR40 and AR42
    conf_process::SetMip("rba_I2c");

    # Set the name of the MultipleConfigurationContainer (MCC) which is used for configuration variants in AR40 context.
    # In AR40 context this name will be used to create the variants returned by GetVariants() and to create the name
    # for the configuration structure.
    # In AR42 context this name will not be used but for OneFitsAll it is called in AR40 and AR42
    conf_process::SetAr40MultipleConfigurationContainer("rba_I2c_ConfigSet");   # only neccesary for AR4.0

    # This API analyzes all input data specified in BAMF with respect to the given list of multiple configuration
    # containers (MCCs). The list of MCCs shall contain all MCCs present in the input data of the current build action.
    conf_process::AnalyzeInputData("rba_I2c_ConfigSet");

    my $rba_I2c_GenerateEmptyFiles_s = 0;

    # Check if input data (invariant or variant) is available
    if(conf_process::IsInputDataAvailable("rba_I2c"))
    {

        # get Machine type and Machine subtype name from the MCU configuration.
        my ($MachineType_s,$MachineSubType_s) = GetMachineType();

        if( $MachineType_s eq 'MCU_RB_JDP_UC1_DEV1_ALL')
        {
            conf_process::Exit("ERROR   rba_I2C  - module interface not support for MDG1_DEV-1 controller, need to remove the I2C configuration");
        }

        # Get the General configuration from the ECUC values for I2C moduele
        my %rba_I2c_General_h = GetGeneral();

        # Mandatory Postbuild dataset configuration validation for the "rba_I2c_ConfigSet"
        my $ecucPostBuildConfig_ph = conf_process::GetInvariantInputData("rba_I2c")->{'rba_I2c_ConfigSet'};

            # validate the postbuild variant.
            CheckPostbuildvariant();

            # get machine dependent file name.
            my $Machinefile_s = GetMachineData($MachineType_s,$path_scripts_s);

            # Get the General configuration from the ECUC values for I2C module
            my ($rba_I2c_Slaveconfig_ph,$rba_I2c_slaveconfigPostbuild_ph) = GetSlaveConfig($MachineType_s);

            # validate the slave configuraiton "$rba_I2c_Slaveconfig_ph".
            if(defined $rba_I2c_Slaveconfig_ph)
            {
                # Get the sequence configuration from the ECUC values for I2C module
                my ($rba_I2c_sequenceconfigPostbuild_ph) = GetSequenceConfig();

                if( $rba_I2c_sequenceconfigPostbuild_ph->{'maxSequenceConnt'} > 0)
                {
                    # Get Hardware configuration
                    GetHwConfig($MachineType_s, $rba_I2c_Slaveconfig_ph, $path_scripts_s, $Machinefile_s);

                    # read Max Hardware unit.
                    my $rba_I2c_MaxHwUnits_ph = GenerateMaxHwUnit($rba_I2c_Slaveconfig_ph, $MachineType_s );

                    # read slave info list
                    my $rba_I2c_SlaveId_s     = GenerateSlaveIdList($rba_I2c_Slaveconfig_ph);

                    # read slave info list.
                    my $rba_I2c_SequenceId_s  = GenerateSequenceIdList($rba_I2c_sequenceconfigPostbuild_ph);

                    # Functions for PbCfgC file generation.
                    my ($rba_I2c_externPostBuildCfg_s,$rba_I2c_PostBuildCfg_ph,$rba_I2c_HeaderFilePostBuildCfg_s ) = GenerateConfigData($rba_I2c_Slaveconfig_ph, $rba_I2c_sequenceconfigPostbuild_ph, $rba_I2c_MaxHwUnits_ph);

                    # add rba_I2c_Config_tst constants
                    (my $rba_I2c_ConfigTypeDeclaration_s,
                     my $rba_I2c_ConfigTypeDefinition_s) = GeneraterbaI2cConfigType($rba_I2c_Slaveconfig_ph);

                    # Invoke the actual generator methods
                    GenerateCfgH($path_scripts_s, $path_headers_s, \%rba_I2c_General_h,
                           $rba_I2c_SlaveId_s, $rba_I2c_ConfigTypeDeclaration_s, $rba_I2c_MaxHwUnits_ph,
                           $rba_I2c_SequenceId_s, $rba_I2c_sequenceconfigPostbuild_ph );

                    GeneratePrvCfgH($path_scripts_s, $path_headers_s, $rba_I2c_externPostBuildCfg_s,$rba_I2c_HeaderFilePostBuildCfg_s );

                    GenerateCfgC($path_scripts_s, $path_csources_s, $rba_I2c_ConfigTypeDefinition_s,
                            $rba_I2c_PostBuildCfg_ph, $rba_I2c_MaxHwUnits_ph);

                    GenerateReport($path_Reports_s, $rba_I2c_Slaveconfig_ph);
                }
                else
                {
                    $rba_I2c_GenerateEmptyFiles_s = 1;
                }
            }
            else
            {
                $rba_I2c_GenerateEmptyFiles_s = 1;
            }

            conf_process::Log("File generation finished.");

    }
    else
    {
        $rba_I2c_GenerateEmptyFiles_s = 1;
    }

    if($rba_I2c_GenerateEmptyFiles_s)
    {
        # Empty file generation for the I2C Module Disable
        GenerateEmptyFiles( $path_scripts_s, $path_headers_s, $path_csources_s);
    }

}

#**********************************************************************************************************************
# Helper methods
#**********************************************************************************************************************
#----------------------------------------------------------------------------------------------------------------------
# GenerateReport: Generates the report text
#
# Parameters:
# $_[0]: Destination directory for the generated file [IN]
# $_[1]: Reference to (already prepared) general configuration [IN]
#----------------------------------------------------------------------------------------------------------------------
sub GenerateReport
{
    # Store supplied information
    my $dirDestination_s               = $_[0];
    my $ecucConfig_ph                  = $_[1];
    #my $nameAction_s                   = (caller(1))[3] . "->" . (caller(0))[3];
    my $nameGenFile_s                  = 'rba_I2C_Report.txt';
    my $contentsGenFile_s              = '';
    $contentsGenFile_s .= "####################################################################################################\n" .
                          "#\n" .
                          "#    __   __   ___  __\n" .
                          "#   /_ / /  / /__  /    /__/\n" .
                          "#  /__/ /__/ __ / /__  /  /\n" .
                          "#\n" .
                          "#\n" .
                         "####################################################################################################\n";

    my @headline_a = ('Slave short name','Slave Address', 'Slave Connected to(HWunit)', 'Slave Connected to(logicalUnit)','Slave Speed Range');
    my @headlength_a;
    my $headsum_s = 0;
    foreach my $headword_s (@headline_a)
    {
        push @headlength_a,(length($headword_s));
    }

    # Loop over all available PostBuild configurations
    # conf_process::GetVariants returns
    # AR40: rba_I2c_ConfigSet_4Cyl, rba_I2c_ConfigSet_6Cyl
    # AR42: VPOSTBS_Variant_Master_4cyl, VPOSTBS_Variant_Master_6cyl
    foreach my $strVariant_s (conf_process::GetVariants())
    {
      # get input data for current variant
      my $PbVar = conf_process::GetConfigName($strVariant_s);

      # loop over all Slave Config
      foreach my $rba_I2c_SlaveConfigname_s (sort(keys(%{$ecucConfig_ph->{'SlaveConfigs'}})))
      {
        #check if this Slave Id is available in this PbVar
        if (exists $ecucConfig_ph->{'SlaveConfigs'}{$rba_I2c_SlaveConfigname_s}{$PbVar})
        {
          my $SlaveAddress_s               = $ecucConfig_ph->{'SlaveConfigs'}{$rba_I2c_SlaveConfigname_s}{$PbVar}{'rba_I2c_SlaveAddress'};
          my $SlaveHwconnectedto           = $ecucConfig_ph->{'SlaveConfigs'}{$rba_I2c_SlaveConfigname_s}{$PbVar}{'rba_I2c_SlaveConnModule'};
          my $SlaveLogicalConnectedto      = $ecucConfig_ph->{'SlaveConfigs'}{$rba_I2c_SlaveConfigname_s}{$PbVar}{'rba_I2c_SlaveConnLogicalModule'};
          my $Slavespeedrange_s            = "RBA_I2C_SLAVE_".$ecucConfig_ph->{'SlaveConfigs'}{$rba_I2c_SlaveConfigname_s}{$PbVar}{'rba_I2c_SlaveSpeed'};
          my @arrayinfo_a = ($rba_I2c_SlaveConfigname_s, $SlaveAddress_s, $SlaveHwconnectedto, $SlaveLogicalConnectedto, $Slavespeedrange_s);
          for(my $i_s = 0; $i_s < scalar(@headlength_a); $i_s++)
          {
            $headlength_a[$i_s] = $headlength_a[$i_s]< length($arrayinfo_a[$i_s])? length($arrayinfo_a[$i_s]):$headlength_a[$i_s];
          }
        }
      }
    }

    foreach my $count_s (@headlength_a)
    {
      $headsum_s+= $count_s;
    }
    $headsum_s = $headsum_s + (scalar(@headlength_a)*3)-1;

    # Loop over all available PostBuild configurations
    # conf_process::GetVariants returns
    # AR40: rba_I2c_ConfigSet_4Cyl, rba_I2c_ConfigSet_6Cyl
    # AR42: VPOSTBS_Variant_Master_4cyl, VPOSTBS_Variant_Master_6cyl
    foreach my $strVariant_s (conf_process::GetVariants())
    {
      # get input data for current variant
      my $PbVar = conf_process::GetConfigName($strVariant_s);

      $contentsGenFile_s .= "\n Slave signal info that configured in Dataset ".$PbVar." PbVariant \n";
      my $hypen_s = "-";
      $contentsGenFile_s .= "-" x $headsum_s."\n";
      $contentsGenFile_s.= CentreAlign(\@headline_a, \@headlength_a);

      # loop over all Slave Config
      foreach my $rba_I2c_SlaveConfigname_s (sort(keys(%{$ecucConfig_ph->{'SlaveConfigs'}})))
      {
        #check if this Slave Id is available in this PbVar
        if (exists $ecucConfig_ph->{'SlaveConfigs'}{$rba_I2c_SlaveConfigname_s}{$PbVar})
        {
          my $SlaveAddress_s                      = $ecucConfig_ph->{'SlaveConfigs'}{$rba_I2c_SlaveConfigname_s}{$PbVar}{'rba_I2c_SlaveAddress'};
          my $SlaveHwconnectedto                  = $ecucConfig_ph->{'SlaveConfigs'}{$rba_I2c_SlaveConfigname_s}{$PbVar}{'rba_I2c_SlaveConnModule'};
          my $SlaveLogicalConnectedto             = $ecucConfig_ph->{'SlaveConfigs'}{$rba_I2c_SlaveConfigname_s}{$PbVar}{'rba_I2c_SlaveConnLogicalModule'};
          my $Slavespeedrange_s                   = "RBA_I2C_SLAVE_".$ecucConfig_ph->{'SlaveConfigs'}{$rba_I2c_SlaveConfigname_s}{$PbVar}{'rba_I2c_SlaveSpeed'};
          my @arrayinfo_a = ($rba_I2c_SlaveConfigname_s, $SlaveAddress_s, $SlaveHwconnectedto, $SlaveLogicalConnectedto, $Slavespeedrange_s);
          $contentsGenFile_s .= CentreAlign(\@arrayinfo_a,\@headlength_a);
        }
      }
      $contentsGenFile_s .= "\n";
    }

    # Write the filled out template to the output file
    conf_process::Log("Writing generated file \"$nameGenFile_s\".");
    conf_process::GenerateFile($dirDestination_s . $nameGenFile_s, $contentsGenFile_s);
}

#----------------------------------------------------------------------------------------------------------------------
# Subroutine Check the post build variant availability in the I2C configuraiton
#
#----------------------------------------------------------------------------------------------------------------------
sub CheckPostbuildvariant
{
    my @postbuildshortName_a;

    # Loop over all available PostBuild configurations
    # conf_process::GetVariants returns
    # AR40: rba_I2c_ConfigSet_4Cyl, rba_I2c_ConfigSet_6Cyl
    # AR42: VPOSTBS_Variant_Master_4cyl, VPOSTBS_Variant_Master_6cyl
    foreach my $strVariant_s (conf_process::GetVariants())
    {
        # get input data for current variant
        my $ecucPostBuildConfig_ph = conf_process::GetInputData("rba_I2c", $strVariant_s)->{rba_I2c_ConfigSet};

        my $ecucrbaI2cSlaveConfig_ShortName_s = $strVariant_s;

        # check the repeat shortname for the slave
        unless ( grep {$_ eq $ecucrbaI2cSlaveConfig_ShortName_s} @postbuildshortName_a)
        {
             push(@postbuildshortName_a, $ecucrbaI2cSlaveConfig_ShortName_s);

        }
        else
        {
              conf_process::Exit(
              "#\n" .
              "#\n" .
              "#   The short name of Postbuild unit short name is repeat in  $ecucPostBuildConfig_ph \n".
              "#\n" .
              "#   configuration file: FileRef\n" .
              "#   module description: Module\n" .
              "#\n");

        }
    }
}

#----------------------------------------------------------------------------------------------------------------------
# Subroutine CentreAlign to align the vertical arrows in report file based onn the total length of each parameter
#
# Parameters:
# @_ :
#----------------------------------------------------------------------------------------------------------------------
sub CentreAlign
{
    my ($arraycol_pa, $headlength_pa) = @_;

    my $space_s = " ";
    my (@arrayali_a, @arrali_a);
    my $format_s;

   for (my $i_s = 0; $i_s < scalar(@$arraycol_pa); $i_s++)
   {
       $arrayali_a[$i_s] = $$headlength_pa[$i_s] >length($$arraycol_pa[$i_s])? int(($$headlength_pa[$i_s]+length($$arraycol_pa[$i_s]))/2):$$headlength_pa[$i_s];
       $arrali_a[$i_s]   = $$headlength_pa[$i_s]-$arrayali_a[$i_s];
       $arrali_a[$i_s]   = $space_s x $arrali_a[$i_s];
       $format_s .= "%".$arrayali_a[$i_s]."s ".$arrali_a[$i_s]."| ";
   }
   $format_s .= "\n";

   sprintf $format_s, @$arraycol_pa;
}
#----------------------------------------------------------------------------------------------------------------------
# GenerateConfigData: Generates the slave Id table information and hardware Id unit information for all datasets.
#
# Parameters:
# $_[0]: Reference to slave configuration dataset information[IN]
# $_[1]: Reference to machine data information[IN]
#----------------------------------------------------------------------------------------------------------------------
sub GenerateConfigData
{
    my $ecucConfig_ph                   = $_[0];
    my $sequenceconfigPostbuild_ph      = $_[1];
    my $rba_I2c_MaxHwUnit_ph            = $_[2];

    # Local variable.
    #---------------
    my %rba_I2c_PostBuildCfg_h;
    #my $FormatText='%-8s';
    my @rba_I2c_externPostBuildCfg_a;
    my @rba_I2c_HeaderFilePostBuildCfg_a;
    my $SequenceConfiglist_s;
    #my $sequenceconfigNotification_ph;
    my @sequenceEndNotification_extern_a;
    my @sequenceEndNotificationHeaderName_a;


    # Loop over all available PostBuild configurations
    # conf_process::GetVariants returns
    # AR40: rba_I2c_ConfigSet_4Cyl, rba_I2c_ConfigSet_6Cyl
    # AR42: VPOSTBS_Variant_Master_4cyl, VPOSTBS_Variant_Master_6cyl
    foreach my $strVariant_s (conf_process::GetVariants())
    {
        # get input data for current variant
        my $PbVar = conf_process::GetConfigName($strVariant_s);

        # get variant suffix needed to build short name of rba_I2C MCC (rba_I2c_ConfigSet)
        # conf_process::GetVariantSuffix returns
        # AR40: The variant name "rba_I2c_ConfigSet_4Cyl" will return "_4Cyl", "rba_I2c_ConfigSet" will return an empty string
        # AR42: The returned variant suffix will always be an empty string.

        # for the I2C_Pbcfg file  generation "rba_I2c_Cfg_Slave_tst" structre (slave info) configuration update.
        my $SlaveConfiglist_s = "";
        foreach my $rba_I2c_SlaveConfigname_s (sort(keys(%{$ecucConfig_ph->{'SlaveConfigs'}})))
        {
            # check if this Slave Id is available in this PbVar
            if (exists $ecucConfig_ph->{'SlaveConfigs'}{$rba_I2c_SlaveConfigname_s}{$PbVar})
            {
                $SlaveConfiglist_s .= "const rba_I2c_Cfg_Slave_tst ".$PbVar."_".$rba_I2c_SlaveConfigname_s."_st = \n{\n";
                push (@rba_I2c_externPostBuildCfg_a , "    extern const rba_I2c_Cfg_Slave_tst ".$PbVar."_".$rba_I2c_SlaveConfigname_s."_st;\n");
                $SlaveConfiglist_s .= "    $ecucConfig_ph->{'SlaveConfigs'}{$rba_I2c_SlaveConfigname_s}{$PbVar}{'rba_I2c_SlaveConnModule'},\n";
                $SlaveConfiglist_s .= "    $ecucConfig_ph->{'SlaveConfigs'}{$rba_I2c_SlaveConfigname_s}{$PbVar}{'rba_I2c_SlaveAddress'},\n";
                $SlaveConfiglist_s .= "    RBA_I2C_SLAVE_".$ecucConfig_ph->{'SlaveConfigs'}{$rba_I2c_SlaveConfigname_s}{$PbVar}{'rba_I2c_SlaveSpeed'}."\n";
                $SlaveConfiglist_s .= "};\n\n\n\n";
            }
        }
        # Create the space after the each postbuild variant
        push(@rba_I2c_externPostBuildCfg_a,"\n");

        # for the I2C_Pbcfg file  generation "rba_I2c_Cfg_Slave_tst" staructre (slave info) configuration update.
        my $firsttime_s = 1;
        my $rba_I2c_confdatadummy_s = 0;

        foreach my $rba_I2c_SlaveNum_s (sort(keys(%{$ecucConfig_ph->{'SlaveIdByNumber'}})))
        {
            $rba_I2c_confdatadummy_s = 1;
            my $rba_I2c_SlaveIdname_s = $ecucConfig_ph->{'SlaveIdByNumber'}{$rba_I2c_SlaveNum_s}{'rba_I2c_SlaveId_ShortName'};
            if (exists $ecucConfig_ph->{'SlaveConfigs'}{$rba_I2c_SlaveIdname_s}{$PbVar})
            {
                $SlaveConfiglist_s .= "const rba_I2c_Cfg_Slave_tst* const ".$PbVar."_Slave_ast[] =\n{\n" if  ($firsttime_s == 1);
                push (@rba_I2c_externPostBuildCfg_a , "    extern const rba_I2c_Cfg_Slave_tst* const ".$PbVar."_Slave_ast[]; \n"  ) if($firsttime_s == 1);
                $firsttime_s = 0;
                $SlaveConfiglist_s .= "    &".$PbVar."_".$rba_I2c_SlaveIdname_s."_st,\n";
            }
            else
            {
                $SlaveConfiglist_s .= "const rba_I2c_Cfg_Slave_tst* const ".$PbVar."_Slave_ast[] =\n{\n" if  ($firsttime_s == 1);
                push (@rba_I2c_externPostBuildCfg_a , "    extern const rba_I2c_Cfg_Slave_tst* const ".$PbVar."_Slave_ast[]; \n"  ) if($firsttime_s == 1);
                $firsttime_s = 0;
                $SlaveConfiglist_s .= "    NULL_PTR,\n";
            }
        }
        # Create the space after the each postbuild variant
        push(@rba_I2c_externPostBuildCfg_a,"\n");

        if ($rba_I2c_confdatadummy_s == 0)
        {
            $SlaveConfiglist_s .= "const rba_I2c_Cfg_Slave_tst* const ".$PbVar."_Slave_ast[] =\n{\n";
            $SlaveConfiglist_s .= "    NULL_PTR,\n";
        }

        $SlaveConfiglist_s .= "};\n\n\n";

        # for the I2C_Pbcfg file  generation "rba_I2c_Cfg_HwUnit_tst" staructre (Hardware info) configuration update.
        $firsttime_s = 1;
        $rba_I2c_confdatadummy_s = 0;

        my $rba_I2c_Available_Channels = $rba_I2c_MaxHwUnit_ph->{'Available_Channels'};

        my ($defineHwUnit_s, $tableHwUnit_s) = ("", "");

        my $rba_I2c_HwUnitPhy_s;
        for($rba_I2c_HwUnitPhy_s = 0; $rba_I2c_HwUnitPhy_s < $rba_I2c_Available_Channels; $rba_I2c_HwUnitPhy_s++ )
        {
            $rba_I2c_confdatadummy_s = 1;
            if (exists $ecucConfig_ph->{'HardwareUnits'}{'HwIdByPhyUnit'}{$rba_I2c_HwUnitPhy_s}{$PbVar})
            {
                $defineHwUnit_s .= "const rba_I2c_Cfg_HwUnit_tst ".$PbVar."_HwUnit_I2cHwUnitId_".$rba_I2c_HwUnitPhy_s."_st =\n{ \n";
                push(@rba_I2c_externPostBuildCfg_a,"    extern const rba_I2c_Cfg_HwUnit_tst ".$PbVar."_HwUnit_I2cHwUnitId_".$rba_I2c_HwUnitPhy_s."_st;\n");
                $defineHwUnit_s .= "    ".$ecucConfig_ph->{'HardwareUnits'}{'HwIdByPhyUnit'}{$rba_I2c_HwUnitPhy_s}{$PbVar}{'rba_I2c_HwId_ShortName'}.",\n";
                if (exists ($ecucConfig_ph->{'HardwareUnits'}{'HwIdByPhyUnit'}{$rba_I2c_HwUnitPhy_s}{$PbVar}{'rba_I2c_HwId_PortConnection'}))
                {
                    $defineHwUnit_s .= "    RBA_I2C_IFX_".$ecucConfig_ph->{'HardwareUnits'}{'HwIdByPhyUnit'}{$rba_I2c_HwUnitPhy_s}{$PbVar}{'rba_I2c_HwId_PortConnection'}.",\n";
                }
                $defineHwUnit_s .= "};\n\n";
                $tableHwUnit_s .= "const rba_I2c_Cfg_HwUnit_tst* const ".$PbVar."_HwUnit_ast[] =\n{\n"  if( $firsttime_s == 1 );
                push(@rba_I2c_externPostBuildCfg_a,"    extern const rba_I2c_Cfg_HwUnit_tst* const ".$PbVar."_HwUnit_ast[];\n") if ($firsttime_s == 1);
                $firsttime_s = 0;
                $tableHwUnit_s .= "    &".$PbVar."_HwUnit_I2cHwUnitId_".$rba_I2c_HwUnitPhy_s."_st,\n";
            }
            else
            {
                $tableHwUnit_s .= "const rba_I2c_Cfg_HwUnit_tst* const ".$PbVar."_HwUnit_ast[] =\n{\n" if  ($firsttime_s == 1);
                push(@rba_I2c_externPostBuildCfg_a,"    extern const rba_I2c_Cfg_HwUnit_tst* const ".$PbVar."_HwUnit_ast[];\n") if ($firsttime_s == 1);
                $firsttime_s = 0;
                $tableHwUnit_s .= "    NULL_PTR,\n";
            }

        }
        # Create the space after the each postbuild variant
        push(@rba_I2c_externPostBuildCfg_a,"\n");

        if ($rba_I2c_confdatadummy_s == 0)
        {
            $tableHwUnit_s .= "const rba_I2c_Cfg_HwUnit_tst* const ".$PbVar."_HwUnit_ast[] =\n{\n";
            push(@rba_I2c_externPostBuildCfg_a,"    extern const rba_I2c_Cfg_HwUnit_tst* const ".$PbVar."_HwUnit_ast[];\n");
            $tableHwUnit_s .= "    NULL_PTR,\n";
        }
        $tableHwUnit_s .= "};\n\n";

        $SequenceConfiglist_s = "";
        foreach my $rba_I2c_SequenceConfigname_s (sort(keys(%{$sequenceconfigPostbuild_ph->{'SequenceConfigset'}->{$PbVar}})))
        {
            # check if this Sequence Id is available in this PbVar
            if (exists $sequenceconfigPostbuild_ph->{'SequenceConfigset'}->{$PbVar}->{$rba_I2c_SequenceConfigname_s})
            {
                $SequenceConfiglist_s .= "const rba_I2c_Cfg_Sequence_tst ".$PbVar."_".$rba_I2c_SequenceConfigname_s."_st = \n{\n";

                push (@rba_I2c_externPostBuildCfg_a , "    extern const rba_I2c_Cfg_Sequence_tst ".$PbVar."_".$rba_I2c_SequenceConfigname_s."_st;\n");

                $SequenceConfiglist_s .= "    $sequenceconfigPostbuild_ph->{'SequenceConfigset'}->{$PbVar}->{$rba_I2c_SequenceConfigname_s}->{'SequenceEndNotification'},\n";

                my $seqEndNoti_s = $sequenceconfigPostbuild_ph->{'SequenceConfigset'}->{$PbVar}->{$rba_I2c_SequenceConfigname_s}->{'SequenceEndNotification'};

                # Header file name inclusion where end notification has been declared
                my $seqEndNotiHeaderName_s = $sequenceconfigPostbuild_ph->{'SequenceConfigset'}->{$PbVar}->{$rba_I2c_SequenceConfigname_s}->{'HeaderFileName'};

                if(($seqEndNoti_s ne "NULL_PTR")&& (!(grep /$seqEndNoti_s/, @sequenceEndNotification_extern_a)))
                {
                  if($seqEndNotiHeaderName_s ne "NULL")
                  {
                        if(!(grep /$seqEndNotiHeaderName_s/, @sequenceEndNotificationHeaderName_a))
                       {
                          push(@sequenceEndNotificationHeaderName_a , $seqEndNotiHeaderName_s);
                       }
                       else
                       {
                           #do nothing since the header file is already included
                       }
                  }
                  else
                  {
                    push(@sequenceEndNotification_extern_a , $seqEndNoti_s);
                  }
                }


                $SequenceConfiglist_s .= "    $sequenceconfigPostbuild_ph->{'SequenceConfigset'}{$PbVar}{$rba_I2c_SequenceConfigname_s}{'SequencePriority'},\n";
                $SequenceConfiglist_s .= "    $sequenceconfigPostbuild_ph->{'SequenceConfigset'}{$PbVar}{$rba_I2c_SequenceConfigname_s}{'SequenceInterface'},\n";
                $SequenceConfiglist_s .= "    $sequenceconfigPostbuild_ph->{'SequenceConfigset'}{$PbVar}{$rba_I2c_SequenceConfigname_s}{'SequenceTxLength'},\n";
                $SequenceConfiglist_s .= "    $sequenceconfigPostbuild_ph->{'SequenceConfigset'}{$PbVar}{$rba_I2c_SequenceConfigname_s}{'SequenceRxLength'},\n";
                $SequenceConfiglist_s .= "    rba_I2c_$sequenceconfigPostbuild_ph->{'SequenceConfigset'}{$PbVar}{$rba_I2c_SequenceConfigname_s}{'SlaveConfigId'}\n";
                $SequenceConfiglist_s .= "};\n\n\n\n";
            }
        }
        # Create the space after the each postbuild variant
        push(@rba_I2c_externPostBuildCfg_a,"\n");

        #   loop over all Sequence Id to create an array of pointer to each Sequence config that is configured gor that PbVar
        #print "I2c confdata null failed\n";
        my $sequenceMax_Size = $sequenceconfigPostbuild_ph->{'maxSequenceConnt'};
        my @sequence_arrypointer_a;
        for(my $var=0; $var<$sequenceMax_Size; $var++)
        {
            $sequence_arrypointer_a[$var]="    NULL_PTR,\n";
        }

        $SequenceConfiglist_s .= "const rba_I2c_Cfg_Sequence_tst* const ".$PbVar."_Sequence_ast[] =\n{\n";
        push(@rba_I2c_externPostBuildCfg_a,"    extern const rba_I2c_Cfg_Sequence_tst* const ".$PbVar."_Sequence_ast[]; \n");

        foreach my $rba_I2c_SequenceConfigname_s (sort(keys(%{$sequenceconfigPostbuild_ph->{'SequenceConfigset'}->{$PbVar}})))
        {
            if (exists $sequenceconfigPostbuild_ph->{'SequenceConfigset'}->{$PbVar}->{$rba_I2c_SequenceConfigname_s}->{'rba_I2cSequenceConfig_Number'})
            {
                 my $SequenceConfig_Num = $sequenceconfigPostbuild_ph->{'SequenceConfigset'}->{$PbVar}->{$rba_I2c_SequenceConfigname_s}->{'rba_I2cSequenceConfig_Number'};
                 $sequence_arrypointer_a[$SequenceConfig_Num] = "    &".$PbVar."_".$rba_I2c_SequenceConfigname_s."_st,\n";
            }

        }

        for(my $var=0; $var<$sequenceMax_Size; $var++)
        {
            $SequenceConfiglist_s .= "    $sequence_arrypointer_a[$var]";
        }
        $SequenceConfiglist_s .= "};\n\n";


        # Assign the generated output for each PbVar
        $rba_I2c_PostBuildCfg_h{$PbVar} = $SlaveConfiglist_s.$defineHwUnit_s.$tableHwUnit_s.$SequenceConfiglist_s;

        # Create the space after the each postbuild varieant
        push(@rba_I2c_externPostBuildCfg_a,"\n\n");

    }

    push(@rba_I2c_externPostBuildCfg_a,"\n\n");

    #Handle end notification function

        foreach my $HeaderFileInclusion (@sequenceEndNotificationHeaderName_a)
        {
            # Header file should be included if HeaderName configuration tag is configured
            $HeaderFileInclusion =~ s/\s+//g; # Remove whitespaces if present
            $HeaderFileInclusion ="#include \"" .$HeaderFileInclusion.".h\"\n";
            push(@rba_I2c_HeaderFilePostBuildCfg_a,$HeaderFileInclusion);
        }

       # push EndNotification function reference to Header file
        foreach my $externNotifi (@sequenceEndNotification_extern_a)
        {
            $externNotifi =~ s/&//;
            $externNotifi = "    extern void ".$externNotifi. "(void); \n";
            push(@rba_I2c_externPostBuildCfg_a,$externNotifi);
        }


    return (\@rba_I2c_externPostBuildCfg_a, \%rba_I2c_PostBuildCfg_h,\@rba_I2c_HeaderFilePostBuildCfg_a);
}

#----------------------------------------------------------------------------------------------------------------------
# GetMachine Type: Subroutine to read the machine type from the MCU
#
#----------------------------------------------------------------------------------------------------------------------
sub GetMachineType
{
    my $xMachineSubType_s;
    my $xMachineType_s;

    # Read the Mcu information
    my $ecucMcu_ph = conf_process::GetInvariantInputData("Mcu");

    # Read the Machine type information
    if (   (exists($ecucMcu_ph->{McuRbCpuInformation}))
        && (exists($ecucMcu_ph->{McuRbCpuInformation}{McuRbMachineDev}))
        && (exists($ecucMcu_ph->{McuRbCpuInformation}{McuRbMachineDev}{content}))
        && (defined($ecucMcu_ph->{McuRbCpuInformation}{McuRbMachineDev}{content})))
    {
        $xMachineType_s = $ecucMcu_ph->{McuRbCpuInformation}{McuRbMachineDev}{content};
    }
    else
    {
        conf_process::Exit("ERROR in configuration script of rba_I2c: CPU info (machine type) not found.");
    }

    # Read the Machine subtype information
    if (   (exists($ecucMcu_ph->{McuRbCpuInformation}))
        && (exists($ecucMcu_ph->{McuRbCpuInformation}{McuRbMachineSubType}))
        && (exists($ecucMcu_ph->{McuRbCpuInformation}{McuRbMachineSubType}{content}))
        && (defined($ecucMcu_ph->{McuRbCpuInformation}{McuRbMachineSubType}{content})))
    {
        $xMachineSubType_s = $ecucMcu_ph->{McuRbCpuInformation}{McuRbMachineSubType}{content};
    }
    else
    {
        conf_process::Exit("ERROR in configuration script of rba_I2c: CPU info (machine sub type) not found.");
    }

    return ($xMachineType_s,$xMachineSubType_s);
}

#**********************************************************************************************************************
# Generator methods
#**********************************************************************************************************************

#----------------------------------------------------------------------------------------------------------------------
# GenerateCfgH: Generates the pre-compile time header file
#
# Parameters:
# $_[0]: Source directory of the template file for this generator [IN]
# $_[1]: Destination directory for the generated file [IN]
# $_[2]: Reference to (already prepared) general configuration [IN]
#----------------------------------------------------------------------------------------------------------------------
sub GenerateCfgH
{
    # Store supplied information
    my $dirSource_s                     = $_[0];
    my $dirDestination_s                = $_[1];
    my $ecucGeneral_ph                  = $_[2];
    my $rba_I2c_SlaveId_s               = $_[3];
    my $rba_I2c_ConfigTypeDeclaration_s = $_[4];
    my $rba_I2c_MaxHwUnit_ph            = $_[5];
    my $rba_I2c_SequenceId_s            = $_[6];
    my $sequenceconfigPostbuild_ph      = $_[7];
    my $nameAction_s                    = (caller(1))[3] . "->" . (caller(0))[3];
    my $nameTemplateFile_s              = 'rba_I2C_Cfg.ht';
    my $nameGenFile_s                   = 'rba_I2C_Cfg.h';
    my $contentsGenFile_s               = '';
    my $rba_I2c_GeneralDefines_s        = '';
    my $rba_I2c_FunctionList_s          = '';
    my $rba_I2c_ModuleEnable = 0;
    my $AR_Version_s = '';

    # Load the template file
    conf_process::Log("Loading template file \"$nameTemplateFile_s\".");

    open(TEMPLATE, '<' . $dirSource_s . $nameTemplateFile_s) or conf_process::Exit("ERROR ($nameAction_s): Template file \"$nameTemplateFile_s\" not found.");
    {
        # Read entire file at once
        local $/;
        $contentsGenFile_s = <TEMPLATE>;
    }
    close(TEMPLATE);

    # create file content if a configuration is present
    if ( (defined $rba_I2c_MaxHwUnit_ph) && ( $rba_I2c_MaxHwUnit_ph ne '') )
    {
        if ($rba_I2c_MaxHwUnit_ph->{'MaxConfigured_Channels'} > 0)
        {
            $rba_I2c_ModuleEnable = 1;
            # add module enabler
            $rba_I2c_GeneralDefines_s .= "/*-------------------------\nrba_I2c Module enabler\n-------------------------*/\n";
            $rba_I2c_GeneralDefines_s .= CreateDefine( "RBA_I2C_CFG_MODULE_ACTIVE", 'STD_ON');
            $rba_I2c_GeneralDefines_s .= "\n";
            # add Header
            $rba_I2c_GeneralDefines_s .= "/*-------------------------\nrba_I2c_General\n-------------------------*/\n";


            # Generate Autosar Version

            # Get Autosar Version info based on ECUC values
            my $AR_Version_ph = GetAutosarVersion();

            $AR_Version_s .= CreateDefine('RBA_I2C_PRV_AR_VERSION_MAJOR'    ,$AR_Version_ph->{MAJOR});
            $AR_Version_s .= CreateDefine('RBA_I2C_PRV_AR_VERSION_MINOR'    ,$AR_Version_ph->{MINOR});
            $AR_Version_s .= CreateDefine('RBA_I2C_PRV_AR_VERSION_REVISION' ,$AR_Version_ph->{REVISION});

            $contentsGenFile_s =~ s/<\/RBA_I2C_PRV_AR_VERSION\/>/$AR_Version_s/;

            # Fill out the rba_I2c_General setting
            $rba_I2c_GeneralDefines_s .= CreateDefine( "RBA_I2C_CFG_CHECK_AUTOSAR_VERSION", (($ecucGeneral_ph->{'rba_I2c_VersionInfoApi'} != 0) ? 'STD_ON' : 'STD_OFF'));
            $rba_I2c_GeneralDefines_s .= CreateDefine( "RBA_I2C_CFG_DEV_ERROR_DETECT", (($ecucGeneral_ph->{'rba_I2c_DevErrorDetect'} != 0) ? 'STD_ON' : 'STD_OFF'));
            $rba_I2c_GeneralDefines_s .= CreateDefine( "RBA_I2C_CFG_VERSION_INFO_API", (($ecucGeneral_ph->{'rba_I2c_VersionInfoApi'} != 0) ? 'STD_ON' : 'STD_OFF'));



            $rba_I2c_GeneralDefines_s .= CreateDefine( "RBA_I2C_CFG_AVAILABLE_CHANNELS", "$rba_I2c_MaxHwUnit_ph->{'Available_Channels'}"."u");
            $rba_I2c_GeneralDefines_s .= CreateDefine( "RBA_I2C_CFG_MAX_CONFIGURED_CHANNELS", "$rba_I2c_MaxHwUnit_ph->{'MaxConfigured_Channels'}"."u");

            foreach my $Ava_chlannels (keys (%{$rba_I2c_MaxHwUnit_ph->{'Channel_list'}}))
            {
                $rba_I2c_GeneralDefines_s .= CreateDefine( $Ava_chlannels, "$rba_I2c_MaxHwUnit_ph->{'Channel_list'}{ $Ava_chlannels}"."u");
            }

            $rba_I2c_GeneralDefines_s .= CreateDefine( "RBA_I2C_CFG_QUEUE_LENGTH", "$sequenceconfigPostbuild_ph->{'maxSequenceConnt'}"."u");
            $rba_I2c_GeneralDefines_s .= "\n";


            #   list of functions
            $rba_I2c_FunctionList_s = join("\n",
                    '/*!',
                    ' ***************************************************************************************************',
                    ' \brief Controller specific function mapping.',
                    ' ***************************************************************************************************',
                    ' */',
                    '' # to get the last line feed
                    );
            $rba_I2c_FunctionList_s .= '// QAC: Suppress WARNING: <=4=(3453) A function could probably be used instead of this function-like macro. MISRA-C:2004 Rule 19.7' . "\n";
            $rba_I2c_FunctionList_s .= '// QAC: Reason: Macros used for function mapping.' . "\n";
            $rba_I2c_FunctionList_s .= '// PRQA S 3453 RBA_I2C_END_3453_SUPRESSION' . "\n";

        }
    }
    else
    {
        $rba_I2c_GeneralDefines_s .= "/*-------------------------\nrba_I2c Module enabler\n-------------------------*/\n";
        $rba_I2c_GeneralDefines_s .= CreateDefine( "RBA_I2C_CFG_MODULE_ACTIVE", 'STD_OFF');
        $rba_I2c_GeneralDefines_s .= "\n";
    }

    if($rba_I2c_ModuleEnable == 1)
    {
        # Insert the generated configuration into template
        $contentsGenFile_s =~ s/<\/RBA_I2C_GENERAL_CONFIG\/>/$rba_I2c_GeneralDefines_s/;
        $contentsGenFile_s =~ s/<\/RBA_I2C_SLAVEID_DEFINES\/>/$rba_I2c_SlaveId_s/;
        $contentsGenFile_s =~ s/<\/RBA_I2C_SEQUENCEID_DEFINES\/>/$rba_I2c_SequenceId_s/;
        $contentsGenFile_s =~ s/<\/RBA_I2C_FUNCTION_DEFINES\/>/$rba_I2c_FunctionList_s/;
        $contentsGenFile_s =~ s/<\/RBA_I2C_PBCONFIG_CONTAINER\/>/$rba_I2c_ConfigTypeDeclaration_s/;
    }
    else
    {
        $rba_I2c_SlaveId_s = '';
        $rba_I2c_FunctionList_s = '';
        $rba_I2c_ConfigTypeDeclaration_s = '';

        # Insert the generated configuration into template
        $contentsGenFile_s =~ s/<\/RBA_I2C_GENERAL_CONFIG\/>/$rba_I2c_GeneralDefines_s/;
        $contentsGenFile_s =~ s/<\/RBA_I2C_SLAVEID_DEFINES\/>/$rba_I2c_SlaveId_s/;
        $contentsGenFile_s =~ s/<\/RBA_I2C_SEQUENCEID_DEFINES\/>/$rba_I2c_SequenceId_s/;
        $contentsGenFile_s =~ s/<\/RBA_I2C_FUNCTION_DEFINES\/>/$rba_I2c_FunctionList_s/;
        $contentsGenFile_s =~ s/<\/RBA_I2C_PBCONFIG_CONTAINER\/>/$rba_I2c_ConfigTypeDeclaration_s/;
    }

    # Write the filled out template to the output file
    conf_process::Log("Writing generated file \"$nameGenFile_s\".");
    conf_process::GenerateFile($dirDestination_s . $nameGenFile_s, $contentsGenFile_s);
}

#----------------------------------------------------------------------------------------------------------------------
# GeneratePrvCfgH: Generates the pre-compile time header file
#
# Parameters:
# $_[0]: Source directory of the template file for this generator [IN]
# $_[1]: Destination directory for the generated file [IN]
# $_[2]: Reference to (already prepared) general configuration [IN]
#----------------------------------------------------------------------------------------------------------------------
sub GeneratePrvCfgH
{
    # Store supplied information
    my $dirSource_s                     = $_[0];
    my $dirDestination_s                = $_[1];
    my $rba_I2c_externPostBuildCfg_s    = $_[2];
    my $rba_I2c_HeaderFilePostBuildCfg_s= $_[3];
    my $nameAction_s                    = (caller(1))[3] . "->" . (caller(0))[3];
    my $nameTemplateFile_s              = 'rba_I2C_Cfg_Prv.ht';
    my $nameGenFile_s                   = 'rba_I2C_Cfg_Prv.h';
    my $contentsGenFile_s               = '';
    my $PrvCfgH_externPostBuildCfg_s;
    my $PrvCfgH_HeaderFilePostBuildCfg_s;


    # Load the template file
    conf_process::Log("Loading template file \"$nameTemplateFile_s\".");

    open(TEMPLATE, '<' . $dirSource_s . $nameTemplateFile_s) or conf_process::Exit("ERROR ($nameAction_s): Template file \"$nameTemplateFile_s\" not found.");
    {
        # Read entire file at once
        local $/;
        $contentsGenFile_s = <TEMPLATE>;
    }
    close(TEMPLATE);

    $PrvCfgH_externPostBuildCfg_s.= "@$rba_I2c_externPostBuildCfg_s";
    $PrvCfgH_HeaderFilePostBuildCfg_s.= "@$rba_I2c_HeaderFilePostBuildCfg_s";

    # Insert the generated configuration into template
    $contentsGenFile_s =~ s/<\/RBA_I2C_EXT_END_NOTIF\/>/$PrvCfgH_externPostBuildCfg_s/;

     # Insert the generated header files into template
    $contentsGenFile_s =~ s/<\/RBA_I2C_INCLUDE_FILES\/>/$PrvCfgH_HeaderFilePostBuildCfg_s/;

    # Write the filled out template to the output file
    conf_process::Log("Writing generated file \"$nameGenFile_s\".");
    conf_process::GenerateFile($dirDestination_s . $nameGenFile_s, $contentsGenFile_s);
}

#----------------------------------------------------------------------------------------------------------------------
# GenerateCfgC: Generates the pre-compile time header file
#
# Parameters:
# $_[0]: Source directory of the template file for this generator [IN]
# $_[1]: Destination directory for the generated c-file [IN]
# $_[2]: Destination directory for the generated h-file [IN]
# $_[3]: Reference to (already prepared) general configuration [IN]
#----------------------------------------------------------------------------------------------------------------------

sub GenerateCfgC
{
  my $dirSource_s                       = $_[0];
  my $dirCFiles_s                       = $_[1];
  my $rba_I2c_ConfigTypeDefinition_s    = $_[2];
  my $rba_I2c_PostBuildCfg_ph           = $_[3];
  my $rba_I2c_MaxHwUnit_ph              = $_[4];

  my $nameTemplateFile_s                = 'rba_I2C_PbCfg.ct';
  my $nameGenFile_s                     = 'rba_I2C_PbCfg.c';
  my $contentsGenFile_s                 = '';
  my $nameAction_s                      = (caller(1))[3] . "->" . (caller(0))[3];

  # Load the template file
  conf_process::Log("Loading template file \"$nameTemplateFile_s\".");

  #create single string out of different post build variants
  my $rba_I2c_PostBuildCfg_s = "";

  if(defined $rba_I2c_MaxHwUnit_ph)
  {
      if($rba_I2c_MaxHwUnit_ph->{'MaxConfigured_Channels'} > 0)
      {
        if (defined $rba_I2c_PostBuildCfg_s)
        {
          foreach my $rba_I2c_PostBuildVar_s (sort(keys(%{$rba_I2c_PostBuildCfg_ph})))
          {
            $rba_I2c_PostBuildCfg_s .= $rba_I2c_PostBuildCfg_ph->{$rba_I2c_PostBuildVar_s};
          }
        }
      }
      else
      {
        $rba_I2c_ConfigTypeDefinition_s = "";
      }
  }
  else
  {
      $rba_I2c_ConfigTypeDefinition_s = "";
  }

  open(TEMPLATE, '<' . $dirSource_s . $nameTemplateFile_s) or conf_process::Exit("ERROR ($nameAction_s): Template file \"$nameTemplateFile_s\" not found.");
  {
    # Read entire file at once
    local $/;
    $contentsGenFile_s = <TEMPLATE>;
  }
  close(TEMPLATE);

  # Insert the generated configuration into template
  $contentsGenFile_s =~ s/<\/RBA_I2C_POSTBUILD_ARRAYS\/>/$rba_I2c_PostBuildCfg_s/;
  $contentsGenFile_s =~ s/<\/RBA_I2C_PBCONFIG_CONTAINER\/>/$rba_I2c_ConfigTypeDefinition_s/;

  # Write the filled out template to the output file
  conf_process::Log("Writing generated file \"$nameGenFile_s\".");
  conf_process::GenerateFile($dirCFiles_s . $nameGenFile_s, $contentsGenFile_s);
}

#----------------------------------------------------------------------------------------------------------------------
# GeneratePrvCfgH: Generates the pre-compile time header file
#
# Parameters:
# $_[0]: Source directory of the template file for this generator [IN]
# $_[1]: Destination directory for the generated file [IN]
# $_[2]: Reference to (already prepared) general configuration [IN]
#----------------------------------------------------------------------------------------------------------------------
sub GenerateEmptyFiles
{

    # Store supplied information
    my $dirSource_s                     = $_[0];
    my $HeaderdirDestination_s          = $_[1];
    my $SourcedirDestination_s          = $_[1];
    my $nameTemplateFile_s;
    my $nameGenFile_s;
    my $contentsGenFile_s               = '';
    my $contentReplace;
    my $rba_I2c_ModuleEnable;
    my $AR_Version_s = '';

    # rba_I2C_Cfg_Prv.h Empty file generation.
    #------------------------------------------
    $nameTemplateFile_s              = 'rba_I2C_Cfg.ht';
    $nameGenFile_s                   = 'rba_I2C_Cfg.h';

    # Load the template file
    conf_process::Log("Loading template file \"$nameTemplateFile_s\".");

    open(TEMPLATE, '<' . $dirSource_s . $nameTemplateFile_s) or conf_process::Exit("ERROR Template file \"$nameTemplateFile_s\" not found.");
    {
        # Read entire file at once
        local $/;
        $contentsGenFile_s = <TEMPLATE>;
    }
    close(TEMPLATE);

    $contentReplace = "";

    $rba_I2c_ModuleEnable .= "/*-------------------------\nrba_I2c Module enabler\n-------------------------*/\n";
    $rba_I2c_ModuleEnable .= CreateDefine( "RBA_I2C_CFG_MODULE_ACTIVE", 'STD_OFF');
    $rba_I2c_ModuleEnable .= "\n";

    # Generate Autosar Version

    # Get Autosar Version info based on ECUC values
    my $AR_Version_ph = GetAutosarVersion();

    $AR_Version_s .= CreateDefine('RBA_I2C_PRV_AR_VERSION_MAJOR'    ,$AR_Version_ph->{MAJOR});
    $AR_Version_s .= CreateDefine('RBA_I2C_PRV_AR_VERSION_MINOR'    ,$AR_Version_ph->{MINOR});
    $AR_Version_s .= CreateDefine('RBA_I2C_PRV_AR_VERSION_REVISION' ,$AR_Version_ph->{REVISION});

    # Insert the generated configuration into template
    $contentsGenFile_s =~ s/<\/RBA_I2C_PRV_AR_VERSION\/>/$AR_Version_s/;
    $contentsGenFile_s =~ s/<\/RBA_I2C_GENERAL_CONFIG\/>/$rba_I2c_ModuleEnable/;
    $contentsGenFile_s =~ s/<\/RBA_I2C_SLAVEID_DEFINES\/>/$contentReplace/;
    $contentsGenFile_s =~ s/<\/RBA_I2C_SEQUENCEID_DEFINES\/>/$contentReplace/;
    $contentsGenFile_s =~ s/<\/RBA_I2C_FUNCTION_DEFINES\/>/$contentReplace/;
    $contentsGenFile_s =~ s/<\/RBA_I2C_PBCONFIG_CONTAINER\/>/$contentReplace/;


    # Write the filled out template to the output file
    conf_process::Log("Writing generated file \"$nameGenFile_s\".");
    conf_process::GenerateFile($HeaderdirDestination_s . $nameGenFile_s, $contentsGenFile_s);

    # rba_I2C_Cfg_Prv.h Empty file generation.
    #------------------------------------------
    $nameTemplateFile_s              = 'rba_I2C_Cfg_Prv.ht';
    $nameGenFile_s                   = 'rba_I2C_Cfg_Prv.h';

    # Load the template file
    conf_process::Log("Loading template file \"$nameTemplateFile_s\".");

    open(TEMPLATE, '<' . $dirSource_s . $nameTemplateFile_s) or conf_process::Exit("ERROR Template file \"$nameTemplateFile_s\" not found.");
    {
        # Read entire file at once
        local $/;
        $contentsGenFile_s = <TEMPLATE>;
    }
    close(TEMPLATE);

    $contentReplace = "";

    # Insert the generated configuration into template
    $contentsGenFile_s =~ s/<\/RBA_I2C_EXT_END_NOTIF\/>/$contentReplace/;
    
	# Insert the generated configuration into template (Header file inclusion section)
    $contentsGenFile_s =~ s/<\/RBA_I2C_INCLUDE_FILES\/>/$contentReplace/;

    # Write the filled out template to the output file
    conf_process::Log("Writing generated file \"$nameGenFile_s\".");
    conf_process::GenerateFile($HeaderdirDestination_s . $nameGenFile_s, $contentsGenFile_s);

    # rba_I2C_Cfg_Prv.h Empty file generation.
    #------------------------------------------
    $nameTemplateFile_s              = 'rba_I2C_PbCfg.ct';
    $nameGenFile_s                   = 'rba_I2C_PbCfg.c';

    # Load the template file
    conf_process::Log("Loading template file \"$nameTemplateFile_s\".");

    open(TEMPLATE, '<' . $dirSource_s . $nameTemplateFile_s) or conf_process::Exit("ERROR Template file \"$nameTemplateFile_s\" not found.");
    {
        # Read entire file at once
        local $/;
        $contentsGenFile_s = <TEMPLATE>;
    }
    close(TEMPLATE);

    $contentReplace = "";

    # Insert the generated configuration into template
    $contentsGenFile_s =~ s/<\/RBA_I2C_POSTBUILD_ARRAYS\/>/$contentReplace/;
    $contentsGenFile_s =~ s/<\/RBA_I2C_PBCONFIG_CONTAINER\/>/$contentReplace/;

    # Write the filled out template to the output file
    conf_process::Log("Writing generated file \"$nameGenFile_s\".");
    conf_process::GenerateFile($SourcedirDestination_s . $nameGenFile_s, $contentsGenFile_s);

    # rba_I2C_Cfg_Prv.h Empty file generation.
    #------------------------------------------
    conf_process::GenerateFile($SourcedirDestination_s . "rba_I2C_Report.txt", "No rba_I2c configuration present");
}

#**********************************************************************************************************************
# Helper methods
#**********************************************************************************************************************
#----------------------------------------------------------------------------------------------------------------------
# WhoAmI: Returns the module name in CamleText
#----------------------------------------------------------------------------------------------------------------------
sub WhoAmI
{
  return 'rba_I2c';
}

#**********************************************************************************************************************
# Helper methods
#**********************************************************************************************************************

#----------------------------------------------------------------------------------------------------------------------
# GetGeneral: Checks the validity of the general configuration data and stores it in a more efficient structure
#
#----------------------------------------------------------------------------------------------------------------------

sub GetGeneral
{
    my %result_h;
    my $nameAction_s = (caller(1))[3] . "->" . (caller(0))[3];

    # Get the invariant input data for the given module. The invariant input data is returned as a reference to
    # a hash containing the input data.
    # In case there is no invariant input data available the returned hash reference is undefined.
    my $ecucGeneral_ph = conf_process::GetInvariantInputData("rba_I2c")->{'rba_I2c_General'};

    # Validate the I2C general configuration from ECUC values
    if(!defined($ecucGeneral_ph))
    {
        $result_h{'rba_I2c_DevErrorDetect'} = 0;
        $result_h{'rba_I2c_VersionInfoApi'} = 0;
    }
    else
    {
        # Validate and store the rba_I2c_DevErrorDetect switch
        if (   ( !defined($ecucGeneral_ph->{'rba_I2c_DevErrorDetect'}->{'content'}))
            || ( $ecucGeneral_ph->{'rba_I2c_DevErrorDetect'}->{'content'} eq '' ) )
        {
            $result_h{'rba_I2c_DevErrorDetect'} = '0';
        }
        else
        {
            $result_h{'rba_I2c_DevErrorDetect'} = GetBoolean($ecucGeneral_ph->{'rba_I2c_DevErrorDetect'}->{'content'}, 'rba_I2c_DevErrorDetect', 'rba_I2c_General');
        }


        # Validate and store the rba_I2c_VersionInfoApi switch
        if ( ( !defined($ecucGeneral_ph->{'rba_I2c_VersionInfoApi'}->{'content'}))
            || ( $ecucGeneral_ph->{'rba_I2c_VersionInfoApi'}->{'content'} eq '' ) )
        {
            $result_h{'rba_I2c_VersionInfoApi'} = '0';
        }
        else
        {
            $result_h{'rba_I2c_VersionInfoApi'} = GetBoolean($ecucGeneral_ph->{'rba_I2c_VersionInfoApi'}->{'content'}, 'rba_I2c_VersionInfoApi', 'rba_I2c_General');
        }
    }
    return %result_h;
}

#----------------------------------------------------------------------------------------------------------------------
# GetSlaveConfig: Checks the validity of the rba_I2c_ConfigSet configuration data and stores it in a more efficient structure
#
# Parameters:
# $_[0]: Machine type [IN]
#
#----------------------------------------------------------------------------------------------------------------------
sub GetSlaveConfig
{
    my $MachineType_s           = $_[0];

    # validate the machine type.
    $MachineType_s =~ /^MCU_RB\_([A-Za-z0-9]+_UC1)([A-Za-z0-9_]+)$/;

    my %result_h;

    my $nameAction_s = (caller(1))[3] . "->" . (caller(0))[3];

    my $rba_I2c_SlaveConfigNumber_global = 0;
    my $slaveConfigsetPostbuild_ph;

    # Loop over all available PostBuild configurations
    # conf_process::GetVariants returns
    # AR40: rba_I2c_ConfigSet_4Cyl, rba_I2c_ConfigSet_6Cyl
    # AR42: VPOSTBS_Variant_Master_4cyl, VPOSTBS_Variant_Master_6cyl
    foreach my $strVariant_s (conf_process::GetVariants())
    {
        # get input data for current variant
        my $ecucPostBuildConfig_ph = conf_process::GetInputData("rba_I2c", $strVariant_s)->{rba_I2c_ConfigSet};

        if(!defined($ecucPostBuildConfig_ph->{'rba_I2c_SlaveConfig'} ))
        {
            conf_process::Exit(
                "#\n" .
                "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                "#\n" .
                "#   Slave is not configured in the  $strVariant_s postbuild configuration dataset  !\n" .
                "#\n" .
                "#   configuration file: FileRef\n" .
                "#   module description: Module\n" .
                "#\n");
        }
        else
        {
            my $ecucPbCfg_Name_s = conf_process::GetConfigName($strVariant_s);

            my $ecucPbCfg_FileRef_s   = $ecucPostBuildConfig_ph->{'FileRef'};

            #counter for number of used Slave Configs
            my ($rba_I2c_SlaveConfigCnt_s, $rba_I2c_HwUnitCnt_s) = (0,0);

            # Loop over all available slave config configurations
            my ($rba_I2cSlaveConfigNumber, $rba_I2c_HwUnitNumber);

            my $slaveConfigset_ph = {};
            my @slaveShortName_a;

            foreach my $ecucrbaI2cSlaveConfig_ph (@{$ecucPostBuildConfig_ph->{'rba_I2c_SlaveConfig'}})
            {
                my $ecucrbaI2cSlaveConfig_ShortName_s;
                my $ecucrbaI2cSlaveConfig_SlaveSpeed_s;
                my $ecucrbaI2cSlaveConfig_FileRef_s;
                my $ecucrbaI2cSlaveConfig_rbaI2cSlaveaddress_s;
                my $ecucrbaI2cSlaveConfig_rbaI2cSlaveConnect_s;
                my $slaveconfig_ph;

                # Validate the shortname
                if ( ( !defined($ecucrbaI2cSlaveConfig_ph->{'rba_I2c_SlaveConfig__KEY'}->{'content'}))
                    || ( $ecucrbaI2cSlaveConfig_ph->{'rba_I2c_SlaveConfig__KEY'}->{'content'} eq '' ) )
                {
                    conf_process::Exit(
                    "#\n" .
                    "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                    "#\n" .
                    "#   The Slave Config Slave name is not configured $ecucPbCfg_Name_s postbuild variant \n" .
                    "#\n" .
                    "#   configuration file: FileRef\n" .
                    "#   module description: Module\n" .
                    "#\n");
                }
                else
                {
                    # remove internal whitespaces
                    if($ecucrbaI2cSlaveConfig_ph->{'rba_I2c_SlaveConfig__KEY'}->{'content'} =~ /\s+/)
                    {
                        conf_process::Exit(
                            "#\n" .
                            "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                            "#\n" .
                            "#      The Slave Config Slave name is configured with blankspace $ecucrbaI2cSlaveConfig_ph->{'rba_I2c_SlaveConfig__KEY'}->{'content'} \n" .
                            "#\n" .
                            "#   configuration file: FileRef\n" .
                            "#   module description: Module\n" .
                            "#\n");
                    }
                    else
                    {
                        $ecucrbaI2cSlaveConfig_ShortName_s = $ecucrbaI2cSlaveConfig_ph->{'rba_I2c_SlaveConfig__KEY'}{'content'};

                    }

                }
                $slaveconfig_ph->{'ShortName'}=$ecucrbaI2cSlaveConfig_ShortName_s;


                # check the repeat shortname for the slave
                unless ( grep {$_ eq $ecucrbaI2cSlaveConfig_ShortName_s} @slaveShortName_a)
                {
                    push(@slaveShortName_a, $ecucrbaI2cSlaveConfig_ShortName_s);

                }
                else
                {
                    conf_process::Exit(
                        "#\n" .
                        "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                        "#\n" .
                        "#   The short name of IFX-hardware unit short name is repeat in  $ecucPostBuildConfig_ph \n".
                        "#\n" .
                        "#   configuration file: FileRef\n" .
                        "#   module description: Module\n" .
                        "#\n");

                }


                # Validate the shortname
                if (   ( !exists($ecucrbaI2cSlaveConfig_ph->{'rba_I2c_SlaveAddress'}))
                    || ( !exists($ecucrbaI2cSlaveConfig_ph->{'rba_I2c_SlaveAddress'}->{'content'}))
                    || ( !defined($ecucrbaI2cSlaveConfig_ph->{'rba_I2c_SlaveAddress'}->{'content'}))
                    || ( $ecucrbaI2cSlaveConfig_ph->{'rba_I2c_SlaveAddress'}->{content} eq '' ) )
                {
                    conf_process::Exit(
                        "#\n" .
                        "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                        "#\n" .
                        "#   The Slave Address is not configured in the  $ecucrbaI2cSlaveConfig_ph \n" .
                        "#\n" .
                        "#   configuration file: FileRef\n" .
                        "#   module description: Module\n" .
                        "#\n");

                }
                else
                {
                    $ecucrbaI2cSlaveConfig_rbaI2cSlaveaddress_s = $ecucrbaI2cSlaveConfig_ph->{'rba_I2c_SlaveAddress'}{'content'};

                    # Validate the slave address
                    # remove internal whitespaces
                    $ecucrbaI2cSlaveConfig_rbaI2cSlaveaddress_s =~ s/\s+//g;

                    # get possible hex value
                    if ($ecucrbaI2cSlaveConfig_rbaI2cSlaveaddress_s =~ m/^0x([0-9a-f]{1,2})$/i)
                    {

                        my $slaveaddressinteger_s = hex($1);

                        #vlidate the I2C-slave address value max limit (0xFE)
                        if ($slaveaddressinteger_s > 0xFE)
                        {
                            conf_process::Exit(
                                "#\n" .
                                "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                                "#\n" .
                                "#   The slave address is exceeds I2C address limit.  1to7 bit slave address and 0 bit is read/write  bit
                                1to7 Slave address refer from the User manuel and read/write control bit set to 0.
                                range for the slave $slaveconfig_ph->{'ShortName'}
                                in $ecucPbCfg_Name_s PbVar found in ECU configuration parameters!. Found value is $ecucrbaI2cSlaveConfig_rbaI2cSlaveaddress_s\n" .
                                "#\n" .
                                "#   configuration file: FileRef\n" .
                                "#   module description: Module\n" .
                                "#\n");
                        }
                        else
                        {
                            my $even_validate_var;

                            $even_validate_var = $slaveaddressinteger_s % 2;

                            # validate Read/Write 0 bit is set Zero
                            if($even_validate_var == 1)
                            {
                                conf_process::Exit(
                                "#\n" .
                                "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                                "#\n" .
                                "#   The slave address is exceeds I2C address limit.  1to7 bit slave address and 0 bit is read/write  bit
                                Condition: 1to7 Slave address - refer from the User manuel and read/write control bit set to 0.
                                Error-> Read/Write bit not equal to zeor.
                                range for the slave $slaveconfig_ph->{'ShortName'}
                                in $ecucPbCfg_Name_s PbVar found in ECU configuration parameters!. Found value is $ecucrbaI2cSlaveConfig_rbaI2cSlaveaddress_s\n" .
                                "#\n" .
                                "#   configuration file: FileRef\n" .
                                "#   module description: Module\n" .
                                "#\n");

                            }
                        }
                    }
                    else
                    {
                        conf_process::Exit(
                            "#\n" .
                            "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                            "#\n" .
                            "#   The slave address needs to follow the hexadecimal format. It is not followed for the slave $slaveconfig_ph->{'ShortName'}
                                in $ecucPbCfg_Name_s PbVar found in ECU configuration parameters!\n" .
                            "#\n" .
                            "#   configuration file: FileRef\n" .
                            "#   module description: Module\n" .
                            "#\n");
                    }

                    $slaveconfig_ph->{"rbaI2cSlaveaddress"}=$ecucrbaI2cSlaveConfig_rbaI2cSlaveaddress_s;

                }


                # Validate the shortname
                if (   ( !exists($ecucrbaI2cSlaveConfig_ph->{'rba_I2c_SlaveSpeed'}))
                    || ( !exists($ecucrbaI2cSlaveConfig_ph->{'rba_I2c_SlaveSpeed'}->{'content'}))
                    || ( !defined($ecucrbaI2cSlaveConfig_ph->{'rba_I2c_SlaveSpeed'}->{'content'}))
                    || ( $ecucrbaI2cSlaveConfig_ph->{'rba_I2c_SlaveSpeed'}->{content} eq '' ) )
                {
                    $slaveconfig_ph->{'rbaI2cSlaveSpeed'} = 'NORMAL_MODE';

                }
                else
                {
                    ## Check slave speed range holds a valid value
                    unless ( grep {$_ eq $ecucrbaI2cSlaveConfig_ph->{'rba_I2c_SlaveSpeed'}->{'content'}} ('NORMAL_MODE', 'FAST_MODE') )

                    {
                        conf_process::Exit(
                            "#\n" .
                            "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                            "#\n" .
                            "#   The rba_I2c_SlaveSpeed section is not a valid selection for the slave $slaveconfig_ph->{'ShortName'}
                            in $ecucPbCfg_Name_s PbVar found in ECU configuration parameters!. The valid selections are NORMAL_MODE and FAST_MODE\n" .
                            "#\n" .
                            "#   configuration file: FileRef\n" .
                            "#   module description: Module\n" .
                            "#\n");
                    }

                    $ecucrbaI2cSlaveConfig_SlaveSpeed_s = $ecucrbaI2cSlaveConfig_ph->{'rba_I2c_SlaveSpeed'}->{'content'};

                    $slaveconfig_ph->{'rbaI2cSlaveSpeed'} = $ecucrbaI2cSlaveConfig_ph->{'rba_I2c_SlaveSpeed'}->{'content'};

                }

                if (
                ( !exists($ecucrbaI2cSlaveConfig_ph->{'rba_I2c_SlaveConnModule'}->{'content'})) ||
                ( $ecucrbaI2cSlaveConfig_ph->{'rba_I2c_SlaveConnModule'}->{content} eq '' ) ||
                ( $ecucrbaI2cSlaveConfig_ph->{'rba_I2c_SlaveConnModule'}->{content}  !~ /^\s*(I2C_)?(\d)/) )
                {
                    conf_process::Exit(
                        "#\n" .
                        "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                        "#\n" .
                        "#   The rba_I2c_SlaveConnModule section is not a valid selection for the slave $slaveconfig_ph->{'ShortName'}
                        in $ecucPbCfg_Name_s PbVar found in ECU configuration parameters!. The valid selections are NORMAL_MODE and FAST_MODE\n" .
                        "#\n" .
                        "#   configuration file: FileRef\n" .
                        "#   module description: Module\n" .
                        "#\n");
                }
                else
                {

                    my $rba_I2c_GetHwConfig_uCInfo_a = rba_I2c_UcMachineInfo::rba_I2c_GetId_uCInfo($MachineType_s);
                    my $rba_I2c_SlaveConnModule_Config_s = $ecucrbaI2cSlaveConfig_ph->{'rba_I2c_SlaveConnModule'}{'content'};
                       $rba_I2c_SlaveConnModule_Config_s  =~ /^\s*(I2C_)?(\d)/;
                       $rba_I2c_SlaveConnModule_Config_s = $2;

                    # Check whether port selection is a valid value.
                    unless ( grep{$_ eq $rba_I2c_SlaveConnModule_Config_s} @{$rba_I2c_GetHwConfig_uCInfo_a})
                    {
                            conf_process::Exit(
                            "#\n" .
                            "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                            "#\n" .
                            "#   The Slave connect module is not a valid selection for the hardware- unit  $rba_I2c_SlaveConnModule_Config_s\n" .
                            "#\n" .
                            "#   configuration file: FileRef\n" .
                            "#   module description: Module\n" .
                            "#\n");
                    }


                    $slaveconfig_ph->{'rbaI2cSlaveConnModule'}      = $rba_I2c_SlaveConnModule_Config_s;
                    $ecucrbaI2cSlaveConfig_rbaI2cSlaveConnect_s     = $rba_I2c_SlaveConnModule_Config_s;

                }


                #check if Slave config has already a number assigned
                if (exists $result_h{'SlaveConfigs'}{$ecucrbaI2cSlaveConfig_ShortName_s})
                {
                    #get existing number
                    $rba_I2cSlaveConfigNumber = $result_h{'SlaveConfigs'}{$ecucrbaI2cSlaveConfig_ShortName_s}{'rba_I2cSlaveConfig_Number'};
                }
                else
                {
                    #new Slave Config
                    $rba_I2cSlaveConfigNumber = $rba_I2c_SlaveConfigNumber_global;
                    $rba_I2c_SlaveConfigNumber_global++;

                    #store number (post build independent)
                    $result_h{'SlaveConfigs'}{$ecucrbaI2cSlaveConfig_ShortName_s}{'rba_I2cSlaveConfig_Number'} = $rba_I2cSlaveConfigNumber;
                }


                my $slaveaddress_pa;
                # Collect the hardware unit information
                # Check that slave address that connected to each hardware unit is unique.
                if (exists $result_h{'HardwareUnits'}{'SlaveAddress'}{$ecucPbCfg_Name_s}{$ecucrbaI2cSlaveConfig_rbaI2cSlaveConnect_s})
                {
                    $slaveaddress_pa = $result_h{'HardwareUnits'}{'SlaveAddress'}{$ecucPbCfg_Name_s}{$ecucrbaI2cSlaveConfig_rbaI2cSlaveConnect_s};
                    if (grep{$_ eq $ecucrbaI2cSlaveConfig_rbaI2cSlaveaddress_s} @{$slaveaddress_pa})
                    {
                        conf_process::Exit(
                            "#\n" .
                            "# ERROR in configuration script of ". WhoAmI() ." ($nameAction_s):\n" .
                            "#\n" .
                            "#   Slave address $ecucrbaI2cSlaveConfig_rbaI2cSlaveaddress_s is replicated for slave $slaveconfig_ph->{'ShortName'}
                            in hardware unit $ecucrbaI2cSlaveConfig_rbaI2cSlaveConnect_s in $ecucPbCfg_Name_s PbVar found in ECU configuration parameters!\n" .
                            "#\n".
                            "#   configuration file: FileRef\n" .
                            "#   module description: Module\n" .
                            "#\n");
                    }
                    else
                    {
                        push (@{$slaveaddress_pa}, $ecucrbaI2cSlaveConfig_rbaI2cSlaveaddress_s);
                        $result_h{'HardwareUnits'}{'SlaveAddress'}{$ecucPbCfg_Name_s}{$ecucrbaI2cSlaveConfig_rbaI2cSlaveConnect_s} = $slaveaddress_pa;
                    }

                }
                else
                {
                    # New hardware unit
                    $rba_I2c_HwUnitNumber = $rba_I2c_HwUnitCnt_s;
                    $rba_I2c_HwUnitCnt_s++;
                    # Add the slave address to hardware unit
                    push (@{$slaveaddress_pa}, $ecucrbaI2cSlaveConfig_rbaI2cSlaveaddress_s);
                    $result_h{'HardwareUnits'}{'SlaveAddress'}{$ecucPbCfg_Name_s}{$ecucrbaI2cSlaveConfig_rbaI2cSlaveConnect_s} = $slaveaddress_pa;

                    # Store based on HwId Number
                    $result_h{'HardwareUnits'}{'HwIdByPhyUnit'}{$ecucrbaI2cSlaveConfig_rbaI2cSlaveConnect_s}{$ecucPbCfg_Name_s}{'rba_I2c_HwId_ShortName'} = $rba_I2c_HwUnitNumber;
                }

                #     Store based on SlaveId Number
                $result_h{'SlaveIdByNumber'}{$rba_I2cSlaveConfigNumber}{'rba_I2c_SlaveId_ShortName'} = $ecucrbaI2cSlaveConfig_ShortName_s;

                # Add rba_I2c_SlaveConfig ShortNames to result hash
                %{$result_h{'SlaveConfigs'}{$ecucrbaI2cSlaveConfig_ShortName_s}{$ecucPbCfg_Name_s}}=
                (
                    'rba_I2c_SlaveAddress'                   => $ecucrbaI2cSlaveConfig_rbaI2cSlaveaddress_s,
                    'rba_I2c_SlaveSpeed'                     => $ecucrbaI2cSlaveConfig_SlaveSpeed_s,
                    'rba_I2c_SlaveConnModule'                => $ecucrbaI2cSlaveConfig_rbaI2cSlaveConnect_s,
                    'rba_I2c_SlaveConnLogicalModule'         => $rba_I2c_HwUnitNumber,
                    'PostBuildCfg'                           => $ecucPbCfg_Name_s,
                    'PbCfg_FileRef'                          => $ecucPbCfg_FileRef_s,
                );
                $rba_I2c_SlaveConfigCnt_s++;

                if( !exists $slaveConfigset_ph->{$ecucrbaI2cSlaveConfig_ShortName_s})
                {
                    $slaveConfigset_ph->{$ecucrbaI2cSlaveConfig_ShortName_s} = $slaveconfig_ph;
                }
                else
                {
                    conf_process::Exit(
                    "#\n" .
                    "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                    "#\n" .
                    "#   Sequence short names-$ecucrbaI2cSlaveConfig_ShortName_s - are repeted in  $ecucPbCfg_Name_s  postbuild dataset".
                    "#\n" .
                    "#   configuration file: FileRef\n" .
                    "#   module description: Module\n" .
                    "#\n");
                }
            }

            if ($rba_I2c_SlaveConfigCnt_s == 0 )
            {
                conf_process::Exit(
                    "#\n" .
                    "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                    "#\n" .
                    "#   Slave configuration is not present in the  $ecucPbCfg_Name_s postbuild variant,
                    if not required the slave configuration we can remove the postbuild variant." .
                    "#\n" .
                    "#   configuration file: FileRef\n" .
                    "#   module description: Module\n" .
                    "#\n");
            }

            #store number of used SlaveConfigs
            $result_h{'UsedSlaveConfigs'}{$ecucPbCfg_Name_s} = $rba_I2c_SlaveConfigCnt_s;

            #   store number of used SlaveConfigs
            $result_h{'UsedHwUnits'}{$ecucPbCfg_Name_s} = $rba_I2c_HwUnitCnt_s;

            # store names of post build variants
            $result_h{'PostBuildNames'}{$ecucPbCfg_Name_s}{'used'} = 1;

            $slaveConfigsetPostbuild_ph->{$ecucPbCfg_Name_s}= $slaveConfigset_ph;
        }
    }

    return ( \%result_h,$slaveConfigsetPostbuild_ph ) ;
}

#----------------------------------------------------------------------------------------------------------------------
# GetSequenceConfig: Checks the validity of the rba_I2c_ConfigSet configuration data and stores it in a more efficient structure
#
#----------------------------------------------------------------------------------------------------------------------
sub GetSequenceConfig
{
    # funciton return Result
    my %result_h;
    # subroutine name.
    my $nameAction_s = (caller(1))[3] . "->" . (caller(0))[3];
    # Global Counter for sequece configuration.
    my $rba_I2c_SequenceConfigNumber_global = 0;
    # local sequence configuration pointer
    my $rba_I2c_sequenceConfig_ph;
    #  Sequece configuration for Postbuild dataset
    my $sequenceConfigsetPostbuild_ph;

    # Loop over all available PostBuild configurations
    # conf_process::GetVariants returns
    # AR40: rba_I2c_ConfigSet_4Cyl, rba_I2c_ConfigSet_6Cyl
    # AR42: VPOSTBS_Variant_Master_4cyl, VPOSTBS_Variant_Master_6cyl
    foreach my $strVariant_s (conf_process::GetVariants())
    {
        # get input data for current variant
        my $ecucPostBuildConfig_ph = conf_process::GetInputData("rba_I2c", $strVariant_s)->{rba_I2c_ConfigSet};

        # read and sequence short name
        my $ecucPbCfg_Name_s = conf_process::GetConfigName($strVariant_s);

        # Read the file info
        my $ecucPbCfg_FileRef_s   = $ecucPostBuildConfig_ph->{'FileRef'};

        #counter for number of used Slave Configs
        my ($rba_I2c_SequenceConfigCnt_s, $rba_I2c_HwUnitCnt_s) = (0,0);

        # Loop over all available slave config configurations
        my ($rba_I2cSequenceConfigNumber);

        # varialble for the Sequence config dataset.
        my $sequenceConfigset_ph;

        # get the each slave configuration from the Postbuild dataset
        foreach my $ecucrbaI2cSlaveConfig_ph (@{$ecucPostBuildConfig_ph->{'rba_I2c_SlaveConfig'}})
        {
           unless(ref $ecucrbaI2cSlaveConfig_ph->{rba_I2c_SequenceConfig} eq 'ARRAY'  && scalar(@{$ecucrbaI2cSlaveConfig_ph->{'rba_I2c_SequenceConfig'}}) >0 )
            {
                    conf_process::Exit(
                    "#\n" .
                    "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                    "#\n" .
                    "#   The Sequence is not configured in Slave configuration \n" .
                    "#\n" .
                    "#   configuration file: FileRef\n" .
                    "#   module description: Module\n" .
                    "#\n");
            }

            # get the each sequence configuration from the Postbuild dataset
            foreach my $ecucrbaI2cSequenceConfig_ph (@{$ecucrbaI2cSlaveConfig_ph->{'rba_I2c_SequenceConfig'}})
            {
                my $ecucrbaI2cSequenceConfig_ShortName_s;
                my $ecucrbaI2cSequenceConfig_rbaI2cSequenceEndNotification_s;
                my $ecucrbaI2cSequenceConfig_rbaI2cSequenceEndNotificationHeaderName_s;
                my $ecucrbaI2cSequenceConfig_rbaI2cSequencePriority_s;
                my $ecucrbaI2cSequenceConfig_rbaI2cSequenceInterface_s;
                my $ecucrbaI2cSequenceConfig_rbaI2cSequenceTxLength_s;
                my $ecucrbaI2cSequenceConfig_rbaI2cSequenceRxLength_s;
                my $ecucrbaI2cSequenceConfig_rbaI2cSlaveConfigId_s = $ecucrbaI2cSlaveConfig_ph->{'rba_I2c_SlaveConfig__KEY'}{'content'};
                my $sequenceConfig_ph;

                # Validate the shortname
                if ( ( !defined($ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceConfig__KEY'}->{'content'}))
                    || ( $ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceConfig__KEY'}->{content} eq '' ) )
                {
                    conf_process::Exit(
                    "#\n" .
                    "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                    "#\n" .
                    "#   The Sequence configi shot name is  not configured  $ecucrbaI2cSequenceConfig_ph \n" .
                    "#\n" .
                    "#   configuration file: FileRef\n" .
                    "#   module description: Module\n" .
                    "#\n");
                }
                else
                {
                    # check any blank space in the Sequence configuration.
                    if($ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceConfig__KEY'}{'content'} =~ /\s+/)
                    {
                        conf_process::Exit(
                        "#\n" .
                        "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                        "#\n" .
                        "#   The Sequence configi shot name is configured with blankspace $ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceConfig__KEY'}{'content'} \n" .
                        "#\n" .
                        "#   configuration file: FileRef\n" .
                        "#   module description: Module\n" .
                        "#\n");
                    }
                    else
                    {
                        # sequence short name store in to the local variable
                        $sequenceConfig_ph->{'ShortName'} = $ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceConfig__KEY'}{'content'};
                        $ecucrbaI2cSequenceConfig_ShortName_s = $ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceConfig__KEY'}{'content'};
                    }

                }


                # Validate the communication type.
                if ( ( !defined($ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceTxLength'}->{'content'}))
                    || ( $ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceTxLength'}->{content} eq '' ) )
                {
                    conf_process::Exit(
                    "#\n" .
                    "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                    "#\n" .
                    "#   The Sequence Transmit length is  not configured  $ecucrbaI2cSequenceConfig_ph - $ecucPostBuildConfig_ph dataset\n" .
                    "#\n" .
                    "#   configuration file: FileRef\n" .
                    "#   module description: Module\n" .
                    "#\n");
                }
                else
                {
                    $ecucrbaI2cSequenceConfig_rbaI2cSequenceTxLength_s = $ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceTxLength'}->{'content'};
                                        # read the sequece priorty from the configuration
                    $sequenceConfig_ph->{"SequenceTxLength"} = $ecucrbaI2cSequenceConfig_rbaI2cSequenceTxLength_s;
                }

                 # Validate the communication type.
                if ( ( !defined($ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceRxLength'}->{'content'}))
                    || ( $ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceRxLength'}->{content} eq '' ) )
                {
                    conf_process::Exit(
                    "#\n" .
                    "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                    "#\n" .
                    "#   The Sequence Receive length is  not configured  $ecucrbaI2cSequenceConfig_ph - $ecucPostBuildConfig_ph dataset\n" .
                    "#\n" .
                    "#   configuration file: FileRef\n" .
                    "#   module description: Module\n" .
                    "#\n");
                }
                else
                {
                    $ecucrbaI2cSequenceConfig_rbaI2cSequenceRxLength_s = $ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceRxLength'}->{'content'};
                    $sequenceConfig_ph->{"SequenceRxLength"} = $ecucrbaI2cSequenceConfig_rbaI2cSequenceRxLength_s;
                }


                # Validate the communication type.
                if ( ( !defined($ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceInterface'}->{'content'}))
                    || ( $ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceInterface'}->{content} eq '' ) )
                {
                    conf_process::Exit(
                    "#\n" .
                    "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                    "#\n" .
                    "#   The Sequence interface is  not configured  $ecucrbaI2cSequenceConfig_ph - $ecucPostBuildConfig_ph dataset\n" .
                    "#\n" .
                    "#   configuration file: FileRef\n" .
                    "#   module description: Module\n" .
                    "#\n");
                }
                else
                {
                    $ecucrbaI2cSequenceConfig_rbaI2cSequenceInterface_s = $ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceInterface'}->{'content'};

                     # validate the sequence priortiy configuration format.
                    unless ( grep{$_ eq $ecucrbaI2cSequenceConfig_rbaI2cSequenceInterface_s} ('RBA_I2C_SEQ_TRANSMIT','RBA_I2C_SEQ_RECEIVE','RBA_I2C_SEQ_TRANSMIT_RECEIVE') )
                    {
                        conf_process::Exit(
                        "#\n" .
                        "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                        "#\n" .
                        "#   The I2C interface Communication type is wrong \n" .
                        "#\n" .
                        "#   configuration file: FileRef\n" .
                        "#   module description: Module\n" .
                        "#\n");
                    }

                    if($ecucrbaI2cSequenceConfig_rbaI2cSequenceInterface_s eq "RBA_I2C_SEQ_TRANSMIT" )
                    {
                        if( ( $ecucrbaI2cSequenceConfig_rbaI2cSequenceTxLength_s == 0 ) || ( $ecucrbaI2cSequenceConfig_rbaI2cSequenceTxLength_s > 32 ) )
                        {
                            conf_process::Exit(
                            "#\n" .
                            "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                            "#\n" .
                            "#   The Sequence Transmit length is faild the condition (1-32)  in  $ecucrbaI2cSequenceConfig_ph - $ecucPostBuildConfig_ph dataset\n" .
                            "#\n" .
                            "#   configuration file: FileRef\n" .
                            "#   module description: Module\n" .
                            "#\n");

                        }

                        $ecucrbaI2cSequenceConfig_rbaI2cSequenceRxLength_s = 0;


                        $sequenceConfig_ph->{"SequenceInterface"} = 'rba_I2c_Tx_e';

                    }

                    if ($ecucrbaI2cSequenceConfig_rbaI2cSequenceInterface_s eq "RBA_I2C_SEQ_RECEIVE" )
                    {
                        if( ( $ecucrbaI2cSequenceConfig_rbaI2cSequenceRxLength_s == 0 ) || ( $ecucrbaI2cSequenceConfig_rbaI2cSequenceRxLength_s > 32 ) )
                        {
                            conf_process::Exit(
                            "#\n" .
                            "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                            "#\n" .
                            "#   The Sequence Receive length is not valid, data length size condition (1-32)  in  $ecucrbaI2cSequenceConfig_ph - $ecucPostBuildConfig_ph dataset\n" .
                            "#\n" .
                            "#   configuration file: FileRef\n" .
                            "#   module description: Module\n" .
                            "#\n");

                        }

                        $ecucrbaI2cSequenceConfig_rbaI2cSequenceTxLength_s = 0;


                        $sequenceConfig_ph->{"SequenceInterface"} = 'rba_I2c_Rx_e';

                    }

                    if ($ecucrbaI2cSequenceConfig_rbaI2cSequenceInterface_s eq "RBA_I2C_SEQ_TRANSMIT_RECEIVE" )
                    {
                        if( ( $ecucrbaI2cSequenceConfig_rbaI2cSequenceTxLength_s == 0 ) || ( $ecucrbaI2cSequenceConfig_rbaI2cSequenceTxLength_s > 32 ) )
                        {
                            conf_process::Exit(
                            "#\n" .
                            "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                            "#\n" .
                            "#   The Sequence Transmit length is faild the condition (1-32)  in  $ecucrbaI2cSequenceConfig_ph - $ecucPostBuildConfig_ph dataset\n" .
                            "#\n" .
                            "#   configuration file: FileRef\n" .
                            "#   module description: Module\n" .
                            "#\n");

                        }

                        if( ( $ecucrbaI2cSequenceConfig_rbaI2cSequenceRxLength_s == 0 ) || ( $ecucrbaI2cSequenceConfig_rbaI2cSequenceRxLength_s > 32 ) )
                        {
                            conf_process::Exit(
                            "#\n" .
                            "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                            "#\n" .
                            "#   The Sequence Transmit interface Receive length should be '0' to '32'  in  $ecucrbaI2cSequenceConfig_ph - $ecucPostBuildConfig_ph dataset\n" .
                            "#\n" .
                            "#   configuration file: FileRef\n" .
                            "#   module description: Module\n" .
                            "#\n");

                        }

                        $sequenceConfig_ph->{"SequenceInterface"} = 'rba_I2c_CombinedTxRx_e';

                    }
                }


                # Validate the sequence endnotification configuration
                if (   ( !exists($ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceEndNotification'}))
                    || ( !exists($ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceEndNotification'}->{'content'}))
                    || ( !defined($ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceEndNotification'}->{'content'}))
                    || ( $ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceEndNotification'}->{content} eq '' )
                    || ( $ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceEndNotification'}->{content} eq 'NULL_PTR' ) )
                {
                    # if the sequence end nofitcation is not present sen default value to Null pointer
                    $ecucrbaI2cSequenceConfig_rbaI2cSequenceEndNotification_s = 'NULL_PTR';
                }
                else
                {
                    $ecucrbaI2cSequenceConfig_rbaI2cSequenceEndNotification_s =  $ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceEndNotification'}->{content};

                    # check any blank space in the Sequence endnofication configuration.
                    if($ecucrbaI2cSequenceConfig_rbaI2cSequenceEndNotification_s =~ /\s+/)
                    {
                        conf_process::Exit(
                        "#\n" .
                        "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                        "#\n" .
                        "#   The Sequence End Notification function is configured with blankspace $ecucrbaI2cSequenceConfig_rbaI2cSequenceEndNotification_s \n" .
                        "#\n" .
                        "#   configuration file: FileRef\n" .
                        "#   module description: Module\n" .
                        "#\n");
                    }

                    $ecucrbaI2cSequenceConfig_rbaI2cSequenceEndNotification_s =  '&'.$ecucrbaI2cSequenceConfig_rbaI2cSequenceEndNotification_s;

                }
                #read the sequence notifcation function name to local varialble
                $sequenceConfig_ph->{"SequenceEndNotification"} = $ecucrbaI2cSequenceConfig_rbaI2cSequenceEndNotification_s;

                # Validate the Header name for sequence end notification configuration
                if (( !exists($ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceEndNotificationHeaderName'}))
                    || ( !exists($ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceEndNotificationHeaderName'}->{'content'}))
                    || ( !defined($ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceEndNotificationHeaderName'}->{'content'}))
                    || ( $ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceEndNotificationHeaderName'}->{content} eq '' ))
                {
                    # if the header name is not present set default value to Null pointer
                    $ecucrbaI2cSequenceConfig_rbaI2cSequenceEndNotificationHeaderName_s = 'NULL';
                }
                else
                {
                    $ecucrbaI2cSequenceConfig_rbaI2cSequenceEndNotificationHeaderName_s =  $ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceEndNotificationHeaderName'}->{content};
                }
                #read the sequence notifcation function's header file name to local varialble
                $sequenceConfig_ph->{"HeaderFileName"} = $ecucrbaI2cSequenceConfig_rbaI2cSequenceEndNotificationHeaderName_s;


                # Check slave speed range holds a valid value
                if (   ( !defined($ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequencePriority'}->{'content'}))
                    || ( $ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequencePriority'}->{content} eq '' ) )
                {
                    # if configuration is not available set the default settingas LOW priority.
                    $ecucrbaI2cSequenceConfig_rbaI2cSequencePriority_s = 'RBA_I2C_SEQ_LOW_PRIORITY';
                }
                else
                {
                    # read the sequence priority.
                    $ecucrbaI2cSequenceConfig_rbaI2cSequencePriority_s = $ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequencePriority'}->{'content'};

                    # validate the sequence priortiy configuration format.
                    unless ( grep{$_ eq $ecucrbaI2cSequenceConfig_rbaI2cSequencePriority_s} ('RBA_I2C_SEQ_HIGH_PRIORITY','RBA_I2C_SEQ_LOW_PRIORITY') )
                    {
                        conf_process::Exit(
                        "#\n" .
                        "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                        "#\n" .
                        "#   The I2C priority section is not a valid selection for the slave $ecucrbaI2cSequenceConfig_ph->{'rba_I2c_SequenceConfig__KEY'}{'content'}
                             in $ecucPbCfg_Name_s PbVar found in ECU configuration parameters!. The valid selections are HIGH_PRIORITY and LOW_PRIORITY\n" .
                        "#\n" .
                        "#   configuration file: FileRef\n" .
                        "#   module description: Module\n" .
                        "#\n");
                    }
                    # read the sequece priorty from the configuration
                    $sequenceConfig_ph->{"SequencePriority"} = $ecucrbaI2cSequenceConfig_rbaI2cSequencePriority_s;
                }

                # read the slave config name
                $sequenceConfig_ph->{"SlaveConfigId"} = $ecucrbaI2cSequenceConfig_rbaI2cSlaveConfigId_s;

                #check if Sequence config has already a number assigned
                if (exists $rba_I2c_sequenceConfig_ph->{'SequenceConfig_Id'}{$ecucrbaI2cSequenceConfig_ShortName_s})
                {
                    #get existing number
                    $rba_I2cSequenceConfigNumber = $rba_I2c_sequenceConfig_ph->{'SequenceConfig_Id'}->{$ecucrbaI2cSequenceConfig_ShortName_s}->{'rba_I2cSequenceConfig_Number'};
                    $sequenceConfig_ph->{"rba_I2cSequenceConfig_Number"} = $rba_I2cSequenceConfigNumber;
                }
                else
                {
                    #new Slave Config
                    $rba_I2cSequenceConfigNumber = $rba_I2c_SequenceConfigNumber_global;
                    $rba_I2c_SequenceConfigNumber_global++;

                    #store number (post build independent)
                    $rba_I2c_sequenceConfig_ph->{'SequenceConfig_Id'}->{$ecucrbaI2cSequenceConfig_ShortName_s}->{'rba_I2cSequenceConfig_Number'} = $rba_I2cSequenceConfigNumber;
                    $sequenceConfig_ph->{"rba_I2cSequenceConfig_Number"} = $rba_I2cSequenceConfigNumber;

                }

                # validate any repeat sequence configuration present
                if( !exists $sequenceConfigset_ph->{$ecucrbaI2cSequenceConfig_ShortName_s})
                {
                    $sequenceConfigset_ph->{$ecucrbaI2cSequenceConfig_ShortName_s} = $sequenceConfig_ph;
                }
                else
                {
                    conf_process::Exit(
                    "#\n" .
                    "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                    "#\n" .
                    "#   Sequence short names- $sequenceConfig_ph->{'ShortName'} - are repeted in  $ecucPbCfg_Name_s  postbuild dataset".
                    "#\n" .
                    "#   configuration file: FileRef\n" .
                    "#   module description: Module\n" .
                    "#\n");
                }
                # seqence configuration is valid increment the counter.
                $rba_I2c_SequenceConfigCnt_s++;
            }

            if ($rba_I2c_SequenceConfigCnt_s == 0 )
            {
                conf_process::Exit(
                    "#\n" .
                    "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                    "#\n" .
                    "#   Sequence configuration is not present in the  $ecucPbCfg_Name_s postbuild variant,
                    if not required the slave configuration we can remove the postbuild variant." .
                    "#\n" .
                    "#   configuration file: FileRef\n" .
                    "#   module description: Module\n" .
                    "#\n");
            }

       }

       if (defined $sequenceConfigset_ph)
       {
           $sequenceConfigsetPostbuild_ph->{$ecucPbCfg_Name_s} = $sequenceConfigset_ph;
       }

    }

    $rba_I2c_sequenceConfig_ph->{'SequenceConfigset'} = $sequenceConfigsetPostbuild_ph;
    $rba_I2c_sequenceConfig_ph->{'maxSequenceConnt'} = $rba_I2c_SequenceConfigNumber_global;

    return ( $rba_I2c_sequenceConfig_ph);
}
#----------------------------------------------------------------------------------------------------------------------
# GetHwConfig: Checks the validity of the rba_I2c_ConfigSet->rba_I2c_HwUnit configuration data and stores it in a more efficient structure
#
# Parameters:
# $_[0]: Machine type[IN]
# $_[1]: Reference to the the slave configuraiton hash [IN]
# $_[2]: Directory which contains the machine dependent files[IN]
# $_[3]: Machine dependent file name [IN]
#
#----------------------------------------------------------------------------------------------------------------------

sub GetHwConfig
{
    my $MachineType_s               = $_[0];
    my $rba_I2c_slaveConfig_ph      = $_[1];
    my $path_scripts_s              = $_[2];
    my $Machinefile_s               = $_[3];

    # Local variable.
    #---------------
    # subroutine name.
    my $nameAction_s = (caller(1))[3] . "->" . (caller(0))[3];

    # validate the machine type.
    $MachineType_s =~ /^MCU_RB\_([A-Za-z0-9]+_UC1)([A-Za-z0-9_]+)$/;

    if( (!defined $1) && (!defined $2) )
    {
        conf_process::Exit("ERROR - Variant can not be extracted from MCU information! ($MachineType_s)");
    }
    my $MachineFamily_s = $1;

    # add check if according perl scipts are available
    require "$path_scripts_s/$Machinefile_s";

    # Loop over all available PostBuild configurations
    # conf_process::GetVariants returns
    # AR40: rba_I2c_ConfigSet_4Cyl, rba_I2c_ConfigSet_6Cyl
    # AR42: VPOSTBS_Variant_Master_4cyl, VPOSTBS_Variant_Master_6cyl
    foreach my $strVariant_s (conf_process::GetVariants())
    {
        # get input data for current variant
        my $ecucPostBuildConfig_ph = conf_process::GetInputData("rba_I2c", $strVariant_s)->{rba_I2c_ConfigSet};

        my $ecucPbCfg_Name_s = conf_process::GetConfigName($strVariant_s);

        # read the file reference
        my $ecucPbCfg_FileRef_s   = $ecucPostBuildConfig_ph->{'FileRef'};

        # validate machine type as not IFX and Hardware configuration is present is error.
        if ((exists ($ecucPostBuildConfig_ph->{'rba_I2c_IfxHwUnit'})) && ($MachineFamily_s ne 'IFX_UC1'))
        {
            conf_process::Exit(
             "#\n" .
              "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
              "#\n" .
              "#   The rba_I2c_HwUnit container is not allowed to include for other devices other (IFX controller)IFX_UC1\n".
              "#   Now, rba_I2c_HwUnit is found in $ecucPbCfg_Name_s PbVar in ECU configuration parameters!. \n" .
              "#\n" .
              "#   configuration file: FileRef\n" .
              "#   module description: Module\n" .
              "#\n");
        }

        # validate machine type as IFX and Hardware configuration is present.
        if( $MachineFamily_s eq 'IFX_UC1' )
        {

            if (exists $ecucPostBuildConfig_ph->{'rba_I2c_IfxHwUnit'})
            {
                my @portselection_a = ('PORTSELECT_A', 'PORTSELECT_B', 'PORTSELECT_C', 'PORTSELECT_D', 'PORTSELECT_E', 'PORTSELECT_F', 'PORTSELECT_G', 'PORTSELECT_H');
                # Loop over all available HwUnit config configurations
                my @hwUnits_a;



                my @hwUnitShortName_a;
                foreach my $ecucrbaI2cHwUnitConfig_ph (@{$ecucPostBuildConfig_ph->{'rba_I2c_IfxHwUnit'}})
                {
                    my $ecucrbaI2cHwUnitConfig_ShortName_s               = $ecucrbaI2cHwUnitConfig_ph->{'rba_I2c_IfxHwUnit__KEY'}{'content'};
                    $ecucrbaI2cHwUnitConfig_ShortName_s                  =~ /^rba_(I2c)_(IfxHwUnit)\_([0-1])/;

                    if(!defined ($3))
                    {
                          conf_process::Exit(
                          "#\n" .
                          "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                          "#\n" .
                          "#   The short name <$ecucrbaI2cHwUnitConfig_ShortName_s> of hardware unit shortnaem should be rba_I2c_IfxHwUnit_x (where x presents the physcial I2C hardware unit number)\n".
                          "#   Ex: I2C0 channel for-rba_I2c_IfxHwUnit_0, I2C1 channel for -rba_I2c_IfxHwUnit_1  \n" .
                          "#    in $ecucPbCfg_Name_s PbVar found in ECU configuration parameters!.   \n" .
                          "#\n" .
                          "#   configuration file: FileRef\n" .
                          "#   module description: Module\n" .
                          "#\n");
                    }

                    unless ( grep {$_ eq $ecucrbaI2cHwUnitConfig_ShortName_s} @hwUnitShortName_a)
                    {
                         push(@hwUnitShortName_a, $ecucrbaI2cHwUnitConfig_ShortName_s);

                    }
                    else
                    {
                          conf_process::Exit(
                          "#\n" .
                          "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                          "#\n" .
                          "#   The short name of IFX-hardware unit short name is repeat in  $ecucPostBuildConfig_ph \n".
                          "#\n" .
                          "#   configuration file: FileRef\n" .
                          "#   module description: Module\n" .
                          "#\n");

                    }

                    my $ecucrbaI2cHwUnitConfig_Module_s                  = $ecucrbaI2cHwUnitConfig_ph->{'Module'};
                    my $ecucrbaI2cHwUnitConfig_FileRef_s                 = $ecucrbaI2cHwUnitConfig_ph->{'FileRef'};
                    my $rba_I2c_Ifx_HwUnitPortSelection_Config_s;        # PORTSELECT_A,PORTSELECT_B, PORTSELECT_C
                    my $rba_I2c_Ifx_HwUnitSlaveConnModule_Config_s;


                    if (   ( !exists($ecucrbaI2cHwUnitConfig_ph->{'rba_I2c_PortSelect'}))
                        || ( !exists($ecucrbaI2cHwUnitConfig_ph->{'rba_I2c_PortSelect'}->{'content'}))
                        || ( !defined($ecucrbaI2cHwUnitConfig_ph->{'rba_I2c_PortSelect'}->{'content'}))
                        || ( $ecucrbaI2cHwUnitConfig_ph->{'rba_I2c_PortSelect'}->{content} eq '' ) )
                    {
                        conf_process::Exit(
                              "#\n" .
                              "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                              "#\n" .
                              "#   The short name should be IFX hardware uint $ecucrbaI2cHwUnitConfig_ph inside $ecucrbaI2cHwUnitConfig_ShortName_s I2C-MUX PORT selection configuration is not available \n".
                              "#    for the hardware unit $ecucrbaI2cHwUnitConfig_ShortName_s is found in $ecucPbCfg_Name_s PbVar in ECU configuration parameters!. \n" .
                              "#\n" .
                              "#   configuration file: FileRef\n" .
                              "#   module description: Module\n" .
                              "#\n");
                    }
                    else
                    {
                        $rba_I2c_Ifx_HwUnitPortSelection_Config_s  = $ecucrbaI2cHwUnitConfig_ph->{'rba_I2c_PortSelect'}{'content'};

                        # Check whether port selection is a valid value.
                        unless ( grep{$_ eq $rba_I2c_Ifx_HwUnitPortSelection_Config_s} @portselection_a)
                        {
                            conf_process::Exit(
                                "#\n" .
                                "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                                "#\n" .
                                "#   The Port selection is not a valid selection for the hardware unit  $ecucrbaI2cHwUnitConfig_ShortName_s\n" .
                                "#    in $ecucPbCfg_Name_s PbVar found in ECU configuration parameters!. The valid selections are  @portselection_a \n" .
                                "#\n" .
                                "#   configuration file: FileRef\n" .
                                "#   module description: Module\n" .
                                "#\n");
                        }
                    }


                    # I2C module Channel info configuration validation.
                    if (

                           ( !exists($ecucrbaI2cHwUnitConfig_ph->{'rba_I2c_HwModule'}->{'content'})) ||
                           ( $ecucrbaI2cHwUnitConfig_ph->{'rba_I2c_HwModule'}->{content} eq '' )  ||
                           ( $ecucrbaI2cHwUnitConfig_ph->{'rba_I2c_HwModule'}->{content} !~ /^\s*(I2C_)?(\d)/) )
                    {
                        conf_process::Exit(
                            "#\n" .
                            "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                            "#\n" .
                            "#   The short name should be rba_I2c_IfxHwUnit container is not valid \n".
                            "#\n" .
                            "#   configuration file: FileRef\n" .
                            "#   module description: Module\n" .
                            "#\n");

                    }
                    else
                    {
                        my $rba_I2c_GetHwConfig_uCInfo_a = rba_I2c_UcMachineInfo::rba_I2c_GetId_uCInfo($MachineType_s);
                        $rba_I2c_Ifx_HwUnitSlaveConnModule_Config_s  = $ecucrbaI2cHwUnitConfig_ph->{'rba_I2c_HwModule'}{'content'};
                        $rba_I2c_Ifx_HwUnitSlaveConnModule_Config_s  =~  /^\s*(I2C_)?(\d)/;
                        $rba_I2c_Ifx_HwUnitSlaveConnModule_Config_s = $2;

                        # Check whether port selection is a valid value.
                        unless ( grep{$_ eq $rba_I2c_Ifx_HwUnitSlaveConnModule_Config_s} @{$rba_I2c_GetHwConfig_uCInfo_a})
                        {
                            conf_process::Exit(
                            "#\n" .
                            "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                            "#\n" .
                            "#   The Port selection is not a valid selection for the hardware unit  $ecucrbaI2cHwUnitConfig_ShortName_s\n" .
                            "#\n" .
                            "#   configuration file: FileRef\n" .
                            "#   module description: Module\n" .
                            "#\n");
                        }

                        if (grep{$_ eq $rba_I2c_Ifx_HwUnitSlaveConnModule_Config_s} @hwUnits_a)
                        {
                                            conf_process::Exit(
                            "#\n" .
                            "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                            "#\n" .
                            "#   The IFX hardware unit Port selection is configure mulitple instent $ecucrbaI2cHwUnitConfig_ShortName_s\n" .
                            "#    in $ecucPbCfg_Name_s PbVar found in ECU configuration parameters!.    \n" .
                            "#\n" .
                            "#   configuration file: FileRef\n" .
                            "#   module description: Module\n" .
                            "#\n");

                        }
                        else
                        {
                            push(@hwUnits_a, $rba_I2c_Ifx_HwUnitSlaveConnModule_Config_s);
                        }

                    }

                    if (exists ($rba_I2c_slaveConfig_ph->{'HardwareUnits'}{'HwIdByPhyUnit'}{$rba_I2c_Ifx_HwUnitSlaveConnModule_Config_s}{$ecucPbCfg_Name_s}))
                    {
                      $rba_I2c_slaveConfig_ph->{'HardwareUnits'}{'HwIdByPhyUnit'}{$rba_I2c_Ifx_HwUnitSlaveConnModule_Config_s}{$ecucPbCfg_Name_s}{'rba_I2c_HwId_PortConnection'} = $rba_I2c_Ifx_HwUnitPortSelection_Config_s;
                    }
                    else
                    {
                        print "\nWARNING: I2C-Harware channel $rba_I2c_Ifx_HwUnitSlaveConnModule_Config_s is configured, but slave configuration is not used\n";
                        print " if not used I2C-Harware channel $rba_I2c_Ifx_HwUnitSlaveConnModule_Config_s, need to remove the hardware configuration form $ecucPbCfg_Name_s \n";
                    }

                }

                my @slavehwUnits_a;
                # Loop all the hardware physical units from slave config data sets.
                foreach my $rba_I2c_HwUnitPhy_s (sort(keys(%{$rba_I2c_slaveConfig_ph->{'HardwareUnits'}{'HwIdByPhyUnit'}})))
                {

                    if (exists $rba_I2c_slaveConfig_ph->{'HardwareUnits'}{'HwIdByPhyUnit'}{$rba_I2c_HwUnitPhy_s}{$ecucPbCfg_Name_s})
                    {
                      push (@slavehwUnits_a, $rba_I2c_HwUnitPhy_s);
                    }
                }

                foreach my $hwunit_s(@slavehwUnits_a)
                {
                    unless ( grep{$_ eq $hwunit_s} @hwUnits_a)
                    {
                      conf_process::Exit(
                        "#\n" .
                        "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                        "#\n" .
                        "#   The hardware unit $hwunit_s is configured in slave confgiration dataset, but not present in HwUnit dataset\n".
                        "#     in $ecucPbCfg_Name_s PbVar found in ECU configuration parameters!.\n" .
                        "#\n" .
                        "#   configuration file: FileRef\n" .
                        "#   module description: Module\n" .
                        "#\n");
                    }
                }
            }
            else
            {
                foreach my $slaveCon_s(keys (%{$rba_I2c_slaveConfig_ph->{'HardwareUnits'}{'HwIdByPhyUnit'}}))
                {
                    if(exists $rba_I2c_slaveConfig_ph->{'HardwareUnits'}{'HwIdByPhyUnit'}{$slaveCon_s}{$ecucPbCfg_Name_s})
                    {
                            $rba_I2c_slaveConfig_ph->{'HardwareUnits'}{'HwIdByPhyUnit'}{$slaveCon_s}{$ecucPbCfg_Name_s}{'rba_I2c_HwId_PortConnection'} = 'PORTSELECT_A';

                            print "\nWARNING: IFX-I2C-Harware channel $slaveCon_s Mux is Not configured, but slave configuration is used in $ecucPbCfg_Name_s dataset \n";
                            print " Default Mux 'PORTSELECT_A' selection updated for the I2C-Harware channel $slaveCon_s  \n";

                    }

                }

            }
        }

        # Loop all the hardware physical units from slave config data sets.
        foreach my $rba_I2c_HwUnitPhy_s (sort(keys(%{$rba_I2c_slaveConfig_ph->{'HardwareUnits'}{'HwIdByPhyUnit'}})))
        {
            # update the hardware validate status.
            if (exists $rba_I2c_slaveConfig_ph->{'HardwareUnits'}{'HwIdByPhyUnit'}{$rba_I2c_HwUnitPhy_s}{$ecucPbCfg_Name_s})
            {
                my $rba_hwValidate_status = 0;

                # Check that physical Hw unit is present in the MCU sub machine type
                my $I2cMachineId_pa = rba_I2c_UcMachineInfo::rba_I2c_GetId_uCInfo($MachineType_s);
                foreach my $i2chwUnit_s(@$I2cMachineId_pa)
                {
                    if ($i2chwUnit_s == $rba_I2c_HwUnitPhy_s)
                    {
                        $rba_hwValidate_status = 1;

                    }
                }

                # Hardware unit validation.
                if ($rba_hwValidate_status == 0)
                {
                     conf_process::Exit(
                    "#\n" .
                    "# ERROR in configuration script of " . WhoAmI() . " ($nameAction_s):\n" .
                    "#\n" .
                    "#   The hardware Channel $rba_I2c_HwUnitPhy_s is not present in $MachineType_s machine type in $ecucPbCfg_Name_s PbVar found in ECU configuration parameters!.\n" .
                    "#\n" .
                    "#   configuration file: FileRef\n" .
                    "#   module description: Module\n" .
                    "#\n");
                }
            }
        }
    }
}

#----------------------------------------------------------------------------------------------------------------------
# GetMachineData: Subroutine to get the hash with the machine specific data
#
# Parameters:
# $_[0]: Machine type[IN]
# $_[1]: Directory which contains the machine dependent files[IN]
#
#----------------------------------------------------------------------------------------------------------------------
sub GetMachineData
{
    my $MachineType_s      = $_[0];
    my $path_scripts_s     = $_[1];

    $MachineType_s =~ /^MCU_RB\_([A-Za-z0-9]+_UC1)([A-Za-z0-9_]+)$/;

    if( (!defined $1) && (!defined $2) )
    {
        conf_process::Exit("ERROR - Variant can not be extracted from MCU information! ($MachineType_s)");
    }

    my $MachineInfoFile_s = "rba_I2C_" . $1 . ".pm";

    #add check if according perl scipts are available
    require "$path_scripts_s/$MachineInfoFile_s";

    return $MachineInfoFile_s;
}


#----------------------------------------------------------------------------------------------------------------------
# GenerateSlaveIdList: Create defines for slave Id names
#
# Parameters:
# $_[0]: Reference to (already prepared) configuration data [IN]
#----------------------------------------------------------------------------------------------------------------------
sub GenerateSlaveIdList
{
    my $ecucConfig_ph                   = $_[0];

    # Local variable.
    #---------------
    my $rba_I2c_SlaveId_s               = '';
    my $maxSignCount_s                  = 0;

    # set start string
    $rba_I2c_SlaveId_s = join("\n",
              '/*!',
              ' ***************************************************************************************************',
              ' \brief Definitions for Slave Id.',
              ' ***************************************************************************************************',
              ' */',
              '' # to get the last line feed
             );

    #loop over all signals
    foreach my $rba_I2c_SignalNum_s (sort(keys(%{$ecucConfig_ph->{'SlaveIdByNumber'}})))
    {
        # increment the signal count.
        $maxSignCount_s++;

        # read the signal name
        my $rba_I2c_Signal_s = $ecucConfig_ph->{'SlaveIdByNumber'}{$rba_I2c_SignalNum_s}{'rba_I2c_SlaveId_ShortName'};

        # update the info for gen file
        $rba_I2c_SlaveId_s .= CreateDefine( "rba_I2c_$rba_I2c_Signal_s", "$rba_I2c_SignalNum_s"."u");
    }
    $rba_I2c_SlaveId_s .= CreateDefine( "rba_I2c_MaxSignalCount_s", "$maxSignCount_s"."u");

    # return value.
    return ($rba_I2c_SlaveId_s);
}

#----------------------------------------------------------------------------------------------------------------------
# GenerateSlaveIdList: Generate Sequence List
#
# Parameters:
# $_[0]: Reference to (already prepared) configuration data [IN]
#----------------------------------------------------------------------------------------------------------------------
sub GenerateSequenceIdList
{
    my $sequenceConfig_ph                   = $_[0];

    # Local variable.
    #---------------

    my $rba_I2c_SequenceId_s                = '';
    my $maxSeqCount_s                       = 0;

    # set start string
    $rba_I2c_SequenceId_s = join("\n",
              '/*!',
              ' ***************************************************************************************************',
              ' \brief Definitions for Sequence Id.',
              ' ***************************************************************************************************',
              ' */',
              '' # to get the last line feed
             );
    # read the sequence ID
    my $slaveconfigID =   $sequenceConfig_ph->{'SequenceConfig_Id'};

    # update the sequence Number.
    foreach my $rba_I2c_SignalNum_s(keys(%{$slaveconfigID}))
    {
        #increment the sequence count.
        $maxSeqCount_s++;

        #read the sequence count.
        my $sequenceNum  = $slaveconfigID->{$rba_I2c_SignalNum_s}->{'rba_I2cSequenceConfig_Number'};

        # update the signal number info to generate file.
        $rba_I2c_SequenceId_s .= CreateDefine( "rba_I2c_$rba_I2c_SignalNum_s", "$sequenceNum"."u");
    }

    #  update the Maxsequence count info to generate file.
    $rba_I2c_SequenceId_s .= CreateDefine( "rba_I2c_MaxSequenceCount_s", "$maxSeqCount_s"."u");

  return ($rba_I2c_SequenceId_s);
}

#----------------------------------------------------------------------------------------------------------------------
# GenerateMaxHwUnit: Get the maximum number of hardware unit that used in all PbVariant
#
# Parameters:
# $_[0]: Reference to (already prepared) configuration data [IN]
# $_[1]: Machine type [IN]
#----------------------------------------------------------------------------------------------------------------------
sub GenerateMaxHwUnit
{
    my $ecucConfig_ph                   = $_[0];
    my $MachineType_s                   = $_[1];

    # Local variable.
    #---------------
    my $rba_I2c_MaxHwUnitCnt_s = 0;
    my %rba_I2C_MaxHwUnitInfo;
    my $rba_I2C_Available_Channel;
    my $rba_I2C_GenerateMaxHwUnit_counter = 0;

    # Get the maximum number of hardware units for all configuration datasets

    #loop over all signals
    foreach my $rba_I2c_PbCfgname_s (sort(keys(%{$ecucConfig_ph->{'UsedHwUnits'}})))
    {
        # read the Hardware uint count.
        my $rba_I2c_HwUnitCnt_s = $ecucConfig_ph->{'UsedHwUnits'}{$rba_I2c_PbCfgname_s};

        # update the Maxhardware unit count.
        $rba_I2c_MaxHwUnitCnt_s = $rba_I2c_HwUnitCnt_s if ($rba_I2c_HwUnitCnt_s > $rba_I2c_MaxHwUnitCnt_s);
    }

    # I2C channel availability in the Hardware
    my $I2cMachineId_pa = rba_I2c_UcMachineInfo::rba_I2c_GetId_uCInfo($MachineType_s);

    # read the available channel
    $rba_I2C_Available_Channel = scalar @{$I2cMachineId_pa};

    # Hardware Hash creation for th available channel
    $rba_I2C_MaxHwUnitInfo{'Available_Channels'} = $rba_I2C_Available_Channel;
    # Hardware Hash creation for th available channel
    $rba_I2C_MaxHwUnitInfo{'MaxConfigured_Channels'} = $rba_I2c_MaxHwUnitCnt_s;

    # available channel info update.
    for($rba_I2C_GenerateMaxHwUnit_counter = 0; $rba_I2C_GenerateMaxHwUnit_counter< $rba_I2C_Available_Channel; $rba_I2C_GenerateMaxHwUnit_counter++)
    {
        # check the hardware channel availability
        if(exists $ecucConfig_ph->{'HardwareUnits'}{'HwIdByPhyUnit'}->{$rba_I2C_GenerateMaxHwUnit_counter})
        {
            # update the channel info to available
            $rba_I2C_MaxHwUnitInfo{'Channel_list'}{'RBA_I2C_CFG_CHANNEL_CONF_AVAILABLE_'.$rba_I2C_GenerateMaxHwUnit_counter} = '1';
        }
        else
        {
             #  update the channel info to  not available
             $rba_I2C_MaxHwUnitInfo{'Channel_list'}{'RBA_I2C_CFG_CHANNEL_CONF_AVAILABLE_'.$rba_I2C_GenerateMaxHwUnit_counter} = '0';
        }

    }

    foreach my $conf_channels_s (keys (%{$ecucConfig_ph->{'HardwareUnits'}{'HwIdByPhyUnit'}}))
    {

        if($conf_channels_s < $rba_I2C_Available_Channel )
        {
           # configured channel is available
        }
        else
        {
            conf_process::Exit(
                    "#\n" .

                    "#   module description: Module\n configured Channel Number is Wrong " .
                    "#\n");

         }
     }

     # return value.
     return (\%rba_I2C_MaxHwUnitInfo);
}

#----------------------------------------------------------------------------------------------------------------------
# GeneraterbaI2cConfigType: Creates a string with the rba_I2c_ConfigType constants
#
# Parameters:
# $_[0]: Reference to (already prepared) configuration data [IN]
#----------------------------------------------------------------------------------------------------------------------
sub GeneraterbaI2cConfigType
{
  my $ecucConfig_ph                   = $_[0];

  # set start text
  my $rba_I2c_ConfigTypeDeclaration_s
    = "/*!\n"
    . " ***************************************************************************************************\n"
    . " \\brief Definitions of rba_I2c_ConfigType for each post build variant\n"
    . " ***************************************************************************************************\n"
    . " */\n";

  my $rba_I2c_ConfigTypeDefinition_s
    = "/****************************************************************************************************\n"
    . " Definitions of rba_I2c_ConfigType for each post build variant\n"
    . "****************************************************************************************************/\n";

        # In AR42 context the set of variants returned by GetVariants() may be a reduced set, because for variants with
    # identical input data only the 1st variant (master variant) is returned while the others are considered to be
    # matching variants to the master variant. Since it must be possible to initialize a component for all top level
    # predefined variants (TLPVs), config name macros must be generated for each matching variant which map to the
    # config name of the master variant.
    # The list returned by GetConfigMappingMacros() may be empty in which case the configuration processor shall
    # generate no marco. In AR40 context special AR42 compatibility macros are generated using the same mechanism.
    foreach my $strConfigMappingMacro_s (conf_process::GetConfigMappingMacros())
    {
        $rba_I2c_ConfigTypeDeclaration_s .= CreateDefine($strConfigMappingMacro_s, conf_process::GetConfigMappingMacroValue($strConfigMappingMacro_s));
    }

    $rba_I2c_ConfigTypeDeclaration_s .="\n\n";

    # Loop over all available PostBuild configurations
    # conf_process::GetVariants returns
    # AR40: rba_I2c_ConfigSet_4Cyl, rba_I2c_ConfigSet_6Cyl
    # AR42: VPOSTBS_Variant_Master_4cyl, VPOSTBS_Variant_Master_6cyl
    foreach my $strVariant_s (conf_process::GetVariants())
    {
    # get input data for current variant
    my $PbVar = conf_process::GetConfigName($strVariant_s);

    # get variant suffix needed to build short name of rba_I2C MCC (rba_I2c_ConfigSet)
    # conf_process::GetVariantSuffix returns
    # AR40: The variant name "rba_I2c_ConfigSet_4Cyl" will return "_4Cyl", "rba_I2c_ConfigSet" will return an empty string
    # AR42: The returned variant suffix will always be an empty string.

    # get number of HwUnits in this post build variant
    my $NumHwUnit_s = $ecucConfig_ph->{'PostBuildNames'}{$PbVar}{'HwUnitsUsed'};

    my $rba_I2c_ConfTypeName_s = $PbVar;

    # add start line
    $rba_I2c_ConfigTypeDefinition_s .= "const rba_I2c_ConfigType " . "$rba_I2c_ConfTypeName_s" . " =\n\{\n";

    # add number of Hw configs
    $rba_I2c_ConfigTypeDefinition_s .= CreateLine(1, "$ecucConfig_ph->{'UsedHwUnits'}{$PbVar},", "number of used HW units" , "");

    # add slave config array of pointer structure of each slave
    $rba_I2c_ConfigTypeDefinition_s .= CreateLine(1, $PbVar."_Slave_ast,", "Slave configurations " , "");

    # add hw unit array of pointer structure of each slave
    $rba_I2c_ConfigTypeDefinition_s .= CreateLine(1, $PbVar."_HwUnit_ast,", "Hw unit configurations" , "");

    # add hw unit array of pointer structure of each slave
    $rba_I2c_ConfigTypeDefinition_s .= CreateLine(1, $PbVar."_Sequence_ast", "Hw unit configurations" , "");

    # add end line
    $rba_I2c_ConfigTypeDefinition_s .= "\};\n\n";

    # create declaration string
    $rba_I2c_ConfigTypeDeclaration_s .= "extern const rba_I2c_ConfigType " . "$rba_I2c_ConfTypeName_s".";\n";
    }

  return ($rba_I2c_ConfigTypeDeclaration_s, $rba_I2c_ConfigTypeDefinition_s);
}


#----------------------------------------------------------------------------------------------------------------------
# GetBoolean: Converts a boolean parameter value (as stored in the arxml file) into the corresponding numeric value
#
# Parameters:
# $_[0]: Boolean parameter value as stored in arxml file [IN]
# $_[1]: Error message if conversion failed [IN]
#----------------------------------------------------------------------------------------------------------------------

sub GetBoolean
{
  # Store supplied information
  my $ecucBooleanValue_s = $_[0];
  my $strErrorMessage_s  = $_[1];

  # Validate supplied parameter value and convert into numeric value
  my $result_s = 0;
  if (($ecucBooleanValue_s eq 'true') || ($ecucBooleanValue_s eq '1'))
  {
    $result_s = 1;
  }
  elsif (($ecucBooleanValue_s eq 'false') || ($ecucBooleanValue_s eq '0'))
  {
    $result_s = 0;
  }
  else
  {
    conf_process::Exit($strErrorMessage_s);
  }
  return $result_s;
}

#----------------------------------------------------------------------------------------------------------------------
# Subroutine to generate a #define
#
# Parameters:
# $_[0]: Name of the define to be generated
# $_[1]: Value which should be assigned to the define
#----------------------------------------------------------------------------------------------------------------------
sub CreateDefine {
  my $name_s  = shift();
  my $value_s = shift();

  return sprintf( "#define %-50s (%s)\n", $name_s, $value_s );
}
#----------------------------------------------------------------------------------------------------------------------
# Subroutine to create a line with an aligned comment at the end
#
# Parameters:
# $_[0]: Number of indentation (4 Spaces per indentation)
# $_[1]: String to be added
# $_[2]: Comment at the end
# $_[3]: End String (e.g. line extender)
#----------------------------------------------------------------------------------------------------------------------
sub CreateLine {
  my $indent  = shift();
  my $string_s = shift();
  my $comment_s = shift();
  my $EndString_s = shift();

  my $entry_s = "";
  for (my$cnt=0; $cnt<$indent; $cnt++)
  {
    $entry_s .= "    ";
  }

  return sprintf( "%-50s/* %s */%s\n", $entry_s.$string_s, $comment_s, $EndString_s);
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

#----------------------------------------------------------------------------------------------------------------------
# Anonymous function required at the end of any Perl module
#----------------------------------------------------------------------------------------------------------------------
{
  1;
}

#<RBHead>
#***********************************************************************************************************************
# $History$
#***********************************************************************************************************************
#</RBHead>


