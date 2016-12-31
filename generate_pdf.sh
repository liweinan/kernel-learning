#!/bin/sh

fold -sw 80 $1 > wrapped.md
#pandoc -s --latex-engine=xelatex --template=template.tex wrapped.md -t latex > $1.latex
# --highlight-style=tango
pandoc --latex-engine=xelatex --template=template.tex wrapped.md -t latex -o $1.pdf
convert -verbose -density 200 $1.pdf -quality 100  -alpha remove -background white out.png
convert -append -trim out-*.png $1.png
rm out*.png
rm *.pdf
rm wrapped.md
