BITS 16

;; Ryan Loizzo
;; 1/23/18
;; Daily 3

start:
       	cli             ; turn off interrupts for the prelude
			; if interrupts are on, a hardware device
			; could alter a register and prevent the
			; computer from booting any further

        mov ax, 07C0h   ; Sets register ax to 07C0h because that is where the bootloader starts
        mov ds, ax      ; Copies the contents of ax to the data segment, because the data segment is used later when the bootloader accesses data.

        add ax, 0020h   ; Adjust the initial stack pointer to add 32 bytes (512 bytes / 16 segments) 
        mov ss, ax      ; Start the stack pointer at the beginning of the bootloader 
        mov sp, 1000h   ; Move the stack pointer 4096 bytes from the beginning of the bootloader, Essentially allocating 4096 (4KB) bytes of memory for the stack

        sti             ; turn the interrupts back on

	;	call print
		
		call readkey
		
		jmp $

readkey:
.loop2:
		mov ah, 0
		int 16h

		mov ah, 0Eh
		int 10h
		
		cmp al, 13
		je .done
		int 10h

		jmp .loop2
.done:
		mov al, 10
		int 10h 
		mov al, 13
		int 10h
		jmp .loop2

print:
		mov si, thestring ; set si register to first char in thestring	
		mov ah, 0Eh
		mov bh, 0h
		mov bl, 0h
.loop:		
		lodsb
		cmp al, 0h
		je .endstr 
		
		int 10h
		jmp .loop

.endstr:
		ret

		thestring db 'mcduck', 0

	; the lines below are nasm directives, NOT x86 instructions
	; they tell nasm to fill the rest of the space from our
	; current location to 510 bytes with zeros
	; then set the last two bytes (dw = define word) to AA55
	; this is by convention, since the firmware validates the bootloader
	; by reading the last two bytes and confirming that they are AA55
	; the firmware will not execute the bootloader without it
	times 510-($-$$) db 0
	dw 0xAA55	; BIOS looks for AA55 at the end of the sector

