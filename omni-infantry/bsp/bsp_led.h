//#ifndef BSP_LED_H
//#define BSP_LED_H
//#include "struct_typedef.h"
//
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
//extern void aRGB_led_show(uint32_t aRGB);
//extern void led_init();
//extern void led_flow();
//extern void led_off_flow();
//extern void led_task(void const *pvParameters);
//extern void led_dashboard();
//extern void led_light(uint8_t led_1,uint8_t led_2,uint8_t led_3,uint8_t led_4,uint8_t led_5,uint8_t led_6,uint8_t led_7);
//
//#define DELAY_TIME 100
//#define DELAY_LONG_TIME 200
//
//#define CHASSIS_TASK_INIT_TIME 357
////PWM端的 io口
//#define LED1_PORT GPIOE
//#define LED1_PIN GPIO_PIN_9
//#define LED2_PORT  GPIOE
//#define LED2_PIN GPIO_PIN_11
//#define LED3_PORT  GPIOE
//#define LED3_PIN  GPIO_PIN_13
//#define LED4_PORT GPIOC
//#define LED4_PIN GPIO_PIN_6
//#define LED5_PORT GPIOI
//#define LED5_PIN GPIO_PIN_6
//#define LED6_PORT GPIOI
//#define LED6_PIN GPIO_PIN_7
//
////用户接口端的
//#define LED7_PORT GPIOB
//#define LED7_PIN GPIO_PIN_14
//
//typedef enum{
//    ON_FLOW=0,
//    OFF_FLOW,
//    AUTO_AIM,
//    SHOOT,
//    CAP,
//    SPIN,
//
//    MODE_NUM
//}led_mode_e;
//
//typedef struct {
//    led_mode_e mode;
//
//
//}led_t;
//
//
//
//#endif
#ifndef BSP_LED_H
#define BSP_LED_H
#include "main.h"

typedef struct			//颜色结构体
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
}RGBColor_TypeDef;

typedef enum
{
    ON_FLOW=0,
    OFF_FLOW,
    AUTO_AIM,
    SHOOT,
    CAP,
    SPIN,

    MODE_NUM
}led_mode_e;

typedef struct
{
    led_mode_e mode;
}led_t;

static uint8_t RGB_Counter = 0;	// 在SPI2发送完成回调中自增的计数器
#define RGB_NUM 7	// 灯珠数量，虽然设成了宏，但是写的时候默认只用7，所以灯珠数量变了的话最好检查每一处

// 帧间隔发送函数
//void RGB_RST(void);
//void RGB_Reset(void);
// 颜色设置函数
void RGB_Set_Color(uint8_t LedId, RGBColor_TypeDef Color);
// RGB 刷新函数
void RGB_Refresh(uint8_t refresh_NUM);

// 点C板自带LED
extern void aRGB_led_show(uint32_t aRGB);
// ui灯板子任务
void led_dashboard();
// 炫酷灯效
void RGB_Single_Color(uint16_t RGB_LEN, uint8_t R, uint8_t G, uint8_t B);	// 单色恒定灯效
void RGB_Flow(uint16_t RGB_LEN, uint8_t R, uint8_t G, uint8_t B);	// 单灯珠流水灯
void RGB_Rainbow(uint8_t RGB_LEN, uint8_t MODE);    // 彩虹灯效

// 各种颜色测试
void RGB_RED(uint16_t RGB_LEN);		//红
void RGB_GREEN(uint16_t RGB_LEN);		//绿
void RGB_BLUE(uint16_t RGB_LEN);		//蓝
void RGB_YELLOW(uint16_t RGB_LEN);		//黄
void RGB_MAGENTA(uint16_t RGB_LEN);	//紫
void RGB_BLACK(uint16_t RGB_LEN);		//黑
void RGB_Test();      //
void RGB_Debug();
void RGB_WHITE(uint16_t RGB_LEN);		//白
void RGB_BRIGHT(uint16_t RGB_LEN);	//亮
#endif /* __WS2812_H */
