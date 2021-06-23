#!/bin/bash

# Sets up user and network security
# Enables kernel drivers for MAX3231 RTC
# Configures system to use the RTC to track time

# Importing logging colours, absolute paths and exit function
script_dir=$(dirname $(realpath $0))
. $script_dir/helper.sh

echo -e "\n${SUCCESS}********** Setting up hardware RTC **********\n${NC}"

# Add configuration to enable kernel drivers
if [ 0 -eq $( grep -c "^dtoverlay=i2c-rtc,ds3231" /boot/config.txt ) ]
then
    echo -e "\ndtoverlay=i2c-rtc,ds3231" | sudo tee -a /boot/config.txt
    echo -n "Reboot required before further RTC setup. Do you want to reboot now? [y/n]: "
    read answer
    if echo "$answer" | grep -iq "^y"; then
        echo "After rebooting, you must run this script again to complete RTC setup."
        sudo reboot
    else
        echo "You will need to reboot and run this script again to complete RTC setup."
        exit 1
    fi
else
    echo "Kernel drivers already disabled, continuing setup"
fi

# Disable the fake hwclock
sudo apt remove fake-hwclock -y || exit_script "Could not remove packages as required"
sudo update-rc.d -f fake-hwclock remove
sudo systemctl disable fake-hwclock

# Modify the hwclock script
if [ 1 -eq $( ls $config_dir | grep -c "hwclock-set" ) ]
then
    sudo cp $config_dir/hwclock-set /lib/udev/
    sudo chmod 755 /lib/udev/hwclock-set
    sudo chown root:root /lib/udev/hwclock-set
else
    echo -e "${ERROR} The hwclock-set file doesn't exist$NC"
    exit 1
fi

# Set the RTC from system time
echo -n "System clock needs to be accurate to set the hardware RTC. Do you want to proceed? [y/n]: "
read answer
if echo "$answer" | grep -iq "^y"; then
    sudo hwclock -w
else
    echo "You will need to run this script again to complete RTC setup."
    exit 1
fi

echo -e "\n${SUCCESS}Hardware RTC setup complete\n${NC}"