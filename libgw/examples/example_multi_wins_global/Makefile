# This is the template Makefile for the projects under  mylib/. Please specify the variable ProjectName first.

ProjectName=example_multi_wins_global

.PHONY: all
all: \
	$(ProjectName)

.PHONY: $(ProjectName)
$(ProjectName):
	$(MAKE) -f $(ProjectName).mak

.PHONY: clean
clean:
	$(MAKE) -f $(ProjectName).mak clean

.PHONY: depends
depends:
	$(MAKE) -f $(ProjectName).mak depends

