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
//		List devices.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD enumerate_drives()
{
	char deviceStr[0x20]={0x00};
	DWORD i=0;
	HANDLE hPD;
	DWORD len;
	DISK_GEOMETRY diskInformation;

	printf(
		"==============================\n"
		" DEVICE NAME\t\tTYPE\n"
		"------------------------------\n");

	for(i=0; i<0x40; i++)
	{
		sprintf_s(deviceStr,0x40,"\\\\.\\PhysicalDrive%d",i);

		hPD = CreateFileA(deviceStr,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
		if(hPD != NULL && hPD != INVALID_HANDLE_VALUE)
		{
			printf("%s",deviceStr);
			if(!DeviceIoControl(hPD,IOCTL_DISK_GET_DRIVE_GEOMETRY,NULL,0,(LPVOID)&diskInformation,sizeof(diskInformation),&len,NULL))
				printf("\n");
			else
			{
				if(diskInformation.MediaType==RemovableMedia)
					printf("\tRemovableMedia\n");
				else if(diskInformation.MediaType==FixedMedia)
					printf("\tfixed\n");
				else
					printf("\tfloppy\n");
			}
		}
		
		CloseHandle(hPD);
	}
	printf("------------------------------\n");
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Reads sectors.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD read_sectors_vmdk_file(int start, int nbSectors, char* vmdk_file)
{
	return read_sectors(start, nbSectors,vmdk_file,0x510000);
}
DWORD read_sectors_device(int start, int nbSectors, char* device)
{
	return read_sectors(start, nbSectors,device,0);
}
DWORD read_sectors(int start, int nbSectors, char* device, DWORD mbr_offset)
{
	HANDLE hPD;
	DWORD lenRead;
	PBYTE buffer;
	DWORD i;
	hPD = CreateFileA(device,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);

	if(hPD == INVALID_HANDLE_VALUE || hPD == NULL)
	{
		printf("[!] Can't open %s\n",device);
		return -1;
	}
	
	if(mbr_offset != 0 || start != 0)
	{
		if(SetFilePointer(hPD,mbr_offset+(start*bytes_per_sector),NULL,FILE_BEGIN) != mbr_offset+(start*bytes_per_sector))
		{
			printf("[!] Can't go to offset 0x%x\n",mbr_offset+(start*bytes_per_sector));
			CloseHandle(hPD);
			return -1;
		}
	}
	buffer = (PBYTE)malloc(nbSectors*bytes_per_sector);
	if(ReadFile(hPD, buffer, nbSectors*bytes_per_sector, &lenRead, NULL))
	{
		for(i=0; i<(int)lenRead; i++)
		{
			if(i % 0x10 == 0)
				printf("\n%.8x\t",i+(start*bytes_per_sector));
			
			printf("%.2x ",buffer[i]);
		}
	}
	else
		printf("[!] Can't read bytes\n");

	free(buffer);
	CloseHandle(hPD);
	

	return 0;
}
