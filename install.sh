#!/bin/bash

a=1
#사용자가 입력한 ID, PW, MEM, HDD, VCPU 갯수를 읽어온다
while read line
do 
    
    if [ $a -eq 1 ]; then
        ID=$line
    elif [ $a -eq 2 ]; then
        PW=$line
    elif [ $a -eq 3 ]; then
        RAM=$line
    elif [ $a -eq 4 ]; then
        HDD=$line
    elif [ $a -eq 5 ]; then
        VCPU=$line
    fi
    a=$(($a+1))
done < user.info

sudo apt-get update
sudo apt-get install rinetd
sudo apt-get install whois
sudo cp ./rinetd.conf /etc/rinetd.conf 
sudo /etc/init.d/rinetd restart

#virtual server install
sudo apt-get install qemu-kvm qemu-system libvirt-bin ubuntu-vm-builder -y
sudo apt-get install acpid -y
sudo apt-get install virtinst virt-viewer -y

gcc -o config config.c
./config

#가상서버 설치
sudo virt-install \
--connect qemu:///system \
--name $ID \
--ram $RAM \
--disk pool=default,size=$HDD,bus=virtio,sparse=false \
--vcpu $VCPU \
--network network=default,model=virtio \
--location 'http://hu.archive.ubuntu.com/dists/precise-updates/main/installer-amd64' \
--initrd-inject='./preseed.cfg' \
--extra-args="locale=en_GB.UTF-8 console-setup/ask_detect=false keyboard-configuration/layoutcode=us file=file:/preseed.cfg vga=788 quiet" \
--os-type=linux \
--os-variant=ubuntuprecise \
--virt-type kvm \
--video=vga \
--noreboot

virsh start $ID
virt-viewer -c qemu:///system $ID 
