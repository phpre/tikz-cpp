SRCDIR := src
OBJDIR := obj
FIGDIR := figs
OUTFIGDIR := ../figs

CC=g++
export LD	:=	$(CC)
LDFLAGS	= -L /opt/homebrew/lib/


SRCS	:= $(shell find $(SRCDIR) -name "*.cpp")
OBJS	:= $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
DEPS	:= $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.d)
TREE	:= $(sort $(patsubst %/,%,$(dir $(OBJS))) lose)
FIGS	:= $(wildcard $(FIGDIR)/*.tex)
PDFS	:= $(FIGS:%.tex=%.pdf)

CFLAGS	=-std=c++23 -MMD -MP -MF $(@:$(OBJDIR)/%.o=$(OBJDIR)/%.d) \
		 -g3 -ggdb -Wall -Wextra
CPPFLAGS= $(CFLAGS)

COMMIT_EPOCH = $(shell git log -1 --pretty=%ct)
THIS_FILE := $(lastword $(MAKEFILE_LIST))


.PHONY: all clean generate_figures

all: generate_figures


.SECONDEXPANSION:
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $$(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

$(TREE): %:
	mkdir -p $@

generate_figures: main
	@mkdir -p $(FIGDIR)
	@./main ./figs/ tex/font tex/color ../macros
	@$(MAKE) -f $(THIS_FILE) figures
	cp $(FIGDIR)/*.pdf $(OUTFIGDIR)


$(FIGDIR)/%.pdf: $(FIGDIR)/%.tex
	@SOURCE_DATE_EPOCH=$(COMMIT_EPOCH) latexmk -silent -pdf -lualatex -use-make -output-directory=$(FIGDIR) $<

figures:	$(PDFS)
	@echo "$^"

main:	$(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

clean:
	$(RM) -r $(OBJDIR) $(FIGDIR) new_files.d main

include new_files.d

new_files.d:
	rm -f $@
	$(eval FIGS := $(wildcard $(FIGDIR)/*.tex))
	$(eval PDFS := $(FIGS:%.tex=%.pdf))
	echo "figures: $(PDFS)" >>$@
