# stlport directory where you cloned
STLPORT_DIR := $(HOME)/stlport-arduino

# library path where stlport will be installed
STLPORT_LIB_DIR := $(USER_LIB_PATH)/stlport

# export flags for sub-make
export CFLAGS CPPFLAGS CXXFLAGS ASFLAGS LDFLAGS

stlport:
	@# build stlport and install static library into objdir
	$(STLPORT_DIR)/configure --target=avr --libdir=$(CURDIR)
	$(MAKE) -C $(STLPORT_DIR) install-release-static

stlport-clean:
	$(MAKE) -C $(STLPORT_DIR) clean

stlport-install-header:
	cp -r $(STLPORT_DIR)/stlport $(STLPORT_LIB_DIR)
	@# create dummy header for arduino
	touch $(STLPORT_LIB_DIR)/stlport.h

stlport-uninstall-header:
	rm -rf $(STLPORT_LIB_DIR)
