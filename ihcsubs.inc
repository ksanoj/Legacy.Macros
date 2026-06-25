|- File: ihcsubs.inc
|- Version 1.1
|- Original File: auto_subs.inc
|- Auto-Subs Shared routines across Auto series macros by Noobhaxor
|- Version 6.2
|- Updated: //2018
|- Modified version of the above file to maintain compatability with the automac's / kissassist
|- Modified to fit the IHC mac's and mod's better
#Event Camping           "#*#seconds to prepare your camp."
#Event GainSomething	 "#*#You have gained|#1#|"
#Event TaskUpdate		 "#*#Your task |#1#| has been updated#*#" 
#Event Zoned             "LOADING, PLEASE WAIT#*#"
#Event Zoned             "You have entered#*#"
#Event CAST_IMMUNE                 "#*# is immune to #*#"
#Event CAST_IMMUNE                 "Your target cannot be mesmerized#*#"
#Event CAST_RESIST                 "#*# resisted your #1#!"
#bind QuestSay			/qsay
#bind BackOff        /backoff
#bind Campfire       /campfire
#bind ChangeVarInt   /changevarint
#bind ToggleVariable /togglevariable
#bind GroupBehindMe  /rallybehind
#bind GroupFrontFace /rallyfront
#bind KillMATarget   /killtarget
#Bind SheepMove	     /sheepmove
#bind IHCHudUpdate	/ihchudupdate
#bind TglUseHud		/tglusehud
#define HUDNAME "IHCMAIN"
#define MQHUDPATH ".\..\MQ2HUD.ini"
|----------------------------------------------------------------------------
|- SUB: Compatibility with KissAssist Aliases Old and New
|----------------------------------------------------------------------------
Sub AliasSetup
	|- KissAssist and AutoMac Common Commands
	/squelch /alias /camphere       /togglevariable ReturnToCamp 1
	/noparse /alias /campoff	    /togglevariable ReturnToCamp 0
	/squelch /alias /campradius     /changevarint General CampRadius
	/squelch /alias /chase          /togglevariable ChaseAssist
	/squelch /alias /chaseoff       /changevarint General ChaseAssist 0        
	/squelch /alias /chaseon        /changevarint General ChaseAssist 1 
	/noparse /alias /instantrelease /togglevariable InstantRelease
	/squelch /alias /meleeon        /togglevariable MeleeOn 1
	/squelch /alias /meleeoff       /togglevariable MeleeOn 0
	/squelch /alias /returntocamp   /togglevariable ReturnToCamp
	/noparse /alias /usefellowship 	/togglevariable UseFellowship
	|- IHC Mac common commands
	/noparse /alias /ihcstart		/multiline ; /docommand /defaulthud ; /docommand /macro IHCMercs\IHC${Me.Class.ShortName}.mac
	/squelch /alias /assistrange	/changevarint Set AssistRange
	/squelch /alias /autoassistat	/changevarint Set AutoAssistAt
	/squelch /alias /burn			/changevarint Toggle BurnNow
	/squelch /alias /burncount		/changevarint Set BurnMobCount
	/squelch /alias /campexact		/changevarint Toggle AutoCampHold
	/squelch /alias /endmedpct		/changevarint Set EndMedPct
	/squelch /alias /gpause			/changevarint Toggle GroupPause
	/squelch /alias /gbackoff		/changevarint Toggle GroupBackOff
	/squelch /alias /medpct			/changevarint Set MedPct
	/squelch /alias /smartassist	/changevarint Toggle UseSmartAssist
	/squelch /alias /tglburn		/changevarint Toggle BurnAlways
	/squelch /alias /tglevents		/changevarint Toggle EventLogic
	/squelch /alias /tglmed			/changevarint Toggle DoMed
	/squelch /alias /tglmelee		/changevarint Toggle UseMelee
	/squelch /alias /deluxehud		/changevarint Toggle DeluxeHud
	/squelch /alias /driverhud		/changevarint Toggle DriverHud
	/squelch /alias /ihcstatus		/changevarint Toggle StatusHud
	/squelch /alias /ihchud			/changevarint Toggle ShowAllHud
	/squelch /alias /ihchuddelete	/changevarint Toggle IHCHudDelete
/return
Sub VarSetup
	/declare acverbose			int outer 0
	/declare autotargetid		int outer 0
	/declare sittimer			timer outer 0
	/declare enablesit			int outer 0
	/declare ReturnToCamp		int outer 0
	/declare AutoCampX			int outer ${Me.X}
	/declare AutoCampY			int outer ${Me.Y}
	/declare changetoini		int outer 0
	/declare targetcleartimer	timer outer 0
	/declare numoftargets		int outer 0
	/declare ChaseAssist		int outer 0		
	|- IHC macro's common variables
	/declare GrPCond 			string outer .
	/declare TnKCond 			string outer .
	/declare PetTank 			int outer 0	
	/declare assistname 		string outer
	/declare assistid 			string outer
	/declare MainAssistType 	string outer
	/declare Ok2Assist			int outer 1
	/declare UseSpellInterrupt	int outer 0
	/declare burnnow			int outer 0
	/declare BackOffFlag		int outer 0
	/declare BeforeCombat		string outer NULL
	/declare GroupPauseFlag		int outer 0
	/declare GroupBackOffFlag	int outer 0
	/declare ReTargetDelay		timer outer	0
	/declare ShitOnCursor		string outer NULL
	/declare CursorTimer		timer outer 0
	/declare NoDisease			int outer 0
	/declare NoCorruption		int outer 0
	/declare NoPoison			int outer 0
	/declare NoFire				int outer 0
	/declare NoCold				int outer 0
	/declare NoChromatic		int outer 0
	/declare NoMagic			int outer 0
	/declare EventSetupFlag		int outer 0
	/declare KillTargetID		int outer 0
	/declare SpawnSearchType	string outer npc
	/echo -----${MacroName} LOADING------
	/if (${Me.XTarget[1].TargetType.NotEqual[Auto Hater]}) {
		/echo Extended Target 1 HAS TO BE SET TO Auto Hater
		/xtarget set 1 Autohater
		/echo Set Extended Target 1 to Auto
	  }
	|-Check for loaded plugins
	/call CheckPlugin MQ2Cast
    /call CheckPlugin MQ2Exchange
	/call CheckPlugin MQ2Rez
	/call CheckPlugin MQ2AdvPath
	/call CheckPlugin MQ2HUD
	/call UnCheckPlugin MQ2Twist
	/squelch /rez accept on
    /squelch /rez loot off
	/squelch /rez pct 90
	|-Disable mq2melee if its running
	/if (${Plugin[MQ2Melee].Name.Equal[mq2Melee]}) {
		/if (${Melee.Enable}) {
			/melee off
		}
	}
/return
|----------------------------------------------------------------------------
|- SUB IHC Common Config Options - Config options that will be the same across all mac's
|----------------------------------------------------------------------------
Sub LoadCommonConfig
	/call LoadIni Combat DoAutoTarget			int 1
	/call LoadIni Combat DoAutoEngage			int 1
	/call LoadIni General FollowToonName 		string NULL
	/call LoadIni General AssistRange			int 100
	/call LoadIni General AutoAssistAt			int 98
	/call LoadIni General AutoCampHold			int 1
	/call LoadIni General AutoCampRadius		int 30
	/call LoadIni General BurnAlways 			int 0
	/call LoadIni General BurnMobCount 			int 3
	/call LoadIni General DoMed				 	int 1
	/call LoadIni General EndMedPct				int 80
	/call LoadIni General MedPct			 	int 80
	/call LoadIni General InstantRelease 		int 0
	/call LoadIni General UseFellowship 		int 0
	/call LoadIni General UseSmartAssist		int 1
	/call LoadIni General UseEventLogic			int 0
	/call LoadIni HUD UseHud					int 1
	/call LoadIni HUD ShowDeluxeHud				int 1
	/call LoadIni HUD ShowDriverHud				int 1
	/call LoadIni HUD ShowMacStatusHud			int 1
	/if (${AutoCampHold}) /moveto dist 2
	/stick set breakontarget on
/return
|----------------------------------------------------------------------------
|- SUB IHC Common Config Options - Config options that will be the same across all mac's
|----------------------------------------------------------------------------
Sub SaveCommonConfig
	/call SetIni Combat DoAutoTarget			int ${DoAutoTarget}
	/call SetIni Combat DoAutoEngage			int ${DoAutoEngage}
	/call SetIni General FollowToonName 		string ${FollowToonName}
	/call SetIni General AssistRange			int ${AssistRange}
	/call SetIni General AutoAssistAt			int ${AutoAssistAt}
	/call SetIni General AutoCampHold			int ${AutoCampHold}
	/call SetIni General AutoCampRadius			int ${AutoCampRadius}
	/call SetIni General BurnAlways 			int ${BurnAlways}
	/call SetIni General BurnMobCount 			int ${BurnMobCount}
	/call SetIni General DoMed				 	int ${DoMed}
	/call SetIni General EndMedPct				int ${EndMedPct}
	/call SetIni General MedPct				 	int ${MedPct}
	/call SetIni General InstantRelease 		int ${InstantRelease}
	/call SetIni General UseFellowship 			int ${UseFellowship}
	/call SetIni General UseSmartAssist			int ${UseSmartAssist}
	/call SetIni General UseEventLogic			int ${UseEventLogic}
	/call SetIni HUD UseHud						int ${UseHud}
	/call SetIni HUD ShowDeluxeHud				int ${ShowDeluxeHud}
	/call SetIni HUD ShowDriverHud				int ${ShowDriverHud}
	/call SetIni HUD ShowMacStatusHud			int ${ShowMacStatusHud}
/return
|----------------------------------------------------------------------------
|- SUB: Bind Change Var Int resets various interger settings from ini file
|- Removed mez and slow toggles. Class specific abilities will be handled in the class specific mac's
|----------------------------------------------------------------------------
Sub Bind_ChangeVarInt(string ISection, string IName, int IVar)
	/docommand /varset changetoini 1
	|-ChaseAssist Var Set
    /if (${IName.Equal[ChaseAssist]} && ${IVar}==1 && ${Target.Type.Equal[PC]}) {
		/docommand /varset FollowToonName ${Target.Name}
		/echo \aw Following ${If[${FollowToonName.Equal[NULL ]},\ar OFF,\ag ${FollowToonName}]}
	} else /if (${IName.Equal[ChaseAssist]} && ${IVar}==0) {
		/varset FollowToonName NULL
		/echo \aw Following \ar OFF
		/if (${AdvPath.Following}) /docommand /afollow off
	}
	|-Camp Radius Set
	/if (${IName.Equal[CampRadius]} && ${IVar}>=20 && ${IVar}<=100) {
		/varset AutoCampRadius ${IVar}
		/echo \aw Camp Radius set to \ag ${IVar}
	} else /if (${IName.Equal[CampRadius]} && ${IVar}<20 && ${IVar}>100) {
		/echo \ar Invalid \aw Camp Radius - Set between 20-100 units
	}
	
	/if (${ISection.Equal[Toggle]}) {
		  |--Burn
		/if (${IName.Equal[BurnNow]}) {
			/if (!${burnnow}) {
				/echo \ag BURNING NOW
				/varset burnnow 1
			} else {
				/echo \aw Resetting BURN to \ar OFF
				/varset burnnow 0
			}
  |--AlwaysBurn
		} else /if (${IName.Equal[BurnAlways]}) {
			/if (!${BurnAlways}) {
				/echo \aw Setting BurnAlways to \ag ON
				/varset BurnAlways 1
			} else {
				/echo \aw Resetting BurnAlways to \ar OFF
				/varset BurnAlways 0
			}
  |-- DoMed
		} else /if (${IName.Equal[DoMed]}) {
			/if (!${DoMed}) {
				/echo \aw Setting DoMed to \ag ON
				/varset DoMed 1
			} else {
				/echo \aw Resetting DoMed to \ar OFF
				/varset DoMed 0
			}
  |-- Use Smart Assist
		} else /if (${IName.Equal[UseSmartAssist]}) {
			/if (!${UseSmartAssist}) {
				/echo \aw Setting UseSmartAssist to \ag ON
				/varset UseSmartAssist 1
			} else {
				/echo \aw Resetting UseSmartAssist to \ar OFF
				/varset UseSmartAssist  0
			}
  |-- Use Event Logic - /tglevents
		} else /if (${IName.Equal[EventLogic]}) {
			/if (!${UseEventLogic}) {
				/echo \aw Setting UseEventLogic to \ag ON
				/varset UseEventLogic 1
				/call ZoneEventChk
			} else {
				/echo \aw Resetting UseEventLogic to \ar OFF
				/varset UseEventLogic  0
			}
  |-- Auto Camp Hold - /campexact
		} else /if (${IName.Equal[AutoCampHold]}) {
			/if (!${AutoCampHold}) {
				/echo \aw Setting AutoCampHold to \ag ON
				/varset AutoCampHold 1
				/moveto dist 2
			} else {
				/echo \aw Resetting AutoCampHold to \ar OFF
				/varset AutoCampHold 0
				/moveto dist 10
			}
		} else /if (${IName.Equal[DeluxeHud]}) {
			/if (!${ShowDeluxeHud}) {
				/echo \aw Setting ShowDeluxeHud to \ag ON
				/varset ShowDeluxeHud 1
			} else {
				/echo \aw Resetting ShowDeluxeHud to \ar OFF
				/varset ShowDeluxeHud 0
			}
		} else /if (${IName.Equal[DriverHud]}) {
			/if (!${ShowDriverHud}) {
				/echo \aw Setting ShowDriverHud to \ag ON
				/varset ShowDriverHud 1
			} else {
				/echo \aw Resetting ShowDriverHud to \ar OFF
				/varset ShowDriverHud 0
			}
		} else /if (${IName.Equal[StatusHud]}) {
			/if (!${ShowMacStatusHud}) {
				/echo \aw Setting ShowMacStatusHud to \ag ON
				/varset ShowMacStatusHud 1
			} else {
				/echo \aw Resetting ShowMacStatusHud to \ar OFF
				/varset ShowMacStatusHud 0
			}
		} else /if (${IName.Equal[ShowAllHud]}) {
			/if (!${ShowMacStatusHud} || !${ShowDeluxeHud} || !${ShowDriverHud}) {
					/varset ShowMacStatusHud 1
					/varset ShowDeluxeHud 1
					/varset ShowDriverHud 1
			}
		} else /if (${IName.Equal[GroupPause]} && ${EQBC.Connected}) {
			/if (!${GroupPauseFlag}) {
					/varset GroupPauseFlag 1
			} else {
					/varset GroupPauseFlag 0
			}
			/docommand /bcg //mqp
		} else /if (${IName.Equal[GroupBackOff]} && ${EQBC.Connected}) {
			/if (!${GroupBackOffFlag}) {
					/varset GroupBackOffFlag 1
			} else {
					/varset GroupBackOffFlag 0
			}
			/docommand /bcg //backoff
		} else /if (${IName.Equal[IHCHudDelete]}) {
			/call DeleteHud
		} else /if (${IName.Equal[UseMelee]}) {
			/if (!${UseMelee}) {
				/echo \aw Setting UseMelee to \ag ON
				/varset UseMelee 1
			} else {
				/echo \aw Resetting UseMelee to \ar OFF
				/varset UseMelee 0
			}
		}
	} else /if (${ISection.Equal[Set]}) {
  |--BurnCount
		/if (${IName.Equal[BurnMobCount]}) {
			/if (${IVar}>=2 && ${IVar}<=100) {
				/varset BurnMobCount ${IVar}
				/echo \aw ${IName} set to \ag ${IVar}
			} else {
				/beep
				/echo \ar Invalid \aw ${IName} - Set between 2-100 mobs
			}
  |--AutoAssistAt
		} else /if (${IName.Equal[AutoAssistAt]}) {
			/if (${IVar}>=10 && ${IVar}<=100) {
				/varset AutoAssistAt ${IVar}
				/echo \aw ${IName} set to \ag ${IVar}
			} else {
				/beep
				/echo \ar Invalid \ag ${IName} \aw HP%- Set between 10-100
			}
  |--AssistRange
		} else /if (${IName.Equal[AssistRange]}) {
			/if (${IVar}>=20 && ${IVar}<=200) {
				/varset AssistRange ${IVar}
				/echo \aw ${IName} set to \ag ${IVar}
			} else {
				/beep
				/echo \ar Invalid \ag ${IName} \aw - Set between 20-200 units
			}
  |--MedPct
		} else /if (${IName.Equal[MedPct]}) {
			/if (${IVar}>=0 && ${IVar}<100) {
				/varset MedPct ${IVar}
				/echo \aw ${IName} set to \ag ${IVar}
			} else {
				/beep
				/echo \ar Invalid \ag ${IName}% \aw - Set between 0-100
			}
		} else /if (${IName.Equal[EndMedPct]}) {
			/if (${IVar}>=0 && ${IVar}<100) {
				/varset MedPct ${IVar}
				/echo \aw ${IName} set to \ag ${IVar}
			} else {
				/beep
				/echo \ar Invalid \ag ${IName}% \aw - Set between 0-100
			}
		}
	}
/return
|-------------------------------------------------------------------------------------
|- SUB: Bind_ToggleVariable  Check whenever the player is changing any variable via /echo
|-------------------------------------------------------------------------------------
Sub Bind_ToggleVariable(string Command, string Command2, string Command3)
    /declare i int local
	/declare OnOff local
    |- Toggle Variables & Set Variables
    /if (${Defined[${Command}]}) {
        /if (${Command2.Length} && ${Command2.NotEqual[null]}) {
            /if (${Select[${Command2},0,off]}) {
                /varset ${Command} 0
                /varset OnOff Off
            } else /if (${Select[${Command2},1,on]}) {
                /varset ${Command} 1
                /varset OnOff On
            } 
        } else /if (${${Command}}) {
			/varset ${Command} 0
            /varset OnOff Off
		} else {
            /varset ${Command} 1
            /varset OnOff On
        }
		/docommand /varset changetoini 1
		|-Camp Toggle
        /if (${Command.Equal[ReturnToCamp]} && ${OnOff.Equal[on]} ) {
			/echo \aw Camp turned \ag ON
			/docommand /varset ReturnToCamp 1
			/docommand /varset AutoCampX ${Me.X}
			/docommand /varset AutoCampY ${Me.Y}
			/if (${AutoCampHold}) {
				/moveto dist 2
			}
        } else /if (${Command.Equal[ReturnToCamp]} && ${OnOff.Equal[off]}) {
            /echo \aw Turning ALL Camp settings to \ar OFF
			/docommand /varset ReturnToCamp 0
			/moveto dist 10
        }
		|-Chase Toon Toggle
		/if (${Command.Equal[ChaseAssist]} && ${OnOff.Equal[on]} && ${Target.Type.Equal[PC]}) {
			/docommand /varset FollowToonName ${Target.Name}
			/echo \aw Following ${If[${FollowToonName.Equal[NULL ]},\ar OFF,\ag ${FollowToonName}]}
		} else /if (${Command.Equal[ChaseAssist]} && ${OnOff.Equal[off]}) {
			/varset FollowToonName NULL
			/echo \aw Following \ar OFF
			/if (${AdvPath.Following}) /docommand /afollow off
		}
		|-Melee Toggle
		/if (${Command.Equal[MeleeOn]} && ${OnOff.Equal[on]} ) {
			/varset UseMelee 1
		} else /if (${Command.Equal[MeleeOn]} && ${OnOff.Equal[off]}) {
			/varset UseMelee 0
		}
		|-InstantRelease on Death
		/if (${Command.Equal[InstantRelease]} && ${OnOff.Equal[on]} ) {
			/varset InstantRelease 1
		} else /if (${Command.Equal[InstantRelease]} && ${OnOff.Equal[off]}) {
			/varset InstantRelease 0
		}
        |-Use Fellowship to port back on death
		/if (${Command.Equal[UseFellowship]} && ${OnOff.Equal[on]} ) {
			/varset UseFellowship 1
		} else /if (${Command.Equal[UseFellowship]} && ${OnOff.Equal[off]}) {
			/varset UseFellowship 0
		}
        /echo \aw >> Set \ag ${Command}\aw: \ay ${OnOff}
    }
/return
|----------------------------------------------------------------------------  
|- SUB: SetControlToon
|----------------------------------------------------------------------------
Sub SetControlToon
	|-Set Main Assist/Tank
	/if (${Target.ID}) {
		/if (${Target.Type.Equal[Pet]}) {
			/varset TnKCond .Pet.
			/varset PetTank 1
			/varset assistname 	${Target.Master.Name}
		} else {
			/varset PetTank 0
			/varset assistname 	${Target.Name}
		}
		/varset assistid 	${Target.ID}
		/varset MainAssistType ${Target.Type}
	} else /if (${Group}) {
		/if (${Group.MainAssist.ID}) {
			/echo \aw No Target Selected - Switching Group Main Assist
			/target id ${Group.MainAssist.ID}
			/delay 1s ${Target.ID}
			/if (${Target.Type.Equal[Pet]}) {
				/varset TnKCond .Pet.
				/varset PetTank 1
				/varset assistname ${Target.Master.Name}
			} else {
				/varset PetTank 0
				/varset assistname ${Target.Name}
			}
		} else {
			/echo \aw No Target Selected - Switching to First Group Member
			/squelch /keypress f2
			/delay 1s ${Target.ID}
			/if (${Target.Type.Equal[Pet]}) {
				/varset TnKCond .Pet.
				/varset PetTank 1
				/varset assistname ${Target.Master.Name}
			} else {
				/varset PetTank 0
				/varset assistname ${Target.Name}
			}
		}
		/varset assistid  ${Target.ID}
		/varset MainAssistType ${Target.Type}
	} else /if (${Me.Class.ShortName.Equal[ENC]}) {
		/beep
		/echo \aw This Macro Requires a Group or Player Target at Start up
		/echo \aw Ending Macro
		/endmac
	} else {
		/echo \aw No Group or Target - Switching to Self Assist mode
		/varset assistname		${Me.Name}
		/varset MainAssistType 	${Me.Type}
		/varset assistid 		${Me.ID}
	}
/return 
|----------------------------------------------------------------------------  
|- SUB: Back Off and stop melee  New Version
|----------------------------------------------------------------------------      
Sub Bind_BackOff  
	/call DoBackOff
/return 
Sub DoBackOff
	/if (${BackOffFlag}) {
		/echo \aw Resetting. No Longer Backing off. \ar OFF
		/varset BackOffFlag 0
	} else {
		/echo \aw Resetting. Backing off. \ag ON
		/if (${Target.ID}) /target clear
		/if (${Me.Combat}) /squelch /attack off 
		/if (${Stick.Active}) /stick off
		/if (${Me.Pet.ID}) /pet back off
		/varset BackOffFlag 1
		/call AutoCampCheck
	}
	/doevents flush Event_BackOff  
/return
|----------------------------------------------------------------------------
|- SUB: Event Camping - end macro
|----------------------------------------------------------------------------
Sub Event_Camping	
	/if (${Me.Casting.ID}) /stopcast
	/end
/return
|----------------------------------------------------------------------------
|- SUB: Event CAST_RESIST
|----------------------------------------------------------------------------
Sub Event_CAST_RESIST
	/varset CastResult CAST_RESIST
/return
|----------------------------------------------------------------------------
|- SUB: Event CAST_IMMUNE
|----------------------------------------------------------------------------
Sub Event_CAST_IMMUNE
	/varset CastResult CAST_IMMUNE
/return
|----------------------------------------------------------------------------
|- SUB: Campfire ${Window[FellowshipWnd].Child[FP_CampPage].Child[FP_CampsiteViewer].Text}
|----------------------------------------------------------------------------
Sub Bind_Campfire
	/if ((${Me.Fellowship.Campfire} && (${Select[${Me.Fellowship.CampfireZone},${Zone.Name}]} || ${Me.Fellowship.CampfireZone.Name.Find[guild hall]})) || (${Me.InInstance}==FALSE)) /return
    /declare FellowCount int local 0
    /declare i int local
    /declare j int local
    /for i 1 to ${SpawnCount[pc radius 50]}
        /for j 1 to ${Me.Fellowship.Members}
            /if (${NearestSpawn[${i},PC radius 50].CleanName.Equal[${Me.Fellowship.Member[${j}]}]}) /varcalc FellowCount ${FellowCount}+1
        /next j
    /next i
    /if (${FellowCount}>=3) {
        /windowstate FellowshipWnd open
		/delay 10
		/nomodkey /notify FellowshipWnd FP_Subwindows tabselect 2
		/nomodkey /notify FellowshipWnd FP_DestroyCampsite leftmouseup
		/delay 5s ${Window[ConfirmationDialogBox].Open}
		/if (${Window[ConfirmationDialogBox].Open}) /nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
		/delay 5s !${Me.Fellowship.Campfire}
		/delay 1s
		/nomodkey /notify FellowshipWnd FP_RefreshList leftmouseup
		/delay 1s        
		/nomodkey /notify FellowshipWnd FP_CampsiteKitList listselect 1
		/delay 1s
		/nomodkey /notify FellowshipWnd FP_CreateCampsite leftmouseup
		/delay 5s ${Me.Fellowship.Campfire}
		/windowstate FellowshipWnd close
		/if (${Me.Fellowship.Campfire}) /echo Campfire Dropped
    }
/return
|----------------------------------------------------------------------------
|- SUB: Event Zoned
|----------------------------------------------------------------------------
Sub Event_Zoned(Message)
	/call Zoning
/return
	
Sub Zoning
	/delay 1m !${Me.Zoning}
	/if (${Bool[${FollowToonName}]}) /delay 1m ${Spawn[${FollowToonName}].ID}
	/delay 50
	/if (${ReturnToCamp}!=0) /varset ReturnToCamp 0
/return
|----------------------------------------------------------------------------
|- SUB: Load Ini
|----------------------------------------------------------------------------
Sub LoadIni(string sectionName, string varName, string varType, string varValue)     
	/if (!${Defined[${varName}]} && ${Defined[varType]}) /declare ${varName} ${varType} outer 0
	/if (!${Ini[${IniFileName},${sectionName},${varName}].Length}) {
		/if (${varValue.Length}) {
			/ini "${IniFileName}" "${sectionName}" "${varName}" "${varValue}"
			/varset ${varName} ${varValue}
			/echo \aw set \ag ${varName} \ay ${varValue}
		}
	} else {
		/varset ${varName} ${Ini[${IniFileName},${sectionName},${varName}]} 
		/echo \aw set \ag ${varName} \ay ${varValue}
	}
/return
|----------------------------------------------------------------------------
|- SUB: Set Ini
|----------------------------------------------------------------------------
Sub SetIni(string sectionName, string varName, string varType, string varValue)
    /if (${varValue.Length}) {
        /ini "${IniFileName}" "${sectionName}" "${varName}" "${varValue}"
    }
/return
|----------------------------------------------------------------------------
|- SUB: FollowToon
|---------------------------------------------------------------------------- 
Sub FollowToon
/if (${Spawn[pc =${FollowToonName}].ID}) {
	/if (${Me.Dead}) /return
	/if (${Spawn[pc =${FollowToonName}].Distance}>20 && !${Spawn[pc =${FollowToonName}].Dead} && ${Spawn[pc =${FollowToonName}].Distance}<400) {
		 /squelch /afollow spawn ${Spawn[pc =${FollowToonName}].ID}
		 /delay 2s ${Spawn[pc =${FollowToonName}].Distance}<20
		 /if (${Spawn[pc =${FollowToonName}].Distance}<20) /squelch /afollow off
	}
}
/return
|----------------------------------------------------------------------------
|- SUB: AutoCampCheck - Modified - AutoCampHold set to 1 will cause a return to camp /loc everytime and not just when we move out of AutoCampRadius
|----------------------------------------------------------------------------
Sub AutoCampCheck
|----Camp Return Logic
	/if (${Spawn[=${FollowToonName}].ID} && (!${Me.Casting.ID}) && !${MoveTo.Moving} && ${ReturnToCamp}==1) {
		/varset ReturnToCamp 0
		/echo \aw FollowToon Detected - \ar Disabling Camp
	} else /if (${Stick.Status.Equal[ON]} && ${ReturnToCamp}==1) {
|-		/varset ReturnToCamp 0
|-		/echo \aw Stick Detected - \ar Disabling Camp
	} else /if (${AdvPath.Following} && ${ReturnToCamp}==1) {
		/varset ReturnToCamp 0
		/echo \aw AdvPath Follow Detected - \ar Disabling Camp
	} else /if (!${Spawn[=${FollowToonName}].ID} && ${Stick.Status.Equal[OFF]} && !${Me.Casting.ID} && ${Math.Distance[${Me.Y},${Me.X}:${AutoCampY},${AutoCampX}]}>${AutoCampRadius} && (${Math.Distance[${Me.Y},${Me.X}:${AutoCampY},${AutoCampX}]}<400) && !${MoveTo.Moving} && ${ReturnToCamp}==1) {
		/moveto loc ${AutoCampY} ${AutoCampX}|on
		:movinghome
		/delay 5
		/if ((${Math.Distance[${Me.Y},${Me.X}:${AutoCampY},${AutoCampX}]}>20) && (${MoveTo.Moving})) /goto :movinghome
		/moveto off
	} else /if (${AutoCampHold} && !${Spawn[=${FollowToonName}].ID} && ${Stick.Status.Equal[OFF]} && !${Me.Casting.ID} && (${Math.Distance[${Me.Y},${Me.X}:${AutoCampY},${AutoCampX}]}<400) && (${Math.Distance[${Me.Y},${Me.X}:${AutoCampY},${AutoCampX}]}>=5) && !${MoveTo.Moving} && ${ReturnToCamp}==1) {
		/moveto loc ${AutoCampY} ${AutoCampX}|on
		:movinghome2
		/delay 5
		/if ((${Math.Distance[${Me.Y},${Me.X}:${AutoCampY},${AutoCampX}]}>5) && (${MoveTo.Moving})) /goto :movinghome2
		/moveto off
	}
/return
|----------------------------------------------------------------------------
|- SUB: BagModRods
|----------------------------------------------------------------------------
Sub BagModRods
	/if ((${Cursor.ID}) && ${CursorTimer}==0 && ${ShitOnCursor.Equal[NULL]} && (${Me.FreeInventory}>1)) {
		/varset ShitOnCursor ${Cursor}
		/varset CursorTimer 1m
	} else /if (${Cursor.ID} && ${CursorTimer}==0 && ${ShitOnCursor.NotEqual[NULL]} && (${Me.FreeInventory}>1)) {
		/echo \aw Something has been left on cursor for over a minute Bagging it
		/autoinventory
		/varset ShitOnCursor NULL
	} else /if (${Cursor.ID} && ${ShitOnCursor.Find[Transvergence]} && (${Me.FreeInventory}>1)) {
		/echo \aw Bagging Mod Rod
		/autoinventory
		/varset ShitOnCursor NULL
	} else /if (${Cursor.ID} && ${ShitOnCursor.Find[Modulation]} && (${Me.FreeInventory}>1)) {
		/echo \aw Bagging Mod Rod
		/autoinventory
		/varset ShitOnCursor NULL
	}
/return
|----------------------------------------------------------------------------
|- SUB: Event Gain Something - EQBC message 
|----------------------------------------------------------------------------    
Sub Event_GainSomething(string Line,string text)
    /if (!${EQBC.Connected}) /return
    /if (${Line.Find["ABILITY POINT!"]}) {
        /docommand /bc [+w+]${Me.Name} gained an AA, now has ${Me.AAPoints} unspent[+x+]
    } else /if (${Line.Find[LEVEL]}) {
        /docommand /bc [+w+]${Me.Name} gained a level, now is Level ${Me.Level}[+x+]
	}
/return 
|----------------------------------------------------------------------------
|- SUB: Event Task Update - EQBC message 
|----------------------------------------------------------------------------    
Sub Event_TaskUpdate(string Line,string name) 
    /if (${EQBC.Connected} && ${acverbose}!=0) /docommand /bc [+t+]Task updated...(${name})[+x+]    
/return
|----------------------------------------------------------------------------
|- SUB: Check Plugin
|----------------------------------------------------------------------------
Sub CheckPlugin(string pluginname)
    /if (!${Bool[${Plugin[${pluginname}]}]}) {
        /squelch /plugin ${pluginname}
        /echo \aw ${pluginname} \ar not detected! \aw This macro requires it! Loading ...
    }
/return	
|----------------------------------------------------------------------------
|- SUB: Death Sub
|----------------------------------------------------------------------------
Sub DeathSub
	/echo You are sleeping with the fishes
	/if (${ReturnToCamp}!=0) /varset ReturnToCamp 0
    :DeathSubwait
	/if (${Window[RespawnWnd].Open} && ${UseFellowship}==1 && ${InstantRelease}==1) {
		/nomodkey /notify RespawnWnd RW_OptionsList listselect 1 
		/delay 1s
		/nomodkey /notify RespawnWnd RW_SelectButton leftmouseup
		/delay 5s ${Me.Zoning}
	}
    /if (${Me.Hovering}) /goto :DeathSubwait
	/delay 1m !${Me.Zoning}
	/if (${UseFellowship}==1 && ${FindItem["Fellowship Registration Insignia"].Timer}==0) {
		/delay 30s ${Me.CombatState.Equal[ACTIVE]}
		/useitem "Fellowship Registration Insignia"
		/delay 2s ${FindItem["Fellowship Registration Insignia"].Timer}!=0
	} else /if (${UseFellowship}==1 && ${FindItem["Fellowship Registration Insignia"].Timer}!=0) {
		/echo \aw Bummer, Insignia on cooldown, you must really suck at this game...
	} else {
		/echo \aw Waiting on someone that knows how to heal to \ar rez me
	}
/return
|----------------------------------------------------------------------------
|- SUB: AutoMed - Modified SitCheck from auto subs, user configureable end/mana % to start medding at 
|- Need: EndMedPct and MedPct variables to use.
|---------------------------------------------------------------------------- 
Sub AutoMed
|---SIT Check
   /if ((${Me.Moving} || ${Me.XTarget[1].ID} || ${MoveTo.Moving} || ${Me.CombatState.Equal[COMBAT]} || ${Stick.Status.Equal[ON]} || ${AdvPath.Following}) && (${enablesit}==1)) /varset enablesit 0
|--Out of Combat MedTime - Sitting
	/if (${Me.Standing} && !${Me.Moving} && ${Stick.Status.Equal[OFF]} && !${Me.XTarget[1].ID} && !${MoveTo.Moving} && !${AdvPath.Following} && ${Me.CombatState.NotEqual[COMBAT]} && ${Me.CombatState.NotEqual[DEBUFFED]} && ${enablesit}==0 && ((${Me.PctMana}<=${MedPct} && ${Me.MaxMana}>1000) || (${Me.PctEndurance}<=${EndMedPct} && ${Me.MaxEndurance}>1000))) {
		/varset enablesit 1
	} else /if (${sittimer}==0 && ${enablesit}==0 && ${Me.Sitting} && (${Me.Moving} || ${Me.XTarget[1].ID} || ${MoveTo.Moving} || ${Me.CombatState.Equal[COMBAT]} || ${Stick.Status.Equal[ON]} || ${AdvPath.Following})) {
		/stand
		/varset sittimer 10s
	} else /if (${sittimer}==0 && ${enablesit}==1 && !${Me.Sitting}) {
		/sit
		/varset sittimer 10s
	}
/return
|----------------------------------------------------------------------------
|- SUB: UnCheck Plugin
|----------------------------------------------------------------------------
Sub UnCheckPlugin(string pluginname)
    /if (${Bool[${Plugin[${pluginname}]}]}) {
        /squelch /plugin ${pluginname} unload
        /echo \ar ${pluginname} detected! \aw Unloading it before it fucks shit up! UnLoading ...
    }
/return	
|----------------------------------------------------------------------------
|- SUB: DiscQueue
|---------------------------------------------------------------------------- 
Sub DiscQueue(string nextdisc)
	/if (!${Me.Standing}) /stand
	/delay 10 ${Me.Standing}
	/if (${Window[SpellBookWnd]}) /book
	/if (${Me.ActiveDisc.ID}) /stopdisc
	/delay 20 !${Me.ActiveDisc.ID}
	/squelch /disc ${nextdisc}
	/delay 20 ${Me.ActiveDisc.ID}
	/delay 8 !${Me.CombatAbilityReady[${nextdisc}]}
	/if (${acverbose}!=0) /echo \aw Casting \ag ${nextdisc}
/return
|----------------------------------------------------------------------------
|- SUB: DiscNow
|---------------------------------------------------------------------------- 
Sub DiscNow(string nextdisc)
	/if (${Window[CastingWindow].Open}) /return
	/if (!${Me.Standing}) /stand
	/delay 10 ${Me.Standing}
	/if (${Window[SpellBookWnd]}) /book
	/squelch /disc ${nextdisc}
	/delay 8 !${Me.CombatAbilityReady[${nextdisc}]}
	/if (${acverbose}!=0) /echo \aw Casting \ag ${nextdisc}
/return
|----------------------------------------------------------------------------
|- SUB: AANow
|---------------------------------------------------------------------------- 
Sub AANow(int nextaa,int aatargetid)
	/if (${Spawn[ID ${aatargetid}].FeetWet}!=${Me.FeetWet} && ${Spawn[id ${aatargetid} npc].ID} && ${Window[CastingWindow].Open}) /return
	/if (!${Me.Standing}) /stand
	/delay 10 ${Me.Standing}
	/if (${Window[SpellBookWnd]}) /book
	/if (${Window[CastingWindow].Open}) /interrupt
	/delay 5 !${Window[CastingWindow].Open}
	/if (${Target.ID}!=${aatargetid} && ${Spawn[id ${aatargetid} npc].ID}) {
		/target id ${aatargetid}
		/delay 10 ${Target.ID}==${aatargetid}
	}
	/squelch /alt act ${nextaa}
	/delay 8 !${Me.AltAbilityReady[${nextaa}]}
	/if (${EQBC.Connected} && ${acverbose}!=0) /docommand /bc Casting [+o+]=>>[+x+] [+g+]${Me.AltAbility[${nextaa}].Name}[+x+] [+o+]<<=[+x+]
	|-/if (${acverbose}!=0) /echo \aw Casting \ag ${Me.AltAbility[${nextaa}].Name}
/return
|----------------------------------------------------------------------------
|- SUB: AbilityNow
|---------------------------------------------------------------------------- 
Sub AbilityNow(string nextability)
	/squelch /doability ${nextability}
	/delay 8 !${Me.AbilityReady[${nextability}]}
	/if (${acverbose}!=0) /echo \aw Casting \ag ${nextability}
/return
|----------------------------------------------------------------------------
|- SUB: ItemNow
|---------------------------------------------------------------------------- 
Sub ItemNow(string nextitem)
	/if (${Window[CastingWindow].Open}) /return
	/if (!${Me.Standing}) /stand
	/delay 10 ${Me.Standing}
	/if (${Window[SpellBookWnd]}) /book
	/if (${Window[CastingWindow].Open}) /interrupt
	/delay 5 !${Window[CastingWindow].Open}
	/useitem ${nextitem}
	/delay 15 !${Me.ItemReady[${nextitem}]}
|--Checks if item is used and does it again if not
	/if (${Me.ItemReady[${nextitem}]}&&${FindItem[${nextitem}].ID}) {
		/useitem ${nextitem}
		/delay 15 !${Me.ItemReady[${nextitem}]}
	}
	/if (${acverbose}!=0) /echo \aw Using Item \ag ${nextitem}
/return
|----------------------------------------------------------------------------
|- SUB: SpellQueue - Modified to use memspell replacement sub and return a cast result
|---------------------------------------------------------------------------- 
Sub SpellQueue(string nextspell,int nextid)
	/if (${Me.CurrentMana}<${Spell[${nextspell}].Mana}) /return
	/if (!${Me.Book[${nextspell}]}) /return
	/varset CastResult CAST_NO_RESULT
	/if (${Me.Sitting}) /stand
	/if (${Window[SpellBookWnd]}) /book
	/if (!${Me.Gem[${nextspell}]}) { 
		/delay 1s !${Me.Casting.ID}
		/call LoadSpellGem "${nextspell}" ${spellmisc}
		/if (!${Me.XTarget[1].ID}) {
			/delay 5s ${Me.GemTimer[${nextspell}]}==0
			/delay 5s ${Me.SpellReady[${nextspell}]}
		}
    }
	/if (${Me.SpellReady[${nextspell}]}) {
		/if (${Target.ID}!=${nextid}) {
			/if (${Me.Combat}) /squelch /attack off
			/target id ${nextid}
			/delay 10 ${Target.ID}==${nextid}
			/if (${Target.FeetWet}!=${Me.FeetWet}) /return
		}
		/if (${Target.Buff[${nextspell}].ID}) /return
		/cast "${nextspell}"
		/delay 10 ${Window[CastingWindow].Open}
		/delay ${Math.Calc[(${Spell[${nextspell}].MyCastTime}*10)+15]} !${Me.Casting.ID}
		/if (((!${EQBC.Connected} || !${Bool[${Plugin[MQ2EQBC]}]}) && ${acverbose}!=0) && (${Cast.Result.Equal[CAST_SUCCESS]})) /docommand /echo \aw ${nextspell} \ar =>> \ag ${Spawn[ID ${nextid}].Name} \ar <<=
		/if ((${EQBC.Connected} && ${acverbose}!=0) && (${Cast.Result.Equal[CAST_SUCCESS]})) /docommand /bc [+o+]${nextspell} =>> ${Spawn[ID ${nextid}].Name} <<=[+x+]
	}
	/if (${CastResult.NotEqual[CAST_CANCELLED]}) {
		/varset CastResult ${Cast.Result}
	} else {
		/varset CastResult CAST_CANCELLED
	}
/return ${CastResult}
|----------------------------------------------------------------------------
|- SUB: FindTarget - melee attack portion removed. Melee attack moved into engage sub to reduce too far away and can not see messages
|---------------------------------------------------------------------------- 
Sub FindTarget
/declare xt int local 1
/declare xtlow int local ${AutoAssistAt}
/declare xtid int local 0
	/if (${Target.Type.Equal[Corpse]}) /target clear
	/if (!${Spawn[${SpawnSearchType} id ${autotargetid}].ID} && ${autotargetid}!=0) {
		/varset autotargetid 0
		/if (${Stick.Status.Equal[ON]}) /stick off
	}
|--Query XTarget to find hp percents
	/for xt 1 to ${Me.XTarget}
		/if (${Me.XTarget[${xt}].PctHPs}<=${xtlow} && ${Spawn[id ${Me.XTarget[${xt}].ID} ${SpawnSearchType}].ID}) {
			/varset xtlow ${Me.XTarget[${xt}].PctHPs}
			/varset xtid ${Me.XTarget[${xt}].ID}
		}
	/next xt
|- Spawn search code to replace above Xtarget search
|- /declare MobIDList string local
|-	:targetsearch
|-	/if (${NearestSpawn[${xt},npc radius ${AssistRange} zradius 15 targetable playerstate 4].ID}) {
|-		/if (!${MobIDList.Find[|${NearestSpawn[${xt},npc radius ${AssistRange} zradius 15 targetable playerstate 4].ID}]} && ${NearestSpawn[${xt},npc radius ${AssistRange} zradius 15 targetable playerstate 4].PctHPs}<=${xtlow}) {
|-			/varset xtid ${NearestSpawn[${xt},npc radius ${AssistRange} zradius 15 targetable playerstate 4].ID}
|-			/varset xtlow ${Spawn[npc id ${xtid}].PctHPs}
|-			/varset MobIDList ${MobIDList}|${xtid}
|-		}		
|-		/varcalc xt ${xt}+1
|-		/goto :targetsearch
|-	}
|--Check target matches MA if not based off lowest hp in XTarget
	/if (${Target.ID}!=${Me.GroupAssistTarget.ID} && ${Group.MainAssist.ID}!=${Me.ID} && ${autotargetid}!=${Me.GroupAssistTarget.ID} && ${Spawn[id ${Me.GroupAssistTarget.ID} ${SpawnSearchType}].ID}) {
		/varset autotargetid ${Me.GroupAssistTarget.ID}
	} else /if ((!${Group.MainAssist.ID} || ${Group.MainAssist.ID}==${Me.ID}) && ${Spawn[${SpawnSearchType} id ${xtid}].ID}) {
		/varset autotargetid ${xtid}
	}
|--Target the new target
	/if (${Target.ID}!=${autotargetid} && ${Spawn[${SpawnSearchType} id ${autotargetid}].ID}) {
		/squelch /target id ${autotargetid}
		/delay 10 ${Target.ID}==${autotargetid}
	}
/return 
|----------------------------------------------------------------------------
|- SUB: Melee Engage Target - Need a StickHow variable in the mac. Ini option for melee just a declared variable set to null for classes that wont enageg in melee
|- Basic use will check the that the target is in assist range, stick and when in melee range start attacking ( reduces too far away and can not see messages )
|- Advanced use ( smart assist ) will check that the target is range of the MA before engaging....keeps melee from charging out after a mob on incoming
|- For classes that have a before combat move, it is recommended  to copy/paste this sub into new macro and rename. Add opening move ( ber, rog ) to just before turning on attack 
|---------------------------------------------------------------------------- 
Sub EngageTarget
	/declare OKRadCal int local 0
	/if (${Me.State.Equal[FEIGN]}) /stand
	|- If not the MA check that the mob is in range of the MA and not just  in assist range before engaging 
	/if (${UseSmartAssist}) {
		/varset OKRadCal ${Spawn[${SpawnSearchType} id ${autotargetid}].MaxRangeTo}
		/if (${Target.ID}==${autotargetid} && ${assistid}!=${Me.ID} && ${Spawn[${assistid}].ID} && ${Spawn[${SpawnSearchType} id ${autotargetid} loc ${Spawn[id ${assistid}].X} ${Spawn[id ${assistid}].Y} radius ${OKRadCal}].ID}) {
			/varset Ok2Assist 1
		} else /if (${assistid}==${Me.ID}) {
			/varset Ok2Assist 1
		} else {
			/varset Ok2Assist 0
		}
	}
	/if (!${Ok2Assist} && ${Stick.Status.Equal[ON]}) /stick off
	/if (${Ok2Assist} && ${UseMelee}==1 && ${Spawn[${SpawnSearchType} id ${autotargetid}].PctHPs}>0 && ${Target.ID}==${autotargetid} && ${Stick.Status.Equal[OFF]}) {
		/if (${StickHow.NotEqual[NULL]}) {
			/stick ${StickHow}
		} else /if (${Math.Calc[${Target.MaxRangeTo}*0.8]} > 29) {
			/stick snaproll rear
		} else {
			/stick ${Math.Calc[${Target.MaxRangeTo}*0.8]} behind uw
		}
		/delay 10 ${Stick.Active}
	}
	/call EngageAttack
/return
|-------------------------------------------------------------------------------------
|- SUB: EngageAttack	
|-------------------------------------------------------------------------------------
Sub EngageAttack
	/if (!${Me.Combat} && ${UseMelee}==1 && ${Target.PctHPs}<=${AutoAssistAt} && ${Target.PctHPs}>0 && ${Target.ID}==${autotargetid} && ${Spawn[id ${autotargetid} ${SpawnSearchType} radius ${AssistRange}].ID} && ${Ok2Assist}) {
		/if (${Target.MaxRangeTo}>=${Target.Distance}) {
			/if (${BeforeCombat.NotEqual[NULL]}) /call ${BeforeCombat}
			/squelch /attack on
			/delay 10 ${Me.Combat}
		}
	}
/return 
|-------------------------------------------------------------------------------------
|- SUB: CastSpell - Taken from kissassist and modified. Better spell casting control for longer casting spells plus return messages for monitoring resists, interruptions, immune
|-------------------------------------------------------------------------------------
Sub CastSpell(string WhatSpell,int WhatID,string sentFrom)
    /declare MaxTryNum int local 2
	/varset CastResult CAST_NO_RESULT
    /if (!${Target.CanSplashLand} && ${Spell[${WhatSpell}].TargetType.Equal[Free Target]}) {
        /echo \ar Splash Spell will not cast at targets location. Skipping ${WhatSpell}
        /return
    }
    /if (${Target.ID}!=${WhatID} && ${Spell[${WhatSpell}].TargetType.NotEqual[Self]} && ${Spawn[id ${WhatID}].ID}) /call CastTarget ${WhatID}
    /if (${Me.Gem[${WhatSpell}]}) {
		/if (${Me.GemTimer[${WhatSpell}]}==0) {
            /casting "${WhatSpell}" -maxtries|${MaxTryNum}
            /while (${Cast.Status.Equal[C]} && ${CastResult.NotEqual[CAST_CANCELLED]}) {
                /delay 5
				/if (${Target.Type.Equal[Corpse]} && ${UseSpellInterrupt}) /stopcast
            }
            /call CastWait "${WhatSpell}"
            /if (${CastResult.NotEqual[CAST_CANCELLED]}) /varset CastResult ${Cast.Result}
			/doevents
        }
    } else {
        /echo \aw Skip Casting \ag ${WhatSpell}. \ar Sell Not Memed.
        /varset CastResult CAST_CANCELLED
    }
/return ${CastResult}  
|-------------------------------------------------------------------------------------
|- SUB: CastWait - Cast Spell sub routine from kissassist
|-------------------------------------------------------------------------------------
Sub CastWait(string WaitSpell)
    /if (${Spell[${WaitSpell}].CastTime}) {
        /while (${Me.Casting.ID} || ${Cast.Status.Equal[c]}) {
            /delay 5
			/if (${Target.Type.Equal[Corpse]} && ${UseSpellInterrupt}) /stopcast
        }
    }
/return
|-------------------------------------------------------------------------------------
|- SUB: CastTarget - Cast Spell sub routine from kissassist
|-------------------------------------------------------------------------------------
Sub CastTarget(int WhatID)
    /squelch /target clear
    /delay 10 !${Target.ID}
    /target id ${WhatID}
    /delay 10 ${Target.ID}==${WhatID}
/return
|------------------------------------------------------------------------------------------
|- SUB: LoadSpellGem - Replaces memspell sub
|------------------------------------------------------------------------------------------
Sub LoadSpellGem(string SpellToMem,int memgem)
	/declare ii int local
    /if (${SpellToMem.Length} && ${SpellToMem.NotEqual[null]}) {
        /if (${Me.Book[${SpellToMem}]}) {
            /if (${Int[${Me.Gem[${SpellToMem}]}]}>0 && ${Int[${Me.Gem[${SpellToMem}]}]}!=${memgem}) {
                /varcalc ii ${Int[${Me.Gem[${SpellToMem}]}]}-1
                /notify CastSpellWnd CSPW_Spell${ii} rightmouseup
                /delay 20 ${Int[${Me.Gem[${SpellToMem}]}]}==0
            }
            /if (!${Int[${Me.Gem[${memgem}].Name.Length}]} || ${Me.Gem[${memgem}].Name.NotEqual[${SpellToMem}]}) {
                /echo \ag Meming \aw ${SpellToMem} in \ag slot ${memgem}
                /memspell ${memgem} "${SpellToMem}"
                /delay 90 ${Me.Gem[${memgem}].Name.Equal[${SpellToMem}]}
            }
        } else {
                /echo \aw Could Not find the spell ${SpellToMem} in your spell book.
        }
    } else {
		/echo \ar Invalid \aw SpellToMem ${SpellToMem} 
	}
/return
|----------------------------------------------------------------------------
|- SUB: FindNumTargets
|---------------------------------------------------------------------------- 
Sub FindNumTargets
	/declare xt int local 1
	/if (!${Defined[numoftargets]}) /declare numoftargets int outer 0
	/varset numoftargets 0
	|--Query XTarget to find npcs
	/for xt 1 to ${Me.XTarget}
		/if (${Me.XTarget[${xt}].Type.Equal[NPC]}) /varcalc numoftargets ${numoftargets}+1
	/next xt
/return 
|----------------------------------------------------------------------------
|- SUB: IDUpdateCheck - keeps MA ID updated if it changes ( Death, Zoning )
|---------------------------------------------------------------------------- 
Sub IDUpdateCheck
	/if (${Spawn[pc =${assistname}].ID} && ${Spawn[pc =${assistname}].ID}!=${assistid}) {
		/echo \aw MA ID Mismatch - \ag Updating
		/varset assistid ${Spawn[pc =${assistname}].ID}
	}
/return
|-------------------------------------------------------------------------------------
|- SUB: Loot Stuff - borrowed from kissassist, modded
|-------------------------------------------------------------------------------------
Sub LootStuff
    /if (!${LootOn} || (!${Me.UseAdvancedLooting} && ${Me.Combat}) || ${Me.Invis}) /return
    /if (${Me.FreeInventory}==0) {
        /echo \aw Inventory is full. Looting \ar OFF
        /varset LootOn 0
        /return
    }
	/if (${LootOn}) {
		/if (${Me.UseAdvancedLooting}) {
			/call UseAdvLoot
		} else {
			/call LootMobs
		}
	}
/return 
|-------------------------------------------------------------------------------------
|- SUB: PlayersInGroup - Checking for PC vs Merc
|-------------------------------------------------------------------------------------
Sub PlayersInGroup
	/declare i int local 0
	/declare gcnt int local 0
	/for i 1 to ${Group.Members}
		/if (${Group.Member[${i}].Type.Equal[PC]}) {
			/varcalc gcnt ${gcnt}+1
		}
	/next i
/return ${gcnt}
|----------------------------------------------------------------------------
|- BIND: GroupFrontFace - Credit EQMules mod of kissassist
|----------------------------------------------------------------------------
Sub Bind_GroupFrontFace(string radius)
	/declare GrpCount int local 0
	/call PlayersInGroup
	/varset GrpCount ${Macro.Return}
	/if (!${GrpCount}) {
		/echo no members in the group to order around, returning
		/return
	}
	/declare i int local 0
	/declare XOff float local 0
	/declare YOff float local 0
	/declare XMove float local 0
	/declare YMove float local 0
	/declare BaseRadians local
	/declare Multiplier local 90
	/if (${GrpCount}==2) {
		/varset Multiplier 45
	} else /if (${GrpCount}==3) {
		/varset Multiplier 30
	} else /if (${GrpCount}==4) {
		/varset Multiplier 23
	} else /if (${GrpCount}==5) {
		/varset Multiplier 18
	}
	/declare MyHeading float local ${Math.Calc[${Me.Heading.Degrees} - ${Multiplier}]}
	/declare BaseRadian float local ${Math.Calc[180 / ${GrpCount}]}
	/for i 1 to ${Group.Members}
		/if (${Group.Member[${i}].Type.Equal[PC]}) {
			/varcalc XMove ${Math.Cos[ ${BaseRadian} * ${Float[${i}]} + (${MyHeading}) ]}
			/varcalc YMove ${Math.Sin[ ${BaseRadian} * ${Float[${i}]} + (${MyHeading}) ]}
			/varcalc XOff ${Me.X} + ( ${Int[${radius}]} * ${XMove} )
			/varcalc YOff ${Me.Y} + ( ${Int[${radius}]} * ${YMove} )
			/bct ${Group.Member[${i}]} //sheepmove ${Me.ID} ${YOff} ${XOff}
		}
	/next i
/return
|----------------------------------------------------------------------------
|- BIND: GroupBehindMe - Credit EQMules mod of kissassist
|----------------------------------------------------------------------------
Sub Bind_GroupBehindMe(string radius)
	/declare GrpCount int local 0
	/call PlayersInGroup
	/varset GrpCount ${Macro.Return}
	/if (!${GrpCount}) {
		/echo no members in the group to order around, returning
		/return
	}
	/declare i int local 0
	/declare XOff float local 0
	/declare YOff float local 0
	/declare XMove float local 0
	/declare YMove float local 0
	/declare BaseRadians local
	/declare Multiplier local 270
	/if (${GrpCount}==2) {
		/varset Multiplier 224
	} else /if (${GrpCount}==3) {
		/varset Multiplier 208
	} else /if (${GrpCount}==4) {
		/varset Multiplier 200
	} else /if (${GrpCount}==5) {
		/varset Multiplier 196
	}
	/declare MyHeading float local ${Math.Calc[${Me.Heading.Degrees} - ${Multiplier}]}
	/declare BaseRadian float local ${Math.Calc[180 / ${GrpCount}]}
	/for i 1 to ${Group.Members}
		/if (${Group.Member[${i}].Type.Equal[PC]}) {
			/varcalc XMove ${Math.Cos[ ${BaseRadian} * ${Float[${i}]} + (${MyHeading}) ]}
			/varcalc YMove ${Math.Sin[ ${BaseRadian} * ${Float[${i}]} + (${MyHeading}) ]}
			/varcalc XOff ${Me.X} + ( ${Int[${radius}]} * ${XMove} )
			/varcalc YOff ${Me.Y} + ( ${Int[${radius}]} * ${YMove} )
			/bct ${Group.Member[${i}]} //sheepmove ${Me.ID} ${YOff} ${XOff}
		}
	/next i
/return
|----------------------------------------------------------------------------
|- BIND: SheepMove - Credit EQMules mod of kissassist
|----------------------------------------------------------------------------
Sub Bind_SheepMove(int shepherdID, int l_YLoc, int l_XLoc)
	/echo shepherdID=${shepherdID} l_YLoc=${l_YLoc} l_XLoc=${l_XLoc}
	/if (!${shepherdID}) {
		/echo cant invoke sheepmove, not enough arguments supplied
		/return
	}
	/declare oldRTC int local 0
	/declare oldChase int local 0
	/if (${Bool[${Plugin[mq2moveutils]}]}==TRUE) {
		/varset oldRTC ${ReturnToCamp}
		/varset oldChase ${ChaseAssist}
		/if (${Bool[${Plugin[mq2advpath]}]}==TRUE) {
        	       	/play off
 		}
		/if (${Bool[${Plugin[mq2navigation]}]}==TRUE || ${Bool[${Plugin[mq2nav]}]}==TRUE) {
        		/if (${Navigation.Active}) {
				/nav stop
			}
        }
		/call Bind_ToggleVariable ReturnToCamp off
		/call Bind_ToggleVariable ChaseAssist off
        /if (${MoveTo.Moving}) /moveto off
		/if (${Stick.Active}) /squelch /stick off
		/moveto mdist 3
		/moveto loc ${l_YLoc} ${l_XLoc}
		/delay 3s ${MoveTo.Moving}
		/delay 30s !${MoveTo.Moving}
		/delay 1s
		/moveto mdist 5
		/face id ${shepherdID}
		/if (${oldChase}) /call Bind_ToggleVariable ChaseAssist on
		/if (${oldRTC}) /call Bind_ToggleVariable ReturnToCamp on
	} else {
		/echo \aw SheepMove failed because you don't have \ag mq2moveutils \aw loaded.
	}
/return
|-------------------------------------------------------------------------------------
|- SUB: Welcome Message
|-------------------------------------------------------------------------------------
Sub WelcomeMsg
	/echo \aw****************************
	/echo \awWelcome to \ag${MacroName}
	/echo \awVersion \ag${IHCVersion}
	/echo \aw****************************
	/echo \aw use \ag /ihchelp \aw for a list of commands
/return
|-------------------------------------------------------------------------------------
|- SUB: NavgationDelay
|-------------------------------------------------------------------------------------
Sub NavgationDelay
	:naving
	/delay 1s
	|-/echo Navigating...
	/if (${Navigation.Active}) /goto :naving
	|-/echo Navigation Finished!
/return 
|-------------------------------------------------------------------------------------
|- SUB: WriteCommonHud
|-------------------------------------------------------------------------------------
Sub WriteCommonHud
	/if (!${Ini[MQHUDPATH,HUDNAME].Length}) {
		/echo \awWriting IHCMAIN HUD please wait...
		/noparse /ini MQHUDPATH HUDNAME "GMIndicator" "3,20,38,255,255,255,${If[${ShowDeluxeHud},GM - ${NearestSpawn[GM]},]}"
		/noparse /ini MQHUDPATH HUDNAME "GameTime" "3,20,50,255,255,255,${If[${ShowDeluxeHud},GameTime: ${GameTime.Hour}:${GameTime.Minute} - ${If[${Bool[${GameTime.Night}]},Night,Day]},]}"
		/noparse /ini MQHUDPATH HUDNAME "Speed" "3,20,62,0,240,0,${If[${ShowDeluxeHud},Speed - ${Math.Calc[${Me.Speed}/3.3].Int},]}"
		/noparse /ini MQHUDPATH HUDNAME "LastTell" "3,20,86,215,0,215,${If[${ShowDeluxeHud},LastTell - ${If[${MacroQuest.LastTell.NotEqual["NULL"]},${MacroQuest.LastTell},]},]}"
		/noparse /ini MQHUDPATH HUDNAME "TributePoints" "3,20,98,0,240,0,${If[${ShowDeluxeHud},Tribute - ${Me.CurrentFavor},]}"
		/noparse /ini MQHUDPATH HUDNAME "XP" "3,20,110,200,200,200,${If[${ShowDeluxeHud},XP - ${Float[${Me.PctExp}]}% into ${Me.Level},]}"
		/noparse /ini MQHUDPATH HUDNAME "AAXP" "3,20,122,240,240,0,${If[${ShowDeluxeHud},A.A. XP - ${Float[${Me.PctAAExp}]}%       Points Available: ${Me.AAPoints}      Spent:${Me.AAPointsSpent}    Total:${Me.AAPointsAssigned},]}"
		/noparse /ini MQHUDPATH HUDNAME "HP" "3,20,134,100,255,100,${If[${ShowDeluxeHud},Current HPs: ${Me.CurrentHPs} of ${Me.MaxHPs},]}"
		/noparse /ini MQHUDPATH HUDNAME "Mana" "3,20,146,100,255,100,${If[${ShowDeluxeHud},Current Mana: ${Me.CurrentMana} of ${Me.MaxMana},]}"
		/noparse /ini MQHUDPATH HUDNAME "Macro" "3,20,26,215,0,215,${If[${ShowDeluxeHud},Macro Running: ${Macro.Name},]}"
		/noparse /ini MQHUDPATH HUDNAME "**Target Loc**" "3,160,182,150,250,150,${If[${Target.ID} && ${ShowDeluxeHud},Target Loc-->  ${Target.Y}    ${Target.X}    ${Target.Z},]}"
		/noparse /ini MQHUDPATH HUDNAME "**Me Loc**" "3,160,194,150,250,150,${If[${ShowDeluxeHud},Me Loc------>  ${Me.Y}    ${Me.X}    ${Me.Z},]}"
		/noparse /ini MQHUDPATH HUDNAME "TargetLevel" "3,20,170,255,255,255,${If[${Target.ID} && ${ShowDeluxeHud},${Target.Name} Lvl ${Target.Level} - ${Target.Class} - ${Target.Race} - ${Target.PctHPs}% Hp,]}"
		/noparse /ini MQHUDPATH HUDNAME "LOS" "3,20,182,50,255,0,${If[${Target.ID} && ${Target.LineOfSight} && ${ShowDeluxeHud},Target in LoS,]}"
		/noparse /ini MQHUDPATH HUDNAME "NOLOS" "3,20,182,255,30,0,${If[${Target.ID} && !${Target.LineOfSight} && ${ShowDeluxeHud},Target NOT in LoS,]}"
		/noparse /ini MQHUDPATH HUDNAME "HP Regen" "3,290,134,255,000,000,${If[${ShowDeluxeHud},HP Regen: ${Me.HPRegen},]}"
		/noparse /ini MQHUDPATH HUDNAME "Mana Regen" "3,290,146,255,000,000,${If[${ShowDeluxeHud},Mana Regen: ${Me.ManaRegen},]}"
		/noparse /ini MQHUDPATH HUDNAME "Petdistance" "3,160,62,255,255,255,${If[${Me.Pet.ID} && ${ShowDeluxeHud},Pet Distance: ${Me.Pet.Distance},]}"
		/noparse /ini MQHUDPATH HUDNAME "TargetSpeed" "3,160,74,255,234,8,${If[${Target.ID} && ${ShowDeluxeHud},RunSpeed is ${Math.Calc[${Target.Speed}/3.3]} MPH,]}"
		/noparse /ini MQHUDPATH HUDNAME "TargetDist" "3,160,86,255,255,255,${If[${Target.ID} && ${ShowDeluxeHud},Target Distance: ${Target.Distance},]}"
		/noparse /ini MQHUDPATH HUDNAME "maxmelee" "3,160,98,255,255,255,${If[${Target.ID} && ${ShowDeluxeHud},Melee Range: ${Target.MaxRange},]}"
		/noparse /ini MQHUDPATH HUDNAME "CursorItemName" "3,290,158,0,240,0,${If[${Cursor.ID} && ${ShowDeluxeHud},Cursor - ${Cursor},]}"
		/noparse /ini MQHUDPATH HUDNAME "CursorFollow" "7,40,40,0,240,0,${If[${Cursor.ID} && ${ShowDeluxeHud},${Cursor},]}"
		/noparse /ini MQHUDPATH HUDNAME "Hunger" "3,160,38,255,255,255,${If[${ShowDeluxeHud},Hunger - ${Me.Hunger}/6000,]}"
		/noparse /ini MQHUDPATH HUDNAME "Thirst" "3,160,50,255,255,255,${If[${ShowDeluxeHud},Thirst - ${Me.Thirst}/6000,]}"
		/noparse /ini MQHUDPATH HUDNAME "AttackSpeed" "3,20,74,240,127,0,${If[${ShowDeluxeHud},AttackSpeed: ${Me.Haste},]}"
		/noparse /ini MQHUDPATH HUDNAME "Direction" "3,118,182,255,0,0,${If[${ShowDeluxeHud},${If[${Target.ID}==NULL,"",${If[${Math.Calc[((${Me.Heading.Degrees}-${Target.HeadingTo.Degrees}+375)%360)*-1]}>-30,"XXX",${If[${Math.Calc[((${Me.Heading.Degrees}-${Target.HeadingTo.Degrees}+360)%360)*-1]}>-180,"<--","-->"]}]}]},]}"
		/noparse /ini MQHUDPATH HUDNAME "ZoneName" "3,380,38,255,255,0,${If[${ShowDeluxeHud},Zone - ${Zone},]}"
		/noparse /ini MQHUDPATH HUDNAME "ShortZoneName" "3,380,50,255,255,255,${If[${ShowDeluxeHud},Short Zone Name - ${Zone.ShortName},]}"
		/noparse /ini MQHUDPATH HUDNAME "ZoneID" "3,380,62,255,255,225,${If[${ShowDeluxeHud},Zone ID# - ${Zone.ID},]}"
		/noparse /ini MQHUDPATH HUDNAME "GroupLeader" "3,380,74,255,255,255,${If[${Group} && ${ShowDeluxeHud},Group Leader : ${Group.Leader.Name},]}"
		/noparse /ini MQHUDPATH HUDNAME "GroupMainAssist" "3,380,86,255,255,255,${If[${Group} && ${ShowDeluxeHud},Main Assist : ${Group.MainAssist.Name},]}"
		/noparse /ini MQHUDPATH HUDNAME "GroupMasterLooter" "3,380,98,255,255,255,${If[${Group} && ${ShowDeluxeHud},Master Looter : ${Group.MasterLooter.Name},]}"
		/noparse /ini MQHUDPATH HUDNAME "Raid1" "3,380,110,180,180,180,${If[${Raid} && ${ShowDeluxeHud},Raid Leader: ${Raid.Leader},]}"
		/noparse /ini MQHUDPATH HUDNAME "Raid2" "3,380,122,255,0,0,${If[${Raid.Members}>0 && ${ShowDeluxeHud},In Raid - ${Raid.Members},]}   ${If[${Raid.Members}>0,Raid Level - ${Raid.AverageLevel},]}"
	|-Driver Hud
		/noparse /ini MQHUDPATH HUDNAME "Group" "3,660,26,255,234,008,${If[${ShowDriverHud} && (${Group.Member[1].Distance}>150 && ${Group.Member[1].Type.Equal[PC]}) || (${Group.Member[2].Distance}>150 && ${Group.Member[2].Type.Equal[PC]}) || (${Group.Member[3].Distance}>150 && ${Group.Member[3].Type.Equal[PC]}) || (${Group.Member[4].Distance}>150 && ${Group.Member[4].Type.Equal[PC]}) || (${Group.Member[5].Distance}>150 && ${Group.Member[5].Type.Equal[PC]}),~~~~~~~~~~ GROUP MEMBER OUT OF RANGE~~~~~~~~~~~,]}"
		/noparse /ini MQHUDPATH HUDNAME "GroupMember1" "3,660,38,255,000,000,${If[${ShowDriverHud} && ${Group.Member[1].Name.NotEqual["NULL"]} && ${Group.Member[1].Type.Equal[PC]} && ${Group.Member[1].Distance}>150 && !${Group.Member[1].Invis},${If[${Group.Member[1].Name.NotEqual["NULL"]},${Group.Member[1]} - ${Group.Member[1].CurrentHPs}%,]} - ${If[${Group.Member[1].Name.NotEqual["NULL"]},${Group.Member[1].Level},]} - ${If[${Group.Member[1].Name.NotEqual["NULL"]},${Group.Member[1].Class},]} - ${If[${Group.Member[1].Name.NotEqual["NULL"]},${Group.Member[1].Distance} VISIBLE,]},]}"
		/noparse /ini MQHUDPATH HUDNAME "GroupMember11" "3,660,38,255,234,008,${If[${ShowDriverHud} && ${Group.Member[1].Name.NotEqual["NULL"]} && ${Group.Member[1].Type.Equal[PC]} && ${Group.Member[1].Distance}>150 && ${Group.Member[1].Invis},${If[${Group.Member[1].Name.NotEqual["NULL"]},${Group.Member[1]} - ${Group.Member[1].CurrentHPs}%,]} - ${If[${Group.Member[1].Name.NotEqual["NULL"]},${Group.Member[1].Level},]} - ${If[${Group.Member[1].Name.NotEqual["NULL"]},${Group.Member[1].Class},]} - ${If[${Group.Member[1].Name.NotEqual["NULL"]},${Group.Member[1].Distance},]} Invis,]}"
		/noparse /ini MQHUDPATH HUDNAME "GroupMember2" "3,660,50,255,000,000,${If[${ShowDriverHud} && ${Group.Member[2].Name.NotEqual["NULL"]} && ${Group.Member[2].Type.Equal[PC]} && ${Group.Member[2].Distance}>150 && !${Group.Member[2].Invis},${If[${Group.Member[2].Name.NotEqual["NULL"]},${Group.Member[2]} - ${Group.Member[2].CurrentHPs}%,]} - ${If[${Group.Member[2].Name.NotEqual["NULL"]},${Group.Member[2].Level},]} - ${If[${Group.Member[2].Name.NotEqual["NULL"]},${Group.Member[2].Class},]} - ${If[${Group.Member[2].Name.NotEqual["NULL"]},${Group.Member[2].Distance} VISIBLE,]},]}"
		/noparse /ini MQHUDPATH HUDNAME "GroupMember22" "3,660,50,255,234,008,${If[${ShowDriverHud} && ${Group.Member[2].Name.NotEqual["NULL"]} && ${Group.Member[2].Type.Equal[PC]} && ${Group.Member[2].Distance}>150 && ${Group.Member[2].Invis},${If[${Group.Member[2].Name.NotEqual["NULL"]},${Group.Member[2]} - ${Group.Member[2].CurrentHPs}%,]} - ${If[${Group.Member[2].Name.NotEqual["NULL"]},${Group.Member[2].Level},]} - ${If[${Group.Member[2].Name.NotEqual["NULL"]},${Group.Member[2].Class},]} - ${If[${Group.Member[2].Name.NotEqual["NULL"]},${Group.Member[2].Distance},]} Invis,]}"
		/noparse /ini MQHUDPATH HUDNAME "GroupMember3" "3,660,62,255,000,000,${If[${ShowDriverHud} && ${Group.Member[3].Name.NotEqual["NULL"]} && ${Group.Member[3].Type.Equal[PC]} && ${Group.Member[3].Distance}>150 && !${Group.Member[3].Invis},${If[${Group.Member[3].Name.NotEqual["NULL"]},${Group.Member[3]} - ${Group.Member[3].CurrentHPs}%,]} - ${If[${Group.Member[3].Name.NotEqual["NULL"]},${Group.Member[3].Level},]} - ${If[${Group.Member[3].Name.NotEqual["NULL"]},${Group.Member[3].Class},]} - ${If[${Group.Member[3].Name.NotEqual["NULL"]},${Group.Member[3].Distance} VISIBLE,]},]}"
		/noparse /ini MQHUDPATH HUDNAME "GroupMember33" "3,660,62,255,234,008,${If[${ShowDriverHud} && ${Group.Member[3].Name.NotEqual["NULL"]} && ${Group.Member[3].Type.Equal[PC]} && ${Group.Member[3].Distance}>150 && ${Group.Member[3].Invis},${If[${Group.Member[3].Name.NotEqual["NULL"]},${Group.Member[3]} - ${Group.Member[3].CurrentHPs}%,]} - ${If[${Group.Member[3].Name.NotEqual["NULL"]},${Group.Member[3].Level},]} - ${If[${Group.Member[3].Name.NotEqual["NULL"]},${Group.Member[3].Class},]} - ${If[${Group.Member[3].Name.NotEqual["NULL"]},${Group.Member[3].Distance},]} Invis,]}"
		/noparse /ini MQHUDPATH HUDNAME "GroupMember4" "3,660,74,255,000,000,${If[${ShowDriverHud} && ${Group.Member[4].Name.NotEqual["NULL"]} && ${Group.Member[4].Type.Equal[PC]} && ${Group.Member[4].Distance}>150 && !${Group.Member[4].Invis},${If[${Group.Member[4].Name.NotEqual["NULL"]},${Group.Member[4]} - ${Group.Member[4].CurrentHPs}%,]} - ${If[${Group.Member[4].Name.NotEqual["NULL"]},${Group.Member[4].Level},]} - ${If[${Group.Member[4].Name.NotEqual["NULL"]},${Group.Member[4].Class},]} - ${If[${Group.Member[4].Name.NotEqual["NULL"]},${Group.Member[4].Distance} VISIBLE,]},]}"
		/noparse /ini MQHUDPATH HUDNAME "GroupMember44" "3,660,74,255,234,008,${If[${ShowDriverHud} && ${Group.Member[4].Name.NotEqual["NULL"]} && ${Group.Member[4].Type.Equal[PC]} && ${Group.Member[4].Distance}>150 && ${Group.Member[4].Invis},${If[${Group.Member[4].Name.NotEqual["NULL"]},${Group.Member[4]} - ${Group.Member[4].CurrentHPs}%,]} - ${If[${Group.Member[4].Name.NotEqual["NULL"]},${Group.Member[4].Level},]} - ${If[${Group.Member[4].Name.NotEqual["NULL"]},${Group.Member[4].Class},]} - ${If[${Group.Member[4].Name.NotEqual["NULL"]},${Group.Member[4].Distance},]} Invis,]}"
		/noparse /ini MQHUDPATH HUDNAME "GroupMember5" "3,660,86,255,000,000,${If[${ShowDriverHud} && ${Group.Member[5].Name.NotEqual["NULL"]} && ${Group.Member[5].Type.Equal[PC]} && ${Group.Member[5].Distance}>150 && !${Group.Member[5].Invis},${If[${Group.Member[5].Name.NotEqual["NULL"]},${Group.Member[5]} - ${Group.Member[5].CurrentHPs}%,]} - ${If[${Group.Member[5].Name.NotEqual["NULL"]},${Group.Member[5].Level},]} - ${If[${Group.Member[5].Name.NotEqual["NULL"]},${Group.Member[5].Class},]} - ${If[${Group.Member[5].Name.NotEqual["NULL"]},${Group.Member[5].Distance} VISIBLE,]},]}"
		/noparse /ini MQHUDPATH HUDNAME "GroupMember55" "3,660,86,255,234,008,${If[${ShowDriverHud} && ${Group.Member[5].Name.NotEqual["NULL"]} && ${Group.Member[5].Type.Equal[PC]} && ${Group.Member[5].Distance}>150 && ${Group.Member[5].Invis},${If[${Group.Member[5].Name.NotEqual["NULL"]},${Group.Member[5]} - ${Group.Member[5].CurrentHPs}%,]} - ${If[${Group.Member[5].Name.NotEqual["NULL"]},${Group.Member[5].Level},]} - ${If[${Group.Member[5].Name.NotEqual["NULL"]},${Group.Member[5].Class},]} - ${If[${Group.Member[5].Name.NotEqual["NULL"]},${Group.Member[5].Distance},]} Invis,]}"
		/noparse /ini MQHUDPATH HUDNAME "Visible" "3,1020,26,255,234,008,${If[${ShowDriverHud} && ${Me.Invis} && ${Me.Grouped} && ((!${Group.Member[1].Invis} && ${Group.Member[1].Type.Equal[PC]} && ${Group.Members}>0) || (!${Group.Member[2].Invis} && ${Group.Member[2].Type.Equal[PC]} && ${Group.Members}>1) || (!${Group.Member[3].Invis} && ${Group.Member[3].Type.Equal[PC]} && ${Group.Members}>2) || (!${Group.Member[4].Invis} && ${Group.Member[4].Type.Equal[PC]} && ${Group.Members}>3) || (!${Group.Member[5].Invis} && ${Group.Member[5].Type.Equal[PC]} && ${Group.Members}>4)),~~~~~~~~~~VISIBLE~~~~~~~~~~~,]}"
		/noparse /ini MQHUDPATH HUDNAME "Vis1" "3,1020,38,255,000,000,${If[${ShowDriverHud} && ${Group.Member[1].Name.NotEqual["NULL"]} && ${Group.Member[1].Type.Equal[PC]} && ${Group.Members}>0 && ${Me.Invis} && !${Group.Member[1].Invis},VISIBLE >>> ${Group.Member[1].Name} <<< VISIBLE,]}"
		/noparse /ini MQHUDPATH HUDNAME "Vis2" "3,1020,50,255,000,000,${If[${ShowDriverHud} && ${Group.Member[2].Name.NotEqual["NULL"]} && ${Group.Member[2].Type.Equal[PC]} && ${Group.Members}>1 && ${Me.Invis} && !${Group.Member[2].Invis},VISIBLE >>> ${Group.Member[2].Name} <<< VISIBLE,]}"
		/noparse /ini MQHUDPATH HUDNAME "Vis3" "3,1020,62,255,000,000,${If[${ShowDriverHud} && ${Group.Member[3].Name.NotEqual["NULL"]} && ${Group.Member[3].Type.Equal[PC]} && ${Group.Members}>2 && ${Me.Invis} && !${Group.Member[3].Invis},VISIBLE >>> ${Group.Member[3].Name} <<< VISIBLE,]}"
		/noparse /ini MQHUDPATH HUDNAME "Vis4" "3,1020,74,255,000,000,${If[${ShowDriverHud} && ${Group.Member[4].Name.NotEqual["NULL"]} && ${Group.Member[4].Type.Equal[PC]} && ${Group.Members}>3 && ${Me.Invis} && !${Group.Member[4].Invis},VISIBLE >>> ${Group.Member[4].Name} <<< VISIBLE,]}"
		/noparse /ini MQHUDPATH HUDNAME "Vis5" "3,1020,86,255,000,000,${If[${ShowDriverHud} && ${Group.Member[5].Name.NotEqual["NULL"]} && ${Group.Member[5].Type.Equal[PC]} && ${Group.Members}>4 && ${Me.Invis} && !${Group.Member[5].Invis},VISIBLE >>> ${Group.Member[5].Name} <<< VISIBLE,]}"
		/noparse /ini MQHUDPATH HUDNAME "GroupStatus" "3,1380,26,255,234,008,${If[${ShowDriverHud} && (${GroupPauseFlag} || ${GroupBackOffFlag}),~~~~~~~~~~GROUP STATUS~~~~~~~~~~~,]}"
		/noparse /ini MQHUDPATH HUDNAME "GroupStatus1" "3,1380,38,255,234,008,${If[${ShowDriverHud} && ${GroupPauseFlag},GROUP PAUSED,]}"
		/noparse /ini MQHUDPATH HUDNAME "GroupStatus2" "3,1380,50,255,234,008,${If[${ShowDriverHud} && ${GroupBackOffFlag},GROUP BACKOFF ACTIVE,]}"
		/noparse /ini MQHUDPATH HUDNAME "Mac1" "3,20,206,255,234,008,${If[${ShowMacStatusHud},=== ${MacroName} STATUS ===,]}"
		/noparse /ini MQHUDPATH HUDNAME "Mac2" "3,175,206,255,0,0,${If[${ShowMacStatusHud} && ${Macro.Paused},MACRO PAUSED,]}"
		/noparse /ini MQHUDPATH HUDNAME "Mac3" "3,175,206,255,0,0,${If[${ShowMacStatusHud} && !${Macro.Paused} && ${BackOffFlag},BACKOFF ACTIVE,]}"
	|-IHC Macro Status HUD
		|-Common + General INI
		/noparse /ini MQHUDPATH HUDNAME "comvar1" "3,20,218,255,255,255,${If[${ShowMacStatusHud},[General] FollowToonName:,]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar2" "3,175,218,255,255,255,${If[${ShowMacStatusHud},${FollowToonName},]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar3" "3,20,230,255,255,255,${If[${ShowMacStatusHud},[General] AssistRange:,]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar4" "3,175,230,0,255,0,${If[${ShowMacStatusHud},${AssistRange},]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar5" "3,20,242,255,255,255,${If[${ShowMacStatusHud},[General] AutoAssistAt:,]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar6" "3,175,242,0,255,0,${If[${ShowMacStatusHud},${AutoAssistAt},]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar7" "3,20,254,255,255,255,${If[${ShowMacStatusHud},[General] AutoCampHold:,]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar8" "3,175,254,0,255,0,${If[${ShowMacStatusHud} && ${AutoCampHold},ON,]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar9" "3,175,254,255,0,0,${If[${ShowMacStatusHud} && !${AutoCampHold},OFF,]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar10" "3,20,266,255,255,255,${If[${ShowMacStatusHud},[General] AutoCampRadius:,]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar11" "3,175,266,0,255,0,${If[${ShowMacStatusHud},${AutoCampRadius},]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar12" "3,20,278,255,255,255,${If[${ShowMacStatusHud},[General] BurnAlways:,]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar13" "3,175,278,0,255,0,${If[${ShowMacStatusHud} && ${BurnAlways},ON,]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar14" "3,175,278,255,0,0,${If[${ShowMacStatusHud} && !${BurnAlways},OFF,]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar15" "3,20,290,255,255,255,${If[${ShowMacStatusHud},[General] BurnMobCount:,]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar16" "3,175,290,0,255,0,${If[${ShowMacStatusHud},${BurnMobCount},]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar17" "3,20,302,255,255,255,${If[${ShowMacStatusHud},[General] DoMed:,]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar18" "3,175,302,0,255,0,${If[${ShowMacStatusHud} && ${DoMed},ON,]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar19" "3,175,302,255,0,0,${If[${ShowMacStatusHud} && !${DoMed},OFF,]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar20" "3,20,314,255,255,255,${If[${ShowMacStatusHud},[General] EndMedPct:,]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar21" "3,175,314,0,255,0,${If[${ShowMacStatusHud},${EndMedPct},]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar22" "3,20,326,255,255,255,${If[${ShowMacStatusHud},[General] MedPct:,]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar23" "3,175,326,0,255,0,${If[${ShowMacStatusHud},${MedPct},]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar24" "3,20,338,255,255,255,${If[${ShowMacStatusHud},[General] InstantRelease:,]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar25" "3,175,338,0,255,0,${If[${ShowMacStatusHud} && ${InstantRelease},ON,]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar26" "3,175,338,255,0,0,${If[${ShowMacStatusHud} && !${InstantRelease},OFF,]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar27" "3,20,350,255,255,255,${If[${ShowMacStatusHud},[General] UseFellowship:,]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar28" "3,175,350,0,255,0,${If[${ShowMacStatusHud} && ${UseFellowship},ON,]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar29" "3,175,350,255,0,0,${If[${ShowMacStatusHud} && !${UseFellowship},OFF,]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar30" "3,20,362,255,255,255,${If[${ShowMacStatusHud},[General] UseSmartAssist:,]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar31" "3,175,362,0,255,0,${If[${ShowMacStatusHud} && ${UseSmartAssist},ON,]}"
		/noparse /ini MQHUDPATH HUDNAME "comvar32" "3,175,362,255,0,0,${If[${ShowMacStatusHud} && !${UseSmartAssist},OFF,]}"
		/echo \awWriting IHCMAIN HUD COMPLETE!
		|- End Common + General
	}
/return
|-------------------------------------------------------------------------------------
|- SUB: DeleteHud
|-------------------------------------------------------------------------------------
Sub DeleteHud
	/echo \awDeleting ${MacroName} HUD...
	/ini MQHUDPATH ${MacroName} NULL NULL
	/delay 1s
	/defaulthud
/return 
|-------------------------------------------------------------------------------------
|- SUB: DeleteHud
|-------------------------------------------------------------------------------------
Sub KillTarget
	/varset KillTargetID ${Me.GroupAssistTarget.ID}
	/varset SpawnSearchType targetable
	/stick id ${Me.GroupAssistTarget.ID}
	/call EngageAttack
/return 
|-------------------------------------------------------------------------------------
|- SUB: BIND TglUseHud
|-------------------------------------------------------------------------------------
Sub Bind_TglUseHud
	/docommand /varset changetoini 1
	/if (!${UseHud}) {
		/docommand /varset UseHud 1
		/echo \aw Setting UseHud to \ag ON
	} else {
		/docommand /varset UseHud 0
		/echo \aw Resetting UseHud \ar OFF
	}
	/call IHCHud
/return
|-------------------------------------------------------------------------------------
|- SUB: BIND KillMATarget
|-------------------------------------------------------------------------------------
Sub Bind_KillMATarget
	/call KillTarget
/return
|-------------------------------------------------------------------------------------
|- SUB: BIND QuestSay
|-------------------------------------------------------------------------------------
Sub Bind_QuestSay(string QSayText)
	/bcga //target id ${Target.ID}
	/delay 5
	/bcga //say ${QSayText}
	/delay 5
/return
|-------------------------------------------------------------------------------------
|- SUB: BIND IHCHudUpdate
|-------------------------------------------------------------------------------------
Sub Bind_IHCHudUpdate
	/if (!${UseHud}) {
		/echo \awUseHud is \arOFF
		/echo Nothing to update
	} else {
		/echo Updating HUD...
		/echo \awDeleting IHCMAIN HUD...
		/ini MQHUDPATH HUDNAME NULL NULL
		/delay 1s
		/call DeleteHud
		/delay 1s
		/call IHCHud
	}
/return
|-------------------------------------------------------------------------------------
|- SUB: Help Common - 
|-------------------------------------------------------------------------------------
Sub CommonHelp
/echo \ag===General Commands=== 
/echo \ag/assistrange\aw - How far away the target can be to Attack
/echo \ag/autoassistat\aw - Sets Mob HP% to start attacking at - ex: /autoassistat 98
/echo \ag/backoff\aw - Backoff and stop combat
/echo \ag/burn\aw - Triggered On Demand Buring
/echo \ag/burncount\aw - Sets the number of targets to Start auto burn - ex: /burncount 3 
/echo \ag/campexact\aw - toggles always returning to /camphere /loc x y after combat otherwise you must be outside /campradius before a return happens
/echo \ag/campfire\aw - Sets/Unsets a fellowship campfire
/echo \ag/camphere\aw - Sets a camp at the current /loc x y
/echo \ag/campoff\aw - Unsets the saved x y /loc
/echo \ag/campradius\aw - Radius around /camphere loc considered "in camp"
/echo \ag/chase\aw - toggle follow the main assist on/off
/echo \ag/chaseoff\aw - stop following the main assist
/echo \ag/chaseon\aw - start following the main assist
/echo \ag/deluxehud\aw - Display the Deluxe portion of the HUD LOTS of Info
/echo \ag/driverhud\aw - Display the driver HUD - For people crontrolling one toon as a "driver". Displays Emergency information about your group ( only visible under certain conditions )
/echo \ag/endmedpct\aw - % Endurance to start medding at /tglmed must be On to start medding
/echo \ag/ralleybehind\aw - All chars running IHC mac's will rally around the rear of the char that uses this command 
/echo \ag/ralleyfront\aw - All chars running IHC mac's will rally around the front of the char that uses this command
/echo \ag/ihchelp\aw - List of available commands
/echo \ag/ihchud\aw - Displays the Full IHC Class Macro Hud
/echo \ag/ihcstatus\aw - Displays Information about the various toggle states
/echo \ag/instantrelease\aw - toggle instant respawn on death
/echo \ag/medpct\aw - % Mana to start medding at /tglmed must be On to start medding
/echo \ag/meleeoff\aw - Turn melee combat off
/echo \ag/meleeon\aw - Turn melee combat on
/echo \ag/returntocamp\aw - toggle returning to /loc x y  set with /camphere if outside of camp radius OR always if /campexact is on
/echo \ag/smartassist\aw - toggles waiting till a mob is in melee range of the MA before engaging even if below assist % on incoming
/echo \ag/tglburn\aw - Burn all the time as spells/abilities become available
/echo \ag/tglmed\aw - Toggle automatic medding, med % can be set in your ini file or with /medpct #
/echo \ag/usefellowship\aw - use fellowship insignia if a campfire is set
/return 