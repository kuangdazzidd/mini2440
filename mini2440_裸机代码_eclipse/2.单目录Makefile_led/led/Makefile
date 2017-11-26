

PRJNAME = led
source = start.o main.o led.o


DEBUG_DIR = Debug
COMPLE  = arm-none-eabi

¡£PHONY : all

objects = $(PRJNAME).bin $(PRJNAME).elf $(PRJNAME).dis

all: $(objects)
	
$(PRJNAME).bin : $(PRJNAME).elf
	$(COMPLE)-objcopy -O binary -S $(PRJNAME).elf $(PRJNAME).bin
	
%.o: %.c
	$(COMPLE)-gcc -g -c -o $@ $<
		
%.o: %.S
	$(COMPLE)-gcc -g -c -o $@ $<
			
$(PRJNAME).elf : $(source)
	$(COMPLE)-ld -Ttext 0x40000000 -g $(source) -o $(PRJNAME).elf
	
$(PRJNAME).dis : $(PRJNAME).elf
	$(COMPLE)-objdump -D -m arm  $(PRJNAME).elf > $(PRJNAME).dis
	
¡£PHONY : clean	
clean:
	rm -f   *.bin *.elf *.o *.dis
	
	