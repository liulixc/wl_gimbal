/////
//#include <launcher.h>
//#include <Detection.h>
//#include "bsp_led.h"
//#include "main.h"
//#include "cmsis_os.h"
//#include "Gimbal.h"
//#include "Chassis.h"
//#include "Cap.h"
//extern chassis_t chassis;
//extern TIM_HandleTypeDef htim5;
//extern launcher_t launcher;
//extern cap_info_t cap_info;
//extern cap2_info_t cap2;
//led_t led;
//
//uint8_t led_flowing;
///**
//  * @brief          aRGB show
//  * @param[in]      aRGB: 0xaaRRGGBB, 'aa' is alpha, 'RR' is red, 'GG' is green, 'BB' is blue
//  * @retval         none
//  */
///**
//  * @brief          显示RGB
//  * @param[in]      aRGB:0xaaRRGGBB,'aa' 是透明度,'RR'是红色,'GG'是绿色,'BB'是蓝色
//  * @retval         none
//  */
//void aRGB_led_show(uint32_t aRGB)
//{
//    static uint8_t alpha;
//    static uint16_t red,green,blue;
//
//    alpha = (aRGB & 0xFF000000) >> 24;
//    red = ((aRGB & 0x00FF0000) >> 16) * alpha;
//    green = ((aRGB & 0x0000FF00) >> 8) * alpha;
//    blue = ((aRGB & 0x000000FF) >> 0) * alpha;
//
//            __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_1, blue);
//            __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2, green);
//            __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, red);
//}
//
//#define LED_SET(n) HAL_GPIO_WritePin(LED##n##_PORT,LED##n##_PIN,GPIO_PIN_SET)
//#define LED_RESET(n) HAL_GPIO_WritePin(LED##n##_PORT,LED##n##_PIN,GPIO_PIN_RESET)
//
//void led_dashboard(){
//    //switch(led.mode){
//    //case SPIN:
//    if(chassis.mode==CHASSIS_SPIN)//灯
//    {
//        LED_RESET(6);
//    }
//    else{
//        LED_SET(6);
//    }
//    //break;
//
//    //case SHOOT:
//    if(ABS(launcher.fire_l.motor_measure->speed_rpm)>500&&ABS(launcher.fire_r.motor_measure->speed_rpm)>500)
//        HAL_GPIO_WritePin(LED5_PORT,LED5_PIN,GPIO_PIN_RESET);
//    else
//        HAL_GPIO_WritePin(LED5_PORT,LED5_PIN,GPIO_PIN_SET);
//    // break;
//
//    //case AUTO_AIM:
//    if(detect_list[DETECT_AUTO_AIM].status==OFFLINE)
//    {
//        HAL_GPIO_WritePin(LED1_PORT,LED1_PIN,GPIO_PIN_SET);
//    }
//    else{
//        HAL_GPIO_WritePin(LED1_PORT,LED1_PIN,GPIO_PIN_RESET);
//    };
//    // break;
//
//    // case CAP:
//    if(cap2.send_data[0]==0xFF)//电容开亮灯
//        HAL_GPIO_WritePin(LED2_PORT,LED2_PIN,GPIO_PIN_RESET);
//    else
//        HAL_GPIO_WritePin( LED2_PORT,LED2_PIN,GPIO_PIN_SET);
//    // }
//}
//
//
//void led_init(){
//    HAL_GPIO_WritePin(LED1_PORT,LED1_PIN,GPIO_PIN_SET);
//    HAL_GPIO_WritePin(LED3_PORT,LED3_PIN,GPIO_PIN_SET);
//    HAL_GPIO_WritePin(LED5_PORT,LED5_PIN,GPIO_PIN_SET);
//    HAL_GPIO_WritePin(LED7_PORT,LED7_PIN,GPIO_PIN_SET);
//    HAL_GPIO_WritePin(LED2_PORT,LED2_PIN,GPIO_PIN_SET);
//    HAL_GPIO_WritePin(LED4_PORT,LED4_PIN,GPIO_PIN_SET);
//    HAL_GPIO_WritePin(LED6_PORT,LED6_PIN,GPIO_PIN_SET);
//}
//
//void led_light(uint8_t led_1,uint8_t led_2,uint8_t led_3,uint8_t led_4,uint8_t led_5,uint8_t led_6,uint8_t led_7){
//    if(led_1==1){
//        HAL_GPIO_WritePin(LED1_PORT,LED1_PIN,GPIO_PIN_RESET);
//    }
//    else{
//        HAL_GPIO_WritePin(LED1_PORT,LED1_PIN,GPIO_PIN_SET);
//    }
//
//    if(led_2==1){
//        HAL_GPIO_WritePin(LED2_PORT,LED2_PIN,GPIO_PIN_RESET);
//    }
//    else{
//        HAL_GPIO_WritePin(LED2_PORT,LED2_PIN,GPIO_PIN_SET);
//    }
//
//    if(led_3==1){
//        HAL_GPIO_WritePin(LED3_PORT,LED3_PIN,GPIO_PIN_RESET);
//    }
//    else{
//        HAL_GPIO_WritePin(LED3_PORT,LED3_PIN,GPIO_PIN_SET);
//    }
//
//    if(led_4==1){
//        HAL_GPIO_WritePin(LED4_PORT,LED4_PIN,GPIO_PIN_RESET);
//    }
//    else{
//        HAL_GPIO_WritePin(LED4_PORT,LED4_PIN,GPIO_PIN_SET);
//    }
//
//    if(led_5==1){
//        HAL_GPIO_WritePin(LED5_PORT,LED5_PIN,GPIO_PIN_RESET);
//    }
//    else{
//        HAL_GPIO_WritePin(LED5_PORT,LED5_PIN,GPIO_PIN_SET);
//    }
//
//    if(led_6==1){
//        HAL_GPIO_WritePin(LED6_PORT,LED6_PIN,GPIO_PIN_RESET);
//    }
//    else{
//        HAL_GPIO_WritePin(LED6_PORT,LED6_PIN,GPIO_PIN_SET);
//    }
//
//    if(led_7==1){
//        HAL_GPIO_WritePin(LED7_PORT,LED7_PIN,GPIO_PIN_RESET);
//    }
//    else{
//        HAL_GPIO_WritePin(LED7_PORT,LED7_PIN,GPIO_PIN_SET);
//    }
//}
//
//
//void led_flow(){
//    led_flowing=1;
//    led_light(1,0,0,0,0,0,1);
//    osDelay(DELAY_TIME);
//
//    led_light(0,0,0,0,0,0,0);
//    osDelay(DELAY_TIME);
//
//    led_light(0,0,1,0,1,0,0);
//    osDelay(DELAY_TIME);
//
//    led_light(0,0,0,1,0,0,0);
//    osDelay(DELAY_TIME);
//
//    led_light(1,0,0,1,0,0,1);
//    osDelay(DELAY_TIME);
//
//    led_light(0,1,0,1,0,1,0);
//    osDelay(DELAY_TIME);
//
//    led_light(0,0,1,1,1,0,0);
//    osDelay(DELAY_TIME);
//
//    led_light(1,0,1,1,1,0,1);
//    osDelay(DELAY_TIME);
//
//    led_light(0,1,1,1,1,1,0);
//    osDelay(DELAY_TIME);
//
//    led_light(1,1,1,1,1,1,1);
//    osDelay(DELAY_TIME);
//
//    led_light(0,0,0,1,0,0,0);
//    osDelay(DELAY_TIME);
//
//    led_light(0,0,1,0,1,0,0);
//    osDelay(DELAY_TIME);
//
//    led_light(0,1,0,0,0,1,0);
//    osDelay(DELAY_TIME);
//
//    led_light(1,0,0,0,0,0,1);
//    osDelay(DELAY_TIME);
//
//    led_light(1,1,0,0,0,1,1);
//    osDelay(DELAY_TIME);
//
//    led_light(1,1,1,0,1,1,1);
//    osDelay(DELAY_TIME);
//
//    led_light(1,1,1,1,1,1,1);
//    osDelay(DELAY_TIME);
//
//    led_light(0,0,0,0,0,0,0);
//    osDelay(DELAY_TIME);
//
//    led_light(1,1,1,1,1,1,1);
//    osDelay(DELAY_TIME);
//
//    led_light(0,0,0,0,0,0,0);
//    osDelay(DELAY_TIME);
//
//    led_light(1,1,1,1,1,1,1);
//    osDelay(2000);
//
//    led_light(0,0,0,0,0,0,0);
//    led_flowing=0;
//}
//
//void led_off_flow(){
//    led_flowing=1;
//    led_light(1,1,1,1,1,1,1);
//    osDelay(DELAY_TIME);
//
//    led_light(0,0,0,0,0,0,0);
//    osDelay(DELAY_TIME);
//
//    led_light(1,1,1,1,1,1,1);
//    osDelay(DELAY_TIME);
//
//    led_light(0,1,1,1,1,1,0);
//    osDelay(DELAY_TIME);
//
//    led_light(1,0,1,1,1,0,1);
//    osDelay(DELAY_TIME);
//
//    led_light(1,1,0,1,0,1,1);
//    osDelay(DELAY_TIME);
//
//    led_light(1,1,1,0,1,1,1);
//    osDelay(DELAY_TIME);
//
//    led_light(0,1,1,0,1,1,0);
//    osDelay(DELAY_TIME);
//
//    led_light(1,0,1,0,1,0,1);
//    osDelay(DELAY_TIME);
//
//    led_light(1,1,0,0,0,1,1);
//    osDelay(DELAY_TIME);
//
//    led_light(0,1,0,0,0,1,0);
//    osDelay(DELAY_TIME);
//
//    led_light(1,0,0,0,0,0,1);
//    osDelay(DELAY_TIME);
//
//    led_light(0,1,0,0,0,1,0);
//    osDelay(DELAY_TIME);
//
//    led_light(0,0,1,0,1,0,0);
//    osDelay(DELAY_TIME);
//
//    led_light(0,0,0,1,0,0,0);
//    osDelay(DELAY_TIME);
//
//    led_light(0,0,0,0,0,0,0);
//    osDelay(DELAY_TIME);
//
//    led_light(0,0,1,0,1,0,0);
//    osDelay(DELAY_TIME);
//
//    led_light(0,1,0,0,0,1,0);
//    osDelay(DELAY_TIME);
//
//    led_light(1,0,0,0,0,0,1);
//    osDelay(DELAY_TIME);
//
//    led_light(0,0,0,0,0,0,0);
//    osDelay(DELAY_TIME);
//
//    led_light(1,0,0,0,0,0,1);
//    osDelay(DELAY_LONG_TIME);
//
//    led_light(0,0,0,0,0,0,0);
//    led_flowing=0;
//}
//
//
//
//void led_task(void const *pvParameters){
//    vTaskDelay(CHASSIS_TASK_INIT_TIME);
//    led_init();
//
//    while(1){
//        if(chassis.last_mode==CHASSIS_RELAX && chassis.mode!=CHASSIS_RELAX){
//            led_flow();
//        }
//        else if(chassis.last_mode!=CHASSIS_RELAX && chassis.mode==CHASSIS_RELAX){
//            led_off_flow();
//        }
//        if(led_flowing==0){
//            led_dashboard();
//            vTaskDelay(10);
//        }
//    }
//
//
//}
//
// 24赛季新UI灯板的新代码


// todo SITREP 24/03/29:
// 经研究发现存在如下奇怪现象：
// RGB_DAT中的红色和蓝色会控制前一位的灯珠，[0]无用，而最后一位同时控制倒数前两个灯珠
// 而绿色更奇怪，除[0]外会影响前一位
// e.g.： {255,255,255},{128,128,128}
// 显示为：{224,128,128},{128,000,000}
// 经过排查，是在HAL_SPI_Transmit_DMA()这一步才产生的，怀疑与内存对齐有关
// 奇怪的是不在FreeRTOS里也会这样
// 信号末端会有一串莫名的乱码，在全亮度下不怎么影响显示
// 好像也是内存导致的，因为每次reset完都不一样


// 作为状态显示使用时
// 1. 小陀螺 = 绿色  标志位: chassis.mode = CHASSIS_SPIN
// 2. 自瞄开 = 蓝色  标志位：detect_list[DETECT_AUTO_AIM].status = ONLINE
// 3. 发射中 = 黄色+橙色  标志位：ABS(launcher.fire_l.motor_measure->speed_rpm) > 500 && ABS(launcher.fire_r.motor_measure->speed_rpm)>500
// 4. 电容开 = 红色  标志位：cap2.send_data[0] == 0xFF
// 5. 弹舱开 = 粉色  标志位：magazine_cover_is_closed = false


// 原理：ws2812灯珠使用单线归零码通讯，不单单看高低电平，而是看高低电平的持续时间来判断0和1
// *虽然不知道板子用的灯珠具体要求多少的持续时长，但ws2812都大差不差
// 将SPI2设为Transmit only Master，模拟高低电平
//（虽然用PWM也行，而且C板的PWM口有足足7个）

#include <stdlib.h>
#include "bsp_led.h"
#include "spi.h"
#include "dma.h"
#include "main.h"
#include "cmsis_os.h"
#include "Gimbal.h"
#include "Chassis.h"
#include "Cap.h"

//声明外部变量
extern chassis_t chassis;
extern TIM_HandleTypeDef htim5;
extern launcher_t launcher;
extern cap_info_t cap_info;
extern cap2_info_t cap2;
extern bool magazine_cover_is_closed;
extern key_board_t KeyBoard;
// 常用的颜色，亮度调的比较低（这个灯珠属实是有点亮）。
//const RGBColor_TypeDef RED      = {5, 0, 0};
//const RGBColor_TypeDef GREEN    = {0, 5, 0};
//const RGBColor_TypeDef BLUE     = {0, 0, 5};
//const RGBColor_TypeDef YELLOW   = {5, 5, 0};
//const RGBColor_TypeDef MAGENTA  = {5, 0, 5};
//const RGBColor_TypeDef BLACK    = {0, 0, 0};
//const RGBColor_TypeDef WHITE    = {0, 5, 5};
//const RGBColor_TypeDef BRIGHT_WHITE    = { 255, 255, 255};

//SPI波特率为5.25M
//模拟bit码:0xC0 为 0 (10000000),0xF8 为 1(11111000)
//                    -_______            -----___  一个单位 = 1/5.25MHz = 0.19us
const uint8_t code[]={0x80,0xF8};

//灯颜色缓存区
RGBColor_TypeDef RGB_DAT[RGB_NUM];
//灯效进行状态旗
uint8_t RGB_Ongoing_flag = 1;
//旧代码里的led模式，貌似没用，但底盘云台的应用层有相关代码，就先留着好了
led_t led;

//————————————————————————————————————————————基础功能实现区————————————————————————————————————————————————

/**
  * @brief 底层SPI发送函数
  */
extern DMA_HandleTypeDef hdma_spi2_tx;
static void SPI_Send(uint8_t* SPI_RGB_BUFFER)
{
    /* 判断上次DMA有没有传输完成 */
    while(HAL_DMA_GetState(&hdma_spi2_tx) != HAL_DMA_STATE_READY);
    /* 发送一个(24bit的)RGB 数据到 2812 */
    HAL_SPI_Transmit_DMA(&hspi2,SPI_RGB_BUFFER,24);
}




/**
  * @brief 颜色设置函数
  * @description 设置LedId个灯珠的颜色缓存设置为Color色，不是直接拿来点灯的
  * @param LedId 灯珠编号，Color 颜色结构体
  */
void RGB_Set_Color(uint8_t LedId, RGBColor_TypeDef Color)
{
    if(LedId < RGB_NUM)
    {
        RGB_DAT[LedId].R = Color.R;
        RGB_DAT[LedId].G = Color.G;
        RGB_DAT[LedId].B = Color.B;
    }
}




/**
  * @brief 颜色刷新函数
  * @param refresh_NUM 要刷新的灯珠数量
  * @description 将颜色数据发送到灯珠，自带帧间隔延时
  */
void RGB_Refresh(uint8_t refresh_NUM)
{
    static uint8_t RGB_BUFFER[24];
    uint8_t data_B ,data_R ,data_G ;
    //将数组颜色转化为 24 个要发送的字节数据
    if(refresh_NUM > 0 && refresh_NUM <= RGB_NUM)
    {
        for(uint8_t i = 0; i < refresh_NUM; ++i) //todo
        {

            //全亮度版
//            data_G = RGB_DAT[i].G;
//            data_R = RGB_DAT[i].R;
//            data_B = RGB_DAT[i].B;
            //PS，2812的发送顺序是从高位起，GRB
            //用于在室内保护眼睛的版本（全亮度有点太亮了）
            int decline=8;//越大灯越暗
            data_G = RGB_DAT[i].G /decline;
            data_R = RGB_DAT[i].R /decline;
            data_B = RGB_DAT[i].B /decline;
            for (uint8_t j = 0; j < 8; j++)
            {
                RGB_BUFFER[07 - j] = code[data_G & 0x01];
                RGB_BUFFER[15 - j] = code[data_R & 0x01];
                RGB_BUFFER[23 - j] = code[data_B & 0x01];
                data_G >>= 1;
                data_R >>= 1;
                data_B >>= 1;
            }
            SPI_Send(RGB_BUFFER);
        }

//        7颗灯珠的数据发送完毕后，发送长时间低电平 标志帧间隔，以便灯珠显示。
        //库函数延时1ms（不好用）
//        vTaskDelay(1);
        //空数据延时1（不好用）
//        for (uint8_t i = 0; i < 25; ++i)
//        {
//            RGB_BUFFER[i] = 0;
//        }
//        SPI_Send(RGB_BUFFER);
        //空数据延时2（差不多能用）
        uint8_t RGB_DAT_NULL[24] = {};
        SPI_Send(RGB_DAT_NULL);
        //空数据延时3 ???
//        uint8_t RGB_DAT_NULL = 0x00;
//        for (uint8_t i = 0; i < 25; ++i)
//        {
//            SPI_Send(&RGB_DAT_NULL);
//        }

    }
}




/**
  * @brief 用于向ws2812发送数据的SPI2的发送完成回调函数
  * @description 每次发送完自增1，用来控制彩虹灯珠的顺序
  */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi->Instance == hspi2.Instance)
    {
        if (RGB_Counter == 255)
        {
            RGB_Counter = 0;
        }
        else
        {
            RGB_Counter++;
        }
    }
}




/**
  * @brief          aRGB show
  * @param[in]      aRGB: 0xaaRRGGBB, 'aa' is alpha, 'RR' is red, 'GG' is green, 'BB' is blue
  * @retval         none
  */
/**
  * @brief          显示RGB
  * @param[in]      aRGB:0xaaRRGGBB,'aa' 是透明度,'RR'是红色,'GG'是绿色,'BB'是蓝色
  * @retval         none
  */
void aRGB_led_show(uint32_t aRGB)//CV来的老代码，控制C板自带的灯
{
    static uint8_t alpha;
    static uint16_t red,green,blue;

    alpha = (aRGB & 0xFF000000) >> 24;
    red = ((aRGB & 0x00FF0000) >> 16) * alpha;
    green = ((aRGB & 0x0000FF00) >> 8) * alpha;
    blue = ((aRGB & 0x000000FF) >> 0) * alpha;
            __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_1, blue);
            __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2, green);
            __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, red);
}






//——————————————————————————————————————————高级功能实现区——————————————————————————————————————————————————
/**
  * @brief 灯板任务
  * @description FreeRTOS, RGB LED dashboard main task
  */
extern RC_ctrl_t rc_ctrl;
void led_task(void const *pvParameters)
{
    vTaskDelay(CHASSIS_TASK_INIT_TIME);
    while(1)
    {
        //——————————————————debug———————————————————————
//        led_dashboard(); 写代码用的传送门

//        chassis.mode = CHASSIS_ONLY;
//        chassis.last_mode = CHASSIS_FOLLOW_GIMBAL;
//        launcher.fire_l.motor_measure->speed_rpm = 600;
//        launcher.fire_r.motor_measure->speed_rpm = 600;
//        detect_list[DETECT_AUTO_AIM].status = OFFLINE;
//        cap2.send_data[0] = 0xFF;
//        magazine_cover_is_closed = true;
        //——————————————————end—————————————————————————

        //刚上电时直接彩虹灯效
        if(chassis.last_mode == CHASSIS_RELAX && chassis.mode == CHASSIS_RELAX)
        {
//            RGB_Debug();
            RGB_Rainbow(RGB_NUM, 1);
            RGB_Ongoing_flag = 1;
        }
        //从失能切换出来时闪一次之后开启仪表盘
        if(chassis.last_mode == CHASSIS_RELAX && chassis.mode != CHASSIS_RELAX)
        {
            if (RGB_Ongoing_flag == 1)
            {
                for (uint8_t i = 0; i < 28; ++i)
                {
                    RGB_Rainbow(RGB_NUM, 2);
                }
            }


            RGB_Ongoing_flag = 0;
        }
            //从其它模式进入失能后开启彩虹
        else if(chassis.last_mode != CHASSIS_RELAX && chassis.mode == CHASSIS_RELAX)
        {
            RGB_Rainbow(RGB_NUM, 0);
            RGB_Ongoing_flag = 1;
        }
            //其它情况下开启仪表盘
        else if(chassis.last_mode != CHASSIS_RELAX && chassis.mode != CHASSIS_RELAX)
        {
            RGB_Ongoing_flag = 0;
        }

        //进入仪表盘
        if(RGB_Ongoing_flag == 0)
        {
            led_dashboard();

        }
//        osDelay(10);
  //     led_dashboard();
        osDelay(1000);
    }
}


/**
  * @brief 灯板debug
  */
void RGB_Debug()
{
    for (int i = 0; i < RGB_NUM; ++i)
    {
        RGB_Set_Color(i, (RGBColor_TypeDef){0, 0, 0});
    }
    RGB_DAT[0].R = 255;
    RGB_DAT[0].B = 255;
    RGB_DAT[0].G = 255;
    RGB_DAT[1].R = 128;
    RGB_DAT[1].B = 128;
    RGB_DAT[1].G = 128;
    //    RGB_Set_Color(13, (RGBColor_TypeDef){0, 255, 0}); //红
//    RGB_Set_Color(1, (RGBColor_TypeDef){255, 255, 0}); //紫
//    RGB_Set_Color(2, (RGBColor_TypeDef){0, 255, 0}); //绿
//    RGB_Set_Color(3, (RGBColor_TypeDef){0, 255, 255}); //青
//    RGB_Set_Color(4, (RGBColor_TypeDef){0, 0, 255}); //蓝
//    RGB_Set_Color(5, (RGBColor_TypeDef){255, 0, 255}); //紫
//    RGB_Set_Color(6, (RGBColor_TypeDef){255, 255, 255}); //白
    RGB_Refresh(RGB_NUM);
    osDelay(500);
}


/**
  * @brief 灯板显示状态程序
  * @description 根据车车状态点亮对应灯珠
  */
extern gimbal_t gimbal;
void led_dashboard()
{
    //清空灯板
    for (uint8_t i = 0; i < RGB_NUM; ++i)
    {
        RGB_Set_Color(i, (RGBColor_TypeDef){0, 0, 0});
    }

//    led_task();写代码用的传送门
// 草稿纸：0 6 2 4 5
    //小陀螺判断
    if(chassis.mode==CHASSIS_SPIN)
    {
        RGB_Set_Color(2, (RGBColor_TypeDef){0, 255, 0});//蓝色
    }
    //发射判断
    if(ABS(launcher.fire_l.motor_measure->speed_rpm) > 500 && ABS(launcher.fire_r.motor_measure->speed_rpm) > 500)
    {
        RGB_Set_Color(6, (RGBColor_TypeDef){255, 255, 0});
    }
    //自瞄判断
//    if(detect_list[DETECT_AUTO_AIM].status == OFFLINE)
//    {
//        RGB_Set_Color(0, (RGBColor_TypeDef){0, 0, 255});
//    }
    //自瞄判断
    if(gimbal.mode==GIMBAL_AUTO||gimbal.mode==GIMBAL_BUFF||gimbal.mode==GIMBAL_SBUFF||gimbal.mode==GIMBAL_FORECAST)
    {
        RGB_Set_Color(0, (RGBColor_TypeDef){0, 0, 255});
    }
    //电容判断
    if(KeyBoard.SHIFT.status == KEY_PRESS)
    {
        RGB_Set_Color(4, (RGBColor_TypeDef){255, 0, 0});//红色
    }
    //弹舱判断
//    if(rc_ctrl.rc.ch[4]<-300)
//    {
//        RGB_Set_Color(5, (RGBColor_TypeDef){255, 0, 255});
//    }
    //将编辑好的灯板数据发送到灯板
    RGB_Refresh(RGB_NUM);
}





/**
 * @brief 单色灯效
 * @description 用单色点亮所有灯珠
 * @param RGB_LEN 灯珠数量
 * @param R 红色值 0~255
 * @param G 绿色值 0~255
 * @param B 蓝色值 0~255
 */
void RGB_Single_Color(uint16_t RGB_LEN, uint8_t R, uint8_t G, uint8_t B)
{
    for(uint8_t i=0; i<RGB_LEN;i++)
    {
        RGB_Set_Color(i, (RGBColor_TypeDef) {R, G, B});
    }
    RGB_Refresh(RGB_LEN);
}




/**
  * @brief RGB彩虹灯效
  * @description 不觉得这很酷吗?作为一名理工男我觉得这太酷了,很符合我对可寻址RGB灯效的想象,科技并带着趣味
  * @param RGB_LEN 灯珠数量，用RGB_NUM的宏定义就行
  * @param Mode 灯效模式，0为逐个渐变，1为一起渐变，2为快速逐个渐变
  */
RGBColor_TypeDef RGB_Rainbow_Value = {255,0, 0};//初始相位
uint8_t Rainbow_Status_Flag = 1; //本来是从0开始的，但是……
void RGB_Rainbow(uint8_t RGB_LEN, uint8_t MODE)
{
    uint8_t rainbow_Step = 0;

    RGB_Ongoing_flag = 1;
    if(RGB_Rainbow_Value.R == 255 && RGB_Rainbow_Value.G == 0 && RGB_Rainbow_Value.B == 0)
    {
        Rainbow_Status_Flag = 1;
    }
    if(RGB_Rainbow_Value.G == 255 && RGB_Rainbow_Value.R == 0)
    {
        Rainbow_Status_Flag = 2;
    }
    if(RGB_Rainbow_Value.B == 255 && RGB_Rainbow_Value.R == 0)
    {
        Rainbow_Status_Flag = 3;
    }

    switch (MODE)
    {
        case 0:
            rainbow_Step = 5;
            break;
        case 1:
            rainbow_Step = 3;
            break;
        case 2:
            rainbow_Step = 15;
            break;
        default:
            rainbow_Step = 1;
            break;
    }

    switch(Rainbow_Status_Flag)
    {
        case 0:
            RGB_Rainbow_Value.R = (RGB_Rainbow_Value.R + rainbow_Step) ;
            break;
        case 1:
            RGB_Rainbow_Value.R = (RGB_Rainbow_Value.R - rainbow_Step) ;
            RGB_Rainbow_Value.G = (RGB_Rainbow_Value.G + rainbow_Step) ;
            break;
        case 2:
            RGB_Rainbow_Value.G = (RGB_Rainbow_Value.G - rainbow_Step) ;
            RGB_Rainbow_Value.B = (RGB_Rainbow_Value.B + rainbow_Step) ;
            break;
        case 3:
            RGB_Rainbow_Value.B = (RGB_Rainbow_Value.B - rainbow_Step) ;
            RGB_Rainbow_Value.R = (RGB_Rainbow_Value.R + rainbow_Step) ;
            break;
        default:
            break;
    }
    switch (MODE)
    {
        case 0://逐个渐变
        case 2://快速逐个渐变
            RGB_Set_Color(RGB_Counter % RGB_NUM, RGB_Rainbow_Value);
            break;
        case 1://一起渐变
            for(uint8_t i=0;i<RGB_LEN;i++)
            {
                RGB_Set_Color(i, RGB_Rainbow_Value);
            }
            break;
        default:
            break;
    }
    RGB_Refresh(RGB_LEN);
    osDelay(50);
    RGB_Ongoing_flag = 0;
}


/**
  * @brief RGB流动灯效
  * @description 还没写完
  * @param RGB_LEN 灯珠数量
  */
void RGB_Flow(uint16_t RGB_LEN, uint8_t R, uint8_t G, uint8_t B) //todo
{
    RGB_Set_Color(RGB_NUM, (RGBColor_TypeDef){0, 0, 0});
    RGB_DAT[RGB_Counter % RGB_LEN] = (RGBColor_TypeDef){R, G, B};
    RGB_Refresh(RGB_LEN);
}


/**
  * @brief 用某种颜色点亮所有灯珠
  * @description 测试灯珠用，虽然不加柔光罩看起来像白色而不是蓝色
  */
void RGB_Test(void)
{
    uint8_t i;
    for(i=0;i<RGB_NUM;i++)
    {
        RGB_Set_Color(i, (RGBColor_TypeDef) {0x66, 0xCC, 0xFF});
    }
    RGB_Refresh(RGB_NUM);
}





//————————————————————————————————————————废代码，以备不时之需————————————————————————————————————————————
//**
//  * @brief 帧间隔发送函数
//  * @description 发送长低电平，让灯珠知道一帧数据发送完毕，可以显示了（使用HAL_Delay）
//  */
//void RGB_RST(void)
//{
//    uint8_t dat[100] = {0};
//    /* 判断上次DMA有没有传输完成 */
//    while(HAL_DMA_GetState(&hdma_spi2_tx) != HAL_DMA_STATE_READY);
//    /* RGB RESET */
//    HAL_SPI_Transmit_DMA(&hspi2,dat,100);
//    HAL_Delay(10);
//    //osDelay(10);
//}



//**
//  * @brief 帧间隔发送函数
//  * @description 发送长低电平，让灯珠知道一帧数据发送完毕，可以显示了（发送空数据）
//  */
//void RGB_Reset(void)
//{
//    uint8_t dat[525] = {0};
//    /* 判断上次DMA有没有传输完成 */
//    while(HAL_DMA_GetState(&hdma_spi2_tx) != HAL_DMA_STATE_READY);
//    /* 发送长低电平（152us) */
//    HAL_SPI_Transmit_DMA(&hspi2,dat,525);
//}
