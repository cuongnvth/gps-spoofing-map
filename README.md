# gps-spoofing-map
Server:
https://drive.google.com/open?id=1o8yo6jae4qDdpNeVKtKQPmWQZRw5veMy

#Add modem Huawei E3276 

sudo gedit /etc/usb_modeswitch.d/12d1:14fe

# Huawei E3276 series
DefaultVendor=0x12d1
DefaultProduct=0x14fe
TargetVendor=0x12d1
TargetProduct=0x1506
MessageContent="55534243123456780000000000000011062000000100000000000000000000"

sudo gedit /lib/udev/rules.d/40-usb_modeswitch.rules

# Huawei E3276
ATTRS{idVendor}=="12d1", ATTRS{idProduct}=="14fe", RUN+="/usr/sbin/usb_modeswitch -c /etc/usb_modeswitch.d/12d1:14fe"

gksudo gedit /etc/modules

usbserial vendor=0x12d1 product=0x14fe
