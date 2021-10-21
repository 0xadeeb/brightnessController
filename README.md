# Device Driver Implementation

This is an implementation of a custom Linux device driver. It reads mouse scroll and changes the brightness of the screen.

The module has been tested on Ubuntu 20.04, kernel moudle 5.14 with a USB mouse.

## Working

Devices are generally represented by their respective files in the _/dev_ directory.

Device files are created using the `mknod` system call.

```
mknod path type major minor
```

- _Path_ - Path where the file is to be created.
- _Type_ - 'c' or 'b' to indicate whether the device is a character device or a block device.
- _Major, Minor_ - The major and minor number of the device. Major number identifies the device driver. Minor number is used to identify the specific instance of the device (if there is more than one). `ls – l` is used to find these numbers.

Device drivers can be built either as part of the kernel or separately as loadable modules. `lsmod` (or, alternatively, `cat /proc/modules`) prints the contents of the `/proc/modules` file, which shows the loadable kernel modules are currently loaded.

Modules can be loaded using the `insmod` command, by giving the name of the object file (_.ko_) to be loaded.

```
insmod module_name
```

Modules can be unloaded using the `rmmod` command.

```
rmmod module_name
```

## Procedure

Execute once:

```
su
mknod -m 666 /dev/mbdriver c 45 1
```

Repeat to test the driver: (`dmesg` is for debugging)

```
make
insmod dev_driver.ko
./tester    #Scroll up and down to change the brightness
rmmod dev_driver
dmesg       #To view the kernel messages
```

Finally:

```
make clean
rm /dev/mbdriver
exit
```
