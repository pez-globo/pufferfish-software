#!/bin/bash

# Getting absolute path of script
script_path=$(dirname $(realpath $0))

# Install required components
$script_path/install.sh "deploy"

# Setup Nginx web server for serving frontend
$script_path/scripts/nginx_setup.sh

# Setup OS Boot Screen
$script_path/scripts/boot_screen.sh

# Setup backend autostarting service on boot
$script_path/scripts/backend_service.sh

# Setup configuration to disable screen blanking and hiding cursor on idle
$script_path/scripts/configure_kiosk.sh

# Setup systemd service to start Chromium Browser in Kiosk Mode on boot
$script_path/scripts/kiosk.sh

# Setup Logging in USB
$script_path/scripts/usb_logging.sh

# Modify eeprom to boot from USB Mass Storage by default
$script_path/scripts/live_usb.sh

# Modify eeprom to setup watchdog to handle Raspberry Pi failures and crashes
$script_path/scripts/setup_watchdog.sh

# Enable Overlayfs and change /boot partition to read-only
$script_path/scripts/overlayfs.sh

# Disable unnecessary background services
$script_path/scripts/disable_services.sh

# Setup security protocols
$script_path/scripts/security.sh

echo -n "Reboot required for changes to take effect. Do you want to reboot now? [y/N]: "
read answer
if echo "$answer" | grep -iq "^y"
then
    sudo reboot
fi