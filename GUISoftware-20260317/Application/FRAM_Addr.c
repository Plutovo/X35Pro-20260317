/* Includes ----------------------------------------------------------------- */
#include "FRAM_Addr.h"
#include "FRAM_Addr_cfg.h"
#include "MainTask.h"
#ifdef __ICCARM__ 
#include "IIC.h"
#endif

/* Private macros ----------------------------------------------------------- */
#define FRAM_ReadDataByte(addr)             RTEEPROMread(addr)
#define FRAM_WriteDataByte(addr,data)       RTEEPROMwrite(addr,data)

/* Private Variables -------------------------------------------------------- */
/*********************************************************************//**
 *  In the IAR compiler, Using EXSRAM memory in SDRAM, 
 *  and It is not allowed to delete!!!
 **********************************************************************/ 

#ifdef __ICCARM__   //IAR compiler
__no_init static char SYSTEM_MODEL_Buf[40]                     POOL_EXRAM;
__no_init static char SYSTEM_SCREEN_Buf[20]                    POOL_EXRAM;
__no_init static char SYSTEM_DevType_Buf[20]                   POOL_EXRAM;
__no_init static char SYSTEM_Move_Buf[SYSTEM_TOTAL_SIZE]       POOL_EXRAM;
__no_init static char SETUP_Move_Buf[SETUP_TOTAL_SIZE]         POOL_EXRAM;

__no_init static char SYSTEM_GUIVer_Buf[8]                     POOL_EXRAM;
__no_init static char SYSTEM_BDU_BOOT_Buf[8]                   POOL_EXRAM;
__no_init static char SYSTEM_BDU_APP_Buf[8]                    POOL_EXRAM;
__no_init static char SYSTEM_EGM_BOOT_Buf[8]                   POOL_EXRAM;
__no_init static char SYSTEM_EGM_APP_Buf[8]                    POOL_EXRAM;
__no_init static char SYSTEM_PMU_BOOT_Buf[8]                   POOL_EXRAM;
__no_init static char SYSTEM_PMU_APP_Buf[8]                    POOL_EXRAM;
#else               //visual studio
          static char SYSTEM_MODEL_Buf[40];
          static char SYSTEM_SCREEN_Buf[20];
          static char SYSTEM_DevType_Buf[20];
          static char SYSTEM_Move_Buf[SYSTEM_TOTAL_SIZE];
          static char SETUP_Move_Buf[SETUP_TOTAL_SIZE];
          
          static char SYSTEM_GUIVer_Buf[8];
          static char SYSTEM_BDU_BOOT_Buf[8];
          static char SYSTEM_BDU_APP_Buf[8];
          static char SYSTEM_EGM_BOOT_Buf[8];
          static char SYSTEM_EGM_APP_Buf[8];
          static char SYSTEM_PMU_BOOT_Buf[8];
          static char SYSTEM_PMU_APP_Buf[8];          
#endif
          
BOARD_VERSION Board_Version = {0};  //存放各个板子的boot和app版本

void FRAM_Addr_WriteInfo(void)
{
    if(FRAM_Addr_SysUseNewAddr() == error)
    {
        FRAM_Addr_DataMove();
    }
    FRAM_Addr_SysStandard();
    FRAM_Addr_SysModelType(MType_Model);
    FRAM_Addr_SysModelType(MType_ScreenModel);
    FRAM_Addr_SysModelType(MType_DeviceType);
    FRAM_Addr_Sys_Version(Board_GUI_APP);
    FRAM_Addr_Sys_Version(Board_BDU_BOOT);
    FRAM_Addr_Sys_Version(Board_BDU_APP);
    FRAM_Addr_Sys_Version(Board_EGM_BOOT);
    FRAM_Addr_Sys_Version(Board_EGM_APP);
    FRAM_Addr_Sys_Version(Board_PMU_BOOT);
    FRAM_Addr_Sys_Version(Board_PMU_APP);
    
}
          
/*********************************************************************//**
 * @brief       Initialize block data in SDRAM
 * @param[in]   None
 * @return      None
 **********************************************************************/ 
          
void FRAM_Addr_Init(void)
{
    memset(SYSTEM_MODEL_Buf,        0,  sizeof(SYSTEM_MODEL_Buf ) );
    memset(SYSTEM_SCREEN_Buf,       0,  sizeof(SYSTEM_SCREEN_Buf) );
    memset(SYSTEM_Move_Buf,         0,  sizeof(SYSTEM_Move_Buf)   );
    memset(SETUP_Move_Buf,          0,  sizeof(SETUP_Move_Buf)    );
    memset(SYSTEM_DevType_Buf,      0,  sizeof(SYSTEM_DevType_Buf));
    
    memset(SYSTEM_GUIVer_Buf,       0,  sizeof(SYSTEM_GUIVer_Buf) );
    memset(SYSTEM_BDU_BOOT_Buf,     0,  sizeof(SYSTEM_BDU_BOOT_Buf) );
    memset(SYSTEM_BDU_APP_Buf,      0,  sizeof(SYSTEM_BDU_APP_Buf) );
    memset(SYSTEM_EGM_BOOT_Buf,     0,  sizeof(SYSTEM_EGM_BOOT_Buf) );
    memset(SYSTEM_EGM_APP_Buf,      0,  sizeof(SYSTEM_EGM_APP_Buf) );
    memset(SYSTEM_PMU_BOOT_Buf,     0,  sizeof(SYSTEM_PMU_BOOT_Buf) );
    memset(SYSTEM_PMU_APP_Buf,      0,  sizeof(SYSTEM_PMU_APP_Buf) );
    memset(&Board_Version,          0,  sizeof(Board_Version));
    
    FRAM_Addr_SysModelInit();
    FRAM_Addr_SysScreenModelInit();
    FRAM_Addr_SysDeviceTypeInit();
    
    FRAM_Addr_SysGUIVerInit();
    FRAM_Addr_SysBDUBOOTInit();
    FRAM_Addr_SysBDUAPPInit();
    FRAM_Addr_SysEGMBOOTInit();
    FRAM_Addr_SysEGMAPPInit();
    FRAM_Addr_SysPMUBOOTInit();
    FRAM_Addr_SysPMUAPPInit();
}


/*********************************************************************//**
 * @brief       Init of the system for the first time
 * @param[in]   None
 * @return      success or error
 **********************************************************************/ 

FlagStatus FRAM_Addr_SysInitFlag(void)
{
    u32 SystemInitFlag = 0;
    
    SystemInitFlag = FRAM_ReadDataByte(SYSTEM_INIT_FLAG0_ADDR) << 24 \
                   | FRAM_ReadDataByte(SYSTEM_INIT_FLAG1_ADDR) << 16 \
                   | FRAM_ReadDataByte(SYSTEM_INIT_FLAG2_ADDR) <<  8 \
                   | FRAM_ReadDataByte(SYSTEM_INIT_FLAG3_ADDR);
    
    if(SystemInitFlag == SYSTEM_INIT_FLAG) //init
    {
        return success;
        //user code add
    }
    else    //no init
    {
        FRAM_WriteDataByte(SYSTEM_INIT_FLAG0_ADDR, (SYSTEM_INIT_FLAG >> 24)&0xFF);
        FRAM_WriteDataByte(SYSTEM_INIT_FLAG1_ADDR, (SYSTEM_INIT_FLAG >> 16)&0xFF);
        FRAM_WriteDataByte(SYSTEM_INIT_FLAG2_ADDR, (SYSTEM_INIT_FLAG >>  8)&0xFF);
        FRAM_WriteDataByte(SYSTEM_INIT_FLAG3_ADDR, (SYSTEM_INIT_FLAG >>  0)&0xFF);
        //read back up addr
        SystemInitFlag = FRAM_ReadDataByte(SYSTEM_INIT_FLAG0_ADDR_BACKUP) << 24 \
                       | FRAM_ReadDataByte(SYSTEM_INIT_FLAG1_ADDR_BACKUP) << 16 \
                       | FRAM_ReadDataByte(SYSTEM_INIT_FLAG2_ADDR_BACKUP) <<  8 \
                       | FRAM_ReadDataByte(SYSTEM_INIT_FLAG3_ADDR_BACKUP);
        if(SystemInitFlag == SYSTEM_INIT_FLAG)
        {
            return success;
            //user code add                
        }
        else
        {
            FRAM_WriteDataByte(SYSTEM_INIT_FLAG0_ADDR_BACKUP,   0x11);
            FRAM_WriteDataByte(SYSTEM_INIT_FLAG1_ADDR_BACKUP,   0x22);
            FRAM_WriteDataByte(SYSTEM_INIT_FLAG2_ADDR_BACKUP,   0x33);
            FRAM_WriteDataByte(SYSTEM_INIT_FLAG3_ADDR_BACKUP,   0x44);                
            
            FRAM_Addr_SysUpdateClear();
            RTEEPROMwrite(Medibus_COMM_ADDR,0x01);
            
            return error;
            //user code add                
        }
    }
}

/*********************************************************************//**
 * @brief       write system init flag to FRAM
 * @param[in]   None
 * @return      None
 **********************************************************************/ 
void FRAM_Addr_SysInitFlag_Init(void)
{
    FRAM_WriteDataByte(SYSTEM_INIT_FLAG0_ADDR, (SYSTEM_INIT_FLAG >> 24)&0xFF);
    FRAM_WriteDataByte(SYSTEM_INIT_FLAG1_ADDR, (SYSTEM_INIT_FLAG >> 16)&0xFF);
    FRAM_WriteDataByte(SYSTEM_INIT_FLAG2_ADDR, (SYSTEM_INIT_FLAG >>  8)&0xFF);
    FRAM_WriteDataByte(SYSTEM_INIT_FLAG3_ADDR, (SYSTEM_INIT_FLAG >>  0)&0xFF); 
    
    FRAM_WriteDataByte(SYSTEM_INIT_FLAG0_ADDR_BACKUP, (SYSTEM_INIT_FLAG >> 24)&0xFF);
    FRAM_WriteDataByte(SYSTEM_INIT_FLAG1_ADDR_BACKUP, (SYSTEM_INIT_FLAG >> 16)&0xFF);
    FRAM_WriteDataByte(SYSTEM_INIT_FLAG2_ADDR_BACKUP, (SYSTEM_INIT_FLAG >>  8)&0xFF);
    FRAM_WriteDataByte(SYSTEM_INIT_FLAG3_ADDR_BACKUP, (SYSTEM_INIT_FLAG >>  0)&0xFF);     
}

/*********************************************************************//**
 * @brief       Init of the system model
 * @param[in]   None
 * @return      Init Success or Error
 **********************************************************************/  

FlagStatus FRAM_Addr_SysModelInit(void)
{
    if(sizeof(STR2(SYSTEM_MODEL)) > SYSTEM_MODEL_SIZE+1)
    {
        return error;
    }
    strcpy(SYSTEM_MODEL_Buf,STR2(SYSTEM_MODEL));
    return success;    
}

/*********************************************************************//**
 * @brief       Read the version of the bootloader
 * @param[in]   buf     pointer to block input data
 * @param[in]   size    size of block data
 * @return      Read BOOT version Success or Error
 **********************************************************************/ 

FlagStatus FRAM_Addr_SysReadBOOTVer(char *buf, u8 size)
{
    if(size < SYSTEM_BOOTVer_SIZE)
    {
        return error;
    }
    buf[0] = FRAM_ReadDataByte(SYSTEM_BOOT_MAIN    );
    buf[1] = FRAM_ReadDataByte(SYSTEM_BOOT_MAIN+1  );
    buf[2] = FRAM_ReadDataByte(SYSTEM_BOOT_SUB     );
    buf[3] = FRAM_ReadDataByte(SYSTEM_BOOT_SUB+1   );
    buf[4] = FRAM_ReadDataByte(SYSTEM_BOOT_DEBUG   );
    buf[5] = FRAM_ReadDataByte(SYSTEM_BOOT_DEBUG+1 );
    buf[6] = FRAM_ReadDataByte(SYSTEM_BOOT_TEST    );
    buf[7] = '\0';
    
    if((buf[1] != '.') || (buf[3] != '.') || (buf[5] != '.'))
    {
        buf[0] = FRAM_ReadDataByte(SYSTEM_BOOT_MAIN_BACKUP    );
        buf[1] = FRAM_ReadDataByte(SYSTEM_BOOT_MAIN_BACKUP+1  );
        buf[2] = FRAM_ReadDataByte(SYSTEM_BOOT_SUB_BACKUP     );
        buf[3] = FRAM_ReadDataByte(SYSTEM_BOOT_SUB_BACKUP+1   );
        buf[4] = FRAM_ReadDataByte(SYSTEM_BOOT_DEBUG_BACKUP   );
        buf[5] = FRAM_ReadDataByte(SYSTEM_BOOT_DEBUG_BACKUP+1 );
        buf[6] = FRAM_ReadDataByte(SYSTEM_BOOT_TEST_BACKUP    );
        buf[7] = '\0';
        if((buf[1] != '.') || (buf[3] != '.') || (buf[5] != '.'))
        {
            return error;
        }
        else
        {
            return success;
        }
    }   
    else
    {
        return success;
    }
}

/*********************************************************************//**
 * @brief       Init of the GUI version
 * @param[in]   None
 * @return      None
 **********************************************************************/

void FRAM_Addr_SysGUIVerInit(void)
{
    SYSTEM_GUIVer_Buf[0] = GUI_VERSION_MAIN;
    SYSTEM_GUIVer_Buf[1] = '.';
    SYSTEM_GUIVer_Buf[2] = GUI_VERSION_SUB;
    SYSTEM_GUIVer_Buf[3] = '.';
    SYSTEM_GUIVer_Buf[4] = GUI_VERSION_DEBUG;
    SYSTEM_GUIVer_Buf[5] = '.';
    SYSTEM_GUIVer_Buf[6] = GUI_VERSION_TEST;
    SYSTEM_GUIVer_Buf[7] = '\0';  
}

/*********************************************************************//**
 * @brief       Read the version of the GUI
 * @param[in]   buf     pointer to block input data
 * @param[in]   size    size of block data
 * @return      Read GUI version Success or Error
 **********************************************************************/

FlagStatus FRAM_Addr_SysReadGUIVer(char *buf, u8 size)
{
    u8 i;
    
    if(size < SYSTEM_GUIVer_SIZE)
    {
        return error;
    }
    
    for(i=0;i<SYSTEM_GUIVer_SIZE;i++)
    {
        buf[i] = FRAM_ReadDataByte(SYSTEM_GUI_MAIN+i);
    } 
    
    if((buf[1] != '.') || (buf[3] != '.') || (buf[5] != '.') \
    || (buf[0] != GUI_VERSION_MAIN)  || (buf[2] != GUI_VERSION_SUB) \
    || (buf[4] != GUI_VERSION_DEBUG) || (buf[6] != GUI_VERSION_TEST))
    {
        for(i=0;i<SYSTEM_GUIVer_SIZE;i++)
        {
            buf[i] = FRAM_ReadDataByte(SYSTEM_GUI_MAIN_BACKUP+i);
        } 
        if((buf[1] != '.') || (buf[3] != '.') || (buf[5] != '.') \
        || (buf[0] != GUI_VERSION_MAIN)  || (buf[2] != GUI_VERSION_SUB) \
        || (buf[4] != GUI_VERSION_DEBUG) || (buf[6] != GUI_VERSION_TEST))
        {
            return error;
        }
        else
        {
            return success;
        }
    }
    else
    {
        return success;
    }   
}

/*********************************************************************//**
 * @brief       Write the version of the GUI
 * @param[in]   None
 * @return      Write GUI version Success or Error
 **********************************************************************/

void FRAM_Addr_SysWriteGUIVer(void)
{
    u8 i = 0;
    
    for(i=0;i<SYSTEM_GUIVer_SIZE;i++)
    {
        FRAM_WriteDataByte(SYSTEM_GUI_MAIN+i,         SYSTEM_GUIVer_Buf[i]);
        FRAM_WriteDataByte(SYSTEM_GUI_MAIN_BACKUP+i,  SYSTEM_GUIVer_Buf[i]);
    }
}

/*********************************************************************//**
 * @brief       clear system update flag
 * @param[in]   None
 * @return      None
 **********************************************************************/

void FRAM_Addr_SysUpdateClear(void)
{
    FRAM_WriteDataByte(ADDRESSFLAG_ADDR,                0);
    FRAM_WriteDataByte(SOFTWARE_UPGRADE_BACKUP_ADDR1,   0);
    FRAM_WriteDataByte(SOFTWARE_UPGRADE_BACKUP_ADDR2,   0);
    FRAM_WriteDataByte(ErasureMarkADD1,                 0);
    FRAM_WriteDataByte(SOFTWARE_UPGRADE_ADDR1,          0);
    FRAM_WriteDataByte(SOFTWARE_UPGRADE_ADDR2,          0);
    FRAM_WriteDataByte(ErasureMarkADD2,                 0);
}

/*********************************************************************//**
 * @brief       Write system use-new-addr flag to FRAM
 * @param[in]   None
 * @return      None
 **********************************************************************/

FlagStatus FRAM_Addr_SysUseNewAddr(void)
{
    u32 SystemUseNewAddr = 0;
    
    SystemUseNewAddr = FRAM_ReadDataByte(SYSTEM_NEWADDR_FLAG0_ADDR) << 24 \
                     | FRAM_ReadDataByte(SYSTEM_NEWADDR_FLAG1_ADDR) << 16 \
                     | FRAM_ReadDataByte(SYSTEM_NEWADDR_FLAG2_ADDR) <<  8 \
                     | FRAM_ReadDataByte(SYSTEM_NEWADDR_FLAG3_ADDR);
    
    if(SystemUseNewAddr == SYSTEM_NEWADDR_FLAG) //init
    {
        return success;
        //user code add
    }
    else    //no init
    {
        FRAM_WriteDataByte(SYSTEM_NEWADDR_FLAG0_ADDR,(SYSTEM_NEWADDR_FLAG >> 24)&0xFF);
        FRAM_WriteDataByte(SYSTEM_NEWADDR_FLAG1_ADDR,(SYSTEM_NEWADDR_FLAG >> 16)&0xFF);
        FRAM_WriteDataByte(SYSTEM_NEWADDR_FLAG2_ADDR,(SYSTEM_NEWADDR_FLAG >>  8)&0xFF);
        FRAM_WriteDataByte(SYSTEM_NEWADDR_FLAG3_ADDR,(SYSTEM_NEWADDR_FLAG >>  0)&0xFF);
        SystemUseNewAddr = FRAM_ReadDataByte(SYSTEM_NEWADDR_FLAG0_ADDR_BACKUP) << 24 \
                         | FRAM_ReadDataByte(SYSTEM_NEWADDR_FLAG1_ADDR_BACKUP) << 16 \
                         | FRAM_ReadDataByte(SYSTEM_NEWADDR_FLAG2_ADDR_BACKUP) <<  8 \
                         | FRAM_ReadDataByte(SYSTEM_NEWADDR_FLAG3_ADDR_BACKUP);
        if(SystemUseNewAddr == SYSTEM_NEWADDR_FLAG) //init        
        {
            return success;
        }
        else
        {
            FRAM_WriteDataByte(SYSTEM_NEWADDR_FLAG0_ADDR_BACKUP,   0x12);
            FRAM_WriteDataByte(SYSTEM_NEWADDR_FLAG1_ADDR_BACKUP,   0x34);
            FRAM_WriteDataByte(SYSTEM_NEWADDR_FLAG2_ADDR_BACKUP,   0x56);
            FRAM_WriteDataByte(SYSTEM_NEWADDR_FLAG3_ADDR_BACKUP,   0x78);
            //FRAM_Addr_SysUpdateClear();
            //user code add
            return error;
        }        
    }    
}

/*********************************************************************//**
 * @brief       Init of the system screen model
 * @param[in]   None
 * @return      Init Success or Error
 **********************************************************************/

FlagStatus FRAM_Addr_SysScreenModelInit(void)
{
    if(sizeof(STR2(SYSTEM_SCREEN_MODEL)) > SYSTEM_SCREEN_SIZE+1)
    {
        return error;
    }
    strcpy(SYSTEM_SCREEN_Buf,STR2(SYSTEM_SCREEN_MODEL));
    
    return success;    
}

/*********************************************************************//**
 * @brief       Write system screen model to FRAM
 * @param[in]   None
 * @return      None
 **********************************************************************/

void FRAM_Addr_SysWriteScreenModel(void)
{
    u8 i;
    
    for(i=0;i<SYSTEM_SCREEN_SIZE;i++)
    {
        FRAM_WriteDataByte(SYSTEM_SCREEN_START_ADDR+i,       SYSTEM_SCREEN_Buf[i]);
        FRAM_WriteDataByte(SYSTEM_SCREEN_START_ADDR_BACKUP+i,SYSTEM_SCREEN_Buf[i]);
    }
}

/*********************************************************************//**
 * @brief       Check whether Sys Model Type has been written to FRAM
 * @param[in]   None
 * @return      success/error
 **********************************************************************/ 


FlagStatus FRAM_Addr_SysModelType(MODEL_Type type)
{
    char *p;
    char System_ModelType[40] = {0} ,i=0;
    char Write_buf[40] = {0};
    char size_buf = 0, size_Len = 0;
    u32 addr = 0,addr_backup = 0;
    
    switch(type)
    {
    case MType_Model:
        size_buf = strlen(SYSTEM_MODEL_Buf);
        size_Len = SYSTEM_MODEL_SIZE;
        addr = SYSTEM_MODEL_START_ADDR;
        addr_backup = SYSTEM_MODEL_START_ADDR_BACKUP;
        strcpy(Write_buf,SYSTEM_MODEL_Buf);
        break;
    case MType_ScreenModel:
        size_buf = strlen(SYSTEM_SCREEN_Buf);
        size_Len = SYSTEM_SCREEN_SIZE;
        addr = SYSTEM_SCREEN_START_ADDR;
        addr_backup = SYSTEM_SCREEN_START_ADDR_BACKUP; 
        strcpy(Write_buf,SYSTEM_SCREEN_Buf);
        break;
    case MType_DeviceType:
        size_buf = strlen(SYSTEM_DevType_Buf);
        size_Len = SYSTEM_MACHINE_TYPE_SIZE;
        addr = SYSTEM_MACHINE_TYPE;
        addr_backup = SYSTEM_MACHINE_TYPE_BACKUP;    
        strcpy(Write_buf,SYSTEM_DevType_Buf);
        break;
    default:
        break;
    }
    
    System_ModelType[size_Len] = '\0';
    for(i=0;i<size_Len;i++)   //读主区域信息
    {
        System_ModelType[i] = FRAM_ReadDataByte(addr+i);
    }  //System_ScreenModel
    
    if(System_ModelType[0] == '\'')   //主区域有
    {
        p = strchr(&System_ModelType[1], '\''); //查找另一个  
        if(p == 0) //没有第二个"\'" 存储错误 
        {
            for(i=0;i<size_Len;i++)//主区域重写，读备份
            {
                FRAM_WriteDataByte(addr+i,Write_buf[i]);
                System_ModelType[i] = FRAM_ReadDataByte(addr_backup+i);
            }
            if(System_ModelType[0] == '\'')   //备份区域有
            {
                p = strchr(&System_ModelType[1], '\'');  //查找另一个  
                if(p == 0) //没有第二个"\'" 存储错误 
                {
                    for(i=0;i<size_Len;i++)//备份区重写
                    {
                        FRAM_WriteDataByte(addr_backup+i,Write_buf[i]);
                    }
                    //user code add    
                    return error;
                }
                else //有信息
                {
                    if(!memcmp(Write_buf,System_ModelType,size_buf))//比较字符串
                    {
                        //user code add
                        return success;
                    }
                    else //当前与存储不一致
                    {
                        for(i=0;i<size_Len;i++)//重写
                        {
                            FRAM_WriteDataByte(addr_backup+i,Write_buf[i]);
                        }
                        //user code add 
                        return error;
                    }
                }
            }
            else //无
            {
                for(i=0;i<size_Len;i++)   //备份区重写
                {
                    FRAM_WriteDataByte(addr_backup+i,Write_buf[i]);
                }  
                //user code add 
                return error;
            }
        }
        else //有机型信息
        {
            if(!memcmp(Write_buf,System_ModelType,size_buf))//比较字符串
            {
                //user code add
                return success;
            }
            else
            {
                for(i=0;i<size_Len;i++)    //不一致 重写
                {
                    FRAM_WriteDataByte(addr+i,Write_buf[i]);
                    FRAM_WriteDataByte(addr_backup+i,Write_buf[i]);
                }
                //user code add 
                return error;
            }
        }
    }
    else //没有型号信息 存储错误
    {
        //read back up addr & write main addr
        for(i=0;i<size_Len;i++)
        {
            FRAM_WriteDataByte(addr+i,Write_buf[i]);
            System_ModelType[i] = FRAM_ReadDataByte(addr_backup+i);
        }
        
        if(System_ModelType[0] == '\'')
        {
            p = strchr(&System_ModelType[1], '\'');  
            if(p == 0) //没有第二个"\'" 存储错误 重写
            {
                for(i=0;i<size_Len;i++)
                {
                    FRAM_WriteDataByte(addr_backup+i,Write_buf[i]);
                }
                
                //user code add
                return error;                
            }
            else //有机型信息
            {
                if(!memcmp(Write_buf,System_ModelType,size_buf))//比较字符串
                {
                    //user code add
                    return success;
                }
                else
                {
                    for(i=0;i<size_Len;i++)
                    {
                        FRAM_WriteDataByte(addr+i,Write_buf[i]);
                    }
                    //user code add 
                    return error;
                }
            }
        }
        else
        {
            for(i=0;i<size_Len;i++)
            {
                FRAM_WriteDataByte(addr_backup+i,Write_buf[i]);
            } 
            //user code add
            return error;
        }
    } 
}

/*********************************************************************//**
 * @brief       Check whether the standard has been written to FRAM
 * @param[in]   None
 * @return      success/error
 **********************************************************************/ 
FlagStatus FRAM_Addr_SysStandard(void)
{
    u8 SysStandard = 0;
    
    SysStandard = FRAM_ReadDataByte(MACHINE_STANDARD_ADDR);
    if(SysStandard == SYSTEM_STANDARD)
    {
        return success;
    }
    else
    {
        FRAM_WriteDataByte(MACHINE_STANDARD_ADDR, SYSTEM_STANDARD);
        SysStandard = FRAM_ReadDataByte(MACHINE_STANDARD_ADDR_BACKUP);
        if(SysStandard == SYSTEM_STANDARD)
        {
            return success;
        }
        else
        {
            FRAM_WriteDataByte(MACHINE_STANDARD_ADDR_BACKUP, SYSTEM_STANDARD);
            return error;
        }
    }
}

/*********************************************************************//**
 * @brief       Init of the system device type
 * @param[in]   None
 * @return      Init Success or Error
 **********************************************************************/

FlagStatus FRAM_Addr_SysDeviceTypeInit(void)
{
    if(sizeof(STR2(SYSTEM_DEVICE_TYPE)) > SYSTEM_MACHINE_TYPE_SIZE+1)
    {
        return error;
    }
    strcpy(SYSTEM_DevType_Buf,STR2(SYSTEM_DEVICE_TYPE));
    
    return success;    
}

/*********************************************************************//**
 * @brief       Write system Device Type to FRAM
 * @param[in]   None
 * @return      None
 **********************************************************************/

void FRAM_Addr_SysWriteDeviceType(void)
{
    u8 i;
    
    for(i=0;i<SYSTEM_MACHINE_TYPE_SIZE;i++)
    {
        FRAM_WriteDataByte(SYSTEM_MACHINE_TYPE+i,       SYSTEM_DevType_Buf[i]);
        FRAM_WriteDataByte(SYSTEM_MACHINE_TYPE_BACKUP+i,SYSTEM_DevType_Buf[i]);
    }
}

void FRAM_Addr_SysBDUBOOTInit(void)
{
    SYSTEM_BDU_BOOT_Buf[0] = Board_Version.BDU_BOOT[0];
    SYSTEM_BDU_BOOT_Buf[1] = '.';
    SYSTEM_BDU_BOOT_Buf[2] = Board_Version.BDU_BOOT[1];
    SYSTEM_BDU_BOOT_Buf[3] = '.';
    SYSTEM_BDU_BOOT_Buf[4] = Board_Version.BDU_BOOT[2];
    SYSTEM_BDU_BOOT_Buf[5] = '.';
    SYSTEM_BDU_BOOT_Buf[6] = Board_Version.BDU_BOOT[3];
    SYSTEM_BDU_BOOT_Buf[7] = '\0';  
}

void FRAM_Addr_SysWriteBDUBOOT(void)
{
    u8 i = 0;
    
    for(i=0;i<SYSTEM_BDU_BOOT_SIZE;i++)
    {
        FRAM_WriteDataByte(SYSTEM_BDU_BOOT_MAIN+i,         SYSTEM_BDU_BOOT_Buf[i]);
        FRAM_WriteDataByte(SYSTEM_BDU_BOOT_MAIN_BACKUP+i,  SYSTEM_BDU_BOOT_Buf[i]);
    }
}

void FRAM_Addr_SysBDUAPPInit(void)
{
    SYSTEM_BDU_APP_Buf[0] = Board_Version.BDU_APP[0];
    SYSTEM_BDU_APP_Buf[1] = '.';
    SYSTEM_BDU_APP_Buf[2] = Board_Version.BDU_APP[1];
    SYSTEM_BDU_APP_Buf[3] = '.';
    SYSTEM_BDU_APP_Buf[4] = Board_Version.BDU_APP[2];
    SYSTEM_BDU_APP_Buf[5] = '.';
    SYSTEM_BDU_APP_Buf[6] = Board_Version.BDU_APP[3];
    SYSTEM_BDU_APP_Buf[7] = '\0';      
}

void FRAM_Addr_SysWriteBDUAPP(void)
{
    u8 i = 0;
    
    for(i=0;i<SYSTEM_BDU_SIZE;i++)
    {
        FRAM_WriteDataByte(SYSTEM_BDU_MAIN+i,         SYSTEM_BDU_APP_Buf[i]);
        FRAM_WriteDataByte(SYSTEM_BDU_MAIN_BACKUP+i,  SYSTEM_BDU_APP_Buf[i]);
    }
}

void FRAM_Addr_SysEGMBOOTInit(void)
{
    SYSTEM_EGM_BOOT_Buf[0] = Board_Version.EGM_BOOT[0];
    SYSTEM_EGM_BOOT_Buf[1] = '.';
    SYSTEM_EGM_BOOT_Buf[2] = Board_Version.EGM_BOOT[1];
    SYSTEM_EGM_BOOT_Buf[3] = '.';
    SYSTEM_EGM_BOOT_Buf[4] = Board_Version.EGM_BOOT[2];
    SYSTEM_EGM_BOOT_Buf[5] = '.';
    SYSTEM_EGM_BOOT_Buf[6] = Board_Version.EGM_BOOT[3];
    SYSTEM_EGM_BOOT_Buf[7] = '\0';     
}

void FRAM_Addr_SysWriteEGMBOOT(void)
{
    u8 i = 0;
    
    for(i=0;i<SYSTEM_EGM_BOOT_SIZE;i++)
    {
        FRAM_WriteDataByte(SYSTEM_EGM_BOOT_MAIN+i,         SYSTEM_EGM_BOOT_Buf[i]);
        FRAM_WriteDataByte(SYSTEM_EGM_BOOT_MAIN_BACKUP+i,  SYSTEM_EGM_BOOT_Buf[i]);
    }
}

void FRAM_Addr_SysEGMAPPInit(void)
{
    SYSTEM_EGM_APP_Buf[0] = Board_Version.EGM_APP[0];
    SYSTEM_EGM_APP_Buf[1] = '.';
    SYSTEM_EGM_APP_Buf[2] = Board_Version.EGM_APP[1];
    SYSTEM_EGM_APP_Buf[3] = '.';
    SYSTEM_EGM_APP_Buf[4] = Board_Version.EGM_APP[2];
    SYSTEM_EGM_APP_Buf[5] = '.';
    SYSTEM_EGM_APP_Buf[6] = Board_Version.EGM_APP[3];
    SYSTEM_EGM_APP_Buf[7] = '\0';       
}

void FRAM_Addr_SysWriteEGMAPP(void)
{
    u8 i = 0;
    
    for(i=0;i<SYSTEM_EGM_SIZE;i++)
    {
        FRAM_WriteDataByte(SYSTEM_EGM_MAIN+i,         SYSTEM_EGM_APP_Buf[i]);
        FRAM_WriteDataByte(SYSTEM_EGM_MAIN_BACKUP+i,  SYSTEM_EGM_APP_Buf[i]);
    }
}

void FRAM_Addr_SysPMUBOOTInit(void)
{
    SYSTEM_PMU_BOOT_Buf[0] = Board_Version.PMU_BOOT[0];
    SYSTEM_PMU_BOOT_Buf[1] = '.';
    SYSTEM_PMU_BOOT_Buf[2] = Board_Version.PMU_BOOT[1];
    SYSTEM_PMU_BOOT_Buf[3] = '.';
    SYSTEM_PMU_BOOT_Buf[4] = Board_Version.PMU_BOOT[2];
    SYSTEM_PMU_BOOT_Buf[5] = '.';
    SYSTEM_PMU_BOOT_Buf[6] = Board_Version.PMU_BOOT[3];
    SYSTEM_PMU_BOOT_Buf[7] = '\0';        
}

void FRAM_Addr_SysWritePMUBOOT(void)
{
    u8 i = 0;
    
    for(i=0;i<SYSTEM_PMU_BOOT_SIZE;i++)
    {
        FRAM_WriteDataByte(SYSTEM_PMU_BOOT_MAIN+i,         SYSTEM_PMU_BOOT_Buf[i]);
        FRAM_WriteDataByte(SYSTEM_PMU_BOOT_MAIN_BACKUP+i,  SYSTEM_PMU_BOOT_Buf[i]);
    }
}

void FRAM_Addr_SysPMUAPPInit(void)
{
    SYSTEM_PMU_APP_Buf[0] = Board_Version.PMU_APP[0];
    SYSTEM_PMU_APP_Buf[1] = '.';
    SYSTEM_PMU_APP_Buf[2] = Board_Version.PMU_APP[1];
    SYSTEM_PMU_APP_Buf[3] = '.';
    SYSTEM_PMU_APP_Buf[4] = Board_Version.PMU_APP[2];
    SYSTEM_PMU_APP_Buf[5] = '.';
    SYSTEM_PMU_APP_Buf[6] = Board_Version.PMU_APP[3];
    SYSTEM_PMU_APP_Buf[7] = '\0';        
}


void FRAM_Addr_SysWritePMUAPP(void)
{
    u8 i = 0;
    
    for(i=0;i<SYSTEM_PMU_SIZE;i++)
    {
        FRAM_WriteDataByte(SYSTEM_PMU_MAIN+i,         SYSTEM_PMU_APP_Buf[i]);
        FRAM_WriteDataByte(SYSTEM_PMU_MAIN_BACKUP+i,  SYSTEM_PMU_APP_Buf[i]);
    }
}


FlagStatus FRAM_Addr_Sys_Version(BOARD_VERSION_Type type)
{
    u8 i;
    u8 size = 0;
    u32 addr = 0, addr_backup = 0;
    char Read_Buf[8] = {0};
    char Write_Buf[8] = {0};
    char tmp_Version[4] = {0};
    
    switch(type)
    {
    case Board_GUI_BOOT:
        break;
    case Board_GUI_APP:
        size = SYSTEM_GUIVer_SIZE;
        addr = SYSTEM_GUI_MAIN;
        addr_backup = SYSTEM_GUI_MAIN_BACKUP;
        memcpy(Write_Buf,SYSTEM_GUIVer_Buf,size);
        tmp_Version[0] = GUI_VERSION_MAIN;
        tmp_Version[1] = GUI_VERSION_SUB;
        tmp_Version[2] = GUI_VERSION_DEBUG;
        tmp_Version[3] = GUI_VERSION_TEST;
        break;
    case Board_BDU_BOOT:
        size = SYSTEM_BDU_BOOT_SIZE;
        addr = SYSTEM_BDU_BOOT_MAIN;
        addr_backup = SYSTEM_BDU_BOOT_MAIN_BACKUP;
        memcpy(Write_Buf,SYSTEM_BDU_BOOT_Buf,size);
        memcpy(tmp_Version,Board_Version.BDU_BOOT,4);
        break;
    case Board_BDU_APP:
        size = SYSTEM_BDU_SIZE;
        addr = SYSTEM_BDU_MAIN;
        addr_backup = SYSTEM_BDU_MAIN_BACKUP;        
        memcpy(Write_Buf,SYSTEM_BDU_APP_Buf,size);
        memcpy(tmp_Version,Board_Version.BDU_APP,4);
        break;
    case Board_EGM_BOOT:
        size = SYSTEM_EGM_BOOT_SIZE;
        addr = SYSTEM_EGM_BOOT_MAIN;
        addr_backup = SYSTEM_EGM_BOOT_MAIN_BACKUP; 
        memcpy(Write_Buf,SYSTEM_EGM_BOOT_Buf,size);
        memcpy(tmp_Version,Board_Version.EGM_BOOT,4);
        break;
    case Board_EGM_APP:
        size = SYSTEM_EGM_SIZE;
        addr = SYSTEM_EGM_MAIN;
        addr_backup = SYSTEM_EGM_MAIN_BACKUP;   
        memcpy(Write_Buf,SYSTEM_EGM_APP_Buf,size);
        memcpy(tmp_Version,Board_Version.EGM_APP,4);
        break;
    case Board_PMU_BOOT:
        size = SYSTEM_PMU_BOOT_SIZE;
        addr = SYSTEM_PMU_BOOT_MAIN;
        addr_backup = SYSTEM_PMU_BOOT_MAIN_BACKUP; 
        memcpy(Write_Buf,SYSTEM_PMU_BOOT_Buf,size);
        memcpy(tmp_Version,Board_Version.PMU_BOOT,4);
        break;
    case Board_PMU_APP:
        size = SYSTEM_PMU_SIZE;
        addr = SYSTEM_PMU_MAIN;
        addr_backup = SYSTEM_PMU_MAIN_BACKUP;    
        memcpy(Write_Buf,SYSTEM_PMU_APP_Buf,size);
        memcpy(tmp_Version,Board_Version.PMU_APP,4);
        break;        
    default:
        break;        
    }
    
    for(i=0;i<size;i++)
    {
        Read_Buf[i] = FRAM_ReadDataByte(addr+i);
    } 
    
    if((Read_Buf[1] != '.') || (Read_Buf[3] != '.') || (Read_Buf[5] != '.')\
    || (Read_Buf[0] != tmp_Version[0])  || (Read_Buf[2] != tmp_Version[1])   \
    || (Read_Buf[4] != tmp_Version[2]) || (Read_Buf[6] != tmp_Version[3]))
    {
        for(i=0;i<size;i++)
        {
            Read_Buf[i] = FRAM_ReadDataByte(addr_backup+i);
            FRAM_WriteDataByte(addr+i,Write_Buf[i]);
        } 
        if((Read_Buf[1] != '.') || (Read_Buf[3] != '.') || (Read_Buf[5] != '.')\
        || (Read_Buf[0] != tmp_Version[0])  || (Read_Buf[2] != tmp_Version[1])   \
        || (Read_Buf[4] != tmp_Version[2]) || (Read_Buf[6] != tmp_Version[3]))
        {
            for(i=0;i<size;i++)
            {
                FRAM_WriteDataByte(addr_backup+i,  Write_Buf[i]);
            }
            return error;
        }
        else
        {
            //user code add
            return success;
        }
    }
    else
    {
        //user code add
        return success;
    }     
}


/*********************************************************************//**
 * @brief       Trend block data clear
 * @param[in]   None
 * @return      None
 **********************************************************************/ 
void FRAM_Addr_TrendClear(void)
{
    int i;
    int size = Trend_RESERVE_START_ADDR - TREND_TIMERANGE;
    
    for(i=0;i<size;i++)
    {
        FRAM_WriteDataByte(TREND_TIMERANGE+i, 0);
    }
}

/*********************************************************************//**
 * @brief       Alarm block data clear
 * @param[in]   None
 * @return      None
 **********************************************************************/ 
void FRAM_Addr_AlarmClear(void)
{
    int i;
    int size = ALARM_RESERVE_START_ADDR - ALARM_RECORD_LIST_ADDR;
    
    for(i=0;i<size;i++)
    {
        FRAM_WriteDataByte(ALARM_RECORD_LIST_ADDR+i, 0);
    }
}


/*********************************************************************//**
 * @brief       result block data clear
 * @param[in]   None
 * @return      None
 **********************************************************************/ 
void FRAM_Addr_ResultClear(void)
{
    int i;
    int size = SelfTest_RESERVE_START_ADDR - INSPHOLD_CST_LOW_ADDR;
    
    for(i=0;i<size;i++)
    {
        FRAM_WriteDataByte(INSPHOLD_CST_LOW_ADDR+i, 0);
    }
}

/*********************************************************************//**
 * @brief       FRAM data move
 * @param[in]   None
 * @return      None
 **********************************************************************/ 

void FRAM_Addr_DataMove(void)
{
    int i = 0, j = 0, Sys_Ofs = 0, Setup_Ofs = 0;
    int size = 0;
    int Sys_oldFlag = 0;
    
    //系统类数据读取
    {
        //触摸屏AD值
        for(i=0;i<8;i++)    
        {
            SYSTEM_Move_Buf[i] = FRAM_ReadDataByte(TOUCH_AD_LEFT_Old+i);
        }
        Sys_Ofs = 8;
        //流量传感器编码
        for(i=0;i<8;i++)
        {
            SYSTEM_Move_Buf[i+Sys_Ofs] = FRAM_ReadDataByte(FLOW_SERIESNUM_ADDR_Old+i);
        }
        Sys_Ofs = 16;
        //软件设备序列号
        for(i=0;i<8;i++)
        {
            SYSTEM_Move_Buf[i+Sys_Ofs] = FRAM_ReadDataByte(DEVICE_NUM_0_ADDR_Old+i);
        }
        Sys_Ofs = 24;
        //设备序列号存在标志
        SYSTEM_Move_Buf[Sys_Ofs] = FRAM_ReadDataByte(DEVICE_FLAG_HIGH_ADDR_Old);
        SYSTEM_Move_Buf[Sys_Ofs+1] = FRAM_ReadDataByte(DEVICE_FLAG_LOW_ADDR_Old);
        Sys_Ofs = 26;
        //美标/欧标切换
        SYSTEM_Move_Buf[Sys_Ofs] = FRAM_ReadDataByte(EGM_STANDARD_ADDR_Old);
        SYSTEM_Move_Buf[Sys_Ofs+1] = 0;
        Sys_Ofs = 28;
        //set 0
        for(i=0;i<SYSTEM_SCREEN_SIZE;i++)
        {
            SYSTEM_Move_Buf[Sys_Ofs] = 0;
        }
        Sys_Ofs = 28+SYSTEM_SCREEN_SIZE;
        //触摸屏校验值读取
        SYSTEM_Move_Buf[Sys_Ofs] = FRAM_ReadDataByte(TOUCH_CHECKSUM_Old);
        SYSTEM_Move_Buf[Sys_Ofs+1] = FRAM_ReadDataByte(TOUCH_CHECKSUM_Old+1);
    }
    //设置类数据读取
    {
        //选配
        for(i=0;i<16;i++)
        {
            SETUP_Move_Buf[i] = FRAM_ReadDataByte(OPTIONAL_0_ADDR_Old+i);
        }
        //语言
        SETUP_Move_Buf[16] = FRAM_ReadDataByte(LANGUAGE_TYPE_ADDR_Old);
        SETUP_Move_Buf[17] = 0;
        //音量
        SETUP_Move_Buf[18] = FRAM_ReadDataByte(ALARM_VOICE_VOLUME_ADDR_Old);
        SETUP_Move_Buf[19] = 0;
        //运行时间
        SETUP_Move_Buf[20] = FRAM_ReadDataByte(RUN_TOTAL_TIME_ADDR_Old);
        SETUP_Move_Buf[21] = 0;
        //时间格式
        SETUP_Move_Buf[22] = FRAM_ReadDataByte(TIMEFORMAT_ADDR_Old);
        //大气压力开关
        SETUP_Move_Buf[23] = FRAM_ReadDataByte(BARO_SWITCH_ADDR_Old);
        //大气压力数值
        SETUP_Move_Buf[24] = FRAM_ReadDataByte(BARO_DATA_HIGH_ADDR_Old);
        SETUP_Move_Buf[25] = FRAM_ReadDataByte(BARO_DATA_LOW_ADDR_Old);
        //笑气开关
        SETUP_Move_Buf[26] = FRAM_ReadDataByte(N2O_SWITCH_ADDR_Old);
        //驱动气体
        SETUP_Move_Buf[27] = FRAM_ReadDataByte(DIRVER_GAS_ADDR_Old);
        //笑气气源开关
        SETUP_Move_Buf[28] = FRAM_ReadDataByte(N2O_SUPPLY_SWITCH_ADDR_Old);
        //空气气源开关
        SETUP_Move_Buf[29] = FRAM_ReadDataByte(AIR_SUPPLY_SWITCH_ADDR_Old);
        //氧气
        SETUP_Move_Buf[30] = FRAM_ReadDataByte(O2_SWITCH_ADDR_Old);
        //AA/CO2
        SETUP_Move_Buf[31] = FRAM_ReadDataByte(ETCO2_SWITCH_ADDR_Old);
        //SPO2
        SETUP_Move_Buf[32] = FRAM_ReadDataByte(SPO2_SWITCH_ADDR_Old);
        //MASIMO BEEPER
        SETUP_Move_Buf[33] = FRAM_ReadDataByte(MASIMO_BEEPER_ADDR_Old);
        //MASIMO FAST SAT
        SETUP_Move_Buf[34] = FRAM_ReadDataByte(MASIMO_FAST_SAT_ADDR_Old);
        //MASIMO SENSITIVITY
        SETUP_Move_Buf[35] = FRAM_ReadDataByte(MASIMO_SENSITIVITY_ADDR_Old);
        //MASIMO SMART TONE
        SETUP_Move_Buf[36] = FRAM_ReadDataByte(MASIMO_SMART_TONE_ADDR_Old);
        //MASIMO AVERAGE TIME
        SETUP_Move_Buf[37] = FRAM_ReadDataByte(MASIMO_AVERAGE_TIME_ADDR_Old);
        //大气压力单位
        SETUP_Move_Buf[38] = FRAM_ReadDataByte(UNIT_BAROMETIRC_ADDR_Old);
        //CO2单位
        SETUP_Move_Buf[39] = FRAM_ReadDataByte(UNIT_CO2_ADDR_Old);
        //身高单位
        SETUP_Move_Buf[40] = FRAM_ReadDataByte(UNIT_HEIGHT_ADDR_Old);
        //气道压力单位
        SETUP_Move_Buf[41] = FRAM_ReadDataByte(UNIT_AIRWAY_ADDR_Old);
        //气源单位
        SETUP_Move_Buf[42] = FRAM_ReadDataByte(UNIT_GAS_ADDR_Old);
        //体重单位
        SETUP_Move_Buf[43] = FRAM_ReadDataByte(UNIT_WEIGHT_ADDR_Old);
        //Ti/IE
        SETUP_Move_Buf[44] = FRAM_ReadDataByte(TI_IE_CHOOSE_FLAG_ADDR_Old);
        //Medibus开关
        SETUP_Move_Buf[45] = FRAM_ReadDataByte(Medibus_COMM_ADDR_Old);
        //-------------------------reserve------------------------------------
        SETUP_Move_Buf[46] = 0;
        SETUP_Move_Buf[47] = 0;
        
        //病人类型
        SETUP_Move_Buf[48] = FRAM_ReadDataByte(PATIENT_INFO_TYPE_ADDR_Old);
        //病人性别
        SETUP_Move_Buf[49] = FRAM_ReadDataByte(PATIENT_INFO_GENDER_ADDR_Old);
        //病人通气类型
        SETUP_Move_Buf[50] = FRAM_ReadDataByte(PATIENT_INFO_VENTTYPE_ADDR_Old);
        //病人IBW
        SETUP_Move_Buf[51] = FRAM_ReadDataByte(PATIENT_INFO_IBW_LOW_ADDR_Old);
        SETUP_Move_Buf[52] = FRAM_ReadDataByte(PATIENT_INFO_IBW_HIGH_ADDR_Old);
        //病人身高
        SETUP_Move_Buf[53] = FRAM_ReadDataByte(PATIENT_INFO_STATURE_ADDR_Old);
        //病人年龄
        SETUP_Move_Buf[54] = FRAM_ReadDataByte(PATIENT_INFO_AGE_ADDR_Old);
        //病人理想潮气量
        SETUP_Move_Buf[55] = FRAM_ReadDataByte(PATIENT_INFO_IDEAVT_HIGH_ADDR_Old);
        SETUP_Move_Buf[56] = FRAM_ReadDataByte(PATIENT_INFO_IDEAVT_LOW_ADDR_Old);
        //-------------------------reserve------------------------------------
        Setup_Ofs = 57;
        j = SetUp_RESERVE3_END_ADDR - SetUp_RESERVE3_START_ADDR + 1;
        for(i=0;i<j;i++)
        {
            SETUP_Move_Buf[Setup_Ofs+i] = 0;
        }
        
        //控制参数
        SETUP_Move_Buf[80] = FRAM_ReadDataByte(BREATH_VT_HIGH_ADDR_Old);
        SETUP_Move_Buf[81] = FRAM_ReadDataByte(BREATH_VT_LOW_ADDR_Old);
        SETUP_Move_Buf[82] = FRAM_ReadDataByte(BREATH_Ti_HIGH_ADDR_Old);
        SETUP_Move_Buf[83] = FRAM_ReadDataByte(BREATH_Ti_LOW_ADDR_Old);
        SETUP_Move_Buf[84] = FRAM_ReadDataByte(BREATH_PRE_BREATHMODE_ADDR_Old);
        SETUP_Move_Buf[85] = FRAM_ReadDataByte(BREATH_PRE_APNOEA_ADDR_Old);
        SETUP_Move_Buf[86] = FRAM_ReadDataByte(BREATH_RATE_ADDR_Old);
        SETUP_Move_Buf[87] = FRAM_ReadDataByte(BREATH_RATE_SIMV_ADDR_Old);
        SETUP_Move_Buf[88] = FRAM_ReadDataByte(BREATH_RATE_OTHER_ADDR_Old);
        SETUP_Move_Buf[89] = FRAM_ReadDataByte(BREATH_TP_ADDR_Old);
        SETUP_Move_Buf[90] = FRAM_ReadDataByte(BREATH_PS_ADDR_Old);
        SETUP_Move_Buf[91] = FRAM_ReadDataByte(BREATH_PINSP_ADDR_Old);
        SETUP_Move_Buf[92] = FRAM_ReadDataByte(BREATH_PTR_ADDR_Old);
        SETUP_Move_Buf[93] = FRAM_ReadDataByte(BREATH_FTR_ADDR_Old);
        SETUP_Move_Buf[94] = FRAM_ReadDataByte(BREATH_PEEP_ADDR_Old);
        SETUP_Move_Buf[95] = FRAM_ReadDataByte(BREATH_TRIGGERMODE_ADDR_Old);
        SETUP_Move_Buf[96] = FRAM_ReadDataByte(BREATH_BREATHMODE_ADDR_Old);
        SETUP_Move_Buf[97] = FRAM_ReadDataByte(BREATH_APNOEA_ADDR_Old);
        SETUP_Move_Buf[98] = FRAM_ReadDataByte(BREATH_SIGHTIMES_ADDR_Old);
        SETUP_Move_Buf[99] = FRAM_ReadDataByte(BREATH_TSLOPE_ADDR_Old);
        SETUP_Move_Buf[100] = FRAM_ReadDataByte(BREATH_ESENS_ADDR_Old);
        SETUP_Move_Buf[101] = FRAM_ReadDataByte(BREATH_FLOWPATTEN_ADDR_Old);
        SETUP_Move_Buf[102] = FRAM_ReadDataByte(BREATH_IE_ADDR_Old);
        
        //-------------------------reserve------------------------------------
        Setup_Ofs = 103;
        j = SetUp_RESERVE3_END_ADDR - SetUp_RESERVE3_START_ADDR + 1;
        for(i=0;i<j;i++)
        {
            SETUP_Move_Buf[Setup_Ofs+i] = 0;
        }
        
        //报警参数设置
        SETUP_Move_Buf[128] = FRAM_ReadDataByte(PH_HIGH_ADDR_Old);
        SETUP_Move_Buf[129] = FRAM_ReadDataByte(PH_LOW_ADDR_Old);
        SETUP_Move_Buf[130] = FRAM_ReadDataByte(MV_HIGH_ADDR_Old);
        SETUP_Move_Buf[131] = FRAM_ReadDataByte(MV_LOW_ADDR_Old);
        SETUP_Move_Buf[132] = FRAM_ReadDataByte(RATE_HIGH_ADDR_Old);
        SETUP_Move_Buf[133] = FRAM_ReadDataByte(RATE_LOW_ADDR_Old);
        SETUP_Move_Buf[134] = FRAM_ReadDataByte(FIO2_HIGH_ADDR_Old);
        SETUP_Move_Buf[135] = FRAM_ReadDataByte(FIO2_LOW_ADDR_Old);
        SETUP_Move_Buf[136] = FRAM_ReadDataByte(VTE_HIGH_ADDR_H_Old);
        SETUP_Move_Buf[137] = FRAM_ReadDataByte(VTE_HIGH_ADDR_L_Old);
        SETUP_Move_Buf[138] = FRAM_ReadDataByte(VTE_LOW_ADDR_H_Old);
        SETUP_Move_Buf[139] = FRAM_ReadDataByte(VTE_LOW_ADDR_L_Old);
        SETUP_Move_Buf[140] = FRAM_ReadDataByte(APNEA_TIME_ADDR_Old);
        SETUP_Move_Buf[141] = FRAM_ReadDataByte(ETCO2_HIGH_ADDR_Old);
        SETUP_Move_Buf[142] = FRAM_ReadDataByte(ETCO2_LOW_ADDR_Old);
        SETUP_Move_Buf[143] = FRAM_ReadDataByte(FICO2_HIGH_ADDR_Old);
        SETUP_Move_Buf[144] = FRAM_ReadDataByte(FIN2O_HIGH_ADDR_Old);
        SETUP_Move_Buf[145] = FRAM_ReadDataByte(FIN2O_LOW_ADDR_Old);
        SETUP_Move_Buf[146] = FRAM_ReadDataByte(ETAA_HIGH_ADDR_Old);
        SETUP_Move_Buf[147] = FRAM_ReadDataByte(ETAA_LOW_ADDR_Old);
        SETUP_Move_Buf[148] = FRAM_ReadDataByte(FIAA_HIGH_ADDR_Old);
        SETUP_Move_Buf[149] = FRAM_ReadDataByte(FIAA_LOW_ADDR_Old);
        SETUP_Move_Buf[150] = FRAM_ReadDataByte(FG_HIGH_ADDR_Old);
        SETUP_Move_Buf[151] = FRAM_ReadDataByte(FG_LOW_ADDR_Old);
        SETUP_Move_Buf[152] = FRAM_ReadDataByte(PLUS_HIGH_ADDR_Old);
        SETUP_Move_Buf[153] = FRAM_ReadDataByte(PLUS_LOW_ADDR_Old);
        SETUP_Move_Buf[154] = FRAM_ReadDataByte(SPO2_HIGH_ADDR_Old);
        SETUP_Move_Buf[155] = FRAM_ReadDataByte(SPO2_LOW_ADDR_Old);
        SETUP_Move_Buf[156] = FRAM_ReadDataByte(PI_HIGH_ADDR_H_Old);
        SETUP_Move_Buf[157] = FRAM_ReadDataByte(PI_HIGH_ADDR_L_Old);
        SETUP_Move_Buf[158] = FRAM_ReadDataByte(PI_LOW_ADDR_H_Old);
        SETUP_Move_Buf[159] = FRAM_ReadDataByte(PI_LOW_ADDR_L_Old);
        
        Setup_Ofs = 160;
        //选配备份
        for(i=0;i<16;i++)
        {
            SETUP_Move_Buf[Setup_Ofs+i] = FRAM_ReadDataByte(OPTIONAL_0_ADDR_BACK_Old+i);
        }
        Setup_Ofs = 176;
        SETUP_Move_Buf[176] = FRAM_ReadDataByte(ETCO2_Kpa_HIGH_ADDR_Old);
        SETUP_Move_Buf[177] = FRAM_ReadDataByte(ETCO2_Kpa_LOW_ADDR_Old);
        SETUP_Move_Buf[178] = FRAM_ReadDataByte(FICO2_Kpa_HIGH_ADDR_Old);
        SETUP_Move_Buf[179] = FRAM_ReadDataByte(FICO2_Kpa_LOW_ADDR_Old);        
        //-------------------------reserve------------------------------------
        //user add
    }
    Sys_oldFlag = FRAM_ReadDataByte(SYSTEM_INIT_HIGH_ADDR_Old)<<8 \
                | FRAM_ReadDataByte(SYSTEM_INIT_LOW_ADDR_Old);

    if(Sys_oldFlag == SYSTEM_INIT_FLAG_Old) //已经初始化过
    {
        FRAM_Addr_SysInitFlag_Init();
    }
    
    //新地址系统设置写入
    size = SYSTEM_RESERVE_START_ADDR - TOUCH_AD_LEFT;
    for(i=0;i<size;i++)
    {
        FRAM_WriteDataByte(TOUCH_AD_LEFT+i, SYSTEM_Move_Buf[i]);
    }
    for(i=0;i<size;i++)//备份域写入
    {
        FRAM_WriteDataByte(0x10000+TOUCH_AD_LEFT+i, SYSTEM_Move_Buf[i]);
    }
    
    //新地址用户相关设置写入
    size = SetUp_RESERVE4_START_ADDR - OPTIONAL_0_ADDR;
    for(i=0;i<size;i++)
    {
        FRAM_WriteDataByte(OPTIONAL_0_ADDR+i, SETUP_Move_Buf[i]);
    }  
    //清除其他信息
    FRAM_Addr_TrendClear();
    FRAM_Addr_AlarmClear();
    FRAM_Addr_ResultClear();
}






