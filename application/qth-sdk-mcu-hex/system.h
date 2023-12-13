#ifndef __SYSTEM_H_
#define __SYSTEM_H_

#ifdef SYSTEM_GLOBAL
  #define SYSTEM_EXTERN
#else
  #define SYSTEM_EXTERN   extern
#endif

//=============================================================================
//Byte order of the frame
//=============================================================================
#define         HEAD_FIRST                      0
#define         HEAD_SECOND                     1        
#define         PROTOCOL_VERSION                2
#define         FRAME_TYPE                      3
#define         LENGTH_HIGH                     4
#define         LENGTH_LOW                      5
#define         DATA_START                      6

//=============================================================================
//Data frame type
//=============================================================================
#define         HEAT_BEAT_CMD                   0x00                               //心跳包
#define         PRODUCT_INFO_CMD                0x01                               //产品信息
// #define         WORK_MODE_CMD                   2                               //查询MCU 设定的模块工作模式	
#define         WIFI_STATE_CMD                  0x03                               //wifi工作状态	
#define         WIFI_RESET_CMD                  0x04                               //重置wifi
#define         DATA_QUERT_CMD                  0x06                               //命令下发
#define         STATE_UPLOAD_CMD                0x07                               //状态上报	 
#define         STATE_QUERY_CMD                 0x08                               //状态查询   
#define         UPDATE_START_CMD                0x0a                            //升级开始
#define         UPDATE_TRANS_CMD                0x0b                            //升级传输 
#define         MULTI_MCU_UPDATE_START_CMD      0x9a                            //多MCU升级开始                           
#define         MULTI_MCU_UPDATE_TRANS_CMD      0x9b                            //多MCU升级传输
#define         GET_ONLINE_TIME_CMD             0x0c                            //获取系统时间(格林威治时间)
#define         WIFI_TEST_CMD                   0x0e                            //wifi功能测试
#define         GET_LOCAL_TIME_CMD              0x1c                            //获取本地时间

#define         STATE_UPLOAD_SYN_CMD            0x22                            //状态上报（同步）
#define         STATE_UPLOAD_SYN_RECV_CMD       0x23                            //状态上报结果通知（同步）

#define         GET_WIFI_STATUS_CMD             0x2b                            //获取当前wifi联网状态
#define         GET_MAC_CMD                     0x2d                            //获取模块mac
#define         GET_BLE_STATUS_CMD              0x35                            //获取蓝牙连接状态
#define         GET_IP_ADDRESS_CMD              0x90                            //获取模组IP地址


//=============================================================================
#define MCU_RX_VER              0x00                                            //模块发送帧协议版本号
#define MCU_TX_VER              0x03                                            //MCU 发送帧协议版本号(默认)
#define PROTOCOL_HEAD           0x07                                            //固定协议头长度
#define FRAME_FIRST             0x55                                            //帧头第一字节
#define FRAME_SECOND            0xaa                                            //帧头第二字节
//============================================================================= 
SYSTEM_EXTERN volatile unsigned char wifi_data_process_buf[PROTOCOL_HEAD + WIFI_DATA_PROCESS_LMT];     //串口数据处理缓存
SYSTEM_EXTERN volatile unsigned char wifi_uart_rx_buf[PROTOCOL_HEAD + WIFI_UART_RECV_BUF_LMT];         //串口接收缓存
SYSTEM_EXTERN volatile unsigned char wifi_uart_tx_buf[PROTOCOL_HEAD + WIFIR_UART_SEND_BUF_LMT];        //串口发送缓存
//
SYSTEM_EXTERN volatile unsigned char *rx_buf_in;
SYSTEM_EXTERN volatile unsigned char *rx_buf_out;

SYSTEM_EXTERN volatile unsigned char stop_update_flag;                                                 //ENABLE:停止一切数据上传  DISABLE:恢复一切数据上传

SYSTEM_EXTERN volatile unsigned char reset_wifi_flag;                                                  //重置wifi标志(TRUE:成功/FALSE:失败)
SYSTEM_EXTERN volatile unsigned char wifi_work_state;                                                  //wifi模块当前工作状态
SYSTEM_EXTERN volatile unsigned char ble_work_state;                                                   //wifi模块当前蓝牙连接状态

/**
 * @brief  写wifi_uart字节
 * @param[in] {dest} 缓存区其实地址
 * @param[in] {byte} 写入字节值
 * @return 写入完成后的总长度
 */
unsigned short set_wifi_uart_byte(unsigned short dest, unsigned char byte);

/**
 * @brief  写wifi_uart_buffer
 * @param[in] {dest} 目标地址
 * @param[in] {src} 源地址
 * @param[in] {len} 数据长度
 * @return 写入结束的缓存地址
 */
unsigned short set_wifi_uart_buffer(unsigned short dest, const unsigned char *src, unsigned short len);

/**
 * @brief  计算校验和
 * @param[in] {pack} 数据源指针
 * @param[in] {pack_len} 计算校验和长度
 * @return 校验和
 */
unsigned char get_check_sum(unsigned char *pack, unsigned short pack_len);

/**
 * @brief  向wifi串口发送一帧数据
 * @param[in] {fr_type} 帧类型
 * @param[in] {fr_ver} 帧版本
 * @param[in] {len} 数据长度
 * @return Null
 */
void wifi_uart_write_frame(unsigned char fr_type, unsigned char fr_ver, unsigned short len);


/**
 * @brief  数据帧处理
 * @param[in] {offset} 数据起始位
 * @return Null
 */
void data_handle(unsigned short offset);

/**
 * @brief  判断串口接收缓存中是否有数据
 * @param  Null
 * @return 是否有数据
 */
unsigned char with_data_rxbuff(void);

/**
 * @brief  读取队列1字节数据
 * @param  Null
 * @return Read the data
 */
unsigned char take_byte_rxbuff(void);
#endif
  
  
