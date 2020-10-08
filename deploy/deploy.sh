#!/bin/bash

# Getting absolute path of script
script_dir=$(dirname $(realpath $0))

# Install required components
./$script_dir/install.sh "deploy"

# Setup Nginx web server for serving frontend
./$script_dir/scripts/nginx_setup.sh

# Setup OS Boot Screen
./$script_dir/scripts/boot_screen.sh

# Setup browser autostart with kiosk mode on boot
./$script_dir/scripts/configure_kiosk.sh

# Setup backend autostarting service on boot
./$script_dir/scripts/backend_service.sh

# Disable unnecessary background services
./$script_dir/scripts/disable_services.sh

# Setup security protocols
./$script_dir/scripts/security.sh

echo -n "Reboot required for changes to take effect. Do you want to reboot now? [y/N]: "
read answer
if echo "$answer" | grep -iq "^y"
then
    sudo reboot
fi