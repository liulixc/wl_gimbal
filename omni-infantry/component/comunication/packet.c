//
// Created by Shockley on 2022/12/5.
//
#include "decode.h"
#include "string.h"
#include "stdio.h"
#include "CRC8_CRC16.h"
#include "protocol_shaob.h"
#include "fifo.h"
#include "cmsis_os.h"
#include "packet.h"
extern void append_CRC16_check_sum(uint8_t * pchMessage,uint32_t dwLength);
/*
** Descriptions: append CRC16 to the end of data
** Input: Data to CRC and append,Stream length = Data + checksum
** Output: True or False (CRC Verify Result)
*/
extern void append_CRC16_check_sum(uint8_t * pchMessage,uint32_t dwLength);
/*
** Descriptions: append CRC8 to the end of data
** Input: Data to CRC and append,Stream length = Data + checksum
** Output: True or False (CRC Verify Result)
*/
extern void append_CRC8_check_sum(unsigned char *pchMessage, unsigned int dwLength);
//USB底层发送函数

extern uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
void encode_send_data(uint16_t cmd_id, void* buf, uint16_t len);


robot_ctrl_info_t robot_ctrl;
chassis_odom_info_t chassis_odom;
extern QueueHandle_t CDC_send_queue;
msg_end_info msg_end;

void rm_queue_data(uint16_t cmd_id,void* buf,uint16_t len ) //uint8_t queue_data[128];
{
    uint16_t index = 0;
    uint8_t queue_data[128];
    memcpy(queue_data,  (void*)&cmd_id, sizeof(uint16_t));
    index +=sizeof(uint16_t);
    memcpy(queue_data + index, (void*)buf, len);
    index += len;
    xQueueSend(CDC_send_queue, queue_data, 50);
}


//把ID和消息内容从队列中取出来,发送数据
void rm_dequeue_send_data(void* buf,uint16_t len)//auto run
{
    uint16_t cmd_id;
    memcpy(&cmd_id,buf,sizeof(uint16_t));
    switch(cmd_id)
    {
        case CHASSIS_ODOM_CMD_ID:  //需要发送的数据包ID号
            encode_send_data(CHASSIS_ODOM_CMD_ID,((uint8_t*)buf+2),sizeof(chassis_odom_info_t));
            break;
        case CHASSIS_CTRL_CMD_ID:
            encode_send_data(CHASSIS_CTRL_CMD_ID,((uint8_t*)buf+2),sizeof(robot_ctrl_info_t ));
            break;
        case VISION_ID:
            encode_send_data(VISION_ID,((uint8_t*)buf+2),sizeof(vision_t));
            break;
    }
}

//实现RM协议的序列化过程
void encode_send_data(uint16_t cmd_id,void* buf ,uint16_t len)
{
    msg_end.end1=END1_SOF;
    msg_end.end2=END2_SOF;
    static uint8_t send_buf[128];  //定义128字节大小缓存数组
    uint16_t index=0;
    frame_header_struct_t referee_send_header;  //定义帧头结构体
    //初始化帧头结构体
    referee_send_header.SOF = HEADER_SOF;
    referee_send_header.data_length = len;
    referee_send_header.seq++;
    //生成CRC8校验
    append_CRC8_check_sum((uint8_t*)&referee_send_header, sizeof(frame_header_struct_t));
    memcpy(send_buf, (uint8_t*)&referee_send_header, sizeof(frame_header_struct_t));
    index += sizeof(frame_header_struct_t);
    //填充ID
    memcpy(send_buf + index, (void*)&cmd_id, sizeof(uint16_t));
    index += sizeof(uint16_t);
    //填充数据包
    memcpy(send_buf + index, (void*)buf, len);
    index += len;
    //生成CRC16校验
    append_CRC16_check_sum(send_buf, REF_HEADER_CRC_CMDID_LEN + len);
    index += sizeof(uint16_t);

    memcpy(send_buf + index,(void*)&msg_end,sizeof(msg_end_info));
    index += sizeof(msg_end_info);
    //调用底层发送函数
    CDC_Transmit_FS(send_buf, index);
}