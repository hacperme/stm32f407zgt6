/******************************************************************************
                                移植须知:
1:MCU必须在while中直接调用mcu_api.c内的wifi_uart_service()函数
2:程序正常初始化完成后,建议不进行关串口中断,如必须关中断,关中断时间必须短,关中断会引起串口数据包丢失
3:请勿在中断/定时器中断内调用上报函数
******************************************************************************/

#include "wifi.h"



/******************************************************************************
                              第一步:初始化
1:在需要使用到wifi相关文件的文件中include "wifi.h"
2:在MCU初始化中调用mcu_api.c文件中的wifi_protocol_init()函数
3:将MCU串口单字节发送函数填入protocol.c文件中uart_transmit_output函数内,并删除#error
4:在MCU串口接收函数中调用mcu_api.c文件内的uart_receive_input函数,并将接收到的字节作为参数传入
5:单片机进入while循环后调用mcu_api.c文件内的wifi_uart_service()函数
******************************************************************************/

/******************************************************************************
                        1:tsl数据点序列类型对照表
                        **请和平台所设置的保持一致**         
******************************************************************************/
// #error "请将功能点ID与类型的匹配关系和平台上设置的保持一致,并删除该行"
const DOWNLOAD_CMD_S download_cmd[] =
{
//   {TSLID_TEST_RAW, TSL_TYPE_RAW},
  {TSLID_TEST_BOOL, TSL_TYPE_BOOL},
  {TSLID_TEST_VALUE, TSL_TYPE_VALUE},
  {TSLID_TEST_FAULT,TSL_TYPE_BITMAP},
  {TSLID_TEST_STRUCT,TSL_TYPE_STRUCT},
  {TSLID_TEST_STRING,TSL_TYPE_STRING},
  {TSLID_TEST_DOUBLE,TSL_TYPE_DOUBLE},
};



/******************************************************************************
                           2:串口单字节发送函数
请将MCU串口发送函数填入该函数内,并将接收到的数据作为参数传入串口发送函数
******************************************************************************/

/**
 * @brief  串口发送数据
 * @param[in] {value} 串口要发送的1字节数据
 * @return Null
 */
void uart_transmit_output(unsigned char value)
{
    // #error "请将MCU串口发送函数填入该函数,并删除该行"

    extern int _write2(int fd, char *ptr, int len) ;

    _write2(1, (char *)&value, 1);
    
/*
    //Example:
    extern void Uart_PutChar(unsigned char value);
    Uart_PutChar(value);	                                //串口发送函数
*/
}

/******************************************************************************
                           第二步:实现具体用户函数
1:APP下发数据处理
2:数据上报处理
******************************************************************************/

/******************************************************************************
                            1:所有数据上报处理
当前函数处理全部数据上报(包括可下发/可上报和只上报)
  需要用户按照实际情况实现:
  1:需要实现可下发/可上报数据点上报
  2:需要实现只上报数据点上报
此函数为MCU内部必须调用
用户也可调用此函数实现全部数据上报
******************************************************************************/


/**
 * @brief  系统所有tsl点信息上传,实现APP和muc数据同步
 * @param  Null
 * @return Null
 * @note   此函数SDK内部需调用，MCU必须实现该函数内数据上报功能，包括只上报和可上报可下发型数据
 */
void all_data_update(void)
{
    // #error "请在此处理可下发可上报数据及只上报数据示例,处理完成后删除该行"

    mcu_tsl_struct_t st = {0};
    unsigned char buf[200] = {0};
    int test_value = 123;
    unsigned char test_bool = 1;
    unsigned char test_string[] = "substring";
    double test_double = 22.34;

    // 请按照实际数据修改每个可下发可上报函数和只上报函数
    mcu_tsl_bool_update(TSLID_TEST_BOOL, 0);                                                               // BOOL型数据上报;
    mcu_tsl_value_update(TSLID_TEST_VALUE, 12);                                                            // VALUE型数据上报;
    mcu_tsl_fault_update(TSLID_TEST_FAULT, 0x03);                                                          // 故障型数据上报;
    mcu_tsl_string_update(TSLID_TEST_STRING, (const unsigned char *)"test_string", strlen("test_string")); // STRING型数据上报;
    mcu_tsl_double_update(TSLID_TEST_DOUBLE, 12.34);                                                       // DOUBLE型数据上报;

    mcu_tsl_struct_init(TSLID_TEST_STRUCT, &st, buf, sizeof(buf));  // 初始化STRUCT类型

    mcu_tsl_struct_add_item(TSLID_TEST_STRUCT_SUB_VALUE, &st, TSL_TYPE_VALUE, (unsigned char *)&test_value, sizeof(test_value)); //添加结构体成员数据

    mcu_tsl_struct_add_item(TSLID_TEST_STRUCT_SUB_BOOL, &st, TSL_TYPE_BOOL, &test_bool, sizeof(test_bool));                         //添加结构体成员数据

    mcu_tsl_struct_add_item(TSLID_TEST_STRUCT_SUB_STRING, &st, TSL_TYPE_STRING, test_string, strlen((const char *)test_string));    //添加结构体成员数据

    mcu_tsl_struct_add_item(TSLID_TEST_STRUCT_SUB_DOUBLE, &st, TSL_TYPE_DOUBLE, (unsigned char *)&test_double, sizeof(test_double));  //添加结构体成员数据
    mcu_tsl_struct_update(&st); // 结构体型数据上报;
}


/******************************************************************************
                                WARNING!!!    
                            2:所有数据上报处理
                            具体请用户自行实现数据处理
******************************************************************************/
/*****************************************************************************
函数名称 : tsl_download_test_bool_handle
功能描述 : 针对TSLID_TEST_BOOL的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char tsl_download_test_bool_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前tsl类型为BOOL
    unsigned char ret;
    //0:off/1:on
    unsigned char bool_value;
    
    bool_value = mcu_get_tsl_download_bool(value,length);
    if(bool_value == 0) {
        //bool off
        printf("set off\r\n");
    }else {
        printf("set on\r\n");
    }
  
    //There should be a report after processing the tsl
    ret = mcu_tsl_bool_update(TSLID_TEST_BOOL,bool_value);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}

/*****************************************************************************
函数名称 : tsl_download_test_value_handle
功能描述 : 针对TSLID_TEST_VALUE的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char tsl_download_test_value_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前tsl类型为VALUE
    unsigned char ret = SUCCESS;
    unsigned long int_value;
    
    int_value = mcu_get_tsl_download_value(value,length);
    /*
    //VALUE type data processing
    
    */

    printf("set int_value:%ld\r\n", int_value);
    
    //There should be a report after processing the tsl
    ret = mcu_tsl_value_update(TSLID_TEST_VALUE,int_value);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}

/*****************************************************************************
函数名称 : tsl_download_test_string_handle
功能描述 : 针对TSLID_TEST_STRING的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char tsl_download_test_string_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前tsl类型为STRING
    unsigned char ret = SUCCESS;
    unsigned char string_value[100] = {0};
    unsigned short string_len = 0;
    
    mcu_get_tsl_download_string(value,length,string_value, &string_len);


    printf("set string_value:%s\r\n", string_value);
    
    //There should be a report after processing the tsl
    ret = mcu_tsl_string_update(TSLID_TEST_STRING,string_value,strlen((const char *)string_value));
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}



/*****************************************************************************
函数名称 : tsl_download_test_double_handle
功能描述 : 针对TSLID_TEST_DOUBLE的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char tsl_download_test_double_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前tsl类型为DOUBLE
    unsigned char ret = SUCCESS;
    double double_value;
    
    double_value = mcu_get_tsl_download_double(value,length);

    
    printf("set double_value:%f\r\n", double_value);
    
    //There should be a report after processing the tsl
    ret = mcu_tsl_double_update(TSLID_TEST_DOUBLE,double_value);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}



/*****************************************************************************
函数名称 : tsl_download_test_struct_handle
功能描述 : 针对TSLID_TEST_STRUCT的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char tsl_download_test_struct_handle(const unsigned char value[], unsigned short length)
{
    // 示例:当前tsl类型为STRUCT
    unsigned char ret = SUCCESS;

    // 解析下发结构体数据变量定义
    mcu_tsl_struct_t st = {0};
    unsigned short tslid;
    unsigned char tsl_type;
    unsigned char tsl_value[100] = {0};
    unsigned short tsl_length = 0;

    unsigned long int_value;
    unsigned char bool_value;
    double double_value;
    unsigned char string_value[100] = {0};
    unsigned short string_len = 0;

    // 回传结构体数据变量定义
    mcu_tsl_struct_t st_send = {0};
    unsigned char buf[200] = {0};
    mcu_tsl_struct_init(TSLID_TEST_STRUCT, &st_send, buf, sizeof(buf));

    // 解析结构体数据
    mcu_tsl_struct_parser(&st, (unsigned char *)value, length);

    // 循环迭代解析结构体的每个成员数据
    while (mcu_tsl_struct_get_item(&st, &tslid, &tsl_type, tsl_value, &tsl_length))
    {
        switch (tslid)
        {
        case TSLID_TEST_STRUCT_SUB_VALUE:
        {

            int_value = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set sub int_value:%ld\r\n", int_value);
            mcu_tsl_struct_add_item(TSLID_TEST_STRUCT_SUB_VALUE, &st_send, TSL_TYPE_VALUE, (unsigned char *)&int_value, sizeof(int_value));
        }
        break;
        case TSLID_TEST_STRUCT_SUB_BOOL:
        {

            bool_value = mcu_get_tsl_download_bool(tsl_value, tsl_length);
            if (bool_value == 0)
            {
                printf("set sub bool off\r\n");
            }
            else
            {
                printf("set sub bool on\r\n");
            }
            mcu_tsl_struct_add_item(TSLID_TEST_STRUCT_SUB_BOOL, &st_send, TSL_TYPE_BOOL, (unsigned char *)&bool_value, sizeof(bool_value));
        }
        break;

        case TSLID_TEST_STRUCT_SUB_DOUBLE:
        {

            double_value = mcu_get_tsl_download_double(tsl_value, tsl_length);

            printf("set sub double_value:%f\r\n", double_value);
            mcu_tsl_struct_add_item(TSLID_TEST_STRUCT_SUB_DOUBLE, &st_send, TSL_TYPE_DOUBLE, (unsigned char *)&double_value, sizeof(double_value));
        }
        break;
        case TSLID_TEST_STRUCT_SUB_STRING:
        {

            mcu_get_tsl_download_string(tsl_value, tsl_length, string_value, &string_len);

            printf("set string_value:%s\r\n", string_value);
            mcu_tsl_struct_add_item(TSLID_TEST_STRUCT_SUB_STRING, &st_send, TSL_TYPE_STRING, (unsigned char *)string_value, strlen((const char *)string_value));
            break;
        }
        default:
            break;
        }
    }

    // There should be a report after processing the tsl

    mcu_tsl_struct_update(&st_send); //结构体型数据回传上报;

    if (ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}

/******************************************************************************
                                WARNING!!!                     
此部分函数用户请勿修改!!
******************************************************************************/

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
unsigned char tsl_download_handle(unsigned short tslid,const unsigned char value[], unsigned short length)
{
    /*********************************
    当前函数处理可下发/可上报数据调用                    
    具体函数内需要实现下发数据处理
    完成用需要将处理结果反馈至APP端,否则APP会认为下发失败
    ***********************************/
    unsigned char ret = SUCCESS;
    switch(tslid) {
        #if 0
        case TSLID_TEST_RAW :
            //测试RAW类型数据处理函数
            ret = tsl_download_test_raw_handle(value,length);
        break;
        #endif
        case TSLID_TEST_BOOL:
            //测试BOOL类型数据处理函数
            ret = tsl_download_test_bool_handle(value,length);
        break;
        case TSLID_TEST_VALUE:
            //测试VALUE类型数据处理函数
            ret = tsl_download_test_value_handle(value,length);
        break;
        case TSLID_TEST_STRING:
            //测试STRING类型数据处理函数
            ret = tsl_download_test_string_handle(value,length);
        break;
        case TSLID_TEST_DOUBLE:
            //测试DOUBLE类型数据处理函数
            ret = tsl_download_test_double_handle(value,length);
        break;
        case TSLID_TEST_STRUCT:
            //测试STRUCT类型数据处理函数
            ret = tsl_download_test_struct_handle(value,length);
        default:
        break;
    }
    return ret;
}

/**
 * @brief  获取所有tsl命令总和
 * @param[in] Null
 * @return 下发命令总和
 * @note   该函数用户不能修改
 */
unsigned char get_download_cmd_total(void)
{
    return(sizeof(download_cmd) / sizeof(download_cmd[0]));
}


/******************************************************************************
                                WARNING!!!                     
此代码为SDK内部调用,请按照实际tsl数据实现函数内部数据
******************************************************************************/

#ifdef SUPPORT_GREEN_TIME
/**
 * @brief  获取到的格林时间
 * @param[in] {time} 获取到的格林时间数据
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void mcu_get_greentime(unsigned char time[])
{
    // #error "请自行完成相关代码,并删除该行"
    /*
    time[0] 为是否获取时间成功标志，为 0 表示失败，为 1表示成功
    time[1] 为年份，0x00 表示 2000 年
    time[2] 为月份，从 1 开始到12 结束
    time[3] 为日期，从 1 开始到31 结束
    time[4] 为时钟，从 0 开始到23 结束
    time[5] 为分钟，从 0 开始到59 结束
    time[6] 为秒钟，从 0 开始到59 结束
    */
    if(time[0] == 1) {
        //正确接收到wifi模块返回的格林数据
        
    }else {
        //获取格林时间出错,有可能是当前wifi模块未联网
    }
}
#endif

#ifdef SUPPORT_MCU_RTC_CHECK
/**
 * @brief  MCU校对本地RTC时钟
 * @param[in] {time} 获取到的格林时间数据
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void mcu_write_rtctime(unsigned char time[])
{
    // #error "请自行完成RTC时钟写入代码,并删除该行"
    /*
    Time[0] 为是否获取时间成功标志，为 0 表示失败，为 1表示成功
    Time[1] 为年份，0x00 表示 2000 年
    Time[2] 为月份，从 1 开始到12 结束
    Time[3] 为日期，从 1 开始到31 结束
    Time[4] 为时钟，从 0 开始到23 结束
    Time[5] 为分钟，从 0 开始到59 结束
    Time[6] 为秒钟，从 0 开始到59 结束
    Time[7] 为星期，从 1 开始到 7 结束，1代表星期一
   */
    if(time[0] == 1) {
        //正确接收到wifi模块返回的本地时钟数据
     
    }else {
        //获取本地时钟数据出错,有可能是当前wifi模块未联网
    }
}
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
void wifi_test_result(unsigned char result,unsigned char rssi)
{
    // #error "请自行实现wifi功能测试成功/失败代码,完成后请删除该行"
    if(result == 0) {
        //测试失败
        if(rssi == 0x00) {
            //未扫描到名称为tuya_mdev_test路由器,请检查
        }else if(rssi == 0x01) {
            //模块未授权
        }
    }else {
        //测试成功
        //rssi为信号强度(0-100, 0信号最差，100信号最强)
    }
}
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
void get_upload_syn_result(unsigned char result)
{
    #error "请自行完成状态同步上报结果代码,并删除该行"
      
    if(result == 0) {
        //同步上报出错
    }else {
        //同步上报成功
    }
}
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
void get_wifi_status(unsigned char result)
{
//   #error "请自行完成获取 WIFI 状态结果代码,并删除该行"
    printf("result:%d\r\n", result);

    switch(result) {
        case 0:
            // wifi状态 1 Wi-Fi 配网状态（蓝牙处于配网状态）
        break;

        case 2:
            // wifi状态 3 WIFI 已配置但未连上路由器
        break;
        
        case 3:
            // wifi状态 4 WIFI 已配置且连上路由器
        break;
        
        case 4:
            // wifi状态 5 已连上路由器且连接到云端
        break;
        
        
        
        default:break;
    }
}
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
void mcu_get_mac(unsigned char mac[])
{
    // #error "请自行完成mac获取代码,并删除该行"
    /*
    mac[0]为是否获取mac成功标志，0x00 表示成功，为0x01表示失败
    mac[1]~mac[6]:当获取 MAC地址标志位如果mac[0]为成功，则表示模块有效的MAC地址
   */
   
    if(mac[0] == 1) {
        //获取mac出错
    }else {
        //正确接收到wifi模块返回的mac地址
    }
}
#endif




