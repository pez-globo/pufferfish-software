#!/bin/bash

# Setting up overlayfs
# Booting /boot partition in read-only mode
# Creating a overlayfs layer on top of / partition

# Message colours
ERROR='\033[1;31mERROR:'
SUCCESS='\033[1;32m'
WARNING='\033[1;33mWARNING:'
NC='\033[0m'

echo -e "\n${SUCCESS}********** Setting up overlayfs **********\n${NC}"

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
else
    echo -e "${WARNING} Boot screen logging is already disabled${NC}"
fi

# Adding overlay module to initramfs modules
if ! grep overlay /etc/initramfs-tools/modules > /dev/null; then
    echo overlay | sudo tee -a /etc/initramfs-tools/modules
else
    echo -e "${WARNING} Overlay already exists in initramfs modules${NC}"
fi

# Copying the overlay mount script to initramfs scripts
if [ 1 -eq $( ls $config_dir | grep -c "overlay" ) ]
then
    sudo cp $config_dir/overlay /etc/initramfs-tools/scripts
else
    echo -e "${ERROR} The overlay file doesn't exist${NC}"
    exit 1
fi

# Creating a new initramfs img
if [ 0 -eq $( ls /boot/ | grep -c "initrd7.img" ) ]
then
    sudo update-initramfs -c -k $(uname -r)
    sudo mv /boot/initrd.img-$(uname -r) /boot/initrd7.img
else
    echo -e "${WARNING} Updated initramfs already exists${NC}"
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
    echo -e "${WARNING} Overlay boot flag already exists in cmdline.txt${NC}"
fi

# Copying overctl script for read-only to read-write switch
if [ 1 -eq $( ls $config_dir | grep -c "overctl" ) ]
then
    sudo cp $config_dir/overctl /usr/local/sbin
else
    echo -e "${ERROR} The overctl file doesn't exist${NC}"
    exit 1
fi

# Changing /boot entry to read-only in fstab
if [ 0 -eq $( cat /etc/fstab | grep -c "defaults,ro" ) ]
then
    sudo sed -e "s/\(.*\/boot.*\)defaults\(.*\)/\1defaults,ro\2/" -i /etc/fstab
fi