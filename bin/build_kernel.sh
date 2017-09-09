#!/bin/bash

echo "Enter the kernel version to build from www.kernel.org:"
read LINUX

sudo apt-get update
sudo apt-get install git fakeroot build-essential ncurses-dev xz-utils libssl-dev bc
wget https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-$LINUX.tar.xz -P ~/
tar -xvf ~/linux-$LINUX.tar.xz
sudo cp /boot/config-$(uname -r) .config
cd ~/linux-$LINUX
sudo make defconfig
sudo make -j2 && sudo make modules_install -j2 && sudo make install -j2
sudo update-initramfs -c -k $LINUX
sudo update-grub
reboot                        
