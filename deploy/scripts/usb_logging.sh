#!/bin/bash

# Getting absolute path of config files
script_dir=$(dirname $(realpath $0))
config_dir=$script_dir/../configs

media_boot="\n/dev/disk/by-label/LOGS      /media/pi/LOGS       auto    rw,nosuid,nodev,x-systemd.device-timeout=5,noauto,nofail          0       0"
fstab_file="/etc/fstab"

# Adding LOGS mount point to /etc/fstab
if [ 0 -eq $( grep -c '/media/pi/LOGS' $fstab_file ) ]; then
    echo -e $media_boot | sudo tee -a $fstab_file
fi

# Create mount file
if [ 1 -eq $( ls $config_dir | grep -c "media-pi-LOGS.mount" ) ]
then
    sudo cp $config_dir/media-pi-LOGS.mount /etc/systemd/system/
    sudo chmod 644 /etc/systemd/system/media-pi-LOGS.mount
else
    echo "The media-pi-LOGS.mount file doesn't exist"
    exit 1
fi

# Enabling mount point service
sudo systemctl daemon-reload
sudo systemctl enable media-pi-LOGS.mount

# Changing nginx log location to USB
nginx_conf='/etc/nginx/nginx.conf'

if [ 0 -eq $( grep -c '/media/pi/LOGS' $nginx_conf ) ]; then
    sudo sed -i 's|/var/log/nginx/access.log|/media/pi/LOGS/nginx_access.log|g' $nginx_conf
    sudo sed -i 's|/var/log/nginx/error.log|/media/pi/LOGS/nginx_error.log|g' $nginx_conf
fi

# Adding rsyslog configuration
if [ 0 -eq $( grep -c 'pufferfish_backend' /etc/rsyslog.conf ) ]; then
    if [ 1 -eq $( ls $config_dir | grep -c "rsyslog.conf" ) ]
    then
        cat $config_dir/rsyslog.conf | sudo tee -a /etc/rsyslog.conf
    else
        echo "The rsyslog.conf file doesn't exist"
        exit 1
    fi
fi

if [ 0 -eq $( ls /etc/ | grep -c "bash.bash_logout" ) ]
then
    sudo touch /etc/bash.bash_logout
fi

# Unmount USB on logout
echo -e "\nsudo umount /dev/sda1" | sudo tee -a /etc/bash.bash_logout

# Run Log rotate every hour
if [ 0 -eq $( ls /etc/cron.hourly/ | grep -c "logrotate" ) ]
then
    sudo mv /etc/cron.daily/logrotate /etc/cron.hourly/
fi

# Add logrotate config
if [ 1 -eq $( ls $config_dir | grep -c "pufferfish_logger" ) ]
then
    sudo cp $config_dir/pufferfish_logger /etc/logrotate.d/
    sudo chmod 644 /etc/logrotate.d/pufferfish_logger
    sudo chown root:root /etc/logrotate.d/pufferfish_logger
else
    echo "The pufferfish_logger file doesn't exist"
    exit 1
fi

sudo systemctl daemon-reload