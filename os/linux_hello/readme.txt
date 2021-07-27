
make 
make app
sudo insmode hellomodule.ko
sudo mknod /dev/helldev -c 232 0
sudo ./app
