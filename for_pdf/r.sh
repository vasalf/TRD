for f in trd.tex; do 
  name=${f%.*}
  rm -f $name.{toc,log,out,aux}
  xelatex -shell-escape $name.tex || exit
  xelatex -shell-escape -interaction=nonstopmode >/dev/null $name.tex || exit
  xelatex -shell-escape -interaction=nonstopmode >/dev/null $name.tex || exit
  rm -f $name.{toc,log,out,aux,dvi,pyg}
  rm -rf _minted*
done
