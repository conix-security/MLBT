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
//		Displays help.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void help()
{
	printf(
		" Reading sectors :\n"
		" -readsectors S N / -l S N : read and displays N sectors starting from sector S\n"
		" -enumdrives / -e : enumerate available devices\n"
		"\n"
		"\n"
		" Install ASCII ART boot message on device :\n"
		" -asciiArt PATH / -aaW PATH : install bootkit on DEVICE\n"
		" -asciiArtSim PATH / -aaS PATH : simulation mode (no writes on DEVICE)\n"
		"      PATH : display PATH file content message\n"
		"      backup files will be created in the current directory.\n"
		"\n"
		" Remove MBR patch using backup files :\n"
		" -restorebackup / -rb PATH : restore MBR using PATH saved mbr data\n"
		" -rtable / -rt : also restore saved partition table\n"
		" -rwipe / -rw : also wipe written patch code\n"
		"\n"
		"\n"
		" Input options :\n"
		" -physicalDrive X / -p X : use \\\\.\\PhysicalDriveX device (0 < X < 9)\n"
		" -device / -d DEVICE : use DEVICE specific device\n"
		" -vmware / -v : DEVICE is a VMDK file (use 0x510000 offset). ONLY for RESTORE.\n"
		"\n"
		"\n"
		" Backup MBR + part. table :\n"
		" -backup1st PATH / -b1 PATH : backup the device 1st sector in PATH file.\n"
		"\n"
		" Write MBR + part. table :\n"
		" -write1st PATH / -w1 PATH : write PATH content at sector #1.\n"
		"\n"
		" Write typical DOS MBR code :\n"
		" -writedos / -wd : install DOS MBR\n"
		"\n"
		"\n"
		" Install raw code on device :\n"
		" -installRaw PATH / -irW PATH : install PATH on device\n"
		" -installRawSim PATH / -irS PATH : simulation mode (no write on device)\n"
		"      PATH code will be loaded and executed at 0000:0800\n"
		"      Overwriten MBR code will be loaded at 0000:0600 (copy it at 0x7c00 and JMP 0x7c00 to boot)\n"
		"\n"
		"\n"
		" Wipe device :\n"
		" -wipe N : wipe N sectors from sector #2 (included)\n"
		"\n"
		"==============================\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Main.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	int start, len, i, mode;

	BOOL vmware_device = FALSE;
	BOOL enumerate = FALSE;
	BOOL read_mode = FALSE;
	BOOL write_ascii_art_mode = FALSE;
	BOOL simulation = FALSE;
	BOOL restore_dos_mode = FALSE;
	BOOL restore_backup_mode = FALSE;
	BOOL wipe_bootkit = FALSE;
	BOOL restore_table = FALSE;
	BOOL install_raw = FALSE;
	BOOL backup_mbr_mode = FALSE;
	BOOL write_mbr_mode = FALSE;
	BOOL wipe_device = FALSE;

	char* device_name = NULL;
	char* path = NULL;
	char* temp_buffer = NULL;

	bytes_per_sector = 0x200;
	len = 0;
	start = 0;
	mode = 0;

	for(i = 0; i<argc; i++)
	{
		if((!_stricmp(argv[i], "-readsectors") || !_stricmp(argv[i], "-l")) && i<argc-2)
		{
			read_mode = TRUE;
			i++;
			start = atoi(argv[i]);
			i++;
			len = atoi(argv[i]);
			mode++;
		}
		else if(!_stricmp(argv[i], "-wipe") && i<argc-1)
		{
			wipe_device = TRUE;
			i++;
			start = atoi(argv[i]);
			mode++;
		}
		else if(!_stricmp(argv[i], "-rt") || !_stricmp(argv[i], "-rtable"))
			restore_table=TRUE;
		else if(!_stricmp(argv[i], "-rw") || !_stricmp(argv[i], "-rwipe"))
			wipe_bootkit=TRUE;
		else if(!_stricmp(argv[i], "-irW") || !_stricmp(argv[i], "-installRaw") && i<argc-1)
		{
			install_raw=TRUE;
			mode++;
			i++;
			path = argv[i];
		}
		else if(!_stricmp(argv[i], "-irS") || !_stricmp(argv[i], "-installRawSim") && i<argc-1)
		{
			install_raw=TRUE;
			mode++;
			simulation = TRUE;
			i++;
			path = argv[i];
		}
		else if(!_stricmp(argv[i], "-aaW") || !_stricmp(argv[i], "-asciiArt") && i<argc-1)
		{
			write_ascii_art_mode=TRUE;
			mode++;
			i++;
			path = argv[i];
		}
		else if(!_stricmp(argv[i], "-aaS") || !_stricmp(argv[i], "-asciiArtSim"))
		{
			write_ascii_art_mode=TRUE;
			mode++;
			simulation = TRUE;
			i++;
			path = argv[i];
		}
		else if((!_stricmp(argv[i], "-p") || !_stricmp(argv[i], "-physicalDrive")) && i<argc-1)
		{
			i++;
			temp_buffer = argv[i];
			if(strlen(temp_buffer)==1)
			{
				if(temp_buffer[0] > 0x2F && temp_buffer[0] < 0x3A)
				{
					device_name = (char*)malloc(19);
					*device_name = 0x00;
					strcpy_s(device_name,19,"\\\\.\\PhysicalDrive");
					strcat_s(device_name,19,temp_buffer);
				}
			}
		}
		else if((!_stricmp(argv[i], "-b1") || !_stricmp(argv[i], "-backup1st")) && i<argc-1)
		{
			mode++;
			backup_mbr_mode = TRUE;
			i++;
			path = argv[i];
		}
		else if((!_stricmp(argv[i], "-w1") || !_stricmp(argv[i], "-write1st")) && i<argc-1)
		{
			mode++;
			write_mbr_mode = TRUE;
			i++;
			path = argv[i];
		}
		else if((!_stricmp(argv[i], "-rb") || !_stricmp(argv[i], "-restorebackup")) && i<argc-1)
		{
			mode++;
			restore_backup_mode = TRUE;
			i++;
			path = argv[i];
		}
		else if((!_stricmp(argv[i], "-d") || !_stricmp(argv[i], "-device")) && i<argc-1)
		{
			i++;
			device_name = argv[i];
		}
		else if(!_stricmp(argv[i], "-wd") || !_stricmp(argv[i], "-writedos"))
		{
			mode++;
			restore_dos_mode=TRUE;
		}
		else if(!_stricmp(argv[i], "-v") || !_stricmp(argv[i], "-vmware"))
			vmware_device = TRUE;
		else if(!_stricmp(argv[i], "-e") || !_stricmp(argv[i], "-enumdrives"))
		{
			enumerate = TRUE;
			mode++;
		}
	}

	printf(
		"\n"
		"==============================\n"
		"============ MLBT ============\n"
		"==============================\n"
		"\n");

	if(mode == 0)
	{
		help();
		return 0;
	}
	if(mode > 1)
	{
		printf("[!] Please only do one time at once.\n");
		return -1;
	}

	if(enumerate)
	{
		enumerate_drives();
		return 0;
	}

	if(device_name == NULL)
	{
		printf("[!] Please specify the device.\n");
		return 0;
	}

	if(vmware_device)
		bytes_per_sector = 0x200;
	else
		if(read_device_information(device_name)!=0)
			return -1;

	if(read_mode && len!=0)
	{
		if(vmware_device)
			read_sectors_vmdk_file(start, len, device_name);
		else
			read_sectors_device(start, len, device_name);
	}
	else if(wipe_device)
	{
		printf("[!] Wipe %s %d sectors starting from sector #2 : are you sure ?\n",device_name, start);
		system("pause");
		wipe_device_sectors(device_name,start);
	}
	else if(install_raw)
	{
		install_raw_shellcode(path, device_name, simulation);
	}
	else if(restore_backup_mode)
	{
		printf("[!] Restore %s auto-saved MBR data on %s : are you sure ?\n",path, device_name);
		system("pause");
		if(vmware_device)
			restore_from_saved_mbr_vmdk_file(path,device_name, restore_table, wipe_bootkit);
		else
			restore_from_saved_mbr_device(path,device_name, restore_table, wipe_bootkit);
	}
	else if(backup_mbr_mode)
	{
		printf("[!] Backup %s MBR 1st sector on %s.\n",path, device_name);
		if(vmware_device)
			backup_mbr_vmdk_file(path,device_name);
		else
			backup_mbr_device(path,device_name);
	}
	else if(write_mbr_mode)
	{
		printf("[!] Write %s saved MBR 1st sector on %s : are you sure ?\n",path, device_name);
		system("pause");
		if(vmware_device)
			write_mbr_vmdk_file(path,device_name);
		else
			write_from_saved_mbr_device(path,device_name);
	}
	else if(restore_dos_mode)
	{
		printf("[!] Write DOS MBR on %s : are you sure ?\n",device_name);
		system("pause");
		if(vmware_device)
			restore_dos_mbr_vmdk_file(device_name);
		else
			restore_dos_mbr_device(device_name);
	}
	else if(write_ascii_art_mode)
	{
		if(vmware_device)
		{
			printf("[!] VMware patching support is not available.\n");
			return -1;
		}
		if(path == NULL)
		{
			printf("[!] There is no defined template.\n");
			return -1;
		}
		if(!simulation)
		{
			printf("[!] Write bootkit on %s : are you sure ?\n",device_name);
			system("pause");
		}
		write_ascii_art_mbr(device_name, simulation, path);
	}
	else
		help();
	
	printf("\n");
	return 0;
}
