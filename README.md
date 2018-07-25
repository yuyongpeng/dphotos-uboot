#### 编译:
	工具链: https://github.com/T-Firefly/prebuilts.git

	make rk3288_defconfig
	./mkv7.sh

#### 升级
	sudo upgrade_tool ul rk3288_bootloader_v1.00.06.bin

#### 切换rootfs
	系统命令行输入:
	reboot bootos1/bootos2

#### kernel
	release_kernel_v2.0_180725.7z

#### uboot
	include/configs/rk_default_config.h
	默认启动分区 #define CONFIG_DEFAULT_ROOTFS  "/dev/mmcblk2p6"

	include/fastboot.h
	#define ENV_BOOTOS1 "/dev/mmcblk2p6"
	#define ENV_BOOTOS2 "/dev/mmcblk2p7"

	以上宏定义，确定有误需要修改

#### 蓝牙
	如需 关闭/打开 蓝牙状态
		hciconfig hci0 down/up
	如果蓝牙不可用 ，可执行: enable_bt   即可

#### 呼吸灯 第一种驱动
    节点：/sys/class/leds
	设置蓝灯呼吸, 默认最大亮度
	echo 1 > /sys/class/leds/blue/blink
	设置呼吸亮度 (0 -- 255)
	echo 10 > /sys/class/leds/blue/brightness
	echo 1 > /sys/class/leds/blue/blink
	关闭呼吸
	echo 0 > /sys/class/leds/blue/blink

	颜色组合，利用肉眼刷新频率低，同时间打开多个
	echo 1 > /sys/class/leds/blue/blink
	echo 1 > /sys/class/leds/green/blink


	设置呼吸时长
	echo "2 1 2 1" > /sys/class/leds/blue/led_time

	"2 1 2 1" --> 依次代表
	aw2013,rise-time-ms: the rise time when led in breathe mode
	aw2013,hold-time-ms: the hold time when led in breathe mode
	aw2013,fall-time-ms: the fall time when led in breathe mode
	aw2013,off-time-ms: the off time when led in breathe mode

#### 呼吸灯 第二种驱动
	节点: /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
	总体来说，都是 echo "寄存器地址 寄存器地址值" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
	参照文档中 Register Detail Description 一节来进行设置

```
		   _____          _____
		  /+   +\        /
		 / +   + \      /
		/  +   +  \    /
--------   +   +  +----+
+      +   +   +  +    +
+  T0  + T1+ T2+T3+ T4 +
+      +   +   +  +    +

```

T1 T3 T4
000: 0.13s
001: 0.26s
010: 0.52s
011: 1.04s
100: 2.08s
101: 4.16s
110: 8.32s
111: 16.64s

T2
000: 0.13s
001: 0.26s
010: 0.52s
011: 1.04s
100: 2.08s
101: 4.16s

	呼吸:                                                                    7bit   ---   1bit
		设置呼吸的效果，（raise，hold，fall，off）假设都设置为 4.16s 那么都是 0 101 0 101 转化为 16 进制就是 0x55
		依据文档

		设置 rasise hold
			蓝灯: echo "0x37 0x55" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
			绿灯: echo "0x3A 0x55" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
			红灯: echo "0x3D 0x55" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
		设置 fall off
			蓝灯: echo "0x38 0x55" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
			绿灯: echo "0x3B 0x55" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
			红灯: echo "0x3E 0x55" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013

		设置呼吸的次数，呼吸指定的次数就关闭, 假设指定次数为 15 次 (最大为 15 次，最小为 0 次也就是不停止，一直循环)
		那么就是 0000 1111 转化为 16 进制就是 0x0f
			蓝灯: echo "0x39 0x0f" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
			绿灯: echo "0x3C 0x0f" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
			红灯: echo "0x3F 0x0f" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013

		设置呼吸灯的最大亮度
		首先先设置灯的 mode 为 PWM mode
			蓝灯: echo "0x31 0x02" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
			绿灯: echo "0x32 0x02" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
			红灯: echo "0x33 0x02" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
		然后设置亮度 (0 ~ 255),最大亮度为 0xff
			蓝灯: echo "0x34 0xff" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
			绿灯: echo "0x35 0xff" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
			红灯: echo "0x36 0xff" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013

		设置呼吸的时长，到了时长就关闭led灯，如果不设置就一直循环。
			这个给的文档中没有相关寄存器的操作，不过可以通过组合以上接口，在应用层进行控制

		设置呼吸灯的颜色，通过RGB进行配比组合颜色
			这个一般是使用一个调色板，然后根据调色板的 rgb 值，对应相应 rbg 灯的亮度值进行微调,
			只是要注意设定的 rgb 值要转换成相应的 16 进制数来赋值给寄存器

		设置呼吸灯的平滑转换，比如设置好呼吸的起始颜色和结束颜色，呼吸效果就是从起始颜色平滑的转换到结束颜色（不知是否可以实现？）
		首先先设置灯为 fade in / fade out 模式
			蓝灯: echo "0x31 0x62" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
			绿灯: echo "0x32 0x62" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
			红灯: echo "0x33 0x62" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
		然后根据上面写的 rbg 值来进行起始颜色和结束颜色的设定
		(
			注意，这里的颜色，根据文档，要么颜色由浅到深，要么颜色由深到浅,
			也就是说，rgb 三色灯的起始亮度值三个要么分别比比原来的都低，要么比原来的都高,并且差值需要很明显

			比如 r 120  ===> r 254
				 g 55   ===> g 101
				 b 104  ===> b 233

			平滑转换的效果才会看得出来
		)

		按照给定的RGB配比数组，进行轮训的显示呼吸效果，比如配比出橙色、橘色、蓝色，然后轮循的显示这3个配比出的颜色
		上面已经说明，可以通过配比三个灯亮度值组合来配比出不同的颜色,关于轮循显示 3 个配比的颜色，这个在寄存器中
		没有找到相应的说明，只能通过应用层去控制

		配色 Demo
			橙色: #FF6100
				蓝灯: echo "0x34 0x00" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
				绿灯: echo "0x35 0x61" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
				红灯: echo "0x36 0xff" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
			橘色: #FF8000
				蓝灯: echo "0x34 0x00" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
				绿灯: echo "0x35 0x80" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
				红灯: echo "0x36 0xff" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
			蓝色: #0000FF
				蓝灯: echo "0x34 0xff" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
				绿灯: echo "0x35 0x00" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
				红灯: echo "0x36 0x00" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
			紫色: #FF00FF
				蓝灯: echo "0x34 0xff" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
				绿灯: echo "0x35 0x00" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
				红灯: echo "0x36 0xff" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013

	灯的效果:
		长亮led灯，可以输入一个时长，到了时间就关闭，如果不设置时长就一直常亮，直到明确给了关闭信号。
		上面已经说明

		设置灯的亮度
		上面已经说明

		设置灯的颜色（单独设置 RGB，也可以通过 RGB 配比显示别的颜色）
		上面已经说明

	控制:
		关闭呼吸灯(只关闭其中一盏，其他的亮)
			蓝灯: echo "0x30 0x06" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
			绿灯: echo "0x30 0x05" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
			红灯: echo "0x30 0x03" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
		打开呼吸灯(只打开其中一盏，其他的灭)
			蓝灯: echo "0x30 0x01" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
			绿灯: echo "0x30 0x02" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
			红灯: echo "0x30 0x04" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013

		全灭呼吸灯
			echo "0x30 0x00" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013
		全开呼吸灯
			echo "0x30 0x07" > /sys/devices/platform/ff170000.i2c/i2c-5/5-0045/AW2013

#### 马达控制好
    echo 237 > /sys/class/gpio/export
    echo out > /sys/class/gpio/gpio237/direction
    echo 1 > /sys/class/gpio/gpio237/value

#### G-sensor
    简单获取上报 x,y,z值  mc_ctl.c
	详细例子可参照修改    mma8452.7z
