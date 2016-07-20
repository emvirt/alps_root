#ifndef GTF_INCLUDED_DATAPOOL_IDENTIFIERS_OF_CONTEXT_1
#define GTF_INCLUDED_DATAPOOL_IDENTIFIERS_OF_CONTEXT_1

#include <GtfTypes/GtfStdDef.h>

#ifndef DP_CONTEXT_ID
    #define DP_CONTEXT_ID ((uint8_t) 0x01)
#else
    #error "Including multiple datapool headers may lead to ambiguous ID definitions!"
#endif

//! @brief Type : bool
#define DP_ID_AAF1_Aaf_WrnLamp ((uint32_t) 0x000000C0)

//! @brief Type : int32_t
#define DP_ID_AFC_Unit ((uint32_t) 0x00000120)

//! @brief Type : int32_t
#define DP_ID_AFLS_trfChgStat_usm ((uint32_t) 0x0000012E)

//! @brief Type : int32_t
#define DP_ID_AHLS_WarnMsg ((uint32_t) 0x0000012F)

//! @brief Type : int32_t
#define DP_ID_AV_Disc_select_No ((uint32_t) 0x0000005A)

//! @brief Type : int32_t
#define DP_ID_AV_Frequency ((uint32_t) 0x0000005B)

//! @brief Type : bool
#define DP_ID_AV_Navi_On_Off ((uint32_t) 0x00000053)

//! @brief Type : int32_t
#define DP_ID_AV_OpState ((uint32_t) 0x00000052)

//! @brief Type : int32_t
#define DP_ID_AV_Play_time_Hour ((uint32_t) 0x00000059)

//! @brief Type : int32_t
#define DP_ID_AV_Play_time_Min ((uint32_t) 0x00000058)

//! @brief Type : int32_t
#define DP_ID_AV_Play_time_Sec ((uint32_t) 0x00000057)

//! @brief Type : int32_t
#define DP_ID_AV_Preset_Number ((uint32_t) 0x00000054)

//! @brief Type : int32_t
#define DP_ID_AV_SDARS_ChannelNo ((uint32_t) 0x0000005C)

//! @brief Type : int32_t
#define DP_ID_AV_SPS_ChannelNo ((uint32_t) 0x0000005D)

//! @brief Type : int32_t
#define DP_ID_AV_Track_Number ((uint32_t) 0x00000056)

//! @brief Type : int32_t
#define DP_ID_AV_Tuner_Area ((uint32_t) 0x00000055)

//! @brief Type : int32_t
#define DP_ID_BCReset_BC_Mode ((uint32_t) 0x000000F5)

//! @brief Type : bool
#define DP_ID_BCReset_RESET_AFC ((uint32_t) 0x000000F6)

//! @brief Type : bool
#define DP_ID_BCReset_RESET_AVS ((uint32_t) 0x000000F7)

//! @brief Type : bool
#define DP_ID_BCReset_RESET_DT ((uint32_t) 0x000000F8)

//! @brief Type : bool
#define DP_ID_BCReset_RESET_TRIP ((uint32_t) 0x000000F9)

//! @brief Type : float
#define DP_ID_BC_AFC ((uint32_t) 0x00000010)

//! @brief Type : int32_t
#define DP_ID_BC_AFC_Valid ((uint32_t) 0x00000013)

//! @brief Type : int32_t
#define DP_ID_BC_DTE ((uint32_t) 0x0000000F)

//! @brief Type : int32_t
#define DP_ID_BC_DTE_Unit ((uint32_t) 0x0000011F)

//! @brief Type : int32_t
#define DP_ID_BC_DTE_Valid ((uint32_t) 0x00000012)

//! @brief Type : float
#define DP_ID_BC_IFC ((uint32_t) 0x00000011)

//! @brief Type : int32_t
#define DP_ID_BC_IFC_Valid ((uint32_t) 0x00000014)

//! @brief Type : int32_t
#define DP_ID_CF_Spas_BEEP_Alarm ((uint32_t) 0x000000D6)

//! @brief Type : int32_t
#define DP_ID_CF_Spas_FIL_Ind ((uint32_t) 0x000000D8)

//! @brief Type : int32_t
#define DP_ID_CF_Spas_FIR_Ind ((uint32_t) 0x000000D9)

//! @brief Type : int32_t
#define DP_ID_CF_Spas_FI_Ind ((uint32_t) 0x000000D7)

//! @brief Type : int32_t
#define DP_ID_CF_Spas_FOL_Ind ((uint32_t) 0x000000DA)

//! @brief Type : int32_t
#define DP_ID_CF_Spas_FOR_Ind ((uint32_t) 0x000000DB)

//! @brief Type : int32_t
#define DP_ID_CF_Spas_FR_Alarm ((uint32_t) 0x000000DC)

//! @brief Type : int32_t
#define DP_ID_CF_Spas_RIL_Ind ((uint32_t) 0x000000DE)

//! @brief Type : int32_t
#define DP_ID_CF_Spas_RIR_Ind ((uint32_t) 0x000000DF)

//! @brief Type : int32_t
#define DP_ID_CF_Spas_RI_Ind ((uint32_t) 0x000000DD)

//! @brief Type : int32_t
#define DP_ID_CF_Spas_ROL_Ind ((uint32_t) 0x000000E0)

//! @brief Type : int32_t
#define DP_ID_CF_Spas_ROR_Ind ((uint32_t) 0x000000E1)

//! @brief Type : int32_t
#define DP_ID_CF_Spas_RR_Alarm ((uint32_t) 0x000000E2)

//! @brief Type : int32_t
#define DP_ID_CLOCK_AC_HU_DisplayDay ((uint32_t) 0x00000118)

//! @brief Type : int32_t
#define DP_ID_CLOCK_AC_HU_DisplayMonth ((uint32_t) 0x00000117)

//! @brief Type : int32_t
#define DP_ID_CLOCK_AC_HU_DisplayYear1 ((uint32_t) 0x00000115)

//! @brief Type : int32_t
#define DP_ID_CLOCK_AC_HU_DisplayYear2 ((uint32_t) 0x00000116)

//! @brief Type : int32_t
#define DP_ID_CLOCK_AC_HU_FTDHour ((uint32_t) 0x000000D1)

//! @brief Type : int32_t
#define DP_ID_CLOCK_AC_HU_FTDMinute ((uint32_t) 0x000000D2)

//! @brief Type : int32_t
#define DP_ID_CLOCK_AC_HU_FTDSeconds ((uint32_t) 0x000000D3)

//! @brief Type : int32_t
#define DP_ID_DEV_SystemState ((uint32_t) 0x00000003)

//! @brief Type : int32_t
#define DP_ID_DIM_DimLevel ((uint32_t) 0x0000003B)

//! @brief Type : bool
#define DP_ID_DIM_Rheostat ((uint32_t) 0x00000122)

//! @brief Type : bool
#define DP_ID_DOOR_FrontLeft ((uint32_t) 0x0000003C)

//! @brief Type : bool
#define DP_ID_DOOR_FrontRight ((uint32_t) 0x0000003D)

//! @brief Type : bool
#define DP_ID_DOOR_Hood ((uint32_t) 0x00000041)

//! @brief Type : bool
#define DP_ID_DOOR_RearLeft ((uint32_t) 0x0000003E)

//! @brief Type : bool
#define DP_ID_DOOR_RearRight ((uint32_t) 0x0000003F)

//! @brief Type : bool
#define DP_ID_DOOR_SunRoof ((uint32_t) 0x00000042)

//! @brief Type : bool
#define DP_ID_DOOR_TrunkLid ((uint32_t) 0x00000040)

//! @brief Type : int32_t
#define DP_ID_EHPS1_Warn ((uint32_t) 0x00000105)

//! @brief Type : bool
#define DP_ID_EMS_EtcLimpMod ((uint32_t) 0x000000BB)

//! @brief Type : bool
#define DP_ID_EMS_FcOpen ((uint32_t) 0x000000BC)

//! @brief Type : int32_t
#define DP_ID_EPB1_EPB_CLU ((uint32_t) 0x000000CE)

//! @brief Type : int32_t
#define DP_ID_EPB_CLU_HOST ((uint32_t) 0x00000043)

//! @brief Type : int32_t
#define DP_ID_ESC_OFF_STEP ((uint32_t) 0x0000010A)

//! @brief Type : int32_t
#define DP_ID_ESP1_AVH_CLU ((uint32_t) 0x000000CD)

//! @brief Type : bool
#define DP_ID_FLEX_Display ((uint32_t) 0x0000004E)

//! @brief Type : int32_t
#define DP_ID_FLEX_Mode ((uint32_t) 0x0000004D)

//! @brief Type : bool
#define DP_ID_FLEX_SW_Pressed ((uint32_t) 0x0000004C)

//! @brief Type : int32_t
#define DP_ID_GAUGE_CoolTemp ((uint32_t) 0x0000000E)

//! @brief Type : int32_t
#define DP_ID_GAUGE_EngineRev ((uint32_t) 0x0000000C)

//! @brief Type : float
#define DP_ID_GAUGE_FuelLevel ((uint32_t) 0x0000000D)

//! @brief Type : float
#define DP_ID_GAUGE_Speed ((uint32_t) 0x0000000A)

//! @brief Type : int32_t
#define DP_ID_GAUGE_SpeedUnit ((uint32_t) 0x0000000B)

//! @brief Type : int32_t
#define DP_ID_GDCNoSave_AutoDoorLock ((uint32_t) 0x000000EC)

//! @brief Type : int32_t
#define DP_ID_GDCNoSave_AutoDoorUnlock ((uint32_t) 0x000000ED)

//! @brief Type : int32_t
#define DP_ID_GDCNoSave_AutoSideMirrorFolding ((uint32_t) 0x000000F4)

//! @brief Type : int32_t
#define DP_ID_GDCNoSave_AutoTripleTurn ((uint32_t) 0x000000F1)

//! @brief Type : int32_t
#define DP_ID_GDCNoSave_HeadLampEscort ((uint32_t) 0x000000F0)

//! @brief Type : int32_t
#define DP_ID_GDCNoSave_HornFeedback ((uint32_t) 0x000000EF)

//! @brief Type : int32_t
#define DP_ID_GDCNoSave_SeatEasyAccess ((uint32_t) 0x000000F3)

//! @brief Type : int32_t
#define DP_ID_GDCNoSave_TwoPressUnlock ((uint32_t) 0x000000EE)

//! @brief Type : int32_t
#define DP_ID_GDCNoSave_WelcomeLight ((uint32_t) 0x000000F2)

//! @brief Type : int32_t
#define DP_ID_GDCSave_AFCAutoReset ((uint32_t) 0x000000E7)

//! @brief Type : int32_t
#define DP_ID_GDCSave_DialFontSize ((uint32_t) 0x000000E8)

//! @brief Type : int32_t
#define DP_ID_GDCSave_Language ((uint32_t) 0x000000E9)

//! @brief Type : int32_t
#define DP_ID_GDCSave_ServiceReminder ((uint32_t) 0x0000012D)

//! @brief Type : int32_t
#define DP_ID_GDCSave_ShiftIndicatorGuidance ((uint32_t) 0x000000E6)

//! @brief Type : int32_t
#define DP_ID_GDCSave_SteeringPositionAlarm ((uint32_t) 0x000000E5)

//! @brief Type : int32_t
#define DP_ID_GDCSave_VoiceGuidanceConfig ((uint32_t) 0x000000EB)

//! @brief Type : int32_t
#define DP_ID_GDCSave_VoiceGuidanceVolume ((uint32_t) 0x000000EA)

//! @brief Type : int32_t
#define DP_ID_GDCSave_WelcomeSound ((uint32_t) 0x000000E4)

//! @brief Type : int32_t
#define DP_ID_GEAR_ShiftGuide ((uint32_t) 0x00000047)

//! @brief Type : int32_t
#define DP_ID_GEAR_ShiftStatus ((uint32_t) 0x00000045)

//! @brief Type : int32_t
#define DP_ID_GEAR_Sports ((uint32_t) 0x00000046)

//! @brief Type : bool
#define DP_ID_GWAY6_KeyReminderWarning ((uint32_t) 0x00000113)

//! @brief Type : bool
#define DP_ID_GWAY6_KeyTeachingSound ((uint32_t) 0x00000114)

//! @brief Type : bool
#define DP_ID_Gway3_ASTDRVWdwStat ((uint32_t) 0x000000C2)

//! @brief Type : bool
#define DP_ID_Gway3_DRVWdwStat ((uint32_t) 0x000000C1)

//! @brief Type : bool
#define DP_ID_Gway3_RLWdwStat ((uint32_t) 0x000000C3)

//! @brief Type : bool
#define DP_ID_Gway3_RRDRVWdwStat ((uint32_t) 0x000000C4)

//! @brief Type : bool
#define DP_ID_Gway6_DeliveryMode ((uint32_t) 0x000000BD)

//! @brief Type : int32_t
#define DP_ID_Gway6_RkeCmd ((uint32_t) 0x000000BE)

//! @brief Type : int32_t
#define DP_ID_HOSTNoSave_AutoDoorLock ((uint32_t) 0x00000078)

//! @brief Type : int32_t
#define DP_ID_HOSTNoSave_AutoDoorUnlock ((uint32_t) 0x00000079)

//! @brief Type : int32_t
#define DP_ID_HOSTNoSave_AutoSideMirrorFolding ((uint32_t) 0x00000080)

//! @brief Type : int32_t
#define DP_ID_HOSTNoSave_AutoTripleTurn ((uint32_t) 0x0000007D)

//! @brief Type : int32_t
#define DP_ID_HOSTNoSave_HeadLampEscort ((uint32_t) 0x0000007C)

//! @brief Type : int32_t
#define DP_ID_HOSTNoSave_HornFeedback ((uint32_t) 0x0000007B)

//! @brief Type : int32_t
#define DP_ID_HOSTNoSave_SeatEasyAccess ((uint32_t) 0x0000007F)

//! @brief Type : int32_t
#define DP_ID_HOSTNoSave_TwoPressUnlock ((uint32_t) 0x0000007A)

//! @brief Type : int32_t
#define DP_ID_HOSTNoSave_WelcomeLight ((uint32_t) 0x0000007E)

//! @brief Type : int32_t
#define DP_ID_HOSTSave_AFCAutoReset ((uint32_t) 0x00000073)

//! @brief Type : int32_t
#define DP_ID_HOSTSave_DialFontSize ((uint32_t) 0x00000074)

//! @brief Type : int32_t
#define DP_ID_HOSTSave_Language ((uint32_t) 0x00000075)

//! @brief Type : int32_t
#define DP_ID_HOSTSave_ServiceReminder ((uint32_t) 0x00000124)

//! @brief Type : int32_t
#define DP_ID_HOSTSave_ShiftIndicatorGuidance ((uint32_t) 0x00000072)

//! @brief Type : int32_t
#define DP_ID_HOSTSave_SteeringPositionAlarm ((uint32_t) 0x00000071)

//! @brief Type : int32_t
#define DP_ID_HOSTSave_VoiceGuidanceConfig ((uint32_t) 0x00000077)

//! @brief Type : int32_t
#define DP_ID_HOSTSave_VoiceGuidanceVolume ((uint32_t) 0x00000076)

//! @brief Type : int32_t
#define DP_ID_HOSTSave_WelcomeSound ((uint32_t) 0x00000070)

//! @brief Type : int32_t
#define DP_ID_IFC_Unit ((uint32_t) 0x00000121)

//! @brief Type : int32_t
#define DP_ID_ISG_Message ((uint32_t) 0x00000049)

//! @brief Type : int32_t
#define DP_ID_ISG_Minute ((uint32_t) 0x0000004A)

//! @brief Type : int32_t
#define DP_ID_ISG_Second ((uint32_t) 0x0000004B)

//! @brief Type : int32_t
#define DP_ID_LCA1_CF_Lca_stat ((uint32_t) 0x000000CF)

//! @brief Type : int32_t
#define DP_ID_LCA1_IndLeft ((uint32_t) 0x0000011A)

//! @brief Type : int32_t
#define DP_ID_LCA1_IndRight ((uint32_t) 0x00000119)

//! @brief Type : bool
#define DP_ID_LDWS_FunStop ((uint32_t) 0x00000107)

//! @brief Type : int32_t
#define DP_ID_LDWS_LHWarning ((uint32_t) 0x00000130)

//! @brief Type : int32_t
#define DP_ID_LDWS_RHWarning ((uint32_t) 0x00000131)

//! @brief Type : int32_t
#define DP_ID_LDWS_SysStat ((uint32_t) 0x00000106)

//! @brief Type : int32_t
#define DP_ID_LKAS_LHWarning ((uint32_t) 0x00000038)

//! @brief Type : int32_t
#define DP_ID_LKAS_RHWarning ((uint32_t) 0x00000039)

//! @brief Type : int32_t
#define DP_ID_LKAS_SysStat ((uint32_t) 0x00000036)

//! @brief Type : int32_t
#define DP_ID_LKAS_SysWarning ((uint32_t) 0x00000037)

//! @brief Type : bool
#define DP_ID_LVR11_CF_Lvr_NFnStat ((uint32_t) 0x000000CC)

//! @brief Type : int32_t
#define DP_ID_LVR11_CF_Lvr_PRelStat ((uint32_t) 0x000000CA)

//! @brief Type : int32_t
#define DP_ID_LVR11_CF_Lvr_ShfErrInf ((uint32_t) 0x000000CB)

//! @brief Type : bool
#define DP_ID_OAT_Unit ((uint32_t) 0x00000123)

//! @brief Type : int32_t
#define DP_ID_OAT_Valid ((uint32_t) 0x00000132)

//! @brief Type : float
#define DP_ID_OAT_Value ((uint32_t) 0x00000048)

//! @brief Type : int32_t
#define DP_ID_ODO_TotalODO ((uint32_t) 0x00000009)

//! @brief Type : bool
#define DP_ID_ODO_Unit ((uint32_t) 0x00000002)

//! @brief Type : int32_t
#define DP_ID_PAS_FrontCenter ((uint32_t) 0x00000029)

//! @brief Type : int32_t
#define DP_ID_PAS_FrontLeft ((uint32_t) 0x00000028)

//! @brief Type : int32_t
#define DP_ID_PAS_FrontRight ((uint32_t) 0x0000002A)

//! @brief Type : int32_t
#define DP_ID_PAS_RearCenter ((uint32_t) 0x0000002C)

//! @brief Type : int32_t
#define DP_ID_PAS_RearLeft ((uint32_t) 0x0000002B)

//! @brief Type : int32_t
#define DP_ID_PAS_RearRight ((uint32_t) 0x0000002D)

//! @brief Type : int32_t
#define DP_ID_PAS_message ((uint32_t) 0x00000027)

//! @brief Type : float
#define DP_ID_SAW_SAS_Angle ((uint32_t) 0x00000051)

//! @brief Type : int32_t
#define DP_ID_SCC3_CF_VSM_Warn ((uint32_t) 0x000000D0)

//! @brief Type : int32_t
#define DP_ID_SCC_ACCInfoDisplay ((uint32_t) 0x00000033)

//! @brief Type : bool
#define DP_ID_SCC_BrakeLight ((uint32_t) 0x0000003A)

//! @brief Type : int32_t
#define DP_ID_SCC_DriverAlertDisplay ((uint32_t) 0x00000030)

//! @brief Type : bool
#define DP_ID_SCC_MainMode ((uint32_t) 0x0000002F)

//! @brief Type : int32_t
#define DP_ID_SCC_Mode ((uint32_t) 0x0000002E)

//! @brief Type : bool
#define DP_ID_SCC_ObjValid ((uint32_t) 0x00000032)

//! @brief Type : bool
#define DP_ID_SCC_TakeOverReq ((uint32_t) 0x00000031)

//! @brief Type : int32_t
#define DP_ID_SCC_TauGapSet ((uint32_t) 0x00000034)

//! @brief Type : int32_t
#define DP_ID_SCC_VSetDis ((uint32_t) 0x00000035)

//! @brief Type : bool
#define DP_ID_SL_Mode ((uint32_t) 0x0000004F)

//! @brief Type : int32_t
#define DP_ID_SL_Value ((uint32_t) 0x00000050)

//! @brief Type : bool
#define DP_ID_SMK1_BatDchWarn ((uint32_t) 0x00000110)

//! @brief Type : int32_t
#define DP_ID_SMK1_CF_Smk_PassLk ((uint32_t) 0x000000BF)

//! @brief Type : bool
#define DP_ID_SMK1_DactWarn ((uint32_t) 0x00000111)

//! @brief Type : bool
#define DP_ID_SMK1_EsclFailWarn ((uint32_t) 0x0000010E)

//! @brief Type : bool
#define DP_ID_SMK1_EsclNotLkWarn ((uint32_t) 0x0000010B)

//! @brief Type : bool
#define DP_ID_SMK1_EsclNotUkWarn ((uint32_t) 0x0000010D)

//! @brief Type : bool
#define DP_ID_SMK1_IdOutWarn ((uint32_t) 0x00000112)

//! @brief Type : bool
#define DP_ID_SMK1_NotPWarnStat ((uint32_t) 0x0000010C)

//! @brief Type : bool
#define DP_ID_SMK1_SsbWarn ((uint32_t) 0x0000010F)

//! @brief Type : int32_t
#define DP_ID_SMK_State ((uint32_t) 0x00000044)

//! @brief Type : int32_t
#define DP_ID_SPAS_CF_Spas_Disp ((uint32_t) 0x000000D4)

//! @brief Type : int32_t
#define DP_ID_SPAS_CF_spas_HMI_Stat ((uint32_t) 0x000000D5)

//! @brief Type : bool
#define DP_ID_SYS_CrankingState ((uint32_t) 0x00000008)

//! @brief Type : bool
#define DP_ID_SYS_DayNightState ((uint32_t) 0x00000007)

//! @brief Type : bool
#define DP_ID_SYS_EngineRunState ((uint32_t) 0x00000006)

//! @brief Type : int32_t
#define DP_ID_SYS_PowerState ((uint32_t) 0x00000004)

//! @brief Type : bool
#define DP_ID_SYS_VehicleRunState ((uint32_t) 0x00000005)

//! @brief Type : int32_t
#define DP_ID_TBT_Azimuth ((uint32_t) 0x00000066)

//! @brief Type : int32_t
#define DP_ID_TBT_Bar_Graph_Level ((uint32_t) 0x00000065)

//! @brief Type : int32_t
#define DP_ID_TBT_Charge ((uint32_t) 0x000000B3)

//! @brief Type : int32_t
#define DP_ID_TBT_Charge_Unit ((uint32_t) 0x000000B2)

//! @brief Type : int32_t
#define DP_ID_TBT_Combined_Side_Street ((uint32_t) 0x00000062)

//! @brief Type : bool
#define DP_ID_TBT_Combined_Side_Street_bit0 ((uint32_t) 0x00000143)

//! @brief Type : bool
#define DP_ID_TBT_Combined_Side_Street_bit1 ((uint32_t) 0x00000144)

//! @brief Type : bool
#define DP_ID_TBT_Combined_Side_Street_bit2 ((uint32_t) 0x00000145)

//! @brief Type : bool
#define DP_ID_TBT_Combined_Side_Street_bit3 ((uint32_t) 0x00000146)

//! @brief Type : int32_t
#define DP_ID_TBT_Direction ((uint32_t) 0x00000060)

//! @brief Type : int32_t
#define DP_ID_TBT_Display_TG ((uint32_t) 0x000000B4)

//! @brief Type : int32_t
#define DP_ID_TBT_Display_Type ((uint32_t) 0x0000005E)

//! @brief Type : int32_t
#define DP_ID_TBT_Distance_Turn_Point ((uint32_t) 0x00000061)

//! @brief Type : int32_t
#define DP_ID_TBT_DistancetoTurnPoint ((uint32_t) 0x00000064)

//! @brief Type : int32_t
#define DP_ID_TBT_EstDist_F ((uint32_t) 0x00000067)

//! @brief Type : int32_t
#define DP_ID_TBT_EstDist_I ((uint32_t) 0x00000068)

//! @brief Type : int32_t
#define DP_ID_TBT_EstDist_Unit ((uint32_t) 0x00000069)

//! @brief Type : int32_t
#define DP_ID_TBT_EstHour ((uint32_t) 0x0000006A)

//! @brief Type : int32_t
#define DP_ID_TBT_EstMin ((uint32_t) 0x0000006B)

//! @brief Type : int32_t
#define DP_ID_TBT_SD_BusOnlyCam ((uint32_t) 0x000000A0)

//! @brief Type : int32_t
#define DP_ID_TBT_SD_CutInCam ((uint32_t) 0x000000A1)

//! @brief Type : int32_t
#define DP_ID_TBT_SD_EtcCam ((uint32_t) 0x000000A7)

//! @brief Type : int32_t
#define DP_ID_TBT_SD_FixedCam ((uint32_t) 0x00000099)

//! @brief Type : int32_t
#define DP_ID_TBT_SD_MobileCam ((uint32_t) 0x0000009B)

//! @brief Type : int32_t
#define DP_ID_TBT_SD_OverLoadCam ((uint32_t) 0x000000A3)

//! @brief Type : int32_t
#define DP_ID_TBT_SD_ParkCam ((uint32_t) 0x000000A2)

//! @brief Type : int32_t
#define DP_ID_TBT_SD_PlateRcgCam ((uint32_t) 0x000000A4)

//! @brief Type : int32_t
#define DP_ID_TBT_SD_ShoulderCam ((uint32_t) 0x000000A6)

//! @brief Type : int32_t
#define DP_ID_TBT_SD_SignCam ((uint32_t) 0x0000009F)

//! @brief Type : int32_t
#define DP_ID_TBT_SD_SignOverCam ((uint32_t) 0x0000009D)

//! @brief Type : int32_t
#define DP_ID_TBT_SD_SpdLimit1 ((uint32_t) 0x0000009A)

//! @brief Type : int32_t
#define DP_ID_TBT_SD_SpdLimit2 ((uint32_t) 0x0000009C)

//! @brief Type : int32_t
#define DP_ID_TBT_SD_SpdLimit3 ((uint32_t) 0x0000009E)

//! @brief Type : int32_t
#define DP_ID_TBT_SD_TrafficCam ((uint32_t) 0x000000A5)

//! @brief Type : int32_t
#define DP_ID_TBT_Scale ((uint32_t) 0x00000063)

//! @brief Type : int32_t
#define DP_ID_TBT_Side_Street ((uint32_t) 0x0000005F)

//! @brief Type : bool
#define DP_ID_TBT_Side_Street_bit00 ((uint32_t) 0x00000133)

//! @brief Type : bool
#define DP_ID_TBT_Side_Street_bit01 ((uint32_t) 0x00000134)

//! @brief Type : bool
#define DP_ID_TBT_Side_Street_bit02 ((uint32_t) 0x00000135)

//! @brief Type : bool
#define DP_ID_TBT_Side_Street_bit03 ((uint32_t) 0x00000136)

//! @brief Type : bool
#define DP_ID_TBT_Side_Street_bit04 ((uint32_t) 0x00000137)

//! @brief Type : bool
#define DP_ID_TBT_Side_Street_bit05 ((uint32_t) 0x00000138)

//! @brief Type : bool
#define DP_ID_TBT_Side_Street_bit06 ((uint32_t) 0x00000139)

//! @brief Type : bool
#define DP_ID_TBT_Side_Street_bit07 ((uint32_t) 0x0000013A)

//! @brief Type : bool
#define DP_ID_TBT_Side_Street_bit08 ((uint32_t) 0x0000013B)

//! @brief Type : bool
#define DP_ID_TBT_Side_Street_bit09 ((uint32_t) 0x0000013C)

//! @brief Type : bool
#define DP_ID_TBT_Side_Street_bit10 ((uint32_t) 0x0000013D)

//! @brief Type : bool
#define DP_ID_TBT_Side_Street_bit11 ((uint32_t) 0x0000013E)

//! @brief Type : bool
#define DP_ID_TBT_Side_Street_bit12 ((uint32_t) 0x0000013F)

//! @brief Type : bool
#define DP_ID_TBT_Side_Street_bit13 ((uint32_t) 0x00000140)

//! @brief Type : bool
#define DP_ID_TBT_Side_Street_bit14 ((uint32_t) 0x00000141)

//! @brief Type : bool
#define DP_ID_TBT_Side_Street_bit15 ((uint32_t) 0x00000142)

//! @brief Type : int32_t
#define DP_ID_TBT_TIME_TYPE ((uint32_t) 0x0000006C)

//! @brief Type : int32_t
#define DP_ID_TCU1_G_SEL_DISP ((uint32_t) 0x000000C5)

//! @brief Type : int32_t
#define DP_ID_TCU3_CF_Tcu_TarGr ((uint32_t) 0x000000C6)

//! @brief Type : bool
#define DP_ID_TCU4_CF_TCU_WarnImg ((uint32_t) 0x000000C8)

//! @brief Type : int32_t
#define DP_ID_TCU4_CF_TCU_WarnMsg ((uint32_t) 0x000000C7)

//! @brief Type : bool
#define DP_ID_TCU4_CF_TCU_WarnSnd ((uint32_t) 0x000000C9)

//! @brief Type : bool
#define DP_ID_TPMS_FrontLeft ((uint32_t) 0x00000026)

//! @brief Type : bool
#define DP_ID_TPMS_FrontRight ((uint32_t) 0x00000025)

//! @brief Type : bool
#define DP_ID_TPMS_RearLeft ((uint32_t) 0x00000024)

//! @brief Type : bool
#define DP_ID_TPMS_RearRight ((uint32_t) 0x00000023)

//! @brief Type : int32_t
#define DP_ID_TPMS_W_LAMP ((uint32_t) 0x00000108)

//! @brief Type : int32_t
#define DP_ID_TREAD_W_LAMP ((uint32_t) 0x00000109)

//! @brief Type : float
#define DP_ID_USER1_AFC ((uint32_t) 0x00000016)

//! @brief Type : int32_t
#define DP_ID_USER1_AFC_Valid ((uint32_t) 0x0000001A)

//! @brief Type : int32_t
#define DP_ID_USER1_AVS ((uint32_t) 0x00000017)

//! @brief Type : int32_t
#define DP_ID_USER1_AVS_Valid ((uint32_t) 0x0000001B)

//! @brief Type : int32_t
#define DP_ID_USER1_DT_HR ((uint32_t) 0x00000018)

//! @brief Type : int32_t
#define DP_ID_USER1_DT_MIN ((uint32_t) 0x00000019)

//! @brief Type : float
#define DP_ID_USER1_Trip ((uint32_t) 0x00000015)

//! @brief Type : float
#define DP_ID_USER2_AFC ((uint32_t) 0x0000001D)

//! @brief Type : int32_t
#define DP_ID_USER2_AFC_Valid ((uint32_t) 0x00000021)

//! @brief Type : int32_t
#define DP_ID_USER2_AVS ((uint32_t) 0x0000001E)

//! @brief Type : int32_t
#define DP_ID_USER2_AVS_Valid ((uint32_t) 0x00000022)

//! @brief Type : int32_t
#define DP_ID_USER2_DT_HR ((uint32_t) 0x0000001F)

//! @brief Type : int32_t
#define DP_ID_USER2_DT_MIN ((uint32_t) 0x00000020)

//! @brief Type : float
#define DP_ID_USER2_Trip ((uint32_t) 0x0000001C)

//! @brief Type : int32_t
#define DP_ID_USM_RemainingDays ((uint32_t) 0x0000006E)

//! @brief Type : int32_t
#define DP_ID_USM_RemainingMonths ((uint32_t) 0x0000006F)

//! @brief Type : bool
#define DP_ID_USM_ServceRemind_Reset ((uint32_t) 0x0000011E)

//! @brief Type : bool
#define DP_ID_USM_ServceRemind_Unit ((uint32_t) 0x0000011D)

//! @brief Type : int32_t
#define DP_ID_USM_ServiceMileage ((uint32_t) 0x0000006D)

//! @brief Type : int32_t
#define DP_ID_USM_ServiceRemind_Distance ((uint32_t) 0x0000011B)

//! @brief Type : int32_t
#define DP_ID_USM_ServiceRemind_Month ((uint32_t) 0x0000011C)

//! @brief Type : bool
#define DP_ID_WARN_ABS ((uint32_t) 0x0000008A)

//! @brief Type : int32_t
#define DP_ID_WARN_AutoHold ((uint32_t) 0x00000095)

//! @brief Type : bool
#define DP_ID_WARN_Battery ((uint32_t) 0x00000103)

//! @brief Type : bool
#define DP_ID_WARN_CheckEngine ((uint32_t) 0x00000093)

//! @brief Type : bool
#define DP_ID_WARN_DISABLEALL ((uint32_t) 0x00000098)

//! @brief Type : int32_t
#define DP_ID_WARN_DrvSeatBeltInd ((uint32_t) 0x000000FB)

//! @brief Type : bool
#define DP_ID_WARN_ECS ((uint32_t) 0x0000008B)

//! @brief Type : bool
#define DP_ID_WARN_ENGINE_Oil ((uint32_t) 0x00000102)

//! @brief Type : int32_t
#define DP_ID_WARN_EPB ((uint32_t) 0x0000008C)

//! @brief Type : int32_t
#define DP_ID_WARN_ESC ((uint32_t) 0x00000096)

//! @brief Type : bool
#define DP_ID_WARN_ESP ((uint32_t) 0x00000101)

//! @brief Type : bool
#define DP_ID_WARN_Filter ((uint32_t) 0x00000104)

//! @brief Type : bool
#define DP_ID_WARN_FrontFog ((uint32_t) 0x00000084)

//! @brief Type : bool
#define DP_ID_WARN_HEATED_STEER ((uint32_t) 0x000000FA)

//! @brief Type : bool
#define DP_ID_WARN_HLpLowNotWarn ((uint32_t) 0x000000FD)

//! @brief Type : bool
#define DP_ID_WARN_HighBeam ((uint32_t) 0x00000081)

//! @brief Type : bool
#define DP_ID_WARN_HighBeamAssist ((uint32_t) 0x00000090)

//! @brief Type : bool
#define DP_ID_WARN_HighTemp ((uint32_t) 0x00000088)

//! @brief Type : bool
#define DP_ID_WARN_IceWarning ((uint32_t) 0x00000100)

//! @brief Type : int32_t
#define DP_ID_WARN_Immobilizer ((uint32_t) 0x00000089)

//! @brief Type : int32_t
#define DP_ID_WARN_LDWS_SCC ((uint32_t) 0x00000097)

//! @brief Type : bool
#define DP_ID_WARN_LowFuel ((uint32_t) 0x00000091)

//! @brief Type : bool
#define DP_ID_WARN_LowWasher ((uint32_t) 0x00000094)

//! @brief Type : int32_t
#define DP_ID_WARN_MDPS_WLmp ((uint32_t) 0x000000FC)

//! @brief Type : bool
#define DP_ID_WARN_ParkingBrakeWarning ((uint32_t) 0x000000FF)

//! @brief Type : bool
#define DP_ID_WARN_RearFog ((uint32_t) 0x00000092)

//! @brief Type : bool
#define DP_ID_WARN_SCC ((uint32_t) 0x00000085)

//! @brief Type : bool
#define DP_ID_WARN_SET ((uint32_t) 0x00000086)

//! @brief Type : int32_t
#define DP_ID_WARN_SWI_Ind ((uint32_t) 0x000000FE)

//! @brief Type : bool
#define DP_ID_WARN_Tail ((uint32_t) 0x00000087)

//! @brief Type : bool
#define DP_ID_WARN_TrunkOpen ((uint32_t) 0x0000008D)

//! @brief Type : bool
#define DP_ID_WARN_TurnLeft ((uint32_t) 0x00000082)

//! @brief Type : bool
#define DP_ID_WARN_TurnRight ((uint32_t) 0x00000083)

//! @brief Type : bool
#define DP_ID_WARN_V120kmh ((uint32_t) 0x0000008F)

//! @brief Type : bool
#define DP_ID_WARN_VSMOff ((uint32_t) 0x0000008E)

//! @brief Type : bool
#define DP_ID_WHEEL_SW_DOWN ((uint32_t) 0x00000128)

//! @brief Type : bool
#define DP_ID_WHEEL_SW_Driving ((uint32_t) 0x0000012C)

//! @brief Type : bool
#define DP_ID_WHEEL_SW_ECO ((uint32_t) 0x0000012B)

//! @brief Type : bool
#define DP_ID_WHEEL_SW_LEFT ((uint32_t) 0x00000129)

//! @brief Type : bool
#define DP_ID_WHEEL_SW_LONG ((uint32_t) 0x00000125)

//! @brief Type : bool
#define DP_ID_WHEEL_SW_OK ((uint32_t) 0x00000126)

//! @brief Type : bool
#define DP_ID_WHEEL_SW_RIGHT ((uint32_t) 0x0000012A)

//! @brief Type : bool
#define DP_ID_WHEEL_SW_UP ((uint32_t) 0x00000127)

#endif //GTF_INCLUDED_DATAPOOL_IDENTIFIERS_OF_CONTEXT_1
