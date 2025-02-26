# 配置GPIO管脚的中断功能，实现GPIO对按键检测
## 关键字: GPIO, 中断，按键检测

**【功能描述】**
+ 示例代码基于HAL接口完成时钟、GPIO控制器初始化和功能配置。在示例代码中通过中断的方式实现对按键的检测。

**【示例配置】**
+ GPIO管脚选择：示例代码中选择GPIO管脚用于通过中断方式进行按键检测。也可以选择其他GPIO管脚用于按键检测功能测试，在"GPIO_Init()"接口中的"g_gpiox.baseAddress"可以配置其它GPIOX，g_gpiox.pins可以配置“GPIO_PIN_0-GPIO_PIN_7”中的任意一个。
  
+ GPIO管脚初始化：调用接口"HAL_GPIO_Init()”完成对示例代码中GPIO管脚的方向、电平、中断模式配置。

**【示例效果】**
+ 当用户烧录编译后的示例代码后，初始化和配置完成后，示例代码中在按键没有按下时Debug串口会一直打印等待按键的log信息，当连接的按键按下时功能正常时Debug串口打印成功log信息；功能异常时按下按键时Debug串口还是会一直打印等待按键的log信息。

**【注意事项】**
+ 示例代码使用UART0进行结果打印输出，需要对UART0配置。
+ 示例代码中使用的GPIO管脚需和按键连接在一起。