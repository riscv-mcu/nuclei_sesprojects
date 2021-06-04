# Nuclei SES Projects 使用说明

## 下载SEGGER Embedded Studio for RISC-V

打开[SEGGER官网Embedded Studio下载页面](https://www.segger.com/downloads/embedded-studio/)，下载SEGGER Embedded Studio for RISC-V（以下简称SES）并安装到本地，要求版本不低于`5.50` 。



## 克隆仓库到本地

打开windows command控制台，输入`git clone git@repo\link` 下载仓库到本地。

**注意**：根据实际仓库SSH或HTTPS修改`git@repo\link`内容。



## 在SES中打开Nuclei SES Projects

下载后双击NucleiExamples.emProject文件即可启动SES并打开Nuclei SES Projects。



## 通过J-LINK连接开发板

目前本工程仅支持使用J-LINK下载调试，由于J-LINK使用20pin接口，需要准备杜邦线，以便跳线连接。

J-LINK与JTAG各接口对应关系如下图：

![image-20210604182718502](doc\pictures\image-20210604112326536.png)

其中`VTref`需要单独接到开发板上3.3V的接口。

`TDI` 接到JTAG的`TDI`接口。

`TMS`接到JTAG的`TMS`接口。

`TCK`接到JTAG的`TCK`接口。

`TDO`接到JTAG的`TDO`接口。

`GND`接到JTAG的`GND`接口。

参考连接实物图如下：

![image-20210604183439857](doc\pictures\image-20210604183439857.png)



## 下载运行调试

SES中打开Nuclei SES Projects如下：

![image-20210604183730907](doc\pictures\image-20210604183730907.png)

选择菜单栏`Build -> Build coremark`编译工程，连接好J-LINK，打开开发板电源，点击菜单栏`Debug -> Go`开始下载调试。调试界面如下：

![image-20210604184203682](doc\pictures\image-20210604184203682.png)

有关调试的详细功能介绍，请参考SES内菜单栏`Help -> Contents` 。



## 切换使用不同打印方式

在头文件`nuclei_demosoc.h`中可以通过修改`DEBUG_SEMIHOST`的值来选择使用哪种打印方式。如果使用Terminal I/O打印输出，此处应设置为1 。如果使用uart串口打印输出，需要设置为0，并且需要准备串口转usb工具连接uart输出到pc主机上。此外需右击当前工程，选择`Options`打开设置页面，左侧目录栏选择`Library`，右侧修改`Runtime Library`为`Embedded Studio`。参考设置如下图：

![image-20210604185142208](doc\pictures\image-20210604185142208.png)
