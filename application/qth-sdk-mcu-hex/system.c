#define SYSTEM_GLOBAL

#include "wifi.h"


extern const DOWNLOAD_CMD_S download_cmd[];



/**
 * @brief  写wifi_uart字节
 * @param[in] {dest} 缓存区其实地址
 * @param[in] {byte} 写入字节值
 * @return 写入完成后的总长度
 */
unsigned short set_wifi_uart_byte(unsigned short dest, unsigned char byte)
{
    unsigned char *obj = (unsigned char *)wifi_uart_tx_buf + DATA_START + dest;
    
    *obj = byte;
    dest += 1;
    
    return dest;
}

/**
 * @brief  写wifi_uart_buffer
 * @param[in] {dest} 目标地址
 * @param[in] {src} 源地址
 * @param[in] {len} 数据长度
 * @return 写入结束的缓存地址
 */
unsigned short set_wifi_uart_buffer(unsigned short dest, const unsigned char *src, unsigned short len)
{
    unsigned char *obj = (unsigned char *)wifi_uart_tx_buf + DATA_START + dest;
    
    my_memcpy(obj,src,len);
    
    dest += len;
    return dest;
}

/**
 * @brief  计算校验和
 * @param[in] {pack} 数据源指针
 * @param[in] {pack_len} 计算校验和长度
 * @return 校验和
 */
unsigned char get_check_sum(unsigned char *pack, unsigned short pack_len)
{
    unsigned short i;
    unsigned char check_sum = 0;
    
    for(i = 0; i < pack_len; i ++) {
        check_sum += *pack ++;
    }
    
    return check_sum;
}

/**
 * @brief  串口发送一段数据
 * @param[in] {in} 发送缓存指针
 * @param[in] {len} 数据发送长度
 * @return Null
 */
static void wifi_uart_write_data(unsigned char *in, unsigned short len)
{
    if((NULL == in) || (0 == len)) {
        return;
    }
    
    while(len --) {
        uart_transmit_output(*in);
        in ++;
    }
}

#if 0

#define __is_print(ch) ((unsigned int)((ch) - ' ') < 127u - ' ')

void dump_hex(const uint8_t *buf, uint32_t size, uint32_t number)
{
    int i, j;
    char buffer[128] = {0};
    int n = 0;

    for (i = 0; i < size; i += number)
    {
        n += snprintf(buffer + n, sizeof(buffer) - n - 1, "%08X: ", i);

        for (j = 0; j < number; j++)
        {
            if (j % 8 == 0)
            {
                n += snprintf(buffer + n, sizeof(buffer) - n - 1, " ");
            }
            if (i + j < size)
                n += snprintf(buffer + n, sizeof(buffer) - n - 1, "%02X ", buf[i + j]);
            else
                n += snprintf(buffer + n, sizeof(buffer) - n - 1, "   ");
        }
        n += snprintf(buffer + n, sizeof(buffer) - n - 1, " ");

        for (j = 0; j < number; j++)
        {
            if (i + j < size)
            {
                n += snprintf(buffer + n, sizeof(buffer) - n - 1, "%c", __is_print(buf[i + j]) ? buf[i + j] : '.');
            }
        }
        n += snprintf(buffer + n, sizeof(buffer) - n - 1, "\n");
        printf("%s", buffer);
        n = 0;
        memset(buffer, 0, sizeof(buffer));
    }
}

#endif


/**
 * @brief  向wifi串口发送一帧数据
 * @param[in] {fr_type} 帧类型
 * @param[in] {fr_ver} 帧版本
 * @param[in] {len} 数据长度
 * @return Null
 */
void wifi_uart_write_frame(unsigned char fr_type, unsigned char fr_ver, unsigned short len)
{
    unsigned char check_sum = 0;
    
    wifi_uart_tx_buf[HEAD_FIRST] = 0x55;
    wifi_uart_tx_buf[HEAD_SECOND] = 0xaa;
    wifi_uart_tx_buf[PROTOCOL_VERSION] = fr_ver;
    wifi_uart_tx_buf[FRAME_TYPE] = fr_type;
    wifi_uart_tx_buf[LENGTH_HIGH] = len >> 8;
    wifi_uart_tx_buf[LENGTH_LOW] = len & 0xff;
    
    len += PROTOCOL_HEAD;
    check_sum = get_check_sum((unsigned char *)wifi_uart_tx_buf, len - 1);
    wifi_uart_tx_buf[len - 1] = check_sum;
    
    wifi_uart_write_data((unsigned char *)wifi_uart_tx_buf, len);
    #if 0
    dump_hex((const uint8_t *)wifi_uart_tx_buf, len, 8);
    #endif
    
}

/**
 * @brief  心跳包检测
 * @param  Null
 * @return Null
 */
static void heat_beat_check(void)
{
    unsigned char length = 0;
    static unsigned char mcu_reset_state = FALSE;
    
    if(FALSE == mcu_reset_state) {
        length = set_wifi_uart_byte(length, FALSE);
        mcu_reset_state = TRUE;
    }else {
        length = set_wifi_uart_byte(length, TRUE);
    }
    
    wifi_uart_write_frame(HEAT_BEAT_CMD, MCU_TX_VER, length);
}

/**
 * @brief  产品信息上传
 * @param  Null
 * @return Null
 */
static void product_info_update(void)
{
    unsigned char length = 0;
    // unsigned char str[10] = {0};
    
    length = set_wifi_uart_buffer(length, (const unsigned char *)"{\"p\":\"", my_strlen((unsigned char *)"{\"p\":\""));
    length = set_wifi_uart_buffer(length,(unsigned char *)PK_PS,my_strlen((unsigned char *)PK_PS));
    length = set_wifi_uart_buffer(length, (const unsigned char *)"\",\"v\":\"", my_strlen((unsigned char *)"\",\"v\":\""));
    length = set_wifi_uart_buffer(length,(unsigned char *)MCU_VER"\"",my_strlen((unsigned char *)MCU_VER"\""));
  
    length = set_wifi_uart_buffer(length, (const unsigned char *)"}", my_strlen((unsigned char *)"}"));
    
    wifi_uart_write_frame(PRODUCT_INFO_CMD, MCU_TX_VER, length);
}

/**
 * @brief  获取制定DPID在数组中的序号
 * @param[in] {dpid} dpid
 * @return dp序号
 */
static unsigned char get_dowmload_dpid_index(unsigned short dpid)
{
    unsigned char index;
    unsigned char total = get_download_cmd_total();
    
    for(index = 0; index < total; index ++) {
        if(download_cmd[index].dp_id == dpid) {
            break;
        }
    }
    
    return index;
}

/**
 * @brief  下发数据处理
 * @param[in] {value} 下发数据源指针
 * @return 返回数据处理结果
 */
static unsigned char data_point_handle(const unsigned char value[])
{
    unsigned short dp_id;
    unsigned char index;
    unsigned char dp_type;
    unsigned char ret;
    unsigned short dp_len;
    
    dp_id = (value[0] << 8) | value[1];
    dp_type = value[2];
    dp_len = value[3] * 0x100;
    dp_len += value[4];
    
    index = get_dowmload_dpid_index(dp_id);

    if(dp_type != download_cmd[index].dp_type) {
        //错误提示
        return FALSE;
    }else {
        ret = dp_download_handle(dp_id,value + 5,dp_len);
    }
    
    return ret;
}





/**
 * @brief  数据帧处理
 * @param[in] {offset} 数据起始位
 * @return Null
 */
void data_handle(unsigned short offset)
{
#ifdef SUPPORT_MCU_FIRM_UPDATE
    unsigned char *firmware_addr = NULL;
    static unsigned short firm_size;                                            //升级包一包的大小
    static unsigned long firm_length;                                           //MCU升级文件长度
    static unsigned char firm_update_flag = 0;                                  //MCU升级标志
    unsigned long dp_len;
    unsigned char firm_flag;                                                    //升级包大小标志
#else
    unsigned short dp_len;
#endif
  
    unsigned char ret;
    unsigned short i,total_len;
    unsigned char cmd_type = wifi_data_process_buf[offset + FRAME_TYPE];
    unsigned char result;


#ifdef WIFI_TEST_ENABLE
    unsigned char rssi;
#endif

    printf("cmd_type:0x%0X\r\n",cmd_type );

    switch(cmd_type)
    {
        case HEAT_BEAT_CMD:                                     //心跳包
            heat_beat_check();
        break;
    
        case PRODUCT_INFO_CMD:                                  //产品信息
            product_info_update();
        break;
        case WIFI_STATE_CMD:                                    //wifi工作状态	
            wifi_work_state = wifi_data_process_buf[offset + DATA_START];
            wifi_uart_write_frame(WIFI_STATE_CMD, MCU_TX_VER, 0);
        break;
        case WIFI_RESET_CMD:                                    //重置wifi(wifi返回成功)
            reset_wifi_flag = RESET_WIFI_SUCCESS;
        break;   
        case DATA_QUERT_CMD:                                    //命令下发
            total_len = (wifi_data_process_buf[offset + LENGTH_HIGH] << 8) | wifi_data_process_buf[offset + LENGTH_LOW];
    
            for(i = 0;i < total_len; ) {
                dp_len = wifi_data_process_buf[offset + DATA_START + i + 2] * 0x100;
                dp_len += wifi_data_process_buf[offset + DATA_START + i + 3];
                //
                ret = data_point_handle((unsigned char *)wifi_data_process_buf + offset + DATA_START + i);
      
                if(SUCCESS == ret) {
                    //成功提示
                }else {
                    //错误提示
                }
      
                i += (dp_len + 4);
            }
        break;
    
        case STATE_QUERY_CMD:                                   //状态查询
            all_data_update();                               
        break;
    
#ifdef SUPPORT_MCU_FIRM_UPDATE
        case UPDATE_START_CMD:                                  //升级开始
            //获取升级包大小全局变量
            firm_flag = PACKAGE_SIZE;
            if(firm_flag == 0) {
                firm_size = 256;
            }else if(firm_flag == 1) {
                firm_size = 512;
            }else if(firm_flag == 2) { 
                firm_size = 1024;
            }

            firm_length = wifi_data_process_buf[offset + DATA_START];
            firm_length <<= 8;
            firm_length |= wifi_data_process_buf[offset + DATA_START + 1];
            firm_length <<= 8;
            firm_length |= wifi_data_process_buf[offset + DATA_START + 2];
            firm_length <<= 8;
            firm_length |= wifi_data_process_buf[offset + DATA_START + 3];
            
            upgrade_package_choose(PACKAGE_SIZE);
            firm_update_flag = UPDATE_START_CMD;
        break;
    
        case UPDATE_TRANS_CMD:                                  //升级传输
            if(firm_update_flag == UPDATE_START_CMD) {
                //停止一切数据上报
                stop_update_flag = ENABLE;
      
                total_len = (wifi_data_process_buf[offset + LENGTH_HIGH] << 8) | wifi_data_process_buf[offset + LENGTH_LOW];
      
                dp_len = wifi_data_process_buf[offset + DATA_START];
                dp_len <<= 8;
                dp_len |= wifi_data_process_buf[offset + DATA_START + 1];
                dp_len <<= 8;
                dp_len |= wifi_data_process_buf[offset + DATA_START + 2];
                dp_len <<= 8;
                dp_len |= wifi_data_process_buf[offset + DATA_START + 3];
      
                firmware_addr = (unsigned char *)wifi_data_process_buf;
                firmware_addr += (offset + DATA_START + 4);
      
                if((total_len == 4) && (dp_len == firm_length)) {
                    //最后一包
                    ret = mcu_firm_update_handle(firmware_addr,dp_len,0);
                    firm_update_flag = 0;
                }else if((total_len - 4) <= firm_size) {
                    ret = mcu_firm_update_handle(firmware_addr,dp_len,total_len - 4);
                }else {
                    firm_update_flag = 0;
                    ret = ERROR;
                }
      
                if(ret == SUCCESS) {
                    wifi_uart_write_frame(UPDATE_TRANS_CMD, MCU_TX_VER, 0);
                }
                //恢复一切数据上报
                stop_update_flag = DISABLE;
            }
        break;
#endif

        case GET_ONLINE_TIME_CMD:                               //获取格林时间
            mcu_get_greentime((unsigned char *)(wifi_data_process_buf + offset + DATA_START));
        break;


        case GET_LOCAL_TIME_CMD:                               //获取本地时间
            mcu_write_rtctime((unsigned char *)(wifi_data_process_buf + offset + DATA_START));
        break;

 
        case WIFI_TEST_CMD:                                     //wifi功能测试（扫描指定路由）
            result = wifi_data_process_buf[offset + DATA_START];
            rssi = wifi_data_process_buf[offset + DATA_START + 1];
            wifi_test_result(result, rssi);
        break;
        case GET_MAC_CMD:                                       //获取模块mac
            mcu_get_mac((unsigned char *)(wifi_data_process_buf + offset + DATA_START));
        break;



        case GET_WIFI_STATUS_CMD:                               //获取当前wifi联网状态
            result = wifi_data_process_buf[offset + DATA_START];
            get_wifi_status(result);
        break;


#ifdef MCU_DP_UPLOAD_SYN
        case STATE_UPLOAD_SYN_RECV_CMD:                         //状态上报（同步）
            result = wifi_data_process_buf[offset + DATA_START];
            get_upload_syn_result(result);
        break;
#endif               
        default:break;
    }
}

/**
 * @brief  判断串口接收缓存中是否有数据
 * @param  Null
 * @return 是否有数据
 */
unsigned char with_data_rxbuff(void)
{
    if(rx_buf_in != rx_buf_out)
        return 1;
    else
        return 0;
}

/**
 * @brief  读取队列1字节数据
 * @param  Null
 * @return Read the data
 */
unsigned char take_byte_rxbuff(void)
{
    unsigned char value = 0;
    
    if(rx_buf_out != rx_buf_in) {
        //有数据
        if(rx_buf_out >= (unsigned char *)(wifi_uart_rx_buf + sizeof(wifi_uart_rx_buf))) {
            //数据已经到末尾
            rx_buf_out = (unsigned char *)(wifi_uart_rx_buf);
        }
        
        value = *rx_buf_out ++;   
    }
    
    return value;
}

