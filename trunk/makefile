#===============================================
#
#
# Copyright (c) 2007-2008 Future Today
# All Rights Reserved.
#  
#
#===============================================

.SILENT:

include common.mk

all: intro engine generator result_info

intro:
	@echo -e "------------------------"
	@echo -e "Generator (c) 2007-2008 Future Today"
	@echo -e "Start build."


result_info:
	@echo -e "\nBuild done. "
	@echo -e "------------------------"

engine:
	$(MAKE) -C "./src/engine" all

generator:
	$(MAKE) -C "./src/generator" all


clean:
	$(MAKE) -C "./src/engine" clean
	$(MAKE) -C "./src/generator" clean
	rm -Rf $(GENERATED_DIR)
