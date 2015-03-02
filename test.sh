#!/bin/bash
if [[ -x mitar ]]; then
	#statements
	chmod +x mitar;
	if [[ -d tmp ]]; then
		echo -ne Borrando tmp...;rm -rf tmp; echo Finalizado
	fi
	mkdir tmp;
	cd tmp;
	echo "Hola Mundo!" > fich1.txt;
	head -n 10 /etc/passwd > fich2.txt;
	head -c 1024  /dev/urandom > fich3.txt;
	../mitar -c -f fichtar.mtar fich1.txt fich2.txt fich3.txt;
	mkdir out;
	cp fichtar.mtar out/;
	cd out;
	../../mitar -x -f fichtar.mtar;
	for i in {1..3}
	do
		diff "fich$i.txt" "../fich$i.txt";
		if [[ $? != 0 ]]; then
			cd ../..;
			echo "La comprobación del fichero fich$i.txt ha fallado";
			exit 1;
		fi	
	done
	cd ../..;
	echo Correcto;
	exit 0;
else echo No existe el archivo ejecutable \"mitar\";
fi