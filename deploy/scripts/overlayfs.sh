#!/bin/bash

# Getting absolute path of config files
script_dir=$(dirname $(realpath $0))
config_dir=$script_dir/../configs

sudo apt update
sudo apt install initramfs-tools -y

if [ 0 -eq $( cat /boot/cmdline.txt | grep -c "console=tty3" ) ]
then
  sudo sed -i 's/console=tty1/console=tty3/g' /boot/cmdline.txt
fi

if ! grep $config_dir/overlay /etc/initramfs-tools/modules > /dev/null; then
  cat $config_dir/overlay | sudo tee -a /etc/initramfs-tools/modules
fi

sudo cp $config_dir/overlay /etc/initramfs-tools/scripts

if [ 1 -eq $( ls /boot/ | grep -c "initrd7.img" ) ]
then
  sudo update-initramfs -c -k $(uname -r)
  sudo mv /boot/initrd.img-$(uname -r) /boot/initrd7.img
else
  echo -e "Updated initramfs already exists"
fi

sudo sed -e "s/initramfs.*//" -i /boot/config.txt

if [ 0 -eq $( cat /boot/config.txt | grep -c "initramfs initrd7.img" ) ]
then
  echo initramfs initrd7.img | sudo tee -a /boot/config.txt
fi

if [ 0 -eq $( ls /boot/ | grep -c "cmdline.txt.overlay" ) ]
then
  sudo cp /boot/cmdline.txt /boot/cmdline.txt.orig
  sudo sed -e "s/\(.*\)/boot=overlay \1/" -i /boot/cmdline.txt
  sudo cp /boot/cmdline.txt /boot/cmdline.txt.overlay
fi

sudo cp $config_dir/overctl /usr/local/sbin

if [ 0 -eq $( cat /etc/fstab | grep -c "defaults,ro" ) ]
then
  sudo sed -e "s/\(.*\/boot.*\)defaults\(.*\)/\1defaults,ro\2/" -i /etc/fstab
fi