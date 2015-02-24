#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mitar.h"

extern char *uso;

/** Copia el nBytes del fichero origen al fichero destino.
 *
 * origen: puntero al descriptor FILE del fichero origen
 * destino: puntero al descriptor FILE del fichero destino
 * nBytes: numero de bytes a copiar
 *
 * Devuelve el numero de bytes copiados o -1 en caso de error. 
 */
int copynFile(FILE *origen, FILE *destino, int nBytes){
 // Completar la funci�n
	void *ptr;
	ptr= malloc(nBytes);
	fread(ptr, nBytes, 1,origen);
	fwrite(ptr,1,nBytes,destino);

	return nBytes;
}

/** Carga en memoria la cabecera del tarball.
 *
 * tarFile: puntero al descriptor FILE del tarball
 * header: direcci�n de un puntero que se inicializar� con la direcci�n de un
 * buffer que contenga los descriptores de los ficheros cargados.
 * nFiles: direcci�n de un entero (puntero a entero) que se inicializar� con el
 * n�mero de ficheros contenido en el tarball (primeros bytes de la cabecera).
 *
 * Devuelve EXIT_SUCCESS en caso de exito o EXIT_FAILURE en caso de error
 * (macros definidas en stdlib.h).
 */
int readHeader(FILE *tarFile, stHeaderEntry **header, int *nFiles){
 // Completar la funci�n 
	stHeaderEntry* array=NULL;
	int nr_files=0;

	array = malloc(sizeof(stHeaderEntry)*nr_files);

	(*nFiles)=nr_files;
	(*header)=array;

	return (EXIT_SUCCESS);
}

/** Carga una cadena de caracteres de un fichero.
 *
 * file: direcci�n de la estructura FILE del fichero
 * buf: direcci�n del puntero en que se inicializar� con la direcci�n donde se
 * copiar� la cadena. Ser� una direcci�n del heap obtenida con malloc.
 *
 * Devuelve: 0 si tuvo exito, -1 en caso de error.
 */
int loadstr( FILE *file, char** buf ){
 // Completar la funci�n 
}

/** crea un tarball a partir de unos ficheros de entrada.
 *
 * nfiles:    numero de ficheros de entrada
 * filenames: array con los nombres de los ficheros de entrada (rutas)
 * tarname:   nombre del tarball
 * 
 * devuelve exit_success en caso de exito y exit_failure en caso de error
 * (macros definidas en stdlib.h).
 *
 * ayuda: calcular primero el espacio necesario para la cabecera del tarball,
 * saltar dicho espacio desde el comienzo del archivo e ir copiando uno a uno
 * los ficheros, rellenando a la vez una representaci�n de la cabecera del
 * tarball en memoria. al final copiar la cabecera al comienzo del tarball.
 * recordar que  que no vale un simple sizeof de stheaderentry para saber el
 * tama�o necesario en disco para el descriptor de un fichero puesto que sizeof
 * no calcula el tama�o de la cadena apuntada por name, sino el tama�o del
 * propio puntero. para calcular el tama�o de una cadena c puede usarse la
 * funci�n strlen.
 */
int createTar(int nFiles, char *fileNames[], char tarName[]) {
 // Completar la funci�n 
	int i=0,fileSize,strShift=0;
	FILE * out = fopen( tarName, 'a+');
	char fileName[];
	stHeaderEntry arrayCabecera[];
	
	arrayCabecera = malloc(sizeof(stHeaderEntry)* nFiles);
	
	FILE* pinput = arrayCabecera + nFiles*sizeof(stHeaderEntry) + sizeof(int);
	
	for(i = 0; i<nFiles;i++){
		//Copia de archivo
		strcpy(fileName,fileNames);

		FILE* tarFile = fopen(fileName, 'a+');

		fseek(tarFile, 0L, SEEK_END);
		fileSize = ftell(tarFile);
		fseek(tarFile,0L, SEEK_SET);
		copynFile(tarFile, pinput,fileSize);
		fclose(tarFile);

		//Escribir la cabecera
		arrayCabecera[i].name = fileNames+strShift;
		strShift += strlen(fileName);
		arrayCabecera[i].size = fileSize;
	}
	fwrite(&nFiles,sizeof(int),1,out);
	strShift=0;
	for(i=0;i<nFiles;i++){
		fwrite(fileNames+strShift,1,strlen(fileNames+strShift),out);
		strShift += strlen(fileNames+strShift);
	}
	fclose(out);
}

/** Extrae todos los ficheros de un tarball.
 *
 * tarName: cadena C con el nombre del tarball
 *
 * Devuelve EXIT_SUCCES en caso de exito y EXIT_FAILURE en caso de error (macros
 * definidas en stdlib.h).
 *
 * AYUDA: debemos cargar primero la cabecera, con lo que nos situaremos al
 * comienzo del primer fichero almacenado. Entonces recorriendo la cabecera
 * sabremos el nombre que debemos ir dando a los ficheros y la cantidad de bytes
 * que debemos leer del fichero para cargar su contenido.
 */
int extractTar(char tarName[]) {
 // Completar la funci�n 
}
  
