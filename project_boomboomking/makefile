arm_include = /opt/arm-unknown-linux-gnu/arm-unknown-linux-gnu/includes/
arm_path = /opt/arm-unknown-linux-gnu/arm-unknown-linux-gnu/lib/
pxa_path = /home/eostommy/microtime/linux/include/

CC = arm-unknown-linux-gnu-gcc
CFLAGS = -L ${arm_path} -I ${pxa_path} -I ${arm_include} -I.

ODIR=obj
LDIR=../lib

man_file = client
_OBJ = sockop.o boomboomking.o $(man_file).o 
EXE = $(man_file)
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
OUT_DIR  = build

$(ODIR)/%.o: %.c 
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OUT_DIR)/$(EXE): $(OBJ)
	mkdir -p $(dir $@)
	$(CC) -lpthread -o $@ $^ $(CFLAGS)

.PHONY: clean

clean: 
		rm -rf $(ODIR)/*.o

.PHONY: cp
cp:
		cp $(OUT_DIR)/$(EXE) /home/eostommy/nfs
		echo "CP to nfs"
		
