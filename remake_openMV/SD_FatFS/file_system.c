/*
 * SD_card.c
 *
 *  Created on: Dec 17, 2023
 *      Author: Do
 */

//	函数：FatFs_Check
//	功能：进行FatFs文件系统的挂载
//
#include "file_system.h"
#include "fatfs.h"
#include "lcd_display.h"

FATFS fs;//磁盘工作区.
FRESULT MyFile_Res;
FIL file;   /* 文件对象 */
UINT MyFile_Num;
BYTE ReadBuffer[100] = {0};
BYTE WriteBuffer[] = "STM32H7 SD test fatfs"; // 要写入的数据

void FatFs_read_Check(void) // 判断FatFs是否挂载成功
{
	//在外部 SD 卡挂载文件系统，文件系统挂载时会对 SD 卡初始化
	MyFile_Res = f_mount(&fs, "0:", 1); //	挂载SD卡

	if (MyFile_Res == FR_OK) // 判断是否挂载成功
	{
		f_setlabel("SD");  //修改磁盘标签
		MyFile_Res = f_open(&file, "0:SD_test.txt", FA_OPEN_EXISTING | FA_READ);
		if (MyFile_Res == FR_OK)
		{
		 MyFile_Res = f_read(&file, ReadBuffer, 100, &MyFile_Num); // 读取文件
		 LCD_ShowString(0, 0, 160, 128, 16, ReadBuffer); //lcd上显示出来读取的文件内容
		 f_close(&file);
		}
	}

}

void FatFs_write_Check(void)
{

  MyFile_Res = f_mount(&fs, "0:", 1);

  MyFile_Res = f_open(&file, "0:write_test.txt",FA_CREATE_ALWAYS | FA_WRITE); // 打开文件，若不存在则创建该文件
  if (MyFile_Res == FR_OK)
  {
     MyFile_Res = f_write(&file, WriteBuffer,sizeof(WriteBuffer), &MyFile_Num); // 向文件写入数据
     if (MyFile_Res == FR_OK)
     {
    	 LCD_ShowString(0, 32, 160, 128, 16, "write ok"); //lcd上显示出来读取的文件内容
     }
  }
  else if(MyFile_Res == FR_INVALID_NAME)
  {
	  LCD_ShowString(0, 32, 160, 128, 16, "FR_INVALID_NAME"); //lcd上显示出来读取的文件内容
  }
  f_close(&file);

  MyFile_Res = f_open(&file, "0:write_test.txt", FA_OPEN_EXISTING | FA_READ);
  if (MyFile_Res == FR_OK)
	{
	 MyFile_Res = f_read(&file, ReadBuffer, 100, &MyFile_Num); // 读取文件
	 LCD_ShowString(0, 48, 160, 128, 16, ReadBuffer); //lcd上显示出来读取的文件内容
	 f_close(&file);
	}
}

