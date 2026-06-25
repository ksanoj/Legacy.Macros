|-ihcxtenchmez.inc

|-------------------------------------------------------------------------------------
|-   Sub MezRadar
|-------------------------------------------------------------------------------------
Sub MezRadar
	/declare NMMob int local 0
    /declare i int local   
	/varset MezMobCount 0
    /varset MezMobAECount 0
    /varset MezAEClosest 0
	/for i 1 to 13
        /if (${Me.XTarget[${i}].ID} && ${Me.XTarget[${i}].TargetType.Equal[Auto Hater]} && ${Me.XTarget[${i}].Type.Equal[NPC]}) {
			/varset NMMob ${Me.XTarget[${i}].ID}
            /varcalc MezMobCount ${MezMobCount}+1
			/if (${Spawn[${MezArray[${i},1]}].Type.Equal[Corpse]} || ${Spawn[${MezArray[${i},1]}].PctHPs}<=0 || !${Spawn[${MezArray[${i},1]}].ID}) /call RemoveFromArray MezArray ${i}
            
			|- Setup closest mob for AE mez target necros can't aemez
            /if (!${MezAEClosest} && ${Spawn[${NMMob}].Distance}<=${MezRadius}) /varset MezAEClosest ${NMMob}
            /if (${MezAEClosest} && ${Spawn[${NMMob}].Distance}<${Spawn[${MezAEClosest}].Distance} && ${Spawn[${NMMob}].Distance}<=${MezRadius}) /varset MezAEClosest ${NMMob}
            /if (${Spawn[${NMMob}].Distance}<=${MezRadius}) /varcalc MezMobAECount ${MezMobAECount}+1 
				
            /if (${NMMob} && (${Spawn[${NMMob}].Type.Equal[Corpse]} || !${Spawn[${NMMob}].ID} || ${Spawn[${NMMob}].Distance}>${MezRadius})) /call RemoveFromArray MezArray ${Select[${NMMob},${MezArray[1,1]},${MezArray[2,1]},${MezArray[3,1]},${MezArray[4,1]},${MezArray[5,1]},${MezArray[6,1]},${MezArray[7,1]},${MezArray[8,1]},${MezArray[9,1]},${MezArray[10,1]},${MezArray[11,1]},${MezArray[12,1]},${MezArray[13,1]}]}
			/if (${MezMobCount}>0 && ${Select[${NMMob},${MezArray[1,1]},${MezArray[2,1]},${MezArray[3,1]},${MezArray[4,1]},${MezArray[5,1]},${MezArray[6,1]},${MezArray[7,1]},${MezArray[8,1]},${MezArray[9,1]},${MezArray[10,1]},${MezArray[11,1]},${MezArray[12,1]},${MezArray[13,1]}]}==0) {
                /if (${DebugMez} || ${DEBUGALL}) /echo \atDEBUG MezRadar: ADDING -> Name: ${Spawn[${NMMob}].Name} ID: ${NMMob} to mezlist \agLine#: ${Macro.CurLine}
                /call AddToArray MezArray ${NMMob} 
            }
            /varset NMMob 0
        }
    /next i
    /if (${DebugMez} || ${DEBUGALL}) /delay 5
    /if (${DebugMez} || ${DEBUGALL}) /echo \atDEBUG MezRadar: MezMobCount: ${MezMobCount} Leave \agLine#: ${Macro.CurLine}
/return
|-------------------------------------------------------------------------------------
|- SUB: Add to Array
|-------------------------------------------------------------------------------------
Sub AddToArray(ArrayName, int AddMobID)
    /if (!${AddMobID}) /return
        /if (${DebugMez} || ${DEBUGALL}) /echo \atDEBUG AddToArray ${AddMobID} Enter \agLine#: ${Macro.CurLine}
        /declare i int local
        /for i 1 to 13
            /if (${${ArrayName}[${i},1].Equal[NULL]}) {
                /varset ${ArrayName}[${i},1] ${Spawn[${AddMobID}].ID}
                /varset ${ArrayName}[${i},2] ${Spawn[${AddMobID}].Level}
                /varset ${ArrayName}[${i},3] ${Spawn[${AddMobID}].CleanName}
                /if (${DebugMez} || ${DEBUGALL}) /echo ARRAY Assign >> ${${ArrayName}[${i},3]} << to ${ArrayName}${i}. \agLine#: ${Macro.CurLine}
				/if (${MezCheckElements}<${i}) {
					/varset MezCheckElements ${i}
				}
                /return
            }
        /next i
    /if (${DebugMez} || ${DEBUGALL}) /echo \atDEBUG AddToArray Leave \agLine#: ${Macro.CurLine}
/return
|-------------------------------------------------------------------------------------
|- SUB: Remove From Array
|-------------------------------------------------------------------------------------
Sub RemoveFromArray(RArrayName, int ArNum)
    /if (${${RArrayName}[${ArNum},1].Equal[NULL]}) /return
    /if (${ArNum}<1 || ${ArNum}>${${RArrayName}.Size}) /return
    /if (${DebugMez} || ${DEBUGALL}) /echo \atDEBUG RemoveFromArray ${ArNum} Enter \agLine#: ${Macro.CurLine}
    /if (${DebugMez} || ${DEBUGALL}) /echo \atDEBUG ARRAY Remove >> ${${RArrayName}[${ArNum},3]} << from ${RArrayName}${ArNum}. \agLine#: ${Macro.CurLine}
    /varset ${RArrayName}[${ArNum},1] NULL
    /varset ${RArrayName}[${ArNum},2] NULL
    /varset ${RArrayName}[${ArNum},3] NULL
    /if (${MezOn} && ${ArNum}<=13) {
        /varset MezCount[${ArNum}] 0
        /varset MezTimer${ArNum} 0
		/if (${MezMobCount}<2 && ${AEMezReset}) /varset AEMezReset 0
		/if (${MezMobCount}==0 && ${MezCheckElements}>0) {
			/varset MezCheckElements 0
		}
    }
    /if (${DebugMez} || ${DEBUGALL}) /echo \atDEBUG RemoveFromArray Leave \agLine#: ${Macro.CurLine}
/return
|-------------------------------------------------------------------------------------
|- SUB: Mez Mobs AE
|-------------------------------------------------------------------------------------
Sub MezMobsAE(int AEMezID)
    /declare i int local 1
    /declare WasChasing int local 0
    
	|- Enchanter AE mez code
    /if (${ChaseAssist}) {
		/varset ChaseAssist 0
		/varset WasChasing 1
		/squelch /stick off
		/moveto off
		/delay 30 !${Me.Moving}
    }
	/if (${EQBC.Connected} && ${MezAnnounce}!=0) {
		/bc I AM AE MEZZING ${MezAESpell}
	} else {
		/echo I AM AE MEZZING ${MezAESpell}
	}
	/call CastSpell "${MezAESpell}" ${AEMezID} Mez
            
	/if (${EQBC.Connected} && ${MezAnnounce}!=0) {
		/bc I JUST CAST AE MEZ ${MezAESpell}
	} else {
		/echo I JUST CAST AE MEZ ${MezAESpell}
	}
    /delay 10 ${Me.SpellReady[${MezAESpell}]}
    /varset MezAETimer ${Spell[${MezAESpell}].Duration.TotalSeconds}s
	/varset AEMezReset 1
    /if (${DebugMez} || ${DEBUGALL}) /echo \atDEBUGMEZ TIMER SET ${MezAETimer} \agLine#: ${Macro.CurLine}
    /if (${WasChasing}) /varset ChaseAssist 1

    |- Reset all mez timers to 0 after AE Mez
    /for i 1 to 30
        /varset MezTimer${i} 0
    /next i
/return	
|-------------------------------------------------------------------------------------
|- SUB: Mez Mobs
|-------------------------------------------------------------------------------------
Sub MezMobs(int MobID, int TimerNum)
    /if (${DebugMez} || ${DEBUGALL}) /echo \atDEBUGMEZ MezMobs Enter MobID:${MobID} Timer#:${TimerNum} \agLine#: ${Macro.CurLine}
    /declare MezFail int local 0
    /declare MezTry int local 1
    /declare ReMez int local 0
    /if (${Me.Combat}) {
        /attack off
        /delay 25 !${Me.Combat}
    }
    /squelch /target id ${MobID}
    /delay 20 ${Target.ID}==${MobID} && ${Target.BuffsPopulated}==TRUE
    /if (${Target.ID}==${MobID}) {
        /if (${Int[${Target.Mezzed.ID}]} && ${Target.Mezzed.Name.Equal[${MezSpell}]}) {
            /if (${DebugMez} || ${DEBUGALL}) /echo DEBUGMEZ MezTimer${TimerNum} ${MezTimer${TimerNum}} Target Info: ${Target.Mezzed.ID} ${Target.Mezzed.Name} ${Target.BuffDuration[${Target.Mezzed.Name}].TotalSeconds} Line#: ${Macro.CurLine}
            /if (${Target.BuffDuration[${Target.Mezzed.Name}].TotalSeconds}>${Math.Calc[(${Spell[${MezSpell}].Duration.TotalSeconds}+${MezMod})*.10]}) {
                /varcalc MezCount[${TimerNum}] 1
                /varcalc MezTimer${TimerNum} (${Target.BuffDuration[${Target.Mezzed.Name}].TotalSeconds}*10)*.85
                /return
            }            
        }
        /if (${MezCount[${TimerNum}]}<1) {
			/if (${EQBC.Connected} && ${MezAnnounce}!=0) {
				/bc "MEZZING -> ${Spawn[${MobID}].CleanName} <- ID:${MobID}"
			} else {
				/echo "MEZZING -> ${Spawn[${MobID}].CleanName} <- ID:${MobID}"
			}
        } else {
			/if (${EQBC.Connected} && ${MezAnnounce}!=0) {
				/bc "ReMEZZING -> ${Spawn[${MobID}].CleanName} <- ID:${MobID}"
			} else {
				/echo "ReMEZZING -> ${Spawn[${MobID}].CleanName} <- ID:${MobID}"
			}
            /varset ReMez 1
        }
        |- Chanter mez code
        :retrymez
        /if (${Me.Class.Name.Equal[Enchanter]}) {
			/call CastSpell "${MezSpell}" ${MobID} Mez
            /varcalc MezFail ${MezFail}+1
            /if (${Macro.Return.Equal[CAST_SUCCESS]}) {
                /if (!${ReMez}) {
					/if (${EQBC.Connected} && ${MezAnnounce}!=0) {
						/bc "JUST MEZZED -> ${MezSpell} on ${Spawn[${MobID}].CleanName}:${MobID}"
					} else {
						/echo "JUST MEZZED -> ${MezSpell} on ${Spawn[${MobID}].CleanName}:${MobID}"
					}
				}
                /if (${ReMez}) {
					/if (${EQBC.Connected} && ${MezAnnounce}!=0) {
						/bc "JUST REMEZZED -> ${MezSpell} on ${Spawn[${MobID}].CleanName}:${MobID}"
					} else {
						/echo "JUST REMEZZED -> ${MezSpell} on ${Spawn[${MobID}].CleanName}:${MobID}"
					}
				}
                /varcalc MezCount[${TimerNum}] ${MezCount[${TimerNum}]}+1
                /varcalc MezTimer${TimerNum} ((${Spell[${MezSpell}].Duration.TotalSeconds}+${MezMod})*10)*.90
                /if (${DebugMez} || ${DEBUGALL}) /echo DEBUGMEZ MezTimer${TimerNum} ${MezTimer${TimerNum}} Line#: ${Macro.CurLine}
            }
            /if (${Macro.Return.Equal[CAST_RESIST]} && ${MezFail}<2) {
				/if (${EQBC.Connected} && ${MezAnnounce}!=0) {
					/bc "MEZ Resisted -> ${Spawn[${MobID}].CleanName} <- ID:${MobID}"
				} else {
					/echo "MEZ Resisted -> ${Spawn[${MobID}].CleanName} <- ID:${MobID}"
				}
                /goto :retrymez
            }
            /if (${Macro.Return.Equal[CAST_IMMUNE]}) {
                /if (!${MezImmuneIDs.Find[|${Target.ID}]}) /call AddMezImmune ${Target.ID}
            }
        }
        /varset MezTry 0
    }
    /if (${DebugMez} || ${DEBUGALL}) /echo \atDEBUGMEZ MezMobs Leave \agLine#: ${Macro.CurLine}
/return 
|-------------------------------------------------------------------------------------
|- SUB: Do Mez Stuff
|-------------------------------------------------------------------------------------
Sub DoMezStuff
    /doevents 
    /declare i int local 0
    /declare j int local 0
	/varset MezWait 0
    |- Reset skip mez on health setting if tank dies to 1%
    /if (!${Spawn[=${MainAssist}].ID}) /varset MezStopHPs 1
				
    /call MezRadar
 
	/if (${DebugMez} || ${DEBUGALL}) /echo DoMezStuff MezMobCount ${MezMobCount} \agLine#: ${Macro.CurLine}
|-	/if (${MezMobCount}==0 && ${MezCheckElements}>0) {
|-			/call ResetMezArrays
|-			/return
|-		}
|-    /if (${MezMobCount}<2 && ${Spawn[=${MainAssist}].ID}) {
|-        /if (${DebugMez} || ${DEBUGALL}) /echo MezMobCount was ${MezMobCount} (so less than 2) & ${MainAssist} is alive so we return \agLine#: ${Macro.CurLine}
|-           /return
|-   }
    /if (${Select[${MezOn},1,3]} && ${MezAECount}>0 && ${MezMobAECount}>=${MezAECount} && ${MezAETimer}==0 && ${AEMezReset}==0)  {
        /if (${DebugMez} || ${DEBUGALL}) /echo I'm about to AEMez \agLine#: ${Macro.CurLine}
        /if (${SpawnCount[npc xtarhater loc ${Spawn[id ${MezAEClosest}].X} ${Spawn[id ${MezAEClosest}].Y} radius ${Spell[${MezAESpell}].AERange}]}>=${SpawnCount[npc loc ${Spawn[id ${MezAEClosest}].X} ${Spawn[id ${MezAEClosest}].Y} radius ${Spell[${MezAESpell}].AERange}]})  /call MezMobsAE ${MezAEClosest}
    }
    /for i 1 to 13
        |- Every Mez test conditon is listed seperately for clarity
        /if (${DebugMez} || ${DEBUGALL}) /echo we are in the mezcondition loop i is: ${i} \agLine#: ${Macro.CurLine}
        |- Test -> Is my single mez spell ready
        /if (!${Me.SpellReady[${MezSpell}]}) {
			/varset MezWait 1
            /if (${DebugMez} || ${DEBUGALL}) /echo ${MezSpell} not ready, goto return \agLine#: ${Macro.CurLine}
            /return
        }
        |- Test -> array not empty,
        /if (${MezArray[${i},1].Equal[NULL]}) {
            /if (${DebugMez} || ${DEBUGALL}) /echo ${i}  MezArray[${i},1] was equal to NULL, goto :SkipMez \agLine#: ${Macro.CurLine}
            /goto :SkipMez
        }
        |- Test -> Target is dead
        /if (${MezArray[${i},3].Find[corpse]} || !${Spawn[${MezArray[${i},1]}].ID} || ${Spawn[${MezArray[${i},1]}].Type.Equal[Corpse]}) {
            /if (${DebugMez} || ${DEBUGALL}) /echo ${i}  ${MezArray[${i},3]} is dead, goto :SkipMez \agLine#: ${Macro.CurLine}
			/call RemoveFromArray MezArray ${i}
			/goto :SkipMez
        }
        |- Is mob in MezRadius distance
        /if (${Spawn[${MezArray[${i},1]}].Distance}>=${MezRadius}) {
            /if (${DebugMez} || ${DEBUGALL}) /echo ${i}  Mob distance is greater than MezRadius: ${Spawn[${MezArray[${i},1]}].Distance} ${MezRadius} goto :SkipMez \agLine#: ${Macro.CurLine}
            /varset MezWait 1
			/goto :SkipMez
        }
        |- Test -> is target MA's current Target
        /if (${Spawn[${MezArray[${i},1]}].ID}==${Me.GroupAssistTarget.ID} && ${Spawn[=${MainAssist}].ID}) {
            /if (${DebugMez} || ${DEBUGALL}) /echo ${Spawn[${MezArray[${i},1]}].Name} had the same ID(${Spawn[${MezArray[${i},1]}].ID}) as MyTargetID(${Me.GroupAssistTarget.ID}) & ${MainAssist}(${Spawn[=${MainAssist}].ID}), goto :SkipMez \agLine#: ${Macro.CurLine}
            /goto :SkipMez
        }
        |- Test -> is taget above mez hps threshold
        /if (${Spawn[${MezArray[${i},1]}].PctHPs}<${MezStopHPs}) {
            /if (${DebugMez} || ${DEBUGALL}) /echo Spawn[MezArray[${i},1]].PctHPs(${Spawn[${MezArray[${i},1]}].PctHPs}) was less than MezStopHPs(${MezStopHPs}), goto :SkipMez \agLine#: ${Macro.CurLine}
            /goto :SkipMez
        }
        |- Test -> is target within levels defined in ini file
        /if (${MezArray[${i},2]}>${MezMaxLevel} || ${MezArray[${i},2]}<${MezMinLevel}) {
            /if (${DebugMez} || ${DEBUGALL}) /echo MezArray[${i},2](${MezArray[${i},2]}) was greater than MezMaxLevel(${MezMaxLevel}) OR less than MezMinLevel(${MezMinLevel}), goto :SkipMez \agLine#: ${Macro.CurLine}
            /goto :SkipMez
        }
        |- Test -> is the target in line of sight
        /if (!${Spawn[${MezArray[${i},1]}].LineOfSight}) {
            /if (${DebugMez} || ${DEBUGALL}) /echo I dont have LineOfSight to Spawn[MezArray[${i},1]](${Spawn[${MezArray[${i},1]}].Name} ID:${Spawn[${MezArray[${i},1]}].ID}), goto :SkipMez \agLine#: ${Macro.CurLine}
            /goto :SkipMez
        }
        |- Test -> is target a giant unmezzable
        /if (${Spawn[${MezArray[${i},1]}].Body.Name.Equal[Giant]}) {
            /if (${DebugMez} || ${DEBUGALL}) /echo Spawn[MezArray[${i},1]] (${Spawn[${MezArray[${i},1]}].Name} ID:${MezArray[${i},1]}) is a giant, goto :SkipMez \agLine#: ${Macro.CurLine}
            /goto :SkipMez
        }
        |- Test -> Do i have enough mana to cast the spell 
        /if (${Me.CurrentMana}<${Spell[${MezSpell}].Mana}) {
            /if (${DebugMez} || ${DEBUGALL}) /echo i didnt have enough mana to cast ${MezSpell}, goto :SkipMez \agLine#: ${Macro.CurLine}
            /goto :SkipMez
        }
        |- Test -> Do i have a mez timer on the mob?
        /if (${MezTimer${i}} > 0) {
            /if (${DebugMez} || ${DEBUGALL}) /echo MezTimer${i}(${MezTimer${i}}) was greater than 0 , goto :SkipMez \agLine#: ${Macro.CurLine}
            /goto :SkipMez
        }
        |- Stop mezzing last mob because pets and mercs won't attack it.
        /if (${MezMobCount}<=1 && ${Spawn[=${MainAssist}].ID} && (${Spawn[=${MainAssist}].Type.Equal[Mercenary]} || ${Spawn[=${MainAssist}].Type.Equal[Pet]})) {
            /if (${DebugMez} || ${DEBUGALL}) /echo MezMobCount(${MezMobCount}) was less or equal to 1 & ( MainAssist(${MainAssist}) was a Mercenary OR a Pet ), goto :SkipMez \agLine#: ${Macro.CurLine}
            /goto :SkipMez
        }
        /if (${Spawn[${MainAssist} ${MainAssistType} group].ID} && ${Select[${MezArray[${i},1]},${Me.XTarget[1].ID},${Me.XTarget[2].ID},${Me.XTarget[3].ID},${Me.XTarget[4].ID},${Me.XTarget[5].ID},${Me.XTarget[6].ID},${Me.XTarget[7].ID},${Me.XTarget[8].ID},${Me.XTarget[9].ID},${Me.XTarget[10].ID},${Me.XTarget[11].ID},${Me.XTarget[12].ID},${Me.XTarget[13].ID}]}==0) {
            /if (${DebugMez} || ${DEBUGALL}) /echo If tank is alive and mob not on xtarget go to :SkipMez \agLine#: ${Macro.CurLine}
            /goto :SkipMez
        }
        /if (${MezImmuneIDs.Find[|${MezArray[${i},1]}]}) {
            /if (${DebugMez} || ${DEBUGALL}) /echo Mez Immune Mob Detected: ${MezArray[${i},3]} go to :SkipMez \agLine#: ${Macro.CurLine}
            /goto :SkipMez
        }
        /if (${DebugMez} || ${DEBUGALL}) /echo \atDEBUGMEZ MezTimer${i} ${MezTimer${i}} ${Spawn[${MezArray[${i},1]}].ID} ${i} \agLine#: ${Macro.CurLine}
        /if (${Select[${MezOn},1,2]}) {
            /if (${DebugMez} || ${DEBUGALL}) /echo im gonna singlemez ${MezArray[${i},1]} \agLine#: ${Macro.CurLine}
            /call MezMobs ${MezArray[${i},1]} ${i}
        }
        :SkipMez
    /next i
    /if (${DebugMez} || ${DEBUGALL}) /echo \atDEBUGMEZ DoMezStuff: Leave \agLine#: ${Macro.CurLine}
/return
|----------------------------------------------------------------------------
|- SUB: Create Timers Mez
|----------------------------------------------------------------------------    
Sub CreateTimersMez
    |- Declare timers for mezzing
    /if (${Select[${Me.Class.ShortName},BRD,ENC,NEC]}) {
        /declare l int local
        /for l 1 to 30
            /if (!${Defined[MezTimer${l}]}) /declare MezTimer${l} timer outer 0
        /next l
    }
/return
|----------------------------------------------------------------------------
|- SUB: Delete Timers Mez
|----------------------------------------------------------------------------    
Sub DeleteTimersMez
    |- Declare timers for mezzing
    /if (${Select[${Me.Class.ShortName},BRD,ENC,NEC]}) {
        /declare l int local
        /for l 1 to 30
			/if (${Defined[MezTimer${l}]}) /deletevar MezTimer${l}
        /next l
    }
/return
|----------------------------------------------------------------------------
|- SUB: ResetMezArrays
|----------------------------------------------------------------------------    
Sub ResetMezArrays
    /declare l int local
    /for l 1 to 13
		/call RemoveFromArray MezArray ${l}
    /next l 
		/varset MezCheckElements 0
/return
|-------------------------------------------------------------------------------------
|- SUB: Event MezBroke
|-------------------------------------------------------------------------------------
Sub Event_MezBroke(meztext,mezmob,mezbreaker)
    /if (${DebugMez} || ${DEBUGALL}) /echo \atDEBUGMEZ event mezbroke Enter \agLine#: ${Macro.CurLine}
    /if (!${MezOn}) /return
|-    /if (${Spawn[${mezbreaker}].CleanName.Equal[${MainAssist}]}) /return
|-    /if (${Spawn[${MainAssist}].Type.Equal[pet]} && ${Spawn[${MainAssist}].Master.CleanName.Equal[${mezbreaker}]}) /return
    /declare tempMobID int local ${Target.ID}
    /assist ${mezbreaker}
    /delay 5s ${Me.AssistComplete}==TRUE
    /if (${Target.ID} && ${Target.ID}==${Me.GroupAssistTarget.ID}) {
        /target id ${tempMobID}
        /delay 10 ${Target.ID}==${tempMobID}
        /return
    }
    /declare i int local
	/if (${EQBC.Connected} && ${MezAnnounce}!=0) {
		/bc >>  ${Spawn[${mezbreaker}].CleanName} << has awakened -> ${mezmob}<-"
	} else {
		/echo >>  ${Spawn[${mezbreaker}].CleanName} << has awakened -> ${mezmob}<-"
	}    
    /for i 1 to 30
        /if (${MezArray[${i},3].Equal[${mezmob}]}) {
            /echo Resetting Mez Timer ${mezmob} ID: ${MezArray[${i},1]}
            /varset MezTimer${i} 0
        }
    /next i
    /doevents flush MezBroke
    /call DoMezStuff
    |- Set MezOn = 2 to let us know that the event was triggered. Will be set back to 1 in other location
    /varset MezBroke 1
    /if (${DebugMez} || ${DEBUGALL}) /echo \atDEBUGMEZ event mezbroke Leave \agLine#: ${Macro.CurLine}
/return
|-------------------------------------------------------------------------------------
|- SUB: Event MezImmune
|-------------------------------------------------------------------------------------
Sub Event_MezImmune(int MezID)
    /if (${Select[${Me.Class.ShortName},Brd,Enc,Nec]}==0) /return
    |- Assign temp var MezImmune list
    /declare ImmuneAdd string local ${MezImmune}
    |- If mezimmune default text with the word null in it assign var spawn clean name
    /if (${ImmuneAdd.Find[null]}) {
        /varset ImmuneAdd ${Spawn[${MezID}].CleanName}
    } else {
        /varset ImmuneAdd ${ImmuneAdd},${Spawn[${MezID}].CleanName}
    }
    /if (!${MezImmune.Find[${Spawn[${MezID}].CleanName}]}) /ini "${InfoFileName}" "${Zone}${If[${Me.InInstance},_I,]}" "MezImmune" "${ImmuneAdd}"
	/if (${EQBC.Connected} && ${MezAnnounce}!=0) {
		/bc "MEZ Immune -> ${Spawn[${MezID}].CleanName} <- ID:${MezID} Adding to MezImmune list."
	} else {
		/echo "MEZ Immune -> ${Spawn[${MezID}].CleanName} <- ID:${MezID} Adding to MezImmune list."
	}		
	|- Reassign mezimmune var the new list
    /varset MezImmune ${ImmuneAdd}
/return
|-------------------------------------------------------------------------------------
|- SUB: AddMezImmune
|-------------------------------------------------------------------------------------
Sub AddMezImmune(int MezID)
	/if (${Select[${Me.Class.ShortName},Brd,Enc,Nec]}==0) /return
    /if (${MezImmuneIDs.Find[|${MezID}]}) /return
    /varset MezImmuneIDs ${MezImmuneIDs}|${MezID} 
	/if (${EQBC.Connected} && ${MezAnnounce}!=0) {
		/bc "MEZ Immune -> ${Spawn[${MezID}].CleanName} <- ID:${MezID} Skipping."
	} else {
		/echo "MEZ Immune -> ${Spawn[${MezID}].CleanName} <- ID:${MezID} Skipping."
	}
/return 