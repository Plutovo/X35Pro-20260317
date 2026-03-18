#ifndef FRAM_ADDR_SYS_CFG_H
#define FRAM_ADDR_SYS_CFG_H
#include "MainTask.h"


//工程定义 只允许打开一个
//#define __PROJECT_X55__
//#define __PROJECT_X50__
#define __PROJECT_X45__
//#define __PROJECT_X40__
//#define __PROJECT_X30__
//#define __PROJECT_R55_R35__
//#define __PROJECT_R50_R30__


/* Private macros ----------------------------------------------------------- */
#define STR1(R)                             #R  
#define STR2(R)                             STR1(R) 
        
/*
 * @Type 系统初始化标志     
 * format 固定为 0x11223344
 */      
#define SYSTEM_INIT_FLAG                    0x11223344
        
/*
 * @Type FRAM是否使用新地址     
 * @format 固定为 0x12345678
 */           
#define SYSTEM_NEWADDR_FLAG                 0x12345678 

   
/*
 * @Type 机器欧标/国标     
 * @Len  2
 * @format EN 1  CN 2
 */         
#ifndef LANGUAGE_CHINESE_FLAG
#define SYSTEM_STANDARD                     (u8)0x01    
#else
#define SYSTEM_STANDARD                     (u8)0x02        
#endif
     
       
//#define SYSTEM_DEVICE_TYPE                 'Anaes'    
//#define SYSTEM_DEVICE_TYPE                 'Vent'
//#define SYSTEM_DEVICE_TYPE                 'AnaesVent'     

/*
 * @Type 机器类型     
 * @Len  <=16
 * @format 'xxxxxx'     麻醉机/呼吸机/麻醉呼吸机
 */  

/*
 * @Type 屏幕型号      
 * @Len  <=16
 * @format 'xxxxxx'
 */ 


/*
 * @Type 机器型号      
 * @Len  <=32
 * @format 'xxxxxx'
 */   

#ifdef __PROJECT_X55__
    #define SYSTEM_SCREEN_MODEL                         '600x800'
    #define SYSTEM_DEVICE_TYPE                          'Anaes'
    #if (LOGO_FLAG == SIRIUSMED_LOGO) \
     || (LOGO_FLAG == NEW_SIRIUSMED_LOGO) \
     || (LOGO_FLAG == NEUTRAL_LOGO)
        #define SYSTEM_MODEL                            'Siriusmed X55'
    #elif (LOGO_FLAG == PENLON_LOGO_330E)   
        #define SYSTEM_MODEL                            'Penlon Prima330E'
    #elif (LOGO_FLAG == PENLON_LOGO_465)
        #define SYSTEM_MODEL                            'Penlon Prima465'
    #elif (LOGO_FLAG == PROMED_LOGO)
        #define SYSTEM_MODEL                            'ProMED X55'
    #elif (LOGO_FLAG == MSW_LOGO)
        #define SYSTEM_MODEL                            'MSW MAJA-X55'
    #else
        #define SYSTEM_MODEL                            'Siriusmed X55'
    #endif
#elif defined __PROJECT_X50__
    #define SYSTEM_SCREEN_MODEL                         '600x800'
    #define SYSTEM_DEVICE_TYPE                          'Anaes'            
    #if (LOGO_FLAG == SIRIUSMED_LOGO) \
     || (LOGO_FLAG == NEW_SIRIUSMED_LOGO) \
     || (LOGO_FLAG == NEUTRAL_LOGO)
        #define SYSTEM_MODEL                            'Siriusmed X55'
    #elif (LOGO_FLAG == ADAVENCED_LOGO)
        #define SYSTEM_MODEL                            'Advanced X50'
    #elif (LOGO_FLAG == BIONICA_LOGO)
        #define SYSTEM_MODEL                            'Bionica X50'
    #elif (LOGO_FLAG == PROMED_LOGO)
        #define SYSTEM_MODEL                            'ProMED X50'
    #elif (LOGO_FLAG == MSW_LOGO)
        #define SYSTEM_MODEL                            'MSW MAJA-X50'
    #else
        #define SYSTEM_MODEL                            'Siriusmed X50'         
    #endif         
#elif defined __PROJECT_X45__
    #define SYSTEM_SCREEN_MODEL                         '800x480'
    #if   (MACHINE_TYPE == X45)
        #define SYSTEM_DEVICE_TYPE                      'Anaes'
        #if (LOGO_FLAG == SIRIUSMED_LOGO) \
         || (LOGO_FLAG == NEW_SIRIUSMED_LOGO) \
		 || (LOGO_FLAG == ENDURE_LOGO)\
         || (LOGO_FLAG == NEUTRAL_LOGO)
            #define SYSTEM_MODEL                        'Siriusmed X45'
        #elif (LOGO_FLAG == ADVANCED_LOGO)
            #define SYSTEM_MODEL                        'Advanced X45'
        #elif (LOGO_FLAG == MSW_LOGO)
            #define SYSTEM_MODEL                        'MSW MAJA(X45)'
        #else
            #define SYSTEM_MODEL                        'Siriusmed X45'
        #endif
    #elif (MACHINE_TYPE == X45A)
        #define SYSTEM_DEVICE_TYPE                      'Anaes'        
        #define SYSTEM_MODEL                            'Siriusmed X45A'
    #elif (MACHINE_TYPE == X45C)
        #define SYSTEM_DEVICE_TYPE                      'Anaes'     
        #define SYSTEM_MODEL                            'Siriusmed X45C'
    #elif (MACHINE_TYPE == X6)
        #define SYSTEM_DEVICE_TYPE                      'AnaesVent' 
        #define SYSTEM_MODEL                            'Siriusmed X6'
    #elif (MACHINE_TYPE == Z6)
        #define SYSTEM_DEVICE_TYPE                      'AnaesVent' 
        #if (LOGO_FLAG == HOSPITECH_LOGO) 
             #define SYSTEM_MODEL                       'Hospitech IMH-Z6'
        #elif (LOGO_FLAG == GALILEA_LOGO) 
             #define SYSTEM_MODEL                       'Genuine Z6'
        #else
             #define SYSTEM_MODEL                       'Hospitech IMH-Z6'
        #endif
    #else
        #define SYSTEM_MODEL                            'Siriusmed X45'
    #endif       
#elif defined __PROJECT_X40__
    #define SYSTEM_SCREEN_MODEL                         '800x600'
    #if (MACHINE_TYPE == X40)
        #define SYSTEM_DEVICE_TYPE                      'Anaes'
        #if (LOGO_FLAG == SIRIUSMED_LOGO) \
         || (LOGO_FLAG == NEW_SIRIUSMED_LOGO) \
         || (LOGO_FLAG == NEUTRAL_LOGO)
             #define SYSTEM_MODEL                       'Siriusmed X40'
        #elif (LOGO_FLAG == PENLON_LOGO)
             #define SYSTEM_MODEL                       'Penlon Prima320'
        #elif (LOGO_FLAG == DRE_LOGO)
             #define SYSTEM_MODEL                       'DRE X40' 
        #elif (LOGO_FLAG == BIONICA_LOGO)
             #define SYSTEM_MODEL                       'Bionica X40' 
        #elif (LOGO_FLAG == MSW_LOGO)
             #define SYSTEM_MODEL                       'MSW MAJA-X40'
        #elif (LOGO_FLAG == FUTURE_LOGO)
             #define SYSTEM_MODEL                       'Future X40'
        #else
             #define SYSTEM_MODEL                       'Siriusmed X40'
        #endif
    #elif (MACHINE_TYPE == X40C)
        #define SYSTEM_DEVICE_TYPE                      'Anaes'             
        #if (LOGO_FLAG == SIRIUSMED_X40C_LOGO)
             #define SYSTEM_MODEL                       'Siriusmed X40C'
        #elif   (LOGO_FLAG == SASVAT_LOGO)
             #define SYSTEM_MODEL                       'Sasvat X40C'
        #elif (LOGO_FLAG == UnitedLux_LOGO)
             #define SYSTEM_MODEL                       'UnitedLux X40C'
        #elif (LOGO_FLAG == THORES_LOGO)
             #define SYSTEM_MODEL                       'Archmed X40C'
        #else
             #define SYSTEM_MODEL                       'Siriusmed X40C'
        #endif
    #elif (MACHINE_TYPE == X5)
        #define SYSTEM_DEVICE_TYPE                      'AnaesVent'              
        #if (LOGO_FLAG == ATESE_LOGO)
             #define SYSTEM_MODEL                       'Atese X5'
        #else
             #define SYSTEM_MODEL                       'Siriusmed X5'
        #endif             
    #elif (MACHINE_TYPE == Z5)
        #define SYSTEM_DEVICE_TYPE                      'AnaesVent'              
        #if (LOGO_FLAG == HOSPITECH_LOGO)
             #define SYSTEM_MODEL                       'Hospitech IMH-Z5'
        #elif (LOGO_FLAG == GENUINE_LOGO)
             #define SYSTEM_MODEL                       'Genuine IDG-Z5'
        #else
             #define SYSTEM_MODEL                       'Hospitech IMH-Z5'
        #endif            
    #endif
#elif defined __PROJECT_X30__
    #define SYSTEM_SCREEN_MODEL                         '640x480'
    #if (MACHINE_TYPE == X30)
        #define SYSTEM_DEVICE_TYPE                      'Anaes'             
        #if (LOGO_FLAG == SIRIUSMED_LOGO) \
         || (LOGO_FLAG == NEW_SIRIUSMED_LOGO) \
         || (LOGO_FLAG == NEUTRAL_LOGO)
             #define SYSTEM_MODEL                       'Siriusmed X30'
        #elif (LOGO_FLAG == HEYER_LOGO)
             #define SYSTEM_MODEL                       'Heyer X30'
        #else
             #define SYSTEM_MODEL                       'Siriusmed X30'
        #endif
    #elif (MACHINE_TYPE == X3)
        #define SYSTEM_DEVICE_TYPE                      'AnaesVent'              
        #if (LOGO_FLAG == ATESE_LOGO)
             #define SYSTEM_MODEL                       'Atese X3'
        #elif (LOGO_FLAG == DRE_LOGO)
             #define SYSTEM_MODEL                       'DRE X3'
        #else
             #define SYSTEM_MODEL                       'Atese X3' 
        #endif
    #elif (MACHINE_TYPE == Z3)
        #define SYSTEM_DEVICE_TYPE                      'AnaesVent'              
        #define SYSTEM_MODEL                            'Hospitech IMH-Z3' 
    #endif
#elif defined __PROJECT_R55_R35__
#elif defined __PROJECT_R50_R30__
#else
#endif
         
   

#endif

