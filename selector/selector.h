/*  Selector for GP2X

    File: selector.h
	
    Copyright (C) 2005-2006  Kounch
    Parts Copyright (C) 2005-2006 Hermes/PS2Reality and modified by Vimacs and god_at_hell(LCD-part)
    Parts Copyright (c) 2004-2005 by Daniel Aarno

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

#define __VERSION_PROG__ "1.2"

#ifndef SELECTORMAIN
#define SELECTORMAIN 

#define MAXLEN 255
#define MAXDIRS 255
#define MAXPATH 512

#define CONSOLELINES 9

int leeArchivo(char *nombre);

int analizaVar (char *linea);

int interfaz(void);

int filtraArchivos(char *nombreDir, char *listaArchivos[], int *numArchivos);

int ejecutaArchivo(char *ruta);

int procesaDirectorio(char *dirActual, char *nombre);

int actualizaInterfaz(int coordenada,char *listaArchivos[], int numArchivos);

int pintaColumnas(int ordenada, int coordenada,char *listaArchivos[],int numArchivos, int linelen);

int muestraError(void);

int leeDatos(void);

#ifndef MACTEST
  int gp2x_getversion(void);

  int gp2x_setcpu(void);
  
  void ClearScreen(int col);
  
  int cargaskin();

  int pintaskin(void);

  void v_putcad(int x,int y,unsigned color,char *cad);

  void gp2x_sound_frame(void *blah, void *buff, int samples);
#endif

#endif
