#ifndef __PROTOCOL_H_
#define __PROTOCOL_H_


/******************************************************************************
                           用户相关信息配置
******************************************************************************/
/******************************************************************************
                       1:修改产品信息
******************************************************************************/
#define PK_PS "p11nUJ_MGo3dldUT3NQcC8w"    //在开发者中心创建产品生成的生成的产品唯一标识和对应的密钥，两者通过下划线连接

#define MCU_VER "1.0.0"         //用户的软件版本,用于MCU固件升级,MCU升级版本需修改



/******************************************************************************
                          2:MCU是否需要支固件升级
如需要支持MCU固件升级,请开启该宏
MCU可调用mcu_api.c文件内的mcu_firm_update_query()函数获取当前MCU固件更新情况
                        ********WARNING!!!**********
当前接收缓冲区为关闭固件更新功能的大小,固件升级包可选择，默认256字节大小
如需要开启该功能,串口接收缓冲区会变大
******************************************************************************/
//#define         SUPPORT_MCU_FIRM_UPDATE                 //开启MCU固件升级功能(默认关闭)
/*  Firmware package size selection  */
#ifdef SUPPORT_MCU_FIRM_UPDATE
#define PACKAGE_SIZE                   0        //包大小为256字节
//#define PACKAGE_SIZE                   1        //包大小为512字节
//#define PACKAGE_SIZE                   2        //包大小为1024字节
#endif
/******************************************************************************
                         3:定义收发缓存:
                    如当前使用MCU的RAM不够,可修改为24
******************************************************************************/
#ifndef SUPPORT_MCU_FIRM_UPDATE
#define WIFI_UART_RECV_BUF_LMT          16              //串口数据接收缓存区大小,如MCU的RAM不够,可缩小
#define WIFI_DATA_PROCESS_LMT           1024              //串口数据处理缓存区大小,根据用户TSL数据大小量定,必须大于24
#else
#define WIFI_UART_RECV_BUF_LMT          128             //串口数据接收缓存区大小,如MCU的RAM不够,可缩小

//请在此处选择合适的串口数据处理缓存大小（根据上面MCU固件升级包选择的大小选择开启多大的缓存）
#define WIFI_DATA_PROCESS_LMT           300             //串口数据处理缓存大小,如需MCU固件升级,若单包大小选择256,则缓存必须大于260
//#define WIFI_DATA_PROCESS_LMT           600             //串口数据处理缓存大小,如需MCU固件升级,若单包大小选择512,则缓存必须大于520
//#define WIFI_DATA_PROCESS_LMT           1200            //串口数据处理缓存大小,如需MCU固件升级,若单包大小选择1024,则缓存必须大于1030

#endif

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

//RAW类型物模型
//备注:
// #define TSLID_TEST_RAW 4

//BOOL类型物模型
//备注:
#define TSLID_TEST_BOOL 1

//INT类型物模型
//备注：
#define TSLID_TEST_VALUE 3

//STRING 类型物模型
//备注:
#define TSLID_TEST_STRING 5

//FAULT类型(特殊结构的物模型)
//备注:
#define TSLID_TEST_FAULT 6

//DOUBLE 类型物模型
//备注:
#define TSLID_TEST_DOUBLE 7

#define TSLID_TEST_STRUCT 2

#if defined(TSLID_TEST_STRUCT)
// #define TSLID_TEST_STRUCT_SUB_RAW 5
#define TSLID_TEST_STRUCT_SUB_BOOL 1
#define TSLID_TEST_STRUCT_SUB_VALUE 2
#define TSLID_TEST_STRUCT_SUB_DOUBLE 3
#define TSLID_TEST_STRUCT_SUB_STRING 4
#endif

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



#ifdef SUPPORT_MCU_FIRM_UPDATE
/**
 * @brief  升级包大小选择
 * @param[in] {package_sz} 升级包大小
 * @ref           0x00: 256byte (默认)
 * @ref           0x01: 512byte
 * @ref           0x02: 1024byte
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void upgrade_package_choose(unsigned char package_sz);

/**
 * @brief  MCU进入固件升级模式
 * @param[in] {value} 固件缓冲区
 * @param[in] {position} 当前数据包在于固件位置
 * @param[in] {length} 当前固件包长度(固件包长度为0时,表示固件包发送完成)
 * @return Null
 * @note   MCU需要自行实现该功能
 */
unsigned char mcu_firm_update_handle(const unsigned char value[],unsigned long position,unsigned short length);
#endif

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
