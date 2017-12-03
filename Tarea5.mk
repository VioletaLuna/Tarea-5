#Make file de la tarea 5

All: *pdf

*.pdf: grafica.png
	pdflatex Results_hw5.tex

grafica.png: resultados.dat
	python Plots.py

resultados.dat: CurvaRotacion.c
	cc CurvaRotacion.c -o CurvaRotacion.x -lm
	./CurvaRotacion.x 