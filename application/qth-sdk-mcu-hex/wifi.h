

#ifndef __WIFI_H_
#define __WIFI_H_

//#include "stm32f1xx.h"
#include "stdio.h"
#include "string.h"
#include "protocol.h"
#include "system.h"
#include "mcu_api.h"
#include "stdint.h"
#include "stdlib.h"

#define MCU_WIFI_SDK_VER "v1.0.4 2023/10/10"


//=============================================================================
//定义常量
//如果编译发生错误: #40: expected an identifier  DISABLE = 0, 类似这样的错误提示，可以包含头文件 #include "stm32f1xx.h" 来解决
//=============================================================================
#ifndef TRUE
#define      TRUE                1
#endif

#ifndef FALSE
#define         FALSE            0
#endif

#ifndef NULL
#define         NULL             ((void *) 0)
#endif

#ifndef SUCCESS
#define         SUCCESS          1
#endif

#ifndef ERROR
#define         ERROR            0
#endif

#ifndef INVALID
#define         INVALID          0xFF
#endif

#ifndef ENABLE
#define         ENABLE           1
#endif

#ifndef DISABLE
#define         DISABLE          0
#endif
//=============================================================================
//tsl 数据点类型
//=============================================================================
#define         TSL_TYPE_RAW                     0x00        //RAW 类型
#define         TSL_TYPE_BOOL                    0x01        //bool 类型
#define         TSL_TYPE_VALUE                   0x02        //int 或者枚举 类型
#define         TSL_TYPE_STRING                  0x03        //string 类型
#define         TSL_TYPE_BITMAP                  0x05        //fault 类型
#define         TSL_TYPE_DOUBLE                  0x11        //double 类型
#define         TSL_TYPE_STRUCT                  0x12        //struct/array 类型
//=============================================================================
//WIFI工作状态
//=============================================================================
#define         WIFI_CONFIG_STATE              0x00
#define         WIFI_NOT_CONNECTED              0x02
#define         WIFI_CONNECTED                  0x03
#define         WIFI_CONN_CLOUD                 0x04
#define         WIFI_LOW_POWER                  0x05
#define         WIFI_SATE_UNKNOW                0xff


//=============================================================================
//wifi复位状态
//=============================================================================
#define         RESET_WIFI_ERROR                0
#define         RESET_WIFI_SUCCESS              1


//=============================================================================
//MCU固件升级状态
//=============================================================================
#define         FIRM_STATE_UN_SUPPORT           0x00                            //不支持 MCU 升级
#define         FIRM_STATE_WIFI_UN_READY        0x01                            //模块未就绪
#define         FIRM_STATE_GET_ERROR            0x02                            //云端升级信息查询失败
#define         FIRM_STATE_NO                   0x03                            //无需升级（云端无更新版本）
#define         FIRM_STATE_START                0x04                            //需升级，等待模块发起升级操作




//=============================================================================
//下发命令
//=============================================================================
typedef struct {
  unsigned short tsl_id;                              //tsl序号
  unsigned char tsl_type;                            //tsl类型
} DOWNLOAD_CMD_S;

#endif
