2018/8/2
1.移植修改
1）添加self_types.h
2)添加errno.h
3)__CC_ARM
#define _ARM_RTT
#define EXCLUDE_ETHERNET_WINDOWS
#ifdef _ARM_RTT
4)

2.仔细核查动态内存分配问题

2018/4/13
1.更新使用rt-thread 3.0.3

2018/4/12
1.重新命名，优化架构
2.更改HAL库为新版本，注意全面更新，不要遗漏
3.实现UDP通信
4.改SPI驱动为LL驱动，恢复正常

2018/4/11
1.形成本项目
