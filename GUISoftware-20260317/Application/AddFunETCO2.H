#ifndef _ADDFUNETCO2_H
#define _ADDFUNETCO2_H

#define CO2_PUMPRUNHOUR    0
#define CO2_BTPSCOMPENSATE 1
#define CO2_O2COMPENSATE   2
#define CO2_PUMPOCCLUSION  3
#define CO2_MODULEONOFF    4
#define CO2_MODULEOFF      5
#define CO2_MODULEON       6
//
void BTPSCompensate();
void O2CO2Compensate();
void CheckCo2PumpRunTime();
void CheckCo2PumpOcclusion();
void AlarmCheckCo2PumpOcclusion(unsigned char Vent_Flag);
void Co2ModuleONOFF(unsigned int onoff);
//

#endif

