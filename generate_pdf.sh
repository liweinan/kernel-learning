#!/bin/sh

fold -sw 80 KernelWorld.md > wrapped.md
pandoc -s --toc --top-level-division=chapter --template=template.tex wrapped.md -t latex > KernelWorld.latex
pandoc --latex-engine=xelatex --template=template.tex --toc --top-level-division=chapter wrapped.md -t latex -o KernelWorld.pdf
