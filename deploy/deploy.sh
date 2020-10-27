#!/bin/bash

# Getting absolute path of script
script_path=$(dirname $(realpath $0))
. $script_path/scripts/helper.sh

# Install required components
$script_path/install.sh "deploy" || exit_script "Development components installation failed"

# Setup Nginx web server for serving frontend
$script_path/scripts/nginx_setup.sh || exit_script "Nginx webserver setup failed"

# Setup OS Boot Screen
$script_path/scripts/boot_screen.sh || exit_script "Boot Screen setup failed"

# Setup backend autostarting service on boot
$script_path/scripts/backend_service.sh || exit_script "Backend service setup failed"

# Setup configuration to disable screen blanking and hiding cursor on idle
$script_path/scripts/configure_kiosk.sh || exit_script "UI configuration setup failed"

# Setup systemd service to start Chromium Browser in Kiosk Mode on boot
$script_path/scripts/kiosk.sh || exit_script "Kiosk service setup failed"

# Setup Logging in USB
$script_path/scripts/usb_logging.sh || exit_script "USB Logging setup failed"

# Modify eeprom to boot from USB Mass Storage by default
$script_path/scripts/live_usb.sh || exit_script "USB Mass Storage setup failed"

# Modify eeprom to setup watchdog to handle Raspberry Pi failures and crashes
$script_path/scripts/setup_watchdog.sh || exit_script "Watchdog setup failed"

# Enable Overlayfs and change /boot partition to read-only
$script_path/scripts/overlayfs.sh || exit_script "Read-only setup failed"

# Disable unnecessary background services
$script_path/scripts/disable_services.sh || exit_script "Disabling background services failed"

# Setup security protocols
$script_path/scripts/security.sh || exit_script "Security protocols setup failed"

echo -n "Reboot required for changes to take effect. Do you want to reboot now? [y/N]: "
read answer
if echo "$answer" | grep -iq "^y"
then
    sudo reboot
fi