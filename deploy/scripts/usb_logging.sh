#!/bin/bash

# Getting absolute path of config files
script_dir=$(dirname $(realpath $0))
config_dir=$script_dir/../configs

media_boot="\n/dev/sda1      /media/pi/LOGS       auto    rw,nosuid,nodev,noauto,nofail          0       0"
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
redirect_err_status='       error_page 404 /;'

if [ 0 -eq $( grep -c 'error_page 404 /;' $nginx_conf ) ]; then
    sudo sed -i "/access.log/i$redirect_err_status" $nginx_conf
fi

if [ 0 -eq $( grep -c '/media/pi/LOGS' $nginx_conf ) ]; then
    sudo sed -i 's/\/var\/log\/nginx\/access.log/\/media\/pi\/LOGS\/nginx_access.log/g' $nginx_conf
    sudo sed -i 's/\/var\/log\/nginx\/error.log/\/media\/pi\/LOGS\/nginx_error.log/g' $nginx_conf
fi