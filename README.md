# Operating system project 2 
### This is the source code and usage of operating system project2.

- ./master_device : the device moudule for master server
- ./slave_device  : the device moudule for slave client
- ./ksocket: the device moudule including the funtions used for kernel socket
- ./user_program : the user program "master" and "slave"


## Before start
use sudo ./clean to clean all excutable files and .ko files
use sudo ./compile.sh to compile the files and install modules.

## run
(N is the number of files, M is {mmap, fcntl} which stand for the method it used.
sudo ./master N f1 ... fN M
sudo ./slave N f1 ... fN ip(socket used this ip to bind)


## experiments
link: https://drive.google.com/drive/mobile/folders/109YWFVgDi5yxDV1KhWHHza3edIu7mkfb?usp=sharing

## Bonus:
```
use *sudo ./compile ASYNC* or *sudo ./compile SYNC* to choose mode

```
