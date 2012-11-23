/*  Selector for GP2X

    File: selector.c
	
    Copyright (C) 2005-2006  Kounch
    Parts Copyright (C) 2005-2006 Hermes/PS2Reality and modified by Vimacs and god_at_hell(LCD-part)
    Parts Copyright (c) 2004-2005 by Daniel Aarno
    Parts Copyright (c) 1998-2005 Gilles Vollant

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
#include <iconv.h>     //Para convertir de UTF-8 a ISO-8859-1
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#include "unzip.h"

extern volatile unsigned short *gp2x_memregs;
void set_speed_clock(int speed)
{
	int mdiv = (speed * 3000000) / 7372800;
	//speed = (mdiv * 7372800) / (3000000);
	mdiv -= 8;
	if(mdiv < 0) mdiv = 0;
	if(mdiv > 255) mdiv = 255;
	gp2x_memregs[0x0910>>1] = (mdiv << 8) | (1 << 2);
	//printf("Set speed clock: %d MHz\n", speed);
}

#ifdef __GP2X__
  #include "minimal.h"
  #include "readpng.h"
  #define WIDTH	320
  #define HEIGHT 240
  #define CPU_SPEED 60
  
  /*From pngread example code:
  * could just include png.h, but this macro is the only thing we need
  * (name and typedefs changed to local versions); note that side effects
  * only happen with alpha (which could easily be avoided with
  * "ush acopy = (alpha);") */
  #define alpha_composite(composite, fg, alpha, bg) {               \
    ush temp = ((ush)(fg)*(ush)(alpha) +                          \
                (ush)(bg)*(ush)(255 - (ush)(alpha)) + (ush)128);  \
    (composite) = (uch)((temp + (temp >> 8)) >> 8);               \
}
  
  /*Colores por defecto*/
  unsigned COLOR_TITULO=0x00ff00; //color del titulo con el copyright, etc
  unsigned COLOR_COMANDO=0xcccccc; //color de la linea de comandos
  unsigned COLOR_LISTADO=0x00ffff; //color de los archivos en el listado
  unsigned COLOR_SELECCION=0xffffff; //color del texto del archivo seleccionado en el listado
  unsigned COLOR_RECTANGULO=0x009000; //color del texto del archivo seleccionado en el listado
  unsigned COLOR_AYUDA=0xff0000; //color del texto de ayuda "Pulsa Start para Salir"
  unsigned COLOR_ERROR=0x00ffff; //color del texto en los mensajes de error
  //unsigned COLORFONDO=0x009000; // color de fondo si no hay imagen de skin
  unsigned COLORFONDO=(((0)&0xF8)<<8)|(((90)&0xFC)<<3)|(((00)&0xF8)>>3);

  char firmVersion [10]; //para guardar la version de firmware detectada
  unsigned short cpuver; //para guardar la version de firmware de forma numerica
  unsigned dclock; //para guardar la velocidad de reloj de la pantalla
  unsigned lfreq; //para guardar la frecuencia de reloj original
  
  unsigned short fondo[320*240]; //Para almacenar una imagen de fondo (si la hay)
#endif

#include "selector.h"
#include "lang.h"

/*Variables globales que se generaran desde el archivo de configuracion*/
char skinPath[MAXLEN]; //Ruta al fondo de la interfaz
int cmdLinesNum; //Numero de opciones de linea de comando detectadas
char *cmdLines[MAXDIRS]; //Vector que almacena las distintas opciones de linea de comando detectadas
char baseDir[MAXPATH]; //Directorio elegido como base para buscar archivos
char *fileFilters[MAXDIRS]; //Vector que almacena las distintas extensiones de archivo a usar como filtro
int fileFiltersNum; //Numero de opciones de linea de comando detectadas
int layoutCode; //Codigo que indica la opcion de disposicion de elementos en la interfaz
int selectRectangle; //Codigo que indica si se debe dibujar o no un rectangulo de selección
int scrollWrap; //Codigo que indica si se activa
int doubleColumn; //Codigo que indica si se activa la vista en dos columnas
unsigned cpuSpeed; //Codigo que indica la velocidad a usar por defecto al lanzar la linea de comandos

int cmdLinesSel; //Variable global que indica la linea de comandos seleccionada actualmente

char *langStrings [LANGSTRINGSN]; //Vector que almacena todos los mensajes de texto del programa

char textoError [MAXLEN]; //para guardar mensajes de error para la funcion muestraError

/*Rutina Main, aqui analizamos los argumentos de linea de comandos*/
int main (int argc, char **argv) 
{
  char cad[MAXLEN];
  
  /*Inicializacion del lenguaje por defecto*/
  initLang(DEFAULTLANG);
  
  #ifdef __GP2X__
    /*Inicializamos la consola */
    gp2x_init(1000, 16, 44100,16,1,50,0); //minimal 0.B

    //Borramos la pantalla
    ClearScreen(COLORFONDO);
    //Mensaje avisando de que el programa está arrancando
    sprintf(cad,langStrings[19]);
    *(cad+39)=0; //Recortamos las lineas para que quede bien en pantalla
    v_putcad(7,14,COLOR_TITULO,cad);

    gp2x_video_waitvsync();gp2x_video_RGB_flip(0); //minimal 0.B

    //Obtenemos el estado de la CPU, etc.
    cpuSpeed=200;
    gp2x_getversion();
    //Cambiamos a CPU_SPEED MHz
    set_speed_clock(CPU_SPEED);
  #endif

  /*Comprobacion de argumentos*/
#ifndef CONFIG_FILE
  if (argc != 2) {
    sprintf (textoError, langStrings[0], --argc);
  	muestraError();
    gp2x_setcpu();
    exit(1); 
  }
#define CONFIG_FILE argv[1]
#endif

  /*Se toma el primer argumento como dato para leer la configuracion*/
  printf(langStrings[1],CONFIG_FILE);

  /*Si hay problemas al leer el archivo de configuracion, nos salimos*/
  if (leeArchivo(CONFIG_FILE))
  {
    sprintf (textoError, langStrings[2]);
    muestraError();
    gp2x_setcpu();
    exit(1);
	}

  /*Si todo ha ido bien, llamamos a la rutina principal*/
  if ( interfaz() )
  {
    sprintf (textoError, langStrings[3]);
    muestraError();
    gp2x_setcpu();
    exit(2);
	}

#ifdef __GP2X__
  printf("Exit...\n");
set_speed_clock(200);
  gp2x_setcpu();
  printf ("End\n");
#endif
  chdir("/usr/gp2x");
  execl("/usr/gp2x/gp2xmenu","/usr/gp2x/gp2xmenu",NULL);
  exit(0);
}


/*Recorre el archivo llamando a las funciones que cargan las variables globales del programa*/
int leeArchivo(char *nombre)
{
  FILE *fp;
  char str[MAXLEN];
  int line;
  
  cmdLinesNum=0;
  fileFiltersNum=0;
  
  //Inicializamos la variable de layout
  layoutCode=0;
  //Inicializamos la variable del rectangulo de seleccion
  selectRectangle=0;
  //Inicializamos la variable de ScrollWrap
  scrollWrap=1;
  //Inicializamos la variable de vista en dos columnas
  doubleColumn=0;

  printf(langStrings[4],nombre);

  fp = fopen(nombre, "rt");
  if(fp == NULL)
  {
    //printf (langStrings[5]);
    return (1);
  }

  for (line=1;fgets(str,MAXLEN,fp);line++)
    {
	if (analizaVar(str))
	  {
	  /*Se ha cargado una variable*/
	  }
	else
	  {
	  /*Es un comentario*/
	  }
	}

  fclose(fp);
  
  //Si no hay ninguna opcion de comandos, devolvemos un error
  if (cmdLines[0] == NULL) return (1);
  //Si no hay ningun directorio base, devolvemos un error
  if (baseDir == NULL) return (2); 
    
  /*Solo para ver que lo ha cargado todo bien, luego tengo que quitarlo*/
  if (skinPath != NULL)
    printf ("\nskinPath=%s\n",skinPath);
  for (line=0;line<cmdLinesNum;line++)
    {
    printf ("cmdLines[%i]=%s\n",line,cmdLines[line]);
    }
  printf ("baseDir=%s\n",baseDir);
  for (line=0;line<fileFiltersNum;line++)
    {
    printf ("fileFilters[%i]=%s\n",line,fileFilters[line]);
    }
  printf("selectRectangle: %i\n",selectRectangle);
  printf("scrollWrap: %i\n",scrollWrap);
  printf("doubleColumn: %i\n",doubleColumn);

  return (0);
}


/*Toma una linea de texto e intenta separarla en base a encontrar un signo '=', obteniendo una clave y un valor*/
/*Si lo consigue, compara con las variables globales a cargar y, si ha encontrado un valor, se lo asigna*/
int analizaVar (char *linea)
{
  char *clave;
  char *valor;
  
  if ( (linea[0] != '#') && (linea[0] != '\n') )
    {
    clave = strtok (linea, "=");
    valor = strtok (NULL, "\n");
    
    //printf(langStrings[6], clave, valor);
    
    if ( !strcmp (clave, "skinPath") && (valor != NULL) ) strcpy(skinPath,valor);
    if ( !strcmp (clave, "cmdLine") && (valor != NULL) )
      {
      cmdLines[cmdLinesNum]=(char*) malloc (MAXLEN);
      strcpy(cmdLines[cmdLinesNum++],valor);
      }
    if ( !strcmp (clave, "baseDir") && (valor != NULL) ) strcpy(baseDir,valor);
    if ( !strcmp (clave, "fileFilter") && (valor != NULL) )
      {
      fileFilters[fileFiltersNum]=(char*) malloc (MAXLEN);
      strcpy(fileFilters[fileFiltersNum++],valor);
      }
    if ( !strcmp (clave, "langCode") && (valor != NULL) ) initLang(valor);
    
    if ( !strcmp (clave, "layoutCode") && (valor != NULL) )
      {
      layoutCode=atoi(valor);
      
      if (layoutCode == 1) //Apaño hasta que lo lea del archivo de config
        {
        COLOR_TITULO=0x44AA44;
        COLOR_COMANDO=0x4444AA; 
        COLOR_LISTADO=0xffffff; 
        COLOR_SELECCION=0x000000; 
        }
      }
    
    if ( !strcmp (clave, "selectRectangle") && (valor != NULL) )
      {
      selectRectangle=atoi(valor);
      }
    
    if ( !strcmp (clave, "scrollWrap") && (valor != NULL) )
      {
      scrollWrap=atoi(valor);
      }
      
    if ( !strcmp (clave, "doubleColumn") && (valor != NULL) )
      {
      doubleColumn=atoi(valor);
      }

    if ( !strcmp (clave, "cpuSpeed") && (valor != NULL) )
    {
      unsigned frecuencias[25]={60,66,80,100,120,133,150,166,200,210,220,240,250,266,270,275,280,285,290,295,300,305,310,315,320}; // por encima de 220 MHz solo funciona con div 1
      int i=24;
      
      cpuSpeed=atoi(valor);
      
      while ((i>0) && (frecuencias[i] > cpuSpeed))
        --i;
            
      /* Deducimos la velocidad de CPU a usar */
      cpuSpeed=frecuencias[i];
    }
    
      //Habia una variable
    return (1);
    }
  //Habia un comentario
  return (0);
}


/* Actualiza la interfaz de usuario al recibir respuesta de la funcion leeDatos */
int interfaz(void)
{
  char dirActual[MAXPATH];
  char *listaArchivos[MAXDIRS];
  int numArchivos;
  char fin;
  char identif;
  char ruta[MAXLEN];
  int x; //coordenada dentro del directorio
  int minline;
  unsigned frecuencias[25]={60,66,80,100,120,133,150,166,200,210,220,240,250,266,270,275,280,285,290,295,300,305,310,315,320}; // por encima de 220 MHz solo funciona con div 1
  int i;
  

  #ifdef __GP2X__
    /*Inicializamos la pantalla */
	
	//printf("cargando skin por primera vez\n");
	if ((x=cargaskin()))
	  {
	  printf ("%i Error(es) en cargaskin\n",x);
      skinPath[0]=0;
	  }

	//printf("pintando skin por primera vez\n");
	pintaskin();
	
	gp2x_video_waitvsync();gp2x_video_RGB_flip(0); //minimal 0.B
  #endif

  cmdLinesSel=0;

  strcpy (dirActual,baseDir);
  x=0;

  /*Inicializamos numArchivos a 0, para que la llamada a filtraArchivos no libere memoria no asignada*/
  numArchivos=0;
  
  filtraArchivos(dirActual,listaArchivos,&numArchivos);

  actualizaInterfaz(x,listaArchivos,numArchivos);
  
  fin=0;
  do {
	minline = (x / CONSOLELINES) * CONSOLELINES;
  minline = (x / (CONSOLELINES + (doubleColumn * CONSOLELINES))) * (CONSOLELINES + (doubleColumn * CONSOLELINES));

	switch (leeDatos())
	  {
	  case 1:
		identif=*listaArchivos[x];
		if (identif == 'F')
		  {
		  /*Construimos la ruta al archivo y mandamos que se ejecute*/
		  sprintf (ruta,"%s/%s",dirActual,listaArchivos[x]+1);
		  ejecutaArchivo(ruta);
		  
  		  actualizaInterfaz(x,listaArchivos,numArchivos);
		  }
		else if (identif == 'D')
		  {
		  /*Mandamos a procesar el directorio*/
		  procesaDirectorio(dirActual,listaArchivos[x]+1);
		  x=0;
		  filtraArchivos(dirActual,listaArchivos,&numArchivos);
		  
		  actualizaInterfaz(x,listaArchivos,numArchivos);
		  }
		break;
      case 2: //Joystick Up
	    if (x > 0)
          --x;
        else if (scrollWrap & 1)//Scroll Wrap
          x=numArchivos-1;
		break;
      case 3: //Joystick Down
	    if (x < numArchivos-1)
          ++x;
        else if (scrollWrap & 1) //Scroll Wrap
          x=0;
		break;
      case 4: //A Button
		if (cmdLinesSel>0) --cmdLinesSel;
		break;
      case 5:  //Y Button
		if (cmdLinesSel<cmdLinesNum-1) ++cmdLinesSel;	    
		break;
	  /*Orden de salida de la app*/
	  case 6: //Start Button
		fin=1;
		break;
      case 7:  //L Button
	    if ( (minline - ( CONSOLELINES * doubleColumn) ) > CONSOLELINES - 1)
          x= minline - CONSOLELINES - ( CONSOLELINES * doubleColumn);
      else if (scrollWrap & 1) //Scroll Wrap
          x=numArchivos-1;
		break;
      case 8:  //R Button
	    if ( (minline + ( CONSOLELINES * doubleColumn) ) < 1 + numArchivos-CONSOLELINES)
          x = minline + CONSOLELINES + ( CONSOLELINES * doubleColumn);
        else if (scrollWrap & 1) //Scroll Wrap
          x=0;
		break;
	  case 9:  //Joystick Left
            if (x > CONSOLELINES - 1)
              {
  	      if (scrollWrap & 2) //Soporte para la peticion 1494023
                {
                x = x - CONSOLELINES;
                }
              else if (doubleColumn)
	        {
	        int paridad;
	      
	        paridad = (x/CONSOLELINES) % 2;
	        x = x - (CONSOLELINES * ( paridad ));
                }
              }
            else if (scrollWrap & 1)
              {
              x = numArchivos - 1;
              }
	    break;
	  case 10: //Joystick Right
	    if ( x  < 1 + numArchivos - CONSOLELINES)
	      {
              if (scrollWrap & 2) //Soporte para la peticion 1494023
  	        {
                x = x + CONSOLELINES;
                }
              else if (doubleColumn)
	        {
	        int paridad;
	        
	        paridad = 1 - (x/CONSOLELINES) % 2;
	        x = x + (CONSOLELINES * ( paridad ));
	        }
	      }
            else if (scrollWrap & 1)
              {
              x = 0;
              }
	    break;
    case 11: //Volume Down
      i=24;
      while ((i>0) && (frecuencias[i] > cpuSpeed))
        --i;
      
      if (i)
        --i;
      
      /* Reducimos la velocidad de CPU a usar */
      cpuSpeed=frecuencias[i];
      
      actualizaInterfaz(x,listaArchivos,numArchivos);
      break;
    case 12: //Volume Up
      i=24;
      while ((i>0) && (frecuencias[i] > cpuSpeed))
        --i;
      
      if (i<24)
        ++i;
                
      /* Aumentamos la velocidad de CPU a usar */
      cpuSpeed=frecuencias[i];
      
      actualizaInterfaz(x,listaArchivos,numArchivos);
      break;
    case 13: //Volume Down & Volume Up
      /* Restauramos la velocidad de CPU a la original */
      cpuSpeed = lfreq;
      
      actualizaInterfaz(x,listaArchivos,numArchivos);
      break;
    default:
        break;
	  }
	  
	if (!fin)
	  {
	  actualizaInterfaz(x,listaArchivos,numArchivos);
	  }
	  
	} while (!fin);

  printf("Interfaz Exit\n");
  return (0);
}

/*Obtiene el listado de los archivos en el directorio especificado, y aplica un filtro en base a la variable global fileFilter*/
/*generando un vector de cadenas de texto, donde cada elemento tiene un descriptor como primer caracter, D para directorio*/
/* y F para un archivo*/
int filtraArchivos(char *directorio, char *listaArchivos[], int *numArchivos)
{
  int i;
  int total;
  DIR *dirp;
  struct dirent *dit;

  /*Si ya se habia usado el vector, liberamos la memoria que utilizaba*/
  total=*numArchivos;
  if (total > 0)
    {
    for (i=0;i<total;i++)
	  {
      free(listaArchivos[i]);
	  }
    }

  printf (langStrings[7],directorio);
  
  //Si el directorio no existe, mensaje de error y fin de programa.
  if ((dirp=opendir(directorio)) == NULL)
    {
    sprintf (textoError, langStrings[8]);
	muestraError();
	#ifdef __GP2X__
      gp2x_setcpu();
	#endif
	exit(1);
    }
	
  total=0;
  while ((dit = readdir(dirp)) != NULL)
	{
	
    /*Si es un directorio, lo añadimos a la lista*/
    if (dit->d_type == DT_DIR)
	  {
	  if (strcmp(dit->d_name,"."))
	    {
	    listaArchivos[total]=(char*) malloc (sizeof(dit->d_name)+1);
	    sprintf(listaArchivos[total++],"D%s", dit->d_name);
	    }
	  //printf (langStrings[9],dit->d_name);
	  }
    else
	  {
	  if (fileFiltersNum>0)
	    {
	    char *fileExt;

		/*Obtenemos la extension del archivo*/
	    if ((fileExt = strrchr(dit->d_name, '.')))
		  {
	      /*Si hay extensiones para elegir, comparamos con la del archivo*/
	      for (i=0;i<fileFiltersNum;i++)
		    {
		    if (!strcasecmp (fileFilters[i],fileExt+1))
		      {
		      listaArchivos[total]=(char*) malloc (sizeof(dit->d_name)+1);
		      sprintf(listaArchivos[total++],"F%s", dit->d_name);
			  
			  //printf (langStrings[10],fileFilters[i],fileExt,dit->d_name);
		      }
		    }
		  }
		}
	  else
		{
		/*Si no hay extensiones para elegir, cogemos el archivo tal cual*/
		listaArchivos[total]=(char*) malloc (sizeof(dit->d_name)+1);
		sprintf(listaArchivos[total++],"F%s", dit->d_name);
		
        //printf (langStrings[11],dit->d_name);
		}
	  }
   }

  if (closedir(dirp) == -1)
	{
	sprintf (textoError, langStrings[12]);
	muestraError();
	#ifdef __GP2X__
      gp2x_setcpu();
	#endif
    exit(1);
    }
	
  *numArchivos=total;

  return(0); 
}


/* Funcion que lanza la linea de comandos actual añadiendo como parametro el archivo elegido en la ruta */
int ejecutaArchivo(char *ruta)
  {
  char dirEjec[MAXPATH];
  char *lArg[MAXDIRS];
  int nArg;
  char *punterito;
  
  int i;
    
  strcpy (dirEjec,cmdLines[cmdLinesSel]);
  
  punterito=strchr(dirEjec,' '); //Localizamos el primer espacio
  if (punterito != NULL) *punterito=0; //Partimos por el primer espacio si lo hubiera
  punterito=strrchr(dirEjec,'/'); //Localizamos la posicion del ultimo separador de directorio
  if (punterito != NULL) *punterito=0;
  
  nArg=0;
  
  lArg[nArg]=strtok(cmdLines[cmdLinesSel]," ");

  while ((lArg[++nArg]=strtok(NULL," ")))
    {
    printf ("lArg[%i]=%s\n",nArg,lArg[nArg]);
	}

  lArg[nArg]=ruta;
  lArg[++nArg]=NULL;
   
  
  /* Un metodo alternativo para lanzar otro proceso
  lArg[0]=(char*) malloc (MAXLEN);
  strcpy(lArg[0],"bash");
  lArg[1]=(char*) malloc (MAXLEN);
  strcpy(lArg[1],"-c");
  
  lArg[2]=(char*) malloc (MAXPATH);
  sprintf(lArg[2],"exec %s %s",cmdLines[cmdLinesSel],ruta);

  nArg=3;
  lArg[nArg]=NULL;
  */

  printf(langStrings[13],dirEjec);
  printf(langStrings[14]);
  
  for (i=0;i<nArg;i++) printf (" %s",lArg[i]);  
  
  printf("\n");
  
  #ifdef __GP2X__
    printf ("New CPU Speed: %i\n", cpuSpeed);
    set_speed_clock(cpuSpeed);
    gp2x_setcpu();
  #endif
  
  chdir(dirEjec);
  if (execvp(lArg[0],lArg))
    {
	//Si ha habido problemas
    #ifdef __GP2X__
	//Volvemos a inicializar la minimal lib
	gp2x_init(1000, 16, 44100,16,1,50,0); //minimal 0.B

	gp2x_getversion();
	set_speed_clock(CPU_SPEED);
    #endif
	
	//Como no hemos podido ejecutar lo que se ha pedido, avisamos con un mensaje de error
 	sprintf (textoError, langStrings[15]);
	muestraError();
	}

  return (1);
  }
  
/* Funcion que actualiza dirActual si es necesario, pasando de dirActual a dirActual/nombre. Si nombre es ".." */
/*entonces recorta el directorio siempre que no vaya por debajo de baseDir */
int procesaDirectorio(char *dirActual, char *nombre)
  {
  char nuevaRuta[MAXLEN];
  char *punteroBarra;	
  
  printf ("%s %s\n",dirActual,nombre);
  
  if (!strcmp(nombre,"."))
	{
	//No hacer nada, ya que vuelve al directorio actual
	}
  else if (!strcmp(nombre,".."))
	{
	//Subir un directorio, salvo si estamos en el directorio base
	if (strcmp(dirActual,baseDir))
	  {
	  punteroBarra=strrchr(dirActual,'/'); //Localizamos la última barra de la ruta
	  strncpy(nuevaRuta,dirActual,punteroBarra-dirActual); //Copiamos la ruta "recortada"
	  *(nuevaRuta + (punteroBarra - dirActual))=0; //Marcamos con 0 para indicar final de cadena
	  strcpy(dirActual,nuevaRuta); //Actualizamos el directorio actual con el nuevo valor
	  printf (langStrings[16], dirActual);
	  }
	}
  else
	{
	sprintf(nuevaRuta,"%s/%s",dirActual,nombre);
	strcpy(dirActual,nuevaRuta);
	printf (langStrings[16], dirActual);
	}
  
  return(0);
  }


/* Funciones para GP2X */
#ifdef __GP2X__

int muestraError(void)
  {
  int n,m;
  unsigned short col;
  char cad[MAXLEN];
  
  col=gp2x_video_RGB_color16(0xff,0,0); //minimal 0.B

  for (n=16;n<304;n++)
    {
    for(m=96;m<136;m++)
      {
      gp2x_video_RGB[0].screen16[n+(m*320)]=col; //minimal 0.B
      }
    }

  sprintf(cad,"*** ERROR ***");
  *(cad+35)=0; //Recortamos las lineas para que quede bien en pantalla
  v_putcad(10,13,COLOR_ERROR,cad);

  sprintf(cad,textoError);
  *(cad+35)=0; //Recortamos las lineas para que quede bien en pantalla
  v_putcad(3,15,COLOR_ERROR,cad);
  
  gp2x_video_waitvsync();gp2x_video_RGB_flip(0); //minimal 0.B

  fprintf (stderr, textoError);
  
  leeDatos();
  
  return (0);
  }


/* Lee la entrada de los controles de la GP2X*/
int leeDatos(void)
  {
  unsigned teclasGP2X;
  unsigned long tick;
  

  tick=gp2x_timer_read();
  while (gp2x_timer_read()-tick < 80); //Esperar varios msec para que la interfaz no sea muy rápida


  teclasGP2X=0;
  while (1)
    {
	teclasGP2X=gp2x_joystick_read();

    if((teclasGP2X & GP2X_VOL_UP) && (teclasGP2X & GP2X_VOL_DOWN))
	  {
      return (13);
	  }
    
    if((teclasGP2X & GP2X_VOL_UP))
	  {
      return (12);
	  }
    
    if((teclasGP2X & GP2X_VOL_DOWN))
	  {
      return (11);
	  }
    
    if((teclasGP2X & GP2X_RIGHT))
	  {
      return (10);
	  }
    
    if((teclasGP2X & GP2X_LEFT))
	  {
	  return (9);
	  }
	
	if((teclasGP2X & GP2X_R))
	  {
	  return (8);
	  }

	if((teclasGP2X & GP2X_L))
	  {
	  return (7);
	  }

	if((teclasGP2X & GP2X_START))
	  {
	  return (6);
	  }

	if((teclasGP2X & GP2X_Y))
	  {
	  return (5);
	  }

	if((teclasGP2X & GP2X_A))
	  {
	  return (4);
	  }

    if((teclasGP2X & GP2X_DOWN))
	  {
	  return (3);
	  }

    if((teclasGP2X & GP2X_UP))
	  {
	  return (2);
	  }

    if((teclasGP2X & GP2X_B) || (teclasGP2X & GP2X_X) || (teclasGP2X & GP2X_SELECT) || (teclasGP2X & GP2X_PUSH))
	  {
	  return (1);
	  }
	}

  return (0);
}


int actualizaInterfaz(int coordenada,char *listaArchivos[],int numArchivos)
  {
  char cad[MAXLEN];
  int linelen;
  char *puntero1, *puntero2;
  

  /*Borramos todo*/
  ClearScreen(COLORFONDO);
  
  /*Pintamos la imagen de fondo (si la hay)*/
  pintaskin();

  /*Definimos la longitud maxima de linea (esto depende de la fuente utilizada)*/
  linelen=51;
  
  if (!layoutCode)
    {
    sprintf(cad," ** Selector v"__VERSION_PROG__" ** (c) 2005-2006 Kounch");
    *(cad+linelen)=0; //Recortamos las lineas para que quede bien en pantalla
	v_putcad(1,1,COLOR_TITULO,cad);
    }

  strcpy (cad, cmdLines[cmdLinesSel]);
  
  puntero1=strchr(cad,' '); //Localizamos el primer espacio
  if (puntero1 != NULL) *puntero1=0; //Partimos por el primer espacio si lo hubiera
  
  puntero2=strrchr(cad,'/'); //Localizamos la posicion del ultimo separador de directorio
  if (puntero2 == NULL) puntero2=cad; //Si no habia separador de directorio, tomamos el comienzo de la cadena

  if (puntero1 != NULL) *puntero1=' '; //Volvemos a poner el primer espacio si lo hubiera
    
  *(puntero2+linelen)=0; //Recortamos las lineas para que quede bien en pantalla
  
  if (!layoutCode)
    {
	v_putcad(1,4,COLOR_COMANDO,puntero2+1);
	}
  else if (layoutCode == 1)
    {
	v_putcad(1,28,COLOR_COMANDO,puntero2+1);
    }
  
  if (doubleColumn)
    {
    pintaColumnas(1, coordenada, listaArchivos, numArchivos, linelen/2);
    pintaColumnas(2+linelen/2, coordenada, listaArchivos, numArchivos, linelen/2);
    }
  else
	pintaColumnas(1, coordenada, listaArchivos, numArchivos, linelen);
  
  sprintf(cad,"Firmware Version: %s CPU Speed: %i",firmVersion,cpuSpeed);
  *(cad+linelen)=0; //Recortamos las lineas para que quede bien en pantalla
  v_putcad(1,26,COLOR_COMANDO,cad);

  if (!layoutCode)
    {
    sprintf(cad,langStrings[17]);
    *(cad+linelen)=0; //Recortamos las lineas para que quede bien en pantalla
    v_putcad(1,28,COLOR_AYUDA,cad);
	}

  gp2x_video_waitvsync();gp2x_video_RGB_flip(0); //minimal 0.B

  return (0);
}


int pintaColumnas(int ordenada, int coordenada,char *listaArchivos[],int numArchivos, int linelen) 
{
 char cad[MAXLEN];
 int minline;
 int maxline;
 int i;
 int fila;
 unsigned colorLetra;


  minline = (coordenada / (CONSOLELINES + (doubleColumn * CONSOLELINES))) * (CONSOLELINES + (doubleColumn * CONSOLELINES));
  maxline = minline + CONSOLELINES;

  if (ordenada > 1)
    {
    minline = minline + CONSOLELINES;
    maxline = maxline + CONSOLELINES;
    }
  
  if (minline > numArchivos) return 0;  
  if (maxline > numArchivos) maxline=numArchivos;
  
  for (i=minline; i<maxline;i++)
    {
    char nombrembyte[MAXLEN];
	/*
    iconv_t cd;
    size_t longitud;
    char *inptr;
    char *outptr;
	*/
    
    //Convertimos los datos del nombre en UTF-8 a ISO-8859-1 para imprimirlos bien en pantalla
	/*
    cd = iconv_open ("ISO-8859-1", "UTF-8");
    longitud=MAXLEN-1;
    inptr=listaArchivos[i];
    outptr=nombrembyte;
    iconv (cd, &inptr, &longitud, &outptr, &longitud);
    iconv_close (cd);
	*/
	strcpy(nombrembyte, listaArchivos[i]);
    
    if (*listaArchivos[i]=='D')
	  {
      //sprintf (cad,"<%s>",listaArchivos[i]+1);
      sprintf (cad,"<%s>",nombrembyte+1);	  
	  }
    else
	  {
      //sprintf (cad,"%s",listaArchivos[i]+1);
      sprintf (cad,"%s",nombrembyte+1);
	  }
        
    fila=(i-minline) * 2 + 7;
	  
    if ((i) == coordenada )
      {
      int m,n;
      
      colorLetra=COLOR_SELECCION;
      
      if (selectRectangle & 1)
        {
        //Dibujamos el rectangulo
        for (n=(ordenada*6)-1;n<(ordenada+linelen)*6+3;n++)
          {
          for(m=(fila)*8-2;m<(fila+1)*8+2;m++)
            {
            gp2x_video_RGB[0].screen16[n+(m*320)]=gp2x_video_RGB_color16(0,0xA0,0); //minimal 0.B
            }
          }
        }
      
      if (selectRectangle & 2) //Soporte para la peticion 1496339
      {
        if (strlen(cad)>(linelen-3))
          *(cad+linelen-3)=0; //Recortamos las lineas para que quede bien en pantalla

        strcpy((cad+strlen(cad)),"<--"); 
      }      
      
      }
    else
      {
       colorLetra=COLOR_LISTADO;
      }
    
    /*Recortamos la l’nea para que quepa*/
    if (strlen(cad) > linelen)
      strcpy((cad+linelen-3),"...");    
                
    *(cad+linelen)=0; //Recortamos las lineas para que quede bien en pantalla
        
    v_putcad(ordenada,fila,colorLetra,cad);
	}
  
  return 0;
}


/* Para obtener la version de firmware, y el estado de la CPU*/
int gp2x_getversion(void)
  {
  FILE *fp;
  char str[MAXLEN];


  fp = fopen("/usr/gp2x/version", "rt");
  if(fp == NULL)
  {
	sprintf (textoError, langStrings[18]);
	muestraError();
    return (1);
  }
  fgets(str,MAXLEN,fp);
  fclose(fp);
  str[5]=0;
  
  strcpy(firmVersion,str);
  
  if (!strcmp(firmVersion,"1.0"))
    cpuver=100;
  else
	cpuver=101;
  
  return 0;
  }
  
/* Para dejar todo como estaba*/
int gp2x_setcpu(void)
  {
  //Borrado de pantalla
  ClearScreen(0);
  gp2x_video_waitvsync();gp2x_video_RGB_flip(0);
  ClearScreen(0);
  gp2x_video_waitvsync();gp2x_video_RGB_flip(0);

  //Recuperamos registros del sistema

  //Cerramos lo relativo a la minimal lib
  //¡Ojo! esta version de gp2x_deinit() debe estar modificada para que no llame al menu de GP2X
  gp2x_deinit();
  
  return 0;
  }


/* se usa para 'borrar' la pantalla virtual con un color */
void ClearScreen(int col)
{
int n;
//unsigned char *c;
//unsigned short col;
//c=&val;
//col=gp2x_video_RGB_color16(c[0],c[1],c[2]); //minimal 0.B

for(n=0;n<320*240;n++)
	{
    gp2x_video_RGB[0].screen16[n]=col; //minimal 0.B
	}
}

/*Carga la imagen en memoria*/
int cargaskin(void)
  {
  ulg image_width, image_height, image_rowbytes;
  int image_channels;
  uch *image_data;
  char archivo[MAXPATH];
  FILE *fp;

  int error = 0;
  int rc;
  double LUT_exponent;               /* just the lookup table */
  double CRT_exponent = 2.2;         /* just the monitor */
  double display_exponent;   /* whole display system */
  
  LUT_exponent = 1.0;   /* assume no LUT:  most PCs */
  display_exponent = LUT_exponent * CRT_exponent;
  
  sprintf (archivo,"%s/fondo.png",skinPath);
  fp = fopen(archivo, "rt");
  if(fp == NULL)
    {
	printf ("El archivo: %s no existe",archivo);
	return (1);
	}
	  
  if ((rc = readpng_init(fp, &image_width, &image_height)) != 0)
    {
    switch (rc)
	  {
      case 1:
        printf("[%s] is not a PNG file: incorrect signature\n", archivo);
        break;
      case 2:
	    printf("[%s] has bad IHDR (libpng longjmp)\n", archivo);
        break;
      case 4:
        printf("insufficient memory\n");
        break;
      default:
        printf("unknown readpng_init() error\n");
        break;
      }
	++error;
	
    }
  else
    {
	/* Leer los datos de la imagen*/
	if ((image_width != 320) || (image_height != 240))
	  {
	  printf ("wrong image size; should be 320x240\n");
	  image_data=0;
	  }
	else
	  {
      image_data = readpng_get_image(display_exponent, &image_channels, &image_rowbytes);
      /*Liberar memoria sin perder los datos de la imagen*/
      readpng_cleanup(FALSE);
	  }
	
    if (!image_data)
	  {
      printf("unable to decode PNG image\n");
	  error++;
      }
	else
	  {
      int red, green, blue;
	  ulg i, row;
      uch *src;
      uch r, g, b, a;
	  uch bg_red=0, bg_green=0, bg_blue=0;
	  unsigned short* dest;

      for (row = 0;  row < image_height;  ++row)
	    {
        src = image_data + row*image_rowbytes;
        dest = fondo + row*320;
        if (image_channels == 3)
		  {
          for (i = image_width;  i > 0;  --i)
		    {
            red   = *src++;
            green = *src++;
            blue  = *src++;
			
		*dest++ = gp2x_video_RGB_color16(red, green, blue); //minimal 0.B
			}
		  }
		else /* if (image_channels == 4) */
		  {
		  for (i = image_width;  i > 0;  --i)
		    {
		    r = *src++;
		    g = *src++;
		    b = *src++;
		    a = *src++;
		    if (a == 255)
			  {
		      red   = r;
		      green = g;
		      blue  = b;
		      }
			else if (a == 0)
			  {
		      red   = bg_red;
		      green = bg_green;
		      blue  = bg_blue;
		      }
			else
			  {
		      /* this macro (from png.h) composites the foreground
		      * and background values and puts the result into the
		      * first argument */
		      alpha_composite(red,   r, a, bg_red);
		      alpha_composite(green, g, a, bg_green);
		      alpha_composite(blue,  b, a, bg_blue);
		      }
			*dest++ = gp2x_video_RGB_color16(red, green, blue); //minimal 0.B
		    }
		  }
	    }
	  free(image_data);
      }
	}

  fclose(fp);
	
  if (error)
    {
	readpng_cleanup(TRUE);
	}	
	
  return (error);
  }


/* pinta la imagen en la pantalla */
int pintaskin(void)
{
	int n,m;
	unsigned short *col;

	if (skinPath[0] != 0)
	{
		for(m=0;m<240;m++)
		{
			col=fondo + m*320;
			for (n=0;n<320;n++)
			{
				gp2x_video_RGB[0].screen16[n+(m*320)]=*col++; //minimal 0.B
			}
		}
	}

	return (0);
}


void v_putcad(int x,int y,unsigned color,char *cad)  // dibuja una cadena de texto
{	
	//while(cad[0]!=0) {v_putchar(x,y,color,cad[0]);cad++;x++;}
	gp2x_fontcolor_set(NULL, (int) color);
	gp2x_printf(NULL,x*6,y*8,cad);
}

#endif
