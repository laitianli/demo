1.加载sctp内核模块
modinfo sctp
modprobe sctp 

2.安装sctp包
yum install lksctp* -y


3.编译时添加-lsctp库


