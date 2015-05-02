#************************************************
#*  BCL: Makefile                               *
#*  =============                               *
#*                                              *
#*  file makefile                               *
#*  `````````````                               *
#*  Makefile for Xpress-BCL C++ examples        *
#*                                              *
#*  (c) 2008 Fair Isaac Corporation             *
#*      author: S.Heipcke, 2000                 *
#************************************************

include makexprb

#ALL=xbchess 
#ALL=sample 
ALL=xbdndp 

all: $(ALL)

clean:
	rm -f $(ALL) *.mat *.lp
