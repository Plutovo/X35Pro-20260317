#ifndef _HL7_CFG_H
#define _HL7_CFG_H
#include "HL7_inc_ext.h"


#define _HL7_ANES   0
#define _HL7_VENT   1
#define _HL_TYPE    _HL7_ANES

#define _HL7_PARA       //监测参数
#define _HL7_CTRLPARA   //控制参数
#define _HL7_ALRM       //报警
#define _HL7_ALRMLIM    //报警限制
#define _HL7_MESG       //文本消息

//单包大小
#define HL7_FRM_MAX_SIZE        150
//
#define HL7_SYNC_FLAG           0x0B
#define HL7_EOP_FLAG1           0x1C
#define HL7_EOP_FLAG2           0x0D



//Segment
#define HL7_SEG_MSH             "MSH"
#define HL7_SEG_PID             "PID"
#define HL7_SEG_OBR             "OBR"
#define HL7_SEG_OBX             "OBX"


//Escape character
#define HL7_ESC_SEG             EOP_FLAG2
#define HL7_ESC_FLD	            '|'
#define HL7_ESC_FLDREP	        '~'
#define HL7_ESC_CMPT            '^'
#define HL7_ESC_SUBCMPT         '&'
#define HL7_ESC_SLASH           '\\'
#define HL7_ESC                 "^~\\&"


#if (LOGO_FLAG == MSW_LOGO)
    #define HL7_SEND_APP            "MSW"
#elif (LOGO_FLAG == PENLON_LOGO) 
    #define HL7_SEND_APP            "Penlon"
#else
    #define HL7_SEND_APP            "Siriusmedica"
#endif

#define HL7_SEND_FCLT           "ID"

#define HL7_MSG_TYPE_ORU        "ORU^R01"


#define HL7_MSG_ID_Para         "2"
#define HL7_MSG_ID_CtrlID       "3"
#define HL7_MSG_ID_Alrm         "4"
#define HL7_MSG_ID_AlrmLim      "5"
#define HL7_MSG_ID_Mesg         "6"


#define HL7_PID_P               "P"
#define HL7_VER                 "2.3.1"
#define HL7_SET_UTF8            "UNICODE UTF-8"


#define HL7_Alrm_High           "1"
#define HL7_Alrm_Medium         "2"
#define HL7_Alrm_Low            "3"
#define HL7_Alrm_Msg            "4"

#define HL7_VAL_ST              "ST"
#define HL7_VAL_TX              "TX"
#define HL7_VAL_FT              "FT"
#define HL7_VAL_NM              "NM"
#define HL7_VAL_CE              "CE"

#define HL7_ANAES               "Anesthesia"
#define HL7_VENT                "Ventilator"

#define HL7_RES_STATE_F         "F"

#define HL7_Alrm_phy            "PHY_ALM"
#define HL7_Alrm_tech           "TECH_ALM"


//monitor para
//Anesthesia
#define HL7_PARA_PEEP           "1"
#define HL7_PARA_PPEAK          "2"
#define HL7_PARA_PPLAT          "3"
#define HL7_PARA_PMEAN          "4"
#define HL7_PARA_VTE            "5"
#define HL7_PARA_VTI            "6"
#define HL7_PARA_MV             "7"
#define HL7_PARA_MVSPN          "8"
#define HL7_PARA_F              "9"
#define HL7_PARA_FSPN           "10"
#define HL7_PARA_IEI            "11"
#define HL7_PARA_COMPL          "12"
#define HL7_PARA_RST            "13"
#define HL7_PARA_PMIN           "14"
#define HL7_PARA_ETCO2          "15"
#define HL7_PARA_FICO2          "16"
#define HL7_PARA_FIO2           "17"
#define HL7_PARA_ETO2           "18"
#define HL7_PARA_FIN2O          "19"
#define HL7_PARA_ETN2O          "20"
#define HL7_PARA_FIAA           "21"
#define HL7_PARA_ETAA           "22"
#define HL7_PARA_FIAA2          "23"
#define HL7_PARA_ETAA2          "24"
#define HL7_PARA_MAC            "25"
#define HL7_PARA_FLOWN2O        "26"
#define HL7_PARA_FLOWAIR        "27"
#define HL7_PARA_FLOWO2         "28"
#define HL7_PARA_SPO2           "29"
#define HL7_PARA_PR             "30"
#define HL7_PARA_PI             "31"
#define HL7_PARA_IEE            "32"


//Ventilator
#define HL7_VENTPARA_VTI        "201"
#define HL7_VENTPARA_VTE        "202"
#define HL7_VENTPARA_MV         "203"
#define HL7_VENTPARA_MVSPN      "204"
#define HL7_VENTPARA_F          "205"
#define HL7_VENTPARA_FSPN       "206"
#define HL7_VENTPARA_PPEAK      "207"
#define HL7_VENTPARA_PMEAN      "208"
#define HL7_VENTPARA_PPLAT      "209"
#define HL7_VENTPARA_PEEP       "210"
#define HL7_VENTPARA_PMIN       "211"
#define HL7_VENTPARA_IEI        "212"
#define HL7_VENTPARA_IEE        "213"
#define HL7_VENTPARA_RST        "214"
#define HL7_VENTPARA_CDYN       "215"
#define HL7_VENTPARA_RSBI       "216"
#define HL7_VENTPARA_FIO2       "217"
#define HL7_VENTPARA_ETCO2      "218"
#define HL7_VENTPARA_FICO2      "219"
#define HL7_VENTPARA_SPO2       "220"
#define HL7_VENTPARA_PULSE      "221"
#define HL7_VENTPARA_PI         "222"
#define HL7_VENTPARA_PEEPI      "223"
#define HL7_VENTPARA_CST        "224"

//ctrl para
//Anesthesia
#define HL7_SET_PEEP            "500"
#define HL7_SET_VT              "501"
#define HL7_SET_F               "502"
#define HL7_SET_IEI             "503"
#define HL7_SET_TSLOPE          "504"
#define HL7_SET_TINSP           "505"
#define HL7_SET_PINSP           "506"
#define HL7_SET_PUSPP           "507"
#define HL7_SET_TPAUSE          "508"
#define HL7_SET_ESENS           "509"
#define HL7_SET_PSENS           "510"
#define HL7_SET_FSENS           "511"
#define HL7_SET_FG              "512"
#define HL7_SET_O2Per           "513"
#define HL7_SET_IEE             "514"
#define HL7_SET_SIGH            "515"

//Ventilator
#define HL7_VENTSET_SIGH        "600"
#define HL7_VENTSET_VT          "601"
#define HL7_VENTSET_IEI         "602"
#define HL7_VENTSET_IEE         "603"
#define HL7_VENTSET_F           "604"
#define HL7_VENTSET_PEEP        "605"
#define HL7_VENTSET_FIO2        "606"
#define HL7_VENTSET_PSENS       "607"
#define HL7_VENTSET_PAUSE       "608"
#define HL7_VENTSET_PINSP       "609"
#define HL7_VENTSET_TSLOPE      "610"
#define HL7_VENTSET_PHIGH       "611"
#define HL7_VENTSET_THIGH       "612"
#define HL7_VENTSET_PLOW        "613"
#define HL7_VENTSET_ESENS       "614"
#define HL7_VENTSET_TLOW        "615"
#define HL7_VENTSET_PSUPP       "616"
#define HL7_VENTSET_TINSP       "617"
#define HL7_VENTSET_FSENS       "618"

//alarm
//Anesthesia
//high
#define HL7_ALRM_O2DOWN         "1000"
#define HL7_ALRM_EGMCOMM        "1001"
#define HL7_ALRM_BDUCOMM        "1002"
#define HL7_ALRM_BATFAIL        "1003"
#define HL7_ALRM_BATDIS         "1004"
#define HL7_ALRM_APNEA          "1005"
#define HL7_ALRM_CONTPREH       "1006"
#define HL7_ALRM_PREH           "1007"
#define HL7_ALRM_O2FAIL         "1008"
#define HL7_ALRM_EGMFAIL        "1009"
#define HL7_ALRM_SWERR          "1010"
#define HL7_ALRM_HWERR          "1011"
#define HL7_ALRM_MOTOR          "1012"
#define HL7_ALRM_CALLOST        "1013"
#define HL7_ALRM_REPADP         "1014"
#define HL7_ALRM_NOADP          "1015"
#define HL7_ALRM_NOLINE         "1016"
#define HL7_ALRM_CO2ACC         "1017"
#define HL7_ALRM_N2OACC         "1018"
#define HL7_ALRM_AAACC          "1019"
#define HL7_ALRM_O2ACC          "1020"
#define HL7_ALRM_TEMPOUT        "1021"
#define HL7_ALRM_PREOUT         "1022"
#define HL7_ALRM_AAUNREL        "1023"
#define HL7_ALRM_O2REQ          "1024"
#define HL7_ALRM_FIAAH          "1025"
#define HL7_ALRM_FTAAH          "1026"
#define HL7_ALRM_MVL            "1027"
#define HL7_ALRM_MVH            "1028"
#define HL7_ALRM_PRE10          "1029"
#define HL7_ALRM_RATEL          "1030"
#define HL7_ALRM_FIO2H          "1031"
#define HL7_ALRM_FIO2L          "1032"
#define HL7_ALRM_ECO2H          "1033"
#define HL7_ALRM_FCO2H          "1034"
#define HL7_ALRM_FN2OH          "1035"
#define HL7_ALRM_STDY           "1036"
#define HL7_ALRM_PREL           "1037"
#define HL7_ALRM_BATLOW         "1038"
#define HL7_ALRM_SPO2FAIL       "1039"
#define HL7_ALRM_PIL            "1040"
#define HL7_ALRM_SPO2L          "1041"
#define HL7_ALRM_PRL            "1042"
#define HL7_ALRM_VTEH           "1043"
#define HL7_ALRM_AAFULL         "1044"
#define HL7_ALRM_CHKLINE        "1045"
#define HL7_ALRM_PWRFAIL        "1046"
#define HL7_ALRM_CO2FAIL        "1047"
#define HL7_ALRM_AAFAIL         "1048"

//medium && low
#define HL7_ALRM_AIRDOWN        "1049"
#define HL7_ALRM_N2ODOWN        "1050"
#define HL7_ALRM_NOABS          "1051"
#define HL7_ALRM_RPREL          "1052"
#define HL7_ALRM_VTEL           "1053"
#define HL7_ALRM_RATEH          "1054"
#define HL7_ALRM_PRH            "1055"
#define HL7_ALRM_OCCLU          "1056"
#define HL7_ALRM_AAWFULL        "1057"
#define HL7_ALRM_PIH            "1058"
#define HL7_ALRM_SPO2H          "1059"
#define HL7_ALRM_ETCO2L         "1060"
#define HL7_ALRM_FIAAL          "1061"
#define HL7_ALRM_ETAAL          "1062"
#define HL7_ALRM_FN2OL          "1063"
#define HL7_ALRM_MACM3          "1064"
#define HL7_ALRM_GASZERO        "1065"
#define HL7_ALRM_AAOVERF        "1066"
#define HL7_ALRM_REPSPO2SEN     "1067"
#define HL7_ALRM_REPSPO2CBE     "1068"
#define HL7_ALRM_REPSPO2ADH     "1069"
#define HL7_ALRM_BATFAILL       "1070"
#define HL7_ALRM_MACL3          "1071"
#define HL7_ALRM_GASCOMM        "1072"
#define HL7_ALRM_SPO2COMM       "1073"
#define HL7_ALRM_ACFAIL         "1074"
#define HL7_ALRM_NOSPO2CBE      "1075"
#define HL7_ALRM_SPO2NPat       "1076"
#define HL7_ALRM_AAOFF          "1077"
#define HL7_ALRM_CO2OFF         "1078"
#define HL7_ALRM_SPO2OFF        "1079"
#define HL7_ALRM_O2OFF          "1080"
#define HL7_ALRM_LOWIQ          "1081"
#define HL7_ALRM_2LIGHT         "1082"
#define HL7_ALRM_NOSPO2SEN      "1083"
#define HL7_ALRM_NOSPO2ADH      "1084"
#define HL7_ALRM_INCOMSEN       "1085"
#define HL7_ALRM_INCOMCBE       "1086"
#define HL7_ALRM_INCOMADH       "1084"
#define HL7_ALRM_CHKCBE         "1088"
#define HL7_ALRM_CHKSEN         "1089"
#define HL7_ALRM_CALO2          "1090"

//Ventilator
#define HL7_VENTALRM_NGAS	    "1251"
#define HL7_VENTALRM_BATFAIL	"1252"
#define HL7_VENTALRM_APNEA	    "1253"
#define HL7_VENTALRM_CONTPREH	"1254"
#define HL7_VENTALRM_PREH	    "1255"
#define HL7_VENTALRM_PEEPH	    "1256"
#define HL7_VENTALRM_PLOWH	    "1257"
#define HL7_VENTALRM_DISPA	    "1258"
#define HL7_VENTALRM_MVL	    "1259"
#define HL7_VENTALRM_MVH	    "1260"
#define HL7_VENTALRM_RATEL	    "1261"
#define HL7_VENTALRM_O2FAIL	    "1262"
#define HL7_VENTALRM_FIO2H	    "1263"
#define HL7_VENTALRM_FIO2L	    "1264"
#define HL7_VENTALRM_ETCO2H	    "1265"
#define HL7_VENTALRM_STDBY	    "1266"
#define HL7_VENTALRM_PREL	    "1267"
#define HL7_VENTALRM_BATL	    "1268"
#define HL7_VENTALRM_SPO2FAIL	"1269"
#define HL7_VENTALRM_PULSEL	    "1270"
#define HL7_VENTALRM_DEFSEN	    "1271"
#define HL7_VENTALRM_UNREGSEN	"1272"
#define HL7_VENTALRM_IQL	    "1273"
#define HL7_VENTALRM_PIL	    "1274"
#define HL7_VENTALRM_SPO2L	    "1275"
#define HL7_VENTALRM_PRL	    "1276"
#define HL7_VENTALRM_VTEH	    "1277"
#define HL7_VENTALRM_O2SL	    "1278"
#define HL7_VENTALRM_AIRSL	    "1279"
#define HL7_VENTALRM_PEEPL	    "1280"
#define HL7_VENTALRM_PLOWL	    "1281"
#define HL7_VENTALRM_PRVCVTL	"1282"
#define HL7_VENTALRM_LKGEOVER	"1283"
#define HL7_VENTALRM_VTEL	    "1284"
#define HL7_VENTALRM_RATEH	    "1285"
#define HL7_VENTALRM_PRATEH	    "1286"
#define HL7_VENTALRM_MINTER	    "1287"
#define HL7_VENTALRM_MLIGHT	    "1288"
#define HL7_VENTALRM_NOCBLE	    "1289"
#define HL7_VENTALRM_LINEOCC	"1290"
#define HL7_VENTALRM_PIH	    "1291"
#define HL7_VENTALRM_SPO2H	    "1292"
#define HL7_VENTALRM_CO2L	    "1293"
#define HL7_VENTALRM_NEBUON	    "1294"
#define HL7_VENTALRM_ACFAIL	    "1295"
#define HL7_VENTALRM_SPO2DIS	"1296"
#define HL7_VENTALRM_NOPATI	    "1297"
#define HL7_VENTALRM_CO2OFF	    "1298"
#define HL7_VENTALRM_SPO2OFF	"1299"
#define HL7_VENTALRM_O2OFF	    "1300"



//text mesg
//Anesthesia
#define HL7_MSG_STDBY           "1500"
#define HL7_MSG_WORK            "1501"
#define HL7_MSG_VCV             "1502"
#define HL7_MSG_SIMVV           "1503"
#define HL7_MSG_PCV             "1504"
#define HL7_MSG_SIMVP           "1505"
#define HL7_MSG_PRVC            "1506"
#define HL7_MSG_SIMVPRVC        "1507"
#define HL7_MSG_PSV             "1508"
#define HL7_MSG_SIGH            "1509"
#define HL7_MSG_MANUAL          "1510"
#define HL7_MSG_ACGO            "1511"
#define HL7_MSG_BACKUP          "1512"

//Ventilator
#define HL7_VENTMSG_STDBY       "1750"
#define HL7_VENTMSG_WORK        "1751"
#define HL7_VENTMSG_VCV         "1752"
#define HL7_VENTMSG_PCV         "1753"
#define HL7_VENTMSG_PRVC        "1754"
#define HL7_VENTMSG_DUOP        "1755"
#define HL7_VENTMSG_APRV        "1756"
#define HL7_VENTMSG_SIMVV       "1757"
#define HL7_VENTMSG_SIMVP       "1758"
#define HL7_VENTMSG_SIMVPR      "1759"
#define HL7_VENTMSG_CPAP        "1760"
#define HL7_VENTMSG_VS          "1761"
#define HL7_VENTMSG_NIVPC       "1762"
#define HL7_VENTMSG_NIVCPAP     "1763"
#define HL7_VENTMSG_BKP         "1764"


//Para Attribute Part
//Anesthesia
#define HL7_ALRM_UP             "2000"
#define HL7_ALRM_LOW            "2001"
//Ventilator
#define HL7_VENTALRM_UP         "2050"
#define HL7_VENTALRM_LOW        "2051"

//model ID
#define HL7_MOD_MIRMAAX         "2100"
#define HL7_MOD_MIRMACO2        "2101"
#define HL7_MOD_MISAOR          "2102"
#define HL7_MOD_MISAAX          "2103"
#define HL7_MOD_MISACO2         "2104"
#define HL7_MOD_DAAO2           "2105"
#define HL7_MOD_DAA             "2106"
#define HL7_MOD_STCO2           "2107"
#define HL7_MOD_NMEDCO2         "2108"
#define HL7_MOD_MSPO2           "2109"
#define HL7_MOD_CNSpO2          "2110"
#define HL7_MOD_ANES            "2111"
#define HL7_MOD_VENT            "2112"

#endif




