I-Logix-RPY-Archive version 8.14.0 C 9810360
{ IProject 
	- _id = GUID f0b0dd3c-dd87-49fd-a1c7-3ce7153042e4;
	- _myState = 8192;
	- _properties = { IPropertyContainer 
		- Subjects = { IRPYRawContainer 
			- size = 1;
			- value = 
			{ IPropertySubject 
				- _Name = "General";
				- Metaclasses = { IRPYRawContainer 
					- size = 1;
					- value = 
					{ IPropertyMetaclass 
						- _Name = "Model";
						- Properties = { IRPYRawContainer 
							- size = 1;
							- value = 
							{ IProperty 
								- _Name = "CommonTypes";
								- _Value = "$ALL";
								- _Type = String;
							}
						}
					}
				}
			}
		}
	}
	- _name = "rba_I2c_dSD";
	- codeUpdateCGTime = 7.4.2017::14:53:25;
	- Dependencies = { IRPYRawContainer 
		- size = 1;
		- value = 
		{ IDependency 
			- _id = GUID 554fd55a-a15c-4665-8641-ff03e7022b84;
			- _myState = 2048;
			- _name = "CodeCentricC";
			- codeUpdateCGTime = 5.8.2017::7:10:19;
			- Stereotypes = { IRPYRawContainer 
				- size = 1;
				- value = 
				{ IHandle 
					- _m2Class = "IStereotype";
					- _subsystem = "PredefinedTypes";
					- _name = "AppliedProfile";
					- _id = GUID d2eca2c1-e5a5-4296-9453-29283c4ed8bc;
				}
			}
			- _modifiedTimeWeak = 5.8.2017::7:10:19;
			- _dependsOn = { INObjectHandle 
				- _m2Class = "IProfile";
				- _name = "CodeCentricC";
				- _id = GUID b2503309-cb02-4df3-88ac-b10f4bb6aaf0;
			}
		}
	}
	- _modifiedTimeWeak = 7.4.2017::14:53:25;
	- _theMainDiagram = { IHandle 
		- _m2Class = "IDiagram";
		- _id = GUID 5f56c74b-1e3d-4262-bf12-05ed21620f33;
	}
	- _UserColors = { IRPYRawContainer 
		- size = 16;
		- value = 16777215; 16777215; 16777215; 16777215; 16777215; 16777215; 16777215; 16777215; 16777215; 16777215; 16777215; 16777215; 16777215; 16777215; 16777215; 16777215; 
	}
	- _defaultSubsystem = { ISubsystemHandle 
		- _m2Class = "ISubsystem";
		- _name = "rba_I2c";
		- _id = GUID 25e56b82-65b2-4104-bf9d-a5d545a5b30d;
	}
	- _component = { IHandle 
		- _m2Class = "IComponent";
		- _name = "DefaultComponent";
		- _id = GUID 54679677-5bd1-4550-a8de-de072add81ad;
	}
	- Multiplicities = { IRPYRawContainer 
		- size = 4;
		- value = 
		{ IMultiplicityItem 
			- _name = "1";
			- _count = 5;
		}
		{ IMultiplicityItem 
			- _name = "*";
			- _count = -1;
		}
		{ IMultiplicityItem 
			- _name = "0,1";
			- _count = -1;
		}
		{ IMultiplicityItem 
			- _name = "1..*";
			- _count = -1;
		}
	}
	- Subsystems = { IRPYRawContainer 
		- size = 4;
		- value = 
		{ ISubsystem 
			- fileName = "rba_I2c";
			- _id = GUID 25e56b82-65b2-4104-bf9d-a5d545a5b30d;
		}
		{ IProfile 
			- fileName = "SoftwareArchitect752C";
			- _persistAs = "$OMROOT\\Settings\\SoftwareArchitect";
			- _id = GUID 73d1c273-f433-4c6d-bb23-abfbac55b0aa;
			- _name = "SoftwareArchitectC";
			- _partOfTheModelKind = referenceunit;
		}
		{ IProfile 
			- fileName = "CodeCentric752C";
			- _persistAs = "$OMROOT\\Settings\\CodeCentric\\";
			- _id = GUID b2503309-cb02-4df3-88ac-b10f4bb6aaf0;
			- _name = "CodeCentricC";
			- _partOfTheModelKind = referenceunit;
		}
		{ IProfile 
			- fileName = "DGS_EC_SoftwareArch_Profile";
			- _persistAs = "..\\..\\..\\..\\..\\..\\..\\..\\..\\..\\new plugin";
			- _id = GUID 8933c50e-92db-4c03-b5c8-2fa194f54d9e;
			- _partOfTheModelKind = referenceunit;
		}
	}
	- Diagrams = { IRPYRawContainer 
		- size = 1;
		- value = 
		{ IDiagram 
			- _id = GUID 5f56c74b-1e3d-4262-bf12-05ed21620f33;
			- _myState = 8192;
			- _properties = { IPropertyContainer 
				- Subjects = { IRPYRawContainer 
					- size = 2;
					- value = 
					{ IPropertySubject 
						- _Name = "Format";
						- Metaclasses = { IRPYRawContainer 
							- size = 1;
							- value = 
							{ IPropertyMetaclass 
								- _Name = "Package";
								- Properties = { IRPYRawContainer 
									- size = 7;
									- value = 
									{ IProperty 
										- _Name = "DefaultSize";
										- _Value = "0,0,216,151";
										- _Type = String;
									}
									{ IProperty 
										- _Name = "Fill.FillColor";
										- _Value = "255,255,255";
										- _Type = Color;
									}
									{ IProperty 
										- _Name = "Font.Font";
										- _Value = "Tahoma";
										- _Type = String;
									}
									{ IProperty 
										- _Name = "Font.Size";
										- _Value = "8";
										- _Type = Int;
									}
									{ IProperty 
										- _Name = "Font.Weight@Child.NameCompartment@Name";
										- _Value = "700";
										- _Type = Int;
									}
									{ IProperty 
										- _Name = "Line.LineColor";
										- _Value = "109,163,217";
										- _Type = Color;
									}
									{ IProperty 
										- _Name = "Line.LineWidth";
										- _Value = "1";
										- _Type = Int;
									}
								}
							}
						}
					}
					{ IPropertySubject 
						- _Name = "ObjectModelGe";
						- Metaclasses = { IRPYRawContainer 
							- size = 1;
							- value = 
							{ IPropertyMetaclass 
								- _Name = "AutoPopulate";
								- Properties = { IRPYRawContainer 
									- size = 1;
									- value = 
									{ IProperty 
										- _Name = "EnabledOnUpdateModel";
										- _Value = "True";
										- _Type = Bool;
									}
								}
							}
						}
					}
				}
			}
			- _name = "ModelOverviewrba_I2c_dSD";
			- _modifiedTimeWeak = 1.2.1990::0:0:0;
			- _lastModifiedTime = "7.4.2017::12:0:15";
			- _graphicChart = { CGIClassChart 
				- _id = GUID 5fe126f8-31f5-4ee5-ae7f-48ff8b2f1a2b;
				- m_type = 0;
				- m_pModelObject = { IHandle 
					- _m2Class = "IDiagram";
					- _id = GUID 5f56c74b-1e3d-4262-bf12-05ed21620f33;
				}
				- m_pParent = ;
				- m_name = { CGIText 
					- m_str = "";
					- m_style = "Arial" 10 0 0 0 1 ;
					- m_color = { IColor 
						- m_fgColor = 0;
						- m_bgColor = 0;
						- m_bgFlag = 0;
					}
					- m_position = 1 0 0  ;
					- m_nIdent = 0;
					- m_bImplicitSetRectPoints = 0;
					- m_nOrientationCtrlPt = 8;
				}
				- m_drawBehavior = 0;
				- m_bIsPreferencesInitialized = 0;
				- elementList = 2;
				{ CGIClass 
					- _id = GUID 46ff69b0-8948-4f6f-85a3-daf5b057c5c6;
					- m_type = 78;
					- m_pModelObject = { IHandle 
						- _m2Class = "IClass";
						- _subsystem = "rba_I2c";
						- _name = "TopLevel";
						- _id = GUID aeaa566d-a2e7-4019-9061-7b11f745f308;
					}
					- m_pParent = ;
					- m_name = { CGIText 
						- m_str = "TopLevel";
						- m_style = "Arial" 10 0 0 0 1 ;
						- m_color = { IColor 
							- m_fgColor = 0;
							- m_bgColor = 0;
							- m_bgFlag = 0;
						}
						- m_position = 1 0 0  ;
						- m_nIdent = 0;
						- m_bImplicitSetRectPoints = 0;
						- m_nOrientationCtrlPt = 5;
					}
					- m_drawBehavior = 0;
					- m_bIsPreferencesInitialized = 0;
					- m_AdditionalLabel = { CGIText 
						- m_str = "";
						- m_style = "Arial" 10 0 0 0 1 ;
						- m_color = { IColor 
							- m_fgColor = 0;
							- m_bgColor = 0;
							- m_bgFlag = 0;
						}
						- m_position = 1 0 0  ;
						- m_nIdent = 0;
						- m_bImplicitSetRectPoints = 0;
						- m_nOrientationCtrlPt = 1;
					}
					- m_polygon = 0 ;
					- m_nNameFormat = 0;
					- m_nIsNameFormat = 0;
					- Compartments = { IRPYRawContainer 
						- size = 2;
						- value = 
						{ CGICompartment 
							- _id = GUID c1731ea8-c321-4d77-9010-70d2e0d78fa4;
							- m_name = "Attribute";
							- m_displayOption = Explicit;
							- m_bShowInherited = 0;
							- m_bOrdered = 0;
							- Items = { IRPYRawContainer 
								- size = 0;
							}
						}
						{ CGICompartment 
							- _id = GUID ee200c45-3424-4f7c-9876-37dd7bbf966f;
							- m_name = "Operation";
							- m_displayOption = Explicit;
							- m_bShowInherited = 0;
							- m_bOrdered = 0;
							- Items = { IRPYRawContainer 
								- size = 0;
							}
						}
					}
					- Attrs = { IRPYRawContainer 
						- size = 0;
					}
					- Operations = { IRPYRawContainer 
						- size = 0;
					}
				}
				{ CGIPackage 
					- _id = GUID 874cf5fc-143e-4212-a095-52f2cbf31b66;
					- m_type = 127;
					- m_pModelObject = { IHandle 
						- _m2Class = "ISubsystem";
						- _name = "rba_I2c";
						- _id = GUID 25e56b82-65b2-4104-bf9d-a5d545a5b30d;
					}
					- m_pParent = GUID 46ff69b0-8948-4f6f-85a3-daf5b057c5c6;
					- m_name = { CGIText 
						- m_str = "rba_I2c";
						- m_style = "Arial" 10 0 0 0 1 ;
						- m_color = { IColor 
							- m_fgColor = 0;
							- m_bgColor = 0;
							- m_bgFlag = 0;
						}
						- m_position = 1 0 0  ;
						- m_nIdent = 0;
						- m_bImplicitSetRectPoints = 0;
						- m_nOrientationCtrlPt = 5;
					}
					- m_drawBehavior = 4104;
					- m_transform = 0.0986842 0 0 0.0556038 40 40 ;
					- m_bIsPreferencesInitialized = 0;
					- m_AdditionalLabel = { CGIText 
						- m_str = "";
						- m_style = "Arial" 10 0 0 0 1 ;
						- m_color = { IColor 
							- m_fgColor = 0;
							- m_bgColor = 0;
							- m_bgFlag = 0;
						}
						- m_position = 1 0 0  ;
						- m_nIdent = 0;
						- m_bImplicitSetRectPoints = 0;
						- m_nOrientationCtrlPt = 1;
					}
					- m_polygon = 4 0 0  0 1151  1216 1151  1216 0  ;
					- m_nNameFormat = 0;
					- m_nIsNameFormat = 0;
				}
				
				- m_access = 'Z';
				- m_modified = 'N';
				- m_fileVersion = "";
				- m_nModifyDate = 0;
				- m_nCreateDate = 0;
				- m_creator = "";
				- m_bScaleWithZoom = 1;
				- m_arrowStyle = 'S';
				- m_pRoot = GUID 46ff69b0-8948-4f6f-85a3-daf5b057c5c6;
				- m_currentLeftTop = 0 0 ;
				- m_currentRightBottom = 0 0 ;
				- m_bFreezeCompartmentContent = 0;
			}
			- _defaultSubsystem = { IHandle 
				- _m2Class = "ISubsystem";
				- _name = "rba_I2c";
				- _id = GUID 25e56b82-65b2-4104-bf9d-a5d545a5b30d;
			}
		}
	}
	- Components = { IRPYRawContainer 
		- size = 1;
		- value = 
		{ IComponent 
			- fileName = "DefaultComponent";
			- _id = GUID 54679677-5bd1-4550-a8de-de072add81ad;
		}
	}
}

