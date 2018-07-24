#ifndef __DATA_TYPES_H__
#define __DATA_TYPES_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sqlite3.h>

#define MEMCLEAR(x) memset(&x, 0, sizeof(x))

/* 消息标记 */
#define start_machine_t    0xAA     //开机
#define data_flow_t        0xBB     //数据采集
#define rfid_msg_t         0xCC     //RFID信息
#define command_tag_t      0xDD     //命令
#define key_msg_t          0xEE     //按键
#define other_type_t       0x00     //其他

/* 设备控制 */
#define on_led1          0
#define off_led1         1
#define on_speaker       2
#define off_speaker      3
#define on_fan           4
#define on_fan_low       5
#define on_fan_mid       6
#define on_fan_high      7
#define off_fan          8
#define on_seven_led     9
#define off_seven_led    10
#define off_machine      11

/* 命令 */
#define DATA_FLAG_ON_LED1        0x00000001
#define DATA_FLAG_OFF_LED1       0x00000002
#define DATA_FLAG_ON_SPEAKER     0x00000004
#define DATA_FLAG_OFF_SPEAKER    0x00000008
#define DATA_FLAG_ON_FAN         0x00000010
#define DATA_FLAG_FAN_LOW        0x00000020
#define DATA_FLAG_FAN_MID        0x00000040
#define DATA_FLAG_FAN_HIGH       0x00000080
#define DATA_FLAG_OFF_FAN        0x00000100
#define DATA_FLAG_ON_SEVEN_LED   0x00000200
#define DATA_FLAG_OFF_SEVEN_LED  0x00000400
#define DATA_FLAG_OFF_MACHINE    0x00000800
#define DATA_FLAG_STREAM         0x00001000
#define DATA_FLAG_GET_RECORD     0x00002000 /* 获取仓库信息 */

#define DATA_CMD_ON_LED1         0
#define DATA_CMD_OFF_LED1        1
#define DATA_CMD_ON_SPEAKER      2
#define DATA_CMD_OFF_SPEAKER     3
#define DATA_CMD_ON_FAN          4
#define DATA_CMD_FAN_LOW         5  
#define DATA_CMD_FAN_MID         6
#define DATA_CMD_FAN_HIGH        7
#define DATA_CMD_OFF_FAN         8
#define DATA_CMD_ON_SEVEN_LED    9
#define DATA_CMD_OFF_SEVEN_LED   10
#define DATA_CMD_OFF_MACHINE     11
#define DATA_CMD_ON_STREAM       12
#define DATA_CMD_OFF_STREAM      13
#define DATA_CMD_GET_RECORD      14

typedef struct _tem_t {
    uint8_t ltem; //温度低位
    uint8_t htem; //温度高位
} tem_t;

typedef struct _hum_t {
    uint8_t lhum; //湿度低位
    uint8_t hhum; //湿度高位
} hum_t;

//光照信息
typedef struct _light_t {
    uint32_t light;
} light_t;

/* 保存加速度传感器，X,Y,Z方向的加速度 */
typedef struct _acc_t {
    int8_t x;
    int8_t y;
    int8_t z;
} acc_t;

/* adc采集的值 */
typedef struct _adc_t {
    uint32_t ad0; //data from A/D channel 0
    uint32_t ad3; //data from A/D channel 1
} adc_t;

/* 设备状态 */
typedef struct _state_t {
    uint8_t led_state;       //保存LED的状态
    uint8_t fan_state;       //风扇状态
    uint8_t buzz_state;      //保存蜂鸣器的状态
    uint8_t seven_led_state; //数码管的状态
} state_t;

typedef struct _rfid_t {
    uint32_t id;           //读这个RFID id识别号的功能
    uint8_t datablock[16]; //保存RFID 的其他信息
    uint8_t purse[4];
    uint8_t eeprom[4];
} rfid_t;

typedef struct _command_t {
    uint8_t operate_id; /* define the object operated */
    uint8_t operation;  /* define the action object should do */
} command_t;

/* 保存按键的值 */
typedef struct _key_t {
    uint8_t key_all;
} mykey_t;

/* 保存环境信息 */
typedef struct _env_msg_t {
    tem_t   tem;     //温度
    hum_t   hum;     //湿度
    acc_t   acc;     //加速度传感器
    adc_t   adc;     //保存ADC的值
    light_t light;   //光照
    state_t state;   //保存设备状态
} env_msg_t;

typedef struct _other_type_d {
    uint8_t other_data[20];
} other_type_d;

typedef union _data_t {
    rfid_t    rfid;
    command_t command;
    env_msg_t env_msg;
    mykey_t   key;
    other_type_d other_msg;
} data_t;

/* 消息格式 */
typedef struct {
    uint8_t  tag;           //消息标记
    uint8_t  slave_address; //设备地址
    uint8_t  data_length;   //数据域的长度
    data_t   data;          //数据域
    uint16_t crc;
} message_t;

/* 数据库记录结构体 */
typedef struct {
    unsigned int id;
    unsigned int num;
    unsigned int time;
    char dir[100];
    unsigned int stock;
} record_t;

/* 客户端与服务器进行通讯的包 */
typedef struct data_cmd {
    int cmd;
} data_cmd_t;

#define DATA_PAKE_TYPE_MSG     0
#define DATA_PAKE_TYPE_RECORD  1

/* 服务器与客户端进行通讯的包 */
typedef struct data_pake {
    int type;
    message_t msg;
    record_t record;
} data_pake_t;

#endif