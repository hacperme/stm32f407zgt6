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

const DOWNLOAD_CMD_S download_cmd[] = {
    // {TSLID_BATTERY_PERCENTAGE_INT, TSL_TYPE_VALUE},
    // {TSLID_REMAIN_TIME_INT, TSL_TYPE_VALUE},
    // {TSLID_REMAIN_CHARGING_TIME_INT, TSL_TYPE_VALUE},
    // {TSLID_TOTAL_INPUT_POWER_INT, TSL_TYPE_VALUE},
    // {TSLID_TOTAL_OUTPUT_POWER_INT, TSL_TYPE_VALUE},
    {TSLID_AC_INFO_STRUCT, TSL_TYPE_STRUCT},
    {TSLID_USB_DATA_STRUCT, TSL_TYPE_STRUCT},
    {TSLID_TYPEC_DATA_STRUCT, TSL_TYPE_STRUCT},
    {TSLID_DC_DATA_STRUCT, TSL_TYPE_STRUCT},
    {TSLID_LED_STATUS_INT, TSL_TYPE_VALUE},
    // {TSLID_AC_INPUT_INT, TSL_TYPE_VALUE},
    // {TSLID_DC_INPUT_INT, TSL_TYPE_VALUE},
    // {TSLID_USB_INPUT_INT, TSL_TYPE_VALUE},
    // {TSLID_TEMP_INT, TSL_TYPE_VALUE},
    {TSLID_DEVICE_STATUS_INT, TSL_TYPE_VALUE},
    {TSLID_STANDBY_TIME_INT, TSL_TYPE_VALUE},
    {TSLID_SCREEN_TIME_INT, TSL_TYPE_VALUE},
    {TSLID_AC_STANDBY_TIME_INT, TSL_TYPE_VALUE},
    {TSLID_DC_STANDBY_TIME_INT, TSL_TYPE_VALUE},
    {TSLID_AC_CHARGING_POWER_LIMIT_INT, TSL_TYPE_VALUE},
    {TSLID_BEEP_INT, TSL_TYPE_VALUE},
    {TSLID_SCREEN_BRIGHTNESS_INT, TSL_TYPE_VALUE},
    {TSLID_DEVICE_WORK_MODE_INT, TSL_TYPE_VALUE},
    {TSLID_TIMING_ARRARY, TSL_TYPE_STRUCT},
    // {TSLID_DEVICE_MODEL_STRING, TSL_TYPE_STRING},
    // {TSLID_BLUETOOTH_MAC_STRING, TSL_TYPE_STRING},
};

tsl_demo_t g_tsl_demo = {
    .battery_percentage = 50,
    .remain_time = 140,
    .remain_charging_time = 70,
    .total_input_power = 149,
    .total_output_power = 567,
    .ac_info = {
        .ac_switch= 1,
        .ac1_output = 0,
        .ac1_output_voltage = 0,
        .ac1_output_current = 0,
        .ac2_output = 0,
        .ac2_output_voltage = 0,
        .ac2_output_current = 0,

    },
    
    .usb_data = {
        .usb_switch = 1,
        .usb1_output = 0,
        .usb1_output_voltage = 0,
        .usb1_output_current = 0,
        .usb2_output = 49,
        .usb2_output_voltage = 5,
        .usb2_output_current = 10,
        .usb3_output = 0,
        .usb3_output_voltage = 0,
        .usb3_output_current = 0,
        .usb4_output = 0,
        .usb4_output_voltage = 0,
        .usb4_output_current = 0,
    },
    .typec_data = {
        .typec_switch = 1,
        .typec1_output = 49,
        .typec1_output_voltage = 5,
        .typec1_output_current = 10,
        .typec2_output = 0,
        .typec2_output_voltage = 0,
        .typec2_output_current = 0,
    },
    .dc_data = {
        .dc_switch = 0,
        .car1_output = 0,
        .car1_output_voltage = 0,
        .car1_output_current = 0,
        .car2_output = 0,
        .car2_output_voltage = 0,
        .car2_output_current = 0,
        .dc12v1_output = 0,
        .dc12v1_output_voltage = 0,
        .dc12v1_output_current = 0,
        .dc12v2_output = 0,
        .dc12v2_output_voltage = 0,
        .dc12v2_output_current = 0,  
    },
    .led_status = 2,
    .ac_input_power = 0,
    .dc_input_power = 0,
    .usb_input_power = 0,
    .temp = 36,
    .device_status = 2,
    
    .standby_time = 5,
    .screen_time = 5,
    .ac_standby_time = 5,
    .dc_standby_time = 5,
    .ac_power_limit = 100,
    .beep = 0,
    .screen_brightness = 50,
    .device_mode = 1,

    .timing={
       {0},
    },
    .device_sn = "stm32f407zgt6",
    .bluetooth_mac = "00:00:00:00:00:00",
};

#define USE_MULTT_DATAP_UPLOAD 1

#if USE_MULTT_DATAP_UPLOAD
static mcu_tsl_datap_t g_datap = {0};
unsigned char g_datap_buf[1024] = {0};
#endif


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

    extern int _write2(int fd, char *ptr, int len) ;

    _write2(1, (char *)&value, 1);
    
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

unsigned char ac_info_update(void)
{
    mcu_tsl_struct_t ac_info = {0};
    unsigned char ac_info_buffer[300] = {0};
    

    mcu_tsl_struct_init(TSLID_AC_INFO_STRUCT, &ac_info, ac_info_buffer, sizeof(ac_info_buffer));
    mcu_tsl_struct_add_item(TSLID_AC_INFO_STRUCT_AC_SWITCH_BOOL, &ac_info,
                            TSL_TYPE_BOOL, (unsigned char *)&g_tsl_demo.ac_info.ac_switch, 1);
    mcu_tsl_struct_add_item(TSLID_AC_INFO_STRUCT_AC1_OUTPUT_INT, &ac_info,
                            TSL_TYPE_VALUE, (unsigned char *)&g_tsl_demo.ac_info.ac1_output, 4);
    mcu_tsl_struct_add_item(TSLID_AC_INFO_STRUCT_AC1_OUTPUT_VOLTAGE_INT, &ac_info,
                            TSL_TYPE_VALUE, (unsigned char *)&g_tsl_demo.ac_info.ac1_output_voltage, 4);
    mcu_tsl_struct_add_item(TSLID_AC_INFO_STRUCT_AC1_OUTPUT_CURRENT_INT, &ac_info,
                            TSL_TYPE_VALUE, (unsigned char *)&g_tsl_demo.ac_info.ac1_output_current, 4);

    mcu_tsl_struct_add_item(TSLID_AC_INFO_STRUCT_AC2_OUTPUT_INT, &ac_info,
                            TSL_TYPE_VALUE, (unsigned char *)&g_tsl_demo.ac_info.ac2_output, 4);
    mcu_tsl_struct_add_item(TSLID_AC_INFO_STRUCT_AC2_OUTPUT_VOLTAGE_INT, &ac_info,
                            TSL_TYPE_VALUE, (unsigned char *)&g_tsl_demo.ac_info.ac2_output_voltage, 4);
    mcu_tsl_struct_add_item(TSLID_AC_INFO_STRUCT_AC2_OUTPUT_CURRENT_INT, &ac_info,
                            TSL_TYPE_VALUE, (unsigned char *)&g_tsl_demo.ac_info.ac2_output_current, 4);
#if USE_MULTT_DATAP_UPLOAD
    return mcu_tsl_datap_add(&g_datap, TSL_TYPE_STRUCT, 0, (const unsigned char *)&ac_info, ac_info.offset);
#else
    return mcu_tsl_struct_update(&ac_info);
#endif
}

unsigned char usb_data_update(void)
{
    mcu_tsl_struct_t ac_info = {0};
    unsigned char ac_info_buffer[300] = {0};

    mcu_tsl_struct_t *usb_data = NULL;
    unsigned char *usb_data_buffer = NULL;

    usb_data = &ac_info;
    usb_data_buffer = ac_info_buffer;

    mcu_tsl_struct_init(TSLID_USB_DATA_STRUCT, usb_data, usb_data_buffer, sizeof(ac_info_buffer));
    mcu_tsl_struct_add_item(TSLID_USB_DATA_STRUCT_USB_SWITCH_BOOL, usb_data,
                            TSL_TYPE_BOOL, (unsigned char *)&g_tsl_demo.usb_data.usb_switch, 1);
    mcu_tsl_struct_add_item(TSLID_USB_DATA_STRUCT_USB1_OUTPUT_INT, usb_data,
                            TSL_TYPE_VALUE, (unsigned char *)&g_tsl_demo.usb_data.usb1_output, 4);
    mcu_tsl_struct_add_item(TSLID_USB_DATA_STRUCT_USB1_OUTPUT_VOLTAGE_INT, usb_data,
                            TSL_TYPE_VALUE, (unsigned char *)&g_tsl_demo.usb_data.usb1_output_voltage, 4);
    mcu_tsl_struct_add_item(TSLID_USB_DATA_STRUCT_USB1_OUTPUT_CURRENT_INT, usb_data,
                            TSL_TYPE_VALUE, (unsigned char *)&g_tsl_demo.usb_data.usb1_output_current, 4);
    mcu_tsl_struct_add_item(TSLID_USB_DATA_STRUCT_USB2_OUTPUT_INT, usb_data,
                            TSL_TYPE_VALUE, (unsigned char *)&g_tsl_demo.usb_data.usb2_output, 4);
    mcu_tsl_struct_add_item(TSLID_USB_DATA_STRUCT_USB2_OUTPUT_VOLTAGE_INT, usb_data,
                            TSL_TYPE_VALUE, (unsigned char *)&g_tsl_demo.usb_data.usb2_output_voltage, 4);
    mcu_tsl_struct_add_item(TSLID_USB_DATA_STRUCT_USB2_OUTPUT_CURRENT_INT, usb_data,
                            TSL_TYPE_VALUE, (unsigned char *)&g_tsl_demo.usb_data.usb2_output_current, 4);
    mcu_tsl_struct_add_item(TSLID_USB_DATA_STRUCT_USB3_OUTPUT_INT, usb_data,
                            TSL_TYPE_VALUE, (unsigned char *)&g_tsl_demo.usb_data.usb3_output, 4);
    mcu_tsl_struct_add_item(TSLID_USB_DATA_STRUCT_USB3_OUTPUT_VOLTAGE_INT, usb_data,
                            TSL_TYPE_VALUE, (unsigned char *)&g_tsl_demo.usb_data.usb3_output_voltage, 4);
    mcu_tsl_struct_add_item(TSLID_USB_DATA_STRUCT_USB3_OUTPUT_CURRENT_INT, usb_data,                    
                            TSL_TYPE_VALUE, (unsigned char *)&g_tsl_demo.usb_data.usb3_output_current, 4);
    mcu_tsl_struct_add_item(TSLID_USB_DATA_STRUCT_USB4_OUTPUT_INT, usb_data,
                            TSL_TYPE_VALUE, (unsigned char *)&g_tsl_demo.usb_data.usb4_output, 4);
    mcu_tsl_struct_add_item(TSLID_USB_DATA_STRUCT_USB4_OUTPUT_VOLTAGE_INT, usb_data,
                            TSL_TYPE_VALUE, (unsigned char *)&g_tsl_demo.usb_data.usb4_output_voltage, 4);
    mcu_tsl_struct_add_item(TSLID_USB_DATA_STRUCT_USB4_OUTPUT_CURRENT_INT, usb_data,
                            TSL_TYPE_VALUE, (unsigned char *)&g_tsl_demo.usb_data.usb4_output_current, 4);
#if USE_MULTT_DATAP_UPLOAD
    return mcu_tsl_datap_add(&g_datap, TSL_TYPE_STRUCT, 0, (const unsigned char *)usb_data, usb_data->offset);
#else
    return mcu_tsl_struct_update(usb_data);
#endif
}

unsigned char typec_data_update(void)
{
    mcu_tsl_struct_t ac_info = {0};
    unsigned char ac_info_buffer[300] = {0};

    mcu_tsl_struct_t *typec_data = NULL;
    unsigned char *typec_data_buffer = NULL;

    typec_data = &ac_info;
    typec_data_buffer = ac_info_buffer;

    mcu_tsl_struct_init(TSLID_TYPEC_DATA_STRUCT, typec_data, typec_data_buffer, sizeof(ac_info_buffer));
    mcu_tsl_struct_add_item(TSLID_TYPEC_DATA_STRUCT_TYPEC_SWITCH_BOOL, typec_data,
                            TSL_TYPE_BOOL, (unsigned char *)&g_tsl_demo.typec_data.typec_switch, 1);
    mcu_tsl_struct_add_item(TSLID_TYPEC_DATA_STRUCT_TYPEC1_OUTPUT_INT, typec_data,
                            TSL_TYPE_VALUE, (unsigned char *)&g_tsl_demo.typec_data.typec1_output, 4);
    mcu_tsl_struct_add_item(TSLID_TYPEC_DATA_STRUCT_TYPEC1_OUTPUT_VOLTAGE_INT, typec_data,
                            TSL_TYPE_VALUE, (unsigned char *)&g_tsl_demo.typec_data.typec1_output_voltage, 4);
    mcu_tsl_struct_add_item(TSLID_TYPEC_DATA_STRUCT_TYPEC1_OUTPUT_CURRENT_INT, typec_data,
                            TSL_TYPE_VALUE, (unsigned char *)&g_tsl_demo.typec_data.typec1_output_current, 4);
    mcu_tsl_struct_add_item(TSLID_TYPEC_DATA_STRUCT_TYPEC2_OUTPUT_INT, typec_data,
                            TSL_TYPE_VALUE, (unsigned char *)&g_tsl_demo.typec_data.typec2_output, 4);
    mcu_tsl_struct_add_item(TSLID_TYPEC_DATA_STRUCT_TYPEC2_OUTPUT_VOLTAGE_INT, typec_data,
                            TSL_TYPE_VALUE, (unsigned char *)&g_tsl_demo.typec_data.typec2_output_voltage, 4);
    mcu_tsl_struct_add_item(TSLID_TYPEC_DATA_STRUCT_TYPEC2_OUTPUT_CURRENT_INT, typec_data,
                            TSL_TYPE_VALUE, (unsigned char *)&g_tsl_demo.typec_data.typec2_output_current, 4);
#if USE_MULTT_DATAP_UPLOAD
    return mcu_tsl_datap_add(&g_datap, TSL_TYPE_STRUCT, 0, (const unsigned char *)typec_data, typec_data->offset);
#else
    return mcu_tsl_struct_update(typec_data);
#endif
}


unsigned char dc_data_update(void)
{
    mcu_tsl_struct_t ac_info = {0};
    unsigned char ac_info_buffer[300] = {0};

    mcu_tsl_struct_t *dc_data = NULL;
    unsigned char *dc_data_buffer = NULL;

    dc_data = &ac_info;
    dc_data_buffer = ac_info_buffer;

    mcu_tsl_struct_init(TSLID_DC_DATA_STRUCT, dc_data, dc_data_buffer, sizeof(ac_info_buffer));
    mcu_tsl_struct_add_item(TSLID_DC_DATA_STRUCT_DC_SWITCH_BOOL, dc_data,
                            TSL_TYPE_BOOL, (unsigned char *)&g_tsl_demo.dc_data.dc_switch, 1);
    mcu_tsl_struct_add_item(TSLID_DC_DATA_STRUCT_CAR1_OUTPUT_INT, dc_data, TSL_TYPE_VALUE,
                            (unsigned char *)&g_tsl_demo.dc_data.car1_output, 4);
    mcu_tsl_struct_add_item(TSLID_DC_DATA_STRUCT_CAR1_OUTPUT_VOLTAGE_INT, dc_data, TSL_TYPE_VALUE,
                            (unsigned char *)&g_tsl_demo.dc_data.car1_output_voltage, 4);
    mcu_tsl_struct_add_item(TSLID_DC_DATA_STRUCT_CAR1_OUTPUT_CURRENT_INT, dc_data, TSL_TYPE_VALUE,
                            (unsigned char *)&g_tsl_demo.dc_data.car1_output_current, 4);
    mcu_tsl_struct_add_item(TSLID_DC_DATA_STRUCT_CAR2_OUTPUT_INT, dc_data, TSL_TYPE_VALUE,
                            (unsigned char *)&g_tsl_demo.dc_data.car2_output, 4);
    mcu_tsl_struct_add_item(TSLID_DC_DATA_STRUCT_CAR2_OUTPUT_VOLTAGE_INT, dc_data, TSL_TYPE_VALUE,
                            (unsigned char *)&g_tsl_demo.dc_data.car2_output_voltage, 4);
    mcu_tsl_struct_add_item(TSLID_DC_DATA_STRUCT_CAR2_OUTPUT_CURRENT_INT, dc_data, TSL_TYPE_VALUE,
                            (unsigned char *)&g_tsl_demo.dc_data.car2_output_current, 4);
    mcu_tsl_struct_add_item(TSLID_DC_DATA_STRUCT_DC12V1_OUTPUT_INT, dc_data, TSL_TYPE_VALUE,
                            (unsigned char *)&g_tsl_demo.dc_data.dc12v1_output, 4);
    mcu_tsl_struct_add_item(TSLID_DC_DATA_STRUCT_DC12V1_OUTPUT_VOLTAGE_INT, dc_data, TSL_TYPE_VALUE,
                            (unsigned char *)&g_tsl_demo.dc_data.dc12v1_output_voltage, 4);
    mcu_tsl_struct_add_item(TSLID_DC_DATA_STRUCT_DC12V1_OUTPUT_CURRENT_INT, dc_data, TSL_TYPE_VALUE,
                            (unsigned char *)&g_tsl_demo.dc_data.dc12v1_output_current, 4);
    mcu_tsl_struct_add_item(TSLID_DC_DATA_STRUCT_DC12V2_OUTPUT_INT, dc_data, TSL_TYPE_VALUE,
                            (unsigned char *)&g_tsl_demo.dc_data.dc12v2_output, 4);
    mcu_tsl_struct_add_item(TSLID_DC_DATA_STRUCT_DC12V2_OUTPUT_VOLTAGE_INT, dc_data, TSL_TYPE_VALUE,
                            (unsigned char *)&g_tsl_demo.dc_data.dc12v2_output_voltage, 4);
    mcu_tsl_struct_add_item(TSLID_DC_DATA_STRUCT_DC12V2_OUTPUT_CURRENT_INT, dc_data, TSL_TYPE_VALUE,
                            (unsigned char *)&g_tsl_demo.dc_data.dc12v2_output_current, 4);
#if USE_MULTT_DATAP_UPLOAD
    return mcu_tsl_datap_add(&g_datap, TSL_TYPE_STRUCT, 0, (const unsigned char *)dc_data, dc_data->offset);
#else
    return mcu_tsl_struct_update(dc_data);
#endif
}

unsigned char timing_update(void)
{
    mcu_tsl_struct_t ac_info = {0};
    unsigned char ac_info_buffer[300] = {0};

    mcu_tsl_struct_t *timing = NULL;
    unsigned char *timing_buffer = NULL;

    mcu_tsl_struct_t timing_item = {0};
    unsigned char timing_item_buffer[100] = {0};

    timing = &ac_info;
    timing_buffer = ac_info_buffer;

    mcu_tsl_struct_init(TSLID_TIMING_ARRARY, timing, timing_buffer, sizeof(ac_info_buffer));
    for (size_t i = 0; i < 10; i++)
    {
        mcu_tsl_struct_init(0, &timing_item, timing_item_buffer, sizeof(timing_item_buffer));
        mcu_tsl_struct_add_item(TSLID_TIMING_ARRARY_TIME_INT, &timing_item, TSL_TYPE_VALUE,
                                (unsigned char *)&g_tsl_demo.timing[i].time, 4);
        mcu_tsl_struct_add_item(TSLID_TIMING_ARRARY_ACTION_BOOL, &timing_item, TSL_TYPE_BOOL,
                                (unsigned char *)&g_tsl_demo.timing[i].action, 1);
        mcu_tsl_struct_add_item(TSLID_TIMING_ARRARY_ACTION_STATUS_BOOL, &timing_item, TSL_TYPE_BOOL,
                                (unsigned char *)&g_tsl_demo.timing[i].action_status, 1);
        mcu_tsl_struct_add_struct(timing, &timing_item);
    }
#if USE_MULTT_DATAP_UPLOAD
    return mcu_tsl_datap_add(&g_datap, TSL_TYPE_STRUCT, 0, (const unsigned char *)timing, timing->offset);
#else
    return mcu_tsl_struct_update(timing);
#endif
}


/**
 * @brief  系统所有tsl点信息上传,实现APP和muc数据同步
 * @param  Null
 * @return Null
 * @note   此函数SDK内部需调用，MCU必须实现该函数内数据上报功能，包括只上报和可上报可下发型数据
 */
void all_data_update(void)
{

#if USE_MULTT_DATAP_UPLOAD
    mcu_tsl_datap_init(&g_datap, g_datap_buf, sizeof(g_datap_buf));

    mcu_tsl_datap_add(&g_datap, TSL_TYPE_VALUE, TSLID_BATTERY_PERCENTAGE_INT, (const unsigned char *)&g_tsl_demo.battery_percentage, 4);
    mcu_tsl_datap_add(&g_datap, TSL_TYPE_VALUE, TSLID_REMAIN_TIME_INT, (const unsigned char *)&g_tsl_demo.remain_time, 4);
    mcu_tsl_datap_add(&g_datap, TSL_TYPE_VALUE, TSLID_REMAIN_CHARGING_TIME_INT, (const unsigned char *)&g_tsl_demo.remain_charging_time, 4);
    mcu_tsl_datap_add(&g_datap, TSL_TYPE_VALUE, TSLID_TOTAL_INPUT_POWER_INT, (const unsigned char *)&g_tsl_demo.total_input_power, 4);
    mcu_tsl_datap_add(&g_datap, TSL_TYPE_VALUE, TSLID_TOTAL_OUTPUT_POWER_INT, (const unsigned char *)&g_tsl_demo.total_output_power, 4);


    ac_info_update();

    usb_data_update();

    typec_data_update();

    dc_data_update();

    mcu_tsl_datap_add(&g_datap, TSL_TYPE_VALUE, TSLID_LED_STATUS_INT, (const unsigned char *)&g_tsl_demo.led_status, 4);
    mcu_tsl_datap_add(&g_datap, TSL_TYPE_VALUE, TSLID_AC_INPUT_INT, (const unsigned char *)&g_tsl_demo.ac_input_power, 4);
    mcu_tsl_datap_add(&g_datap, TSL_TYPE_VALUE, TSLID_DC_INPUT_INT, (const unsigned char *)&g_tsl_demo.dc_input_power, 4);
    mcu_tsl_datap_add(&g_datap, TSL_TYPE_VALUE, TSLID_TEMP_INT, (const unsigned char *)&g_tsl_demo.temp, 4);
    mcu_tsl_datap_add(&g_datap, TSL_TYPE_VALUE, TSLID_DEVICE_STATUS_INT, (const unsigned char *)&g_tsl_demo.device_status, 4);
    mcu_tsl_datap_add(&g_datap, TSL_TYPE_VALUE, TSLID_STANDBY_TIME_INT, (const unsigned char *)&g_tsl_demo.standby_time, 4);
    mcu_tsl_datap_add(&g_datap, TSL_TYPE_VALUE, TSLID_SCREEN_TIME_INT, (const unsigned char *)&g_tsl_demo.screen_time, 4);
    mcu_tsl_datap_add(&g_datap, TSL_TYPE_VALUE, TSLID_AC_STANDBY_TIME_INT, (const unsigned char *)&g_tsl_demo.ac_standby_time, 4);
    mcu_tsl_datap_add(&g_datap, TSL_TYPE_VALUE, TSLID_DC_STANDBY_TIME_INT, (const unsigned char *)&g_tsl_demo.dc_standby_time, 4);
    mcu_tsl_datap_add(&g_datap, TSL_TYPE_VALUE, TSLID_AC_CHARGING_POWER_LIMIT_INT, (const unsigned char *)&g_tsl_demo.ac_power_limit, 4);
    mcu_tsl_datap_add(&g_datap, TSL_TYPE_VALUE, TSLID_BEEP_INT, (const unsigned char *)&g_tsl_demo.beep, 4);
    mcu_tsl_datap_add(&g_datap, TSL_TYPE_VALUE, TSLID_SCREEN_BRIGHTNESS_INT, (const unsigned char *)&g_tsl_demo.screen_brightness, 4);
    mcu_tsl_datap_add(&g_datap, TSL_TYPE_VALUE, TSLID_DEVICE_WORK_MODE_INT, (const unsigned char *)&g_tsl_demo.device_mode, 4);

    timing_update();

    mcu_tsl_datap_add(&g_datap, TSL_TYPE_STRING, TSLID_DEVICE_MODEL_STRING, (const unsigned char *)g_tsl_demo.device_sn, my_strlen((unsigned char *)g_tsl_demo.device_sn));
    mcu_tsl_datap_add(&g_datap, TSL_TYPE_STRING, TSLID_BLUETOOTH_MAC_STRING, (const unsigned char *)g_tsl_demo.bluetooth_mac, my_strlen((unsigned char *)g_tsl_demo.bluetooth_mac));

    mcu_tsl_datap_update(&g_datap, 0);

#else
    mcu_tsl_value_update(TSLID_BATTERY_PERCENTAGE_INT, g_tsl_demo.battery_percentage);
    mcu_tsl_value_update(TSLID_REMAIN_TIME_INT, g_tsl_demo.remain_time);
    mcu_tsl_value_update(TSLID_REMAIN_CHARGING_TIME_INT, g_tsl_demo.remain_charging_time);
    mcu_tsl_value_update(TSLID_TOTAL_INPUT_POWER_INT, g_tsl_demo.total_input_power);
    mcu_tsl_value_update(TSLID_TOTAL_OUTPUT_POWER_INT, g_tsl_demo.total_output_power);

    ac_info_update();

    usb_data_update();

    typec_data_update();

    dc_data_update();

    mcu_tsl_value_update(TSLID_LED_STATUS_INT, g_tsl_demo.led_status);
    mcu_tsl_value_update(TSLID_AC_INPUT_INT, g_tsl_demo.ac_input_power);
    mcu_tsl_value_update(TSLID_DC_INPUT_INT, g_tsl_demo.dc_input_power);
    mcu_tsl_value_update(TSLID_USB_INPUT_INT, g_tsl_demo.usb_input_power);
    mcu_tsl_value_update(TSLID_TEMP_INT, g_tsl_demo.temp);
    mcu_tsl_value_update(TSLID_DEVICE_STATUS_INT, g_tsl_demo.device_status);
   
    mcu_tsl_value_update(TSLID_STANDBY_TIME_INT, g_tsl_demo.standby_time);
    mcu_tsl_value_update(TSLID_SCREEN_TIME_INT, g_tsl_demo.screen_time);
    mcu_tsl_value_update(TSLID_AC_STANDBY_TIME_INT, g_tsl_demo.ac_standby_time);
    mcu_tsl_value_update(TSLID_DC_STANDBY_TIME_INT, g_tsl_demo.dc_standby_time);
    mcu_tsl_value_update(TSLID_AC_CHARGING_POWER_LIMIT_INT, g_tsl_demo.ac_power_limit);
    mcu_tsl_value_update(TSLID_BEEP_INT, g_tsl_demo.beep);
    mcu_tsl_value_update(TSLID_SCREEN_BRIGHTNESS_INT, g_tsl_demo.screen_brightness);
    mcu_tsl_value_update(TSLID_DEVICE_WORK_MODE_INT, g_tsl_demo.device_mode);

    timing_update();

    mcu_tsl_string_update(TSLID_DEVICE_MODEL_STRING,
     (const unsigned char *)g_tsl_demo.device_sn, my_strlen((unsigned char *)g_tsl_demo.device_sn));
    mcu_tsl_string_update(TSLID_BLUETOOTH_MAC_STRING,
     (const unsigned char *)g_tsl_demo.bluetooth_mac, my_strlen((unsigned char *)g_tsl_demo.bluetooth_mac));

#endif
}



/*****************************************************************************
函数名称 : tsl_download_led_status_int_handle
功能描述 : 针对 TSLID_LED_STATUS_INT 的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char tsl_download_led_status_int_handle(const unsigned char value[], unsigned short length)
{
    unsigned char ret = SUCCESS;
    
    g_tsl_demo.led_status = mcu_get_tsl_download_value(value,length);


    printf("set g_tsl_demo.led_status:%d\r\n", g_tsl_demo.led_status);
    
    //There should be a report after processing the tsl
    ret = mcu_tsl_value_update(TSLID_LED_STATUS_INT,g_tsl_demo.led_status);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}

/*****************************************************************************
函数名称 : tsl_download_device_status_int_handle
功能描述 : 针对 TSLID_DEVICE_STATUS_INT 的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char tsl_download_device_status_int_handle(const unsigned char value[], unsigned short length)
{
    unsigned char ret = SUCCESS;
    
    g_tsl_demo.device_status = mcu_get_tsl_download_value(value,length);


    printf("set g_tsl_demo.device_status:%d\r\n", g_tsl_demo.device_status);
    
    //There should be a report after processing the tsl
    ret = mcu_tsl_value_update(TSLID_DEVICE_STATUS_INT,g_tsl_demo.device_status);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}




/*****************************************************************************
函数名称 : tsl_download_standby_time_int_handle
功能描述 : 针对 TSLID_STANDBY_TIME_INT 的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char tsl_download_standby_time_int_handle(const unsigned char value[], unsigned short length)
{
    unsigned char ret = SUCCESS;
    
    g_tsl_demo.standby_time = mcu_get_tsl_download_value(value,length);


    printf("set g_tsl_demo.standby_time:%d\r\n", g_tsl_demo.standby_time);
    
    //There should be a report after processing the tsl
    ret = mcu_tsl_value_update(TSLID_STANDBY_TIME_INT, g_tsl_demo.standby_time);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}


/*****************************************************************************
函数名称 : tsl_download_standby_time_int_handle
功能描述 : 针对 TSLID_SCREEN_TIME_INT 的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char tsl_download_screen_time_int_handle(const unsigned char value[], unsigned short length)
{
    unsigned char ret = SUCCESS;
    
    g_tsl_demo.screen_time = mcu_get_tsl_download_value(value,length);


    printf("set g_tsl_demo.screen_time:%d\r\n", g_tsl_demo.screen_time);
    
    //There should be a report after processing the tsl
    ret = mcu_tsl_value_update(TSLID_SCREEN_TIME_INT, g_tsl_demo.screen_time);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}

/*****************************************************************************
函数名称 : tsl_download_ac_standby_time_int_handle
功能描述 : 针对 TSLID_AC_STANDBY_TIME_INT 的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char tsl_download_ac_standby_time_int_handle(const unsigned char value[], unsigned short length)
{
    unsigned char ret = SUCCESS;
    
    g_tsl_demo.ac_standby_time = mcu_get_tsl_download_value(value,length);


    printf("set g_tsl_demo.ac_standby_time:%d\r\n", g_tsl_demo.ac_standby_time);
    
    //There should be a report after processing the tsl
    ret = mcu_tsl_value_update(TSLID_AC_STANDBY_TIME_INT, g_tsl_demo.ac_standby_time);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}

/*****************************************************************************
函数名称 : tsl_download_dc_standby_time_int_handle
功能描述 : 针对 TSLID_DC_STANDBY_TIME_INT 的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char tsl_download_dc_standby_time_int_handle(const unsigned char value[], unsigned short length)
{
    unsigned char ret = SUCCESS;
    
    g_tsl_demo.dc_standby_time = mcu_get_tsl_download_value(value,length);


    printf("set g_tsl_demo.dc_standby_time:%d\r\n", g_tsl_demo.dc_standby_time);
    
    //There should be a report after processing the tsl
    ret = mcu_tsl_value_update(TSLID_DC_STANDBY_TIME_INT, g_tsl_demo.dc_standby_time);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}

/*****************************************************************************
函数名称 : tsl_download_ac_charging_power_limit_int_handle
功能描述 : 针对 TSLID_AC_CHARGING_POWER_LIMIT_INT 的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char tsl_download_ac_charging_power_limit_int_handle(const unsigned char value[], unsigned short length)
{
    unsigned char ret = SUCCESS;
    
    g_tsl_demo.ac_power_limit = mcu_get_tsl_download_value(value,length);


    printf("set g_tsl_demo.ac_power_limit:%d\r\n", g_tsl_demo.ac_power_limit);
    
    //There should be a report after processing the tsl
    ret = mcu_tsl_value_update(TSLID_AC_CHARGING_POWER_LIMIT_INT, g_tsl_demo.ac_power_limit);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}

/*****************************************************************************
函数名称 : tsl_download_beep_int_handle
功能描述 : 针对 TSLID_BEEP_INT 的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char tsl_download_beep_int_handle(const unsigned char value[], unsigned short length)
{
    unsigned char ret = SUCCESS;
    
    g_tsl_demo.beep = mcu_get_tsl_download_value(value,length);


    printf("set g_tsl_demo.beep:%d\r\n", g_tsl_demo.screen_time);
    
    //There should be a report after processing the tsl
    ret = mcu_tsl_value_update(TSLID_BEEP_INT, g_tsl_demo.beep);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}

/*****************************************************************************
函数名称 : tsl_download_screen_brightness_int_handle
功能描述 : 针对 TSLID_SCREEN_BRIGHTNESS_INT 的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char tsl_download_screen_brightness_int_handle(const unsigned char value[], unsigned short length)
{
    unsigned char ret = SUCCESS;
    
    g_tsl_demo.screen_brightness = mcu_get_tsl_download_value(value,length);


    printf("set g_tsl_demo.screen_brightness:%d\r\n", g_tsl_demo.screen_brightness);
    
    //There should be a report after processing the tsl
    ret = mcu_tsl_value_update(TSLID_SCREEN_BRIGHTNESS_INT, g_tsl_demo.screen_brightness);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}


/*****************************************************************************
函数名称 : tsl_download_device_work_mode_int_handle
功能描述 : 针对 TSLID_DEVICE_WORK_MODE_INT 的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char tsl_download_device_work_mode_int_handle(const unsigned char value[], unsigned short length)
{
    unsigned char ret = SUCCESS;
    
    g_tsl_demo.device_mode = mcu_get_tsl_download_value(value,length);


    printf("set g_tsl_demo.device_mode:%d\r\n", g_tsl_demo.device_mode);
    
    //There should be a report after processing the tsl
    ret = mcu_tsl_value_update(TSLID_DEVICE_WORK_MODE_INT, g_tsl_demo.device_mode);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}







/*****************************************************************************
函数名称 : tsl_download_ac_info_struct_handle
功能描述 : 针对 TSLID_AC_INFO_STRUCT 的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char tsl_download_ac_info_struct_handle(const unsigned char value[], unsigned short length)
{
    unsigned char ret = SUCCESS;

    // 解析下发结构体数据变量定义
    mcu_tsl_struct_t st = {0};
    unsigned short tslid;
    unsigned char tsl_type;
    unsigned char tsl_value[100] = {0};
    unsigned short tsl_length = 0;

       // 解析结构体数据
    mcu_tsl_struct_parser(&st, (unsigned char *)value, length);

    // 循环迭代解析结构体的每个成员数据
    while (mcu_tsl_struct_get_item(&st, &tslid, &tsl_type, tsl_value, &tsl_length))
    {
        switch (tslid)
        {
        case TSLID_AC_INFO_STRUCT_AC_SWITCH_BOOL:
            g_tsl_demo.ac_info.ac_switch = mcu_get_tsl_download_bool(tsl_value, tsl_length);
            printf("set g_tsl_demo.ac_info.ac_switch:%d\r\n", g_tsl_demo.ac_info.ac_switch);
        break;
        case TSLID_AC_INFO_STRUCT_AC1_OUTPUT_INT:
            g_tsl_demo.ac_info.ac1_output = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.ac_info.ac1_output:%d\r\n", g_tsl_demo.ac_info.ac1_output);
        break;
        case TSLID_AC_INFO_STRUCT_AC1_OUTPUT_VOLTAGE_INT:
            g_tsl_demo.ac_info.ac1_output_voltage = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.ac_info.ac1_output_voltage:%d\r\n", g_tsl_demo.ac_info.ac1_output_voltage);
        break;
        case TSLID_AC_INFO_STRUCT_AC1_OUTPUT_CURRENT_INT:
            g_tsl_demo.ac_info.ac1_output_current = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.ac_info.ac1_output_current:%d\r\n", g_tsl_demo.ac_info.ac1_output_current);
        break;
        case TSLID_AC_INFO_STRUCT_AC2_OUTPUT_INT:
            g_tsl_demo.ac_info.ac2_output = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.ac_info.ac2_output:%d\r\n", g_tsl_demo.ac_info.ac2_output);
        break;
        case TSLID_AC_INFO_STRUCT_AC2_OUTPUT_VOLTAGE_INT:
            g_tsl_demo.ac_info.ac2_output_voltage = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.ac_info.ac2_output_voltage:%d\r\n", g_tsl_demo.ac_info.ac2_output_voltage);
        break;
        case TSLID_AC_INFO_STRUCT_AC2_OUTPUT_CURRENT_INT:
            g_tsl_demo.ac_info.ac2_output_current = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.ac_info.ac2_output_current:%d\r\n", g_tsl_demo.ac_info.ac2_output_current);
        break;

        default:
            break;
        }
    }

    // There should be a report after processing the tsl

    ret = ac_info_update();

    if (ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}


/*****************************************************************************
函数名称 : tsl_download_usb_data_struct_handle
功能描述 : 针对 TSLID_USB_DATA_STRUCT 的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char tsl_download_usb_data_struct_handle(const unsigned char value[], unsigned short length)
{
    unsigned char ret = SUCCESS;

    // 解析下发结构体数据变量定义
    mcu_tsl_struct_t st = {0};
    unsigned short tslid;
    unsigned char tsl_type;
    unsigned char tsl_value[100] = {0};
    unsigned short tsl_length = 0;

       // 解析结构体数据
    mcu_tsl_struct_parser(&st, (unsigned char *)value, length);

    // 循环迭代解析结构体的每个成员数据
    while (mcu_tsl_struct_get_item(&st, &tslid, &tsl_type, tsl_value, &tsl_length))
    {
        switch (tslid)
        {
        case TSLID_USB_DATA_STRUCT_USB_SWITCH_BOOL:
            g_tsl_demo.usb_data.usb_switch = mcu_get_tsl_download_bool(tsl_value, tsl_length);
            printf("set g_tsl_demo.usb_data.usb_switch:%d\r\n", g_tsl_demo.usb_data.usb_switch);
        break;
        case TSLID_USB_DATA_STRUCT_USB1_OUTPUT_INT:
            g_tsl_demo.usb_data.usb1_output = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.usb_data.usb1_output:%d\r\n", g_tsl_demo.usb_data.usb1_output);
        break;
        case TSLID_USB_DATA_STRUCT_USB1_OUTPUT_VOLTAGE_INT:
            g_tsl_demo.usb_data.usb1_output_voltage = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.usb_data.usb1_output_voltage:%d\r\n", g_tsl_demo.usb_data.usb1_output_voltage);
        break;
        case TSLID_USB_DATA_STRUCT_USB1_OUTPUT_CURRENT_INT:
            g_tsl_demo.usb_data.usb1_output_current = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.usb_data.usb1_output_current:%d\r\n", g_tsl_demo.usb_data.usb1_output_current);
        break;
        case TSLID_USB_DATA_STRUCT_USB2_OUTPUT_INT:
            g_tsl_demo.usb_data.usb2_output = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.usb_data.usb2_output:%d\r\n", g_tsl_demo.usb_data.usb2_output);
        break;
        case TSLID_USB_DATA_STRUCT_USB2_OUTPUT_VOLTAGE_INT:
            g_tsl_demo.usb_data.usb2_output_voltage = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.usb_data.usb2_output_voltage:%d\r\n", g_tsl_demo.usb_data.usb2_output_voltage);
        break;
        case TSLID_USB_DATA_STRUCT_USB2_OUTPUT_CURRENT_INT:
            g_tsl_demo.usb_data.usb2_output_current = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.usb_data.usb2_output_current:%d\r\n", g_tsl_demo.usb_data.usb2_output_current);
        break;
        case TSLID_USB_DATA_STRUCT_USB3_OUTPUT_INT:
            g_tsl_demo.usb_data.usb3_output = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.usb_data.usb3_output:%d\r\n", g_tsl_demo.usb_data.usb3_output);
        break;
        case TSLID_USB_DATA_STRUCT_USB3_OUTPUT_VOLTAGE_INT:
            g_tsl_demo.usb_data.usb3_output_voltage = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.usb_data.usb3_output_voltage:%d\r\n", g_tsl_demo.usb_data.usb3_output_voltage);
        break;
        case TSLID_USB_DATA_STRUCT_USB3_OUTPUT_CURRENT_INT:
            g_tsl_demo.usb_data.usb3_output_current = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.usb_data.usb3_output_current:%d\r\n", g_tsl_demo.usb_data.usb3_output_current);
        break;
        case TSLID_USB_DATA_STRUCT_USB4_OUTPUT_INT:
            g_tsl_demo.usb_data.usb4_output = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.usb_data.usb4_output:%d\r\n", g_tsl_demo.usb_data.usb4_output);
        break;
        case TSLID_USB_DATA_STRUCT_USB4_OUTPUT_VOLTAGE_INT:
            g_tsl_demo.usb_data.usb4_output_voltage = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.usb_data.usb4_output_voltage:%d\r\n", g_tsl_demo.usb_data.usb4_output_voltage);
        break;
        case TSLID_USB_DATA_STRUCT_USB4_OUTPUT_CURRENT_INT:
            g_tsl_demo.usb_data.usb4_output_current = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.usb_data.usb4_output_current:%d\r\n", g_tsl_demo.usb_data.usb4_output_current);
        break;
        
        default:
            break;
        }
    }

    // There should be a report after processing the tsl

    ret = usb_data_update();

    if (ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}


/*****************************************************************************
函数名称 : tsl_download_typec_data_struct_handle
功能描述 : 针对 TSLID_TYPEC_DATA_STRUCT 的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char tsl_download_typec_data_struct_handle(const unsigned char value[], unsigned short length)
{
    unsigned char ret = SUCCESS;

    // 解析下发结构体数据变量定义
    mcu_tsl_struct_t st = {0};
    unsigned short tslid;
    unsigned char tsl_type;
    unsigned char tsl_value[100] = {0};
    unsigned short tsl_length = 0;

       // 解析结构体数据
    mcu_tsl_struct_parser(&st, (unsigned char *)value, length);

    // 循环迭代解析结构体的每个成员数据
    while (mcu_tsl_struct_get_item(&st, &tslid, &tsl_type, tsl_value, &tsl_length))
    {
        switch (tslid)
        {
        case TSLID_TYPEC_DATA_STRUCT_TYPEC_SWITCH_BOOL:
            g_tsl_demo.typec_data.typec_switch = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.typec_data.typec_switch:%d\r\n", g_tsl_demo.typec_data.typec_switch);
        break;
        case TSLID_TYPEC_DATA_STRUCT_TYPEC1_OUTPUT_INT:
            g_tsl_demo.typec_data.typec1_output = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.typec_data.typec1_output:%d\r\n", g_tsl_demo.typec_data.typec1_output);
        break;
        case TSLID_TYPEC_DATA_STRUCT_TYPEC1_OUTPUT_VOLTAGE_INT:
            g_tsl_demo.typec_data.typec1_output_voltage = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.typec_data.typec1_output_voltage:%d\r\n", g_tsl_demo.typec_data.typec1_output_voltage);
        break;
        case TSLID_TYPEC_DATA_STRUCT_TYPEC1_OUTPUT_CURRENT_INT:
            g_tsl_demo.typec_data.typec1_output_current = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.typec_data.typec1_output_current:%d\r\n", g_tsl_demo.typec_data.typec1_output_current);
        break;
        case TSLID_TYPEC_DATA_STRUCT_TYPEC2_OUTPUT_INT:
            g_tsl_demo.typec_data.typec2_output = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.typec_data.typec2_output:%d\r\n", g_tsl_demo.typec_data.typec2_output);
        break;
        case TSLID_TYPEC_DATA_STRUCT_TYPEC2_OUTPUT_VOLTAGE_INT:
            g_tsl_demo.typec_data.typec2_output_voltage = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.typec_data.typec2_output_voltage:%d\r\n", g_tsl_demo.typec_data.typec2_output_voltage);
        break;
        case TSLID_TYPEC_DATA_STRUCT_TYPEC2_OUTPUT_CURRENT_INT:
            g_tsl_demo.typec_data.typec2_output_current = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.typec_data.typec2_output_current:%d\r\n", g_tsl_demo.typec_data.typec2_output_current);
        break;

        default:
            break;
        }
    }

    // There should be a report after processing the tsl

    ret = typec_data_update();

    if (ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}


/*****************************************************************************
函数名称 : tsl_download_dc_data_struct_handle
功能描述 : 针对 TSLID_DC_DATA_STRUCT 的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char tsl_download_dc_data_struct_handle(const unsigned char value[], unsigned short length)
{
    unsigned char ret = SUCCESS;

    // 解析下发结构体数据变量定义
    mcu_tsl_struct_t st = {0};
    unsigned short tslid;
    unsigned char tsl_type;
    unsigned char tsl_value[100] = {0};
    unsigned short tsl_length = 0;

       // 解析结构体数据
    mcu_tsl_struct_parser(&st, (unsigned char *)value, length);

    // 循环迭代解析结构体的每个成员数据
    while (mcu_tsl_struct_get_item(&st, &tslid, &tsl_type, tsl_value, &tsl_length))
    {
        switch (tslid)
        {
        case TSLID_DC_DATA_STRUCT_DC_SWITCH_BOOL:
            g_tsl_demo.dc_data.dc_switch = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.dc_data.dc_switch:%d\r\n", g_tsl_demo.dc_data.dc_switch);
        break;
        case TSLID_DC_DATA_STRUCT_CAR1_OUTPUT_INT:
            g_tsl_demo.dc_data.car1_output = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.dc_data.car1_output:%d\r\n", g_tsl_demo.dc_data.car1_output);
        break;
        case TSLID_DC_DATA_STRUCT_CAR1_OUTPUT_VOLTAGE_INT:
            g_tsl_demo.dc_data.car1_output_voltage = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.dc_data.car1_output_voltage:%d\r\n", g_tsl_demo.dc_data.car1_output_voltage);
        break;
        case TSLID_DC_DATA_STRUCT_CAR1_OUTPUT_CURRENT_INT:
            g_tsl_demo.dc_data.car1_output_current = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.dc_data.car1_output_current:%d\r\n", g_tsl_demo.dc_data.car1_output_current);
        break;
        case TSLID_DC_DATA_STRUCT_CAR2_OUTPUT_INT:
            g_tsl_demo.dc_data.car2_output = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.dc_data.car2_output:%d\r\n", g_tsl_demo.dc_data.car2_output);
        break;
        case TSLID_DC_DATA_STRUCT_CAR2_OUTPUT_VOLTAGE_INT:
            g_tsl_demo.dc_data.car2_output_voltage = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.dc_data.car2_output_voltage:%d\r\n", g_tsl_demo.dc_data.car2_output_voltage);
        break;
        case TSLID_DC_DATA_STRUCT_CAR2_OUTPUT_CURRENT_INT:
            g_tsl_demo.dc_data.car2_output_current = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.dc_data.car2_output_current:%d\r\n", g_tsl_demo.dc_data.car2_output_current);
        break;
        case TSLID_DC_DATA_STRUCT_DC12V1_OUTPUT_INT:
            g_tsl_demo.dc_data.dc12v1_output = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.dc_data.dc12v1_output:%d\r\n", g_tsl_demo.dc_data.dc12v1_output);
        break;
        case TSLID_DC_DATA_STRUCT_DC12V1_OUTPUT_VOLTAGE_INT:
            g_tsl_demo.dc_data.dc12v1_output_voltage = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.dc_data.dc12v1_output_voltage:%d\r\n", g_tsl_demo.dc_data.dc12v1_output_voltage);
        break;
        case TSLID_DC_DATA_STRUCT_DC12V1_OUTPUT_CURRENT_INT:
            g_tsl_demo.dc_data.dc12v1_output_current = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.dc_data.dc12v1_output_current:%d\r\n", g_tsl_demo.dc_data.dc12v1_output_current);
        break;
        case TSLID_DC_DATA_STRUCT_DC12V2_OUTPUT_INT:
            g_tsl_demo.dc_data.dc12v2_output = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.dc_data.dc12v2_output:%d\r\n", g_tsl_demo.dc_data.dc12v2_output);
        break;
        case TSLID_DC_DATA_STRUCT_DC12V2_OUTPUT_VOLTAGE_INT:
            g_tsl_demo.dc_data.dc12v2_output_voltage = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.dc_data.dc12v2_output_voltage:%d\r\n", g_tsl_demo.dc_data.dc12v2_output_voltage);
        break;
        case TSLID_DC_DATA_STRUCT_DC12V2_OUTPUT_CURRENT_INT:
            g_tsl_demo.dc_data.dc12v2_output_current = mcu_get_tsl_download_value(tsl_value, tsl_length);
            printf("set g_tsl_demo.dc_data.dc12v2_output_current:%d\r\n", g_tsl_demo.dc_data.dc12v2_output_current);
        break;

        default:
            break;
        }
    }

    // There should be a report after processing the tsl

    ret = dc_data_update();

    if (ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}


/*****************************************************************************
函数名称 : tsl_download_timing_arrary_handle
功能描述 : 针对 TSLID_TIMING_ARRARY 的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char tsl_download_timing_arrary_handle(const unsigned char value[], unsigned short length)
{
    unsigned char ret = SUCCESS;

    // 解析下发结构体数据变量定义
    mcu_tsl_struct_t st = {0};
    unsigned short tslid;
    unsigned char tsl_type;
    unsigned char tsl_value[100] = {0};
    unsigned short tsl_length = 0;
    int index = 0;

     mcu_tsl_struct_t item_st = {0};
    unsigned short item_tslid;
    unsigned char item_tsl_type;
    unsigned char item_tsl_value[100] = {0};
    unsigned short item_tsl_length = 0;

    // 解析数组数据
    mcu_tsl_struct_parser(&st, (unsigned char *)value, length);

    // 循环迭代解析数组的每个成员数据
    while (mcu_tsl_struct_get_item(&st, &tslid, &tsl_type, tsl_value, &tsl_length))
    {
        mcu_tsl_struct_parser(&item_st, (unsigned char *)tsl_value, tsl_length);
        while (mcu_tsl_struct_get_item(&item_st, &item_tslid, &item_tsl_type, item_tsl_value, &item_tsl_length))
        {
            switch (item_tslid)
            {
            case TSLID_TIMING_ARRARY_TIME_INT:
                g_tsl_demo.timing[index].time = mcu_get_tsl_download_value(item_tsl_value, item_tsl_length);
                printf("set g_tsl_demo.timing[%d].time:%d\r\n", index, g_tsl_demo.timing[index].time);
            break;
            case TSLID_TIMING_ARRARY_ACTION_BOOL:
                g_tsl_demo.timing[index].action = mcu_get_tsl_download_value(item_tsl_value, item_tsl_length);
                printf("set g_tsl_demo.timing[%d].action:%d\r\n", index, g_tsl_demo.timing[index].action);
            break;
            case TSLID_TIMING_ARRARY_ACTION_STATUS_BOOL:
                g_tsl_demo.timing[index].action_status = mcu_get_tsl_download_value(item_tsl_value, item_tsl_length);
                printf("set g_tsl_demo.timing[%d].action_status:%d\r\n", index, g_tsl_demo.timing[index].action_status);
            default:
                break;
            }
        }
        index++;
    }

    // There should be a report after processing the tsl

    ret = timing_update();

    if (ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}

/******************************************************************************
                                WARNING!!!    
                            2:所有数据上报处理
                            具体请用户自行实现数据处理
******************************************************************************/

/******************************************************************************
                                WARNING!!!                     
此部分函数用户请勿修改!!
******************************************************************************/

#ifdef SUPPORT_MCU_FIRM_UPDATE
/**
 * @brief  升级包大小选择
 * @param[in] {fr_type} 帧类型
 * @param[in] {package_sz} 升级包大小
 * @ref           0x00: 256byte (默认)
 * @ref           0x01: 512byte
 * @ref           0x02: 1024byte
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void mcu_upgrade_package_choose(unsigned char fr_type, unsigned char package_sz)
{
    // #error "请自行实现请自行实现升级包大小选择代码,完成后请删除该行"
    unsigned short send_len = 0;
    send_len = set_wifi_uart_byte(send_len, package_sz);
	wifi_uart_write_frame(fr_type, MCU_TX_VER, send_len);
}

/**
 * @brief  MCU进入固件升级模式
 * @param[in] {value} 固件缓冲区
 * @param[in] {position} 当前数据包在于固件位置
 * @param[in] {length} 当前固件包长度(固件包长度为0时,表示固件包发送完成)
 * @return Null
 * @note   MCU需要自行实现该功能
 */
#if 1
unsigned char mcu_firm_update_handle(const unsigned char* module_name, const unsigned char value[],unsigned long position,unsigned short length)
{
    // #error "请自行完成MCU固件升级代码,完成后请删除该行"
    if(length == 0) {
        //固件数据发送完成
        /* 接受完成需要重启进入bootloader完成升级（上报新的版本），如果不重启，FC41D询问MCU版本，而MCU仍然回复历史版本，固件将会被重复下发五次 */
    }else {
        //固件数据处理
      
    }
    
    return SUCCESS;
}

#else
// OTA示例参考代码
unsigned char mcu_firm_update_handle(const unsigned char* module_name, const unsigned char value[],unsigned long position,unsigned short length)
{
    static FILE *fp = NULL;
	static unsigned char module[32] = {0};

    if(length == 0) {
        //固件数据发送完成
        long size = ftell(fp);
        printf("=============================================================================Package Received OK! Total Length:%d Bytes(%d Kb)\r\n", size, size/1024);

        fclose(fp);
        fp = NULL;
        //这里强制终止进程，客户可以校验通过后，重启进入bootloader，如果不重启，FC41D询问MCU版本，而MCU仍然回复"1.0.0"，那么会造成固件重复下发。
        exit(0);
    }else {
        if(fp == NULL)
		{
#ifdef SUPPORT_MULTI_COMPONENT
			if (NULL != module_name)
			{
				memset(module, 0, sizeof(module));
				sprintf((char*)module, "%s.bin", module_name);
				printf("<%s:%d>module name = %s\r\n", __FILE__, __LINE__, module);/*tips*/
				fp = fopen((char*)module, "wb");
			}
			else
#endif
				fp = fopen("mcu_firmware.bin", "wb");
		}

        fseek(fp, position, SEEK_SET);
        fwrite(value, length, 1, fp);

        //固件数据处理
        if(position == 0)
        {
            printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! This is a new Transmission !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n");
        }
        printf("=============================================================================Frame Received Offset:%d -- length:%d\r\n", position, length);
    }

    return SUCCESS;
}
#endif
#endif

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
        case TSLID_AC_INFO_STRUCT:
            ret = tsl_download_ac_info_struct_handle(value, length);
        break;

        

        case TSLID_USB_DATA_STRUCT:
            ret = tsl_download_usb_data_struct_handle(value, length);
        break;

        case TSLID_TYPEC_DATA_STRUCT:
            ret = tsl_download_typec_data_struct_handle(value, length);
        break;

        case TSLID_DC_DATA_STRUCT:
            ret = tsl_download_dc_data_struct_handle(value, length);
        break;

        case TSLID_LED_STATUS_INT:
            ret = tsl_download_led_status_int_handle(value, length);
        break;

        case TSLID_DEVICE_STATUS_INT:
            ret = tsl_download_device_status_int_handle(value, length);

        case TSLID_STANDBY_TIME_INT:
            ret = tsl_download_standby_time_int_handle(value, length);
        break;

        case TSLID_SCREEN_TIME_INT:
            ret = tsl_download_screen_time_int_handle(value, length);
        break;
        case TSLID_AC_STANDBY_TIME_INT:
            ret = tsl_download_ac_standby_time_int_handle(value, length);
        break;
        case TSLID_DC_STANDBY_TIME_INT:
            ret = tsl_download_dc_standby_time_int_handle(value, length);
        break;
        case TSLID_AC_CHARGING_POWER_LIMIT_INT:
            ret = tsl_download_ac_charging_power_limit_int_handle(value, length);   
        break;
        case TSLID_BEEP_INT:
            ret = tsl_download_beep_int_handle(value, length);
        break;
        case TSLID_SCREEN_BRIGHTNESS_INT:
            ret = tsl_download_screen_brightness_int_handle(value, length);
        break;
        case TSLID_DEVICE_WORK_MODE_INT:
            ret = tsl_download_device_work_mode_int_handle(value, length);
        break;
        case TSLID_TIMING_ARRARY:
            ret = tsl_download_timing_arrary_handle(value, length);
        break;

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
            //未扫描到名称为quectel_mdev_test路由器,请检查
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
 * @ref       0x05: wifi状态 6 模组处于低功耗模式
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void get_wifi_status(unsigned char result)
{
//   #error "请自行完成获取 WIFI 状态结果代码,并删除该行"
    wifi_work_state = result;
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
        
        case 5:
            // wifi状态 6 模组处于低功耗模式
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

/**
 * @brief  获取 BLE 状态结果
 * @param[in] {result} 指示 WIFI 工作状态
 * @ref       0x00: ble状态 1 设备未配网，蓝牙未连接
 * @ref       0x01: ble状态 2 设备未配网，蓝牙已连接
 * @ref       0x02: ble状态 3 设备已配网，蓝牙未连接
 * @ref       0x03: ble状态 4 设备已配网，蓝牙已连接
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void get_ble_status(unsigned char result)
{
//   #error "请自行完成获取 WIFI 状态结果代码,并删除该行"
    ble_work_state = result;
    printf("result:%d\r\n", result);

    switch(result) {
        case 0:
            // ble状态 1 设备未配网，蓝牙未连接
            break;

        case 1:
            // ble状态 2 设备未配网，蓝牙已连接
            break;

        case 2:
            // ble状态 3 设备已配网，蓝牙未连接
            break;

        case 3:
            // ble状态 4 设备已配网，蓝牙已连接
            break;


        default:break;
    }
}

/**
 * @brief  获取当前模组的IP地址
 * @param[in] {ip} 模块 IP 数据
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void get_ip_address(unsigned char ip[])
{
    /*
        模组未获取到IP地址，ip[1]的值为0x00
        模组获取到IP后，ip[1]~ip[N-1]:为当前模组的IP地址
        N = (*(ip-2)<<8 | *(ip-1)<<0) - 1;
    */
    uint16_t length = ((*(ip-2)) << 8 | (*(ip-1)) << 0) - 1;

    printf("IP Address Length:%d\r\n", length);
    if(ip[1] == 0x00) {
        printf("Get IP address error!\r\n");
        return;
    }else {
#if 0
        for(uint8_t i=1; i<=length; i++)
            printf("i:%d -- %c\r\n", i, ip[i]);
        printf("\r\n");
#endif
    }

    char ip_addr_buff[32] = {0};
    my_memcpy(ip_addr_buff, &ip[1], length);
    printf("length:%ld -- IP:%s\r\n", my_strlen((unsigned char *)ip_addr_buff), ip_addr_buff);
}

#endif
