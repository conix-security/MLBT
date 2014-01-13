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
//		Restores MBR from backup file (created while patching).
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD restore_from_saved_mbr_device(char* backup, char* device_name, BOOL write_partition_table, BOOL wipe_bootkit)
{
	return restore_from_saved_mbr(backup, device_name,0,write_partition_table,wipe_bootkit);
}
DWORD restore_from_saved_mbr_vmdk_file(char* backup, char* vmdk_path, BOOL write_partition_table, BOOL wipe_bootkit)
{
	return restore_from_saved_mbr(backup, vmdk_path, 0x510000,write_partition_table,wipe_bootkit);
}
DWORD restore_from_saved_mbr(char* backup, char* device, DWORD mbr_offset, BOOL write_partition_table, BOOL wipe_bootkit)
{
	HANDLE hPD;
	DWORD len;
	PBYTE originalMbr;
	DWORD beginWipe, wipeSize;

	printf("[-] Reading backup file\n");
	hPD = CreateFileA(backup,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
	if(hPD == NULL || hPD == INVALID_HANDLE_VALUE)
	{
		printf("[!] Can't open %s (read)\n",backup);
		return -1;
	}
	if(!ReadFile(hPD, &beginWipe, 4, &len, NULL))
	{
		printf("[!] Can't read.\n");
		CloseHandle(hPD);
		return -1;
	}
	if(!ReadFile(hPD, &wipeSize, 4, &len, NULL))
	{
		printf("[!] Can't read.\n");
		CloseHandle(hPD);
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

	if(!write_partition_table)
	{
		printf("[-]Writing saved MBR.\n");
		if(!WriteFile(hPD,originalMbr,MBR_BOOTLOADER_AREA_SIZE,&len,NULL))
		{
			printf("[!] Can't write %s sector at offset 0x%x\n",device,mbr_offset);
			CloseHandle(hPD);
			free(originalMbr);
			return -1;
		}
	}
	else
	{
		printf("[-]Writing original MBR + partition table. Continue ?\n");
		system("pause");
		if(!WriteFile(hPD,originalMbr,bytes_per_sector,&len,NULL))
		{
			printf("[!] Can't write %s sector at offset 0x%x\n",device,mbr_offset);
			CloseHandle(hPD);
			free(originalMbr);
			return -1;
		}
	}

	free(originalMbr);

	if(wipe_bootkit)
	{
		if(wipeSize > 0x10000 || beginWipe == 0)
		{
			printf("[!] Backup file error.\n");
			CloseHandle(hPD);
			return -1;
		}

		printf("[-]Wiping %d bytes at 0x%x. Continue ?\n", wipeSize, beginWipe);
		system("pause");

		originalMbr = (PBYTE)malloc(wipeSize);
		memset(originalMbr,0,wipeSize);
		if(SetFilePointer(hPD,beginWipe,NULL,FILE_BEGIN) != beginWipe)
		{
			printf("[!] Can't go to offset 0x%x\n",beginWipe);
			CloseHandle(hPD);
			free(originalMbr);
			return -1;
		}
		if(!WriteFile(hPD,originalMbr,wipeSize,&len,NULL))
		{
			printf("[!] Can't wipe %d bytes at 0x%x\n",wipeSize,beginWipe);
			CloseHandle(hPD);
			free(originalMbr);
			return -1;
		}

	}

	free(originalMbr);
	CloseHandle(hPD);
	return 0;
}
