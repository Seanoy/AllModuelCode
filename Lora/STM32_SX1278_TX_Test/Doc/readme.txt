SX1278端口配置：

SPI 
以及相应管教配置在sx1276-hal.c文件里面配置
射频开关芯片收发切换：       CTRL  /CTRL
CTRL:  PA2                   1     0      RF_RX
/CTRL: PA3                   0     1      RF_TX

DIO0: PA0    可做收发中断用
DIO1: PB1
DIO2: PB2
DIO3: PA8
DIO4: PA11
DIO5: PA12

LED指示灯： PB0