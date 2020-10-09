#!/bin/bash

# Getting absolute path of config files
script_dir=$(dirname $(realpath $0))
config_dir=$script_dir/../configs

# Installing initramfs
sudo apt update
sudo apt install initramfs-tools -y

# Disabling console logging during boot process
if [ 0 -eq $( cat /boot/cmdline.txt | grep -c "console=tty3" ) ]
then
  sudo sed -i 's/console=tty1/console=tty3/g' /boot/cmdline.txt
fi

# Adding overlay module to initramfs modules
if ! grep overlay /etc/initramfs-tools/modules > /dev/null; then
  echo overlay >> /etc/initramfs-tools/modules
fi

# Copying the overlay mount script to initramfs scripts
sudo cp $config_dir/overlay /etc/initramfs-tools/scripts

# Creating a new initramfs img
if [ 1 -eq $( ls /boot/ | grep -c "initrd7.img" ) ]
then
  sudo update-initramfs -c -k $(uname -r)
  sudo mv /boot/initrd.img-$(uname -r) /boot/initrd7.img
else
  echo -e "Updated initramfs already exists"
fi

# Removing initramfs config if any
sudo sed -e "s/initramfs.*//" -i /boot/config.txt

# Adding new initramfs config and img name
if [ 0 -eq $( cat /boot/config.txt | grep -c "initramfs initrd7.img" ) ]
then
  echo initramfs initrd7.img | sudo tee -a /boot/config.txt
fi

# Creating a backup of cmdline.txt files
if [ 0 -eq $( cat /boot/cmdline.txt | grep -c "boot=overlay" ) ]
then
  sudo cp /boot/cmdline.txt /boot/cmdline.txt.orig
  sudo sed -e "s/\(.*\)/boot=overlay \1/" -i /boot/cmdline.txt
  sudo cp /boot/cmdline.txt /boot/cmdline.txt.overlay
else
  echo -e "Overlay boot flag already exists in cmdline.txt"
fi

# Copying overctl script for read-only to read-write switch
sudo cp $config_dir/overctl /usr/local/sbin

# Changing /boot entry to read-only in fstab
if [ 0 -eq $( cat /etc/fstab | grep -c "defaults,ro" ) ]
then
  sudo sed -e "s/\(.*\/boot.*\)defaults\(.*\)/\1defaults,ro\2/" -i /etc/fstab
fi