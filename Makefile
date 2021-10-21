obj-m = dev_driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc tester.c -o tester -lSDL2main -lSDL2

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm tester
