I-Logix-RPY-Archive version 8.14.0 C 9810360
{ IProject 
	- _id = GUID 5c714b90-5f0d-46db-94c7-83e791335c89;
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
	- _name = "rba_Dma_dSD";
	- _modifiedTimeWeak = 6.12.2017::10:19:53;
	- _theMainDiagram = { IHandle 
		- _m2Class = "IDiagram";
		- _id = GUID cb2c6869-c7fe-4b75-a6b1-75aecab8a5f5;
	}
	- _UserColors = { IRPYRawContainer 
		- size = 16;
		- value = 16777215; 16777215; 16777215; 16777215; 16777215; 16777215; 16777215; 16777215; 16777215; 16777215; 16777215; 16777215; 16777215; 16777215; 16777215; 16777215; 
	}
	- _defaultSubsystem = { ISubsystemHandle 
		- _m2Class = "ISubsystem";
		- _name = "rba_Dma";
		- _id = GUID 84288f41-4170-4d54-8ae6-2fc49e450c88;
	}
	- _component = { IHandle 
		- _m2Class = "IComponent";
		- _name = "DefaultComponent";
		- _id = GUID e9fe0dfd-af1f-4b6a-b937-69ef5fe6a6b9;
	}
	- Multiplicities = { IRPYRawContainer 
		- size = 4;
		- value = 
		{ IMultiplicityItem 
			- _name = "1";
			- _count = -1;
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
		- size = 3;
		- value = 
		{ ISubsystem 
			- fileName = "rba_Dma";
			- _id = GUID 84288f41-4170-4d54-8ae6-2fc49e450c88;
		}
		{ IProfile 
			- fileName = "SoftwareArchitect752C";
			- _persistAs = "$OMROOT\\Settings\\SoftwareArchitect";
			- _id = GUID 73d1c273-f433-4c6d-bb23-abfbac55b0aa;
			- _name = "SoftwareArchitectC";
			- _partOfTheModelKind = referenceunit;
		}
		{ IProfile 
			- fileName = "DGS_EC_SoftwareArch_Profile";
			- _persistAs = "C:\\Users\\RUT1COB\\Downloads\\BSWMD\\ArxmlGeneratorPlugin";
			- _id = GUID 8933c50e-92db-4c03-b5c8-2fa194f54d9e;
			- _partOfTheModelKind = referenceunit;
		}
	}
	- Diagrams = { IRPYRawContainer 
		- size = 2;
		- value = 
		{ IDiagram 
			- _id = GUID cb2c6869-c7fe-4b75-a6b1-75aecab8a5f5;
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
			- _name = "ModelOverviewrba_Dma_dSD";
			- _modifiedTimeWeak = 1.2.1990::0:0:0;
			- _lastModifiedTime = "6.12.2017::10:19:53";
			- _graphicChart = { CGIClassChart 
				- _id = GUID 9f189a4f-cb43-451f-9bf3-8d873ce9e50e;
				- m_type = 0;
				- m_pModelObject = { IHandle 
					- _m2Class = "IDiagram";
					- _id = GUID cb2c6869-c7fe-4b75-a6b1-75aecab8a5f5;
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
					- _id = GUID 267dbeaf-d2f1-48bc-a5fd-7bb809277211;
					- m_type = 78;
					- m_pModelObject = { IHandle 
						- _m2Class = "IClass";
						- _subsystem = "rba_Dma";
						- _name = "TopLevel";
						- _id = GUID 3d7f850e-a5d7-4469-a581-fc03de4a5f56;
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
							- _id = GUID b5afa0ea-6990-4f27-befd-fb880184a7b8;
							- m_name = "Attribute";
							- m_displayOption = Explicit;
							- m_bShowInherited = 0;
							- m_bOrdered = 0;
							- Items = { IRPYRawContainer 
								- size = 0;
							}
						}
						{ CGICompartment 
							- _id = GUID 0a3866ae-86da-4895-aae4-4b30176e2f92;
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
					- _id = GUID 9f6ce3e7-f0bd-48c5-80ee-a6a1580ef4fe;
					- m_type = 127;
					- m_pModelObject = { IHandle 
						- _m2Class = "ISubsystem";
						- _name = "rba_Dma";
						- _id = GUID 84288f41-4170-4d54-8ae6-2fc49e450c88;
					}
					- m_pParent = GUID 267dbeaf-d2f1-48bc-a5fd-7bb809277211;
					- m_name = { CGIText 
						- m_str = "rba_Dma";
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
				- m_pRoot = GUID 267dbeaf-d2f1-48bc-a5fd-7bb809277211;
				- m_currentLeftTop = 0 0 ;
				- m_currentRightBottom = 0 0 ;
				- m_bFreezeCompartmentContent = 0;
			}
			- _defaultSubsystem = { IHandle 
				- _m2Class = "ISubsystem";
				- _name = "rba_Dma";
				- _id = GUID 84288f41-4170-4d54-8ae6-2fc49e450c88;
			}
		}
		{ IDiagram 
			- _id = GUID 010db095-ebd5-4a1d-98f1-6f8840991e91;
			- _myState = 8192;
			- _name = "Model1";
			- _modifiedTimeWeak = 1.2.1990::0:0:0;
			- _lastModifiedTime = "5.12.2017::7:32:30";
			- _graphicChart = { CGIClassChart 
				- _id = GUID eed9f8c2-0210-4b2e-8c04-8b70cad321ee;
				- m_type = 0;
				- m_pModelObject = { IHandle 
					- _m2Class = "IDiagram";
					- _id = GUID 010db095-ebd5-4a1d-98f1-6f8840991e91;
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
				- elementList = 1;
				{ CGIClass 
					- _id = GUID 599cfe41-f639-40d6-95be-4caa549b643f;
					- m_type = 78;
					- m_pModelObject = { IHandle 
						- _m2Class = "IClass";
						- _subsystem = "rba_Dma";
						- _name = "TopLevel";
						- _id = GUID 3d7f850e-a5d7-4469-a581-fc03de4a5f56;
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
							- _id = GUID a35bc772-092b-4f5c-bded-770dbb039756;
							- m_name = "Attribute";
							- m_displayOption = Explicit;
							- m_bShowInherited = 0;
							- m_bOrdered = 0;
							- Items = { IRPYRawContainer 
								- size = 0;
							}
						}
						{ CGICompartment 
							- _id = GUID 5abcb444-9fbe-4fcc-9a08-736b3c71aab9;
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
				
				- m_access = 'Z';
				- m_modified = 'N';
				- m_fileVersion = "";
				- m_nModifyDate = 0;
				- m_nCreateDate = 0;
				- m_creator = "";
				- m_bScaleWithZoom = 1;
				- m_arrowStyle = 'S';
				- m_pRoot = GUID 599cfe41-f639-40d6-95be-4caa549b643f;
				- m_currentLeftTop = 0 0 ;
				- m_currentRightBottom = 0 0 ;
				- m_bFreezeCompartmentContent = 0;
			}
			- _defaultSubsystem = { IHandle 
				- _m2Class = "ISubsystem";
				- _name = "rba_Dma";
				- _id = GUID 84288f41-4170-4d54-8ae6-2fc49e450c88;
			}
		}
	}
	- Components = { IRPYRawContainer 
		- size = 1;
		- value = 
		{ IComponent 
			- fileName = "DefaultComponent";
			- _id = GUID e9fe0dfd-af1f-4b6a-b937-69ef5fe6a6b9;
		}
	}
}

