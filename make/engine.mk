#Makefile hackery shamelessly lifted from LK https://github.com/littlekernel


PLAT = $(shell uname -s)


#Macro used to get directory of makefile
GET_LOCAL_DIR    = $(patsubst %/,%,$(dir $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))))
