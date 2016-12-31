#!/bin/sh

fold -sw 80 $1 > wrapped.md
pandoc -s --latex-engine=xelatex --template=template.tex wrapped.md -t latex > $1.latex
pandoc --latex-engine=xelatex --template=template.tex wrapped.md -t latex -o $1.pdf
