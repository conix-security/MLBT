MLBT
====

This tool is just a p0c I wrote while working on bootkits. It allows
many things such as dumping sectors on a specific device, placing
16 bit payload at startup (it writes a specific code which will load
and execute your payload, and relocates the original bootstrap code
which you can load and execute after processing), display ascii art
(or simple messages) before booting (a special payload is generated),
save/restore the actual bootstrap + partition table, etc.

Payloads will be loaded and executed at 0000:0800, and overwritten
code is at 0000:0600. To execute original bootstrap, just copy 0x200
bytes from 0000:0600 to 0000:7c00, or load the sector with the original
backuped bootstrap at 0000:7c000. To find it, parse the code at sector
1.

While writing anything on a device, a dump is performed, and I strongly
advise you to keep this dump on a separate device, just in case you
overwrite your partition table / bootloader :].

Examples:
=========

Dump 4 sectors : MLB.exe -p 1 -l 0 4
Display "pwn.txt" ascii art message at boot : MLB.exe -p 1 -aaW ".\pwn.txt"
Run "print_boot2" code at boot : MLB.exe -p 0 -irW "print_boot2"
Run "keypwn" code on PhysicalDrive1 : MLB.exe -p 1 -irW "keypwn"
Wipe 30 sectors (starting at sector #2) of PhysicalDrive1 : MLB.exe -p 1 -wipe 30
Backup MBR + part. table of PhysicalDrive0 into "backup.bin" : MLB.exe -p 0 -b1 backup.bin


The keypwn example:
===================

The keypwn.asm code purpose is to be written into a removable drive such
as a USB key. While booting it, it allows you to place an ascii art
message at the primary disk startup (works the same as the ascii art
feature of the regular tool works). It should not overwrite anything but
the MBR code (not the bootloader : it checks for null bytes code caves),
but the original 1st sector is backuped into the USB key (sector #0xF)
for rescue purpose. The second option allows you to restore the saved
data to the disk.
