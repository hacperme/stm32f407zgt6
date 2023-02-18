#ifndef __PROTOCOL_H_
#define __PROTOCOL_H_

#include <stdbool.h>

/******************************************************************************
                           用户相关信息配置
******************************************************************************/
/******************************************************************************
                       1:修改产品信息
******************************************************************************/
#define PK_PS "p11pjq_N2ZDQmtRa0pNQktt"    //在开发者中心创建产品生成的生成的产品唯一标识和对应的密钥，两者通过下划线连接

#define MCU_VER "1.0.0"         //用户的软件版本,用于MCU固件升级,MCU升级版本需修改



/******************************************************************************
                         3:定义收发缓存:
                    如当前使用MCU的RAM不够,可修改为24
******************************************************************************/

#define WIFI_UART_RECV_BUF_LMT          1024              //串口数据接收缓存区大小,如MCU的RAM不够,可缩小
#define WIFI_DATA_PROCESS_LMT           1024              //串口数据处理缓存区大小,根据用户TSL数据大小量定,必须大于24


#define WIFIR_UART_SEND_BUF_LMT         1024              //根据用户需要发送的物模型数据大小量定,必须大于48


/******************************************************************************
                      4:MCU是否需要支持校时功能
如需要请开启该宏,并在Protocol.c文件内mcu_write_rtctime实现代码
mcu_write_rtctime内部有#err提示,完成函数后请删除该#err
mcu在wifi模块正确联网后可调用mcu_get_system_time()函数发起校时功能
******************************************************************************/
#define         SUPPORT_MCU_RTC_CHECK                //开启校时功能

/******************************************************************************
                      5:MCU是否需要支持wifi功能测试
如需要请开启该宏,并且mcu在需要wifi功能测试处调用mcu_api.c文件内mcu_start_wifitest
并在protocol.c文件wifi_test_result函数内查看测试结果,
wifi_test_result内部有#err提示,完成函数后请删除该#err
******************************************************************************/
#define         WIFI_TEST_ENABLE                //开启WIFI产测功能（扫描指定路由）

/******************************************************************************
                      6:MCU是否需要开启获取当前WIFI联网状态功能
如需要请开启该宏,并且mcu在需要获取当前WIFI联网状态处调用mcu_api.c文件内mcu_get_wifi_connect_status
并在protocol.c文件wifi_test_result函数内查看结果,
wifi_test_result内部有#err提示,完成函数后请删除该#err
******************************************************************************/
#define         GET_WIFI_STATUS_ENABLE                  //开启获取当前WIFI联网状态功能

/******************************************************************************
                      7:MCU是否需要开启获取模块mac地址功能
如需要请开启该宏,并且mcu在需要获取模块mac地址处调用mcu_api.c文件内mcu_get_module_mac
并在protocol.c文件mcu_get_mac函数内查看结果,
mcu_get_mac内部有#err提示,完成函数后请删除该#err
******************************************************************************/
#define         GET_MODULE_MAC_ENABLE                   //开启获取模块mac地址功能

/******************************************************************************
                      8:MCU是否需要支持获取格林时间功能
如需要请开启该宏,并且mcu在需要获取格林时间处调用mcu_api.c文件内mcu_get_green_time
并在protocol.c文件mcu_get_greentime函数内查看结果,
mcu_get_greentime内部有#err提示,完成函数后请删除该#err
mcu在wifi模块正确联网后可调用mcu_get_green_time()函数发起校时功能
******************************************************************************/
#define         SUPPORT_GREEN_TIME                //开启格林时间功能

/******************************************************************************
                      9:MCU是否需要开启同步状态上报功能
1) 此命令为同步指令，MCU 数据状态上报后，需要等待模块返回结果；
2) 每次发送模组都会有响应，WIFI 模组未响应前不可多次上报；
3) 网络不好，数据难以及时上报时，模块会在 5 后返回失败，MCU 需要等待大于 5 秒。
******************************************************************************/
//#define         MCU_TSL_UPLOAD_SYN                   //开启同步状态上报功能






/******************************************************************************
                        10:物模型序列号重新定义
                        此处数据请和移远平台创建的功能点一致      
******************************************************************************/
// #error "请将功能点ID与平台上设置的保持一致,并删除该行"


// 电池电量
#define TSLID_BATTERY_PERCENTAGE_INT 1
// 剩余可用时长
#define TSLID_REMAIN_TIME_INT 2
// 剩余充电时长
#define TSLID_REMAIN_CHARGING_TIME_INT 3
// 总输入功率
#define TSLID_TOTAL_INPUT_POWER_INT 4
// 总输出功率
#define TSLID_TOTAL_OUTPUT_POWER_INT 5
// AC_信息
#define TSLID_AC_INFO_STRUCT 6
#if defined(TSLID_AC_INFO_STRUCT)
     // AC开关
     #define TSLID_AC_INFO_STRUCT_AC_SWITCH_BOOL 1
     // AC1输出功率
     #define TSLID_AC_INFO_STRUCT_AC1_OUTPUT_INT 2
     // AC1输出电压
     #define TSLID_AC_INFO_STRUCT_AC1_OUTPUT_VOLTAGE_INT 3
     // AC1输出电流
     #define TSLID_AC_INFO_STRUCT_AC1_OUTPUT_CURRENT_INT 4
     // AC2输出功率
     #define TSLID_AC_INFO_STRUCT_AC2_OUTPUT_INT 5
     // AC2输出电压
     #define TSLID_AC_INFO_STRUCT_AC2_OUTPUT_VOLTAGE_INT 6
     // AC2输出电流
     #define TSLID_AC_INFO_STRUCT_AC2_OUTPUT_CURRENT_INT 7
#endif

// USB信息
#define TSLID_USB_DATA_STRUCT 7
#if defined(TSLID_USB_DATA_STRUCT)
     // USB开关
     #define TSLID_USB_DATA_STRUCT_USB_SWITCH_BOOL 1
     // USB1输出功率
     #define TSLID_USB_DATA_STRUCT_USB1_OUTPUT_INT 2
     // USB1输出电压
     #define TSLID_USB_DATA_STRUCT_USB1_OUTPUT_VOLTAGE_INT 3
     // USB1输出电流
     #define TSLID_USB_DATA_STRUCT_USB1_OUTPUT_CURRENT_INT 4
     // USB2输出功率
     #define TSLID_USB_DATA_STRUCT_USB2_OUTPUT_INT 5
     // USB2输出电压
     #define TSLID_USB_DATA_STRUCT_USB2_OUTPUT_VOLTAGE_INT 6
     // USB2输出电流
     #define TSLID_USB_DATA_STRUCT_USB2_OUTPUT_CURRENT_INT 7
     // USB3输出功率
     #define TSLID_USB_DATA_STRUCT_USB3_OUTPUT_INT 8
     // USB3输出电压
     #define TSLID_USB_DATA_STRUCT_USB3_OUTPUT_VOLTAGE_INT 9
     // USB3输出电流
     #define TSLID_USB_DATA_STRUCT_USB3_OUTPUT_CURRENT_INT 10
     // USB4输出功率
     #define TSLID_USB_DATA_STRUCT_USB4_OUTPUT_INT 11
     // USB4输出电压
     #define TSLID_USB_DATA_STRUCT_USB4_OUTPUT_VOLTAGE_INT 12
     // USB4输出电流
     #define TSLID_USB_DATA_STRUCT_USB4_OUTPUT_CURRENT_INT 13
#endif

// TypeC信息
#define TSLID_TYPEC_DATA_STRUCT 8
#if defined(TSLID_TYPEC_DATA_STRUCT)
     // TypeC开关
     #define TSLID_TYPEC_DATA_STRUCT_TYPEC_SWITCH_BOOL 1
     // TypeC1 输出功率
     #define TSLID_TYPEC_DATA_STRUCT_TYPEC1_OUTPUT_INT 2
     // TypeC1 输出电压
     #define TSLID_TYPEC_DATA_STRUCT_TYPEC1_OUTPUT_VOLTAGE_INT 3
     // TypeC1 输出电流
     #define TSLID_TYPEC_DATA_STRUCT_TYPEC1_OUTPUT_CURRENT_INT 4
     // TypeC2 输出功率
     #define TSLID_TYPEC_DATA_STRUCT_TYPEC2_OUTPUT_INT 5
     // TypeC2 输出电压
     #define TSLID_TYPEC_DATA_STRUCT_TYPEC2_OUTPUT_VOLTAGE_INT 6
     // TypeC2 输出电流
     #define TSLID_TYPEC_DATA_STRUCT_TYPEC2_OUTPUT_CURRENT_INT 7

#endif

// DC信息
#define TSLID_DC_DATA_STRUCT 9
#if defined(TSLID_DC_DATA_STRUCT)
     // DC开关
     #define TSLID_DC_DATA_STRUCT_DC_SWITCH_BOOL 1
     // CAR1输出功率
     #define TSLID_DC_DATA_STRUCT_CAR1_OUTPUT_INT 2
     // CAR1输出电压
     #define TSLID_DC_DATA_STRUCT_CAR1_OUTPUT_VOLTAGE_INT 3
     // CAR1输出电流
     #define TSLID_DC_DATA_STRUCT_CAR1_OUTPUT_CURRENT_INT 4
     // CAR2输出功率
     #define TSLID_DC_DATA_STRUCT_CAR2_OUTPUT_INT 5
     // CAR2输出电压
     #define TSLID_DC_DATA_STRUCT_CAR2_OUTPUT_VOLTAGE_INT 6
     // CAR2输出电流
     #define TSLID_DC_DATA_STRUCT_CAR2_OUTPUT_CURRENT_INT 7
     // DC12V1输出功率
     #define TSLID_DC_DATA_STRUCT_DC12V1_OUTPUT_INT 8
     // DC12V1输出电压
     #define TSLID_DC_DATA_STRUCT_DC12V1_OUTPUT_VOLTAGE_INT 9
     // DC12V1输出电流
     #define TSLID_DC_DATA_STRUCT_DC12V1_OUTPUT_CURRENT_INT 10
     // DC12V2输出功率
     #define TSLID_DC_DATA_STRUCT_DC12V2_OUTPUT_INT 11
     // DC12V2输出电压
     #define TSLID_DC_DATA_STRUCT_DC12V2_OUTPUT_VOLTAGE_INT 12
     // DC12V2输出电流
     #define TSLID_DC_DATA_STRUCT_DC12V2_OUTPUT_CURRENT_INT 13

#endif


// LED指示灯状态
#define TSLID_LED_STATUS_INT 10
// AC充电输入功率
#define TSLID_AC_INPUT_INT 11
// DC充电输入功率
#define TSLID_DC_INPUT_INT 12
// USB充电输入功率
#define TSLID_USB_INPUT_INT 13
// 设备温度
#define TSLID_TEMP_INT 14
// 设备状态
#define TSLID_DEVICE_STATUS_INT 15
// 设备待机时间
#define TSLID_STANDBY_TIME_INT 16
// 息屏时间
#define TSLID_SCREEN_TIME_INT 17
// 交流待机时间
#define TSLID_AC_STANDBY_TIME_INT 18
// 直流待机时间
#define TSLID_DC_STANDBY_TIME_INT 19
// AC充电功率上限
#define TSLID_AC_CHARGING_POWER_LIMIT_INT 20
// 蜂鸣器设置
#define TSLID_BEEP_INT 21
// 屏幕亮度
#define TSLID_SCREEN_BRIGHTNESS_INT 22
// 设备工作模式
#define TSLID_DEVICE_WORK_MODE_INT 23

// 定时开关机
#define TSLID_TIMING_ARRARY 24
#if defined(TSLID_TIMING_ARRARY) // 嵌套struct类型
     // 时间
     #define TSLID_TIMING_ARRARY_TIME_INT 1
     // 执行动作
     #define TSLID_TIMING_ARRARY_ACTION_BOOL 2
     // 动作状态
     #define TSLID_TIMING_ARRARY_ACTION_STATUS_BOOL 3
#endif

// 设备序列号
#define TSLID_DEVICE_MODEL_STRING 25
// 蓝牙Mac地址
#define TSLID_BLUETOOTH_MAC_STRING 26

typedef struct
{
     // 时间
     int time;
     // 执行动作
     bool action;
     // 动作状态
     bool action_status;
} cycle_timing_t;

typedef struct tsl_demo_st
{
     // 电池电量
     int battery_percentage;
     // 剩余可用时长
     int remain_time;
     // 剩余充电时长
     int remain_charging_time;
     // 总输入功率
     int total_input_power;
     // 总输出功率
     int total_output_power;
     // AC_信息
     struct
     {
          // AC开关
          bool ac_switch;
          // AC1输出功率
          int ac1_output;
          // AC1输出电压
          int ac1_output_voltage;
          // AC1输出电流
          int ac1_output_current;
          // AC2输出功率
          int ac2_output;
          // AC2输出电压
          int ac2_output_voltage;
          // AC2输出电流
          int ac2_output_current;
     } ac_info;

     // USB信息
     struct
     {
          // USB开关
          bool usb_switch;

          // USB1输出功率
          int usb1_output;

          // USB1输出电压
          int usb1_output_voltage;

          // USB1输出电流
          int usb1_output_current;

          // USB2输出功率
          int usb2_output;

          // USB2输出电压
          int usb2_output_voltage;

          // USB2输出电流
          int usb2_output_current;

          // USB3输出功率
          int usb3_output;

          // USB3输出电压
          int usb3_output_voltage;
          // USB3输出电流
          int usb3_output_current;

          // USB4输出功率
          int usb4_output;

          // USB4输出电压
          int usb4_output_voltage;

          // USB4输出电流
          int usb4_output_current;

     } usb_data;

     // TypeC信息
     struct
     {
          // TypeC开关
          bool typec_switch;
          // TypeC1输出功率
          int typec1_output;
          // TypeC1输出电压
          int typec1_output_voltage;
          // TypeC1输出电流
          int typec1_output_current;
          // TypeC2输出功率
          int typec2_output;
          // TypeC2输出电压
          int typec2_output_voltage;
          // TypeC2输出电流
          int typec2_output_current;
     } typec_data;
     // DC信息

     struct
     {
          // DC开关
          bool dc_switch;

          // CAR1输出功率
          int car1_output;

          // CAR1输出电压
          int car1_output_voltage;

          // CAR1输出电流
          int car1_output_current;

          // CAR2输出功率
          int car2_output;

          // CAR2输出电压
          int car2_output_voltage;

          // CAR2输出电流
          int car2_output_current;

          // DC12V1输出功率
          int dc12v1_output;

          // DC12V1输出电压
          int dc12v1_output_voltage;

          // DC12V1输出电流
          int dc12v1_output_current;

          // DC12V2输出功率
          int dc12v2_output;

          // DC12V2输出电压
          int dc12v2_output_voltage;

          // DC12V2输出电流
          int dc12v2_output_current;

     } dc_data;

     // LED指示灯状态
     int led_status;
     // AC充电输入功率
     int ac_input_power;

     // DC充电输入功率
     int dc_input_power;

     // USB充电输入功率
     int usb_input_power;
     // 设备温度
     int temp;
     // 设备状态
     int device_status;
     // 设备待机时间
     int standby_time;
     // 息屏时间
     int screen_time;
     // 交流待机时间
     int ac_standby_time;
     // 直流待机时间
     int dc_standby_time;
     // AC充电功率上限
     int ac_power_limit;
     // 蜂鸣器设置
     int beep;
     // 屏幕亮度
     int screen_brightness;
     // 设备工作模式
     int device_mode;
     // 定时开关机
     cycle_timing_t timing[10];
     // 设备序列号
     const char *device_sn;
     // 蓝牙Mac地址
     const char *bluetooth_mac;
} tsl_demo_t;

/**
 * @brief  串口发送数据
 * @param[in] {value} 串口要发送的1字节数据
 * @return Null
 */
void uart_transmit_output(unsigned char value);

/**
 * @brief  系统所有tsl点信息上传,实现APP和muc数据同步
 * @param  Null
 * @return Null
 * @note   MCU必须实现该函数内数据上报功能
 */
void all_data_update(void);

/**
 * @brief  tsl下发处理函数
 * @param[in] {tslid} tslid 序号
 * @param[in] {value} tsl数据缓冲区地址
 * @param[in] {length} tsl数据长度
 * @return tsl处理结果
 * -           0(ERROR): 失败
 * -           1(SUCCESS): 成功
 * @note   该函数用户不能修改
 */
unsigned char tsl_download_handle(unsigned short tslid,const unsigned char value[], unsigned short length);

/**
 * @brief  获取所有tsl命令总和
 * @param[in] Null
 * @return 下发命令总和
 * @note   该函数用户不能修改
 */
unsigned char get_download_cmd_total(void);


#ifdef SUPPORT_GREEN_TIME
/**
 * @brief  获取到的格林时间
 * @param[in] {time} 获取到的格林时间数据
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void mcu_get_greentime(unsigned char time[]);
#endif

#ifdef SUPPORT_MCU_RTC_CHECK
/**
 * @brief  MCU校对本地RTC时钟
 * @param[in] {time} 获取到的格林时间数据
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void mcu_write_rtctime(unsigned char time[]);
#endif

#ifdef WIFI_TEST_ENABLE
/**
 * @brief  wifi功能测试反馈
 * @param[in] {result} wifi功能测试结果
 * @ref       0: 失败
 * @ref       1: 成功
 * @param[in] {rssi} 测试成功表示wifi信号强度/测试失败表示错误类型
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void wifi_test_result(unsigned char result,unsigned char rssi);
#endif



#ifdef MCU_TSL_UPLOAD_SYN
/**
 * @brief  状态同步上报结果
 * @param[in] {result} 结果
 * @ref       0: 失败
 * @ref       1: 成功
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void get_upload_syn_result(unsigned char result);
#endif

#ifdef GET_WIFI_STATUS_ENABLE
/**
 * @brief  获取 WIFI 状态结果
 * @param[in] {result} 指示 WIFI 工作状态
 * @ref       0x00: wifi状态 1 Wi-Fi 配网状态（蓝牙处于配网状态）
 * @ref       0x02: wifi状态 3 WIFI 已配置但未连上路由器
 * @ref       0x03: wifi状态 4 WIFI 已配置且连上路由器
 * @ref       0x04: wifi状态 5 已连上路由器且连接到云端
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void get_wifi_status(unsigned char result);
#endif



#ifdef GET_MODULE_MAC_ENABLE
/**
 * @brief  获取模块mac结果
 * @param[in] {mac} 模块 MAC 数据
 * @ref       mac[0]: 为是否获取mac成功标志，0x00 表示成功，0x01 表示失败
 * @ref       mac[1]~mac[6]: 当获取 MAC地址标志位如果mac[0]为成功，则表示模块有效的MAC地址
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void mcu_get_mac(unsigned char mac[]);
#endif


#endif
