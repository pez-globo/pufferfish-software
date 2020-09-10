### Cloning Raspberry Pi SD Card

#### Linux and MacOS

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
<br/>
<br/>
If you are a linux user you can shrink this cloned img file using  
```sh
$ cd scripts/
$ sudo ./pishrink.sh -vd /path/to/save/rpi.img /path/to/save/pishrink_rpi.img
```

#### References
Pishrink - https://github.com/Drewsif/PiShrink