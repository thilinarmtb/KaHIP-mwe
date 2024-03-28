CC ?= mpicc
CFLAGS ?= -O2 -g
KAHIP_DIR ?= ${HOME}/KaHIP

all: mwe

mwe : mwe.c
	$(CC) $(CFLAGS) mwe.c -o mwe -I$(KAHIP_DIR)/include -Wl,-rpath,$(KAHIP_DIR)/lib -L$(KAHIP_DIR)/lib -lparhip_interface -lkahip

clean:
	@rm mwe || true
