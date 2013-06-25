org 0x0
	ld a, 5
start:
	dec a 
	jp z, end
	jp start
end:
	halt
