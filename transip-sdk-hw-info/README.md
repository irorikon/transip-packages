### 使用说明

该驱动用于 TransIP 路由器的硬件属性描述

### dts写法

1. 识别驱动的 compatible 属性为 transip-hw-info
2. 支持任意形式，任意数量的字符串属性，可根据实际情况直接添加
3. 支持任意形式，任意数量的布尔值属性，可根据实际情况直接添加
4. 支持固定形式的 factory_data 属性，factory_data 属性已 cells 的格式描述，目前支持 device_mac, device_sn, country_code 等

   **例：**

   ```xml-dtd
           gl_hw {
                   compatible = "gl-hw-info";

                   #address-cells = <1>;
                   #size-cells = <1>;
                   wan = "eth0";
                   lan = "eth1.1";
                   build-in-modem = "1-1.2";
                   usb;
   				function-switch;
                   factory_data {
                           device_mac = <&art 0>;
                           device_sn = <&art 0x30>;
                           country_code = <&art 0x88>;
                   };
           };
   ```

### proc节点说明

1. 系统启动后，会在proc目录下生成 transip-hw-info 文件节点
2. transip-hw-info 会依据设备树将对应的属性以文件形式展开
3. 对应的属性文件支持 read 操作，可以直接使用 cat 命令查看内容

   **例：**

```shell
   root@OpenWrt:/# ll /proc/gl-hw-info/
   dr-xr-xr-x    2 root     root             0 Feb 16 06:39 ./
   dr-xr-xr-x   47 root     root             0 Jan  1  1970 ../
   -r--r--r--    1 root     root             0 Feb 16 06:39 country_code
   -r--r--r--    1 root     root             0 Feb 16 06:39 device_ddns
   -r--r--r--    1 root     root             0 Feb 16 06:39 device_mac
   -r--r--r--    1 root     root             0 Feb 16 06:39 device_sn
   -r--r--r--    1 root     root             0 Feb 16 06:39 device_sn_bak
   -r--r--r--    1 root     root             0 Feb 16 06:39 lan
   -r--r--r--    1 root     root             0 Feb 16 06:39 usb
   -r--r--r--    1 root     root             0 Feb 16 06:39 wan
   root@OpenWrt:/# cat /proc/gl-hw-info/device_mac
   94:83:c4:06:00:c2

```
