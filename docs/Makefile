# ===== Usage ================================================================
#
# NOTE:
#   When running these commands at the command line, replace $(TARGET) with
#   the actual value of the TARGET variable.
#
#
# make                  Compile all *.md files to PDFs
# make <filename>.pdf   Compile <filename>.md to a PDF
# make <filename>.tex   Generate the intermediate LaTeX for <filename>.md
#
#
# make clean            Get rid of all generated files
#
# ============================================================================

SOURCES = $(shell find . -name 'ss*.md' | sort)
LATEX_FLAGS = --toc --latex-engine=xelatex

all:$(patsubst %.md,%.pdf,$(wildcard *.md)) \
	$(patsubst platform/%.md,platform/%.pdf,$(wildcard platform/*.md)) \
	$(patsubst platform/%.md,platform/%-book.pdf,$(wildcard platform/*.md))

%-book.pdf: %-book.yml %.md $(SOURCES)
	pandoc $(LATEX_FLAGS) --template=template/manual.tex -o $@ $^
	mv $@ pdf/$@

# Generalized rule: how to build a .pdf from each .md
%.pdf: %.md
	pandoc $(LATEX_FLAGS) --template=template/article.tex -o $@ $<
	mv $@ pdf/$@

# Generalized rule: how to build a .tex from each .md
%.tex: %.md
	pandoc $(LATEX_FLAGS) -o $@ $<
	mv $@ pdf/$@

clean:
	rm -f *.aux *.log *.nav *.out *.snm *.toc *.vrb *.tex *.pdf pdf/*.tex pdf/*.pdf pdf/platform/*.tex pdf/platform/*.pdf || true

.PHONY: all clean
