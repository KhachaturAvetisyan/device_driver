sudo insmod fake.ko
dmesg
sudo mknod /dev/scull c 243 0
sudo chmod 777 /dev/scull
