# Tutorial: [How to write a simple side scrolling game in GBDK](https://pastebin.com/F3tHLj68) by Jason

CC = lcc -Wa-l -Wl-m  -Wl-j
PNG2ASSET = png2asset

BINS	= neon_gb.gb

# Add entries here to have them converted into metasprites
PNGSOURCES   = sprite.png
PNG_CSOURCES = $(PNGSOURCES:%.png=%.c)
PNG_CHEADERS = $(PNGSOURCES:%.png=%.h)

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

# Use png2asset to convert the png into C formatted metasprite data
# -sh 16   : Sets sprite height to 16 (width remains automatic)
# -spr8x16 : Use 8x16 hardware sprites
# -c ...   : Set C output file
%.c:	%.png
	$(PNG2ASSET) $< -sh 16 -spr8x16 -c $@ 

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
sprite.o:	sprite.c
	$(CC) -Wf-bo2 -c -o $@ $<

neon_gb.gb:	neon_gb.o bank_1_title.o bank_2_game.o sprite.o
	$(CC) -Wl-yt0x1A -Wl-yo4 -Wl-ya4 -o $@ neon_gb.o bank_1_title.o bank_2_game.o sprite.o
