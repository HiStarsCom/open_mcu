# UART-DMA模式接收数据
## 关键字: UART, DMA模式接收

**【功能描述】**
+ 在UART DMA模式下，通过DMA将接收的数据搬运到指定的内存空间。

**【示例配置】**
+ 预设置的缓存：需要使用者自行开辟内存空间，将内存首地址和需要接收的字符长度作为入参传入HAL_UART_ReadDMA()。

+ 串口0的接收模式配置为DMA模式。

+ 初始化DMA接收通道，需配置源地址和目标地址增长方式以及中断等。可通过DMA配置界面进行更改。

+ DMA接收完成中断：DMA接收中断开启，接收完成之后，会调用接收完成回调函数“DMA_Channel2CallBack”， 回调函数的名称可通过“HAL_UART_RegisterCallBack”进行更改。

+ 串口0会打印出接收到的数据和提示信息。

**【示例效果】**
+ 向串口0发送数据，串口0会将接收的数据打印。

**【注意事项】**
+ 串口通信的波特率，校验位等配置需保持一致，可以通过UART配置界面进行更改。