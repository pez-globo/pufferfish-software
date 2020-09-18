#!/bin/bash

# Setup Nginx web server for serving frontend
./scripts/nginx_setup.sh

# Setup OS Boot Screen
./scripts/boot_screen.sh

# Setup browser autostart with kiosk mode on boot
./scripts/configure_kiosk.sh

# Setup backend autostarting service on boot
./scripts/backend_service.sh

# Disable unnecessary background services
./scripts/disable_services.sh

# Setup security protocols
./scripts/setup_security.sh

# Convert filesystem into read-only mode
./scripts/read_only_os.sh