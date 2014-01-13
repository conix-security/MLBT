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
//		Wipe sectors starting from sector #2
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD wipe_device_sectors(char* device_name, DWORD count)
{
	PBYTE buffer;
	HANDLE hPD;
	DWORD len;

	if(count > 0x1000)
	{	
		printf("[!] Too many sectors.\n");
		return -1;
	}

	buffer = (PBYTE)malloc(bytes_per_sector*count);
	memset(buffer, 0, bytes_per_sector*count);
	
	hPD = CreateFileA(device_name,GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
	if(SetFilePointer(hPD,bytes_per_sector,NULL,FILE_BEGIN) != bytes_per_sector)
	{
		printf("[!] Can't go to offset 0x%x\n",bytes_per_sector);
		CloseHandle(hPD);
		free(buffer);
		return -1;
	}
	if(!WriteFile(hPD,buffer,bytes_per_sector*count,&len,NULL))
	{
		printf("[!] Can't wipe.\n");
		CloseHandle(hPD);
		free(buffer);
		return -1;
	}

	printf("[-] Sectors wiped.\n");
	CloseHandle(hPD);
	free(buffer);
	return 0;
}
