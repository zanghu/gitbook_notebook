# [使用HAXM为QEMU for Windows加速](https://blog.csdn.net/weixin_33851429/article/details/94030540)

QEMU是一款支持多种CPU的[虚拟化](https://so.csdn.net/so/search?q=虚拟化&spm=1001.2101.3001.7020)软件，本身基于软件虚拟化，效率不高，在linux下可以基于KVM加速，据说可以获得接近物理机的性能。网上有很多关于QEMU+KVM的使用说明，但是KVM仅用于linux系统，在windows无法使用。关于在windows下如何对QEMU加速的文章特别少，我将自己的探索结果记录下来，以供参考。

我本来一度以为在windows下无法使用QEMU加速，早期的kqemu已经不在支持，无意间看到windows下的android模拟器使用HAXM加速，于是搜索HAXM与QEMU，找到官方的一篇文章，说明QEMU支持HAXM加速。[Accelerating QEMU on Windows with HAXM](https://www.qemu.org/2017/11/22/haxm-usage-windows/)

HAXM是intel推出专用于android 模拟器的加速器，目前已经开源，不限于对android模拟器的加速，支持windows和mac系统，windows支持win7及以上版本，下载地址 [github haxm](https://github.com/intel/haxm)

安装之前确保禁用hyper-v，还要确保BIOS中加CPU虚拟化启用。

下载之后的zip文件加压，以管理员权限运行silent_install.bat -log install.log，完成安装。

在qemu中使用 -machine accel=hax 加载加速器，例如：

qemu-system-x86_64 -L . .\system.img -machine accel=hax

其他

在qemu中输入 ctrl+alt+del组合键，首先ctrl+alt+2，调到控制台窗口输入sendkey ctrl-alt-delete,回车，然后ctrl+alt+1返回虚拟机界面。

silent_install.bat -v  查看HAXM的版本

silent_install.bat -c  检查是否支持CPU虚拟化

转载于:https://www.cnblogs.com/bingzhu/p/10746102.html

相关资源：[*qemu*虚拟机*加速*器k*qemu*_*qemu**加速*,*qemu*虚拟机*加速*-*嵌入式*文档类...](https://download.csdn.net/download/wjwhust/197234?spm=1001.2101.3001.5697)



# [Accelerating QEMU on Windows with HAXM](https://www.qemu.org/2017/11/22/haxm-usage-windows/)

In this post, I’m going to introduce a useful technique to people who are using, or are interested in using, QEMU on Windows. Basically, you can make the most of your hardware to accelerate QEMU virtual machines on Windows: starting with its 2.9.0 release, QEMU is able to take advantage of [Intel HAXM](https://github.com/intel/haxm) to run x86 and x86_64 VMs with hardware acceleration.

If you have used QEMU on Linux, you have probably enjoyed the performance boost brought by [KVM](https://wiki.qemu.org/Features/KVM): the same VM runs a lot faster when you launch QEMU with the `-accel kvm` (or `-enable-kvm`) option, thanks to hardware-assisted virtualization. On Windows, you can achieve a similar speed-up with `-accel hax` (or `-enable-hax`), after completing a one-time setup process.

First, make sure your host system meets the requirements of HAXM:

1. An Intel CPU that supports

    

   Intel VT-x with Extended Page Tables (EPT)

   .

   - Intel CPUs that do not support the said feature are almost extinct now. If you have a Core i3/i5/i7, you should be good to go.

2. Windows 7 or later.

   - HAXM works on both 32-bit and 64-bit versions of Windows. For the rest of this tutorial, I’ll assume you are running 64-bit Windows, which is far more popular than 32-bit nowadays.

Next, check your BIOS (or UEFI boot firmware) settings, and make sure VT-x (or Virtualization Technology, depending on your BIOS) is enabled. If there is also a setting for Execute Disable Bit, make sure that one is enabled as well. In most cases, both settings are enabled by default.

- If your system is protected against changes to BIOS, e.g. you have enabled BitLocker Drive Encryption or any other tamper protection mechanism, you may need to take preventive measures to avoid being locked out after changing the said BIOS settings.

After that, if you are on Windows 8 or later, make sure Hyper-V is disabled. This is especially important for Windows 10, which enables Hyper-V by default. The reason is that Hyper-V makes exclusive use of VT-x, preventing HAXM and other third-party hypervisors (such as VMware and VirtualBox) from seeing that hardware feature. There are a number of ways to disable Hyper-V; one of them is to bring up the *Start* menu, type *Windows Features* and Enter, uncheck *Hyper-V* in the resulting dialog, and click on *OK* to confirm.

- Note that changing the Hyper-V setting could also trigger the alarm of the tamper protection mechanism (e.g. BitLocker) that may be enabled on your system. Again, make sure you won’t be locked out after the reboot.

![Disabling Hyper-V in Windows Features](https://www.qemu.org/screenshots/windows-features-hyperv.png)

Now you’re ready to install HAXM, which needs to run as a kernel-mode driver on Windows so as to execute the privileged VT-x instructions. Simply download the latest HAXM release for Windows [here](https://software.intel.com/en-us/articles/intel-hardware-accelerated-execution-manager-intel-haxm), unzip, and run `intelhaxm-android.exe` to launch the installation wizard. (Despite the file name, Android is not the only guest OS that can be accelerated by HAXM.)

![Installing HAXM on Windows](https://www.qemu.org/screenshots/haxm-installer-windows.png)

If you haven’t installed QEMU, now is the time to do it. I recommend getting the latest stable release from [here](https://qemu.weilnetz.de/w64/). At the time of this writing, the latest stable release is 2.10.1 (build 20171006), so I downloaded `qemu-w64-setup-20171006.exe`, which is an easy-to-use installer.

With all that, we’re ready to launch a real VM in QEMU. You can use your favorite QEMU disk image, provided that the guest OS installed there is compatible with the x86 (i386) or x86_64 (amd64) architecture. My choice for this tutorial is `debian_wheezy_amd64_standard.qcow2`, which contains a fresh installation of the standard Debian Wheezy system for x86_64, available [here](https://people.debian.org/~aurel32/qemu/amd64/). To boot it, open a new command prompt window, switch to your QEMU installation directory (e.g. `cd "C:\Program Files\qemu"`), and run:

```
qemu-system-x86_64.exe -hda X:\path\to\debian_wheezy_amd64_standard.qcow2 -accel hax
```

You don’t have to leave the screen as the VM boots up, because soon you’ll be able to see the Debian shell and log in.

![Debian Wheezy (Standard) booted up in QEMU+HAXM](https://www.qemu.org/screenshots/qemu-debian-wheezy-shell-with-haxm.png)

To feel the difference made by HAXM acceleration, shut down the VM, and relaunch it without `-accel hax`:

```
qemu-system-x86_64.exe -hda X:\path\to\debian_wheezy_amd64_standard.qcow2
```

If you’re still not impressed, try a more sophisticated VM image such as `debian_wheezy_amd64_desktop.qcow2`, which boots to a desktop environment. VMs like this are hardly usable without hardware acceleration.

That’s it! I hope HAXM gives you a more enjoyable QEMU experience on Windows. You may run into issues at some point, because there are, inevitably, bugs in HAXM (e.g. booting an ISO image from CD-ROM doesn’t work at the moment). But the good news is that HAXM is now open source on [GitHub](https://github.com/intel/haxm), so everyone can help improve it. Please create an issue on GitHub if you have a question, bug report or feature request.

# [Intel Hardware Accelerated Execution Manager (HAXM)](https://github.com/intel/haxm)

HAXM is a cross-platform hardware-assisted virtualization engine (hypervisor), widely used as an accelerator for [Android Emulator](https://developer.android.com/studio/index.html) and [QEMU](https://www.qemu.org/). It has always supported running on Windows and macOS, and has been ported to other host operating systems as well, such as Linux and NetBSD.

HAXM runs as a kernel-mode driver on the host operating system, and provides a KVM-like interface to user space, thereby enabling applications like QEMU to utilize the hardware virtualization capabilities built into modern Intel CPUs, namely [Intel Virtualization Technology](https://www.intel.com/content/www/us/en/virtualization/virtualization-technology/intel-virtualization-technology.html).

## Downloads

The latest HAXM release for Windows and macOS hosts are available [here](https://github.com/intel/haxm/releases).

## Contributing

Detailed instructions for building and testing HAXM can be found at:

- [Manual for Linux](https://github.com/intel/haxm/blob/master/docs/manual-linux.md)
- [Manual for macOS](https://github.com/intel/haxm/blob/master/docs/manual-macos.md)
- [Manual for Windows](https://github.com/intel/haxm/blob/master/docs/manual-windows.md)

If you would like to contribute a patch to the code base, please also read [these guidelines](https://github.com/intel/haxm/blob/master/CONTRIBUTING.md).

## Reporting an Issue

You are welcome to file a [GitHub issue](https://github.com/intel/haxm/issues) if you discover a general HAXM bug or have a feature request.

However, please do not use the GitHub issue tracker to report security vulnerabilities. If you have information about a security issue or vulnerability with HAXM, please send an email to [secure@intel.com](mailto:secure@intel.com), and use the PGP key located at https://www.intel.com/security to encrypt any sensitive information.

## Code of Conduct

This project has adopted the Contributor Covenant, in the hope of building a welcoming and inclusive community. All participants in the project should adhere to this [code of conduct](https://github.com/intel/haxm/blob/master/CODE_OF_CONDUCT.md).



# [在windows 10上使用qemu安装Windows 10虚拟机](https://blog.csdn.net/quentin_d/article/details/123167286)

纯记录安装步骤，供研究使用，日常工作建议使用VmWare VirtualBox HyperV等

qemu版本

λ qemu-system-x86_64 -version
QEMU emulator version 6.2.0 (v6.2.0-11889-g5b72bf03f5-dirty)
Copyright (c) 2003-2021 Fabrice Bellard and the QEMU Project developers

1 下载windows 10安装 ISO文件 Win10_1909_English_x64.iso

2 下载Qemu中设备的virtio驱动，可以用于加速设备的模拟，提升虚拟机运行速度
到下面的网址找一个最新版本下载，我用的是virtio-win-0.1.215.iso

https://fedorapeople.org/groups/virt/virtio-win/direct-downloads/archive-virtio/
https://fedorapeople.org/groups/virt/virtio-win/direct-downloads/archive-virtio/virtio-win-0.1.215-2/virtio-win-0.1.215.iso

https://fedorapeople.org/groups/virt/virtio-win/direct-downloads/archive-virtio/virtio-win-0.1.215-2/

3 创建磁盘

 qemu-img create -f qcow2 hdd.img 40G

4 启动虚拟机进行安装

这里使用了 whpx 加速，适合安装并启用了hyper-v的机器，也可以使用intel的haxm来加速，下载地址 https://github.com/intel/haxm，haxm与hyper-v不兼容，启用了hyper-v就不能使用haxm了，根据机器选一个

qemu-system-x86_64.exe -accel whpx -machine q35 -smp sockets=1,cores=1,threads=2 -m 2048 -usb -device usb-kbd -device usb-tablet -rtc base=localtime -net nic,model=virtio -drive file=1909.img,media=disk,if=virtio -drive file=Win10_1909_English_x64.iso,media=cdrom -drive file=virtio-win-0.1.215.iso,media=cdrom 

qemu中无法直接得知whpx是否正常启用，不过如果看到qemu输出包含这一行，一般whpx就正常启用了，还可以是应用process explorer等工具查看 WinHvPlatform.dll是否正常加载

Windows Hypervisor Platform accelerator is operational

当然，就算正常加载了，后面还可以报错，甚至影响功能，比如后面的savevm命令就不能使用
有时候可能会看到这一行（whpx貌似已经正常进行加速了，目前不知这个报错会影响哪些）

(qemu) whpx: injection failed, MSI (0, 0) delivery: 0, dest_mode: 0, trigger mode: 0, vector: 0, lost (c0350005)

这时候可以加一个参数kernel-irqchip=off来去除这个报错（当然也不能解决savevm无法使用的问题）

qemu-system-x86_64 -accel whpx,kernel-irqchip=off -machine ....

5 然后就是正常的windows安装步骤，值得注意的是下面一步，如果到了分区那一步找不到磁盘，需要点击下面的 Load Driver，找到下载的iso里的 viostor 目录里 w10\amd64 子目录里查找并安装驱动，然后就可以了正常分区

![在这里插入图片描述](https://img-blog.csdnimg.cn/6f98c9e1fc06497ba5c196d41d3022e9.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAcXVlbnRpbl9k,size_20,color_FFFFFF,t_70,g_se,x_16)

![在这里插入图片描述](https://img-blog.csdnimg.cn/ed5e5895348e493bb2d9a4974061d559.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAcXVlbnRpbl9k,size_20,color_FFFFFF,t_70,g_se,x_16)

![在这里插入图片描述](https://img-blog.csdnimg.cn/85cc1181da144e66a80cc8ea9e4c5401.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAcXVlbnRpbl9k,size_20,color_FFFFFF,t_70,g_se,x_16)

6 安装完windows之后，需要安装网卡驱动才能上网，打开设备管理器，点击update driver，选择驱动盘，会自动找到驱动并安装

![在这里插入图片描述](https://img-blog.csdnimg.cn/4f457f8b0ebb4125a152b69236c01655.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAcXVlbnRpbl9k,size_20,color_FFFFFF,t_70,g_se,x_16)

![在这里插入图片描述](https://img-blog.csdnimg.cn/c388e05ec7ab4fc5a8c89256629296dd.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAcXVlbnRpbl9k,size_20,color_FFFFFF,t_70,g_se,x_16)


7 继续安装驱动盘里的驱动，一个是 qemu-ga-x86_64.msi，一个是qemu guest agent，virtio-win-guest-tools.exe

8 安装spice webdavd，以便进行文件传输，暂时省略
可以参考
https://www.guyrutenberg.com/2018/10/25/sharing-a-folder-a-windows-guest-under-virt-manager/
https://askubuntu.com/questions/899916/how-to-share-folder-with-windows-10-guest-using-virt-manager-kvm

9 建立内存快照，以便将一些更改操作放在快照里，后面可以方便切换或者迅速恢复设置

9.1 关闭虚拟机
9.2 建立快照磁盘

$ qemu-img create -b hdd.img -F qcow2 -f qcow2 snapshot.img

9.3 启动虚拟机

qemu-system-x86_64 -accel whpx -machine q35 -smp sockets=1,cores=1,threads=2 -m 2048 -usb -device usb-kbd -device usb-tablet -rtc base=localtime -net nic,model=virtio -drive file=snapshot.img,media=disk,if=virtio -monitor stdio

9.4 进行一些设置或者操作
9.5 在monitor窗口或者stdio中保存快照 ，然后退出
注意/待解决：可能由于whpx的bug，导致无法保存快照，如果是kvm hax tcg可能不存在这个问题

(qemu)savevm windows
(qemu)quit

10 从快照中启动虚拟机

qemu-system-x86_64 -accel whpx -machine q35 -smp sockets=1,cores=1,threads=2 -m 2048 -usb -device usb-kbd -device usb-tablet -rtc base=localtime -net nic,model=virtio  file=snapshot.img,media=disk,if=virtio -loadvm windows

11 可以将上面的操作结果放到docker里

$ mv hdd.img snapshot.img image
$ cd image
$ docker build -t windows/test . 

refer
1 https://pve.proxmox.com/wiki/Windows_VirtIO_Drivers
2 https://github.com/aerokube/windows-images
3 https://www.qemu.org/docs/master/system/invocation.html?highlight=kernel-irqchip
4 https://www.digitalfoundations.com.au/blog/qemu-5-on-windows-host-with-whpx/
5 https://qemu-project.gitlab.io/qemu/system/monitor.html
6 https://github.com/qemu/qemu/blob/master/docs/system/images.rst

福利
xemu是一款基于qemu的xbox模拟器，可以玩xbox优秀，https://xemu.app

https://openwrt.org/docs/guide-user/virtualization/qemu