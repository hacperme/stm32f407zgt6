#define MCU_API_GLOBAL

#include "wifi.h"

/**
 * @brief  hex转bcd
 * @param[in] {Value_H} 高字节
 * @param[in] {Value_L} 低字节
 * @return 转换完成后数据
 */
unsigned char hex_to_bcd(unsigned char Value_H,unsigned char Value_L)
{
    unsigned char bcd_value;
    
    if((Value_H >= '0') && (Value_H <= '9'))
        Value_H -= '0';
    else if((Value_H >= 'A') && (Value_H <= 'F'))
        Value_H = Value_H - 'A' + 10;
    else if((Value_H >= 'a') && (Value_H <= 'f'))
        Value_H = Value_H - 'a' + 10;
     
    bcd_value = Value_H & 0x0f;
    
    bcd_value <<= 4;
    if((Value_L >= '0') && (Value_L <= '9'))
        Value_L -= '0';
    else if((Value_L >= 'A') && (Value_L <= 'F'))
        Value_L = Value_L - 'a' + 10;
    else if((Value_L >= 'a') && (Value_L <= 'f'))
        Value_L = Value_L - 'a' + 10;
    
    bcd_value |= Value_L & 0x0f;

    return bcd_value;
}

/**
 * @brief  求字符串长度
 * @param[in] {str} 字符串地址
 * @return 数据长度
 */
unsigned long my_strlen(unsigned char *str)  
{
    unsigned long len = 0;
    if(str == NULL) { 
        return 0;
    }
    
    for(len = 0; *str ++ != '\0'; ) {
        len ++;
    }
    
    return len;
}

/**
 * @brief  把src所指内存区域的前count个字节设置成字符c
 * @param[out] {src} 待设置的内存首地址
 * @param[in] {ch} 设置的字符
 * @param[in] {count} 设置的内存长度
 * @return 待设置的内存首地址
 */
void *my_memset(void *src,unsigned char ch,unsigned short count)
{
    unsigned char *tmp = (unsigned char *)src;
    
    if(src == NULL) {
        return NULL;
    }
    
    while(count --) {
        *tmp ++ = ch;
    }
    
    return src;
}

/**
 * @brief  内存拷贝
 * @param[out] {dest} 目标地址
 * @param[in] {src} 源地址
 * @param[in] {count} 拷贝数据个数
 * @return 数据处理完后的源地址
 */
void *my_memcpy(void *dest, const void *src, unsigned short count)  
{  
    unsigned char *pdest = (unsigned char *)dest;  
    const unsigned char *psrc  = (const unsigned char *)src;  
    unsigned short i;
    
    if(dest == NULL || src == NULL) { 
        return NULL;
    }
    
    if((pdest <= psrc) || (pdest > psrc + count)) {  
        for(i = 0; i < count; i ++) {  
            pdest[i] = psrc[i];  
        }  
    }else {
        for(i = count; i > 0; i --) {  
            pdest[i - 1] = psrc[i - 1];  
        }  
    }  
    
    return dest;  
}

/**
 * @brief  字符串拷贝
 * @param[in] {dest} 目标地址
 * @param[in] {src} 源地址
 * @return 数据处理完后的源地址
 */
char *my_strcpy(char *dest, const char *src)  
{
    if((NULL == dest) || (NULL == src)) {
        return NULL;
    }
    
    char *p = dest;
    while(*src!='\0') {
        *dest++ = *src++;
    }
    *dest = '\0';
    return p;
}

/**
 * @brief  字符串比较
 * @param[in] {s1} 字符串 1
 * @param[in] {s2} 字符串 2
 * @return 大小比较值
 * -         0:s1=s2
 * -         <0:s1<s2
 * -         >0:s1>s2
 */
int my_strcmp(char *s1 , char *s2)
{
    while( *s1 && *s2 && *s1 == *s2 ) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

/**
 * @brief  将int类型拆分四个字节
 * @param[in] {number} 4字节原数据
 * @param[out] {value} 处理完成后4字节数据
 * @return Null
 */
void int_to_byte(unsigned long number,unsigned char value[4])
{
    value[0] = number >> 24;
    value[1] = number >> 16;
    value[2] = number >> 8;
    value[3] = number & 0xff;
}

/**
 * @brief  将4字节合并为1个32bit变量
 * @param[in] {value} 4字节数组
 * @return 合并完成后的32bit变量
 */
unsigned long byte_to_int(const unsigned char value[4])
{
    unsigned long nubmer = 0;

    nubmer = (unsigned long)value[0];
    nubmer <<= 8;
    nubmer |= (unsigned long)value[1];
    nubmer <<= 8;
    nubmer |= (unsigned long)value[2];
    nubmer <<= 8;
    nubmer |= (unsigned long)value[3];
    
    return nubmer;
}

/**
 * @brief  raw型tsl数据上传
 * @param[in] {tslid} tslid号
 * @param[in] {value} 当前tsl值指针
 * @param[in] {len} 数据长度
 * @return Null
 * @note   Null
 */
unsigned char mcu_tsl_raw_update(unsigned short tslid,const unsigned char value[],unsigned short len)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    //
    send_len = set_wifi_uart_byte(send_len, (tslid >> 8));
    send_len = set_wifi_uart_byte(send_len, tslid & 0xff);
    send_len = set_wifi_uart_byte(send_len,TSL_TYPE_RAW);
    //
    send_len = set_wifi_uart_byte(send_len,len / 0x100);
    send_len = set_wifi_uart_byte(send_len,len % 0x100);
    //
    send_len = set_wifi_uart_buffer(send_len,(unsigned char *)value, len);
    
    wifi_uart_write_frame(STATE_UPLOAD_CMD,MCU_TX_VER,send_len);

    return SUCCESS;
}

/**
 * @brief  bool型tsl数据上传
 * @param[in] {tslid} tslid号
 * @param[in] {value} 当前tsl值
 * @return Null
 * @note   Null
 */
unsigned char mcu_tsl_bool_update(unsigned short tslid,unsigned char value)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;

    send_len = set_wifi_uart_byte(send_len, (tslid >> 8));
    send_len = set_wifi_uart_byte(send_len, tslid & 0xff);
    send_len = set_wifi_uart_byte(send_len,TSL_TYPE_BOOL);
    //
    send_len = set_wifi_uart_byte(send_len,0);
    send_len = set_wifi_uart_byte(send_len,1);
    //
    if(value == FALSE) {
        send_len = set_wifi_uart_byte(send_len,FALSE);
    }else {
        send_len = set_wifi_uart_byte(send_len,1);
    }
    
    wifi_uart_write_frame(STATE_UPLOAD_CMD, MCU_TX_VER, send_len);
    
    return SUCCESS;
}

/**
 * @brief  value型tsl数据上传
 * @param[in] {tslid} tslid号
 * @param[in] {value} 当前tsl值
 * @return Null
 * @note   Null
 */
unsigned char mcu_tsl_value_update(unsigned short tslid, unsigned long value)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    
    send_len = set_wifi_uart_byte(send_len, (tslid >> 8));
    send_len = set_wifi_uart_byte(send_len, tslid & 0xff);
    send_len = set_wifi_uart_byte(send_len,TSL_TYPE_VALUE);
    //
    send_len = set_wifi_uart_byte(send_len,0);
    send_len = set_wifi_uart_byte(send_len,4);
    //
    send_len = set_wifi_uart_byte(send_len,value >> 24);
    send_len = set_wifi_uart_byte(send_len,value >> 16);
    send_len = set_wifi_uart_byte(send_len,value >> 8);
    send_len = set_wifi_uart_byte(send_len,value & 0xff);
    
    wifi_uart_write_frame(STATE_UPLOAD_CMD,MCU_TX_VER,send_len);
    
    return SUCCESS;
}

/**
 * @brief  fault型tsl数据上传
 * @param[in] {tslid} tslid号
 * @param[in] {value} 当前tsl值
 * @return Null
 * @note   Null
 */
unsigned char mcu_tsl_fault_update(unsigned short tslid, unsigned long value)
{
    unsigned short send_len = 0;
     
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    
    send_len = set_wifi_uart_byte(send_len, (tslid >> 8));
    send_len = set_wifi_uart_byte(send_len, tslid & 0xff);
    send_len = set_wifi_uart_byte(send_len,TSL_TYPE_BITMAP);
    //
    send_len = set_wifi_uart_byte(send_len,0);
    
    if((value | 0xff) == 0xff) {
        send_len = set_wifi_uart_byte(send_len,1);
        send_len = set_wifi_uart_byte(send_len,value);
    }else if((value | 0xffff) == 0xffff) {
        send_len = set_wifi_uart_byte(send_len,2);
        send_len = set_wifi_uart_byte(send_len,value >> 8);
        send_len = set_wifi_uart_byte(send_len,value & 0xff);
    }else {
        send_len = set_wifi_uart_byte(send_len,4);
        send_len = set_wifi_uart_byte(send_len,value >> 24);
        send_len = set_wifi_uart_byte(send_len,value >> 16);
        send_len = set_wifi_uart_byte(send_len,value >> 8);
        send_len = set_wifi_uart_byte(send_len,value & 0xff);
    }    
    
    wifi_uart_write_frame(STATE_UPLOAD_CMD, MCU_TX_VER, send_len);

    return SUCCESS;
}

/**
 * @brief  string型tsl数据上传
 * @param[in] {tslid} tslid号
 * @param[in] {value} 当前tsl值指针
 * @param[in] {len} 数据长度
 * @return Null
 * @note   Null
 */
unsigned char mcu_tsl_string_update(unsigned short tslid, const unsigned char value[],unsigned short len)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    //
    send_len = set_wifi_uart_byte(send_len, (tslid >> 8));
    send_len = set_wifi_uart_byte(send_len, tslid & 0xff);
    send_len = set_wifi_uart_byte(send_len,TSL_TYPE_STRING);
    //
    send_len = set_wifi_uart_byte(send_len,len / 0x100);
    send_len = set_wifi_uart_byte(send_len,len % 0x100);
    //
    send_len = set_wifi_uart_buffer(send_len,(unsigned char *)value,len);
    
    wifi_uart_write_frame(STATE_UPLOAD_CMD,MCU_TX_VER,send_len);
    
    return SUCCESS;
}

/**
 * @brief  fault型tsl数据上传
 * @param[in] {tslid} tslid号
 * @param[in] {value} 当前tsl值
 * @return Null
 * @note   Null
 */
unsigned char mcu_tsl_double_update(unsigned short tslid, double value)
{
    unsigned short send_len = 0;
    unsigned char *p = (unsigned char *)&value;

    if (stop_update_flag == ENABLE)
        return SUCCESS;

    send_len = set_wifi_uart_byte(send_len, (tslid >> 8));
    send_len = set_wifi_uart_byte(send_len, tslid & 0xff);
    send_len = set_wifi_uart_byte(send_len, TSL_TYPE_DOUBLE);
    //
    send_len = set_wifi_uart_byte(send_len, 0);
    send_len = set_wifi_uart_byte(send_len, 8);
    //
    send_len = set_wifi_uart_byte(send_len, p[7]);
    send_len = set_wifi_uart_byte(send_len, p[6]);
    send_len = set_wifi_uart_byte(send_len, p[5]);
    send_len = set_wifi_uart_byte(send_len, p[4]);
    send_len = set_wifi_uart_byte(send_len, p[3]);
    send_len = set_wifi_uart_byte(send_len, p[2]);
    send_len = set_wifi_uart_byte(send_len, p[1]);
    send_len = set_wifi_uart_byte(send_len, p[0]);
   
    wifi_uart_write_frame(STATE_UPLOAD_CMD, MCU_TX_VER, send_len);

    return SUCCESS;
}

/**
 * @brief  struct/arrary型tsl数据上传
 * @param[in] {tslid} tslid号
 * @param[in] {value} 当前tsl值指针
 * @param[in] {len} 数据长度
 * @return Null
 * @note   Null
 */
unsigned char mcu_tsl_struct_update(mcu_tsl_struct_t *st)
{
    unsigned short send_len = 0;
    if(st == NULL)
        return ERROR;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;

    send_len = set_wifi_uart_buffer(send_len,(unsigned char *)st->buffer, st->offset);
    
    wifi_uart_write_frame(STATE_UPLOAD_CMD,MCU_TX_VER,send_len);
    
    return SUCCESS;
}


/**
 * @brief  struct/arrary型tsl数据点初始化
 * @param[in] {void} 无
 * @return 无
 */
int mcu_tsl_struct_init(unsigned short tslid, mcu_tsl_struct_t *st, unsigned char *buffer, unsigned short buffer_len)
{
    if(st == NULL || buffer == NULL || buffer_len == 0)
        return ERROR;
    
    st->buffer = buffer;
    st->buffer_len = buffer_len;
    st->offset = 0;

    st->buffer[st->offset++] = (tslid >> 8);
    st->buffer[st->offset++] = (tslid & 0xff);
    st->buffer[st->offset++] = TSL_TYPE_STRUCT;
    // length
    st->buffer[st->offset++] = st->value_len / 0x100;
    st->buffer[st->offset++] = st->value_len % 0x100;

    return SUCCESS;
}

int mcu_tsl_struct_add_item(unsigned short tslid, mcu_tsl_struct_t *st, unsigned char type, unsigned char *value, unsigned short len)
{
    if (st == NULL || value == NULL || len == 0)
        return ERROR;

    if (st->offset + len + 5 > st->buffer_len)
        return ERROR;
    switch (type)
    {
    case TSL_TYPE_RAW:
        st->buffer[st->offset++] = (tslid >> 8);
        st->buffer[st->offset++] = (tslid & 0xff);
        st->buffer[st->offset++] = TSL_TYPE_RAW;
        // length
        st->buffer[st->offset++] = len / 0x100;
        st->buffer[st->offset++] = len % 0x100;
        my_memcpy(&st->buffer[st->offset], value, len);
        st->offset += len;

        // 更新总长度
        st->value_len+=len+5;
        st->buffer[3] = st->value_len / 0x100;
        st->buffer[4] = st->value_len % 0x100;
        break;
    case TSL_TYPE_BOOL:
        st->buffer[st->offset++] = (tslid >> 8);
        st->buffer[st->offset++] = (tslid & 0xff);
        st->buffer[st->offset++] = TSL_TYPE_BOOL;
        // length
        st->buffer[st->offset++] = 0;
        st->buffer[st->offset++] = 1;
        st->buffer[st->offset++] = (value[0] == FALSE) ? FALSE : 1;

        // 更新总长度
        st->value_len+=len+5;
        st->buffer[3] = st->value_len / 0x100;
        st->buffer[4] = st->value_len % 0x100;
        break;
    case TSL_TYPE_VALUE:
        st->buffer[st->offset++] = (tslid >> 8);
        st->buffer[st->offset++] = (tslid & 0xff);
        st->buffer[st->offset++] = TSL_TYPE_VALUE;
        // length
        st->buffer[st->offset++] = 0;
        st->buffer[st->offset++] = 4;
        st->buffer[st->offset++] = value[3];
        st->buffer[st->offset++] = value[2];
        st->buffer[st->offset++] = value[1];
        st->buffer[st->offset++] = value[0];

        // 更新总长度
        st->value_len+=len+5;
        st->buffer[3] = st->value_len / 0x100;
        st->buffer[4] = st->value_len % 0x100;
        break;

    case TSL_TYPE_STRING:
        st->buffer[st->offset++] = (tslid >> 8);
        st->buffer[st->offset++] = (tslid & 0xff);
        st->buffer[st->offset++] = TSL_TYPE_STRING;
        // length
        st->buffer[st->offset++] = len / 0x100;
        st->buffer[st->offset++] = len % 0x100;
        my_memcpy(&st->buffer[st->offset], value, len);
        st->offset += len;

        // 更新总长度
        st->value_len+=len+5;
        st->buffer[3] = st->value_len / 0x100;
        st->buffer[4] = st->value_len % 0x100;

        break;
    case TSL_TYPE_DOUBLE:
        st->buffer[st->offset++] = (tslid >> 8);
        st->buffer[st->offset++] = (tslid & 0xff);
        st->buffer[st->offset++] = TSL_TYPE_DOUBLE;
        // length
        st->buffer[st->offset++] = 0;
        st->buffer[st->offset++] = 8;
        st->buffer[st->offset++] = value[7];
        st->buffer[st->offset++] = value[6];
        st->buffer[st->offset++] = value[5];
        st->buffer[st->offset++] = value[4];
        st->buffer[st->offset++] = value[3];
        st->buffer[st->offset++] = value[2];
        st->buffer[st->offset++] = value[1];
        st->buffer[st->offset++] = value[0];

        // 更新总长度
        st->value_len+=len+5;
        st->buffer[3] = st->value_len / 0x100;
        st->buffer[4] = st->value_len % 0x100;

        break;
    default:
        break;
    }
    return SUCCESS;
}

int mcu_tsl_struct_parser(mcu_tsl_struct_t *st, unsigned char *buffer, unsigned short buffer_len)
{

    if (st == NULL || buffer == NULL || buffer_len == 0)
        return ERROR;

    st->buffer = buffer;
    st->buffer_len = buffer_len;
    st->offset = 0;
    st->value_len = 0;

    return SUCCESS;
}

int mcu_tsl_struct_get_item(mcu_tsl_struct_t *st, unsigned short *tslid, unsigned char *type, unsigned char *value, unsigned short *len)
{
    unsigned short offset = 0;
    unsigned short value_len = 0;
    
    if (st == NULL || tslid == NULL || type == NULL || value == NULL || len == NULL)
        return ERROR;
    if ((st->offset+5) > st->buffer_len)
        return ERROR;

    *tslid = (st->buffer[st->offset] << 8) | st->buffer[st->offset+1];
    st->offset += 2;
    *type = st->buffer[st->offset++];
    value_len = (st->buffer[st->offset] << 8) | st->buffer[st->offset+1];
    st->offset += 2;
    *len = value_len;

    offset = st->offset + 5;

    my_memcpy(value, &st->buffer[offset], value_len);
    st->offset += value_len;

    return SUCCESS;
}

/**
 * @brief  bool型tsl数据同步上传
 * @param[in] {tslid} tslid号
 * @param[in] {value} 当前tsl值指针
 * @return Null
 * @note   Null
 */
unsigned char mcu_tsl_bool_update_syn(unsigned short tslid,unsigned char value)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    
    send_len = set_wifi_uart_byte(send_len, (tslid >> 8));
    send_len = set_wifi_uart_byte(send_len, tslid & 0xff);
    send_len = set_wifi_uart_byte(send_len,TSL_TYPE_BOOL);
    //
    send_len = set_wifi_uart_byte(send_len,0);
    send_len = set_wifi_uart_byte(send_len,1);
    //
    if(value == FALSE) {
        send_len = set_wifi_uart_byte(send_len,FALSE);
    }else {
        send_len = set_wifi_uart_byte(send_len,1);
    }
    
    wifi_uart_write_frame(STATE_UPLOAD_SYN_CMD, MCU_TX_VER, send_len);
    
    return SUCCESS;
}

/**
 * @brief  value型tsl数据同步上传
 * @param[in] {tslid} tslid号
 * @param[in] {value} 当前tsl值指针
 * @return Null
 * @note   Null
 */
unsigned char mcu_tsl_value_update_syn(unsigned short tslid,unsigned long value)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    
    send_len = set_wifi_uart_byte(send_len, (tslid >> 8));
    send_len = set_wifi_uart_byte(send_len, tslid & 0xff);
    send_len = set_wifi_uart_byte(send_len,TSL_TYPE_VALUE);
    //
    send_len = set_wifi_uart_byte(send_len,0);
    send_len = set_wifi_uart_byte(send_len,4);
    //
    send_len = set_wifi_uart_byte(send_len,value >> 24);
    send_len = set_wifi_uart_byte(send_len,value >> 16);
    send_len = set_wifi_uart_byte(send_len,value >> 8);
    send_len = set_wifi_uart_byte(send_len,value & 0xff);
    
    wifi_uart_write_frame(STATE_UPLOAD_SYN_CMD, MCU_TX_VER, send_len);
    
    return SUCCESS;
}

/**
 * @brief  fault型tsl数据同步上传
 * @param[in] {tslid} tslid号
 * @param[in] {value} 当前tsl值指针
 * @return Null
 * @note   Null
 */
unsigned char mcu_tsl_fault_update_syn(unsigned short tslid,unsigned long value)
{
    unsigned short send_len = 0;
     
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    
    send_len = set_wifi_uart_byte(send_len, (tslid >> 8));
    send_len = set_wifi_uart_byte(send_len, tslid & 0xff);
    send_len = set_wifi_uart_byte(send_len,TSL_TYPE_BITMAP);
    //
    send_len = set_wifi_uart_byte(send_len,0);
    
    if((value | 0xff) == 0xff) {
        send_len = set_wifi_uart_byte(send_len,1);
        send_len = set_wifi_uart_byte(send_len,value);
    }else if((value | 0xffff) == 0xffff) {
        send_len = set_wifi_uart_byte(send_len,2);
        send_len = set_wifi_uart_byte(send_len,value >> 8);
        send_len = set_wifi_uart_byte(send_len,value & 0xff);
    }else {
        send_len = set_wifi_uart_byte(send_len,4);
        send_len = set_wifi_uart_byte(send_len,value >> 24);
        send_len = set_wifi_uart_byte(send_len,value >> 16);
        send_len = set_wifi_uart_byte(send_len,value >> 8);
        send_len = set_wifi_uart_byte(send_len,value & 0xff);
    }    
    
    wifi_uart_write_frame(STATE_UPLOAD_SYN_CMD,MCU_TX_VER,send_len);

    return SUCCESS;
}


/**
 * @brief  mcu获取raw型下发tsl值
 * @param[in] {value} tsl数据缓冲区地址
 * @param[in] {len} tsl数据长度
 * @param[out] {value} raw 数据缓冲区地址
 * @param[out] {out_len} raw 数据长度
 * @return 当前tsl值
 * @note   Null
 */
unsigned char mcu_get_tsl_download_raw(const unsigned char value[],unsigned short len, unsigned char *out, unsigned short *out_len)
{
    unsigned short i;
    
    *out_len = len;
    for(i=0;i<len;i++) {
        out[i] = value[i];
    }
    
    return SUCCESS;
}

/**
 * @brief  mcu获取bool型下发tsl值
 * @param[in] {value} tsl数据缓冲区地址
 * @param[in] {len} tsl数据长度
 * @return 当前tsl值
 * @note   Null
 */
unsigned char mcu_get_tsl_download_bool(const unsigned char value[],unsigned short len)
{
    return(value[0]);
}
/**
 * @brief  mcu获取value型下发tsl值
 * @param[in] {value} tsl数据缓冲区地址
 * @param[in] {len} tsl数据长度
 * @return 当前tsl值
 * @note   Null
 */
unsigned long mcu_get_tsl_download_value(const unsigned char value[],unsigned short len)
{
    return(byte_to_int(value));
}

/**
 * @brief  mcu获取string型下发tsl值
 * @param[in] {value} tsl数据缓冲区地址
 * @param[in] {len} tsl数据长度
 * @param[out] {value} string 数据缓冲区地址
 * @param[out] {out_len} string 数据长度
 * @return 当前tsl值
 * @note   Null
 */
unsigned char mcu_get_tsl_download_string(const unsigned char value[],unsigned short len, unsigned char *out, unsigned short *out_len)
{
    unsigned short i;
    
    *out_len = len;
    for(i=0;i<len;i++) {
        out[i] = value[i];
    }
    
    return SUCCESS;
}

/**
 * @brief  mcu获取fault型下发tsl值
 * @param[in] {value} tsl数据缓冲区地址
 * @param[in] {len} tsl数据长度
 * @return 当前tsl值
 * @note   Null
 */
unsigned long mcu_get_tsl_download_fault(const unsigned char value[],unsigned short len)
{
    unsigned long temp = 0;

    if (len == 1)
    {
        temp = value[0];
    }
    else if (len == 2)
    {
        temp = value[0] << 8;
        temp |= value[1];
    }
    else
    {
        temp = value[0] << 24;
        temp |= value[1] << 16;
        temp |= value[2] << 8;
        temp |= value[3];
    }

    return temp;
}

/**
 * @brief  mcu获取double型下发tsl值
 * @param[in] {value} tsl数据缓冲区地址
 * @param[in] {len} tsl数据长度
 * @return 当前tsl值
 * @note   Null
 */
double mcu_get_tsl_download_double(const unsigned char value[], unsigned short len)
{
    double temp = 0;
    unsigned char *p = (unsigned char *)&temp;
    p[0] = value[7];
    p[1] = value[6];
    p[2] = value[5];
    p[3] = value[4];
    p[4] = value[3];
    p[5] = value[2];
    p[6] = value[1];
    p[7] = value[0];

    return temp;
}

/**
 * @brief  串口接收数据暂存处理
 * @param[in] {value} 串口收到的1字节数据
 * @return Null
 * @note   在MCU串口处理函数中调用该函数,并将接收到的数据作为参数传入
 */
void uart_receive_input(unsigned char value)
{
    // #error "请在串口接收中断中调用uart_receive_input(value),串口数据由MCU_SDK处理,用户请勿再另行处理,完成后删除该行" 
    
    if(1 == rx_buf_out - rx_buf_in) {
        //串口接收缓存已满
    }else if((rx_buf_in > rx_buf_out) && ((rx_buf_in - rx_buf_out) >= sizeof(wifi_uart_rx_buf))) {
        //串口接收缓存已满
    }else {
        //串口接收缓存未满
        if(rx_buf_in >= (unsigned char *)(wifi_uart_rx_buf + sizeof(wifi_uart_rx_buf))) {
            rx_buf_in = (unsigned char *)(wifi_uart_rx_buf);
        }
        
        *rx_buf_in ++ = value;
    }
}

/**
 * @brief  串口接收多个字节数据暂存处理
 * @param[in] {value} 串口要接收的数据的源地址
 * @param[in] {data_len} 串口要接收的数据的数据长度
 * @return Null
 * @note   如需要支持一次多字节缓存，可调用该函数
 */
void uart_receive_buff_input(unsigned char value[], unsigned short data_len)
{
    // #error "请在需要一次缓存多个字节串口数据处调用此函数,串口数据由MCU_SDK处理,用户请勿再另行处理,完成后删除该行" 
    
    unsigned short i = 0;
    for(i = 0; i < data_len; i++) {
        uart_receive_input(value[i]);
    }
}

/**
 * @brief  wifi串口数据处理服务
 * @param  Null
 * @return Null
 * @note   在MCU主函数while循环中调用该函数
 */
void wifi_uart_service(void)
{
    // #error "请直接在main函数的while(1){}中添加wifi_uart_service(),调用该函数不要加任何条件判断,完成后删除该行" 
    static unsigned short rx_in = 0;
    unsigned short offset = 0;
    unsigned short rx_value_len = 0;
    
    while((rx_in < sizeof(wifi_data_process_buf)) && with_data_rxbuff() > 0) {
        wifi_data_process_buf[rx_in ++] = take_byte_rxbuff();
    }
    
    if(rx_in < PROTOCOL_HEAD)
        return;
    
    while((rx_in - offset) >= PROTOCOL_HEAD) {
        if(wifi_data_process_buf[offset + HEAD_FIRST] != FRAME_FIRST) {
            offset ++;
            continue;
        }
        
        if(wifi_data_process_buf[offset + HEAD_SECOND] != FRAME_SECOND) {
            offset ++;
            continue;
        }  
        
        if(wifi_data_process_buf[offset + PROTOCOL_VERSION] != MCU_RX_VER) {
            offset += 2;
            continue;
        }      
        
        rx_value_len = wifi_data_process_buf[offset + LENGTH_HIGH] * 0x100;
        rx_value_len += (wifi_data_process_buf[offset + LENGTH_LOW] + PROTOCOL_HEAD);
        if(rx_value_len > sizeof(wifi_data_process_buf) + PROTOCOL_HEAD) {
            offset += 3;
            continue;
        }
        
        if((rx_in - offset) < rx_value_len) {
            break;
        }
        
        //数据接收完成
        if(get_check_sum((unsigned char *)wifi_data_process_buf + offset,rx_value_len - 1) != wifi_data_process_buf[offset + rx_value_len - 1]) {
            //校验出错
            //printf("crc error (crc:0x%X  but data:0x%X)\r\n",get_check_sum((unsigned char *)wifi_data_process_buf + offset,rx_value_len - 1),wifi_data_process_buf[offset + rx_value_len - 1]);
            offset += 3;
            continue;
        }
        
        extern void dump_hex(const uint8_t *buf, uint32_t size, uint32_t number);
        printf("<<:\r\n");
        dump_hex((const uint8_t *)wifi_data_process_buf + offset, rx_value_len, 8);
        printf("\r\n");
        
        data_handle(offset);
        offset += rx_value_len;
    }//end while

    rx_in -= offset;
    if(rx_in > 0) {
        my_memcpy((char *)wifi_data_process_buf, (const char *)wifi_data_process_buf + offset, rx_in);
    }
}

/**
 * @brief  协议串口初始化函数
 * @param  Null
 * @return Null
 * @note   在MCU初始化代码中调用该函数
 */
void wifi_protocol_init(void)
{
    // #error " 请在main函数中添加wifi_protocol_init()完成wifi协议初始化,并删除该行"
    rx_buf_in = (unsigned char *)wifi_uart_rx_buf;
    rx_buf_out = (unsigned char *)wifi_uart_rx_buf;
    
    stop_update_flag = DISABLE;   
    wifi_work_state = WIFI_SATE_UNKNOW;

}



/**
 * @brief  MCU获取复位wifi成功标志
 * @param  Null
 * @return 复位标志
 * -           0(RESET_WIFI_ERROR):失败
 * -           1(RESET_WIFI_SUCCESS):成功
 * @note   1:MCU主动调用mcu_reset_wifi()后调用该函数获取复位状态
 *         2:如果为模块自处理模式,MCU无须调用该函数
 */
unsigned char mcu_get_reset_wifi_flag(void)
{
    return reset_wifi_flag;
}

/**
 * @brief  MCU主动重置wifi工作模式
 * @param  Null
 * @return Null
 * @note   1:MCU主动调用,通过mcu_get_reset_wifi_flag()函数获取重置wifi是否成功
 *         2:如果为模块自处理模式,MCU无须调用该函数
 */
void mcu_reset_wifi(void)
{
    reset_wifi_flag = RESET_WIFI_ERROR;
    
    wifi_uart_write_frame(WIFI_RESET_CMD, MCU_TX_VER, 0);
}



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
unsigned char mcu_get_wifi_work_state(void)
{
    return wifi_work_state;
}

#ifdef SUPPORT_GREEN_TIME
/**
 * @brief  MCU获取格林时间
 * @param  Null
 * @return Null
 * @note   MCU需要自行调用该功能
 */
void mcu_get_green_time(void)
{
    wifi_uart_write_frame(GET_ONLINE_TIME_CMD, MCU_TX_VER, 0);
}
#endif

#ifdef SUPPORT_MCU_RTC_CHECK
/**
 * @brief  MCU获取系统时间,用于校对本地时钟
 * @param  Null
 * @return Null
 * @note   MCU主动调用完成后在mcu_write_rtctime函数内校对rtc时钟
 */
void mcu_get_system_time(void)
{
    wifi_uart_write_frame(GET_LOCAL_TIME_CMD, MCU_TX_VER, 0);
}
#endif

#ifdef WIFI_TEST_ENABLE
/**
 * @brief  mcu发起wifi功能测试
 * @param  Null
 * @return Null
 * @note   MCU需要自行调用该功能
 */
void mcu_start_wifitest(void)
{
    wifi_uart_write_frame(WIFI_TEST_CMD, MCU_TX_VER, 0);
}
#endif



#ifdef GET_WIFI_STATUS_ENABLE
/**
 * @brief  获取当前wifi联网状态
 * @param  Null
 * @return Null
 * @note   MCU需要自行调用该功能
 */
void mcu_get_wifi_connect_status(void)
{
    wifi_uart_write_frame(GET_WIFI_STATUS_CMD, MCU_TX_VER, 0);
}
#endif

#ifdef GET_MODULE_MAC_ENABLE
/**
 * @brief  获取模块MAC
 * @param  Null
 * @return Null
 * @note   MCU需要自行调用该功能
 */
void mcu_get_module_mac(void)
{
    wifi_uart_write_frame(GET_MAC_CMD, MCU_TX_VER, 0);
}
#endif





