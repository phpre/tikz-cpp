export SHELL := /usr/bin/env bash

ifeq ($(V),1)
    SILENTMSG := @true
    SILENTCMD :=
else
    SILENTMSG := @echo
    SILENTCMD := @
endif


export CXX	:= g++
export AR	:= ar


CPPFLAGS := -std=c++23 -O3 -Wall -Wextra

## build main library
LIBDIR		:= lib
OBJLIBDIR	:= lobj
OUTLIBDIR	:= out/lib
OUTLIBINC	:= out/lib/include

LIB_SRCS := $(shell find $(LIBDIR) -name "*.cpp")
LIB_OBJS := $(LIB_SRCS:$(LIBDIR)/%.cpp=$(OBJLIBDIR)/%.o)
LIB_DEPS := $(LIB_SRCS:$(LIBDIR)/%.cpp=$(OBJLIBDIR)/%.d)
LIB_TREE := $(sort $(patsubst %/,%,$(dir $(LIB_OBJS))))

DEPENDS += $(LIB_OBJS:.o=.d)
export LNAME := tikzcpp
export LIBBIN := $(OUTLIBDIR)/lib$(LNAME).a

.PHONY: generate_figures $(LIBBIN)
.DEFAULT_GOAL = generate_figures

$(LIBBIN): $(LIB_OBJS)
	$(SILENTCMD)mkdir -p $(OUTLIBDIR)
	$(SILENTCMD)mkdir -p $(OUTLIBINC)
	$(SILENTCMD)rm -f "$(LIBBIN)"
	$(SILENTCMD)$(AR) rcs "$(LIBBIN)" $(LIB_OBJS)
	$(SILENTCMD)cp $(LIBDIR)/*.h $(OUTLIBINC)/
	@echo built ... $(notdir $@)

$(LIB_TREE): %:
	mkdir -p $@

.SECONDEXPANSION:
$(OBJLIBDIR)/%.o: $(LIBDIR)/%.cpp | $$(@D)
	$(SILENTMSG) CXX $(notdir $<)
	$(SILENTCMD)$(CXX) -MMD -MP -MF $(@:$(OBJLIBDIR)/%.o=$(OBJLIBDIR)/%.d) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

## build executable for tikz code generation
SRCDIR	:= src
OBJDIR	:= obj
BINDIR	:= bin
OUTDIR	:= out

SRCS := $(shell find $(SRCDIR) -name "*.cpp")
BINS := $(SRCS:$(SRCDIR)/%.cpp=$(BINDIR)/%)
OBJS := $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(LIBBIN)
	$(SILENTCMD)mkdir -p $(OBJDIR)
	$(SILENTMSG) CXX $(notdir $<)
	$(SILENTCMD)$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(OUTLIBINC) -o $@ -c $<

$(BINDIR)/%: $(OBJDIR)/%.o
	$(SILENTCMD)mkdir -p $(BINDIR)
	$(SILENTCMD)$(CXX)  $(LDFLAGS) -L$(OUTLIBDIR) -l$(LNAME) -o $@ $<
	@echo built ... $(notdir $@)

$(TREE): %:
	mkdir -p $@


## produce and compile figures
FIGDIR := figs
TEXDIR := tex

GEN_FIGS := $(SRCS:$(SRCDIR)/%.cpp=generate_figures_%)
FIGS := $(wildcard $(FIGDIR)/*.tex)
PDFS := $(FIGS:%.tex=%.pdf)

COMMIT_EPOCH = $(shell git log -1 --pretty=%ct)
THIS_FILE := $(lastword $(MAKEFILE_LIST))

generate_single_%: $(BINDIR)/%
	@mkdir -p $(FIGDIR)
	@mkdir -p $(OUTDIR)
	@$< "$(FIGDIR)/$(notdir $<)_" "$(TEXDIR)/"
	@$(MAKE) -f $(THIS_FILE) figures
	cp $(FIGDIR)/$(notdir $<)_*.pdf $(OUTDIR)

generate_figures_%: $(BINDIR)/%
	@mkdir -p $(FIGDIR)
	@mkdir -p $(OUTDIR)
	@$< "$(FIGDIR)/$(notdir $<)_" "$(TEXDIR)/"

$(FIGDIR)/%.pdf: $(FIGDIR)/%.tex
	@SOURCE_DATE_EPOCH=$(COMMIT_EPOCH) latexmk -f -silent -pdf -lualatex -use-make -output-directory=$(FIGDIR) $< >/dev/null || true

figures:	$(PDFS)
	@echo "built ... $^"

generate_figures: $(GEN_FIGS)
	@$(MAKE) -f $(THIS_FILE) figures
	cp $(FIGDIR)/*.pdf $(OUTDIR)

clean:
	$(SILENTCMD)rm -r $(OBJLIBDIR) $(OBJDIR) $(BINDIR) $(FIGDIR) new_files.d || true

include new_files.d

new_files.d:
	rm -f $@
	$(eval FIGS := $(wildcard $(FIGDIR)/*.tex))
	$(eval PDFS := $(FIGS:%.tex=%.pdf))
	echo "figures: $(PDFS)" >>$@
