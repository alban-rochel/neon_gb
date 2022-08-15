# Tutorial: [How to write a simple side scrolling game in GBDK](https://pastebin.com/F3tHLj68) by Jason

CC = lcc -Wa-l -Wl-m  -Wl-j

BINS	= neon_gb.gb

all:	$(BINS)

%.o:	%.c
	$(CC) -c -o $@ $<

%.s:	%.c
	$(CC) -S -o $@ $<

%.o:	%.s
	$(CC) -c -o $@ $<

%.gb:	%.o
	$(CC) -o $@ $<

clean:
	rm -f *.o *.lst *.map *.gb *~ *.rel *.cdb *.ihx *.lnk *.sym *.asm *.noi *.sav

# Compile bank 0 (no ROM)
#      RAM bank 0 : -Wf-ba0
#
neon_gb.o:	neon_gb.c
	$(CC) -Wf-ba0 -c -o $@ $<

# Compile bank 1 (ROM~+RAM~)
#      ROM bank 1 : -Wf-bo1
#      RAM bank 1 : -Wf-ba1
#
bank_1_title.o:	bank_1_title.c
	$(CC) -Wf-bo1 -c -o $@ $<

# Compile bank 2 (ROM~+RAM~)
#      ROM bank 2 : -Wf-bo2
#      RAM bank 2 : -Wf-ba2
#
bank_2_game.o:	bank_2_game.c
	$(CC) -Wf-bo2 -c -o $@ $<

neon_gb.gb:	neon_gb.o bank_1_title.o bank_2_game.o
	$(CC) -Wl-yt0x1A -Wl-yo4 -Wl-ya4 -o $@ neon_gb.o bank_1_title.o bank_2_game.o
