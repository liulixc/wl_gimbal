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
#include "bsp_usart.h"
#include "decode.h"
#include "Detection.h"

void usb_fifo_init();
void decode_task(void const * arg);
extern robot_ctrl_info_t robot_ctrl;

//usb fifo 控制结构体
fifo_s_t usb_fifo;

//usb fifo环形缓存区
uint8_t usb_fifo_buf[512];

//协议解包控制结构体
unpack_data_t decode_unpack_obj;

//反序列化函数
void decode_unpack_fifo_data(void);
uint16_t decode_data_solve(uint8_t *frame);
extern uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);

//协议序列化函数
void encode_send_data(uint16_t cmd_id, void* buf, uint16_t len);

frame_header_struct_t decode_receive_header;

void decode_task(void const * argument);

//协议解析函数，freertos调用
void decode_task(void const *arg)
{
    //USB FIFO 初始化
    usb_fifo_init();
    while(1)
    {
        decode_unpack_fifo_data();
        osDelay(1);
    }
}

//USB FIFO 初始化
void usb_fifo_init(void)
{
    fifo_s_init(&usb_fifo, usb_fifo_buf, 512);
}


//usb 接受中断
void usb_receiver(uint8_t *buf, uint32_t len)
{
    fifo_s_puts(&usb_fifo, (char*)buf, len);
}
int flag3=0;

//反序列化
void decode_unpack_fifo_data()
{
    uint8_t byte=0;
    uint8_t sof=HEADER_SOF;
    unpack_data_t *p_obj = &decode_unpack_obj;
    while ( fifo_s_used(&usb_fifo) )
    {
        byte = fifo_s_get(&usb_fifo);
        switch(p_obj->unpack_step)
        {
            //查找帧头
            case STEP_HEADER_SOF:
            {
                if(byte==sof)
                {
                    p_obj->unpack_step = STEP_LENGTH_LOW;
                    p_obj->protocol_packet[p_obj->index++] = byte;
                }
                else
                {
                    p_obj->index = 0;
                }
            }break;
            //获取数据长度低字节
            case STEP_LENGTH_LOW:
            {
                p_obj->data_len = byte;
                p_obj->protocol_packet[p_obj->index++] = byte;
                p_obj->unpack_step = STEP_LENGTH_HIGH;
            }break;
            //获取数字长度高字节
            case STEP_LENGTH_HIGH:
            {
                p_obj->data_len |= (byte << 8);
                p_obj->protocol_packet[p_obj->index++] = byte;

                if(p_obj->data_len < (REF_PROTOCOL_FRAME_MAX_SIZE - REF_HEADER_CRC_CMDID_LEN))
                {
                    p_obj->unpack_step = STEP_FRAME_SEQ;
                }
                else
                {
                    p_obj->unpack_step = STEP_HEADER_SOF;
                    p_obj->index = 0;
                }
            }break;
            //记录协议序列号
            case STEP_FRAME_SEQ:
            {
                p_obj->protocol_packet[p_obj->index++] = byte;
                p_obj->unpack_step = STEP_HEADER_CRC8;
            }break;
            //校验帧头
            case STEP_HEADER_CRC8:
            {
                p_obj->protocol_packet[p_obj->index++] = byte;

                if (p_obj->index == REF_PROTOCOL_HEADER_SIZE)
                {
                    if ( verify_CRC8_check_sum(p_obj->protocol_packet, REF_PROTOCOL_HEADER_SIZE) )
                    {
                        p_obj->unpack_step = STEP_DATA_CRC16;
                    }
                    else
                    {
                        p_obj->unpack_step = STEP_HEADER_SOF;
                        p_obj->index = 0;
                    }
                }
            }break;
            //校验整帧16
            case STEP_DATA_CRC16:
            {
                if (p_obj->index < (REF_HEADER_CRC_CMDID_LEN + p_obj->data_len))
                {
                    p_obj->protocol_packet[p_obj->index++] = byte;
                }
                if (p_obj->index >= (REF_HEADER_CRC_CMDID_LEN + p_obj->data_len))
                {
                    p_obj->unpack_step = STEP_HEADER_SOF;
                    p_obj->index = 0;

                    if ( verify_CRC16_check_sum(p_obj->protocol_packet, REF_HEADER_CRC_CMDID_LEN + p_obj->data_len) )
                    {
                        flag3=1;
                        //成功解析信息
                        decode_data_solve(p_obj->protocol_packet);

                    }
                }
            }break;
            //解包失败，重新查找帧头
            default:
            {
                p_obj->unpack_step = STEP_HEADER_SOF;
                p_obj->index = 0;
            }break;
        }
    }
    //供视觉调试的设计
    if(flag3==0){
        robot_ctrl.fire_command=0;
    }
    flag3=0;
}
//把frame的信息转到对应的结构体中
uint16_t decode_data_solve(uint8_t *frame)
{
    uint8_t index = 0;
    uint16_t cmd_id = 0;

    memcpy(&decode_receive_header, frame, sizeof(frame_header_struct_t));
    index += sizeof(frame_header_struct_t);

    memcpy(&cmd_id, frame + index, sizeof(uint16_t));
    index += sizeof(uint16_t);

    switch (cmd_id)
    {
        //接受控制码对应信息包
        case CHASSIS_CTRL_CMD_ID:
        {
            memcpy(&robot_ctrl, frame + index, sizeof(robot_ctrl_info_t));
            detect_handle(DETECT_AUTO_AIM);
            break;
        }
        default:
        {
            break;
        }
    }
    index += decode_receive_header.data_length + 2;
    return index;
}
