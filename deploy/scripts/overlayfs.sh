#!/bin/bash

# Getting absolute path of config files
script_dir=$(dirname $(realpath $0))
config_dir=$script_dir/../configs

sudo apt update
sudo apt install initramfs-tools -y

sudo sed -i 's/console=tty1/console=tty3/g' /boot/cmdline.txt

if ! grep $config_dir/overlay /etc/initramfs-tools/modules > /dev/null; then
  echo $config_dir/overlay | sudo tee -a /etc/initramfs-tools/modules
fi

sudo cp $config_dir/overlay /etc/initramfs-tools/scripts

sudo update-initramfs -c -k $(uname -r)
sudo mv /boot/initrd.img-$(uname -r) /boot/initrd7.img

sudo sed -e "s/initramfs.*//" -i /boot/config.txt
echo initramfs initrd7.img | sudo tee -a /boot/config.txt

sudo cp /boot/cmdline.txt /boot/cmdline.txt.orig
sudo sed -e "s/\(.*\)/boot=overlay \1/" -i /boot/cmdline.txt
sudo cp /boot/cmdline.txt /boot/cmdline.txt.overlay

sudo cp overctl /usr/local/sbin

sudo sed -e "s/\(.*\/boot.*\)defaults\(.*\)/\1defaults,ro\2/" -i /etc/fstab