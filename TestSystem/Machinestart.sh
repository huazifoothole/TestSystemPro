#/bin/bash
cd /root/TestSystem/
ko_path=/lib/modules/$(uname -r)/kernel/drivers/hwmon
if  !  lsmod  |  fgrep w83627   -q ;   then
	modprobe w83627ehf 2>/dev/null
fi
if  !  lsmod  |  fgrep coretemp   -q ;   then
	insmod $ko_path/coretemp.ko
fi
if  !  lsmod  |  fgrep usbreader_fc14  -q ;   then
	insmod ./usbreader_fc14.ko
fi
#./TestMachine-V2
#sensors | grep temp1 |awk '{print $1 $2}'|cut -c 1-13
