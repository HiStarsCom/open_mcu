# 单次触发ADC的同步采样功能,主动查询转换完成标志位读取结果
## 关键字: ADC，同步采样，查询转换完成

**【功能描述】**
+ 示例代码基于HAL接口完成时钟、中断和ADC控制器初始化和功能配置。ADC在同步采样模式下，使用软件触发ADC两条通路同步同时采样，触发采样后软件判断ADC转换完成后，如果转换完成则读取结果。

**【示例配置】**
+ ADC触发源：软件。使用接口”HAL_ADC_SoftTrigMultiSample()”完成软件同时触发。同步采样，要求两条采样通路触发源一致。

+ ADC采样源： 挂载到SOC0和SOC1的外部采样源。SOC的在文件“system_init.c”中配置，同步采样模式选择的SOC0和SOC1，它们的配置由"SystemInit()”接口中的"ADC_SYNCSAMPLE_GROUP_1"决定，ADC_SYNCSAMPLE_GROUP_1对应的两个SOC为SOC0和SOC1，可以配置为(ADC_SYNCSAMPLE_GROUP_1~8)。

+ ADC采样结果：自行判断转换完成后，再读取结果。使用接口“HAL_ADC_CheckSocFinish()”判断是否转换完成，返回值OK表示转换结束，再使用“HAL_ADC_GetConvResult()”接口，分别读取两路采样结果。

**【示例效果】**
+ 当用户烧录编译后的示例代码后，初始化和配置完成，软件触发后，ADC进行单次采样转换。等待5ms后检查ADC采样结是否转换完成，若转换完成串口依次打印出两条通路的采样结果，若采样未完成打印未完成提示字符串。

**【注意事项】**
+ 示例代码使用UART0进行结果打印输出，需要对UART0配置。
