SDCC=/usr/libexec/sdcc/sdcc
SDLD=sdld
OBJECTS=SSD1306.ihx SSD1306.rel SSD1306.asm SSD1306.cdb SSD1306.lst SSD1306.map SSD1306.sym SSD1306.lk SSD1306.rst

.PHONY: all clean flash

all: $(OBJECTS)

clean:
	rm -f $(OBJECTS)

flash: SSD1306.ihx
	stm8flash -cstlinkv2 -pstm8s103f3 -w SSD1306.ihx

%.ihx: %.c
	$(SDCC) -lstm8 -mstm8 --out-fmt-ihx $(CFLAGS) $(LDFLAGS) $<
