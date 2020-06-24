cd ./ksocket
make CFLAGS=-D$1
insmod ksocket.ko
cd ../master_device
make
insmod master_device.ko
cd ../slave_device
make
insmod slave_device.ko
cd ../user_program
make

