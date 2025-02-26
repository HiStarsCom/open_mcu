# 配置WDG参数，验证看门狗复位功能，中断回调中进行喂狗操作
## 关键字: WDG，中断， 喂狗

**【功能描述】**
+ 示例代码基于HAL接口完成时钟、中断和WDG初始化和功能配置。设置喂狗时间，计时值递减到0后触发中断，中断中喂狗则不复位，否则第二次计数到0没喂狗则产生复位操作。

**【示例配置】**
+ 在"SystemInit()”接口中配置WDG的初始值、中断和复位使能等参数。

+ 调用"HAL_WDG_Start()"启动看门狗。

+ 设置中断回调函数"WDG_CallbackFunc()",在中断回调函数中调用"HAL_WDG_Refresh()"进行喂狗操作，如果调用喂狗操作则不复位，否则第二次计数为0则系统复位，程序重新执行。

**【示例效果】**
+ 当用户烧录编译后的示例代码后，初始化和配置完成后。串口打印testlog，中断触发后打印中断log，如果在中断中调用喂狗操作，则不产生复位。否则第二次触发中断则系统复位，程序重新运行，循环往复。

**【注意事项】**
+ 若想不复位，可在第一次计数清零触发中断时喂狗，为喂狗窗口，该版IP实际超时复位时间为两倍计数值，第二次计数清零才复位。用户可在回调函数中清中断，清中断系统自动喂狗，导致复位功能失效，可降规作为timer使用。