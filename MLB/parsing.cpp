// Copyright 2013 Conix Security, Adrien Chevalier
// adrien.chevalier@conix.fr
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "bootkit_tool.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Parses partition table to find code cave.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD get_first_sector_offset_from_first_sector(PBYTE first_sector)
{
	PBYTE partition_table_ptr = first_sector+0x1be;
	PDWORD current_sector_offset;
	DWORD min_sector_offset=0xFFFFFFFF;
	DWORD i;
	
	for(i=0; i<4; i++)
	{
		current_sector_offset = (PDWORD)(partition_table_ptr+8);
		if(*current_sector_offset != 0x0 && *current_sector_offset < min_sector_offset)
			min_sector_offset = *current_sector_offset;

		partition_table_ptr = partition_table_ptr + 0x10;
	}

	return min_sector_offset;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Reads device info (bytes per sector, etc.).
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD read_device_information(char* device)
{
	HANDLE hPD;
	DWORD len;
	DISK_GEOMETRY diskInformation;

	printf("[-] Reading drive information\n");
	hPD = CreateFileA(device,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
	if(hPD == NULL || hPD == INVALID_HANDLE_VALUE)
	{
		printf("[!] Can't open %s (read)\n",device);
		return -1;
	}
	if(!DeviceIoControl(hPD,IOCTL_DISK_GET_DRIVE_GEOMETRY,NULL,0,(LPVOID)&diskInformation,sizeof(diskInformation),&len,NULL))
	{
		printf("[!] Can't get %s information.\n",device);
		CloseHandle(hPD);
		return -1;
	}

	bytes_per_sector = diskInformation.BytesPerSector;
	sectors_per_track = diskInformation.SectorsPerTrack;
	tracks_per_cylinder = diskInformation.TracksPerCylinder;

	if(bytes_per_sector != 0x200)
	{
		printf("[!] Only 512 bytes/sector drives are handled (for now).\n");
		CloseHandle(hPD);
		return -1;
	}

	CloseHandle(hPD);
	return 0;
}
