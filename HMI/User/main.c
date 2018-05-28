
#include "stm32f10x.h"
#include "systemconfig.h"
#include "uartDriver.h"
#include "ledDriver.h"
#include "lkdGuiUse.h"
#include "dlt634_HMImaster_disk.h"
#include "hmi101.h"
#include "switchDriver.h"
#include "keyDriver.h"
#include "GuiDisplay.h"

int main(void)
{
	CLOSE_ALL_INT();
	SystemconfigInit();
	LedDriverInit();
	LedInit();
	KeyDriverInit();
	SwitchDriverInit();
	uartInit();
	LcdInit();
	ZiKuInit();
	OPEN_ALL_INT();
	GUIStartInterface();
//	GuiUpdateDisplayAll();
	DLT634_HMI_MASTER_INIT(0);
	
	while(1)                            
	{
		ScanSwitchStatus();
		ScanKeyStatus();
		Hmi101Main();
	}
}

/* END */
