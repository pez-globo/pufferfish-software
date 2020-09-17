# Pufferfish-Vent-Software Deployment

### Table of Contents

1. [Installation (Development Environment)](#installation-(development-environment))
2. [Deployment (Production Environment)](#deployment-(production-environment)) 
    1. [Security Setup](#security-setup)
    2. [Backend Service Autostart on Boot](#backend-service-autostart-on-boot)
    3. [Frontend Browser Autostart on Boot](#frontend-browser-autostart-on-boot)
    4. [Read-Only OS](#read-only-os)
3. [Cloning Raspberry Pi SD Card](#cloning-raspberry-pi-sd-card)
    1. [Cloning](#cloning)
    2. [Pishrink](#pishrink)
    3. [Writing img to SD Card](#writing-img-to-sd-card)
4. [References](#references)

### Installation (Development Environment)
All the software components can be installed by running `setup.sh` on a terminal.
```sh
$ ./install.sh
```

This will install the required components for pufferfish-vent-software in the given order.  
- pyenv
- poetry
- node
- yarn
---
### Deployment (Production Environment)
These instructions are to setup Raspberry Pi with Pufferfish-Vent-Software in a production environment.

#### Security Setup

```sh
$ ./scripts/setup_security.sh
```

#### Backend Service Autostart on Boot

```sh
$ ./scripts/backend_service.sh
```

#### Frontend Browser Autostart on Boot

```sh
$ ./scripts/configure_kiosk.sh
```

#### Read-Only OS
The filesystem needs to be read-only to avoid writing to SD Card in order to increase the lifetime of a SD Card

```sh
$ ./scripts/read_only_os.sh
```

---
### Cloning Raspberry Pi SD Card

#### Cloning

Mount the SD Card using a card reader  
Execute the respective command to find the mount point of your SD Card  
<br/>
**Linux**
```sh
$ sudo fdisk -l
```
**MacOS**
```sh
$ diskutil list
```

Use `dd` command to clone the SD Card  
Assuming `/dev/sdb` as the mount point for your SD Card. Replace it with the mounting point on your system.  

```sh
$ sudo dd if=/dev/sdb of=/path/to/save/rpi.img
```
This will create an `img` file which can be used to flash to a brand new SD Card  

#### Pishrink

If you are a linux user you can shrink this cloned img file using  
```sh
$ sudo ./scripts/pishrink.sh -vd /path/to/saved/rpi.img /path/to/save/pishrink_rpi.img
```

#### Writing img to SD Card

Mount the SD Card using a card reader  
Execute the respective command to find the mount point of your SD Card  
<br/>
**Linux**
```sh
$ sudo fdisk -l
```
**MacOS**
```sh
$ diskutil list
```

If your SD Card already has an OS installed you might have more than one partition for your SD Card such as `/dev/sdb1` and `/dev/sdb2`  
In that case you'll have to unmount the partitions before flashing a new OS.  
Unmount the partitions using command below with respective mount points  
```sh
$ sudo umount /dev/sdb1
```

Now you can flash the SD Card with your cloned OS using `dd`  
```sh
$ sudo dd bs=4M if=/path/to/saved/rpi.img of=/dev/sdb
```
---
### References
Pishrink - https://github.com/Drewsif/PiShrink