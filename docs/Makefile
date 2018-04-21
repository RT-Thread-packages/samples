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
LATEX_FLAGS = --toc --latex-engine=xelatex
EXTRA_FLAGS = -f markdown+grid_tables+table_captions
MANUAL_DIR = platform
MANUAL_POSTFIX = manual
PANDOC = pandoc
MKDOCS2PANDOC = mkdocs2pandoc
MANUAL_TEMPLATE = template/manual.tex
ARTICLE_TEMPLATE = template/article.tex
CHAPTERS = chapters
PREFACE = preface
BUILD_DIR = build

all:$(patsubst %.md,$(BUILD_DIR)/%.pdf,$(wildcard *.md)) \
	$(patsubst $(MANUAL_DIR)/%.md,$(BUILD_DIR)/%-$(MANUAL_DIR).pdf,$(wildcard $(MANUAL_DIR)/*.md)) \
	$(patsubst $(MANUAL_DIR)/%.md,$(BUILD_DIR)/%-$(MANUAL_POSTFIX).pdf,$(wildcard $(MANUAL_DIR)/*.md))

$(BUILD_DIR)/%-$(MANUAL_POSTFIX).pdf: $(MANUAL_DIR)/%.yml
	$(MKDOCS2PANDOC) -f $^ > $(BUILD_DIR)/$(CHAPTERS).pd
	$(PANDOC) --chapters -o $(PREFACE).tex $(PREFACE).md
	$(PANDOC) $(LATEX_FLAGS) $(EXTRA_FLAGS) --template=$(MANUAL_TEMPLATE) -o $@ $(BUILD_DIR)/$(CHAPTERS).pd

$(BUILD_DIR)/%-$(MANUAL_DIR).pdf: $(MANUAL_DIR)/%.md
	$(MKDOCS2PANDOC) -c $^ > $(BUILD_DIR)/$(CHAPTERS).pd
	$(PANDOC) $(LATEX_FLAGS) $(EXTRA_FLAGS) --template=$(ARTICLE_TEMPLATE) -o $@ $(BUILD_DIR)/$(CHAPTERS).pd

# Generalized rule: how to build a .pdf from each .md
$(BUILD_DIR)/%.pdf: %.md
	$(MKDOCS2PANDOC) -c $^ > $(BUILD_DIR)/$(CHAPTERS).pd
	$(PANDOC) $(LATEX_FLAGS) $(EXTRA_FLAGS) --template=$(ARTICLE_TEMPLATE) -o $@ $(BUILD_DIR)/$(CHAPTERS).pd

# Generalized rule: how to build a .tex from each .md
$(BUILD_DIR)/%.tex: %.md
	$(PANDOC) $(LATEX_FLAGS) -o $@ $<

clean:
	rm -f $(BUILD_DIR)/* $(MAUNUL_DIR)/*.pdf *.aux *.log *.nav *.out *.snm *.toc *.vrb *.tex *.pdf *.pd  || true

.PHONY: all clean
