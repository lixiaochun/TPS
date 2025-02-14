/* api_driver.h -
 *
 * Copyright 2005-2008 suzhou Keda co.
 *
 * description :
 * -------------------------
 * Purpose: for nip api.
 *
 * modification history
 * -------------------------
 * 2006-1-10 zhangyan write first.
 */
#ifndef API_DRIVER_H
#define API_DRIVER_H


/* for RS485 api */
int nip_driver_rs485_get_txsts(int fd, int * mode);
int nip_driver_rs485_set_rxtx(int fd, int mode);
int nip_driver_rs485_set_auto(int fd, int timer, int rate, int timeout);

/* for rawflash api */
#include "api/api_driver/api_rawflash.h"
#include "api/api_driver/part_abi.h"
int copy_to_app_part(char *src, char *dst);
int update_appimage(char *srcfile);

/* for RTC api */
int nip_rtc_to_sys_clock(void);
int nip_rtc_from_sys_clock(void);



/* for hardware info api */
#define MAX_CPU_NAME 128
#define MAX_UBOOT_INFO 64
#define MAX_KERNEL_INFO 128
#define NIP_GET_HARDWARE_INFO		NIP_DRV_BASE + 1

struct hwinfo_getback{
	int  ret;

	char cpuinfo[MAX_CPU_NAME];
	char binfo[MAX_UBOOT_INFO];
	char kinfo[MAX_KERNEL_INFO];
	
	long memory_size;
	long flash_size;
	long cpu_frequency;
	long cpm_frequency;
	long bus_frequency;
	
	int eth_num;
  	int v35_num;
 	int e1_num;	
};

extern int nip_get_hwinfo(struct hwinfo_getback *);
extern void nip_api_info_register();
#endif	/* API_DRIVER_H */

