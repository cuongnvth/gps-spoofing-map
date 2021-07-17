# gps-spoofing-map
Server:
https://drive.google.com/open?id=1iKJQ5kOzct6gQ3pVsIHPh4HjSVX0Rr99

# Cài đặt gps-spoofing-map trên Ubuntu 18.04
+ Cài đặt UHD, GNURadio, QT5
+ Biên dịch lại phần mềm 
+ Cài đăt NodeJs V10, gói PM2
+ Thiết lập USB 4G
+ Thiết lập khởi động 

# xử lý lỗi thường gặp
sudo apt-get install -y pkg-config



# Phương pháp cài driver cho USB 4G Huawei trên ubuntu 16.04

Method 1:

sudo gedit /lib/udev/rules.d/40-usb_modeswitch.rules

ACTION=="add", SUBSYSTEM=="usb", ATTRS{idVendor}=="12d1", ATTRS{idProduct}=="14fe", RUN+="/usr/sbin/usb_modeswitch -v 12d1 -p 14fe -M '55534243123456780000000000000011062000000100000000000000000000'"


Method 2:

+ 
sudo gedit /etc/usb_modeswitch.d/12d1:14fe



DefaultVendor=0x12d1

DefaultProduct=0x14fe

TargetVendor=0x12d1

TargetProduct=0x1506

MessageContent="55534243123456780000000000000011062000000100000000000000000000"



+
sudo gedit /lib/udev/rules.d/40-usb_modeswitch.rules


ATTRS{idVendor}=="12d1", ATTRS{idProduct}=="14fe", RUN+="/usr/sbin/usb_modeswitch -c /etc/usb_modeswitch.d/12d1:14fe"



+
gksudo gedit /etc/modules


usbserial vendor=0x12d1 product=0x14fe


# add modem manager 

sudo apt-get install modem-manager-gui

# sua loi khong nhan usb
check lsusb -t

sudo modprobe -r usbhid && sleep 5 && sudo modprobe usbhid

usbhid la driver reset



