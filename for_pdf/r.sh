for f in trd.tex; do 
  name=${f%.*}
  rm -f $name.{toc,log,out,aux}
  pdflatex -shell-escape $name.tex || exit
  pdflatex -shell-escape -interaction=nonstopmode >/dev/null $name.tex || exit
  rm -f $name.{toc,log,out,aux,dvi,pyg}
  rm -rf _minted*
done
