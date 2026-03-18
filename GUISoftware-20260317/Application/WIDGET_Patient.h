#ifndef _WIDGET_PATIENT_H
#define _WIDGET_PATIENT_H
//#include "stdtypes.h"
#include "GUI.h"
#include "WM.h"
//#include "Config.h"
#include "MainTask.h"

// typedef unsigned char u8;
#ifndef u8
#define u8 unsigned char
#endif

#define PID_SIZE	32
#define PN_SIZE		32
#define PID_MARK	'\''
#define EDIT_PID_SIZE	(PID_SIZE-2)
#define EDIT_PN_SIZE	(PN_SIZE-2)

#define IBW_ADULT_MALE         71
#define IBW_ADULT_WEMALE       52
#define STATURE_ADULT_MALE     175
#define STATURE_ADULT_WEMALE   160
#define AGE_ADULT				32

#define IBW_PEAD               17
#define STATURE_PEAD_MALE      100
#define STATURE_PEAD_WEMALE    100

#ifdef LANGUAGE_CHINESE_FLAG
#define AGE_PEAD				9
#define IBW_INFANT				5
#define STATURE_INFANT			70
#define AGE_INFANT				3
#else
#define AGE_PEAD				8
#define IBW_INFANT				5
#define STATURE_INFANT			57
#define AGE_INFANT				6
#endif


typedef enum
{
	PATIENTTYPE_ADULT = 0,
	PATIENTTYPE_PAEDIATRIC,
	PATIENTTYPE_INFANT,
}PType_t;

typedef enum
{
	GENDER_MALE = 0,
	GENDER_FEMALE,
}PGender_t;

typedef enum
{
	VENTTYPE_INV = 0,
	VENTTYPE_NIV,
}PVentType_t;

typedef enum
{
	DISP_CO2_VALUE = 0,
	DISP_BAROMETRIC_VALUE,
	DISP_GAS_SUPPLY_VALUE,
	DISP_HEIGHT_VALUE,
	DISP_WEIGHT_VALUE,
	DISP_AIRWAY_VALUE,
}DISP_PATIENT_t;

typedef struct 
{
	char		Patient_ID[PID_SIZE];	//病历号
	char		Patient_Name[PN_SIZE];	//病人姓名
	PType_t		Patient_Type;			//0:Adult成年人的    1:Paediatric儿童的   2：Infant婴儿的
	PGender_t	Patient_Gender;			//0:Male男性的 1:Female女性的
	PVentType_t	Patient_VentType;		//0:invasive插管  1：niv无创
	u8			Patient_Stature;		//身高
	u8			Patient_Age;			//年龄
	float		Patient_IBW;			//IBW值
}Patient_Info_t;

WM_HWIN Patient_InfoWinCreate(void);
void Patient_WriteInfo(void);

void Patient_ReadID(Patient_Info_t *info);
void Patient_WriteID(Patient_Info_t *info);
void Patient_ReadName(Patient_Info_t *info);
void Patient_WriteName(Patient_Info_t *info);

Patient_Info_t* Patient_GetInfo(void);
char* Patient_GetPID(void);
char* Patient_GetPName(void);
PType_t Patient_GetPType(void);
PGender_t Patient_GetPGender(void);
PVentType_t Patient_GetPVentType(void);
u8 Patient_GetStature(void);
u8 Patient_GetAge(void);
float Patient_GetIBW(void);

void Patient_SetPID(char *buf, char size);
void Patient_SetPName(char *buf, char size);
void Patient_SetPType(PType_t type);
void Patient_SetPGender(PGender_t type);
void Patient_SetPVentType(PVentType_t type);
void Patient_SetStature(u8 data);
void Patient_SetAge(u8 data);
void Patient_SetIBW(float data);
u8 Patient_GetStartChooseFlag(void);
int UnitConversion_Reverse(int UnitFlag,int UnitNum,int value);
int UnitConversion(int UnitFlag,int UnitNum,int value);

#endif
