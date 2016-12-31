#!/bin/sh

fold -sw 80 $1 > wrapped.md
pandoc -s --toc --top-level-division=chapter --template=template.tex wrapped.md -t latex > $1.latex
pandoc --latex-engine=xelatex --template=template.tex --toc --top-level-division=chapter wrapped.md -t latex -o $1.pdf
