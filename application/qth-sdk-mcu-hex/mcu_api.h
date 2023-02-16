#ifndef __MCU_API_H_
#define __MCU_API_H_


#ifdef MCU_API_GLOBAL
  #define MCU_API_EXTERN
#else
  #define MCU_API_EXTERN   extern
#endif


typedef struct 
{
  unsigned char *buffer;
  unsigned short buffer_len;
  unsigned short value_len;
  unsigned short offset;
}mcu_tsl_struct_t;



/**
 * @brief  hex转bcd
 * @param[in] {Value_H} 高字节
 * @param[in] {Value_L} 低字节
 * @return 转换完成后数据
 */
unsigned char hex_to_bcd(unsigned char Value_H,unsigned char Value_L);

/**
 * @brief  求字符串长度
 * @param[in] {str} 字符串地址
 * @return 数据长度
 */
unsigned long my_strlen(unsigned char *str);

/**
 * @brief  把src所指内存区域的前count个字节设置成字符c
 * @param[out] {src} 待设置的内存首地址
 * @param[in] {ch} 设置的字符
 * @param[in] {count} 设置的内存长度
 * @return 待设置的内存首地址
 */
void *my_memset(void *src,unsigned char ch,unsigned short count);

/**
 * @brief  内存拷贝
 * @param[out] {dest} 目标地址
 * @param[in] {src} 源地址
 * @param[in] {count} 拷贝数据个数
 * @return 数据处理完后的源地址
 */
void *my_memcpy(void *dest, const void *src, unsigned short count);

/**
 * @brief  字符串拷贝
 * @param[in] {dest} 目标地址
 * @param[in] {src} 源地址
 * @return 数据处理完后的源地址
 */
char *my_strcpy(char *dest, const char *src);

/**
 * @brief  字符串比较
 * @param[in] {s1} 字符串 1
 * @param[in] {s2} 字符串 2
 * @return 大小比较值
 * -         0:s1=s2
 * -         <0:s1<s2
 * -         >0:s1>s2
 */
int my_strcmp(char *s1 , char *s2);

/**
 * @brief  将int类型拆分四个字节
 * @param[in] {number} 4字节原数据
 * @param[out] {value} 处理完成后4字节数据
 * @return Null
 */
void int_to_byte(unsigned long number,unsigned char value[4]);

/**
 * @brief  将4字节合并为1个32bit变量
 * @param[in] {value} 4字节数组
 * @return 合并完成后的32bit变量
 */
unsigned long byte_to_int(const unsigned char value[4]);

/**
 * @brief  raw型tsl数据上传
 * @param[in] {tslid} tslid号
 * @param[in] {value} 当前tsl值指针
 * @param[in] {len} 数据长度
 * @return Null
 * @note   Null
 */
unsigned char mcu_tsl_raw_update(unsigned short tslid,const unsigned char value[],unsigned short len);

/**
 * @brief  bool型tsl数据上传
 * @param[in] {tslid} tslid号
 * @param[in] {value} 当前tsl值
 * @return Null
 * @note   Null
 */
unsigned char mcu_tsl_bool_update(unsigned short tslid,unsigned char value);

/**
 * @brief  value型tsl数据上传
 * @param[in] {tslid} tslid号
 * @param[in] {value} 当前tsl值
 * @return Null
 * @note   Null
 */

/**
 * @brief  value型tsl数据上传
 * @param[in] {tslid} tslid号
 * @param[in] {value} 当前tsl值
 * @return Null
 * @note   Null
 */
unsigned char mcu_tsl_value_update(unsigned short tslid,unsigned long value);

/**
 * @brief  fault型tsl数据上传
 * @param[in] {tslid} tslid号
 * @param[in] {value} 当前tsl值
 * @return Null
 * @note   Null
 */
unsigned char mcu_tsl_fault_update(unsigned short tslid,unsigned long value);


/**
 * @brief  string型tsl数据上传
 * @param[in] {tslid} tslid号
 * @param[in] {value} 当前tsl值指针
 * @param[in] {len} 数据长度
 * @return Null
 * @note   Null
 */
unsigned char mcu_tsl_string_update(unsigned short tslid,const unsigned char value[],unsigned short len);

/**
 * @brief  double型tsl数据上传
 * @param[in] {tslid} tslid号
 * @param[in] {value} 当前tsl值
 * @return Null
 * @note   Null
 */
unsigned char mcu_tsl_double_update(unsigned short tslid, double value);

/**
 * @brief  struct/arrary型tsl数据点初始化
 * @param[in] {tslid} tslid号
 * @param[in] {st} 结构体变量指针
 * @param[in] {buffer} 组装结构体数据的缓存地址
 * @param[in] {buffer_len} 与buffer对应的缓存大小
 * @return Null
 * @note   Null
 */
int mcu_tsl_struct_init(unsigned short tslid, mcu_tsl_struct_t *st, unsigned char *buffer, unsigned short buffer_len);

/**
 * @brief  添加结构体的成员数据
 * @param[in] {tslid} 结构体成员的tslid号
 * @param[in] {st} 结构体变量指针
 * @param[in] {type} 成员的数据类型
 * @param[in] {value} 成员的数据地址
 * @param[in] {len} 成员的数据value的大小
 * @return Null
 * @note   Null
 */
int mcu_tsl_struct_add_item(unsigned short tslid, mcu_tsl_struct_t *st, unsigned char type, unsigned char *value, unsigned short len);

/**
 * @brief  struct/arrary型tsl数据上传
 * @param[in] {st} 结构体变量指针
 * @return Null
 * @note   Null
 */
unsigned char mcu_tsl_struct_update(mcu_tsl_struct_t *st);

#ifdef MCU_TSL_UPLOAD_SYN
/**
 * @brief  bool型tsl数据同步上传
 * @param[in] {tslid} tslid号
 * @param[in] {value} 当前tsl值指针
 * @return Null
 * @note   Null
 */
unsigned char mcu_tsl_bool_update_syn(unsigned short tslid,unsigned char value);

/**
 * @brief  value型tsl数据同步上传
 * @param[in] {tslid} tslid号
 * @param[in] {value} 当前tsl值指针
 * @return Null
 * @note   Null
 */
unsigned char mcu_tsl_value_update_syn(unsigned short tslid,unsigned long value);

/**
 * @brief  fault型tsl数据同步上传
 * @param[in] {tslid} tslid号
 * @param[in] {value} 当前tsl值指针
 * @return Null
 * @note   Null
 */
unsigned char mcu_tsl_fault_update_syn(unsigned short tslid,unsigned long value);
#endif

/**
 * @brief  mcu获取raw型下发tsl值
 * @param[in] {value} tsl数据缓冲区地址
 * @param[in] {len} tsl数据长度
 * @param[out] {value} raw 数据缓冲区地址
 * @param[out] {out_len} raw 数据长度
 * @return 当前tsl值
 * @note   Null
 */
unsigned char mcu_get_tsl_download_raw(const unsigned char value[],unsigned short len, unsigned char *out, unsigned short *out_len);

/**
 * @brief  mcu获取bool型下发tsl值
 * @param[in] {value} tsl数据缓冲区地址
 * @param[in] {len} tsl数据长度
 * @return 当前tsl值
 * @note   Null
 */
unsigned char mcu_get_tsl_download_bool(const unsigned char value[],unsigned short len);
/**
 * @brief  mcu获取value型下发tsl值
 * @param[in] {value} tsl数据缓冲区地址
 * @param[in] {len} tsl数据长度
 * @return 当前tsl值
 * @note   Null
 */
unsigned long mcu_get_tsl_download_value(const unsigned char value[],unsigned short len);

/**
 * @brief  mcu获取string型下发tsl值
 * @param[in] {value} tsl数据缓冲区地址
 * @param[in] {len} tsl数据长度
 * @param[out] {value} string 数据缓冲区地址
 * @param[out] {out_len} string 数据长度
 * @return 当前tsl值
 * @note   Null
 */
unsigned char mcu_get_tsl_download_string(const unsigned char value[],unsigned short len, unsigned char *out, unsigned short *out_len);


/**
 * @brief  mcu获取double型下发tsl值
 * @param[in] {value} tsl数据缓冲区地址
 * @param[in] {len} tsl数据长度
 * @return 当前tsl值
 * @note   Null
 */
double mcu_get_tsl_download_double(const unsigned char value[],unsigned short len);

/**
 * @brief  mcu解析struct/arrary型下发tsl值
 * @param[in] {st} 结构体变量地址
 * @param[in] {buffer} tsl数据缓冲区地址
 * @param[in] {buffer_len} tsl数据长度
 * @return 当前tsl值
 * @note   Null
 */
int mcu_tsl_struct_parser(mcu_tsl_struct_t *st, unsigned char *buffer, unsigned short buffer_len);

/**
 * @brief  mcu获取struct/arrary型下发的成员tsl值
 * @param[in] {st} 结构体变量地址
 * @param[out] {tslid} 存储结构体/数组成员tslid的变量地址
 * @param[out] {type} 存储结构体/数组成员类型的地址
 * @param[out] {value} 存储结构体/数组成员数据的地址
 * @param[out] {len} 存储结构体/数组成员数据长度的地址
 * @return 当前tsl值
 * @note   类似一个迭代器，需要循环调用此接口获取出所有的元素
 */
int mcu_tsl_struct_get_item(mcu_tsl_struct_t *st, unsigned short *tslid, unsigned char *type, unsigned char *value, unsigned short *len);

/**
 * @brief  串口接收数据暂存处理
 * @param[in] {value} 串口收到的1字节数据
 * @return Null
 * @note   在MCU串口处理函数中调用该函数,并将接收到的数据作为参数传入
 */
void uart_receive_input(unsigned char value);

/**
 * @brief  串口接收多个字节数据暂存处理
 * @param[in] {value} 串口要接收的数据的源地址
 * @param[in] {data_len} 串口要接收的数据的数据长度
 * @return Null
 * @note   如需要支持一次多字节缓存，可调用该函数
 */
void uart_receive_buff_input(unsigned char value[], unsigned short data_len);

/**
 * @brief  wifi串口数据处理服务
 * @param  Null
 * @return Null
 * @note   在MCU主函数while循环中调用该函数
 */
void wifi_uart_service(void);

/**
 * @brief  协议串口初始化函数
 * @param  Null
 * @return Null
 * @note   在MCU初始化代码中调用该函数
 */
void wifi_protocol_init(void);

/**
 * @brief  MCU获取复位wifi成功标志
 * @param  Null
 * @return 复位标志
 * -           0(RESET_WIFI_ERROR):失败
 * -           1(RESET_WIFI_SUCCESS):成功
 * @note   1:MCU主动调用mcu_reset_wifi()后调用该函数获取复位状态
 *         
 */
unsigned char mcu_get_reset_wifi_flag(void);

/**
 * @brief  MCU主动重置wifi工作模式
 * @param  Null
 * @return Null
 * @note   1:MCU主动调用,通过mcu_get_reset_wifi_flag()函数获取重置wifi是否成功
 *         
 */
void mcu_reset_wifi(void);



/**
 * @brief  MCU主动获取当前wifi工作状态
 * @param  Null
 * @return wifi work state
 * -          WIFI_CONFIG_STATE: Wi-Fi 配网状态（蓝牙处于配网状态）
 * -          WIFI_NOT_CONNECTED: WIFI配置成功但未连上路由器
 * -          WIFI_CONNECTED: WIFI配置成功且连上路由器
 * -          WIFI_CONN_CLOUD: WIFI已经连接上云服务器
 * @note   
 */
unsigned char mcu_get_wifi_work_state(void);


#ifdef SUPPORT_GREEN_TIME
/**
 * @brief  MCU获取格林时间
 * @param  Null
 * @return Null
 * @note   MCU需要自行调用该功能
 */
void mcu_get_green_time(void);
#endif

#ifdef SUPPORT_MCU_RTC_CHECK
/**
 * @brief  MCU获取系统时间,用于校对本地时钟
 * @param  Null
 * @return Null
 * @note   MCU主动调用完成后在mcu_write_rtctime函数内校对rtc时钟
 */
void mcu_get_system_time(void);
#endif

#ifdef WIFI_TEST_ENABLE
/**
 * @brief  mcu发起wifi功能测试
 * @param  Null
 * @return Null
 * @note   MCU需要自行调用该功能
 */
void mcu_start_wifitest(void);
#endif


#ifdef GET_WIFI_STATUS_ENABLE
/**
 * @brief  获取当前wifi联网状态
 * @param  Null
 * @return Null
 * @note   MCU需要自行调用该功能
 */
void mcu_get_wifi_connect_status(void);
#endif


#ifdef GET_MODULE_MAC_ENABLE
/**
 * @brief  获取模块MAC
 * @param  Null
 * @return Null
 * @note   MCU需要自行调用该功能
 */
void mcu_get_module_mac(void);
#endif




#endif
