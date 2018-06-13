#### 编译:
	rk3288_defconfig
	./mkv7.sh

#### 升级
	sudo upgrade_tool ul rk3288_bootloader_v1.00.06.bin

#### 切换rootfs
	系统命令行输入:
	reboot bootos1/bootos2


#### 代码修改部分

#### uboot 验证
##### uboot trust 分区可去掉
	测试，我将不同的 rootfs 分别放在 第6，第7 分区

	root=/dev/mmcblk2p6    ----> p6指当次启动 rootfs 在第6个分区

##### uboot 确定 rootfs 是否相对应

	include/configs/rk_default_config.h
	默认启动分区 #define CONFIG_DEFAULT_ROOTFS  "/dev/mmcblk2p6"

	include/fastboot.h
	#define ENV_BOOTOS1 "/dev/mmcblk2p6"
	#define ENV_BOOTOS2 "/dev/mmcblk2p7"

	以上宏定义，确定有误需要修改

#### kernel 补丁
	打上补丁: 0001-multiboot-add-bootos-n-to-reboot-modes.patch

	drivers/power/reset/reboot-mode.c


