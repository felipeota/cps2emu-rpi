/*  Selector for GP2X

    File: lang.c
	
    Copyright (C) 2005  Kounch

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lang.h"

extern char *langStrings [LANGSTRINGSN];

int initLang(char *language)
  {
  int j;
  
  printf ("Init Lang: %s\n",language);
  
  for (j=0;j<LANGSTRINGSN;j++)
    {
	if (langStrings[j]==NULL) langStrings[j]=(char*) malloc (MAXSTRINGLEN);
	}

  if (!strcmp(language,"TR"))	
	{
	strcpy(langStrings[0],"Yanlis arguman, %i zaten kullanilmis, sadece bir kez kullanilmasi gerekir.\n");
	strcpy(langStrings[1],"Yapilandirma dosyasi yolu: %s\n");
	strcpy(langStrings[2],"Yapilandirma dosyasi okunurken hata olustu:-(\n");
	strcpy(langStrings[3],"Kullanici arayuz rutininde hata.\n");
	strcpy(langStrings[4],"%s dosyasi analiz ediliyor...\n");
	strcpy(langStrings[5],"Dosya acililirken hata olutu.\n");
	strcpy(langStrings[6],"%s anahtari icinde %s degeri bulundu");
	strcpy(langStrings[7],"\nGecerli yol=%s\n");
	strcpy(langStrings[8],"HATA: Klasore erisilemiyor.\n");
	strcpy(langStrings[9],"Klasor olarak atanan: %s\n");
	strcpy(langStrings[10],".%s uzantili %s dosyasi icin atanan : %s\n");
	strcpy(langStrings[11],"Filtre yok seklinde atanan : %s\n");
	strcpy(langStrings[12],"HATA: Klasor kapatilamiyor.\n");
	strcpy(langStrings[13],"\n%s klasorune git.");
	strcpy(langStrings[14],"\nBaslat ");
	strcpy(langStrings[15],"\nBaslatirken hata olustu.\n");
	strcpy(langStrings[16],"%s klasorunu ac");
	strcpy(langStrings[17],"Cikmak icin START'a basin.");
	strcpy(langStrings[18],"Firmware dosya versiyonu okunurken hata olustu.\n");
	strcpy(langStrings[19],"Yukleniyor...");
	}
  else if (!strcmp(language,"EN"))
    {
	strcpy(langStrings[0],"Wrong arguments, detected %i, there should be only one.\n");
	strcpy(langStrings[1],"Config file path: %s\n");
	strcpy(langStrings[2],"Error while reading config file :-(\n");
	strcpy(langStrings[3],"Error in user interface routine.\n");
	strcpy(langStrings[4],"Analyzing file %s...\n");
	strcpy(langStrings[5],"Error while opening file.\n");
	strcpy(langStrings[6],"Found key %s with value %s");
	strcpy(langStrings[7],"\nCurrent Path=%s\n");
	strcpy(langStrings[8],"ERROR: Cannot access to directory.\n");
	strcpy(langStrings[9],"Assigned as it is directory: %s\n");
	strcpy(langStrings[10],"Assigned as extension .%s comparison with %s: %s\n");
	strcpy(langStrings[11],"Assigned as there are no filters: %s\n");
	strcpy(langStrings[12],"ERROR: Cannot close directory.\n");
	strcpy(langStrings[13],"\nGo to dir %s");
	strcpy(langStrings[14],"\nExecute ");
	strcpy(langStrings[15],"\nError while executing.\n");
	strcpy(langStrings[16],"Enter dir %s");
	strcpy(langStrings[17],"Press Start to Exit");
	strcpy(langStrings[18],"Error reading firmware file version.\n");
	strcpy(langStrings[19],"Loading...");
    }
  else if (!strcmp(language,"ES"))
    {
	strcpy(langStrings[0],"Argumentos incorrectos, se han detectado %i, deberia ser solo uno.\n");
	strcpy(langStrings[1],"Ruta del archivo de configuracion: %s\n");
	strcpy(langStrings[2],"Error al leer el archivo de configuracion :-(\n");
	strcpy(langStrings[3],"Error en la rutina de interfaz de usuario.\n");
	strcpy(langStrings[4],"Analizando el archivo %s...\n");
	strcpy(langStrings[5],"Error abriendo el archivo.\n");
	strcpy(langStrings[6],"Encontrada clave %s con valor %s");
	strcpy(langStrings[7],"\nDirectorio Actual=%s\n");
	strcpy(langStrings[8],"ERROR: No se pudo acceder al directorio.\n");
	strcpy(langStrings[9],"Asignado por ser directorio: %s\n");
	strcpy(langStrings[10],"Asignado por extension .%s comparada con %s: %s\n");
	strcpy(langStrings[11],"Asignado por no haber filtros: %s\n");
	strcpy(langStrings[12],"ERROR: No se pudo cerrar el directorio.\n");
	strcpy(langStrings[13],"\nIr al directorio %s");
	strcpy(langStrings[14],"\nEjecutar ");
	strcpy(langStrings[15],"\nError al ejecutar.\n");
	strcpy(langStrings[16],"Entrar en dir %s");
	strcpy(langStrings[17],"Pulse Start para Salir");
	strcpy(langStrings[18],"Error abriendo el archivo de version de firmware.\n");
	strcpy(langStrings[19],"Cargando...");
	}

  else
    {
	fprintf(stderr,"LANG ERROR!!! Returning to default ");
	initLang(DEFAULTLANG);
	}
	
  return (0);
  }
