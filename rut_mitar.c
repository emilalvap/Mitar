#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mitar.h"
#include <limits.h>
#define MAX_NAME_SIZE 32
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
	int i=0;
	void* ptr = malloc(1);
	int rt=0;
	for(i=0;i<nBytes && feof(origen)==0;i++){
		rt = fread(ptr,1,1,origen);
		if(feof(origen)==0){
			if(rt !=1) return (-1);
			rt = fwrite(ptr,1,1,destino);
			if(rt !=1) return (-1);
		}
		else break;
	}
	free(ptr);
	return i;
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
	int i =0;
	int* psize = malloc(sizeof(int));
	char* pname = NULL;
	int size;
	nr_files = *nFiles;
	
	array = malloc(sizeof(stHeaderEntry)*nr_files);
	
	//Se lee la información
	for(i =0;i<nr_files;i++){
		//nombre
		
		size = loadstr(tarFile,&pname);
		if(size != 0 ) return (EXIT_FAILURE);
		array[i].name = pname;	
		//tamaño
		size = fread(psize,4,1,tarFile);
		//if(size != 4 ) return (EXIT_FAILURE);
		array[i].size = *psize;
		
		
	}

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
	int rt=0;
	int found = -1;
	int namesize = 0;
	char* name = malloc(1);
	while(found == -1 && namesize<MAX_NAME_SIZE){
		rt = fread(name,1,1,file);
		if(rt !=1) return (EXIT_FAILURE);
		if(*name=='\0')found=0;
		namesize++;
	}
	
	rt = fseek(file, -namesize,SEEK_CUR);
	if(rt !=0) return (EXIT_FAILURE);
	
	free(name);

	name = malloc(namesize);
	rt = fread(name,namesize,1,file);
	if(rt !=1) return (EXIT_FAILURE);
	*buf = name;
	return found;
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
	int i,shift_amount=0, rt=0,size;
	void* ptr = malloc(1);
	FILE *out;
	FILE* input;
	stHeaderEntry* arrayCabecera;
	arrayCabecera = malloc(sizeof(stHeaderEntry)* nFiles);

	for(i=0;i<nFiles;i++){
		shift_amount+=strlen(fileNames[i])+1;
		arrayCabecera[i].name = fileNames[i];
	}
	shift_amount+= (nFiles+1)*4;

	out = fopen(tarName,"w");
	if(out == NULL) return (EXIT_FAILURE);

	rt = fwrite(ptr,shift_amount,1,out);
	
	for(i=0;i<nFiles;i++){

		input = fopen(fileNames[i],"r");
		if(input == NULL) return (EXIT_FAILURE);
		size = copynFile(input,out,INT_MAX);

		if(size != -1) arrayCabecera[i].size = size;
		else return (EXIT_FAILURE);
		if(fclose(input)!=0) return (EXIT_FAILURE);
	}

	rt = fseek(out,0,SEEK_SET);
	if(rt != 0) return (EXIT_FAILURE);
	
	rt = fwrite(&nFiles,4,1,out);
	//if(rt!=4) return (EXIT_FAILURE);

	for(i=0;i<nFiles;i++){
		
		rt = fwrite(arrayCabecera[i].name,strlen(arrayCabecera[i].name)+1,1,out);
		if(rt == -1) return (EXIT_FAILURE);
		rt = fwrite(&arrayCabecera[i].size,4,1,out);
		if(rt == -1) return (EXIT_FAILURE);
	}	
	free(arrayCabecera);
	free(ptr);
	if(fclose(out)!=0) return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
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
	int rt=0,i;
	int* nr_files = malloc(sizeof(int));	
	FILE* out;
	FILE* tar = fopen(tarName,"r");	
	stHeaderEntry* arrayCabecera;
	if(tar==NULL) return (EXIT_FAILURE);
	rt = fread(nr_files,4,1,tar);
	//if(rt!=4) return (EXIT_FAILURE);
	rt = readHeader(tar,&arrayCabecera,nr_files);
	
	for(i=0;i<*nr_files;i++){
		out = fopen(arrayCabecera[i].name, "w");
		if(out==NULL) return (EXIT_FAILURE);
		//Escribimos el archivo			
		rt = copynFile(tar,out,arrayCabecera[i].size);
		if(rt!= arrayCabecera[i].size)return (EXIT_FAILURE);
		if(fclose(out)!=0) return (EXIT_FAILURE);	
	}
	free(nr_files);
	free(arrayCabecera);
	if(fclose(tar)!=0) return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
  
