#!/bin/bash

# Getting absolute path of script
script_dir=$(dirname $(realpath $0))

# Install required components
./$script_dir/install.sh "deploy"

# Setup Nginx web server for serving frontend
./$script_dir/scripts/nginx_setup.sh

# Setup OS Boot Screen
./$script_dir/scripts/boot_screen.sh

# Setup backend autostarting service on boot
./$script_dir/scripts/backend_service.sh

# Setup configuration to disable screen blanking and hiding cursor on idle
./$script_dir/scripts/configure_kiosk.sh

# Setup systemd service to start Chromium Browser in Kiosk Mode on boot
./$script_dir/scripts/kiosk.sh

# Setup Logging in USB
./$script_dir/scripts/usb_logging.sh

# Modify eeprom to boot from USB Mass Storage by default
./$script_dir/scripts/live_usb.sh

# Modify eeprom to setup watchdog to handle Raspberry Pi failures and crashes
./$script_dir/scripts/setup_watchdog.sh

# Enable Overlayfs and change /boot partition to read-only
./$script_dir/scripts/overlayfs.sh

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