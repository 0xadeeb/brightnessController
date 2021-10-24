# Device Driver Implementation

This is an implementation of a custom Linux device driver. It reads mouse scroll and changes the brightness of the screen.

The module has been tested on Ubuntu 20.04, kernel moudle 5.14 with a USB mouse.

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
dmesg
```

Finally:

```
make clean
rm /dev/mbdriver
exit
```
