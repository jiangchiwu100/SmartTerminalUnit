
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
#include "inoutUser.h"

int main(void)
{
	CLOSE_ALL_INT();
	SystemconfigInit();
	LedDriverInit();
	KeyDriverInit();
	SwitchDriverInit();
	uartInit();
	LcdInit();
	ZiKuInit();
	OPEN_ALL_INT();
	GUIStartInterface();
	DLT634_HMI_MASTER_INIT(0);
	while(1)                            
	{
		ScanSwitchStatus();
		ScanKeyStatus();
		Hmi101Main();
		InOutUserMain();
	}
}

/* END */
