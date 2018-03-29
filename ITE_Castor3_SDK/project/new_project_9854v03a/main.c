﻿#include <unistd.h>
#include "SDL/SDL.h"
#include "ite/itp.h"
#include "project.h"
#include "scene.h"

#if 1
#define DVDD_COUNT			63
#define BL_EN_PWM			64	
#endif

int SDL_main(int argc, char *argv[])
{
    int ret = 0;

#ifdef CFG_CHECK_FILES_CRC_ON_BOOTING
    int restryCount = 0;

    BackupInit();
retry_backup:
    ret = UpgradeInit();
    if (ret)
    {
        if (++restryCount > 2)
        {
            printf("cannot recover from backup....\n");
            goto end;
        }
        BackupRestore();
        goto retry_backup;
    }
    BackupSyncFile();
#else
    ret = UpgradeInit();
    if (ret)
        goto end;
#endif

#if 1 //my.wei add for SAT, move to itp_backlight.c
		{
			ithGpioSetMode(DVDD_COUNT, ITH_GPIO_MODE0);
			ithGpioSetOut(DVDD_COUNT);	
			ithGpioSet(DVDD_COUNT); 
			ithGpioEnable(DVDD_COUNT);	
	
			ithGpioSetMode(BL_EN_PWM, ITH_GPIO_MODE0);
			ithGpioSetOut(BL_EN_PWM);
			ithGpioSet(BL_EN_PWM);	
			ithGpioEnable(BL_EN_PWM);
		}
#endif

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
    
    ConfigInit();
#ifdef CFG_NET_ENABLE
    NetworkInit();
#endif    
    SceneInit();
    AudioInit();
    SceneLoad();
    ret = SceneRun();

    SceneExit();
    AudioExit();
    ConfigExit();
    SDL_Quit();

end:
    ret = UpgradeProcess(ret);
    exit(ret);
    return ret;
}
