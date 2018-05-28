
#include "IAP.h"
#include "ff.h"
#include "diskio.h"			/* FatFs lower layer API */
#include "delay.h"
#include "drv_fm25vxx.h"
#include "usart.h"
#include "led.h"
#include "iwdg.h" 

iapfun jump2app;//跳转函数指针

/*文件系统相关变量*/
FATFS fs[_VOLUMES];//逻辑磁盘工作区.	 
FIL file;	  		//文件1
uint32_t SectorError = 0;
HAL_StatusTypeDef FlashStatus=HAL_OK;//flash操作返回变量
uint32_t FileSize;//文件大小
FLASH_EraseInitTypeDef FlashEraseInit;//flash操作变量
uint32_t readFileBuff[READEACHSIZE/4];
uint32_t *pSDRAM;
UINT br,bw;			//读写变量

struct ProgramState_ ProgramState;//存放FM25XX读出变量

uint8_t *run_bin="ftu_run.bin";//运行版本文件名
//uint8_t *updata_bin="ftu_updata.bin";//更新版本文件名


void GetFatfsInfo(void)
{
	DWORD fre_clust, fre_sect, tot_sect;
	FATFS *fs;
	u8 res=0;
	res = f_getfree("0:", &fre_clust, &fs);
	if(res == 0){
		tot_sect = (fs->n_fatent - 2) * fs->csize;
    fre_sect = fre_clust * fs->csize;
		printf("\r\n%10lu KiB total drive space.\r\n%10lu KiB available.\r\n", tot_sect / 2, fre_sect / 2);
	}
}
/**
  * @Description: IAP初始化
  * @param:  void
  * @return: void
  * @updata: 
  */

void FATFSInit(void)
{
	u8 res=0;
	
	res=f_mount(&fs[0],"0:",1); 						//挂载FLASH.
	if(res==0X0D)										//FLASH磁盘,FAT文件系统错误,重新格式化FLASH
	{
		#ifdef DEBUG_
		printf("磁盘格式化");
		#endif
		res=f_mkfs("0:",0,4096);						//格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
		if(res==0)
		{
			f_setlabel((const TCHAR *)"1:ALIENTEK");	//设置Flash磁盘的名字为：ALIENTEK
		}else 
		delay_ms(1000);
	}
	GetFatfsInfo();
}
/**
  * @Description: 升级主程序
  * @param:  void
  * @return: void
  * @updata: 
  */
void Iapmain(void)
{
	if(readProgramFlag() != 0){
		#ifdef DEBUG_
		printf("Iapmain_FM25读出失败\r\n");
		#endif
		return;
	}
	if(ProgramState.firstRun != IS_FIRSTRUN_FLAG){			//第一次上电
		#ifdef DEBUG_
		printf("第一次上电\r\n");
		#endif
		iap_load_app(IAPSTARTADDR,IAPJUMP_FIRSTRUN);
		return;
	}
	if(ProgramState.currentState == IS_WILLUPDATA_FLAG){	//有程序要更新
		#ifdef DEBUG_
		printf("有升级更新\r\n");
		#endif	
		programUpdata();
		return;
	}
	else if(ProgramState.currentState == IS_RUNOK_FLAG){	//无更新，运行正常
		#ifdef DEBUG_
		printf("无更新,程序正常\r\n");
		#endif
		iap_load_app(IAPSTARTADDR,IAPJUMP_RUNOK);
		return;
	}
	else if(ProgramState.currentState != IS_RUNOK_FLAG){	//运行异常
		#ifdef DEBUG_
		printf("无更新,程序异常\r\n");
		#endif
		programRunError();
		return;
	}
	else{
	}
}

/**
  * @Description: 更新程序
  * @param:  void
  * @return: void
  */
void programUpdata(void)
{
	uint8_t tempflag = 0;
	uint8_t tempVaule = 0;
	uint8_t i;
	for(i = 0; i < 4; i++){
		tempflag = SaveRunProgram();
		if(tempflag != 0){//如果失败
			if(i >= 3){
				#ifdef DEBUG_
				printf("备份失败,代码号=%d\r\n",tempflag);
				#endif
				return;
			}
		}
		else{
			break;
		}
	}
	tempVaule = IS_ROLLBACK_FLAG;//清更新标志
	FM25VxxWriteData(ADDR_FRAM_UPDATE+1, NULL, &tempVaule, 1);
	tempflag = updateProgramToFlash();
	if(tempflag != 0x00){
		#ifdef DEBUG_
		printf("写flash失败,代码号=%d\r\n",tempflag);
		#endif
		return;
	}
	iap_load_app(IAPSTARTADDR,IAPJUMP_UPDATA);
}

/**
  * @Description: 备份程序
  * @param:  void
  * @return: 0：成功  1：失败
  * @updata: 
  */
uint8_t SaveRunProgram(void)
{
	u8 res=0;
	uint32_t i=0,j=0;
	uint32_t tempFlashData;
	res=f_mkfs("0:",0,4096);//格式化FLASH
	if(res==0){
		f_setlabel((const TCHAR *)"1:ALIENTEK");	//设置Flash磁盘的名字为：ALIENTEK
	}
	else 
		delay_ms(1000);
	res = f_open(&file,(TCHAR *)run_bin, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	if(res != FR_OK ){
		return 0x01;
	}
	j=0;
	for(i = IAPSTARTADDR; i < IAPENDADDR; i += 4){//备份程序
        WDG_FEED();
		tempFlashData = STMFLASH_ReadWord(i);
		readFileBuff[j] = tempFlashData;/* 缓冲 4096字节 然后写入文件 */
		j++;
		if(j == READEACHSIZE/4){//4096
			res = f_write(&file,readFileBuff,READEACHSIZE,(UINT *)&bw);
			if(res != FR_OK){
				f_close(&file);	
				return 0x02;
			}
			if(bw != READEACHSIZE){
				GetFatfsInfo();
				f_close(&file);	
				return 0x03;
			}
			j=0;
			#ifdef DEBUG_
			printf("Saving......0%X\r\n",i);
			#endif
		}
	}
	f_close(&file);
	#ifdef DEBUG_
	printf(" SaveRunProgramOK\r\n");
	#endif
	return 0x00;
}
/**
  * @Description: 程序运行异常处理
  * @param:  void
  * @return: void
  * @updata: 
  */
void programRunError(void)
{
	uint8_t tempReturn;
	tempReturn = writeProgramToFlash(run_bin);
	if(tempReturn != 0x00){
		#ifdef DEBUG_
		printf("writeProgramToFlashError 错误码:%d\r\n",tempReturn);
		#endif
		return;
	}
	iap_load_app(IAPSTARTADDR,IAPJUMP_ROLLBACK);
}

/**
  * @Description: 写程序到flash
  * @param:  fileName 程序文件名
  * @return: 0 成功   !0 失败
  * @updata: 
  */
uint8_t writeProgramToFlash(uint8_t *fileName)
{
	uint32_t i,j;
	uint32_t tempcount=0;
	uint8_t tempflag=0;
	uint8_t nbSectors=0;
	uint8_t res = 0;
	
	res = f_open(&file, (TCHAR *)fileName, FA_READ);
	#ifdef DEBUG_
	printf("writeProgramToFlash文件名:%s\r\n",fileName);
	#endif
	if(res != FR_OK){
		#ifdef DEBUG_
		printf("open失败 失败号 = %d\r\n",res);
		#endif
		return 0x01;
	}
	FileSize = f_size(&file);
	nbSectors = (FileSize/1024/128) + 1;
	if(nbSectors > MAXBSECTOR){
		nbSectors = MAXBSECTOR;
		FileSize = nbSectors * 128 * 1024;
	}
	FlashEraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;//擦除类型，扇区擦除 
	FlashEraseInit.Sector = BEGINSECTOR;//要擦除的扇区
	FlashEraseInit.NbSectors = 1;//一次只擦除一个扇区
	FlashEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;//电压范围，VCC=2.7~3.6V之间
    HAL_FLASH_Unlock();
    for(i=0;i<nbSectors;i++){
        FlashEraseInit.Sector = (BEGINSECTOR+i);//要擦除的扇区
        FlashStatus = HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError);//擦除扇区
        if(FlashStatus != HAL_OK){
            printf("FLASHEx_Erase %X\r\n",FlashStatus);
			break;
        }
        WDG_FEED();
        printf("FLASHEx_Erase %X\r\n",FlashStatus);
    }
	if(FlashStatus != HAL_OK){
		f_close(&file);
		return 0x03;
	}
	/*烧写程序*/
	for(i = 0;i < FileSize;i += READEACHSIZE){
		res=f_read(&file,readFileBuff,READEACHSIZE,(UINT*)&br);
		#ifdef DEBUG_
		printf("writing...... %X\r\n",IAPSTARTADDR + i);
		#endif
		if(res == FR_OK){
			if(br%4 == 0){
				br = br/4;		
			}
			else{
				br = br/4 +1;
			}
			for(j =0 ;j < br; j++){
				FlashStatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, IAPSTARTADDR + j*4 + i,(uint64_t)(readFileBuff[j]));
				if(FlashStatus != HAL_OK){
					tempflag = 1;
					break;
				}
			}
		}
		if(tempflag){
			break;
		}
		tempcount++;
	}
	if(i/READEACHSIZE != tempcount){//更新失败
		f_close(&file);	
		return 0x04;
	}
	f_close(&file);	
	#ifdef DEBUG_
	printf("writeProgramToFlashOK\r\n");
	#endif
	return 0x00;
}

/**
  * @Description: 更新程序到flash
  * @param:  None
  * @return: None
  * @updata: 
  */
uint8_t updateProgramToFlash(void)
{
	uint32_t j;
	uint8_t nbSectors = 0;

	FileSize = *((uint32_t *)(SDRAMSTARTADDR));
	#ifdef DEBUG_
	printf("FileSize%X\r\n",FileSize);
	#endif
	nbSectors = (FileSize/1024/128) + 1;
	if(nbSectors > MAXBSECTOR){
		nbSectors = MAXBSECTOR;
	}
	FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除 
	FlashEraseInit.Sector=MAXBSECTOR;   					//要擦除的扇区
	FlashEraseInit.NbSectors=nbSectors;                     //一次只擦除一个扇区
	FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间
    HAL_FLASH_Unlock();
    WDG_FEED();//喂狗
	FlashStatus = HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError);//擦除扇区
	if(FlashStatus != HAL_OK){
		return 0x03;
	}
	/*烧写程序*/
	for(j = 0;j < FileSize + 4; j+=4){
        WDG_FEED();
		FlashStatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,IAPSTARTADDR+j,\
            (uint64_t)(*((uint32_t *)(SDRAMSTARTADDR + j + 4))));
		if(FlashStatus != HAL_OK){
			#ifdef DEBUG_
			printf("SDRAM->FLASH ERROR\r\n");
			#endif
			return 0x04;
		}
		#ifdef DEBUG_
		//printf("updateing:%X  %X\r\n",(uint32_t *)(0XC0400000 + j),*((uint32_t *)(0XC0400000 + j)));
		#endif
	}	
	#ifdef DEBUG_
	printf("writeProgramToFlashOK\r\n");
	#endif
	iap_load_app(IAPSTARTADDR,IAPJUMP_UPDATA);
	return 0x00;
}
/**
  * @Description: 读flash固定地址的内容
  * @param:  faddr 地址 
  * @return: 此地址值
  * @updata: 
  */
u32 STMFLASH_ReadWord(u32 faddr)
{
	return *(vu32*)faddr; 
}
/**
  * @Description: 升级跳转程序
  * @param:  appxaddr  要跳转的地址
  * @return: NONE
  * @updata: 
  */
void iap_load_app(u32 appxaddr,uint8_t useID)
{
	uint32_t i;
	uint8_t tempVaule = 0xFF;
	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	//检查栈顶地址是否合法.
	{ 
		/*写标志状态*/
		if(useID == IAPJUMP_FIRSTRUN){//第一次上电
			
		}
		else if(useID == IAPJUMP_UPDATA){//更新
			tempVaule = IS_UPDATAFINISH_FLAG;
			FM25VxxWriteData(ADDR_FRAM_UPDATE+1,NULL,&tempVaule,1);
		}
		else if(useID == IAPJUMP_RUNOK){//无更新，运行正常
			
		}
		else if(useID == IAPJUMP_ROLLBACK){//回退
			tempVaule = IS_ROLLBACK_FLAG;
			FM25VxxWriteData(ADDR_FRAM_UPDATE + 1, NULL, &tempVaule, 1);
		}
		for(i=0;i<0x2000000;i++){
			*(vu8*)(0XC0000000+i)=0xAA;
		}	
		#ifdef DEBUG_
		printf("跳转成功,addr = 0%x,MSP = %x,flag = %d\r\n",appxaddr,(*(vu32*)appxaddr),tempVaule);	
		#endif
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		__set_MSP(*(vu32*)appxaddr);				//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		
		INTX_DISABLE();
        WDG_FEED();
		jump2app();									//跳转到APP.
	}
	tempVaule = IS_UPDATAFAULT_FLAG;
	FM25VxxWriteData(ADDR_FRAM_UPDATE+1, NULL, &tempVaule, 1);
	#ifdef DEBUG_
	printf("跳转失败addr = 0%x,MSP = %x\r\n",appxaddr,(*(vu32*)appxaddr));
	#endif
}
/**
  * @Description: 读取在线升级相应的状态标志
  * @param:  NONE
  * @return: 0:成功   1：失败
  * @updata: 
  */
uint8_t readProgramFlag(void)
{
	uint8_t tempreadbuff[2];
	uint8_t tempflag;
	tempflag = FM25VxxReadData(ADDR_FRAM_UPDATE, NULL, tempreadbuff, 2);
	if(tempflag == TRUE){
		ProgramState.firstRun = tempreadbuff[0];
		ProgramState.currentState = tempreadbuff[1];
		#ifdef DEBUG_
		printf("firstrun:%X   currentState:%X\r\n",ProgramState.firstRun,ProgramState.currentState);
		#endif
		return 0;
	}
	return 1;	
}

/**
  * @Description: 设置标志位为首次上电
  * @param:  NONE
  * @return: 0:成功  1：失败
  * @updata: 
  */
uint8_t SetFlagIsFirstRun(void)
{
	uint8_t tempflag, i;
	uint8_t tempVaule = 0xAA;
	for(i = 0;i < 3; i++){
		tempflag = FM25VxxWriteData(ADDR_FRAM_UPDATE, NULL, &tempVaule, 1);
		if(tempflag == TRUE){
			#ifdef DEBUG_
			printf("修改第一次上电成功\r\n");
			#endif
			return 0;
		}
	}
	return 1;
}

/**
  * @Description: 开机计数
  * @param:  NONE
  * @return: NONE
  * @updata: 
  */
void PowerOnCount(void)
{
	uint8_t powerOnIs;
	uint8_t tempflag;
	
	tempflag = FM25VxxReadData(ADDR_FRAM_UPDATE + POWERON_OFFSET, NULL, &powerOnIs, 1);
	if(tempflag == TRUE){
		powerOnIs ++;
		FM25VxxWriteData(ADDR_FRAM_UPDATE + POWERON_OFFSET,NULL,&powerOnIs,1);
		#ifdef DEBUG_
		printf("开机次数: %d \r\n",powerOnIs);
		#endif
	}
	else{
		#ifdef DEBUG_
		printf("获取开机次数失败 \r\n");
		#endif
	}	
}

uint8_t Rxbuffer[1024*3];
uint32_t RxBuffersize;
uint8_t rxfinish = 0;
void testIAP(void)
{
	u8 res;
	uint8_t testbuffer[4]={0x55,0x02,0x03,0x04};
	uint8_t step =0;
	while(rxfinish == 0);
	if(Rxbuffer[2]==0x03){
		return;
	}
	if(step == 0){
		testbuffer[0] = Rxbuffer[0];
		testbuffer[1] = Rxbuffer[1];
		res = FM25VxxWriteData(ADDR_FRAM_UPDATE, NULL, testbuffer, 2);
		if(res == TRUE){
			#ifdef DEBUG_
			printf("FM25写入成功\r\n");
			#endif
		}
		if(testbuffer[1] == IS_WILLUPDATA_FLAG){
			#ifdef DEBUG_
			printf("等待接收程序\r\n");
			#endif
			step = 1;
		}
		else{
			step = 2;
		}
		rxfinish = 0;
		RxBuffersize = 0;
	}
}
