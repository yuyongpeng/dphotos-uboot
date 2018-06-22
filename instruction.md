#### lcd
	驰展、科万煜、卡莱德 屏幕已支持
#### touch
    触摸屏未到
#### wifi
	nl80211
#### 蓝牙 
	如需 关闭/打开 蓝牙状态
		hciconfig hci0 down/up
	如果蓝牙不可用 ，可执行: enable_bt   即可
	设备适配接口，可自行下载系统自身工具的源代码 查找
#### 按键
    节点： /dev/input/event2
    上报键值： 212
    参照： getkey.c
#### 呼吸灯
    节点：/sys/class/leds
    3个目录对应，rgb 控制
#### 马达控制好
    echo 237 > /sys/class/gpio/export
    echo out > /sys/class/gpio/gpio237/direction
    echo 1 > /sys/class/gpio/gpio237/value

#### G-sensor
    简单获取上报 x,y,z值  mc_ctl.c
	详细例子可参照修改    mma8452.7z
