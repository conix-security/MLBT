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
//		Writes "backup" 1st sector (MBR + part table) to disk.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD write_mbr_vmdk_file(char* backup, char* device_name)
{
	return write_mbr(backup, device_name, 0x510000);
}
DWORD write_from_saved_mbr_device(char* backup, char* device_name)
{
	return write_mbr(backup, device_name, 0);
}

DWORD write_mbr(char* backup, char* device, DWORD mbr_offset)
{
	HANDLE hPD;
	DWORD len;
	PBYTE originalMbr;

	printf("[-] Reading backup file\n");
	hPD = CreateFileA(backup,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
	if(hPD == NULL || hPD == INVALID_HANDLE_VALUE)
	{
		printf("[!] Can't open %s (read)\n",backup);
		return -1;
	}
	originalMbr = (PBYTE)malloc(bytes_per_sector);
	if(!ReadFile(hPD, originalMbr, bytes_per_sector, &len, NULL))
	{
		printf("[!] Can't read.\n");
		free(originalMbr);
		CloseHandle(hPD);
		return -1;
	}
	CloseHandle(hPD);

	hPD = CreateFileA(device,GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
	if(hPD == NULL || hPD == INVALID_HANDLE_VALUE)
	{
		printf("[!] Can't open %s (write)\n",device);
		CloseHandle(hPD);
		free(originalMbr);
		return -1;
	}
	if(mbr_offset != 0)
	{
		if(SetFilePointer(hPD,mbr_offset,NULL,FILE_BEGIN) != mbr_offset)
		{
			printf("[!] Can't go to offset 0x%x\n",mbr_offset);
			CloseHandle(hPD);
			free(originalMbr);
			return -1;
		}
	}

	printf("[-]Writing original MBR + partition table. Continue ?\n");

	system("pause");
	if(!WriteFile(hPD,originalMbr,bytes_per_sector,&len,NULL))
	{
		printf("[!] Can't write %s sector at offset 0x%x\n",device,mbr_offset);
		CloseHandle(hPD);
		free(originalMbr);
		return -1;
	}

	free(originalMbr);
	CloseHandle(hPD);
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Reads 1st sector (MBR + part table) and writes it into "backup" file.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD backup_mbr_device(char* backup, char* device_name)
{
	return backup_mbr(backup, device_name, 0);
}
DWORD backup_mbr_vmdk_file(char* backup, char* device_name)
{
	return backup_mbr(backup, device_name, 0x510000);
}
DWORD backup_mbr(char* backup, char* device, DWORD mbr_offset)
{
	HANDLE hPD;
	HANDLE hFile;
	DWORD lenRead;
	PBYTE buffer;

	hPD = CreateFileA(device,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
	if(hPD == INVALID_HANDLE_VALUE || hPD == NULL)
	{
		printf("[!] Can't open %s\n",device);
		return -1;
	}
	if(mbr_offset != 0)
	{
		if(SetFilePointer(hPD,mbr_offset,NULL,FILE_BEGIN) != mbr_offset)
		{
			printf("[!] Can't go to offset 0x%x\n",mbr_offset);
			CloseHandle(hPD);
			return -1;
		}
	}

	buffer = (PBYTE)malloc(bytes_per_sector);
	if(ReadFile(hPD, buffer, bytes_per_sector, &lenRead, NULL))
	{
		hFile = CreateFileA(backup, GENERIC_WRITE,0,NULL,OPEN_ALWAYS,0,NULL);
		if(hFile == INVALID_HANDLE_VALUE || hFile == NULL)
		{
			printf("[!] Can't open %s\n",backup);
			free(buffer);
			CloseHandle(hPD);
			return -1;
		}
		if(!WriteFile(hFile,buffer,bytes_per_sector,&lenRead,NULL))
		{
			printf("[!] Can't write %s\n",backup);
			free(buffer);
			CloseHandle(hPD);
			CloseHandle(hFile);
			return -1;
		}
		CloseHandle(hFile);
		printf("[-] MBR (+ partition table) written in %s.\n",backup);
	}
	else
		printf("[!] Can't read MBR.\n");

	free(buffer);
	CloseHandle(hPD);
	return 0;
}
