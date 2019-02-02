###############################################################
#Variables
#		invoke as: make all
#
###############################################################

CFLAGS = -D_POSIX_SOURCE
LDFLAGS = 
CC = cc
LD = cc

TARG = copy

OBJS = a2.o

###############################################################
# Rules
# NB: an indented line has tab character in column 1
###############################################################
all:	$(TARG)

$(TARG): $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(TARG)

clean:
	rm -f $(OBJS) $(TARG) a.out

###############################################################
# Dependencies -- nonde in this program, list as needed
###############################################################
