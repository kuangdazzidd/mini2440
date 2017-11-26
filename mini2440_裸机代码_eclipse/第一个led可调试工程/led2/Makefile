COMPLE = arm-none-eabi

led_c.bin : start.S main.c led.c
	$(COMPLE)-gcc -g -c -o start.o start.S
	$(COMPLE)-gcc -g -c -o main.o main.c
	$(COMPLE)-gcc -g -c -o led.o led.c
	$(COMPLE)-ld -Ttext 0x40000000 -g start.o led.o main.o -o led_c.elf
	$(COMPLE)-objcopy -O binary -S led_c.elf led_c.bin
	$(COMPLE)-objdump -D -m arm  led_c.elf > led_c.dis
clean:
	rm -f   *.bin *.elf *.o