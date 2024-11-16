/*       
 *	search:
 *      =============================================================
 *	"header"		header del programa
 *	"ejemplos"		ejemplos de uso (lineas de comando )
 *	"documentacion" 	alguna documentacion
 *	"ojo"			cosas a tener en cuenta !!!!!
 *	"programa"		programa
 *
 *
 */



/*
 *	//header//
 *
 *	sim_gmap01.c
 *	
 *
 *	2020_07_26
 *
 *
 *	Generador de datos para simular dataset
 *
 * 	Opciones:
 * 	-h   		forma de uso
 *	
 *	-v		verbose
 *
 *
 *	Changes pending...
 *	
 *	genera_transaccion
 *	- directamente poner el nro_per al principio ....
 *	  ( y asi no hay que correr el paso -pruebas=8 para cambiar de lugar nro trx con nro per )
 *
 *	
 *
 *
 *
 */



/*
 *	//ejemplos//
 *
 * 	Ejemplos de uso:
 *
 *	1) Mostrar uso
 *
 *	sim_gmap01
 *	sim_gmap01 -h
 *
 *
 *	2) Opciones de debug
 *
 *	sim_gmap01 -v                     activa debug, nivel full ... equivalente a d5 
 *	sim_gmap01 -v -opciones=Ln        letra L (i informativo d debug e no_me_acuerdo)  
 *	                                  Nro n nivel 1 a 5 ... 5 full
 *
 *	3) Cargar un mapa (verifica formato)
 *
 *	sim_gmap01 -pmp=mapa01.txt
 *	sim_gmap01 -pmp=mapa01.txt -v -opciones=d5
 *
 *	
 *	4) Cargar un mapa y un archivo con definiciones de comercios
 *
 *	sim_gmap01 -pmp=mapa01.txt -cde=defcom01.txt
 *
 *	
 *	5) Cargar un mapa y un archivo con def de comercions, y generar base de comercios
 *
 *	sim_gmap01 -pmp=mapa01.txt -cde=defcom01.txt -gco=comercio01.txt
 *
 *
 *	6) Generar un archivo de rubros
 *
 *	sim_gmap01 -gru=rubro01.txt
 *	sim_gmap01 -gru=rubro01.txt -v 
 *	sim_gmap01 -gru=rubro01.txt -v -opciones=d5
 *
 *	7) Cargar un archivo de rubros
 *
 *	sim_gmap01 -rub=rubro01.txt 
 *	sim_gmap01 -rub=rubro01.txt -v
 *	sim_gmap01 -rub=rubro01.txt -v -opciones=d5
 *
 *
 *	8) Generar un archivo de definicion de comercios (defcom01.txt)
 *
 *	sim_gmap01 -rub=rubro01.txt -gdc=defcom01.txt -v -opciones=d5
 *
 *  
 *	9) Cargar una tabla de factor de multiplicacion de comercios
 *
 *	sim_gmap01 -tb2=tabla02.txt -v -opciones=d5
 *
 *	
 *	X) Cargar un mapa y un archio de comercios
 *
 *	sim_gmap01 -pmp=mapa01.txt -com=comercio01.txt
 *
 */


/*
 *	//documentacion//
 *
 *	Como generar numeros al azar (ojo, inicializar con srand ) 
 *
 *	numero = rand () % (N-M+1) + M;   // Este está entre M y N
 *
 *	maximos para esta version
 *
 *	el mapa es de 10 x 10 zonas
 *	maximo 4096 comercios x zona
 *	maximo 100 tipos de comercios diferentes
 *	maximo 40 comercios de un tipo en cada zona 
 *	 ( 100 tipos x 40 comercios de ese tipo = 4000 < 4096 )
 *
 *
 *	Como es el proceso para ir generando todo esto ???
 *	
 *	paso 001
 *	Generar archivo de mapa (mapa01.txt)
 *	Este archivo se generar a mano con editor ...
 *	Las lineas son del tipo:
 *
 *	0,0,codearea_00,XX,XX,4096
 *
 *	Donde ... 
 *	0		es posicion x de la zona/area en el mapa
 *	0		es posicion y de la zona/area en el mapa
 *	codearea_00	es el codigo de area (0,0 en este ej)
 *	XX		Dx , Densidad X= Nula Rural Baja Media Alta
 *	XX		Nx , Nivel socio economico X = Nulo Bajo Medio Alto
 *	4096		Numero de comercios que tiene esa zona 
 *			(pusimos todos en 4096, y un campo 'valido' 0 no, 1 si
 *			para mayor facilidad)
 *
 *	paso 002
 *	Generar archivo de rubros (rubro01.txt)
 *	Este archivo se genera con
 *	sim_gmap01 -gru=rubro01.txt 
 *	y luego hay que editarlo a mano, completando el
 *	campo 'descripcion' (si se quiere)
 *	las lineas son del tipo:
 *
 *	0,rubro_0000,descripcion
 *	
 *	Donde:
 *	0		es numero de rubro
 *	rubro_0000	nombre del rubro
 *	descripcion	descripcion del rubro
 *
 *
 *
 *	Recordar ... generar numeros al azar 
 *	numero = rand () % (N-M+1) + M;   // Este está entre M y N
 *	
 *
 */


/*
 * 	nivel de verbose
 *
 * 	-v	activa verbose (por defecto, se pone en 'd5' )
 * 	-v -opciones=XX
 *
 * 		XX puede ser d0 d1 d2 d3 d4 
 *
 *
 * 	d0	no imprime nada
 *
 * 	d1	muestra lo minimo indispensable, 
 * 		- comienzo y finalizacion de procesos
 * 		- resultados generales
 * 		- estadisticas
 *
 *	d2,d3	distintos niveles de debug / informacion
 *
 *	d4	imprime todo
 *	
 *	d5	especial para funciones gp
 *
 */




/*
 *	errores
 *
 *	101	no encontro archivo de input 
 *	102	no pudo abrir archivo de output para nuevo corpus
 *	103	no encontro archivo de diccionario
 *	104	no encontro archivo de marcas
 *	105	no encontro archivo de gramatica
 *	106	no pudo abrir archivo de output para diferencia contra diccionario
 *	107	no pudo abrir archivo de parametros de mapa
 *	108	no pudo abrir archivo de definicion de comercios
 *	109	no pudo abrir archivo de output de comercios
 *	110	no pudo abrir archivo de input de comercios
 *	111	no pudo abrir archivo de output de rubros
 *	112	no pudo abrir archivo de input de rubros
 *	113	no pudo abrir archivo de output de definicion de rubros
 *	114	no pudo abrir archivo de input tabla de factor multiplicacion comercios
 *	115	no pudo abrir archivo de input tabla de factor multiplicacion defcom
 *	116	no pudo abrir archivo de input de test
 *	117	no pudo abrir archivo de input de perfiles
 *	118	no pudo abrir archivo de outpt de definicion de perfiles 
 *	119	no pudo abrir archivo de input tabla de perfiles
 *	120	no pudo abrir archivo de output de personas
 *	121	no pudo abrir archivo de input de distribucion de perfiles x zona
 *	122	no pudo abrir archivo de output de transacciones 
 *	123	no pudo abrir archivo de input de personas
 *	124	no pudo abrir archivo de input de transacciones 
 *	125	no pudo abrir archivo de output de transacciones truchas
 *	126	no pudo abrir archivo de input  de transacciones estadisticas 
 *	127	no pudo abrir archivo de output de weka
 *	128	no pudo abrir archivo de input  para fix a
 *	129	no pudo abrir archivo de output aux
 *	130	no pudo abrir archivo de output log
 *	131	no pudo abrir transacciones r+
 *
 *
 * 	301	overflow cantidad de marcas 
 *	302	overflow cantidad de elementos en mapa
 *	303	overflow cantidad de definicion de comercio
 *	304	overflow cantidad de rubros
 *	305	overflow cantidad de lineas para tab de fact de multip de comercios
 *	306	overflow cantidad de lineas para tab de fact de multip de def com
 *	307	overflow cantidad de lineas de comercio
 *	308	overflow cantidad de lineas en perfiles
 *	309	overflow cantidad de lineas para tab de param de perfiles
 *	310	overflow cantidad de tipos de perfiles
 *	311	overflow cantidad de personas en genera_personas
 *	312	overflow cantidad de personas en carga de personas 
 *	313	overflow cantidad de transacciones en carga (input)
 *	314	overflow cantidad de registros fix a (input)
 *	315	overflow cantidad de registros en mat de trx
 *
 * 	501	overflow cantidad de tokens parseados en linea
 *
 *	601	no se encontro elemento buscado en genera personas
 *	602	cantidad de comercios en zona supera MAX_ZCOA
 *	603	error en cantidad de trx fra en prueba6
 *	604	error en cantidad de trx fra en weka
 *	605	error en pruebas8
 *	606	cambios en call_algoritmo !!! - parametro q
 *
 * 	701	error separando segmento 1 (no encontro marca de sep )
 * 	702	error separando segmento 2
 * 	703	error separando segmento 3
 *
 *	801	error en codigo de densidad en genera defcom
 *	802	es necesario cargar mapa para el proceso                                  
 *	803	es necesario cargar definicion de comercios para el proceso
 *	804	es necesario cargar tabla2 factor de multiplicacion para comercios
 *	805	es necesario cargar rubros para el proceso
 *	806	es necesario cargar tabla1 factor de multiplicacion para def comercios
 *	807	es necesario cargar tabla4 tipos de perfiles x zona
 *	808	es necesario cargar tabla perf0N perfiles de tarjetashab para el proceso -per
 *	809	es necesario cargar tabla de personas para el proceso ... -cpe
 *	810	es necesario cargar tabla de comercios para el proceso ... -com
 *	811	es necesario un archivo de input para el proceso ... -inp
 *	812	es necesario un archivo de trx de input para el proceso ... -sta 
 *	813	es necesario un archivo de trx de input para el proceso ... -inp 
 *	814	es necesario un archivo det output  para el proceso ... -out 
 *	815	es necesario que esten cargadas personas para el proceso !! -cpe
 *	
 *	901	error de malloc
 *	902	error de malloc en genera_personas
 *	903	error de malloc en genera_transacciones
 *	904	error de malloc en load transacciones
 *	905	error de malloc en load fix a
 *	906	error de malloc en proceso de prueba 6
 *	907	error de malloc en proceso de prueba 3 (pipeline)
 *	908	error de malloc en proceso exec 1 (new pipeline)
 *	909	error de malloc en obj_ptr s5
 *	910	error de malloc en load personas
 *	911	error de malloc trx en proceso1
 *
 *	999	error debug
 */


/*
 *	//programa//
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define	DEBUG 0

#define	MAXD	16	/* buffer de juguete */
#define	MAXV	64	/* buffer chico */
#define	MAXR	128	/* buffer razonable */
#define	MAXB	1024	/* buffer grande */
#define	MAXT	200 	/* maximo de tokens en tabla */
#define	MAXF	64	/* largo de archivos de input - output - marcas etc */
#define	MAXP	128	/* maximo de palabras en general */

/*
 *
 *	Funciones, variables y estructuras para manejor de parametros globales
 *
 */

#define	GP_SET 1
#define	GP_GET 0

int	gp_default();
int	gp_init(int ,char **);
int	gp_test();
int	gp_print();
int	gp_parser();
int	gp_fq(int,int);
int	gp_fverbose(char *);
char	*gp_tm();
char	*gp_fp(int,int,char **);

int	gp_q_partype1=0;	/* parametero tipo "name" */
int	gp_q_partype2=0;	/* parametero tipo "-something" */
int	gp_q_partype3=0;	/* parametero tipo "-someoption=somename" */


int	gp_verbose=0;		/* verbose 0 no 1 si */
int	gp_help=0;		/* help 0 no 1 si */
int	gp_diccionario=0;	/* diccionario 0 no 1 si */
int	gp_minusculas=0;	/* output en minuscula 0 no 1 si */
int	gp_fsentencia=0;	/* archivo de salida en formato sentencias 0 no 1 si */
int	gp_eol=0;		/* fuerzo string EOL al final de la linea ... a veces hay . en medio de la linea */
int	gp_niveldes=0;		/* nivel de descripcion que se vuelca en archivo de salida (solo con fsentencia = 0 ) */
				/* 0 normal 1 sentencia y numero de token 2 .... agrego cosas del diccionario si se usa */
int	gp_tabmrk=0;		/* usar archivo de marcas 0 no 1 si */
int	gp_detneg=0;		/* detectar negacion */
int	gp_tipneg=0;		/* tipo de negacion */

int	gp_tpar[3];		/* vector con cantidad de parametros de cada tipo */

char	gp_opciones[MAXV];	/* opciones adicionales, informadas de la forma -opciones=[letras]  */
char	gp_pruebas[MAXV];	/* pruebas ...    -prueba=[nro] ej:  -pruebas=1 */
char	gp_exec[MAXV];		/* execs ...      -exec=[rnp]   ej:  -exec=1    */
char	gp_proc[MAXV];		/* procs ...      -proc=[rnp]   ej:  -proc=1    */
char	gp_fyh[MAXR];		/* fecha y hora */


int	gp_reidx;		/* re indexar archivo de transacciones */
int	gp_pause;		/* pausa al mostrar ... */

/*
 *
 *	Funciones, variables y estructuras globales en el programa
 *
 */

int	abro_files();
int	cierro_files();
int	mostrar_cargas();
int	mostrar_reportes();
int	genera_comercios();
int	genera_personas();
int	genera_transacciones();
int	genera_fraudes();
int	genera_rubros();
int	genera_defcom();
int	genera_defper();
int	densidad();
int	nivelsocioeconomico();
int	cantidad_n1();

int	build_idx1(FILE *);


int	gna(int,int);
char	*gnf(int);			/* genera fecha al azar */
char	*gnh(int,int);			/* genera hora al azar */
char	*gsf(char *);			/* string para imprimir fecha */
char	*gsh(char *);			/* string para imprimir hora */
int	determina_elegibles();
int	determina_vector_elegibles();
int	cambio_persona(int);
int	calcular_atrib_trx(int);


int	bpmp_print();
int	btb1_print();
int	btb2_print();
int	btb3_print();
int	btb4_print();


char	*desde_igual(char *s);
char	*pasar_a_minusc(char *s);
char	*df(char *);
char	*espero_enter(int);
int	clear_screen();


int	tiene_igual(char *);
int	linea_vacia(char *);
int	tipo_char(char );
int	es_numero(char);
int	es_num_tk(char *);
int	es_word(char *);
int	es_puntuacion(char *);
int	char_demed(char);
int	mostrar(int,int,char *);
int	separar_seg(char *);
int	error(int);
int	uso(int);
int	m_estado();

int	proceso_principal();
int	proceso_prueba();
int	proceso_stat();
int	proceso_weka();
int	proceso_fxa();
int	nnparse();
int	nxt_tkn(int);

int	pro_pruebas1();
int	pro_pruebas2();
int	pro_pruebas3();
int	pro_pruebas4();
int	pro_pruebas5();
int	pro_pruebas6();
int	pro_pruebas7();
int	pro_pruebas8();
int	pro_pruebas9();


int	pro_exec1();
int	pro_exec2();
int	pro_exec3();
int	pro_exec4();
int	pro_exec5();
int	pro_exec6();
int	pro_exec7();
int	pro_exec8();
int	pro_exec9();

int	pro_proceso1();
int	pro_proceso2();
int	pro_proceso3();
int	pro_proceso4();
int	pro_proceso5();
int	pro_proceso6();
int	pro_proceso7();
int	pro_proceso8();
int	pro_proceso9();


int	re_indexa_trx();

int	calculo_at1(int,int);
int	calculo_at2(int,int);
int	calculo_at3(int,int);
int	calculo_at4(int,int);
int	calculo_at5(int,int);
int	calculo_at6(int,int);
int	calculo_at7(int,int);
int	calculo_at8(int,int);
int	calculo_at9(int,int);

int	calculo_trx_at1(int,int);
int	calculo_trx_at2(int,int);
int	calculo_trx_at3(int,int);
int	calculo_trx_at4(int,int);
int	calculo_trx_at5(int,int);
int	calculo_trx_at6(int,int);
int	calculo_trx_at7(int,int);
int	calculo_trx_at8(int,int);
int	calculo_trx_at9(int,int);


#define	TC_EOL	0
#define	TC_BLA	1
#define	TC_CCE	2	/* algunos caracteres especiales  ',;:/' */
#define	TC_PNT	3	/* punto '.' */
#define	TC_PAA	4	/* parent '([{' */
#define TC_PAC	5	/* parent ')]}' */
#define	TC_LET	6	/* letras */
#define	TC_NUM	7	/* numero */
#define	TC_CVR	8	/* caracteres varios ' % */
#define	TC_GBJ	9	/* guion bajo '_' */
#define	TC_RST	99


char	finp[MAXF];
char	fout[MAXF];
char	fou2[MAXF];
char	fdic[MAXF];
char	fmrk[MAXF];
char	fgrm[MAXF];

char	faux[MAXF];	/* archivo aux para salidas segun necesidad */
char	flog[MAXF];	/* archivo aux para salidas segun necesidad */

char	ftst[MAXF];	/* archio de testing */

char	ftb1[MAXF];	/* archivo tabla1 ... factor de multiplicacion de definicon de comercios  */
char	ftb2[MAXF];	/* archivo tabla2 ... factor de multiplicacion de comercios */
char	ftb3[MAXF];	/* archivo tabla3 ... factor de multiplicacion de perfiles */
char	ftb4[MAXF];	/* archivo tabla4 ... distribucion de perfiles x zona  */

char	fpmp[MAXF];	/* archivo con paramtros de mapa */
char	fcde[MAXF];	/* archivo de definicion de comercios (input) */
char	fgco[MAXF];	/* archivo de comercios (output) */
char	fcom[MAXF];	/* archivo de comercios (input) */
char	fgru[MAXF];	/* archivo de rubros (output) */
char	frub[MAXF];	/* archivo de rubros (input) */
char	fgdc[MAXF];	/* archivo de definicion de comercios (output) */
char	fper[MAXF];	/* archivo de perfiles de cardholders */
char	fgdp[MAXF];	/* archivo de definicion de perfiles de cardh x zona  */
char	fgpe[MAXF];	/* archivo de de personas */
char	fcpe[MAXF];	/* archivo de de personas */
char	fgtx[MAXF];	/* archivo de transacciones */
char	ftrx[MAXF];	/* archivo de transacciones (input) */
char	ftru[MAXF];	/* archivo de transacciones truchas (output) */
char	fsta[MAXF];	/* estadisticas sobre archivo de input (transacciones) */
char	fwek[MAXF];	/* archivo para weka (output ) */
char	ffxa[MAXF];	/* archivo para fix a  (input ) */


int	ffinp;
int	ffout;
int	ffou2;
int	ffdic;
int	ffmrk;
int	ffgrm;

int	ffaux;		/* archivo aux para output segun necesidad */
int	fflog;		/* archivo log para output segun necesidad */

int	fftst;
int	ffprb;		/* utilizo -prueba=1 ... 2 .. 3 etc */
int	ffexc;		/* utilizo -exec=1 .. 2  .. 3 etc   */
int	ffpro;		/* utilizo -proceso=1 .. 2 .. 3 etc */

int	fftb1;
int	fftb2;
int	fftb3;
int	fftb4;

int	ffpmp;		/* input  - mapa                           */

int	ffgru;		/* output - rubro                          */
int	ffrub;		/* input  - rubro                          */

int	ffgdc;		/* output - definicion de comercio         */
int	ffcde;		/* input  - definicion de comercios        */

int	ffgco;		/* output - comercio                       */
int	ffcom;		/* input  - comercio                       */

int	ffgdp;		/* output - perfil de personas x zona      */
int	ffper;		/* input  - perfil de personas             */

int	ffgpe;		/* output - personas                       */
int	ffcpe;		/* input  - personas                       */

int	ffgtx;		/* output - transacciones                  */ 
int	fftrx;		/* input  - transacciones                  */ 
int	fftru;		/* output - transacciones truchas          */ 

int	ffsta;		/* input  - trx completo - genera estad !  */ 

int	ffwek;		/* output - archivo para weka              */ 
int	fffxa;		/* input  - archivo que sale de proceso -pruebas=5    */ 
			/* 
			 * fxa - fix 'a'
			 * sirve para buscar si las transacciones truchas
			 * generadas , justo de casualidad, tienen una 
			 * trx que cae en el medio de la secuencia de truchas !!!!
			 */

FILE	*hfinp;
FILE	*hfout;
FILE	*hfou2;
FILE	*hfdic;
FILE	*hfmrk;
FILE	*hfgrm;

FILE	*hfaux;
FILE	*hflog;

FILE	*hftst;

FILE	*hftb1;
FILE	*hftb2;
FILE	*hftb3;
FILE	*hftb4;

FILE	*hfpmp;
FILE	*hfcde;
FILE	*hfgco;
FILE	*hfcom;
FILE	*hfgru;
FILE	*hfrub;
FILE	*hfgdc;
FILE	*hfper;
FILE	*hfgdp;
FILE	*hfgpe;
FILE	*hfcpe;
FILE	*hfgtx;
FILE	*hftrx;
FILE	*hftru;
FILE	*hfsta;
FILE	*hfwek;
FILE	*hffxa;


int	flag_caracteres;
int	flag_before_exit;


/*
 *	Estructuras y variables para tokens de las lineas procesadas
 *
 */

int	q_tk;
char	tk[MAXT][MAXB];






/*	Estructuras y variables para la base de marcas */

#define	MAXK	64	/* cantidad de marcas */
#define	MAXM	20	/* tamano maximo de palabra / marca */

int	bm_load();
char	*bm_tag(char *);


char	bm_key[MAXK][MAXM];
char	bm_mrk[MAXK][MAXM];

int	q_bm;		/* cantidad de marcas en base */




/* Estructuras para guardar segmentos de la linea
 * seg 0   nro registro
 * seg 1   aff / neg / pos 
 * seg 2   palabra de referencia
 * seg 3   frase 
 *
 */

char	segmento[4][MAXB];
char	marcas[3][10] = { " t1 ", " t2 ", " t3 " };
int	tipo_marca=0;

/*
 *	Estructuras y variables para generar numeros al azar 
 */

int	azar0200(int);
int	rn200[3][200];



#if 0
int	azar070();
int	azar010();
int	azar020();

int	rn070[70];	/* lista de 70 numeros ordenados al azar */
int	rn010[10];	/* lista de 10 numeros ordenados al azar */
int	rn020[20];	/* lista de 20 numeros ordenados al azar */

int	ofs070 = 0;	/* offset 70 */
int	ofs010 = 70;	/* offset 10 */
int	ofs020 = 80;	/* offset 20 */
#endif


/*
 *
 *	Estructuras y variables para diccionario de palabras
 *
 *
 */



int	bd_load();			/* funcion de carga de diccionario */
int	bd_buscar(char *);		/* funcion para buscar una palabra en dic */

int	q_wrd;				/* cantidad de palabras en diccionario */

typedef	struct	tnode	*nodeptr;
typedef	struct	tnode
{	char	*wrd;
	int	num;
	struct	tnode	*nx;
}	node;

nodeptr	npx,np1,*npa;



/*
 *
 *	Estructuras y variables para la base de gramatica nga 
 *
 */


int	bg_load();

int	q_grm;				/* cantidad de reglas en la gramatica nga */

typedef	struct	tregla	*reglaptr;
typedef	struct	tregla
{	char	*wrd;			/* puntero a la palabra */
	char	nm[4];			/* para identificar la regla */
	int	tip;			/* tipo de matching ... 1 exacto 2 parcial */
	int	num;			/* si es OR de palabras, cantidad de palabras alternativas, si es "tokens" .. cantidad a saltear */
	int	mrk;			/* spare use */
	struct	tregla	*nx;		/* proximo en la cadena (cuando es OR ) */
}	regla;

reglaptr	rpx,rp1,*rpa,*rpb;

reglaptr	tbr[20][64];		/* tabla de reglas ... 20 reglas ... 64 elementos x regla max */



/*
 *
 *	Estructuras y variables para lex / yacc
 *
 */


int yyparse();
int readInputForLexer( char *buffer, int *numBytesRead, int maxBytesToRead );

static int globalReadOffset;

int	gc_pal_e;		/* palabra encontrada */

int	gc_si;
int	gc_no;
int	v_flag;
int	l_flag;
int	tok;
int	tok_si;
int	tok_no;

char   *spp      = "                            ";

char 	*globalInputText;
char	buffer[4096];
char	global_ref[MAXP];

char	yy_buffer[64];
char	*yy_ptr;

int	gramatik(char *,int);

int	f_cneg;			/* flag si parser encontro negacion en la linea */
int	f_sneg;			/* flag si parser NO encontro negacion en la linea */

int	f_cb;			/* flag continuo buscando ... (en el abol que arma yacc ) */
int	f_pe_lin;		/* flag palabra encontrada en linea  (en yacc ) */
int	f_pe_ora;		/* flag palabra encontrada en frase nomimal (en yacc) */





/*
 *
 *	Estructuras y variables para la base de mapa  
 *
 */

#define	MAX_CPX	1000			/* cada zona es 1000 x 1000 ... para px py local */
#define	MAX_FCM  10			/* maxima cantidad de fila x columna de mapa */
#define	MAX_QEM	128			/* maxima cantidad de elementos en mapa, pensado: 10 x 10 */

int	tlmap[MAX_FCM][MAX_FCM][2];		/* matriz desde hasta  (offsets) */
int	zlmap[MAX_FCM][MAX_FCM][2];		/* matriz desde hasta  (offsets) */

int	bpmp_load();

int	q_pmp;				/* cantidad de elementos en mapa */

typedef	struct	tmapa	*mapaptr;

typedef	struct	tmapa
{		
	int	px;			/* posicion eje x */
	int	py;			/* posicion eje y */
	char	nm[16];			/* nombre del area OJO maximo !!!! */
	char	den[4];			/* densidad: DX .. X= (1 letra) Nula Rural Baja Media Alta */
	int	iden;			/* densidad: (numerico si hace falta ... ) */
	char	nse[4];			/* nivel socio economico: NX ... X= (1 letra) Nulo Baja Media Alta */
	int	inse;			/* nivel socio economico: (numerico si hace falta ) */ 
	int	qneg;			/* cantidad de comercios / negocios que hay en esa zona */
	int	ptr_p;			/* puntero al primer comercio en base de comercios */
	int	ptr_u;			/* puntero al ultimo comercio en base de comercios */
	int	sp1;			/* spare use */
#if 0
	struct	tmapa	*nx;		/* proximo en la cadena (cuando es OR ) */
#endif
}	mapa;

#if 0
mapaptr		rpx,rp1,*rpa,*rpb;
#endif


mapaptr		mmapa[MAX_FCM][MAX_FCM];	/* matriz mapa:  MAX_FCM x MAX_FCM pointers a tmapa ( 10 x 10) */



/*
 *
 *	Estructuras y variables para la base de comercio  
 *
 */


#define	MAX_QCM		4096		/* maxima cantidad de comercios en una zona del mapa */

int	bco1_load();
int	bcom_load();

int	q_com;				/* cantidad de comercios en mapa */


typedef	struct	tcome	*comeptr;

typedef	struct	tcome
{		
	int	ncom;			/* numero de comercio */
	int	zon_px;			/* a que zona pertence px */
	int	zon_py;			/* a que zona pertence py */
	int	i1,i2;			/* spare despues sacar */
	char	z_nom[64];		/* nombre de la zona */
	char	z_den[4];		/* densidad de la zona */
	char	z_nse[4];		/* nivel socio enconomico de la zona */
	int	i_den;			/* densidad de la zona, numero */
	int	i_nse;			/* nivel socio enonomico, numero */
	char	r_nom[64];		/* nombre del rubro */
	int	min;			/* cantidad minima de negocios del rubro en la zona */
	int	max;			/* cantidad maxima de negocios del rubro en la zona */
	int	qnt;			/* cantidad de negocios del rubro en la zona (gen al azr) */
	int	min2;
	int	max2;
	int	qnt2;
	int	i_tic;			/* tipo de rubro (normal, rural, caro) */
	int	px;			/* posicion eje x */
	int	py;			/* posicion eje y */
	char	co_nom[64];		/* nombre del comercio */
	int	valido;			/* 0 no valido 1 si valido */
	char	nact[64];		/* nombre de la actividad */
	char	nse[4];			/* nivel socio economico Baja Media Alta ... Nx ... x = B m o A */
	int	c_nse;			/* nivel socio econ del comercio ... 0 1 2 baja media alta */
	int	n_rub;			/* numero de rub (dentro de tic ) */
	int	p_rub;			/* ptr a vector de rubros */
	int	co_sp1;			/* spare use */
	int	co_sp2;			/* spare use */
}	come;

#if 0
mapaptr		rpx,rp1,*rpa,*rpb;
#endif



comeptr		mcome[409600];		/* vector de comercios (no puede ser menor a MAX_FCM x MAX_FCM * MAX_QCM !!! */
					/* ojo: revisar bcom_load si se cambia este valor !! */
/*
 *	4096	409600
 *	5000	500000
 *	8000	800000
 *     10000   1000000		no sera mucho !!?
 */



/*
 *
 *	Estructuras y variables para la tabla de definicion de comercios
 *
 */


#if 0
#define	MAX_FCM  10			/* maxima cantidad de fila x columna de mapa */
#define	MAX_QEM	128			/* maxima cantidad de elementos en mapa, pensado: 10 x 10 */
#endif

#define	MAX_CDE  30000			/* maxima cantidad de tipos de comercios ( 100 tipos x 3 nse ... x 100 zonas)  */

int	bcde_load();

int	q_cde;				/* cantidad de elementos en definicion de comercios */

typedef	struct	tlcde	*lcdeptr;

typedef	struct	tlcde
{		
	int	px;			/* posicion eje x (comercio pertenece a esta zona x,y) */
	int	py;			/* posicion eje y */
	char	nzon[32];		/* nombre de la zona */
	char	mden[4];		/* densidad de la zona en mapa original */
	char	mnse[4];		/* nse      de la zona en mapa original */
	int	iden;			/* densidad de la zona ... numerico */
	int	inse;			/* nivel socio economico: (numerico si hace falta ) */ 
	char	nact[32];		/* nombre de la actividad ... si es kiosko pesca electrodomesticos etc */
	char	den[4];			/* densidad en numero ... */
	char	nse[4];			/* nivel socio economico  en nro ... : NX ... X= (1 letra) Nulo Baja Media Alta */
	int	qcom_min;		/* cantidad de comercios minimo */
	int	qcom_max;		/* cantidad de comercios maximo */
	int	qnt;			/* cantidad a generar de negocios de rubro nnn */
	int	tic; 			/* tipo de comercio / negocio de rubro nnn 0 Normal, 1 rural, 2 caro */
	int	valido;			/* 0 no se usa 1 si se usa */
	int	sp1;			/* spare use */
#if 0
	struct	tlcde	*nx;		/* proximo en la cadena (cuando es OR ) */
#endif
}	lcde;

#if 0
mapaptr		rpx,rp1,*rpa,*rpb;
#endif


lcdeptr		mlcde[MAX_CDE];		/* tabla definicion de comercios MAX_FCM x MAX_CDE pointers a tclde */




/*
 *
 *	Estructuras y variables para la tabla de rubros
 *
 */

#define	MAX_DEN	5			/* total de densidades (Nulo, Rural, Baja, Media, Alta) */
#define	MAX_NSE	3			/* total de nivel socio economico (Bajo, Medio, Alto )  */
#define	MAX_TIC	3			/* total de tipo comercio (normal, rural, caro ) */
#define	MAX_FAM	3			/* total de tipo familia mumerosa ... baja media alta */
#define	MAX_PRM 2			/* total parametros x cada den/nse/tic ... ( minimo, maximo ) */


#if 0
#define	MAX_FCM  10			/* maxima cantidad de fila x columna de mapa */
#define	MAX_QEM	128			/* maxima cantidad de elementos en mapa, pensado: 10 x 10 */
#define	MAX_CDE  10000			/* maxima cantidad de tipos de comercios ( 100 tipos ... x 100 zonas)  */
#endif

#define	MAX_RUB  600			/* maxima cantidad de rubros */
#define	MAX_RXT	 200			/* maxima cantidad de rubros x tic */

char	mtic[3][4] = {"NO","RU","CA"};	/* tipo de rubros ... 3 .. deberia ser MAX_TIC !! */
char	mnse[3][4] = {"BA","ME","AL"};	/* tipo de nse    ... 3 .. deberia ser MAX_ .... (futuro) !! */

int	brub_load();

int	q_rub;				/* cantidad de elementos en rubros */
int	q_rxt;				/* cantidad de rubros x tic */
int	q_tic;				/* cantidad de tic */

typedef	struct	tlrub	*lrubptr;

typedef	struct	tlrub
{		
	int	ru_nro;			/* numero de rubro */
	int	ru_tic;			/* numero de tic */
	char	ru_mtic[4];		/* tic ... texto */
	char	ru_nom[64];			/* nombre del rubro */
	char	ru_desc[128];		/* descripcion */		
	int	ru_sp1;			/* spare use */
#if 0
	struct	tlrub	*nx;		/* proximo en la cadena (cuando es OR ) */
#endif
}	lrub;

#if 0
lrubptr		rpx,rp1,*rpa,*rpb;
#endif


lrubptr		mlrub[600];		/* tabla de rubros MAX_RXT * MAX_TIC ...  */






/*
 *
 *	Estructuras y variables para la distribucion de comercios x densidad de zona
 *
 */

/*
 *	ejemplo (como ver la matriz )
 *	Densidad        zona con densidad tipo .... Nula (no vive nadie), Rural, Baja (ciudad), Media, Alta
 *	Nivel SE        nivel socio economico de la zona (Bajo, Medio, Alto) 
 *	Tic             tipo de negocio ... Normal (hay en cualquier lugar), Rural (mas comun en zonas rurales),
 *	                caro (mas comun en zonas de nivel socio economico medio, alto
 *	min,max         Es el min, max de negocios ,de cada tipo tic, que hay en una zona segun densidad, nse
 *
 *
 *
 *	Densidad  
 *	          nse B                  nse M                 nse A
 *	Nula      0, 0   0, 0   0, 0     0, 0   0, 0   0, 0    0, 0   0, 0   0, 0
 *	Rural	  2, 4   4, 8   0, 0     3, 5   5, 9   0, 0    3, 5   2, 4   1, 1
 *	Baja     40,60   1, 2   1, 3    45,75   1, 1   8,16   65,78   0, 0  10,19
 *	Media
 *	Alta
 *
 *	Esto dice !!! 
 *	En una zona de densidad BAJA, de nivel socioencomico Medio, 
 *	deberia haber:
 *	  entre  45 y 75 negocios del tipo 'normal'
 *	  entre   1 y  1 negocios del tipo 'rutal'
 *	  entre   8 y 16 negocios del tipo 'caro'
 *
 */


/*			   dens (1 a 5 ), nse (1 a 3), rubro ( nor, rur, caro ), cantidad (min,max) */
/*      nor (1 a 70 )
 *      rur (1 a 10 )
 *      car (1 a 20 )
 */


int	prueba();

#if 0
int	dstc[5][3][3][2] = 
	{
		{
	  			{ { 0, 0},{ 0, 0},{ 0, 0} },
	  			{ { 0, 0},{ 0, 0},{ 0, 0} },
	  			{ { 0, 0},{ 0, 0},{ 0, 0} }
		},

		{
	  			{ { 2, 4},{ 4, 8},{ 0, 0} },
	  			{ { 3, 5},{ 5, 9},{ 0, 0} },
	  			{ { 3, 5},{ 2, 4},{ 1, 1} }
		},

		{
	  			{ {40,60},{ 1, 2},{ 1, 3} },
	  			{ {45,70},{ 1, 1},{ 8,16} },
	  			{ {50,70},{ 0, 0},{10,19} }
		},

		{
	  			{ {55,65},{ 0, 1},{11,20} },
	  			{ {56,67},{ 0, 2},{12,20} },
	  			{ {57,69},{ 0, 3},{13,20} }
		},

		{
	  			{ {22,65},{ 0, 2},{10,15} },
	  			{ {23,66},{ 0, 3},{10,16} },
	  			{ {24,67},{ 0, 4},{10,17} }
		}
	};

#endif


/*
 *
 *	Estructuras y variables para el factor de multiplicacion de comercios x densidad de zona
 *
 */


/*
 * #
 * #
 * #
 * #	Factor de multiplicacion
 * #	90 numeros ...
 * #
 * #
 * #                | ------------------------------- tipo de negocio ------------------------------------|
 * #                Normal                        Rural                         Caro
 * #  densidad      mi   ma   mi   ma   mi   ma   mi   ma   mi   ma   mi   ma   mi   ma   mi   ma   mi   ma
 * #
 * #       nse      B         M         A         B         M         A         B         M         A
 * 
 * densidad_nula ,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
 * densidad_rural,  11,  19,  15,  21,  18,  28,  12,  21,  15,  27,  18,  31,   1,   3,   2,   4,   4,   8
 * densidad_baja ,  35,  55,  45,  75,  55,  95,  11,  17,  21,  31,  55,  91,   5,  10,  10,  20,  30,  50
 * densidad_media,  55,  95, 105, 155,  70, 110,   3,   7,   4,   8,   2,   3,  20,  40,  50,  90,  70, 100
 * densidad_alta , 110, 190, 210, 290, 310, 390,   1,   5,   1,   4,   1,   3,  60,  70,  90, 110, 110, 150
 * 
 * #
 * #  campos:
 * #  1             2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19
 * #  densidad      mi  ma  mi  ma  mi  ma  mi  ma  mi  ma  mi  ma  mi  ma  mi  ma  mi  ma
 * #
 * #
 */




#if 0
#define	MAX_DEN	5			/* total de densidades (Nulo, Rural, Baja, Media, Alta) */
#define	MAX_NSE	3			/* total de nivel socio economico (Bajo, Medio, Alto )  */
#define	MAX_TIC	3			/* total de tipo comercio (normal, rural, caro ) */
#define	MAX_PRM 2			/* total parametros x cada den/nse/tic ... ( minimo, maximo ) */
#endif 

#define	MAX_TB1	5			/* maximo de lineas en tabla de fm ... coincide con densidades */
int	btb1_load();
int	fdmd[MAX_DEN][MAX_NSE][MAX_TIC][MAX_PRM] ;	/* tabla factor de multiplicacion definicion comercios fdmd */


#define	MAX_TB2	5			/* maximo de lineas en tabla de fm ... coincide con densidades */
int	btb2_load();
int	fdmc[MAX_DEN][MAX_NSE][MAX_TIC][MAX_PRM] ;	/* tabla factor de multiplicacion comercios fdmc */


#define	MAX_TB3	5			/* maximo de lineas en tabla de fm perf... coincide con densidades */
int	btb3_load();
int	fdmp[MAX_DEN][MAX_NSE][MAX_FAM][MAX_PRM] ;	/* tabla factor de multiplicacion perfiles fdmp */



/*
 *
 *	Estructuras y variables para los tipos de perfiles 
 *
 *	tipos de perfiles segun den,nse de zona 
 *
 *	DR,NB,   33,    22,     5,        20,     10,      5,        3,     1,     1
 */

#define	MAX_TB4	16			/* maxima cantidad de tipos de perfiles */

int	fx_tb4(int,int,char *);
int	btb4_load();
int	btb4_print();

int	q_tpf;				/* cantidad de tipo de perfiles */


typedef	struct	ttper	*tperptr;

typedef	struct	ttper
{		
	int	tp_nro;			/* numero de perfil */
	char	tp_nom[64];		/* nombre de perfil */
	char	tp_den[4];		/* densidad en la zona */
	char	tp_nse[4];		/* nivel socio economico B M A en la zona  */
	int	tp_iden;
	int	tp_inse;
	int	tp_por[3][3];		/* distribucion (porcentaje) segun Fam Nun / nse (del perfil) */
	int	tp_sp1;			/* spare */
}	tper;


tperptr		mtper[MAX_TB4];		/* tabla de tipos de perfil */


/*
 *
 *	Estructuras y variables para los perfiles de tarjethabientes 
 *
 */



/*
 * #
 * #	perf03.txt
 * #
 * #	perfiles de 'tarjetahabientes'
 * #
 * 
 * per_00,FB,NB, 10, 20,70,  100,  500,20,  500, 2000,10, 2000, 4000
 * per_01,FM,NB, 20, 30,80,  100,  500,15,  500, 2000,05, 2000, 4000
 * per_02,FA,NB, 30, 50,90,  100,  800,15,  800, 2200,05, 2200, 4500
 * 
 * per_03,FB,NM, 20, 60,30,  100,  800,50,  800, 2000,20, 2000, 6000
 * per_04,FM,NM, 40, 80,40,  100,  800,55,  800, 3000, 5, 3000, 8000
 * per_05,FA,NM, 60,120,50,  100, 1000,45, 1000, 4000, 5, 4000, 9000
 * 
 * per_06,FB,NA,100,140,20,  200, 1400,70, 1400, 5000,10, 5000,10000
 * per_07,FM,NA,120,180,30,  200, 1400,65, 1400, 6000, 5, 6000,12000
 * per_08,FA,NA,160,220,35,  200, 1800,60, 1800, 8000, 5, 8000,30000 
 * 
 * #
 * #	campos
 * #	nombre del perfil
 * #	famila numeroa (Baja, Media, Alta)
 * #	nivel se, (Bajo, Medio, Alto)
 * #	min de compras x mes
 * #	max de compras x mes
 * #	porcentaje_1 			(distribucion de los gastos)
 * #	monto min
 * #	monto max
 * #	porcentaje_2 
 * #	monto min
 * #	monto max
 * #	porcentaje_3 
 * #	monto min
 * #	monto max
 * #
 *
 */


#define	MAX_PFL	20			/* maxima cantidad de perfiles */

int	fx_per(int,int,char *);

int	bper_load();			/* carga de perfiles */
int	bper_print();			/* carga de perfiles */

int	q_per;				/* cantidad de perfiles en base */

typedef	struct	tperf	*perfptr;

typedef	struct	tperf
{		
	int	pe_nro;			/* numero de perfil */
	char	pe_nom[64];		/* nombre de perfil */
	char	pe_fam[4];		/* familia numerosa Baja Media Alta */
	char	pe_nse[4];		/* nivel socio economico B M A */
	int	pe_ifam;
	int	pe_inse;
	int	pe_cxm[2];		/* compras por mes min max */
	int	pe_dis[3][3];		/* distribucion de gastos ... 3 ternas, porc,min,max */
	int	re_sp1;			/* spare */
}	perf;

perfptr		perf1,perf2,*rperf1,*rperf2;

perfptr		mperf[MAX_PFL];		/* tabla de rubros MAX_RXT * MAX_TIC ...  */



/*
 *
 *	Estructuras y variables para los tarjethabientes / personas 
 *
 */

#define	MAX_PER	220000

int	fx_cpe(int,int,char *);
int	bcpe_load();
int	bcpe_print();			/* carga de personas  */

int	q_gpe;				/* cantidad de personas */


typedef	struct	tpers	*persptr;

typedef	struct	tpers
{		
	int	ps_nro;			/* numero de la persona */
	char	ps_nom[64];		/* nombre de la persona */
	int	ps_zon_px;		/* coord x de la zona */
	int	ps_zon_py;		/* coord y de la zona */
	char	ps_zon_den[4];		/* den de la zona */
	char	ps_zon_nse[4];		/* nse de la zona */
	int	ps_zon_i_den;		/* den de la zona */
	char	ps_zon_i_nse;		/* nse de la zona */
	int	ps_i_tpf;		/* tipo de perfil */
	char	ps_n_tpf[64];		/* nombre del tipo de perfil */
	char	ps_pe_nom[64];		/* nombre de ... ??? */
	char	ps_fam[4];		/* familia numerosa Baja Media Alta */
	char	ps_nse[4];		/* nivel socio economico B M A */
	int	ps_ifam;
	int	ps_inse;
	int	ps_cxm[2];		/* compras por mes min max */
	int	ps_dis[3][3];		/* distribucion de gastos ... 3 ternas, porc,min,max */
	int	ps_idx_off;		/* offset en vector de trx */
	int	ps_idx_qnt;		/* cantidad de transacciones de la persona a partir de offset  */
	int	ps_sp1;			/* spare */
}	pers;

persptr		pers1,pers2,*rpers1,*rpers2;

persptr		mpers[MAX_PER];		/* tabla de rubros MAX_RXT * MAX_TIC ...  */

int		zpers[MAX_PER][20];	/* super tablota con los ultmos 10 reg de trx de c persona ??? */

/*
 *
 *	Estructuras y variables para las transacciones  
 *	(OJO!!! trx en etapa de proceso !!! cuando van a disco en formato final, es ttrax !! )
 *
 *	Transacciones va x por una persona !!
 *
 */



#define	MAX_TRX	 10000			/* transacciones x persona en un anio ???? */
#define MAX_TR2	 1000000		/* transacciones (input file ) 
					 * - sirve para cargar un de trx para probar cosas en memoria
					 * - estabdar en 1 M ojo con las pruebas !!!!
					 */


int	fp_trx(int,int,char *);		/* parsear una linea del archivo de transacciones */
int	fp_trx2(int,int,char *);	/* parsear una linea del archivo de transacciones (para pruebas8 ) */
int	fp_trx3(int,int,char *);	/* parsear una linea del archivo de transacciones (para pruebas2 ) */
int	fx_trx(int,int,char *);

int	btrx_load();			/* carga de trx */
int	btrx_print();			/* print de trx */

int	q_trx;				/* cantidad de transacciones */
int	t_trx;				/* total de trx (que no es lo mismo !! ) */


/* tr1 */

typedef	struct	ttrnx	*trnxptr;

typedef	struct	ttrnx
{		
					/*	check columns ...
					 *	A  si lo copie o no sobre el registro mientras codifico !!!!
					 *	B  si lo meto en el registro fisico a grabar 
					 */

					/* A B  
					/* 1    el campo ya esta copiado */
					/* 2    esta copiado, pero hay que refinar ... calcular dia de la semana */

					/* datos de la transaccion ------------------ */
	int	tx_trx_nro;		/* 1 1  numero */
	int	tx_trx_dia;		/* 2    dia de la semana*/
	char	tx_trx_fecha[16];	/* 1 1  fecha */
	char	tx_trx_hora[16];	/* 1 1  hora */
	int	tx_trx_monto;		/* 1 1  monto */
	
					/* datos del articulo de la transaccion ----- */
	int	tx_art_desc;		/*      descripcion */

					/* datos de la zona de la transaccion ------- */
	int	tx_zon_px;		/* 1 1  coord px */
	int	tx_zon_py;		/* 1 1  coord py */
	char	tx_zon_nom[64];		/* 1    nombre de la zona  */
	char	tx_zon_den[4];		/* 1    densidad DR DN DB DM DA */	
	char	tx_zon_nse[4];		/* 1    nivel se NB NM NA */
	int	tx_zon_iden;		/* 1 1  densidad 0 - 4 */
	int	tx_zon_inse;		/* 1 1  nse 0 - 2 */

					/* datos del comercio de la transaccion ----- */
	int	tx_com_nro;		/* 1 1  numero de comercio */
	int	tx_com_px;		/* 1 1  coord px */
	int	tx_com_py;		/* 1 1  coord py */	
	char	tx_com_nom[64];		/* 1    nombre */
	char	tx_com_rub[64];		/* 1    rubro */					/* y la actividad ???? */
	int	tx_com_nrub;		/* 1    nro de rubro */
	int	tx_com_itic;		/* 1 1  tipo 0 normal 1 rural 2 caro */
	char	tx_com_nse[4];		/* 1    nivel se */
	int	tx_com_inse;		/* 1 1  nivel socio econ numero */
	
					/* datos de la persona de la transaccion ----- */
	int	tx_per_nro;		/* 1    numero */
	int	tx_per_px;		/* 1    vive en zona... px */	
	int	tx_per_py;		/* 1    vive en zona... py */
	char	tx_per_nom[64];		/* 1    nombre */	
	char	tx_per_fam[4];		/* 1    Fam Numerosa  FB FM FA */
	char	tx_per_nse[4];		/* 1    nivel se fam  NB NM NA */
	int	tx_per_ifam;		/* 1    familia numerosa numero */
	int	tx_per_inse;		/* 1    familia nse numero */

					/* calificadores */
	int	tx_cal_n1;		/*      el sistema lo detecto como fraude 0 no 1 si */
	int	tx_cal_n2;		/*      es fraude 0 no 1 si */

}	trnx;


trnxptr		ptrnx1,ptrnx2,*rtrnx1,*rtrnx2;

trnxptr		mtrnx[MAX_TRX];		/* tabla de transacciones */

trnx	tr1,tr2;			/* una st trx para juntar la info a grabar */
trnx	tr_s;				/* tr para stats */


int	create_fdt(trnxptr);		/* create fraud detection trx */
int	graba_gtx(FILE*,trnxptr);

char	*gnf_fd(int);			/* genera fecha al azar para fd  */
char	*gnf_fd2(trnxptr,int);		/* genera fecha al azar para fd  */
char	*gnh_hd2(trnxptr,int);

int	compara(trnxptr,trnxptr);
int	ordenar_trx(const void * ptr1, const void * ptr2);


#if 0

/*
 *	Registro de trx en files
 *
 */

					/* datos de la transaccion */
		tr1.tx_trx_fecha	
		tr1.tx_trx_hora
		tr1.tx_trx_nro		
		tr1.tx_trx_monto
		tr1.tx_art_desc
					/* datos de la zona */
		tr1.tx_zon_px
		tr1.tx_zon_py
		tr1.tx_zon_iden
		tr1.tx_zon_inse
					/* datos del comercio  */
		tr1.tx_com_nro
		tr1.tx_com_px
		tr1.tx_com_py
		tr1.tx_com_inse	
		tr1.tx_com_itic	
		tr1.tx_com_nrub
					/* datos de la persona  */
		tr1.tx_per_nro	
		tr1.tx_per_px	
		tr1.tx_per_py
		tr1.tx_per_ifam	
		tr1.tx_per_inse	
					/* datos para entrenamiento de ML */
		tr1.tx_cal_n1	
		tr1.tx_cal_n2 
#endif

/*
 *
 *	Estructuras y variables para las transacciones (input file) 
 *
 *	En el caso de las transacciones, la estructura utilizada
 *	para grabar file y cargar file, son diferentes.
 *
 */




typedef	struct	ttrax	*traxptr;

typedef	struct	ttrax
{		
					/* datos de la transaccion ------------------ */
	char	tx_trx_fecha[12];	/* 1 1  fecha */
	char	tx_trx_hora[12];	/* 1 1  hora */
	int	tx_trx_dia;		/* 2    dia de la semana*/
	int	tx_trx_nro;		/* 1 1  numero */
	int	tx_trx_monto;		/* 1 1  monto */
	
					/* datos del articulo de la transaccion ----- */
	int	tx_art_desc;		/*      descripcion */

					/* datos de la zona de la transaccion ------- */
	int	tx_zon_px;		/* 1 1  coord px */
	int	tx_zon_py;		/* 1 1  coord py */
	int	tx_zon_iden;		/* 1 1  densidad 0 - 4 */
	int	tx_zon_inse;		/* 1 1  nse 0 - 2 */

					/* datos del comercio de la transaccion ----- */
	int	tx_com_nro;		/* 1 1  numero de comercio */
	int	tx_com_px;		/* 1 1  coord px */
	int	tx_com_py;		/* 1 1  coord py */	
	int	tx_com_inse;		/* 1 1  nivel socio econ numero */
	int	tx_com_itic;		/* 1 1  tipo 0 normal 1 rural 2 caro */
	int	tx_com_nrub;		/* 1    nro de rubro */
	
					/* datos de la persona de la transaccion ----- */
	int	tx_per_nro;		/* 1    numero */
	int	tx_per_px;		/* 1    vive en zona... px */	
	int	tx_per_py;		/* 1    vive en zona... py */
	int	tx_per_ifam;		/* 1    familia numerosa numero */
	int	tx_per_inse;		/* 1    familia nse numero */

					/* calificadores */
	int	tx_cal_n1;		/*      el sistema lo detecto como fraude 0 no 1 si */
	int	tx_cal_n2;		/*      es fraude 0 no 1 si */

}	trax;


traxptr		trax1,trax2,*rtrax1,*rtrax2;

traxptr		mtrax[MAX_TR2];		/* tabla de transacciones */

trax	transaccion2;			/* una st trx para juntar la info a grabar */
trax	tr_x;



int	graba_trx(FILE*,traxptr);




/*
 *
 *	Estructuras y variables para los registros de fix a
 *
 */


#define	MAX_FXA	50000		/* maxima cantidad de trx truchas generadas */

int	bfxa_load();
int	bfxa_print();

int	q_fxa;				/* cantidad de reg fix a */


typedef	struct	trfxa	*rfxaptr;

typedef	struct	trfxa
{		
	int	fxa_nreg;		/* numero de registro en trx */
	int	fxa_per;		/* numero de persona */
	char	fxa_fecha[16];		/* fecha de la trx */
	char	fxa_hora[16];		/* hora */
	int	fxa_cal_n1;		/* 0 no trux 1 si trux */
	int	fxa_cal_n2;		/* 0 todavia no la vio como trux 1 si lo vio */

}	rfxa;

#if 0
rfxaptr		rfxa1,rfxa2,*rfxa_ptr;
#endif



rfxaptr		mrfxa[MAX_FXA];		/* vector de registros fix a  */








/*
 *
 *	Estructuras y variables para la 'eligibilidad' de los comercios
 *
 *
 */

#define	MAX_ZCO  12000			/* vector de comercios de una zona */
#define	MAX_VCO	200000			/* vector con punteros a comercios */

int	q_vec;				/* cant de elementos en vector */

int	el_mapa[MAX_FCM][MAX_FCM];	/* mapa para determinar zonas eligibles */

int	el_come[MAX_VCO];


int	zo_come[MAX_ZCO][2];		/* debe ser algo mayor al mayor de los comercios x zona */ 
					/* vector 0 nro comercio 1 distancia a un comercio determinado */

/*
 *	Estructuras para bcvs_load ... carga generica de archivo campos separados por comas
 */

int	csv_parser(char *, int (*)(int,int,char *));

int	bcvs_load(char *, FILE *, int (*)(int,int,char *));
int	fx(int,int,char *);

int	bpru_load();


/*
 *
 *	factores ML 
 *
 */

int	cfml;			/* chotex factor multiplicador de ML */




/*
 *	index en pipe de trx  
 */


int	gt_trx[100];
int	clasificador;
int	gt_at[100];
int	call_algoritmo_ml(int);
int	cl_00,cl_01,cl_10,cl_11,cl_tt;






/*
 *
 *	Estructuras y variables para indice de persona / trx
 *
 */


int	fp_idx1(int,int,char *);
int	bidx1_load();

int	q_idx1;				/* cantidad de elementos en indice */


typedef	struct	tidx1	*idx1ptr;

typedef	struct	tidx1
{		
	int	ix_nro;			/* numero de la persona */
	int	ix_trx;			/* numero de la trx     */
}	idx1;

idx1	s_idx1;





/*
 *	nuevo invento ...
 *	malloc con puntero a puntero ....
 */

#define	MAX_OBJ		140000000		/* maxima cantidad de objetos en area */

int	*obj_ptr;












/*
 *	//ojo//  //agregar//
 *
 *	atenti !!! por ahora , la combinacion, en mapa01.txt, DR, NN explota !!
 *	o sea, si densidad es nula, nse debe ser nulo tambien !!
 *
 *	agregar ....
 *	- revisar la linea de comando !!! combinaciones validas y sintaxis
 *	- cierta validacion de los formatos de las tablas que suben 
 *
 *	Seria piola tener un identificado !tipo en los archivos, o similar,
 *	y que los procesos de carga, (bxxx_load ) verifiquen con ese
 *	codigo que el tipo de archivo es el que corresponde.
 *
 *
 *	Eleccion del negocio en la trx !!
 *	tbn es variable, a mejorar en el futuro
 *	pero ...
 * 
 *	Segun 'fam' y 'nse' ....  
 *	definimos un 'combo' de porcentajes 
 *	
 *	ej:
 *	tiene distribucion
 *	70 %  100   500
 *	20 %  500  2000
 *	10 % 2000  4000
 *
 *	ademas ....
 *	(como distribuir las compras en 'zonas' !???  )
 * 
 *	segundo 'tipo' de trx truchas !
 *	- podria ser ... en lugares diferentes,
 *	  todas por internet !!!!
 *
 *
 */




/*
 *	para utilizar xx_per
 *	poner en prox if ...
 *
 */

#if 0
#define	XX_PER	1
#endif


#if	XX_PER
char	xx_per[5000][10];
#endif



/*
 * -----------------------------------------------------------------------------------
 *
 *	main
 *
 *
 *
 * -----------------------------------------------------------------------------------
 */

int	main(argc,argv)
int	argc;
char	**argv;
{
	int	n;

	/* miscelaneas gobales */
	flag_before_exit = 0;
	srand (time(NULL));

	/* init varios */
	gp_default();
	gp_init(argc,argv);
	gp_parser();

	/* shit finger ? */
	if (gp_help)
	 	uso(0);


	/* programa */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra a programa principal \n\n",gp_tm());
		gp_print();
	}


	/* opciones ingresadas  */
	if (gp_fverbose("d2"))
	{	gp_test();
	}


	/* abrir files */
	abro_files();


	/* proceso principal */
	proceso_principal();

	/* cerrar files */
	cierro_files();


	/* reportes ... */
	if (gp_fverbose("i1"))
	{
		mostrar_reportes();
	}

	/* si hay algo que hacer antes de salir, es aca ! */
	if (flag_before_exit)
	{	
		printf ("Atencion: .... \n");
	}

	/* programa */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de programa principal \n\n",gp_tm());
	}

}




/*
 * -----------------------------------------------------------------------------------
 *
 * 	reportes 
 *
 * 	muestra reportes al finalizar todos los procesos
 *
 *
 * -----------------------------------------------------------------------------------
 */



int	mostrar_reportes()
{
	int	n;

	/* proceso  */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra a mostrar reportes \n\n",gp_tm());
	}


	if ( ffcde || ffgdc )
	{
		n = cantidad_n1();
		printf ("Cantidad de tipos de comercio a generar ... %d\n",n);
	}


	/* proceso  */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de mostrar cargas \n\n",gp_tm());
	}
}


/*
 * -----------------------------------------------------------------------------------
 *
 * 	proceso_principal
 *
 * 	procesa zonas definidas en el mapa
 *
 *
 * -----------------------------------------------------------------------------------
 */

int	proceso_principal()
{

	int	i;
	int	px,py;

	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra a proceso principal \n\n",gp_tm());
	}


	/* muestro resultado de cargas, si las hubo */
	if (gp_fverbose("d3"))
	{
		mostrar_cargas();
	}



	/* si es 'prueba' .... proceso alternativo */
	if ( fftst)
		proceso_prueba();


	if ( ffprb)
	{
		if (ffprb == 1)
			pro_pruebas1();
		if (ffprb == 2)
			pro_pruebas2();
		if (ffprb == 3)
			pro_pruebas3();
		if (ffprb == 4)
			pro_pruebas4();
		if (ffprb == 5)
			pro_pruebas5();
		if (ffprb == 6)
			pro_pruebas6();
		if (ffprb == 7)
			pro_pruebas7();
		if (ffprb == 8)
			pro_pruebas8();
		if (ffprb == 9)
			pro_pruebas9();
	}	

	if ( ffexc)
	{
		if (ffexc == 1)	
			pro_exec1();
#if 0
		if (ffexc == 2)
			pro_exec2();
		if (ffexc == 3)
			pro_exec3();
		if (ffexc == 4)
			pro_exec4();
		if (ffexc == 5)
			pro_exec5();
		if (ffexc == 6)
			pro_exec6();
		if (ffexc == 7)
			pro_exec7();
		if (ffexc == 8)
			pro_exec8();
		if (ffexc == 9)
			pro_exec9();
#endif
	}


	/* si utilizo -gdc ... generar tabla de definicion de comercios */
	if ( ffgdc)
		genera_defcom();

	/* si utilizo -gco ... generar tabla de comercios */
	if ( ffgco)
		genera_comercios();

	/* si utilizo -gru ... generar tabla de rubros */
	if ( ffgru)
		genera_rubros();

	/* si utilizo -gdp .... generar tabla de definicion de perfiles x zona */
	if ( ffgdp)
		genera_defper();

	/* si utilizo -gpe .... generar tabla de personas x zona */
	if ( ffgpe)
		genera_personas();

	/* si utilizo -gtx .... generar tabla de transacciones
	 * atenti:
	 * Si se combina -gtx y -tru... graba las truchas
	 * a continuacion del archivo de transacciones 'verd'
	 */
	if ( ffgtx)
		genera_transacciones();

	/* si utilizo -tru .... generar tabla de trx truchas */
	if ( fftru)
		genera_fraudes();

	/* si utilizo -i ...... reindexar file */
	if (gp_reidx)
		re_indexa_trx();

	/* is utilizo -sta .... statistics de transacciones    */
	if ( ffsta)
		proceso_stat();

	/* is utilizo -wek .... archivo para weka   */
	if ( ffwek)
		proceso_weka();

	/* is utilizo -fxa .... archivo para fix a   */
	if ( fffxa)
		proceso_fxa();






	/* si utilizo -com ... grabar tabla de comercios  */
	if ( ffgco )
	{
		for (i=0; i<q_com; i++)
		{	
			fprintf (hfgco,"%06d,%03d,%03d,%s,%s,%s,%d,%d,%s,%s,%03d,%03d,%3d,%d,%04d,%04d,%s,%d,%3d,%3d,%3d,%3d,%3d\n",
				(*mcome[i]).ncom,
				(*mcome[i]).zon_px,
				(*mcome[i]).zon_py,
				(*mcome[i]).z_nom,
				(*mcome[i]).z_den,
				(*mcome[i]).z_nse,
				(*mcome[i]).i_den,
				(*mcome[i]).i_nse,
				(*mcome[i]).nact,
				(*mcome[i]).r_nom,
				(*mcome[i]).min,
				(*mcome[i]).max,
				(*mcome[i]).qnt,
				(*mcome[i]).i_tic,
				(*mcome[i]).px,
				(*mcome[i]).py,
				(*mcome[i]).nse,
				(*mcome[i]).c_nse,
				(*mcome[i]).min2,
				(*mcome[i]).max2,
				(*mcome[i]).qnt2,
				(*mcome[i]).n_rub,
				(*mcome[i]).p_rub
				);
		}
	}


	/* si utilizo -gru ... grabar tabla de rubros */
	if ( ffgru)
	{
		for (i=0; i<q_rub; i++)
		{	
			fprintf (hfgru,"%06d,%d,%s,%s,%s\n",
				(*mlrub[i]).ru_nro,
				(*mlrub[i]).ru_tic,
				mtic[  (*mlrub[i]).ru_tic ],
				(*mlrub[i]).ru_nom,
				(*mlrub[i]).ru_desc
				);
		}

	}
	
	/* si utilizo -gdc ... grabar tabla de definicion de comercios */
	if ( ffgdc)
	{
		/*
		 * OJO !!!! nact ??? nombre ???
		 * tamanio ... y es el rubro ??
		 *
		 */

		for (i=0; i< q_cde ; i++)
			if ( mlcde[i] !=  (lcdeptr) NULL )
			{
				fprintf (hfgdc,"%02d,%02d,%s,%s,%s,%s,%s,%s,%05d,%05d,%03d,%03d,%d\n",
					(*mlcde[i]).px,
					(*mlcde[i]).py,
					(*mlcde[i]).nzon,
					(*mlcde[i]).mden,
					(*mlcde[i]).mnse,
					(*mlcde[i]).nact,
					(*mlcde[i]).den,
					(*mlcde[i]).nse,
					(*mlcde[i]).qcom_min,
					(*mlcde[i]).qcom_max,
					(*mlcde[i]).qnt,
					(*mlcde[i]).tic,
					(*mlcde[i]).valido
					);
			}
	}

	/* si utilizo -gpe ... grabar tabla de personas  */
	if ( ffgpe)
	{
		for (i=0; i<q_gpe; i++)
		{
			fprintf (hfgpe,"%06d,%s,%02d,%02d,%s,%s,%d,%d,%02d,%s,%s,%s,%s,%d,%d,%3d,%3d,%2d,%4d,%5d,%2d,%4d,%5d,%2d,%4d,%5d\n",
				(*mpers[i]).ps_nro,
				(*mpers[i]).ps_nom, 
				(*mpers[i]).ps_zon_px,
				(*mpers[i]).ps_zon_py,
				(*mpers[i]).ps_zon_den,
				(*mpers[i]).ps_zon_nse,
				(*mpers[i]).ps_zon_i_den,
				(*mpers[i]).ps_zon_i_nse,
				(*mpers[i]).ps_i_tpf,
				(*mpers[i]).ps_n_tpf,
				(*mpers[i]).ps_pe_nom,
				(*mpers[i]).ps_fam,
				(*mpers[i]).ps_nse,
				(*mpers[i]).ps_ifam,
				(*mpers[i]).ps_inse,
				(*mpers[i]).ps_cxm[0],
				(*mpers[i]).ps_cxm[1],
				(*mpers[i]).ps_dis[0][0], 
				(*mpers[i]).ps_dis[0][1], 
				(*mpers[i]).ps_dis[0][2], 
				(*mpers[i]).ps_dis[1][0], 
				(*mpers[i]).ps_dis[1][1], 
				(*mpers[i]).ps_dis[1][2], 
				(*mpers[i]).ps_dis[2][0], 
				(*mpers[i]).ps_dis[2][1], 
				(*mpers[i]).ps_dis[2][2] 
				);
		}
	}


	if ( ffpro)
	{
		if (ffpro == 1)
			pro_proceso1();
		if (ffpro == 2)
			pro_proceso2();
#if 0
		if (ffpro == 3)
			pro_proceso3();
		if (ffpro == 4)
			pro_proceso4();
		if (ffpro == 5)
			pro_proceso5();
		if (ffpro == 6)
			pro_proceso6();
		if (ffpro == 7)
			pro_proceso7();
		if (ffpro == 8)
			pro_proceso8();
		if (ffpro == 9)
			pro_proceso9();
#endif
	}	





	/* reporting */
	if (gp_fverbose("i1") )
	{
		if (ffgdc)
		{
			printf ("Grabo %d registros en archivo %s \n",q_cde,fgdc);
		}
	}
	
	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de proceso principal \n\n",gp_tm());
	}

}


/*
 * -----------------------------------------------------------------------------------
 *
 *	re_indexa_trx
 *
 *	reindexa el archivo de transacciones desde 0  ...
 *
 * -----------------------------------------------------------------------------------
 */


int	re_indexa_trx()
{
	int	i,j,k;
	int	i_idx;
	int	t_cnt;
	int	q_reg;

	char	b1[MAXB];
	char	n_idx[16];


	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra a re indexa trx \n\n",gp_tm());
	}


	/* necesitamos transacciones indicadas en -inp ...  */
	if ( !ffinp)
		error(813);

	/* necesitamos archivo de salida en -out   ...  */
	if ( !ffout)
		error(814);

	i_idx = 0;	/* trucho ... para que parezca pipeline !!! */
	t_cnt = 0;
	q_reg = 0;

	while (fgets(b1,MAXB,hfinp) != NULL)
	{
		sprintf (n_idx,"%010d",i_idx);

		for (i=0; i< 10; i++)
			b1[16+i] = n_idx[i];

		fprintf (hfout,"%s",b1);

		if (b1[strlen(b1)-2] == '1')	
			t_cnt++;

		i_idx++;
		q_reg++;
	}

	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("Cantidad de registros re indexados  %d \n",q_reg);
		printf ("Cantidad de registros truchos       %d \n",t_cnt); 
	}


	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de re indexa trx \n\n",gp_tm());
	}

}

/*
 * -----------------------------------------------------------------------------------
 *
 *	proceso_prueba
 *
 *	pruebas ...
 *
 * -----------------------------------------------------------------------------------
 */

int	proceso_prueba()
{
	int	v[200];
	int	i,j,k;
	int	n,q;


	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra proceso prueba \n\n",gp_tm());
	}
		

#if 0
	bpru_load();
#endif
	q = 10;

	for (i=0; i<q; i++)
	{
		v[i] = gna(1,1000);
	}


	for (i=0; i<q-1; i++)
		for (j=i+1; j<q; j++)
			if ( v[j] < v[i] )
			{
				n = v[j];
				v[j] = v[i];		
				v[i] = n;
			}


	for (i=0; i<q; i++)
		printf ("v[%2d] %4d \n",i,v[i]);

	printf ("\n");
					
	


	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale proceso prueba \n\n",gp_tm());
	}
}



/*
 * -----------------------------------------------------------------------------------
 *
 *	pro_exec 1
 *
 *	exec aparte ...
 *
 * -----------------------------------------------------------------------------------
 */

/*
 *	procesa un pipeline !!
 *	
 *	variables que gobiernan el funcionamiento ...
 *
 *	f_inversa ( 0 ... va de menor a mayor indice , 1 ... a la inversa )
 *
 */


#if 0

int	pro_exec1()
{
	return 1;
}

#endif


#if 1


/*
 *	proceso pipeline 
 *
 *	con esta configuracion
 *	trabaja siempre con N trx  
 *	carga inicial de 2N trx
 *	procesa las N ultimas cargadas 
 *	al final de cada proceso, mueve las N ultimas a la primera posicion
 *	(porque se supone que el proceso requiere a las anteriores para consulta ! 
 *	al volver a cargar, pisa las ultimas N ... (que fueron movidas) 
 *	usa un vector de 2N posiciones ...
 */



int	pp_qtr_car_ini;			/* numero de trx iniciales a cargar   */
int	pp_qtr_car_nxt;			/* numero de trx siguientes a cargar  */
int	pp_qtr_prx_car;			/* numero de trx en proxima carga    */
int	pp_ptr_bse_ven_ant;		/* base de ventana a procesar ant */
int	pp_ptr_bse_pro;			/* base de trx a partir de cual procesar */
int	pp_ptr_bse_car;			/* base de trx a partir de cual cargar */

int	pp_ptr_bse_car;			/* puntero base donde comenzar a cargar */

int	pp_ptr_bse_ven;			/* ptr a base de ventana */

int	pp_qtr;				/* cantidad de trx en ventana  */
int	pp_trx;				/* nro de transaccion actual */
int	pp_ptr;				/* ptr sobre la base ... (depende de inversa o no ) */
int	pp_pro;				/* proceso solo pp_pro de la ventana (ojo con f_mover !! ) */

int	pro_exec1()
{
	char	b1[MAXB];

	int	i,j,k;
	int	f1;


				/* flags */
	int	f_hay_pipe;
	int	f_mover;
	int	f_buscar;
	int	f_primera;

				/* setings */
	int	f_inversa;

	char	*read_status;

	int	mm1,mm2;



	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra proceso exec 1 \n\n",gp_tm());
	}
		
	/* necesitamos transacciones indicadas en -inp ...  */
	if ( !ffinp)
		error(813);

	/* necesitamos archivo de salida en -out   ...  */
	if ( !ffout)
		error(814);


	/* algunos inits */                   
	for (i=0; i< MAX_TR2; i++)
	{
		mtrax[i] = (traxptr ) malloc(sizeof(trax));
			if (mtrax[i] == NULL )
				error(908); 
	}


	pp_qtr_car_ini = 1000000;			/* numero de trx iniciales a cargar   */
	pp_qtr_car_nxt =  500000;			/* numero de trx siguientes a cargar  */
	pp_pro         =  500000;			/* procesar de a ... */
	pp_ptr_bse_pro =  500000;			/* procesar a partir de ... */

	f_hay_pipe = 1;				/* set flags ... */
	f_buscar   = 1;
	f_primera  = 1;
	f_inversa  = 0;				/* 0 .. recorrer de menor indice a mayor ... 1 a la inversa */

	mm1 = 100000;
	mm2 = 100000;



	/* leo reg x reg del archivo de input */
	while (f_hay_pipe)
	{
		if (f_buscar)
		{
			if (!f_primera)
			{
				f_mover = 1;
				pp_ptr_bse_car = pp_ptr_bse_car + pp_qtr_prx_car;
				pp_qtr_prx_car = pp_qtr_car_nxt;		/* numero de trx en proxima carga    */
				
				pp_ptr_bse_car = pp_ptr_bse_pro;		/* cargar a partir de pos ... */

			}


			if (f_primera)
			{
				f_primera = 0;
				f_mover   = 0;
				pp_ptr_bse_car = 0;
				pp_qtr_prx_car = pp_qtr_car_ini;		/* numero de trx en proxima carga    */
				
				pp_ptr_bse_ven_ant = 0;				/* base de ventana a procesar ant */

				pp_ptr_bse_car = 0;				/* cargar a partir de pos ... */
			}
			
			if (f_mover)
			{
				printf ("Mover ... \n");
			
				for (i=0; i<pp_ptr_bse_pro; i++)
				{
					memcpy ( mtrax[i], mtrax[i+pp_ptr_bse_pro], sizeof(trax));	
				}
			}

			f1      = 1;
			pp_qtr  = 0;



			/* busco siguiente 'bloque' de pipe */
			while (f1 && (read_status = fgets(b1,MAXB,hfinp)) != NULL)
			{
				memset ( &tr_x , 0 , sizeof ( tr_x) );
				csv_parser(b1,fp_trx3);

#if 0
				memcpy ( mtrax[ pp_ptr_bse_car + pp_qtr ],&tr_x,sizeof(trax) );
#endif

				memcpy ( mtrax[ pp_ptr_bse_car +  pp_qtr ],&tr_x,sizeof(trax) );

				if ( mm1 == 1 || (pp_qtr && pp_qtr % mm1 == 0) )
				{
					printf ("%s Leo pipe (Cant trx: %6d) trx nro: %10d (%8d,%s,%s)  %s\n", 
						gp_tm(),
						pp_qtr,
						tr_x.tx_trx_nro,
						tr_x.tx_per_nro,
						tr_x.tx_trx_fecha,
						tr_x.tx_trx_hora,
						" " );
				}

				pp_qtr++;

				if ( pp_qtr == pp_qtr_prx_car )
					f1 = 0;
			}

			printf ("-----\n");

			f_buscar = 0;

#if 0
			pp_qtr = pp_qtr_prx_car;			/* cantidad en ventana */
#endif
			pp_trx = 0;					/* contador */

			pp_ptr_bse_ven = pp_ptr_bse_ven_ant;		/* base de ventana a procesar  */
			pp_ptr_bse_ven_ant = pp_ptr_bse_ven + pp_qtr;	/* base de ventana a procesar  */

			if ( ! f_inversa )
			{	
				pp_ptr = 0;
			}
			if (   f_inversa )
			{
				pp_ptr = pp_qtr - 1;			
			}	


		}
		/* if (f_buscar ...  */



		/* aca hago algo con las transaciones mientras tengo 'pipe' en memoria */
		if (!f_buscar)
		{
			/* En este punto, paso x cada transaccion en el pipeline !!!  */


#if 0
			fprintf (hfout,"%10d %10d %8d %s\n",
				pp_trx,
				(*mtrax[pp_ptr_bse_ven + pp_ptr]).tx_trx_nro,
				(*mtrax[pp_ptr_bse_ven + pp_ptr]).tx_per_nro,
				" ");
#endif
			fprintf (hfout,"%10d %10d %8d %s\n",
				pp_trx,
				(*mtrax[pp_ptr_bse_pro + pp_ptr]).tx_trx_nro,
				(*mtrax[pp_ptr_bse_pro + pp_ptr]).tx_per_nro,
				" ");


#if 0
			if ( mm2 == 1 || (pp_trx && pp_trx % mm2 == 0 )  )
			{
				printf ("%s Proceso  (n_tr: %6d) tr nro: %10d  f/h %s:%s    %s\n",
					gp_tm(),
					pp_trx,
					(*mtrax[pp_ptr_bse_ven + pp_ptr]).tx_trx_nro,
					(*mtrax[pp_ptr_bse_ven + pp_ptr]).tx_trx_fecha,
					(*mtrax[pp_ptr_bse_ven + pp_ptr]).tx_trx_hora,
					" ");
			}
#endif

			if ( mm2 == 1 || (pp_trx && pp_trx % mm2 == 0 )  )
			{
				printf ("%s Proceso  (n_tr: %6d) tr nro: %10d  f/h %s:%s    %s\n",
					gp_tm(),
					pp_trx,
					(*mtrax[pp_ptr_bse_pro + pp_ptr]).tx_trx_nro,
					(*mtrax[pp_ptr_bse_pro + pp_ptr]).tx_trx_fecha,
					(*mtrax[pp_ptr_bse_pro + pp_ptr]).tx_trx_hora,
					" ");
			}


			pp_trx++;

			if ( ! f_inversa )
			{	
				pp_ptr++;
			}
			if (   f_inversa )
			{
				pp_ptr--;
			}	

			if ( pp_trx == pp_qtr || pp_trx == pp_pro )
			{
				f_buscar = 1;
				if (read_status == NULL)
					f_hay_pipe = 0;
			}
		}
		/* if (!f_buscar ... */


	}
	/* while (f_hay_pipe ... */




	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale proceso exec 1 \n\n",gp_tm());
	}
}


#endif



#if 0


/*	
 *	proceso pipeline 
 * 	va cargando de a 2N ...
 *	y procesa
 */



int	pp_qtr_car_ini;			/* numero de trx iniciales a cargar   */
int	pp_qtr_car_nxt;			/* numero de trx siguientes a cargar  */
int	pp_qtr_prx_car;			/* numero de trx en proxima carga    */
int	pp_ptr_bse_ven_ant;		/* base de ventana a procesar ant */
int	pp_ptr_bse_pro;			/* base de trx a partir de cual procesar */
int	pp_ptr_bse_car;			/* base de trx a partir de cual cargar */

int	pp_ptr_bse_car;			/* puntero base donde comenzar a cargar */

int	pp_ptr_bse_ven;			/* ptr a base de ventana */

int	pp_qtr;				/* cantidad de trx en ventana  */
int	pp_trx;				/* nro de transaccion actual */
int	pp_ptr;				/* ptr sobre la base ... (depende de inversa o no ) */
int	pp_pro;				/* proceso solo pp_pro de la ventana (ojo con f_mover !! ) */

int	pro_exec1()
{
	char	b1[MAXB];

	int	i,j,k;
	int	f1;


				/* flags */
	int	f_hay_pipe;
	int	f_mover;
	int	f_buscar;
	int	f_primera;

				/* setings */
	int	f_inversa;

	char	*read_status;

	int	mm1,mm2;



	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra proceso exec 1 \n\n",gp_tm());
	}
		
	/* necesitamos transacciones indicadas en -inp ...  */
	if ( !ffinp)
		error(813);

	/* necesitamos archivo de salida en -out   ...  */
	if ( !ffout)
		error(814);


	/* algunos inits */                   
	for (i=0; i< MAX_TR2; i++)
	{
		mtrax[i] = (traxptr ) malloc(sizeof(trax));
			if (mtrax[i] == NULL )
				error(908); 
	}


	pp_qtr_car_ini = 100;			/* numero de trx iniciales a cargar   */
	pp_qtr_car_nxt = 100;			/* numero de trx siguientes a cargar  */
	pp_pro         = 100;			/* procesar de a ... */
	pp_ptr_bse_pro =   0;			/* procesar a partir de ... */

	f_hay_pipe = 1;				/* set flags ... */
	f_buscar   = 1;
	f_primera  = 1;
	f_inversa  = 0;				/* 0 .. recorrer de menor indice a mayor ... 1 a la inversa */

	mm1 = 1;
	mm2 = 1;



	/* leo reg x reg del archivo de input */
	while (f_hay_pipe)
	{
		if (f_buscar)
		{
			if (!f_primera)
			{
				f_mover = 1;
				pp_ptr_bse_car = pp_ptr_bse_car + pp_qtr_prx_car;
				pp_qtr_prx_car = pp_qtr_car_nxt;		/* numero de trx en proxima carga    */
				
				pp_ptr_bse_car =  0;				/* cargar a partir de pos ... */

			}


			if (f_primera)
			{
				f_primera = 0;
				f_mover   = 0;
				pp_ptr_bse_car = 0;
				pp_qtr_prx_car = pp_qtr_car_ini;		/* numero de trx en proxima carga    */
				
				pp_ptr_bse_ven_ant = 0;				/* base de ventana a procesar ant */

				pp_ptr_bse_car = 0;				/* cargar a partir de pos ... */
				/* aca hay dependencia del '50' ... para cambiar luego */
			}
			
			if (f_mover)
			{
				printf ("Mover ... \n");
			
#if 0
				for (i=0; i<50; i++)
				{
					memcpy ( mtrax[i], mtrax[i+50], sizeof(trax));	
					/* aca hay dependencia del '50' ... para cambiar luego */
				}
#endif
			}

			f1      = 1;
			pp_qtr  = 0;



			/* busco siguiente 'bloque' de pipe */
			while (f1 && (read_status = fgets(b1,MAXB,hfinp)) != NULL)
			{
				memset ( &tr_x , 0 , sizeof ( tr_x) );
				csv_parser(b1,fp_trx3);

#if 0
				memcpy ( mtrax[ pp_ptr_bse_car + pp_qtr ],&tr_x,sizeof(trax) );
#endif

				memcpy ( mtrax[ pp_ptr_bse_car +  pp_qtr ],&tr_x,sizeof(trax) );

				if ( mm1 == 1 || (pp_qtr && pp_qtr % mm1 == 0) )
				{
					printf ("%s Leo pipe (Cant trx: %6d) trx nro: %10d (%8d,%s,%s)  %s\n", 
						gp_tm(),
						pp_qtr,
						tr_x.tx_trx_nro,
						tr_x.tx_per_nro,
						tr_x.tx_trx_fecha,
						tr_x.tx_trx_hora,
						" " );
				}

				pp_qtr++;

				if ( pp_qtr == pp_qtr_prx_car )
					f1 = 0;
			}

			printf ("-----\n");

			f_buscar = 0;

#if 0
			pp_qtr = pp_qtr_prx_car;			/* cantidad en ventana */
#endif
			pp_trx = 0;					/* contador */

			pp_ptr_bse_ven = pp_ptr_bse_ven_ant;		/* base de ventana a procesar  */
			pp_ptr_bse_ven_ant = pp_ptr_bse_ven + pp_qtr;	/* base de ventana a procesar  */

			if ( ! f_inversa )
			{	
				pp_ptr = 0;
			}
			if (   f_inversa )
			{
				pp_ptr = pp_qtr - 1;			
			}	


		}
		/* if (f_buscar ...  */



		/* aca hago algo con las transaciones mientras tengo 'pipe' en memoria */
		if (!f_buscar)
		{
			/* En este punto, paso x cada transaccion en el pipeline !!!  */


#if 0
			fprintf (hfout,"%10d %10d %8d %s\n",
				pp_trx,
				(*mtrax[pp_ptr_bse_ven + pp_ptr]).tx_trx_nro,
				(*mtrax[pp_ptr_bse_ven + pp_ptr]).tx_per_nro,
				" ");
#endif
			fprintf (hfout,"%10d %10d %8d %s\n",
				pp_trx,
				(*mtrax[pp_ptr_bse_pro + pp_ptr]).tx_trx_nro,
				(*mtrax[pp_ptr_bse_pro + pp_ptr]).tx_per_nro,
				" ");


#if 0
			if ( mm2 == 1 || (pp_trx && pp_trx % mm2 == 0 )  )
			{
				printf ("%s Proceso  (n_tr: %6d) tr nro: %10d  f/h %s:%s    %s\n",
					gp_tm(),
					pp_trx,
					(*mtrax[pp_ptr_bse_ven + pp_ptr]).tx_trx_nro,
					(*mtrax[pp_ptr_bse_ven + pp_ptr]).tx_trx_fecha,
					(*mtrax[pp_ptr_bse_ven + pp_ptr]).tx_trx_hora,
					" ");
			}
#endif

			if ( mm2 == 1 || (pp_trx && pp_trx % mm2 == 0 )  )
			{
				printf ("%s Proceso  (n_tr: %6d) tr nro: %10d  f/h %s:%s    %s\n",
					gp_tm(),
					pp_trx,
					(*mtrax[pp_ptr_bse_pro + pp_ptr]).tx_trx_nro,
					(*mtrax[pp_ptr_bse_pro + pp_ptr]).tx_trx_fecha,
					(*mtrax[pp_ptr_bse_pro + pp_ptr]).tx_trx_hora,
					" ");
			}


			pp_trx++;

			if ( ! f_inversa )
			{	
				pp_ptr++;
			}
			if (   f_inversa )
			{
				pp_ptr--;
			}	

			if ( pp_trx == pp_qtr || pp_trx == pp_pro )
			{
				f_buscar = 1;
				if (read_status == NULL)
					f_hay_pipe = 0;
			}
		}
		/* if (!f_buscar ... */


	}
	/* while (f_hay_pipe ... */




	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale proceso exec 1 \n\n",gp_tm());
	}
}


#endif


/*
 * -----------------------------------------------------------------------------------
 *
 *	pro_proceso 1 
 *
 *	otros procesos para correr, encadendos a procesos principales etc
 *
 * -----------------------------------------------------------------------------------
 */

/*
 *	Proceso para recorrer ... una a una ... todas las personas,
 *	y obtener todas las transacciones ...
 *
 */


int	pro_proceso1()
{
	char	b1[MAXB];
	char	*read_status;

	double	fs_pos;

	int	i;
	int	fs_err;


	int	per_nro;
	int	per_off;
	int	per_qnt;

	FILE	*hfupd;
	int	ffupd = 1;
	char	fupd[64] = "trx10_oo8.txt";


	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra a proceso proc 1 \n\n",gp_tm());
	}

	/* necesitamos archivo formato  persona,transaccion (idx1) ...  -inp ...  */
	if ( !ffinp)
		error(813);

	/* si no cargo personas  no se puede hacer el proceso ... -cpe  */
	if (!ffcpe)
		error(809);

	/* por ahora ... aca */
	if ( ffupd && ((hfupd = fopen (fupd,"r+")) == NULL) )
	{
		error(131);
	}

	/* settings */
	/* algunos inits */
	for (i=0; i< MAX_TRX; i++)
	{
		mtrax[i] = (traxptr ) malloc(sizeof(trax));
			if (mtrax[i] == NULL )
				error(911); 
	}


	build_idx1(hfinp);


	/* pruebas */
	for (per_nro=0; per_nro< 205400; per_nro++)
	{
		/* aca .... tendria que obtener offset y cantidad ed trx
		 * de cada persona 'i'
		 * y cargar las transacciones en ... cual ??? trnx trax ???
		 */	


		per_off = (*mpers[per_nro]).ps_idx_off ;
		per_qnt = (*mpers[per_nro]).ps_idx_qnt ; 


		for (i=0; i<per_qnt; i++)
		{
#if 0
			printf ("lookup  per: %8d idx: %4d trx: %10d %s\n",
				per_nro,
				i,
				*(obj_ptr + per_off + i),
				" ");
#endif


			fs_pos = (double) ( (double) *(obj_ptr + per_off + i ) * (double) 95 );

			fs_err = fseek(hfupd,  fs_pos  ,SEEK_SET);
			if ( fs_err == -1 )
			{
				printf ("Error en fseek !!! \n");
				printf ("Intento fseek ... %20lf \n",
					(double) *(obj_ptr + per_off + i ) * (double) 95 );
				exit(1);
			}



			read_status = fgets(b1,MAXB,hfupd);
			if (read_status == NULL)
				error(132);



			memset ( &tr_x , 0 , sizeof ( tr_x) );
			csv_parser(b1,fp_trx3);

			memcpy ( mtrax[i],&tr_x,sizeof(trax) );

		}


		printf ("persona: %10d \n",per_nro);

#if 0
		for (i=0; i<per_qnt; i++)
		{
			printf ("per_nro: %8d  i: %4d   per2: %8d 		%s\n",
				per_nro,
				i,
				(*mtrax[i]).tx_per_nro,
				" ");
		}
#endif

				
		printf ("\n");
	}


	/* por ahora ... aca */
	fclose(hfupd);
	

	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de proceso proc 1 \n\n",gp_tm());
	}
}


/*
 * -----------------------------------------------------------------------------------
 *
 *	pro_proceso 2 
 *
 *	otros procesos para correr, encadendos a procesos principales etc
 *
 * -----------------------------------------------------------------------------------
 */

int	pro_proceso2()
{
	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra a proceso proc 2 \n\n",gp_tm());
	}

	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de proceso proc 2 \n\n",gp_tm());
	}
}




/*
 * -----------------------------------------------------------------------------------
 *
 *	pro_prueba 1
 *
 *	pruebas ...
 *
 * -----------------------------------------------------------------------------------
 */

/*
 *	pruebo qsort con trx
 *
 *	imprimo matriz zlmap ...
 *
 */


/*
 *	pruebo qsort con trx
 */


int	pro_pruebas1()
{

	/*
	 * pruebas con const !!
	 */

	int 	*ptr; 					// *ptr is an int value
	int 	const *ptrToConst; 			// *ptrToConst is a constant (int: integer value)
	int 	* const constPtr; 			// constPtr is a constant (int *: integer pointer)
	int 	const * const constPtrToConst; 		// constPtrToConst is a constant (pointer)
                                  			 // as is *constPtrToConst (value)

	char	b1[MAXB];

	int	i,j,k;
	int	q1,q2,q3;

	int	f1,f2;
	int	n1,n2;


	int	i_idx;
	int	i_per;

	int	t_trx_per;		/* total de trx de la persona */

	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra proceso prueba 1 \n\n",gp_tm());
	}
		

	/* necesitamos transacciones indicadas en -inp ...  */
	if ( !ffinp)
		error(813);

	/* necesitamos archivo de salida en -out   ...  */
	if ( !ffout)
		error(814);


	i_idx = 0;
	t_trx_per = 0;


	i_idx = 0;
	t_trx_per = 0;


	q_trx =0;

	bcvs_load("Cargando transacciones \n", hfinp, fx_trx );

	printf ("Cantidad de trx cargadas: %10d\n",q_trx);

	for (i=0; i<q_trx; i++)
	{	
		if ( i % 1000 == 0)
			printf ("%5d: %06d\n",i,(*mtrax[i]).tx_per_nro);
	}


	printf ("Entro a qsort \n");

	/* Ordenar el vector por precio/volumen */
	qsort((void*)mtrax, q_trx , sizeof(traxptr), ordenar_trx);

	printf ("Salgo de qsort \n");

	for (i=0; i<q_trx; i++)
	{	
		if ( i % 1000 == 0)
			printf ("%5d: %06d\n",i,(*mtrax[i]).tx_per_nro);
	}

	for (i=0; i<q_trx; i++)
		graba_trx(hfout,mtrax[i]);

	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale proceso prueba 1 \n\n",gp_tm());
	}
}

int	ordenar_trx (ptr1,ptr2)
const	void	*ptr1;
const	void	*ptr2;
{
	static	int	q = 0;

	int	per1,per2;


#if 0
	printf ("ord_trx1: (%4d)  %8d %8d\n",++q,
		(* (traxptr *) ptr1)->tx_per_nro,
		(* (traxptr *) ptr2)->tx_per_nro
		);

	printf ("ord_trx2: (%4d)  %8d %8d\n",++q,
		(** (traxptr *) ptr1).tx_per_nro,
		(** (traxptr *) ptr2).tx_per_nro
		);
#endif

	per1 = (** (traxptr *) ptr1).tx_per_nro;
	per2 = (** (traxptr *) ptr2).tx_per_nro;

	return (per1 - per2);
}
 




/*
 *	imprimo zlmap
 */

#if 0

int	pro_pruebas1()
{
	int	px,py;


	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra proceso prueba 1 \n\n",gp_tm());
	}
		
	for (px=0; px< MAX_FCM; px++)
		for (py=0; py< MAX_FCM; py++)
			printf ("%3d %3d %8d %8d \n",
				px,
				py,	
				zlmap[px][py][0],
				zlmap[px][py][1]
				);

	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale proceso prueba 1 \n\n",gp_tm());
	}
}

#endif

/*
 * -----------------------------------------------------------------------------------
 *
 *	pro_prueba 2
 *
 *	pruebas ...
 *
 * -----------------------------------------------------------------------------------
 */

int	pro_pruebas2()
{
	int	i,j,k;
	int	f1;

	char	*xx;
	char	b1[MAXB];

	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra proceso prueba 2\n\n",gp_tm());
	}
		
	/* necesitamos transacciones indicadas en -inp ...  */
	if ( !ffinp)
		error(813);

	/* algunos inits */
	for (i=0; i< MAX_TR2; i++)
	{
		mtrax[i] = (traxptr ) malloc(sizeof(trax));
			if (mtrax[i] == NULL )
				error(907); 
	}


	f1 = 1;
	k  = 0;

	while (f1 && (xx = fgets(b1,MAXB,hfinp)) != NULL)
	{
		memset ( &tr_x , 0 , sizeof ( tr_x) );
		csv_parser(b1,fp_trx3);

		printf ("1 k:  %4d  trx nro: %10d %s:%s %s\n",
			k,
			tr_x.tx_trx_nro,
			tr_x.tx_trx_fecha,
			tr_x.tx_trx_hora,
			" ");



		memcpy ( mtrax[k],&tr_x,sizeof(trax) );

		printf ("2 k:  %4d  trx nro: %10d %s:%s %s\n",
			k,
			(*mtrax[k]).tx_trx_nro,
			(*mtrax[k]).tx_trx_fecha,
			(*mtrax[k]).tx_trx_hora,
			" ");

		k++;
		if ( k == 100)
			f1 = 0;
	}

	


	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale proceso prueba 2 \n\n",gp_tm());
	}
}




/*
 *	imprime matriz
 */

#if 0

int	pro_pruebas2()
{
	int	i_den,i_nse,i_tic;


	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra proceso prueba 2\n\n",gp_tm());
	}
		

	for (i_den=0; i_den< 5; i_den++)
		for (i_nse=0; i_nse<3; i_nse++)
			for (i_tic=0; i_tic<3; i_tic++)
			{

				printf ("i_den: %d  i_nse: %d  i_tic: %d  ->  (%2d,%2d) \n",
					i_den,i_nse,i_tic,
					fdmc[i_den][i_nse][i_tic][0],
					fdmc[i_den][i_nse][i_tic][1]
					);
			}


	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale proceso prueba 2 \n\n",gp_tm());
	}
}

#endif


/*
 * -----------------------------------------------------------------------------------
 *
 *	pro_pruebas 3
 *
 *	pruebas ...
 *
 * -----------------------------------------------------------------------------------
 */


#define	MAX_PIP	1000000		/* max pipeline in memory */
int	trx_ptr;
int	trx_get;
int	bse_trx_ptr ;
int	bse_trx_get ;


/*
 *	pipeline de trx !!!
 *
 *	cargamos trx de archivo txt seq simulando 'pipeline'
 *
 *	Se carga un primer bloque de 1M de trx
 *	y luego sigue loop cargando de a 500 k trx cada vez,
 *	de manera de tener siempre como minimo, 500 k trx
 *	en memoria detras de cualquier trx a revisar.
 *
 *	sobre esas 500 k trx, se buscan las ultimas N
 *	de cada cardholder, y con eso se calculan los atributos.
 *
 */

/* pruebas3 ml */

int	pro_pruebas3()
{
	char	b1[MAXB];

	int	k_trx;
	int	n_trx;
	int	p_trx;


	int	f_hay_pipe;
	int	f_mover;
	int	f_buscar;
	int	f_primera;

	int	f1;
	int	mm1,mm2;

	int	gc_trx_ver;		/* cont global trx verdaderas - segun dato entrenamiento */
	int	gc_trx_fra;		/* cont global trx fraudulent - segun dato entrenamiento */

	int	i,j,k;

	char	*xx;

	int	c_espero;

	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra proceso prueba 3 \n\n",gp_tm());
	}
		
	/* necesitamos transacciones indicadas en -inp ...  */
	if ( !ffinp)
		error(813);

	/* necesitamos archivo de salida en -out   ...  */
	if ( !ffout)
		error(814);

	/* algunos inits */
	for (i=0; i< MAX_TR2; i++)
	{
		mtrax[i] = (traxptr ) malloc(sizeof(trax));
			if (mtrax[i] == NULL )
				error(907); 
	}



/* s6 */
#if 1

	bse_trx_ptr = MAX_PIP;
	bse_trx_get = MAX_PIP;

	cl_tt  = 0;
	cl_00  = 0;
	cl_01  = 0;
	cl_10  = 0;
	cl_11  = 0;


	gc_trx_ver = 0;
	gc_trx_fra = 0;

	mm1 = 100000;
	mm2 = 1000;

	p_trx = 0;

	f_hay_pipe = 1;

	f_buscar = 1;

	f_primera = 1;


	/* leo reg x reg del archivo de input */
	while (f_hay_pipe)
	{
		if (f_buscar)
		{
			if (!f_primera)
			{
				f_mover = 1;
				trx_ptr = bse_trx_ptr;
				trx_get = bse_trx_get;
			}

			if (f_primera)
			{
				f_primera = 0;
				f_mover   = 0;
				
				trx_ptr   = 0;
				trx_get   = bse_trx_get * 2;
			}
			
			if (f_mover)
			{
				for (i=0; i<k_trx; i++)
				{
					memcpy ( mtrax[i], mtrax[i+trx_ptr], sizeof(trax));	/* aca decia 500000 */
				}
			}

			k_trx =0;
			f1 = 1;


			/* busco siguiente 'bloque' de pipe */
			while (f1 && (xx = fgets(b1,MAXB,hfinp)) != NULL)
			{
				memset ( &tr_x , 0 , sizeof ( tr_x) );
				csv_parser(b1,fp_trx3);

				memcpy ( mtrax[trx_ptr+k_trx],&tr_x,sizeof(trax) );

				if ( k_trx && k_trx % mm1 == 0)
				{
					printf ("%s Leo pipe (Cant trx: %8d)     trx nro: %10d (%8d,%s,%s)  %s\n", 
						gp_tm(),
						k_trx,
						tr_x.tx_trx_nro,
						tr_x.tx_per_nro,
						tr_x.tx_trx_fecha,
						tr_x.tx_trx_hora,
						" ");
				}

				k_trx++;

				if (k_trx == trx_get)
					f1 = 0;
			}

			printf ("\n\n");


			if (trx_get == bse_trx_get * 2)
				k_trx = bse_trx_get;

			f_buscar = 0;
			n_trx = 0;

			if (xx == NULL)
				f_hay_pipe = 0;

		}
		/* if (f_buscar ...  */



		/* aca hago algo con las transaciones mientras tengo 'pipe' en memoria */
		if (!f_buscar)
		{
			/* En este punto, paso x cada transaccion en el pipeline !!!  */


#if 1
			calcular_atrib_trx( n_trx );
#endif

			fprintf (hfout,"%10d (%8d) %10d %8d %s\n",
				p_trx,
				n_trx,
				(*mtrax[n_trx]).tx_trx_nro,
				(*mtrax[n_trx]).tx_per_nro,
				" ");

			p_trx++;


#if 1
			if ( (*mtrax[n_trx]).tx_cal_n2 == 0)
				gc_trx_ver++;

			if ( (*mtrax[n_trx]).tx_cal_n2 == 1)
				gc_trx_fra++;

			if ( n_trx && n_trx % mm2 == 0 )
			{
				printf ("%s Proceso  (n_tr: %6d) tr nro: %10d  f/h %s:%s (%8d/%8d/%8d)     %s\n",
					gp_tm(),
					n_trx,
					(*mtrax[n_trx]).tx_trx_nro,
					(*mtrax[n_trx]).tx_trx_fecha,
					(*mtrax[n_trx]).tx_trx_hora,
					p_trx,gc_trx_ver,gc_trx_fra,
					" ");
			}
#endif

			n_trx++;

			if ( n_trx == k_trx)
			{
				f_buscar = 1;
			}
		}
		/* if (!f_buscar ... */


	}
	/* while (f_hay_pipe ... */

#endif
/* s6 */




	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale proceso prueba 3 \n\n",gp_tm());
	}
}



/*
 *	calcular_atrib_trx
 *
 *	En base al 'universo' observado (conjunto de N ult trx de c/ persona )
 *	calcular Nq atrib de observacion
 *	llamar al modelo (caja negra / algoritmo ) -> hallar clasificador 
 */


int	calcular_atrib_trx ( n_trx )
int	n_trx;
{
	int	at1,at2,at3,at4,at5,at6,at7,at8,at9;
	int	w1,w2;

	int	i,j,k;
	int	n_per;
	int	n_des;

	int	f1,f2;
	int	n_max;
	int	i_trx;

	int	n_cal;
	int	cl,n_cal2;

	int	q_xx;

	static	int	f_override = 0;

	static	int	c_espero = 1000;
	static	int	f_espero = 1;
	static	char 	*z;

	n_max = 500000;	/* todavia no me queda clase cual de las trx_ptr o trx_get es */
	n_des = 10;


	n_per = (*mtrax[n_trx]).tx_per_nro;
	n_cal = (*mtrax[n_trx]).tx_cal_n2;
	
	
	f1 = 1;
	i_trx = 0;
	i=0;

	/* busco transacciones de la misma persona */
	while (f1)
	{
		if ( n_per == (*mtrax[n_trx+i]).tx_per_nro )
		{
			gt_trx[i_trx] = n_trx+i;
			i_trx++;
		}

		i++;

		/* 
		 * condiciones ...
		 * Si encontre nro deseado de trx de la persona ...
 		 * Si recorri todo el pipeline en memoria 
		 *
		 */

		if ( i_trx == n_des)
			f1 = 0;

		if ( i == n_max )
			f1 = 0;

	}

		
	/* sobre las encontradas ...
	 * - calcular atributos
	 * - consultar la 'cajita negra'
	 * - devolver resultados (acumular en algun lugar )
	 */

	w1 = 0;
	w2 = i_trx;

	at1 = calculo_trx_at1(w1,w2);
	at2 = calculo_trx_at2(w1,w2);	
	at3 = calculo_trx_at3(w1,w2);	
	at4 = calculo_trx_at4(w1,w2);	
#if 0
	at5 = calculo_trx_at5(w1,w2);	
	at6 = calculo_trx_at6(w1,w2);	
	at7 = calculo_trx_at7(w1,w2);	
	at8 = calculo_trx_at8(w1,w2);	
	at9 = calculo_trx_at9(w1,w2);	
#endif
			
	gt_at[0] = at1;
	gt_at[1] = at2;
	gt_at[2] = at3;
	gt_at[3] = at4;
	q_xx = 4;

	clasificador = call_algoritmo_ml(q_xx);
	cl = clasificador;


	cl_tt++;

	if ( n_cal == 0 && clasificador == 0)
		cl_00++;
	if ( n_cal == 0 && clasificador == 1)
		cl_01++;
	if ( n_cal == 1 && clasificador == 0)
		cl_10++;
	if ( n_cal == 1 && clasificador == 1)
		cl_11++;
	


	n_cal2 = (*mtrax[gt_trx[0]]).tx_cal_n2;


	/* despliega info x pantalla si eligio avanzar una x una */

	if (gp_pause)
	{

	clear_screen();

	printf ("\n");
	printf ("Trx  Nro :   %10d\n", n_trx);
	printf ("Tarj Nro :   %10d\n", n_per);
	printf ("Cant trx :   %10d\n", i_trx);
	printf ("Fraude?  :   %10s\n", n_cal2 ? "SI":"NO");
	printf ("\n");

	printf ("Atrib 1  :   %10d\n",at1);
	printf ("Atrib 2  :   %10d\n",at2);
	printf ("Atrib 3  :   %10d\n",at3);
	printf ("Atrib 4  :   %10d\n",at4);
	printf ("Clasif f :   %10s (es : %s) %s \n", cl ? "SI":"NO" , n_cal2 ? "SI":"NO" , abs(cl - n_cal2) ? "!!!!":" " );
	printf ("\n");

	printf ("                                                                         La Trx es Fraud ? \n");
	printf ("                                                                         NO        SI    \n");

	printf ("Cant trx :   %10d                                                                      y fue clasificada ... \n",cl_tt);
	printf ("Cant 0/0 :   %10d   (Trx fraud NO clasif como NO )             | %8d | %8d | veces como NO \n",cl_00, cl_00,cl_10);
	printf ("Cant 1/1 :   %10d   (Trx fraud SI clasif como SI )             | %8d | %8d | veces como SI \n",cl_11, cl_01,cl_11);
	printf ("Cant E1  :   %10d   (Trx fraud NO clasif como SI ) \n",cl_01);
	printf ("Cant E2  :   %10d   (Trx fraud SI clasif como NO ) \n",cl_10);
	printf ("\n");

	for (i=0; i<i_trx; i++)
	{
		printf ("trx      : %3d %6d %10d  %s %s %8d (%d) %s\n",
			i,
			(*mtrax[gt_trx[i]]).tx_per_nro,
			(*mtrax[gt_trx[i]]).tx_trx_nro,
			gsf( (*mtrax[gt_trx[i]]).tx_trx_fecha ),
			gsh( (*mtrax[gt_trx[i]]).tx_trx_hora ),
			(*mtrax[gt_trx[i]]).tx_com_nro,
			(*mtrax[gt_trx[i]]).tx_cal_n2,
			" ");
	}
	printf ("\n\n");


	/* eventos en los que quiero parar para que se vea !!! */
	if ( !f_override)
	{

	if ( abs(cl - n_cal2) )
		f_espero = 1;

	if ( n_cal2 == 1 )
		f_espero = 1;

	if ( n_cal2 == 1 && cl == 1)
		f_espero = 1;

	}

	if (f_espero)
	{	z=espero_enter(1);
		if ( *z == '1')
		{
			f_espero = 0;
			c_espero = 10;
		}
		if ( *z == '2')
		{
			f_espero = 0;
			c_espero = 100;
		}
		if ( *z == '3')
		{
			f_espero = 0;
			c_espero = 1000;
		}
		if ( *z == '9')
		{
			f_override = 1;
			f_espero   = 0;
		}
	}
	else
	{
		c_espero--;
		if (!c_espero)
			f_espero = 1;	
	}


	}
	/* if (gp_pause ... */		
	

	/* si no eligio pausa ... imprimo tipo stream */
	if ( !gp_pause)
	{

	printf ("Trx  Nro :   %10d  ", n_trx);
	printf ("Tarj Nro :   %10d  ", n_per);
	printf ("Cant trx :   %10d  ", i_trx);
	printf ("Fraude?  :   %10s  ", n_cal2 ? "SI":"NO");
	printf ("\n");

	printf ("Atrib 1  :   %10d  ",at1);
	printf ("Atrib 2  :   %10d  ",at2);
	printf ("Atrib 3  :   %10d  ",at3);
	printf ("Atrib 4  :   %10d  ",at4);
	printf ("Clasif f :   %10s (es : %s) %s ", cl ? "SI":"NO" , n_cal2 ? "SI":"NO" , abs(cl - n_cal2) ? "!!!!":" " );
	printf ("\n");

	for (i=0; i<i_trx; i++)
	{
		printf ("trx      : %3d %6d %10d  %s %s %8d (%d) %s\n",
			i,
			(*mtrax[gt_trx[i]]).tx_per_nro,
			(*mtrax[gt_trx[i]]).tx_trx_nro,
			gsf( (*mtrax[gt_trx[i]]).tx_trx_fecha ),
			gsh( (*mtrax[gt_trx[i]]).tx_trx_hora ),
			(*mtrax[gt_trx[i]]).tx_com_nro,
			(*mtrax[gt_trx[i]]).tx_cal_n2,
			" ");
	}
	printf ("\n\n");





	}
	/* if (!gp_pause ... */




}
/* pruebas3 ml */


int	call_algoritmo_ml(q)
int	q;
{
	/* q es cantidad de atributos en gt_at ... */
	/*
	 * aca va el 'algoritmo' de ML que resulta
	 * de haber entrenado con todo el dataset !!!!
	 */

	/*
	 * version reducida pra la POC
	 * clasificador
	 * 0 - no fraude
	 * 1 - fraude
	 *
	 */

	int	at1,at2,at3,at4;
	int	clasificador;


	at1 = gt_at[0];
	at2 = gt_at[1];
	at3 = gt_at[2];
	at4 = gt_at[3];


	if ( q != 4)
		error( 606 );

	clasificador = 0;


	/* arbol ID3 recortado ... */

	if ( at1 <= 2 && at2 < 2000 && at4 >= 6 && ( at4 >= 3 && at4 <=  6 ) && (at3 >=  4300 && at3 <=  8700 )  )
		clasificador = 1;

	if ( at1 <= 2 && at2 < 2000 && at4 >= 6 && ( at4 >= 6 && at4 <=  9 ) && (at3 >=  8700 && at3 <= 12300 )  )
		clasificador = 1;

	if ( at1 <= 2 && at2 < 2000 && at4 >= 6 && ( at4 >= 8 && at4 <= 10 ) && (at3 >= 12300 && at3 <= 18500 )  )
		clasificador = 1;


	return clasificador;

}


/*
 * -----------------------------------------------------------------------------------
 *
 *	pro_pruebas 3 ext  - calculo de atributos para suministrar al 'algoritmo'
 *
 *	pruebas ...
 *
 * -----------------------------------------------------------------------------------
 */






/*
 *	calculo sencillo de algunos atributos ...
 *	aprox. tomando en cuenta
 *	- todas las fechas son 2018
 *	- un dia mas o menos no importa (x los meses )
 */



/*
 *	at1
 *	me fijo si las transacciones son mas o menos
 *	conscutivas en fechas
 */

int	calculo_trx_at1(f1,f2)
int	f1;
int	f2;
{
	char	s[MAXV];

	int	i,j,k;
	int	q;
	int	a1,a2,m1,m2,d1,d2;
	
	q=0;

	for (i=f1; i<=f2-1; i++)
	{	

		memset (s,0,16);
		strncpy(s,(*mtrax[gt_trx[i]]).tx_trx_fecha,4); 
		a1 = atoi(s);
	
		memset (s,0,16);
		strncpy(s,(*mtrax[gt_trx[i]]).tx_trx_fecha+4,2);
		m1 = atoi(s);
	
		memset (s,0,16);
		strncpy(s,(*mtrax[gt_trx[i]]).tx_trx_fecha+6,2);
		d1 = atoi(s);


		memset (s,0,16);
		strncpy(s,(*mtrax[gt_trx[i+1]]).tx_trx_fecha,4); 
		a2 = atoi(s);
	
		memset (s,0,16);
		strncpy(s,(*mtrax[gt_trx[i+1]]).tx_trx_fecha+4,2);
		m2 = atoi(s);
	
		memset (s,0,16);
		strncpy(s,(*mtrax[gt_trx[i+1]]).tx_trx_fecha+6,2);
		d2 = atoi(s);


		q +=  abs (  (m1 * 30 + d1 ) - (m2 * 30 + d2 ) );
	}

	return q;
}


/*
 *	at2
 *	me fijo si las transacciones son mas o menos
 *	conscutivas en horas
 */

int	calculo_trx_at2(f1,f2)
int	f1;
int	f2;
{
	char	hora[16];

	int	h1,m1,h2,m2;
	int	q;

	char	b[16];

	int	i,j;

	q = 0;

	for (i=f1; i<=f2-1; i++)
	{
		memset (b,0,16);
		strncpy(b,(*mtrax[gt_trx[i]]).tx_trx_hora,2);
		h1 = atoi(b);
	
		memset (b,0,16);
		strncpy(b,(*mtrax[gt_trx[i]]).tx_trx_hora+2,2);
		m1 = atoi(b);
	
		memset (b,0,16);
		strncpy(b,(*mtrax[gt_trx[i+1]]).tx_trx_hora,2);
		h2 = atoi(b);
	
		memset (b,0,16);
		strncpy(b,(*mtrax[gt_trx[i+1]]).tx_trx_hora+2,2);
		m2 = atoi(b);

		q += abs ( ( h1 * 60 + m1 ) - ( h2 * 60 + m2 ) );
	
	}

	return q;		

}

/*
 *	at3
 *	me fijo si las transacciones son mas o menos
 *	en negocios cercanos !! (este vale oro )
 */

int	calculo_trx_at3(f1,f2)
int	f1;
int	f2;
{
	int	q;
	int	i;

	q = 0;

	for (i=f1; i<=f2-1; i++)
	{
		q += abs ( (*mtrax[gt_trx[i]]).tx_com_px - (*mtrax[gt_trx[i+1]]).tx_com_px ) + 
		     abs ( (*mtrax[gt_trx[i]]).tx_com_py - (*mtrax[gt_trx[i+1]]).tx_com_py ) ; 
	}

	return q;
}




/*
 *	at4
 *
 */

int	calculo_trx_at4(f1,f2)
int	f1;
int	f2;
{
	int	q;
	int	i;
	int	r,a;


	q = f2 - f1;

	return q;
}



/*
 *	at5
 *	me fijo si nro de la trx es par o impar
 *	(este de relleno )
 */

int	calculo_trx_at5(f1,f2)
int	f1;
int	f2;
{
	int	q;
	int	i;
	int	r,a;

	q = (*mtrnx[f1]).tx_trx_nro % 2;

	return q;
}



/*
 *	at6
 *	me fijo si la suma de los montos es 'alto'
 *	(este de relleno )
 */

int	calculo_trx_at6(f1,f2)
int	f1;
int	f2;
{
	int	q;
	int	i;
	int	r,a;

	a = 0;


	for (i=f1; i<=f2-1; i++)
	{
		a += (*mtrnx[i]).tx_trx_monto ;
	}

	q = 0;
	if ( a > 15000 )
		q = 1;

	return q;
}



/*
 *	at7
 *	numero al azar (ruido)
 *	(este de relleno )
 */

int	calculo_trx_at7(f1,f2)
int	f1;
int	f2;
{
	int	q;
	int	i;
	int	r,a;

	q = gna(f1,f2);

	return q;
}


/*
 *	at8
 *	numero al azar (ruido)
 *	(este de relleno )
 */

int	calculo_trx_at8(f1,f2)
int	f1;
int	f2;
{
	int	q;
	int	i;
	int	r,a;

	q = gna(f1,f2);

	return q;
}


/*
 *	at9
 *	numero al azar (ruido)
 *	(este de relleno )
 */

int	calculo_trx_at9(f1,f2)
int	f1;
int	f2;
{
	int	q;
	int	i;
	int	r,a;

	q = gna(f1,f2);

	return q;
}






















/*
 * -----------------------------------------------------------------------------------
 *
 *	pro_pruebas 4
 *
 *	pruebas ...
 *
 * -----------------------------------------------------------------------------------
 */

int	pro_pruebas4()
{
	int	i,j,k;
	int	q1,q2;
	int	f1,f2;
	int	n1,n2;

	int	c_nro,c_px,c_py;

	int	tb[2000];

	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra proceso prueba 4 \n\n",gp_tm());
	}
		

	if ( !ffcom)
		error(810);


	printf ("Cantidad de comercios cargados ... %d \n\n",q_com);
	

	/* por cada comercio .... */
	for (i=0; i<q_com; i++)
	{

		c_nro = (*mcome[i]).ncom;
		c_px  = (*mcome[i]).px;
		c_py  = (*mcome[i]).py;


		for (j=0; j<q_com; j++)
		{
			n1 = abs ( (*mcome[j]).px - c_px ) + abs ( (*mcome[j]).py - c_py );
			tb[j] = n1;
	
			printf ("%6d com %6d (%4d %4d) - com %6d (%4d %4d)\n",
				tb[j],
				c_nro,c_px,c_py,
				(*mcome[j]).ncom,
				(*mcome[j]).px,
				(*mcome[j]).py
				);
		}
	

	}



	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale proceso prueba 4 \n\n",gp_tm());
	}
}



/*
 * -----------------------------------------------------------------------------------
 *
 *	pro_pruebas 5
 *
 *	pruebas ...
 *
 * -----------------------------------------------------------------------------------
 */


#if 1

int	pro_pruebas5()
{
	int	i,j,k;
	int	f1;
	int	v1,v2;

	char	b1[MAXB];
	char	*read_status;

	int	per,per_ant;
	int	f_primera;
	int	t_offset;


	int	mm1;


	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra proceso prueba 5 \n\n",gp_tm());
	}

	/* necesitamos archivo formato  persona,transaccion (idx1) ...  -inp ...  */
	if ( !ffinp)
		error(813);

	/* si no cargo personas  no se puede hacer el proceso ... -cpe  */
	if (!ffcpe)
		error(809);

	obj_ptr = (int *) malloc ( MAX_OBJ * sizeof(int) );
	if (obj_ptr == NULL)
		error(909);

	
	/* leo registros */

	mm1 = 1000000;

	f_primera =  1;
	per_ant   = -1;

	f1 = 1;

	q_idx1 = 0;

	while (f1)
	{
 		read_status = fgets(b1,MAXB,hfinp);

		memset ( &s_idx1 , 0 , sizeof ( s_idx1) );
		csv_parser(b1,fp_idx1);

		*(obj_ptr + q_idx1) = s_idx1.ix_trx;


		if ( per_ant == s_idx1.ix_nro )
		{
			v2++;
		}
		
		if ( per_ant != s_idx1.ix_nro || read_status == NULL )
		{
			if ( f_primera != 1)
			{
				if (gp_fverbose("d3"))
				{	
					printf ("Grabo: q_idx: %10d  per: %10d v1: %10d off: %10d q_trx: %6d %s\n",
						q_idx1,
						per_ant,
						v1,
						t_offset,
						v2,
						" ");
				}

				
				(*mpers[per_ant]).ps_idx_off = t_offset;
				(*mpers[per_ant]).ps_idx_qnt = v2;


				t_offset += v2;
				per_ant = s_idx1.ix_nro;
				v1 = per_ant;
				v2 = 1;
			}
	
			if ( f_primera == 1)
			{
				f_primera = 0;

				t_offset  = 0;
				per_ant = s_idx1.ix_nro;
				v1 = per_ant;
				v2 = 1;
			}
		}

		q_idx1++;


		if (read_status == NULL)
			f1 = 0;
	}



	if (gp_fverbose("d2"))
	{	
		printf ("Cantidad de transacciones ... %10d\n",q_idx1);
	}


	if (gp_fverbose("d3"))
	{	

		/* ejemplo con la persona 3 */
		k = 3;
	
		for (i = 0; i< (*mpers[k]).ps_idx_qnt; i++)
		{	
			printf ("pers: %10d  trx_idx: %4d  trx_nro: %10d %s\n",
				k,
				i,
				*(obj_ptr + (*mpers[k]).ps_idx_off + i),
				" ");
		}
	}



	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale proceso prueba 5 \n\n",gp_tm());
	}

}

#endif


/* define XX_PER */
#if	XX_PER


int	pro_pruebas5()
{
	char	b1[MAXB];

	int	i,j,k;
	int	q1,q2,q3;

	int	f1,f2;
	int	n1,n2;


	int	i_idx;
	int	i_per;

	int	t_trx_per;		/* total de trx de la persona */

	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra proceso prueba 5 \n\n",gp_tm());
	}
		

	/* necesitamos transacciones indicadas en -inp ...  */
	if ( !ffinp)
		error(813);

	/* necesitamos archivo de salida en -out   ...  */
	if ( !ffout)
		error(814);


	i_idx = 0;
	i_per = atoi( xx_per[0]); 
	t_trx_per = 0;

	for (i=0; i<5000; i++ )
	{

	i_per = atoi( xx_per[i]); 
	i_idx = 0;
	t_trx_per = 0;

	printf ("Persona: %10d\n",i_per);

	rewind(hfinp);


	/* leo reg x reg del archivo de input */
	while (fgets(b1,MAXB,hfinp) != NULL)
	{

		if ( i_idx % 10000000 == 0)
		{
			printf ("i_idx: %10d (%10d)\n",i_idx,i_per);

			fprintf(hfout,"persona: %10d\n",i_per);
		}


		memset ( &tr_s , 0 , sizeof ( tr_s) );

		csv_parser(b1,fp_trx);

		if ( tr_s.tx_per_nro == i_per)
		{
			printf ("%10d %10d %s %s %d %d\n",
				i_idx,i_per,
				tr_s.tx_trx_fecha,
				tr_s.tx_trx_hora,
				tr_s.tx_cal_n2,
				tr_s.tx_cal_n1
				);

			fprintf (hfout,"%10d,%10d,%s,%s,%d,%d\n",
				i_idx,i_per,
				tr_s.tx_trx_fecha,
				tr_s.tx_trx_hora,
				tr_s.tx_cal_n2,
				tr_s.tx_cal_n1
				);
				
			fflush(hfout);
		
			t_trx_per++;
		}

		i_idx++;
	}


	}

	printf ("Total de transaccions    %10d\n",i_idx);
	printf ("Persona nro              %10d\n",i_per);
	printf ("Cant de trx de la per    %10d\n",t_trx_per);
	
	printf ("\n");




	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale proceso prueba 5 \n\n",gp_tm());
	}
}


#endif
/* define XX_PER */


/*
 * -----------------------------------------------------------------------------------
 *
 *	pro_pruebas 6
 *
 *	pruebas ...
 *
 * -----------------------------------------------------------------------------------
 */

/*
 *	Al generar las transacciones 'buenas' por un lado, y las 'truchas' por otro
 *	se dan situaciones en donde, al ordenar todo por fecha, quedan algunas
 *	trx 'buenas' intercaladas entre las 'malas' y necesitamos que estas esten
 *	todas contiguas
 *
 *	Este proceso sive para leer un archivo de trx, ordenado por usuario 
 *	y por fecha , ir cargando en memoria todas las trx de un usuario
 *	y tenerlas disponibles para cualquier arreglo que haya que hacer.
 *	
 *	tambien seria la base de cualquier proceso (weka por ejemplo),
 *	que necesite generar un analis de 'las ultimas N trx ' de un usuario
 *
 */


int	pro_pruebas6()
{
	char	b1[MAXB];
	char	b2[MAXB];

	int	tnv[10];		/* tabla 'no van' */

	int	i,j,k;
	int	i1,i2,i3;
	int	j1,j2,j3;
	int	q0,q1,q2,q3,q7;

	int	f1,f2,f3,f4,f5,f6,f7,f8;
	int	n1,n2;

	int	flag;

	char	*x;


	int	k_tru;

	int	i_idx;
	int	i_per;

	int	t_trx_per;		/* total de trx de la persona */
	int	i_per_ant;		/* persona anterior */


	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra proceso prueba 6 \n\n",gp_tm());
	}

	/* necesitamos transacciones indicadas en -inp ...  */
	if ( !ffinp)
		error(813);

	/* necesitamos archivo de output -out ...  */
	if ( !ffout)
		error(814);



	/* algunos inits */
	for (i=0; i< MAX_TRX; i++)
	{
		mtrnx[i] = (trnxptr ) malloc(sizeof(trnx));
			if (mtrnx[i] == NULL )
				error(906);
	}


	k_tru =0;



	i_idx      =  0;
	i_per_ant  = -1;
	flag       =  1;
	f4         =  0;


	/* leo reg x reg del archivo de input */
	while (flag)
	{
		x = fgets(b1,MAXB,hfinp);

		if ( x == NULL )
		{	

			f4 = 1;
			flag = 0;
		}

		if ( x != NULL)
		{
			memset ( &tr_s , 0 , sizeof ( tr_s) );
			csv_parser(b1,fp_trx);
			i_per = tr_s.tx_per_nro;


			if ( i_per_ant == i_per )
			{
				memcpy ( mtrnx[i_idx], &tr_s, sizeof (trnx) );
				i_idx++;
				if ( i_idx == MAX_TRX )
					error( 315 );

				f4 = 0;
			}

			if ( i_per_ant != i_per  )
			{

				if ( i_per_ant == -1 )
				{
					f4 = 0;
				}

				if ( i_per_ant != -1 )
				{
					f4 = 1;
				}

			}
		}

		/* hago la revision */
		if (f4)
		{


			/* tengo que ordenarlos en la memoria !!!!! */
			/* 
			 * vector de punteros a estructuras ...
			 *
			 */

			q0 = i_idx;

			for (i1=0; i1<q0-1; i1++)
				for (j1=i1+1; j1<q0; j1++)
					if ( compara ( mtrnx[j1],mtrnx[i1] ) > 0 ) 
					{
						mtrnx[q0] = mtrnx[j1];
						mtrnx[j1] = mtrnx[i1];
						mtrnx[i1] = mtrnx[q0];
					}

#if 1
			q0 = i_idx;
			printf ("A ver como quedo ??? \n");
			for (i1 = 0; i1 < q0; i1++)
			{
				printf ("%s %s %10d , %10d %d %d\n",
					(*mtrnx[i1]).tx_trx_fecha,
					(*mtrnx[i1]).tx_trx_hora,
					(*mtrnx[i1]).tx_trx_nro,
					(*mtrnx[i1]).tx_per_nro,
					(*mtrnx[i1]).tx_cal_n1,
					(*mtrnx[i1]).tx_cal_n2
					);

			}

			printf ("\n\n");

#endif

			/* verifico tiene trx con fraude */
			f5 = 0;
			for (i=0; i< i_idx; i++)
			{
				if ( (*mtrnx[i]).tx_cal_n2 == 1)
					f5++;
			}

			
			
			/* verifico si todas las truchas estan seguidas !! */
			i  = 0;
			f6 = 1;
			f7 = 0;		/* asumo que estan todas consecutivas */
			f8 = 1;
			q7 = 0;


			for (j=0; f6 && j< i_idx; j++)
			{
				if ( (*mtrnx[i+j]).tx_cal_n2 == 1 )
				{
					k=0;

					while (f8)
					{
						if ( (*mtrnx[i+j+k]).tx_cal_n2 == 1  )
							q7++;
						if ( (*mtrnx[i+j+k]).tx_cal_n2 == 0  )
						{	tnv[f7] = i+j+k;
							f7++;
						}

						if (q7 == f5)
							f8=0;
						k++;
						if (k >100)
						{
							error(603);  
						}

					}
				}

			}

		
			/* prueba */
			if (gp_fverbose("d1"))
			{	printf ("Rev: per %10d con %10d regs (%10d fra) mod? %d  \n",i_per_ant,i_idx,f5,f7);
			}


			/* aca tendria que generar output ... sin las trx intercaladas con las truchas */
			for (i=0; i< i_idx; i++)
			{
				f1 = 1;
				for (j=0; j<f7; j++)
				{
					if ( i == tnv[j] )
						f1 = 0;
				}

				if (f1)
				{
					if (ffout)
					{
						if ( (*mtrnx[i]).tx_cal_n2 == 1)
							k_tru++;

	fprintf (hfout,"%s,%s,%010d,%06d,%02d,%02d,%02d,%d,%d,%06d,%04d,%04d,%d,%d,%03d,%08d,%02d,%02d,%d,%d,%d,%d\n",   
						(*mtrnx[i]).tx_trx_fecha,
						(*mtrnx[i]).tx_trx_hora,
						(*mtrnx[i]).tx_trx_nro,		
						(*mtrnx[i]).tx_trx_monto,
						(*mtrnx[i]).tx_art_desc,
						(*mtrnx[i]).tx_zon_px,
						(*mtrnx[i]).tx_zon_py,
						(*mtrnx[i]).tx_zon_iden,
						(*mtrnx[i]).tx_zon_inse,
						(*mtrnx[i]).tx_com_nro,
						(*mtrnx[i]).tx_com_px,
						(*mtrnx[i]).tx_com_py,
						(*mtrnx[i]).tx_com_inse,	
						(*mtrnx[i]).tx_com_itic,	
						(*mtrnx[i]).tx_com_nrub,
						(*mtrnx[i]).tx_per_nro,	
						(*mtrnx[i]).tx_per_px,	
						(*mtrnx[i]).tx_per_py,
						(*mtrnx[i]).tx_per_ifam,	
						(*mtrnx[i]).tx_per_inse,	
						(*mtrnx[i]).tx_cal_n1,	
						(*mtrnx[i]).tx_cal_n2 
						);
					}
					/* if (ffout ... */

				}
			}


			if (ffaux)
			{
				if (f5)
					fprintf (hfaux,"FRA: per %10d con %10d regs (%10d fra) mod? %d  \n",i_per_ant,i_idx,f5,f7);

				if (f7)
					fprintf (hfaux,"MOD: per %10d con %10d regs (%10d fra) mod? %d  \n",i_per_ant,i_idx,f5,f7);

				fprintf (hfaux,"REV: per %10d con %10d regs (%10d fra) mod? %d  \n",i_per_ant,i_idx,f5,f7);

			}


			printf ("Voy por persona                                          ... %10d y %10d fraud .. grabadas \n",
				i_per_ant,k_tru );
			
		}
		/* if (f4 ... */


		if ( i_per_ant != i_per )
		{
			memcpy ( mtrnx[0], &tr_s, sizeof (trnx) );
			i_per_ant = i_per;
			i_idx =  1;
		}

	}




	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de proceso prueba 6 \n\n",gp_tm());
	}
}




int	compara( t1,t2)
trnxptr	t1;
trnxptr t2;
{
	char	b1[MAXB];
	char	b2[MAXB];

	sprintf (b1,"%s%s", (*t1).tx_trx_fecha , (*t1).tx_trx_hora );
	sprintf (b2,"%s%s", (*t2).tx_trx_fecha , (*t2).tx_trx_hora );

	return (strncmp(b1,b2,14));
}
		






/*
 * -----------------------------------------------------------------------------------
 *
 *	pro_pruebas 7
 *
 *	pruebas ... usar este para archivos 'nro_per,fecha,hora ...' de may a men
 *
 * -----------------------------------------------------------------------------------
 */

/*
 *	Al generar las transacciones 'buenas' por un lado, y las 'truchas' por otro
 *	se dan situaciones en donde, al ordenar todo por fecha, quedan algunas
 *	trx 'buenas' intercaladas entre las 'malas' y necesitamos que estas esten
 *	todas contiguas
 *
 *	Este proceso sive para leer un archivo de trx, en formato
 *	nro_per,fecha,hora, ... trx ...
 *	ya ordenado de mayor a menor con el sort....
 *
 *	 
 *	Carga en memoria todas las trx de un usuario
 *	y las tiene disponibles para cualquier arreglo que haya que hacer.
 *
 *	En este caso, elimina las trx genuinas intercaladas entre las verdaderas
 *
 *	
 *	tambien seria la base de cualquier proceso (weka por ejemplo),
 *	que necesite generar un analis de 'las ultimas N trx ' de un usuario
 *
 */


int	pro_pruebas7()
{
	char	b1[MAXB];
	char	b2[MAXB];
	char	mdfb[8];

	int	tnv[10];		/* tabla 'no van' */

	int	i,j,k;
	int	i1,i2,i3;
	int	j1,j2,j3;
	int	q0,q1,q2,q3,q7;

	int	f1,f2,f3,f4,f5,f6,f7,f8;
	int	n1,n2;

	int	flag;

	char	*x;


	int	k_tru;

	int	i_idx;
	int	i_per;

	int	t_trx_per;		/* total de trx de la persona */
	int	i_per_ant;		/* persona anterior */


	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra proceso prueba 6 \n\n",gp_tm());
	}

	/* necesitamos transacciones indicadas en -inp ...  */
	if ( !ffinp)
		error(813);

	/* necesitamos archivo de output -out ...  */
	if ( !ffout)
		error(814);



	/* algunos inits */
	for (i=0; i< MAX_TRX; i++)
	{
		mtrnx[i] = (trnxptr ) malloc(sizeof(trnx));
			if (mtrnx[i] == NULL )
				error(906);
	}


	k_tru =0;



	i_idx      =  0;
	i_per_ant  = -1;
	flag       =  1;
	f4         =  0;


	/* leo reg x reg del archivo de input */
	while (flag)
	{
		x = fgets(b1,MAXB,hfinp);

		if ( x == NULL )
		{	

			f4 = 1;
			flag = 0;
		}

		if ( x != NULL)
		{
			memset ( &tr_s , 0 , sizeof ( tr_s) );
			csv_parser(b1,fp_trx2);
			i_per = tr_s.tx_per_nro;


			if ( i_per_ant == i_per )
			{
				memcpy ( mtrnx[i_idx], &tr_s, sizeof (trnx) );
				i_idx++;
				if ( i_idx == MAX_TRX )
					error( 315 );

				f4 = 0;
			}

			if ( i_per_ant != i_per  )
			{

				if ( i_per_ant == -1 )
				{
					f4 = 0;
				}

				if ( i_per_ant != -1 )
				{
					f4 = 1;
				}

			}
		}

		/* hago la revision */
		if (f4)
		{

			q0 = i_idx;

#if 0
			/* en esta version NO hay que ordenar, ya paso por el sort */
			/* tengo que ordenarlos en la memoria !!!!! */
			/* 
			 * vector de punteros a estructuras ...
			 *
			 */

			q0 = i_idx;

			for (i1=0; i1<q0-1; i1++)
				for (j1=i1+1; j1<q0; j1++)
					if ( compara ( mtrnx[j1],mtrnx[i1] ) > 0 ) 
					{
						mtrnx[q0] = mtrnx[j1];
						mtrnx[j1] = mtrnx[i1];
						mtrnx[i1] = mtrnx[q0];
					}

#endif

#if 0
			q0 = i_idx;

			printf ("A ver como quedo ??? \n");
			for (i1 = 0; i1 < q0; i1++)
			{
				printf ("%s %s %10d , %10d %d %d\n",
					(*mtrnx[i1]).tx_trx_fecha,
					(*mtrnx[i1]).tx_trx_hora,
					(*mtrnx[i1]).tx_trx_nro,
					(*mtrnx[i1]).tx_per_nro,
					(*mtrnx[i1]).tx_cal_n1,
					(*mtrnx[i1]).tx_cal_n2
					);

			}

			printf ("\n\n");

#endif

			/* verifico tiene trx con fraude */
			f5 = 0;
			for (i=0; i< i_idx; i++)
			{
				if ( (*mtrnx[i]).tx_cal_n2 == 1)
					f5++;
			}

		

/* corto f4 */
#if 1
			
			/* verifico si todas las truchas estan seguidas !! */
			i  = 0;
			f6 = 1;
			f7 = 0;		/* asumo que estan todas consecutivas */
			f8 = 1;
			q7 = 0;


			for (j=0; f6 && j< i_idx; j++)
			{
				if ( (*mtrnx[i+j]).tx_cal_n2 == 1 )
				{
					k=0;

					while (f8)
					{
						if ( (*mtrnx[i+j+k]).tx_cal_n2 == 1  )
							q7++;
						if ( (*mtrnx[i+j+k]).tx_cal_n2 == 0  )
						{	tnv[f7] = i+j+k;
							f7++;
						}

						if (q7 == f5)
							f8=0;
						k++;
						if (k >100)
						{
							error(603);  
						}

					}
				}

			}


/* corto f4 */
#endif  

/* corto f4 */
#if 1

			/* aca tendria que generar output ... sin las trx intercaladas con las truchas */
			for (i=0; i< i_idx; i++)
			{
				f1 = 1;
				for (j=0; j<f7; j++)
				{
					if ( i == tnv[j] )
						f1 = 0;
				}

				if (f1)
				{
					if (ffout)
					{
						if ( (*mtrnx[i]).tx_cal_n2 == 1)
							k_tru++;

	fprintf (hfout,"%s,%s,%010d,%06d,%02d,%02d,%02d,%d,%d,%06d,%04d,%04d,%d,%d,%03d,%08d,%02d,%02d,%d,%d,%d,%d\n",   
						(*mtrnx[i]).tx_trx_fecha,
						(*mtrnx[i]).tx_trx_hora,
						(*mtrnx[i]).tx_trx_nro,		
						(*mtrnx[i]).tx_trx_monto,
						(*mtrnx[i]).tx_art_desc,
						(*mtrnx[i]).tx_zon_px,
						(*mtrnx[i]).tx_zon_py,
						(*mtrnx[i]).tx_zon_iden,
						(*mtrnx[i]).tx_zon_inse,
						(*mtrnx[i]).tx_com_nro,
						(*mtrnx[i]).tx_com_px,
						(*mtrnx[i]).tx_com_py,
						(*mtrnx[i]).tx_com_inse,	
						(*mtrnx[i]).tx_com_itic,	
						(*mtrnx[i]).tx_com_nrub,
						(*mtrnx[i]).tx_per_nro,	
						(*mtrnx[i]).tx_per_px,	
						(*mtrnx[i]).tx_per_py,
						(*mtrnx[i]).tx_per_ifam,	
						(*mtrnx[i]).tx_per_inse,	
						(*mtrnx[i]).tx_cal_n1,	
						(*mtrnx[i]).tx_cal_n2 
						);
					}
					/* if (ffout ... */

				}
				/* if (f1 ... */
			}


			if (ffaux)
			{
				if (f5)
					fprintf (hfaux,"FRA: per %10d con %10d regs (%10d fra) mod? %d  \n",i_per_ant,i_idx,f5,f7);

				if (f7)
					fprintf (hfaux,"MOD: per %10d con %10d regs (%10d fra) mod? %d  \n",i_per_ant,i_idx,f5,f7);

				fprintf (hfaux,"REV: per %10d con %10d regs (%10d fra) mod? %d  \n",i_per_ant,i_idx,f5,f7);

			}


			strcpy(mdfb,"  ");
			if (f7)
				strcpy(mdfb," X");

			/* prueba */
			if (gp_fverbose("d2"))
			{	printf ("Rev: per %10d con %10d regs (%10d fra) borro: %d t.frd %6d %s\n",
					i_per_ant,i_idx,f5,f7,k_tru,mdfb);
			}

#endif
/* corto f4 */

			
		}
		/* if (f4 ... */


		if ( i_per_ant != i_per )
		{
			memcpy ( mtrnx[0], &tr_s, sizeof (trnx) );
			i_per_ant = i_per;
			i_idx =  1;
		}

	}




	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de proceso prueba 6 \n\n",gp_tm());
	}
}










/*
 * -----------------------------------------------------------------------------------
 *
 *	pro_pruebas8
 *
 *	cambio de posicion campos para usar el sort del sistema
 *
 * -----------------------------------------------------------------------------------
 */

/*
 * necesito que la linea tenga 
 * nro_per, fecha, hora 
 * porque el sort no me deja sortear por suma de claves !!!!
 *
 * prox. version ... que haga el sort en memoria ...
 */


int	pro_pruebas8 ()
{
	int	i,j,k;
	int	i_idx;
	int	t_cnt;

	char	b1[MAXB];
	char	b2[MAXB];
	char	n_idx[16];

	char	b3[MAXB] = 
"          1         2         3         4         5         6         7         8         9         ";
	char	b4[MAXB] = 
"0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";


	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra a pruebas 8 \n\n",gp_tm());
	}


	i_idx = 0;
	t_cnt = 0;

	while (fgets(b1,MAXB,hfinp) != NULL)
	{
#if 0
          1         2         3         4         5         6         7         8         9
0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

20181231,200917,0133250320,003590,99,07,06,3,1,182956,7081,6994,1,0,119,00205399,09,08,2,2,0,0
#endif

		strncpy(b2,b1+72,9);		/* copio la persona (mas la coma) */
		strncpy(b2+9,b1,16);		/* fecha y hora (mas la coma) */
		strncpy(b2+25,b1+27,45);	/* lo que esta despues de fecha-hora-nrotrx hasta nro per */
		strncpy(b2+70,b1+16,11);	/* nro trx */
		strncpy(b2+81,b1+81,14);	/* desde nro per hasta el final (mas el new line)  */


#if 0

	printf (">%s<\n>%s<\n>%s<\n>%s<\n%d %d\n",b3,b4,b1,b2,strlen(b1),strlen(b2));

#endif

		if ( strlen(b1) != strlen(b2) )
			error(315);

		fprintf (hfout,"%s",b2);

		if (b2[strlen(b2)-1] == '1')	
			t_cnt++;

		i_idx++;
	}

	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("Cantidad de registros reformateados %d \n",i_idx);
		printf ("Cantidad de registros truchos       %d \n",t_cnt); 
		printf ("\n");
	}


	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de pruebas 8 \n\n",gp_tm());
	}

}





/*
 * -----------------------------------------------------------------------------------
 *
 *	pro_pruebas9
 *
 *	calcula atributos para archivo weka (version para trx10_oo2.txt !!! )
 *
 * -----------------------------------------------------------------------------------
 */

/*
 *	Para explorar con ML, la propuesta es hacer el analisis
 *	de las ultimas N transacciones de una persona, al momento de
 *	evaluar una transaccion en particular ...
 *
 *	Por lo tanto, necesitamos ... 
 *	- al procesar una transaccion ...
 *	- obtener rapidamente las ultimas N transacciones de esa persona !!!
 *	- es recorrer el archivo al reves !!!!!
 *	- y hacer un indice ??? 
 *
 *	Atenti ..
 *	Este proceso, para hacerlo rapido en una lap ... requiere
 *	el arhivo de trx ordenado por  persona - fecha - hora
 *	de menor a mayor !!!! 
 *	O sea ... 
 *	
 *	Fecha      Hora     usuario
 *	enero   1  10:00    001
 *	enero   1  11:00    001
 *	enero   3  10:00    001
 *      dicie  22  13:00    001
 *      dicie  22  14:00    001
 *	etc
 *
 *
 *	Para un archivo de trx tipo pipeline ... hay que recorrer
 *	todo el archivo y juntar las trx de una persona !!!!
 *	Por eso necesitamos este archivo ordenado...
 *	2020_08_18   ultima act -> este anda !!
 */


int	pro_pruebas9()
{
	char	b1[MAXB];
	char	b2[MAXB];
	char	*x;

	int	tnv[10];		/* tabla 'no van' */
	int	i,j,k;
	int	i1,i2,i3;
	int	j1,j2,j3;
	int	q0,q1,q2,q3,q7;
	int	f1,f2,f3,f4,f5,f6,f7,f8,f9;
	int	n1,n2;

	int	flag;

	int	i_idx;
	int	q_idx;
	int	i_per;

	int	t_trx_per;		/* total de trx de la persona */
	int	i_per_ant;		/* persona anterior */

					/* variables weka ... */
	int	w1,w2,w3;
	int	h1;			/* hipotesis */

	int	at1,at2,at3,at4,at5,at6,at7,at8,at9;

	h1 = 3;				/* verifico con las ultimas h1 trx de la persona */

	/* proceso  */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra a proceso 9 ( weka ) \n\n",gp_tm());

	}


/* weka */
#if 1

	/* necesitamos transacciones indicadas en -inp ...  */
	if ( !ffinp)
		error(813);

	/* necesitamos archivo de salida -out   ...  */
	if ( !ffout)
		error(814);


	/* algunos inits */
	for (i=0; i< MAX_TRX; i++)
	{
		mtrnx[i] = (trnxptr ) malloc(sizeof(trnx));
			if (mtrnx[i] == NULL )
				error(906);
	}



	i_idx      =  0;
	i_per_ant  = -1;
	flag       =  1;
	f4         =  0;


	/* leo reg x reg del archivo de input */
	while (flag)
	{
		x = fgets(b1,MAXB,hfinp);

		if ( x == NULL )
		{	
			f4 = 1;
			flag = 0;
		}

		if ( x != NULL)
		{
			memset ( &tr_s , 0 , sizeof ( tr_s) );
			csv_parser(b1,fp_trx);
			i_per = tr_s.tx_per_nro;


			if ( i_per_ant == i_per )
			{
				memcpy ( mtrnx[i_idx], &tr_s, sizeof (trnx) );
				i_idx++;
				f4 = 0;
			}

			if ( i_per_ant != i_per  )
			{
				if ( i_per_ant == -1 )
				{
					f4 = 0;
				}

				if ( i_per_ant != -1 )
				{
					f4 = 1;
				}
			}
		}

		/* 
		 *	En este punto ....
		 *	tengo TODAS las transacciones de una persona 
		 *	ordenadas por fecha - hora
		 *	A partir aca ... x cada registro, tomando de
		 *	a N ... hago el calculo de atributos !!! 
		 */

		if (f4)
		{
#if 0
			/* tengo que invertir los registros para hacer mas natural el calculo */
			q0 = i_idx;

			i1 = 0;
			j1 = q0 - 1;
			f9 = 1;

			while (f9)
			{
				if ( j1 - i1 < 1)
					f9 = 0;

				if (f9)
				{
					ptrnx1    = mtrnx[i1]; 
					mtrnx[i1] = mtrnx[j1];
					mtrnx[j1] = ptrnx1;
				}

				i1++;
				j1--;
			}

#endif

			/* verifico tiene trx con fraude */
			f5 = 0;
			for (i=0; i< i_idx; i++)
			{
				if ( (*mtrnx[i]).tx_cal_n2 == 1)
					f5++;
			}
			
#if 0
			/* verifico si todas las truchas estan seguidas !! */
			i  = 0;
			f6 = 1;
			f7 = 0;		/* asumo que estan todas consecutivas */
			f8 = 1;
			q7 = 0;

			for (j=0; f6 && j< i_idx; j++)
			{
				if ( (*mtrnx[i+j]).tx_cal_n2 == 1 )
				{
					k=0;
					while (f8)
					{
						if ( (*mtrnx[i+j+k]).tx_cal_n2 == 1  )
							q7++;
						if ( (*mtrnx[i+j+k]).tx_cal_n2 == 0  )
						{	tnv[f7] = i+j+k;
							f7++;
						}

						if (q7 == f5)
							f8=0;
						k++;
						if (k >100)
						{
							error(604);  
						}
					}
				}

			}

#endif
		
			/* prueba */
			if (gp_fverbose("d3"))
			{	printf ("WEKA: per %10d con %10d regs (%10d fra) \n",i_per_ant,i_idx,f5);
			}

			/* calculo de atributos ... es ACA */

			q_idx = i_idx;

			for (w1 = 0; w1 < q_idx - h1; w1++)
			{
				at1 = calculo_at1(w1,w1+h1-1);
				at2 = calculo_at2(w1,w1+h1-1);	
				at3 = calculo_at3(w1,w1+h1-1);	
				at4 = calculo_at4(w1,w1+h1-1);	
				at5 = calculo_at5(w1,w1+h1-1);	
				at6 = calculo_at6(w1,w1+h1-1);	
				at7 = calculo_at7(w1,w1+h1-1);	
				at8 = calculo_at8(w1,w1+h1-1);	
				at9 = calculo_at9(w1,w1+h1-1);	

			

			/* grabo ... */
			if (ffout)
			{
				strcpy(b1,"  ");
				if ( (*mtrnx[w1]).tx_cal_n2 == 1)
					strcpy(b1," X");
				
				fprintf (hfout,"%6d,%6d,%6d,%6d,%6d,%6d,%6d,%6d,%6d,%d %s\n",
					at1,at2,at3,at4,at5,at6,at7,at8,at9,
					(*mtrnx[w1]).tx_cal_n2,b1);

#if 0
				fprintf (hfout,"%6d,%6d,%6d,%6d,%6d,%6d,%6d,%6d,%6d,%d %s (%6d) %4d\n",
					at1,at2,at3,at4,at5,at6,at7,at8,at9,
					(*mtrnx[w1]).tx_cal_n2,b1,(*mtrnx[w1]).tx_per_nro,w1);
#endif


			}

			if (ffaux)
			{

			}

			}
			/* for (w1 ... */
			
		}
		/* if (f4 ... */


		if ( i_per_ant != i_per )
		{
			memcpy ( mtrnx[0], &tr_s, sizeof (trnx) );
			i_per_ant = i_per;
			i_idx =  1;
		}

	}

#endif
/* weka */



	/* proceso  */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de proceso 8 ( weka ) \n\n",gp_tm());

	}


	
}


/* build idx */
#if 1

/*
 * -----------------------------------------------------------------------------------
 *
 *	build_idx 1
 *
 *	arma indice persona -> posiciones de las transacciones  ...
 *
 * -----------------------------------------------------------------------------------
 */

/*
 *	OJOJOJOJOJ
 *	tiene que estar cargado el vector de personas !!!!
 *
 */


int	build_idx1(hfidx)
FILE	*hfidx;
{
	int	i,j,k;
	int	f1;
	int	v1,v2;

	char	b1[MAXB];
	char	*read_status;

	int	per,per_ant;
	int	f_primera;
	int	t_offset;


	int	mm1;


	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra a proceso build idx 1  \n\n",gp_tm());
	}


#if 0
	/* como verifico esto ? si no cargo personas  no se puede hacer el proceso ... -cpe  */
	if (!ffcpe)
		error(809);
#endif
	if (q_gpe == 0)
		error(815);



	obj_ptr = (int *) malloc ( MAX_OBJ * sizeof(int) );
	if (obj_ptr == NULL)
		error(909);

	
	/* leo registros */

	mm1 = 1000000;

	f_primera =  1;
	per_ant   = -1;

	f1 = 1;

	q_idx1 = 0;

	while (f1)
	{
 		read_status = fgets(b1,MAXB,hfidx);

		memset ( &s_idx1 , 0 , sizeof ( s_idx1) );
		csv_parser(b1,fp_idx1);

		*(obj_ptr + q_idx1) = s_idx1.ix_trx;


		if ( per_ant == s_idx1.ix_nro )
		{
			v2++;
		}
		
		if ( per_ant != s_idx1.ix_nro || read_status == NULL )
		{
			if ( f_primera != 1)
			{
				if (gp_fverbose("d3"))
				{	
					printf ("Grabo: q_idx: %10d  per: %10d v1: %10d off: %10d q_trx: %6d %s\n",
						q_idx1,
						per_ant,
						v1,
						t_offset,
						v2,
						" ");
				}

				(*mpers[per_ant]).ps_idx_off = t_offset;
				(*mpers[per_ant]).ps_idx_qnt = v2;


				t_offset += v2;
				per_ant = s_idx1.ix_nro;
				v1 = per_ant;
				v2 = 1;
			}
	
			if ( f_primera == 1)
			{
				f_primera = 0;

				t_offset  = 0;
				per_ant = s_idx1.ix_nro;
				v1 = per_ant;
				v2 = 1;
			}
		}

		q_idx1++;


		if (read_status == NULL)
			f1 = 0;
	}



	if (gp_fverbose("d3"))
	{	
		printf ("Cantidad de transacciones (build idx1)  ... %10d\n",q_idx1);
	}


	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de proceso build idx 1 \n\n",gp_tm());
	}
}


#endif
/* build idx */





/*
 * -----------------------------------------------------------------------------------
 *
 *	genera_defper
 *
 *	genera tabla con definicion de perfiles x zona 
 *
 * -----------------------------------------------------------------------------------
 */

int	genera_defper()
{

	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra a genera definicion de perfiles \n\n",gp_tm());
	}


	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de genera definicion de perfiles \n\n",gp_tm());
	}
}


/*
 * -----------------------------------------------------------------------------------
 *
 *	genera_defcom
 *
 *	genera tabla con definicion de comercios 
 *
 * -----------------------------------------------------------------------------------
 */

int	genera_defcom()
{

	int	i,j,k;
	int	px,py;
	int	nse;			/* nivel socio economico */
	char	zona[64];		/* nombre de la zona */
	char	mden[4];		/* densidad de zona en mapa */
	char	mnse[4];		/* nse de zona en mapa */


	int	i_den,i_nse,i_tic;	/* sub-indices  densidad, nse, tipo_comercio */

	int	rnm[3][3];		/* matriz .. [nse][tic] */
	
	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra genera definicion de comercios \n\n",gp_tm());
	}

	/* si no cargo mapa ... no se puede generar def comercios */
	if (!ffpmp)
		error(802);

	/* si no cargo tabla de fm ... no se puede generar def comercios */
	if (!fftb1)
		error(806);

	q_cde = 0;

	/* init punteros */
	for (i=0; i<MAX_CDE ; i++)  
		mlcde[i] = (lcdeptr ) NULL;


	/* por cada Zona */
	for (px=0; px< MAX_FCM; px++)
	{
		for (py=0; py< MAX_FCM; py++)
		{
			

			/* Densidad: 0 Nula 1 Rural 2 Baja 3 Media 4 Alta */
			i_den = densidad( (*mmapa[px][py]).den );

			/* Nivel SE: 0 Nulo  - - -   2 Bajo 3 Medio 4 Alto */
			nse = nivelsocioeconomico( (*mmapa[px][py]).nse );

			/* en matriz de numeros random, van de 0 a 2 */
			nse = nse - 2;

			
			/* zona */
			strcpy(zona, (*mmapa[px][py]).nm );
			strcpy(mden, (*mmapa[px][py]).den );
			strcpy(mnse, (*mmapa[px][py]).nse );


			/* zona con densidad 0 no tiene comercios */
			if (i_den)
			{

				/* genero, al azar, la cantidad de comercios que deberia tener una zona,
				 * basado en:
				 * densidad  (nula, rural, baja, media, alta)
				 *     nivel socio economico  ( bajo, medio, alto )
				 *           tipo de comercio ( normal, rural , caro )
				 *
				 * Asi: (x ejemplo)
				 * 
				 * Un zona de densidad BAJA .. nse BAJO ... 
				 *     tendra 3 pares de numeros min, max, para generar cuentos comercios tiene 
				 *         de tipo normal ... fdmd[den BAJA][nse BAJO][normal][min][max]
				 *         de tipo rural  ... fdmd[den BAJA][nse BAJO][rural ][min][max]
				 *         de tipo caro   ... fdmd[den BAJA][nse BAJO][caro  ][min][max]
				 *
				 * Con esos tres pares de numeros, generan al azar tres numeros (dentro de min,max)
				 * que seran utilizados luego, para generar esa cantidad de negocios de ese tipo
				 *
				 */
 
				for ( i_nse=0; i_nse<3; i_nse++)
					for( i_tic=0; i_tic<3; i_tic++)
						rnm[i_tic][i_nse] =
							gna( fdmd[i_den][i_tic][i_nse][0],fdmd[i_den][i_tic][i_nse][1] );

				if (gp_fverbose("d3"))
				{
					printf ("(%2d,%2d), densidad tipo %d \n",px,py,i_den);

					for ( i_nse=0; i_nse<3; i_nse++)
					{	for( i_tic=0; i_tic<3; i_tic++)
						{	printf ("den: %2d  nse: %2d tic: %2d (min,max) %3d %3d ->  %3d\n",
								i_den,i_nse,i_tic,
								fdmd[i_den][i_nse][i_tic][0],
								fdmd[i_den][i_nse][i_tic][1],
								rnm[i_nse][i_tic]
								);
						}
						printf ("\n");
					}
		
					printf ("\n\n");
				}	


				/* genero lineas defcom x cada tipo de negocio, segun densidad, nse de la zona */


				for (i_tic=0; i_tic<3; i_tic++)
				{
					if (rnm[nse][i_tic])
					{

						/* aca, proceso de buscar negocio ... */
#if 0
						rubro = busco_rubro();
#endif
						mlcde[q_cde] = (lcdeptr) malloc(sizeof(lcde)) ; 
						(*mlcde[q_cde]).px = px;
						(*mlcde[q_cde]).py = py;
						strcpy ( (*mlcde[q_cde]).nzon,zona);
						strcpy ( (*mlcde[q_cde]).mden,mden);
						strcpy ( (*mlcde[q_cde]).mnse,mnse);

						(*mlcde[q_cde]).qcom_min = fdmd[i_den][nse][i_tic][0];
						(*mlcde[q_cde]).qcom_max = fdmd[i_den][nse][i_tic][1];
						(*mlcde[q_cde]).qnt   = rnm[nse][i_tic];
						(*mlcde[q_cde]).tic = i_tic;

						sprintf ( (*mlcde[q_cde]).nact,"r_%04d_q_%04d",q_cde,rnm[nse][i_tic]);
						sprintf ( (*mlcde[q_cde]).den,"%2d",i_den);
						sprintf ( (*mlcde[q_cde]).nse,"%2d",nse);
						(*mlcde[q_cde]).valido = 1;
						
						q_cde++;
										
					}					
				}
				/* for (i_tic...  */
			

			} else
			{
				if (gp_fverbose("d3"))
				{	printf ("densidad tipo %d (no hay comercios)\n",i_den);
					printf ("\n\n\n");
				}
			}

			/* if */


		}
		/* for (py ... ) */
	}
	/* for (px ... ) */

	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale genera definicion de comercios \n\n",gp_tm());
	}

}


/*
 * -----------------------------------------------------------------------------------
 *
 *	cantidad_n1 
 *
 *	algunas estadisticas ...
 *
 * -----------------------------------------------------------------------------------
 */

int	cantidad_n1()
{
	int	n,i;


	n=0;

	for (i=0; i<q_cde; i++)
		n += ( (*mlcde[i]).qnt );

	return n;

}


/*
 * -----------------------------------------------------------------------------------
 *
 *	densidad 
 *
 *	determina densidad (de la zona )
 *
 * -----------------------------------------------------------------------------------
 */


/* 	devuelve posicion de x[1] en s[] 
 *	contando a partir de 0
 */


int	densidad(x)
char	*x;
{
	int i,j;
	char s[10];
	
	i=0;
	j=0;
	strcpy(s,"NRBMA");

	while (s[i])
	{	if (*(x+1) == s[i])
			j = i;
		i++;
	}
	return j;

}


/*
 * -----------------------------------------------------------------------------------
 *
 *	nivel socio economico 
 *
 *	determina nivel socio economico (de la zona )
 *
 * -----------------------------------------------------------------------------------
 */



int	nivelsocioeconomico(x)
char	*x;
{
	int i,j;
	char s[10];
	
	i=0;
	j=0;
	strcpy(s,"N_BMA");

	while (s[i])
	{	if (*(x+1) == s[i])
			j = i;
		i++;
	}
	return j;

}





/*
 * -----------------------------------------------------------------------------------
 *
 *	genera_comercios
 *
 *	por cada zona, genera los comercios 
 *
 * -----------------------------------------------------------------------------------
 */

int	genera_comercios()
{
	int	i,j,k,qn;
	int	px,py;
	int	pxa,pya;		/* px py anteriores */
	int	com_offset;
	int	i_dc;			/* definicion de comercio */
	int	q1,q2;
	int	t1,t2;
	int	i1,i2,i3;
	int	d1,d2;
	int	n1,n2;
	int	m1,m2;
	int	cp1,cp2;		/* para posicion local del comercio */
	int	nc1,nc2,c_nse;
	int	r1,r2;			/* rubro */
	int	prub;
	int	cpx,cpy;		/* px py comercio */
	int	q_nco;
	int	q_cosum;
	int	i_tic;
	char	s_nse[4];




	/* proceso  */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra a genera_comercios \n\n",gp_tm());
	}

	/* si no cargo mapa ... no se puede generar comercios */
	if (!ffpmp)
		error(802);

	/* si no cargo definicion de comercios ... no se puede generar comercios */
	if (!ffcde)
		error(803);

	/* si no cargo tabla con factor de multip 1 ... no se puede generar comercios */
	if (!fftb1)
		error(806);

	/* si no cargo tabla con factor de multip 2 ... no se puede generar comercios */
	if (!fftb2)
		error(804);

	/* si no cargo tabla de rubros ... no se puede generar comercios */
	if (!ffrub)
		error(805);


	cpx   =  0;
	cpy   =  0;
	q_nco =  0;
	pxa   = -1;
	pya   = -1;

	/* genero el resto de la info x cada comercio segun la definicion de defcom */
	for (i_dc = 0 ; i_dc < q_cde; i_dc++)
	{
		if (gp_fverbose("d3"))
		{
			printf ("Proceso defcom nro: %d \n",i_dc);
		}

		if (gp_fverbose("d4"))
		{
			printf ("REB: %2d %2d %s den: %s %d nse: %s %d qnt: %3d tic: %3d \n\n", 
				(*mlcde[i_dc]).px,
				(*mlcde[i_dc]).py,
				(*mlcde[i_dc]).nzon,
				(*mlcde[i_dc]).mden,
				(*mlcde[i_dc]).iden,
				(*mlcde[i_dc]).mnse,
				(*mlcde[i_dc]).iden,
				(*mlcde[i_dc]).qnt,
				(*mlcde[i_dc]).tic
				);
		}


		/* genero rubros al azar para todos los tic ! */
		azar0200(0);
		azar0200(1);
		azar0200(2);
	
		px = (*mlcde[i_dc]).px;
		py = (*mlcde[i_dc]).py;

		q1 = (*mlcde[i_dc]).qnt;		/* cantidad de rubros que tengo que generar */
		t1 = (*mlcde[i_dc]).tic;		/* tipo tic de los rubros */
		d1 = (*mlcde[i_dc]).iden;		/* densidad de la zona */
		n1 = (*mlcde[i_dc]).inse;		/* nse de la zona */

		if (px != pxa || px != pya)
			q_cosum = 0;
	
		/* para cada uno de los rubros de tipo tic ... */
		for (i1=0; i1<q1; i1++)
		{
			m1 =  fdmc[d1][n1][t1][0];
			m2 =  fdmc[d1][n1][t1][1];

			q2 =  gna( fdmc[d1][n1][t1][0],fdmc[d1][n1][t1][1]);
		
			r1 = rn200[t1][i1 % MAX_RXT];


			for (i2=0; i2<q2; i2++)
			{
				if (gp_fverbose("d3"))
				{
					printf ("REC: px,py: (%2d,%2d) %s %d %d loop: (%4d,%3d,%3d) fm: (%3d,%3d,%3d), rub/tic: (%3d,%3d)\n\n",
						px,py,
						(*mlcde[i_dc]).nzon,d1,n1,
						i_dc,i1,i2,
						m1,m2,q2,
						r1,t1
						);
				}


				/* numero de rubro en el vector, segun el tic */
				prub = ( t1 * MAX_RXT ) + r1;

				/* genero las posicion x y dentro de la zona, pero con valores absolutos en todos el mapa */
				cp1 = ( px * MAX_CPX );
				cp2 = ( py * MAX_CPX );

				cpx = gna ( cp1, cp1 + MAX_CPX -1 );
				cpy = gna ( cp2, cp2 + MAX_CPX -1 );


				/* genero en nse del comercio en funcion del nse de la zona */

				i_tic = (*mlcde[i_dc]).tic;

				if ( i_tic == 0)
				{	nc1 = 0; 
					nc2 = 1;
				}
				if ( i_tic == 1)
				{	nc1 = 0;		
					nc2 = 2;
				}
				if ( i_tic == 2)
				{	nc1 = 1;
					nc2 = 2;
				}

				c_nse = gna(nc1,nc2);
				strcpy(s_nse, mnse[c_nse]);


				if (gp_fverbose("d3"))
				{
					printf ("REG: %06d,%3d,%3d,%2d,%2d,%s,%s,%s,%d,%d,%s,%s,%3d,%3d,%3d,%d,%4d,%4d,%s,%3d,%3d,%3d,%3d,%3d\n",

					q_nco,i1,i2,				/* numero secuencial del comercio y de los val de loop */
					px,
					py, 
					(*mlcde[i_dc]).nzon, 
					(*mlcde[i_dc]).mden,	
					(*mlcde[i_dc]).mnse,
					(*mlcde[i_dc]).iden,
					(*mlcde[i_dc]).inse,
	
					(*mlcde[i_dc]).nact, 			/* deberia ser nombre del rubro ... */
					(*mlrub[prub]).ru_nom,			/* son lo mismo ??? */
			
					(*mlcde[i_dc]).qcom_min,
					(*mlcde[i_dc]).qcom_max,
					(*mlcde[i_dc]).qnt,
					(*mlcde[i_dc]).tic,

					/* ncom */				/* numero  del comercio */
					cpx,
					cpy,
					s_nse,					/* nse del comercio .. .generado ... */
					m1,m2,q2,
					r1,
					prub
					);

				}

				/* La carga de todos los datos a estruct mcome, es ACA !!!! */

				mcome[q_nco] = (comeptr ) malloc(sizeof(come));

				(*mcome[q_nco]).ncom = q_nco;
				(*mcome[q_nco]).i1   = i1;		/* sacar later */
				(*mcome[q_nco]).i2   = i2;		/* sacar later */
				(*mcome[q_nco]).zon_px   = px;
				(*mcome[q_nco]).zon_py   = py;

				strcpy ( (*mcome[q_nco]).z_nom, (*mlcde[i_dc]).nzon );
				strcpy ( (*mcome[q_nco]).z_den, (*mlcde[i_dc]).mden );
				strcpy ( (*mcome[q_nco]).z_nse, (*mlcde[i_dc]).mnse );

				(*mcome[q_nco]).i_den   = (*mlcde[i_dc]).iden;
				(*mcome[q_nco]).i_nse   = (*mlcde[i_dc]).inse;

				strcpy ( (*mcome[q_nco]).nact,	(*mlcde[i_dc]).nact );		/* rubro (actividad) de defcom */
				strcpy ( (*mcome[q_nco]).r_nom,	(*mlrub[prub]).ru_nom);		/* nombre del rubro */

				(*mcome[q_nco]).min     = (*mlcde[i_dc]).qcom_min;
				(*mcome[q_nco]).max     = (*mlcde[i_dc]).qcom_max;
				(*mcome[q_nco]).qnt     = (*mlcde[i_dc]).qnt;
				(*mcome[q_nco]).i_tic   = (*mlcde[i_dc]).tic;

				(*mcome[q_nco]).px      = cpx;
				(*mcome[q_nco]).py      = cpy;

				strcpy ( (*mcome[q_nco]).nse, s_nse );			/* nse a calcular */
				(*mcome[q_nco]).c_nse   = c_nse;				/* nse a calcular */

				(*mcome[q_nco]).min2    = m1;
				(*mcome[q_nco]).max2    = m2;
				(*mcome[q_nco]).qnt2    = q2;

				(*mcome[q_nco]).n_rub   = r1;
				(*mcome[q_nco]).p_rub   = prub;

				q_nco++;


			}
			
			pxa = px;
			pya = py;
		}

	}
	/* for (i_dc ... */




	/* compruebo integridad de los comercios generados */
	if (gp_fverbose("d3"))
	{
		for (i=0; i<q_nco; i++)
		{	
			printf ("%8d %8d (zon: %3d %3d  %s %s %s %d %d) %s %s ( %3d %3d %3d %d) (%4d %4d) %s %d ( %3d %3d %3d) ( %3d %3d)\n",
				i,
				(*mcome[i]).ncom,
				(*mcome[i]).zon_px,
				(*mcome[i]).zon_py,
				(*mcome[i]).z_nom,
				(*mcome[i]).z_den,
				(*mcome[i]).z_nse,
				(*mcome[i]).i_den,
				(*mcome[i]).i_nse,
				(*mcome[i]).nact,
				(*mcome[i]).r_nom,
				(*mcome[i]).min,
				(*mcome[i]).max,
				(*mcome[i]).qnt,
				(*mcome[i]).i_tic,
				(*mcome[i]).px,
				(*mcome[i]).py,
				(*mcome[i]).nse,
				(*mcome[i]).c_nse,
				(*mcome[i]).min2,
				(*mcome[i]).max2,
				(*mcome[i]).qnt2,
				(*mcome[i]).n_rub,
				(*mcome[i]).p_rub
			

				);
		}
	}


	/* utilizamos q_com en general para tener la cantidad de comercios en la tabla */
	q_com = q_nco;


	if (gp_fverbose("d3"))
	{
		printf ("Cantidad de comercios generados  %d \n\n",q_com);
	}

	/* proceso  */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de genera_comercios \n\n",gp_tm());
	}

}



/*
 * -----------------------------------------------------------------------------------
 *
 *	genera_personas
 *
 *	genera tabla de personas
 *
 * -----------------------------------------------------------------------------------
 */

int	genera_personas()
{
	int	i,j,k;
	int	i1,i2,i3;
	int	f1,f2;
	int	q1,q2;
	int	px,py;	

	int	q_sum;
	int	i_tpf;
	int	q_personas;
	int	vn[16];
	int	i_fam;
	int	i_nse;

	/* proceso  */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra a genera_personas \n\n",gp_tm());
	}

	/* si no cargo mapa ... no se puede generar personas */
	if (!ffpmp)
		error(802);

	/* si no cargo perfiles de tarjetahabientes  ...no se puede generar personas */
	if (!ffper)
		error(808);

	/* si no cargo tabla con tipos de perfil x zona no se puede generar personas */
	if (!fftb4)
		error(807);

	q_sum = 0;
	q_gpe = 0;

	for (px=0; px < MAX_FCM; px++ )
	{
		for (py=0; py < MAX_FCM; py++)
		{
			if ( mmapa[px][py] != (mapaptr ) NULL )
			{
				i_tpf = -1;

				for ( i=0; i< q_tpf; i++)
					if ( !strcmp( (*mmapa[px][py]).den,(*mtper[i]).tp_den  ) &&
					     !strcmp( (*mmapa[px][py]).nse,(*mtper[i]).tp_nse  )      )    
						i_tpf = i;

				if (i_tpf != -1)
				{
					q_personas = (*mmapa[px][py]).qneg;

					/* debug  */
					if (gp_fverbose("d4"))
					{	

					/* aqui tengo el registro den,nse ... de perfil para zona de mapa px py */
					printf ("REP:(%2d,%2d) i_tpf: %2d %5d %s\n",
						px,py,i_tpf,q_personas,(*mtper[i_tpf]).tp_nom);

					}

					/*
					 *	en este punto ... para la zona px py ... 
					 *	tenemos el nro de registro en mtper que tiene
					 *	los 9 numeros que indican como distribuir cada uno de los 9 perfiles disponibles 
					 *	en esa zona ...
					 *	Ese numero es un porcentaje (numero c/ 100 habitantes ). 
					 * 	El loop sigue
					 *	x cada uno de esos coeficientes ... 
					 *
					 */

					/* paso  la matriz de porcentajes a vector */
					for (i_nse = 0; i_nse < 3; i_nse++)
						for (i_fam = 0; i_fam < 3; i_fam++)
							vn[i_fam * 3 + i_nse ] = (*mtper[i_tpf]).tp_por[i_fam][i_nse];

					for (i1=0; i1<9; i1++)	
					{	
						q1 = (q_personas * vn[i1]) / 100;
						q_sum += q1;

						for (i2=0; i2<q1; i2++)
						{

							/* debug  */
							if (gp_fverbose("d4"))
							{	

							printf ("PPP: %4d %4d (%2d,%2d) %s %5d %5d --  ",
								i1,i2,px,py,
								(*mtper[i_tpf]).tp_nom,
								vn[i1],q1
								);
	
							printf ("%3d %s %s %s %d %d,%3d %3d,%5d %5d %5d %5d %5d %5d %5d %5d %5d %s\n",
								(*mperf[i1]).pe_nro,
								(*mperf[i1]).pe_nom,
								(*mperf[i1]).pe_fam,
								(*mperf[i1]).pe_nse,
								(*mperf[i1]).pe_ifam,
								(*mperf[i1]).pe_inse,
								(*mperf[i1]).pe_cxm[0],
								(*mperf[i1]).pe_cxm[1],
								(*mperf[i1]).pe_dis[0][0],
								(*mperf[i1]).pe_dis[0][1],
								(*mperf[i1]).pe_dis[0][2],
								(*mperf[i1]).pe_dis[1][0],
								(*mperf[i1]).pe_dis[1][1],
								(*mperf[i1]).pe_dis[1][2],
								(*mperf[i1]).pe_dis[2][0],
								(*mperf[i1]).pe_dis[2][1],
								(*mperf[i1]).pe_dis[2][2],
								"|"
								);
							}
		


						/* aca hay que guardar todo este resultado en el futuro registro de personas ....			
						 * crear otra struct para personas !!! ... ya tenemos perf tper ... mmmm crd (cardholder)
						 */

							mpers[q_gpe] = (persptr ) malloc (sizeof (pers));
							if (mpers[q_gpe] == NULL )
								error(902);


							/* cargo todo en vector de personas mpers ... */
							(*mpers[q_gpe]).ps_nro       = q_gpe;
							sprintf ( (*mpers[q_gpe]).ps_nom ,"pers_n_%06d",q_gpe);
							(*mpers[q_gpe]).ps_zon_px    = px;
							(*mpers[q_gpe]).ps_zon_py    = py;
							strcpy ( (*mpers[q_gpe]).ps_zon_den, (*mmapa[px][py]).den );
							strcpy ( (*mpers[q_gpe]).ps_zon_nse, (*mmapa[px][py]).nse );
							(*mpers[q_gpe]).ps_zon_i_den = (*mmapa[px][py]).iden;
							(*mpers[q_gpe]).ps_zon_i_nse = (*mmapa[px][py]).inse;
							(*mpers[q_gpe]).ps_i_tpf     = i_tpf;
							strcpy ( (*mpers[q_gpe]).ps_n_tpf, (*mtper[i_tpf]).tp_nom );
							strcpy ( (*mpers[q_gpe]).ps_pe_nom, (*mperf[i1]).pe_nom );
							strcpy ( (*mpers[q_gpe]).ps_fam, (*mperf[i1]).pe_fam );
							strcpy ( (*mpers[q_gpe]).ps_nse, (*mperf[i1]).pe_nse );
							(*mpers[q_gpe]).ps_ifam      =   (*mperf[i1]).pe_ifam ;
							(*mpers[q_gpe]).ps_inse      =   (*mperf[i1]).pe_inse ;
							(*mpers[q_gpe]).ps_cxm[0]    =   (*mperf[i1]).pe_cxm[0];
							(*mpers[q_gpe]).ps_cxm[1]    =   (*mperf[i1]).pe_cxm[1];

							(*mpers[q_gpe]).ps_dis[0][0] =   (*mperf[i1]).pe_dis[0][0];
							(*mpers[q_gpe]).ps_dis[0][1] =   (*mperf[i1]).pe_dis[0][1];
							(*mpers[q_gpe]).ps_dis[0][2] =   (*mperf[i1]).pe_dis[0][2];
							(*mpers[q_gpe]).ps_dis[1][0] =   (*mperf[i1]).pe_dis[1][0];
							(*mpers[q_gpe]).ps_dis[1][1] =   (*mperf[i1]).pe_dis[1][1];
							(*mpers[q_gpe]).ps_dis[1][2] =   (*mperf[i1]).pe_dis[1][2];
							(*mpers[q_gpe]).ps_dis[2][0] =   (*mperf[i1]).pe_dis[2][0];
							(*mpers[q_gpe]).ps_dis[2][1] =   (*mperf[i1]).pe_dis[2][1];
							(*mpers[q_gpe]).ps_dis[2][2] =   (*mperf[i1]).pe_dis[2][2];


							if (q_gpe >= MAX_PER )
								error( 311 );

							q_gpe++;


						}
						/* for (i2 ... ) */

					}
				}

				/* si no encontro perfiles para la zona, y no es zona de desidad Nula ... entonces estamos mal */
				if (i_tpf == -1)
				{
					if (  !strcmp( (*mmapa[px][py]).den,"DR")   )    
						error( 601 );
				}
			}
		}
		/* for (py ...) */
	}
	/* for (px ...) */
		

	/* reporte   */
	if (gp_fverbose("d1"))
	{	
		printf ("Cantidad de personas a generar (calculo)  %d\n",q_sum);
		printf ("Cantidad de personas generadas (real)     %d\n",q_gpe);
	}

	
	/* mostrar   */
	if (gp_fverbose("d3"))
	{	printf ("%s Listado de personas generadas \n\n",gp_tm());

		for (i=0; i<q_gpe; i++)
		{
			printf ("%06d %s %02d %02d %s %s %d %d %02d %s %s %s %s %d %d %3d %3d %2d %4d %5d %2d %4d %5d %2d %4d %5d   %s\n",
				(*mpers[i]).ps_nro,
				(*mpers[i]).ps_nom, 
				(*mpers[i]).ps_zon_px,
				(*mpers[i]).ps_zon_py,
				(*mpers[i]).ps_zon_den,
				(*mpers[i]).ps_zon_nse,
				(*mpers[i]).ps_zon_i_den,
				(*mpers[i]).ps_zon_i_nse,
				(*mpers[i]).ps_i_tpf,
				(*mpers[i]).ps_n_tpf,
				(*mpers[i]).ps_pe_nom,
				(*mpers[i]).ps_fam,
				(*mpers[i]).ps_nse,
				(*mpers[i]).ps_ifam,
				(*mpers[i]).ps_inse,
				(*mpers[i]).ps_cxm[0],
				(*mpers[i]).ps_cxm[1],
				(*mpers[i]).ps_dis[0][0], 
				(*mpers[i]).ps_dis[0][1], 
				(*mpers[i]).ps_dis[0][2], 
				(*mpers[i]).ps_dis[1][0], 
				(*mpers[i]).ps_dis[1][1], 
				(*mpers[i]).ps_dis[1][2], 
				(*mpers[i]).ps_dis[2][0], 
				(*mpers[i]).ps_dis[2][1], 
				(*mpers[i]).ps_dis[2][2], 
				"|"
				);
		}
	}

	/* proceso  */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de genera_personas \n\n",gp_tm());
	}


}


/*
 * -----------------------------------------------------------------------------------
 *
 *	genera_transacciones
 *
 *	genera tabla de transacciones
 *
 * -----------------------------------------------------------------------------------
 */

int	genera_transacciones()
{
	int	i,j,k;

	int	i_mes;
	int	i_per;
	int	i_trx;

	int	m_dis[3][3];
	int	i_dis,j_dis;

	int	pe_tx_min;
	int	pe_tx_max;

	int	tx_rn_min;
	int	tx_rn_max;

	int	co_nro;
	int	co_ptr;

	int	i1,i2;
	int	q1,q2;
	int	n_aux;

	int	q_t[3];

	int	tx_monto;
	char	tx_fecha[16];
	char	tx_hora[16];

	int	px,py;
	int	pxa,pya;

	int	f1;
	int	hasta;		/* hasta que numero de persona .. default q_gpe */

	int	i_per_ant;
	int	f_cambio_persona;
	int	q_cambio_persona;

	/* proceso  */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra a genera_transacciones \n\n",gp_tm());
	}

	/* si no cargo comercios no se puede generar transacciones */
	if (!ffcom)
		error(810);

	/* si no cargo personas  no se puede generar transacciones */
	if (!ffcpe)
		error(809);

	/* reporte  */
	if (gp_fverbose("d1"))
	{	printf ("Genera transacciones para %d personas \n\n",q_gpe);
	}

	
	/* para detectar que cambio la persona */
	i_per_ant = -1;
	f_cambio_persona = -1;
	q_cambio_persona = 0;

	/* para detectar cambio de zona de la persona */
	pxa = -1;
	pya = -1;

	/* sub indice de transaccion */
	i_trx = 0;

#if 1
	hasta = q_gpe;
#endif
#if 0
	hasta = 5000;
#endif

	/* por cada persona ... voy a generar sus transacciones */
	for (i_per = 0; i_per < hasta; i_per++ )
	{

		/* borro datos de la trx */
		memset ( &tr1 , 0 , sizeof ( tr1) );

		/* obtengo todos los datos de la persona que necesito */
		pe_tx_min = (*mpers[i_per]).ps_cxm[0];
		pe_tx_max = (*mpers[i_per]).ps_cxm[1];

		/* copio en reg trx -- datos de la persona */
		tr1.tx_per_nro   = (*mpers[i_per]).ps_nro;
		tr1.tx_per_px    = (*mpers[i_per]).ps_zon_px;
		tr1.tx_per_py    = (*mpers[i_per]).ps_zon_py;
		tr1.tx_per_ifam  = (*mpers[i_per]).ps_ifam;
		tr1.tx_per_inse  = (*mpers[i_per]).ps_inse;

		strcpy( tr1.tx_per_nom, (*mpers[i_per]).ps_nom );
		strcpy( tr1.tx_per_fam, (*mpers[i_per]).ps_fam );
		strcpy( tr1.tx_per_nse, (*mpers[i_per]).ps_nse );
		

		/* armo la matriz de porcentajes / rangos de gastos */
		for (i_dis = 0 ; i_dis < 3; i_dis++ )
		{
			for (j_dis = 0; j_dis < 3; j_dis++ )
			{
				m_dis[i_dis][j_dis] = (*mpers[i_per]).ps_dis[i_dis][j_dis];
			}
		}

#if 0
		printf ("Matriz de rangos ... \n");
		for (i=0; i<3; i++)
		{	printf ("rango %d :   ",i);

			for (j=0; j<3; j++)
				printf ("%5d ",m_dis[i][j]);
			printf ("\n");
		}
#endif


		if (pxa != tr1.tx_per_px || pya != tr1.tx_per_py )
		{

			/* inits varios */
			for (px=0; px<MAX_FCM; px++)
				for (py=0; py<MAX_FCM; py++)
					el_mapa[px][py]=0;

			/* matriz de zonas eligibles */
			determina_elegibles();

			/* vector de negocios elegibles */
			determina_vector_elegibles();
		
			pxa = tr1.tx_per_px;
			pya = tr1.tx_per_py;
		}

		/* por cada mes */
		for (i_mes = 0; i_mes < 12; i_mes++ )
		{

			/* q1 es la cant de trx x mes de la persona, en el rango indicado */
			q1 = gna(pe_tx_min,pe_tx_max);

			/* q_t[x] es la proporcion de trx a realizar x montos en el rango x */
			for (i_dis =0; i_dis < 3; i_dis++)
				q_t[i_dis] = q1 * m_dis[i_dis][0] / 100;


#if 0
			printf ("tot trx: %2d   r1: %2d  r2: %2d  r3: %2d \n",q1,q_t[0],q_t[1],q_t[2] );
#endif

	
			/* transacciones de la persona x mes */
			for (i1=0; i1<3; i1++)
			{

				/* por cada uno de los rangos indicados , genero trx */
				for (i2=0; i2 < q_t[i1]; i2++)
				{

					/*
					 *	loop principal que genera LA transaccion
					 *	"gtrx"
					 */

					/* genero fecha y hora de la trx */
					strcpy (tx_fecha,gnf(i_mes));
					strcpy (tx_hora,gnh(8,20));			/* refinar ... horario x rubro ?? */
					
					/* genero el monto */
					tx_rn_min = m_dis[i1][1];
					tx_rn_max = m_dis[i1][2];
					n_aux  = gna(tx_rn_min,tx_rn_max) / 10;
					n_aux = n_aux * 10;
					if (gna(1,10) >= 5)
						n_aux += 5;

					tx_monto = n_aux;

					
					/* selecciono comercio entre los elegibles ! */
					co_nro = gna(0,q_vec - 1);
					co_ptr = el_come[co_nro];
		/* 
		 *
		 */

					/* copio en reg trx -- datos del comercio */
					tr1.tx_com_nro  = (*mcome[co_ptr]).ncom;         
					tr1.tx_com_px   = (*mcome[co_ptr]).px;	/* coord px py del comercio !! no de la zona */
					tr1.tx_com_py   = (*mcome[co_ptr]).py;
					tr1.tx_com_itic = (*mcome[co_ptr]).i_tic;
					tr1.tx_com_inse = (*mcome[co_ptr]).c_nse;
					tr1.tx_com_nrub = (*mcome[co_ptr]).n_rub;

					strcpy( tr1.tx_com_nom , (*mcome[co_ptr]).co_nom );		
					strcpy( tr1.tx_com_rub , (*mcome[co_ptr]).r_nom  );
					strcpy( tr1.tx_com_nse , (*mcome[co_ptr]).nse    );


					/* copio en reg trx -- datos de la zona de la transaccion */
					tr1.tx_zon_px   = (*mcome[co_ptr]).zon_px;
					tr1.tx_zon_py   = (*mcome[co_ptr]).zon_py;
					tr1.tx_zon_iden = (*mcome[co_ptr]).i_den;
					tr1.tx_zon_inse = (*mcome[co_ptr]).i_nse;

					strcpy( tr1.tx_zon_nom , (*mcome[co_ptr]).z_nom );
					strcpy( tr1.tx_zon_den , (*mcome[co_ptr]).z_den );
					strcpy( tr1.tx_zon_nse , (*mcome[co_ptr]).z_nse );

					/* copio en reg trx -- datos del articulo de la transaccion */	/* refinar a fururo */
					tr1.tx_art_desc = 99;
    

					/* reservo espacio para guardar datos de la trx
					 * una corrida full, genera 135 M de trx ...
					 * no tiene sentido por ahora, guardarla en memoria
					 */
#if 0
					mtrnx[i_per] = (trnxptr ) malloc(sizeof(trnx));
					if (mtrnx[i_per] == NULL )
						error(903);
#endif

					/* guardo rn reg trx -- datos de la trx */
					tr1.tx_trx_nro   = i_trx;
					tr1.tx_trx_dia   = 0;			/* refinar .. calcular dia de la semana */
					tr1.tx_trx_monto = tx_monto;		/* refinar .. ajustar a 0 o 5 el ultimo digito */
					strcpy ( tr1.tx_trx_fecha, tx_fecha );
					strcpy ( tr1.tx_trx_hora , tx_hora  );

					/* guardo en reg trx -- datos para usar en entrenaiento  */
					tr1.tx_cal_n1 = 0;
					tr1.tx_cal_n2 = 0;



					/* impresion para debugging ... */
#if 1
					if ( i_trx % 100000 == 0)
#endif
	printf ("V_%s %s i_trx %10d i_per %6d i_mes %2d i1 %3d i2 %3d  (%5d,%5d,%5d) co: %6d pos: %4d %4d zon: %2d %2d \n",
							tx_fecha,tx_hora,i_trx,i_per,i_mes,
							i1,i2,tx_rn_min,tx_rn_max,tx_monto,
							co_nro,		
							tr1.tx_com_px,
							tr1.tx_com_py,
							tr1.tx_zon_px,
							tr1.tx_zon_py
							);

					/* grabamos registro en file */

	fprintf (hfgtx,"%s,%s,%010d,%06d,%02d,%02d,%02d,%d,%d,%06d,%04d,%04d,%d,%d,%03d,%08d,%02d,%02d,%d,%d,%d,%d\n",   
						tr1.tx_trx_fecha,
						tr1.tx_trx_hora,
						tr1.tx_trx_nro,		
						tr1.tx_trx_monto,
						tr1.tx_art_desc,
						tr1.tx_zon_px,
						tr1.tx_zon_py,
						tr1.tx_zon_iden,
						tr1.tx_zon_inse,
						tr1.tx_com_nro,
						tr1.tx_com_px,
						tr1.tx_com_py,
						tr1.tx_com_inse,	
						tr1.tx_com_itic,	
						tr1.tx_com_nrub,
						tr1.tx_per_nro,	
						tr1.tx_per_px,	
						tr1.tx_per_py,
						tr1.tx_per_ifam,	
						tr1.tx_per_inse,	
						tr1.tx_cal_n1,	
						tr1.tx_cal_n2 
						);



					/* siguiente trx */
					i_trx++;

				}
				/* for (i2= ... */

			}
			/* for (i1 ... ) */

		}
		/* for (i_mes ... ) */

		/* si vamos a agregar transacciones para fraud detection .. es ACA 
		 * cada ... 500 ... personas, agarramos la ultima de esa persona
		 * y generamos un lote de trx 'fraudulentas' al algoritmo de ML
		 */

		if (i_per % 500 == 0)
		{
#if 0
			/* finalmente transacciones truchas se generan aparte */
			create_fdt(&tr1);		
#endif

			/* si puso archivo de salida -out ... grabo las 'seleccionadas' para fabricar fd */

			if (ffout)
			{
				fprintf (hfout,"%s,%s,%010d,%06d,%02d,%02d,%02d,%d,%d,%06d,%04d,%04d,%d,%d,%03d,%08d,%02d,%02d,%d,%d,%d,%d\n",   
					tr1.tx_trx_fecha,
					tr1.tx_trx_hora,
					tr1.tx_trx_nro,		
					tr1.tx_trx_monto,
					tr1.tx_art_desc,
					tr1.tx_zon_px,
					tr1.tx_zon_py,
					tr1.tx_zon_iden,
					tr1.tx_zon_inse,
					tr1.tx_com_nro,
					tr1.tx_com_px,
					tr1.tx_com_py,
					tr1.tx_com_inse,	
					tr1.tx_com_itic,	
					tr1.tx_com_nrub,
					tr1.tx_per_nro,	
					tr1.tx_per_px,	
					tr1.tx_per_py,
					tr1.tx_per_ifam,	
					tr1.tx_per_inse,	
					tr1.tx_cal_n1,	
					tr1.tx_cal_n2 
					);
			}
			/* if (ffout ... */
		}
	}	
	/* for (i_per ... ) */


	printf ("Cantidad de trx generadas .... %d \n\n",i_trx);
	t_trx = i_trx;

	/* proceso  */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de genera_transacciones \n\n",gp_tm());
	}

}


/*
 * -----------------------------------------------------------------------------------
 *
 *	create_fdt 
 *
 *	crear algunas transacciones fraudulentas a partir de una trx
 *
 * -----------------------------------------------------------------------------------
 */

int	create_fdt(tr)
trnxptr	tr;
{
	int	i,j,k;
	int	q0,q1,q2;
	int	f1,f2;
	int	n1,n2;
	int	z0,z1;

	int	zon_px,zon_py;
	int	com_px,com_py;





	zon_px = (*tr).tx_zon_px;
	zon_py = (*tr).tx_zon_py;

	com_px = (*tr).tx_com_px;
	com_py = (*tr).tx_com_py;


	/* proceso  */
	if (gp_fverbose("d3"))
	{	
		printf ("\n\ngenero trnx fraudulentas: \n\n");

		printf ("q_com        %10d\n",q_com);
		printf ("trx_nro      %10d\n",(*tr).tx_trx_nro);
		printf ("trx_per      %10d\n",(*tr).tx_per_nro);
		printf ("num com      %10d\n",(*tr).tx_com_nro);
		printf ("trx_per_px   %10d\n",(*tr).tx_per_px);
		printf ("trx_per_py   %10d\n",(*tr).tx_per_py);
		printf ("trx_zon_px   %10d\n",(*tr).tx_zon_px);
		printf ("trx_zon_py   %10d\n",(*tr).tx_zon_py);
		printf ("trx_com_px   %10d\n",(*tr).tx_com_px);
		printf ("trx_com_py   %10d\n",(*tr).tx_com_py);
		printf ("zlmap ..[0]  %10d\n",zlmap[zon_px][zon_py][0]);
		printf ("zlmap ..[1]  %10d\n",zlmap[zon_px][zon_py][1]);

		printf ("\n\n");

	}



	/* armo tabla nro_comercio / distancia  para todos los comercios
	 * que pertenen a la misma zona
	 * que el comercio 'cabecera' del fraude 
	 */

	for (i=0; i< zlmap[zon_px][zon_py][1]; i++)
	{
		zo_come[i][0] = (*mcome[ zlmap[zon_px][zon_py][0] + i ]).ncom ;
		zo_come[i][1] = abs ( (*mcome[ zo_come[i][0] ]).px - com_px ) + abs ( (*mcome[ zo_come[i][0]     ]).py - com_py );

		/* no tengo en cuenta la distancia del negocio a si mismo */
		if (zo_come[i][1] == 0 )
			zo_come[i][1] = 10000;
					
#if 0
		printf ("i: %4d zon (%d,%d)  zlmap: %5d %5d  zo_come[i][0]: %5d  ncom: %5d (%5d,%5d) dist: %6d \n",
			i,
			zon_px,zon_py, zlmap[zon_px][zon_py][0],zlmap[zon_px][zon_py][1],
			zo_come[i][0],
			(*mcome[ zo_come[i][0] ]).ncom,
			(*mcome[ zo_come[i][0] ]).px,
			(*mcome[ zo_come[i][0] ]).py,
			zo_come[i][1]
			);


#endif
	}


	/*
	 * Ordeno el vector comercio/distancia por distancias, de menor a mayor
	 * como esta incluido en el vector el comercio de la trx 'cabecera'
	 * este aparece con distancia 10000 ... al fonde de la lista
	 * para que no sea elegido de nuevo 
	 *
	 * ordenar sort 
	 */

	q0 = zlmap[zon_px][zon_py][1]; 

	for (i=0; i<q0-1; i++)
		for (j=i+1; j<q0; j++)
			if ( zo_come[j][1] < zo_come[i][1] )
			{
				z0 = zo_come[j][0];
				z1 = zo_come[j][1];

				zo_come[j][0] = zo_come[i][0];
				zo_come[j][1] = zo_come[i][1];

				zo_come[i][0] = z0;
				zo_come[i][1] = z1;

			}

#if 0
	for (i=0; i<q0; i++)
	{
		printf ("i: %4d zon (%d,%d)  zlmap: %5d %5d  zo_come[i][0]: %5d  ncom: %5d (%5d,%5d) dist: %6d \n",
			i,
			zon_px,zon_py, zlmap[zon_px][zon_py][0],zlmap[zon_px][zon_py][1],
			zo_come[i][0],
			(*mcome[ zo_come[i][0] ]).ncom,
			(*mcome[ zo_come[i][0] ]).px,
			(*mcome[ zo_come[i][0] ]).py,
			zo_come[i][1]
			);

	}
		printf ("\n\n");
	
#endif


}




/*
 * -----------------------------------------------------------------------------------
 *
 *	cambio_persona
 *
 *	marca una persona cada N cambios ....
 *
 * -----------------------------------------------------------------------------------
 */

/*
 *	Para generar transacciones fraudulentas al azar ,
 *	elegimos cada tanto una persona, y sobre esa,
 *	generamos transacciones sesgadas.
 *	ej
 *	Si tenemos 200,000 personas, y hacemos modulo 100,
 *	tendremos 2000 casos de transacciones fraudulentas ...
 *	en 135,000,000 de transacciones .... mmmm poco ..
 */


int	cambio_persona(q)
int	q;
{
	static	int	q_per = 0;

	if ( q % 100 == 0 )
	{
		q_per++;
		printf ("PER: cambio de persona nro ... %6d   cantidad seleccionadas para trx c/ frad %d \n",q,q_per);
	}

	
}

/*
 * -----------------------------------------------------------------------------------
 *
 *	determina_vector_elegibles
 *
 *	enumera en vector, los negocios que se pueden utilizar para compras
 *
 * -----------------------------------------------------------------------------------
 */


int	determina_vector_elegibles()
{
	int	i,j;
	int 	px,py;
	int	i_vec;

	px = tr1.tx_per_px;
	py = tr1.tx_per_py;



	static	int	pepe=0;

	i_vec = 0;

	for (px=0; px<MAX_FCM; px++)
		for (py=0; py<MAX_FCM; py++)
		{

			if (el_mapa[px][py])
			{

				for (i=0; i< zlmap[px][py][1]; i++)
				{

					el_come[i_vec] = (*mcome[ zlmap[px][py][0] + i ]).ncom ;
					
#if 0
			printf ("(%d,%d) el_mapa: %d zlmap: %5d %5d i: %4d  i_vec: %5d  el_come[i_vec]: %5d  ncom: %5d \n",
						px,py,el_mapa[px][py],
						zlmap[px][py][0],zlmap[px][py][1],
						i,i_vec,
						el_come[i_vec],
						(*mcome[ el_come[i_vec] ]).ncom
						);

#endif

					i_vec++;
		
					if (i_vec == MAX_VCO)
						error(999);
				}

			}
		}



#if 0
	printf ("\n\nCE (%06d) Cantidad de comercios elegibles  %6d \n\n",pepe,i_vec);
#endif

	q_vec = i_vec;
	pepe++;

}

/*
 * -----------------------------------------------------------------------------------
 *
 *	determina_elegibles
 *
 *	marca las zonas de las que se buscaran comercios al azar 
 *
 * -----------------------------------------------------------------------------------
 */


int	determina_elegibles()
{
	int	i,j;
	int 	px,py;

	px = tr1.tx_per_px;
	py = tr1.tx_per_py;
	

#if 0
	for (i = px - 3; i <= px + 3; i++ )
		for (j = py - 3 ; j <= py + 3; j++)
		{
			if ( i >= 0 && j >= 0 && i < MAX_FCM && j < MAX_FCM )
			{
				el_mapa[i][j] = 4;
			}
		}
#endif


	for (i = px - 2; i <= px + 2; i++ )
		for (j = py - 2 ; j <= py + 2; j++)
		{
			if ( i >= 0 && j >= 0 && i < MAX_FCM && j < MAX_FCM )
			{
				el_mapa[i][j] = 3;
			}
		}

	for (i = px - 1; i <= px + 1; i++ )
		for (j = py - 1 ; j <= py + 1; j++)
		{
			if ( i >= 0 && j >= 0 && i < MAX_FCM && j < MAX_FCM )
			{
				el_mapa[i][j] = 2;
			}
		}

	el_mapa[px][py]=1;

#if 0
	printf ("Matrix elegible ... \n\n");

	for (i = px - 3; i <= px + 3; i++ )
		for (j = py - 3 ; j <= py + 3; j++)
		{
			if ( i >= 0 && j >= 0 && i < MAX_FCM && j < MAX_FCM )
			{
				printf ("%d %d %d\n",i,j,el_mapa[i][j]);
			}
		}
		
	printf ("\n\n");
#endif

}

/*
 * -----------------------------------------------------------------------------------
 *
 *	genera_rubros
 *
 *	genera los rubros de comercios 
 *
 * -----------------------------------------------------------------------------------
 */

/*
 *	v2. 2020_08_03
 *	- definimos MAX_TIC como la cantidad de tipos de rubros ...
 *	- todos tienen MAX_RXT rubros
 *	- los numeros de rubro, ordenados al azar, van todos en una unica matriz
 *	
 */

int	genera_rubros()
{
	int	i,j,k,qn;
	int	px,py;
	int	com_offset;
	int	ptr;
	int	i_tic,i_rub;


	/* proceso  */
	if (gp_fverbose("d1"))
	{	printf ("%s genera_rubros \n\n",gp_tm());
	}


	q_rxt = MAX_RXT;
	q_tic = MAX_TIC;
	q_rub = 0;


	for (i_tic=0; i_tic<q_tic; i_tic++)
	{

		for (i_rub=0; i_rub<q_rxt; i_rub++)
		{
			ptr = q_rxt * i_tic + i_rub;

		printf ("genero rubro para ptr: %4d\n\n",ptr);

			mlrub[ptr] = (lrubptr ) malloc(sizeof(lrub));

			(*mlrub[ptr]).ru_nro = ptr;
			(*mlrub[ptr]).ru_tic = i_tic;
			sprintf( (*mlrub[ptr]).ru_mtic, "%s", mtic[i_tic] );
			sprintf( (*mlrub[ptr]).ru_nom,   "t_%02d_r_%03d",i_tic,i_rub);
			sprintf( (*mlrub[ptr]).ru_desc, "nro_%04d_tic_%02d_%s_rub_%03d",ptr,i_tic,mtic[i_tic],i_rub);

			q_rub++;

		}
	}
	
	printf ("voy a imprimir ..%d rubros .. \n\n",q_rub);


	if (gp_fverbose("d3"))
	{
		for (i_rub = 0; i_rub < q_rub; i_rub++ )
		{
			printf ("%8d %03d %d %s %s %s\n",
				i_rub,
				(*mlrub[i_rub]).ru_nro,
				(*mlrub[i_rub]).ru_tic,
				(*mlrub[i_rub]).ru_mtic,
				(*mlrub[i_rub]).ru_nom,
				(*mlrub[i_rub]).ru_desc
				);
		}
	}



#if 0
	for (i=0; i<q_rub ; i++)
	{
		mlrub[i] = (lrubptr ) malloc(sizeof(lrub));

		/* tabla de rubros ... 
		 * primeros offset70 ... Normal
		 * segundos offset10 ... Rural
		 * terceros offset20 ... Caro
		 */

		if ( ofs070 <= i && i < ofs010 )
			ptr = 0;
		if ( ofs010 <= i && i < ofs020 )
			ptr = 1;
		if ( ofs020 <= i )
			ptr = 2;


		(*mlrub[i]).nro = i;
		sprintf( (*mlrub[i]).nm,   "rubro_%04d",i);
		sprintf( (*mlrub[i]).desc, "rub_%04d_%s_descr",i,tic[ptr]);
	}


	if (gp_fverbose("d3"))
	{
		for (i=0; i<q_rub; i++)
		{	
			printf ("%8d %03d %s %s\n",
				i,
				(*mlrub[i]).nro,
				(*mlrub[i]).nm,
				(*mlrub[i]).desc
				);
		}
	}

#endif

}




/*
 * -----------------------------------------------------------------------------------
 *
 *	abro_files
 *
 *	
 *
 * -----------------------------------------------------------------------------------
 */

int	abro_files()
{


	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra a abro_files \n\n",gp_tm());
	}

	if ( ffinp && ((hfinp = fopen (finp,"r")) == NULL) )
	{
		error(101);
	}

	if ( ffout && ((hfout = fopen (fout,"w")) == NULL) )
	{
		error(102);
	}

	if ( fflog && ((hflog = fopen (flog,"w")) == NULL) )
	{
		error(130);
	}

	if ( fftst && ((hftst = fopen (ftst,"r")) == NULL) )
	{
		error(116);
	}

	

	if ( ffpmp )
	{
		if ( (hfpmp = fopen (fpmp,"r")) == NULL )
		{
			error(107);
		}

		bpmp_load();
	}

	if ( ffgdc && ((hfgdc = fopen (fgdc,"w")) == NULL) )
	{
		error(113);
	}


	if ( ffcde )
	{
		if ( (hfcde = fopen (fcde,"r")) == NULL )
		{
			error(108);
		}

		bcde_load();
	}

	if ( ffgco && ((hfgco = fopen (fgco,"w")) == NULL) )
	{
		error(109);
	}

	if ( ffcom )
	{
		if ( (hfcom = fopen (fcom,"r")) == NULL )
		{
			error(110);
		}

		bcom_load();
	}

	if ( ffgru && ((hfgru = fopen (fgru,"w")) == NULL) )
	{
		error(111);
	}

	if ( ffrub )
	{
		if ( (hfrub = fopen (frub,"r")) == NULL )
		{
			error(112);
		}

		brub_load();
	}

	if ( fftb1 )
	{
		if ( (hftb1 = fopen (ftb1,"r")) == NULL )
		{
			error(115);
		}

		btb1_load();
	}

	if ( fftb2 )
	{
		if ( (hftb2 = fopen (ftb2,"r")) == NULL )
		{
			error(114);
		}

		btb2_load();
	}

	if ( fftb3 )
	{
		if ( (hftb3 = fopen (ftb3,"r")) == NULL )
		{
			error(119);
		}

		btb3_load();
	}

	if ( fftb4 )
	{
		if ( (hftb4 = fopen (ftb4,"r")) == NULL )
		{
			error(121);
		}

		btb4_load();
	}

	if ( ffper )
	{
		if ( (hfper = fopen (fper,"r")) == NULL )
		{
			error(117);
		}

		bper_load();
	}

	if ( ffgdp && ((hfgdp = fopen (fgdp,"w")) == NULL) )
	{
		error(118);
	}

	if ( ffgpe && ((hfgpe = fopen (fgpe,"w")) == NULL) )
	{
		error(120);
	}

	if ( ffcpe )
	{
		if ( (hfcpe = fopen (fcpe,"r")) == NULL )
		{
			error(123);
		}

		bcpe_load();
	}

	if ( ffgtx && ((hfgtx = fopen (fgtx,"w")) == NULL) )
	{
		error(122);
	}

	if ( fftrx )
	{
		if ( (hftrx = fopen (ftrx,"r")) == NULL )
		{
			error(124);
		}

		btrx_load();
	}


	if ( fftru && ((hftru = fopen (ftru,"w")) == NULL) )
	{
		error(125);
	}

	if ( ffsta && ((hfsta = fopen (fsta,"r")) == NULL) )
	{
		error(126);
	}

	if ( ffwek && ((hfwek = fopen (fwek,"w")) == NULL) )
	{
		error(127);
	}

	if ( fffxa )
	{
		if ( (hffxa = fopen (ffxa,"r")) == NULL )
		{
			error(128);
		}

		bfxa_load();
	}


	if ( ffaux && ((hfaux = fopen (faux,"w")) == NULL) )
	{
		error(129);
	}





#if 0
	if (gp_diccionario)
	{
		strcpy(fou2,df(fdic));

		if ( (hfdic = fopen (fdic,"r")) == NULL )
		{
			error(103);
		}

		if ( (hfou2 = fopen (fou2,"w")) == NULL )
		{
			error(106);
		}

		bd_load();
	}

	if (gp_tabmrk)
	{
		if ( (hfmrk = fopen (fmrk,"r")) == NULL )
		{
			error(104);
		}

		bm_load();
	}


	if (gp_tipneg == 2)
	{
		if ( (hfgrm = fopen (fgrm,"r")) == NULL )
		{
			error(105);
		}

		bg_load();
	}

#endif 

	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de abro_files \n\n",gp_tm());
	}


}



/*
 * -----------------------------------------------------------------------------------
 *
 *	cierro_files
 *
 *	
 *
 * -----------------------------------------------------------------------------------
 */


int	cierro_files()
{

	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("%s Entro a cierro_files \n\n",gp_tm());
	}


	if ( ffinp)
		fclose(hfinp);

	if ( ffout)
		fclose(hfout);

	if ( fflog)
		fclose(hflog);

	if ( fftst)
		fclose(hftst);

	if ( ffpmp)
		fclose(hfpmp);

	if ( ffcde)
		fclose(hfcde);

	if ( ffgco)
		fclose(hfgco);

	if ( ffcom)
		fclose(hfcom);

	if ( ffgru)
		fclose(hfgru);

	if ( ffrub)
		fclose(hfrub);

	if ( ffgdc)
		fclose(hfgdc);

	if ( fftb1)
		fclose(hftb1);

	if ( fftb2)
		fclose(hftb2);

	if ( fftb3)
		fclose(hftb3);

	if ( fftb4)
		fclose(hftb4);

	if ( ffper)
		fclose(hfper);

	if ( ffgdp)
		fclose(hfgdp);

	if ( ffgpe)
		fclose(hfgpe);

	if ( ffcpe)
		fclose(hfcpe);

	if ( ffgtx)
		fclose(hfgtx);

	if ( fftrx)
		fclose(hftrx);

	if ( fftru)
		fclose(hftru);

	if ( ffsta)
		fclose(hfsta);

	if ( ffwek)
		fclose(hfwek);

	if ( fffxa)
		fclose(hffxa);

	if ( ffaux)
		fclose(hfaux);



#if 0
	if (gp_diccionario)
	{
		fclose(hfdic);
		fclose(hfou2);
	}

	if (gp_tabmrk)
		fclose(hfmrk);
	if (gp_tipneg == 2)
		fclose(hfgrm);

#endif

	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de cierro_files \n\n",gp_tm());
	}



}



/*
 * -----------------------------------------------------------------------------------
 *
 *	proceso_weka
 *
 *	calcula atributos para archivo weka
 *
 * -----------------------------------------------------------------------------------
 */

/*
 *	Para explorar con ML, la propuesta es hacer el analisis
 *	de las ultimas N transacciones de una persona, al momento de
 *	evaluar una transaccion en particular ...
 *
 *	Por lo tanto, necesitamos ... 
 *	- al procesar una transaccion ...
 *	- obtener rapidamente las ultimas N transacciones de esa persona !!!
 *	- es recorrer el archivo al reves !!!!!
 *	- y hacer un indice ??? 
 *
 *	Atenti ..
 *	Este proceso, para hacerlo rapido en una lap ... requiere
 *	el arhivo de trx ordenado por  persona - fecha - hora
 *	Para un archivo de trx tipo pipeline ... hay que recorrer
 *	todo el archivo y juntar las trx de una persona !!!!
 */


int	proceso_weka()
{
	char	b1[MAXB];
	char	b2[MAXB];
	char	*x;

	int	tnv[10];		/* tabla 'no van' */
	int	i,j,k;
	int	i1,i2,i3;
	int	j1,j2,j3;
	int	q0,q1,q2,q3,q7;
	int	f1,f2,f3,f4,f5,f6,f7,f8;
	int	n1,n2;

	int	flag;

	int	i_idx;
	int	q_idx;
	int	i_per;

	int	t_trx_per;		/* total de trx de la persona */
	int	i_per_ant;		/* persona anterior */

					/* variables weka ... */
	int	w1,w2,w3;
	int	h1;			/* hipotesis */

	int	at1,at2,at3,at4,at5,at6,at7,at8,at9;

	h1 = 3;				/* verifico con las ultimas h1 trx de la persona */

	/* proceso  */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra a proceso weka \n\n",gp_tm());

	}


/* weka */
#if 1

	/* necesitamos transacciones indicadas en -inp ...  */
	if ( !ffinp)
		error(813);


	/* algunos inits */
	for (i=0; i< MAX_TRX; i++)
	{
		mtrnx[i] = (trnxptr ) malloc(sizeof(trnx));
			if (mtrnx[i] == NULL )
				error(906);
	}



	i_idx      =  0;
	i_per_ant  = -1;
	flag       =  1;
	f4         =  0;


	/* leo reg x reg del archivo de input */
	while (flag)
	{
		x = fgets(b1,MAXB,hfinp);

		if ( x == NULL )
		{	
			f4 = 1;
			flag = 0;
		}

		if ( x != NULL)
		{
			memset ( &tr_s , 0 , sizeof ( tr_s) );
			csv_parser(b1,fp_trx);
			i_per = tr_s.tx_per_nro;


			if ( i_per_ant == i_per )
			{
				memcpy ( mtrnx[i_idx], &tr_s, sizeof (trnx) );
				i_idx++;
				f4 = 0;
			}

			if ( i_per_ant != i_per  )
			{
				if ( i_per_ant == -1 )
				{
					f4 = 0;
				}

				if ( i_per_ant != -1 )
				{
					f4 = 1;
				}
			}
		}

		/* 
		 *	En este punto ....
		 *	tengo TODAS las transacciones de una persona 
		 *	ordenadas por fecha - hora
		 *	A partir aca ... x cada registro, tomando de
		 *	a N ... hago el calculo de atributos !!! 
		 */

		if (f4)
		{
#if 0
			/* tengo que ordenarlos en la memoria (segun se requiera)  */
			q0 = i_idx;

			for (i1=0; i1<q0-1; i1++)
				for (j1=i1+1; j1<q0; j1++)
					if ( compara ( mtrnx[j1],mtrnx[i1] ) > 0 ) 
					{
						mtrnx[q0] = mtrnx[j1];
						mtrnx[j1] = mtrnx[i1];
						mtrnx[i1] = mtrnx[q0];
					}
			}
#endif

			/* verifico tiene trx con fraude */
			f5 = 0;
			for (i=0; i< i_idx; i++)
			{
				if ( (*mtrnx[i]).tx_cal_n2 == 1)
					f5++;
			}
			
#if 0
			/* verifico si todas las truchas estan seguidas !! */
			i  = 0;
			f6 = 1;
			f7 = 0;		/* asumo que estan todas consecutivas */
			f8 = 1;
			q7 = 0;

			for (j=0; f6 && j< i_idx; j++)
			{
				if ( (*mtrnx[i+j]).tx_cal_n2 == 1 )
				{
					k=0;
					while (f8)
					{
						if ( (*mtrnx[i+j+k]).tx_cal_n2 == 1  )
							q7++;
						if ( (*mtrnx[i+j+k]).tx_cal_n2 == 0  )
						{	tnv[f7] = i+j+k;
							f7++;
						}

						if (q7 == f5)
							f8=0;
						k++;
						if (k >100)
						{
							error(604);  
						}
					}
				}

			}

#endif
		
			/* prueba */
			if (gp_fverbose("d3"))
			{	printf ("WEKA: per %10d con %10d regs (%10d fra) \n",i_per_ant,i_idx,f5);
			}

			/* calculo de atributos ... es ACA */

			q_idx = i_idx;

			for (w1 = 0; w1 < q_idx - h1; w1++)
			{
				at1 = calculo_at1(w1,w1+h1-1);
				at2 = calculo_at2(w1,w1+h1-1);	
				at3 = calculo_at3(w1,w1+h1-1);	
				at4 = calculo_at4(w1,w1+h1-1);	
				at5 = calculo_at5(w1,w1+h1-1);	
				at6 = calculo_at6(w1,w1+h1-1);	
				at7 = calculo_at7(w1,w1+h1-1);	
				at8 = calculo_at8(w1,w1+h1-1);	
				at9 = calculo_at9(w1,w1+h1-1);	

			

			/* grabo ... */
			if (ffwek)
			{
				strcpy(b1,"  ");
				if ( (*mtrnx[w1]).tx_cal_n2 == 1)
					strcpy(b1," X");
				
				fprintf (hfwek,"%6d,%6d,%6d,%6d,%6d,%6d,%6d,%6d,%6d,%d %s (%6d) %4d\n"
					,at1,at2,at3,at4,at5,at6,at7,at8,at9,(*mtrnx[w1]).tx_cal_n2,b1,(*mtrnx[w1]).tx_per_nro,w1);
			}
			if (ffout)
			{

			}
			if (ffaux)
			{

			}

			}
			/* for (w1 ... */
			
		}
		/* if (f4 ... */


		if ( i_per_ant != i_per )
		{
			memcpy ( mtrnx[0], &tr_s, sizeof (trnx) );
			i_per_ant = i_per;
			i_idx =  1;
		}

	}

#endif
/* weka */



	/* proceso  */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de proceso weka \n\n",gp_tm());

	}


	
}






/*
 *	calculo sencillo de algunos atributos ...
 *	aprox. tomando en cuenta
 *	- todas las fechas son 2018
 *	- un dia mas o menos no importa (x los meses )
 */



/*
 *	at1
 *	me fijo si las transacciones son mas o menos
 *	conscutivas en fechas
 */

int	calculo_at1(f1,f2)
int	f1;
int	f2;
{
	char	s[MAXV];

	int	i,j,k;
	int	q;
	int	a1,a2,m1,m2,d1,d2;
	
	q=0;

	for (i=f1; i<=f2-1; i++)
	{	

		memset (s,0,16);
		strncpy(s,(*mtrnx[i]).tx_trx_fecha,4); 
		a1 = atoi(s);
	
		memset (s,0,16);
		strncpy(s,(*mtrnx[i]).tx_trx_fecha+4,2);
		m1 = atoi(s);
	
		memset (s,0,16);
		strncpy(s,(*mtrnx[i]).tx_trx_fecha+6,2);
		d1 = atoi(s);


		memset (s,0,16);
		strncpy(s,(*mtrnx[i+1]).tx_trx_fecha,4); 
		a2 = atoi(s);
	
		memset (s,0,16);
		strncpy(s,(*mtrnx[i+1]).tx_trx_fecha+4,2);
		m2 = atoi(s);
	
		memset (s,0,16);
		strncpy(s,(*mtrnx[i+1]).tx_trx_fecha+6,2);
		d2 = atoi(s);


		q +=  abs (  (m1 * 30 + d1 ) - (m2 * 30 + d2 ) );
	}

	return q;
}


/*
 *	at2
 *	me fijo si las transacciones son mas o menos
 *	conscutivas en horas
 */

int	calculo_at2(f1,f2)
int	f1;
int	f2;
{
	char	hora[16];

	int	h1,m1,h2,m2;
	int	q;

	char	b[16];

	int	i,j;

	q = 0;

	for (i=f1; i<=f2-1; i++)
	{
		memset (b,0,16);
		strncpy(b,(*mtrnx[i]).tx_trx_hora,2);
		h1 = atoi(b);
	
		memset (b,0,16);
		strncpy(b,(*mtrnx[i]).tx_trx_hora+2,2);
		m1 = atoi(b);
	
		memset (b,0,16);
		strncpy(b,(*mtrnx[i+1]).tx_trx_hora,2);
		h2 = atoi(b);
	
		memset (b,0,16);
		strncpy(b,(*mtrnx[i+1]).tx_trx_hora+2,2);
		m2 = atoi(b);

		q += abs ( ( h1 * 60 + m1 ) + ( h2 * 60 + m2 ) );
	
	}

	return q;		

}

/*
 *	at3
 *	me fijo si las transacciones son mas o menos
 *	en negocios cercanos !! (este vale oro )
 */

int	calculo_at3(f1,f2)
int	f1;
int	f2;
{
	int	q;
	int	i;

	q = 0;

	for (i=f1; i<=f2-1; i++)
	{
		q += abs ( (*mtrnx[i]).tx_com_px - (*mtrnx[i+1]).tx_com_px ) + 
		     abs ( (*mtrnx[i]).tx_com_py - (*mtrnx[i+1]).tx_com_py ) ; 
	}

	return q;
}




/*
 *	at4
 *	me fijo cuantos rubros diferentes de los comercios
 *	(este de relleno )
 */

int	calculo_at4(f1,f2)
int	f1;
int	f2;
{
	int	q;
	int	i;
	int	r,a;

	r = (*mtrnx[f1]).tx_com_nrub;

	q = 0;

	for (i=f1+1; i<=f2-1; i++)
	{
		a = 0;
		if ( r != (*mtrnx[i]).tx_com_nrub )
			a = 1;
 
		q += a; 
	}

	return q;
}



/*
 *	at5
 *	me fijo si nro de la trx es par o impar
 *	(este de relleno )
 */

int	calculo_at5(f1,f2)
int	f1;
int	f2;
{
	int	q;
	int	i;
	int	r,a;

	q = (*mtrnx[f1]).tx_trx_nro % 2;

	return q;
}



/*
 *	at6
 *	me fijo si la suma de los montos es 'alto'
 *	(este de relleno )
 */

int	calculo_at6(f1,f2)
int	f1;
int	f2;
{
	int	q;
	int	i;
	int	r,a;

	a = 0;


	for (i=f1; i<=f2-1; i++)
	{
		a += (*mtrnx[i]).tx_trx_monto ;
	}

	q = 0;
	if ( a > 15000 )
		q = 1;

	return q;
}



/*
 *	at7
 *	numero al azar (ruido)
 *	(este de relleno )
 */

int	calculo_at7(f1,f2)
int	f1;
int	f2;
{
	int	q;
	int	i;
	int	r,a;

	q = gna(f1,f2);

	return q;
}


/*
 *	at8
 *	numero al azar (ruido)
 *	(este de relleno )
 */

int	calculo_at8(f1,f2)
int	f1;
int	f2;
{
	int	q;
	int	i;
	int	r,a;

	q = gna(f1,f2);

	return q;
}


/*
 *	at9
 *	numero al azar (ruido)
 *	(este de relleno )
 */

int	calculo_at9(f1,f2)
int	f1;
int	f2;
{
	int	q;
	int	i;
	int	r,a;

	q = gna(f1,f2);

	return q;
}




















/*
 * -----------------------------------------------------------------------------------
 *
 *	proceso_stat
 *
 *	estadisticas de un archivo de transacciones
 *
 * -----------------------------------------------------------------------------------
 */


int	proceso_stat()
{
	char	b1[MAXB];

	int	i,j,k;
	int	q1,q2,q3;

	int	i_idx;
	int	t_cnt;



	/* proceso  */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra a proceso stats \n\n",gp_tm());

	}

	if ( !ffsta)
		error(812);
		

	i_idx = 0;
	t_cnt = 0;

	/* leo reg x reg del archivo de input */
	while (fgets(b1,MAXB,hfsta) != NULL)
	{
		memset ( &tr_s , 0 , sizeof ( tr_s) );

		csv_parser(b1,fp_trx);

		if ( tr_s.tx_cal_n2 == 1)
			t_cnt++;

		i_idx++;
	}




	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("Cantidad de registros           %10d \n",i_idx);
		printf ("Cantidad de registros c/fraud   %10d \n",t_cnt); 
	}


	/* proceso  */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de proceso stats \n\n",gp_tm());

	}


	
}





/*
 * -----------------------------------------------------------------------------------
 *
 *	proceso_fxa 
 *
 *	fix a .. es para revisar que no hayan quedado trx validas entre las truchas
 *
 * -----------------------------------------------------------------------------------
 */

int	proceso_fxa()
{
	char	b1[MAXB];

	int	flag;
	int	i,j,k;
	int	q1,q2,q3;

	int	f1,f2,f3,f4;
	int	n1,n2;

	int	i_idx;
	int	i_per;

	int	t_trx_per;		/* total de trx de la persona */
	int	t_per_fxa;		/* total de personas que hay que arreglar  ! */

	int	i_per_ant;
	int	i_per_tot;

	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra proceso fxa  \n\n",gp_tm());
	}
		

	/* paso 1 ... detectarlos !! */

	t_per_fxa =  0;
	i_per_ant = -1;
	i_per_tot =  0;
	i         =  0;
	flag      =  1;

	while (flag)
	{

		if ( (*mrfxa[i]).fxa_per != i_per_ant )
		{
			i_per = (*mrfxa[i]).fxa_per ;
			i_per_tot++;
			i_per_ant = i_per;
			t_trx_per =  0;


		/* a ver cuantas trx tiene */
		f1 = 1;
		for (j=0; f1 && i+j < q_fxa ; j++)
		{	
			if ( (*mrfxa[i+j]).fxa_per == i_per )
				t_trx_per++;
			else
			{	
				f1=0;
			}
		}


		/* verifico que estan las truchas !! */
		f2 = 0;
		for (j=0; j<t_trx_per; j++)
			if ( (*mrfxa[i+j]).fxa_cal_n1 == 1)
				f2++;

		/* verifico si todas las truchas estan seguidas !! */
		f3 = 1;
		f4 = 0;		/* asumo que estan todas consecutivas */

		for (j=0; f3 && j<t_trx_per; j++)
		{
			if ( (*mrfxa[i+j]).fxa_cal_n1 == 1 )
			{
				f3 = 0;

				for (k=0; k<f2; k++)
				{
					if ( (*mrfxa[i+j+k]).fxa_cal_n1 == 0)
						f4 = 1;
				}
			}
		}

		
		

		printf ("i_per: %10d cantidad trx: %10d  cant fd: %2d   cons?(0 si 1 no): %d \n",i_per,t_trx_per,f2,f4);


		if (f4)
			t_per_fxa++;



		}

		/* sigo ?? */
		i++;

		if (i >=  q_fxa)
			flag = 0;
	}

	printf ("Revise personas : %10d\n",i_per_tot);
	printf ("Hay que arreglar: %10d\n",t_per_fxa);

	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de proceso fxa  \n\n",gp_tm());
	}
}











/*
 * -----------------------------------------------------------------------------------
 *
 *	mostrar_cargas
 *
 *	mostrar resultados de cargas, si los hubo
 *
 * -----------------------------------------------------------------------------------
 */

int	mostrar_cargas()
{

	int px,py;

	/* proceso  */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra a mostrar cargas \n\n",gp_tm());
	}

	/* si hubo cargas, las muestro aca */

	/* proceso  */
	if (gp_fverbose("d3"))
	{	

		/* si cargo mapa */
		if ( ffpmp )
			bpmp_print();
	
		/* si cargo tabla de factores de multip */
		if ( fftb1 )
			btb1_print();
	
		/* si cargo tabla de factores de multip */
		if ( fftb2 )
			btb2_print();
	
		/* si cargo tabla de perfiles */
		if ( ffper )
			bper_print();
	
	}

	/* proceso  */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de mostrar cargas \n\n",gp_tm());
	}

}








/*
 * -----------------------------------------------------------------------------------
 *
 *	gna
 *
 *	genera numero al azar
 *	numero = rand () % (N-M+1) + M;   // Este está entre M y N
 *
 * -----------------------------------------------------------------------------------
 */


int	gna(min,max)
int	min;
int	max;
{
	int nro;

	nro = rand() % (max-min+1) + min;

	return nro;
}



/*
 * -----------------------------------------------------------------------------------
 *
 *	gnf
 *
 *	genera numero al azar
 *	numero = rand () % (N-M+1) + M;   // Este está entre M y N
 *
 * -----------------------------------------------------------------------------------
 */


char	*gnf(mes)
int	mes;
{
	static	char	fecha[16];
	int	a,m,d;

	a = 2018;

	m = mes+1;

	if ( m == 2 )		
		d = gna(1,28);

	if ( m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12)
		d = gna(1,31);

	if ( m == 4 || m == 6 || m == 9 || m == 11 )
		d = gna(1,30);


	sprintf (fecha,"%04d%02d%02d",a,m,d);

	return fecha;
}





/*
 * -----------------------------------------------------------------------------------
 *
 *	gnh
 *
 *	genera hora al azar
 *	numero = rand () % (N-M+1) + M;   // Este está entre M y N
 *
 * -----------------------------------------------------------------------------------
 */


char	*gnh(desde,hasta)
int	desde;
int	hasta;
{
	static	char	hora[16];
	int	h,m,s;

	h = gna(desde,hasta);
	m = gna(0,59);
	s = gna(0,59);

	sprintf (hora,"%02d%02d%02d",h,m,s);

	return hora;
}





/*
 * -----------------------------------------------------------------------------------
 *
 *	gnf_fd
 *
 *	Genera fecha al azar tuneada para trx truchas 
 *
 *
 * -----------------------------------------------------------------------------------
 */


char	*gnf_fd(mes)
int	mes;
{
	static	char	fecha[16];
	int	a,m,d;


	a = 2018;

	m = mes+1;

	d = gna(4,25);

	sprintf (fecha,"%04d%02d%02d",a,m,d);

	return fecha;
}




/*
 * -----------------------------------------------------------------------------------
 *
 *	gnf_fd2
 *
 *	Genera fecha al azar tuneada para trx truchas 
 *
 *
 * -----------------------------------------------------------------------------------
 */


char	*gnf_fd2(tr,idx)
trnxptr	tr;
int	idx;
{
	static	char	fecha[16];
	int	a,m,d;

	char	s[16];

	
	memset (s,0,16);
	strncpy(s,(*tr).tx_trx_fecha,4);
	a = atoi(s);

	memset (s,0,16);
	strncpy(s,(*tr).tx_trx_fecha+4,2);
	m = atoi(s);

	memset (s,0,16);
	strncpy(s,(*tr).tx_trx_fecha+6,2);
	d = atoi(s);




	/* ojo, relacionado con i4 de genera_fraude !!   */
	if (idx > 6)
		d++;

	sprintf (fecha,"%04d%02d%02d",a,m,d);

#if 0
	printf ("gnf_fd2: %4d %4d %4d (%s) \n",a,m,d,fecha);
#endif

	return fecha;
}



/*
 * -----------------------------------------------------------------------------------
 *
 *	gnh_fd2
 *
 *	Genera hora al azar tuneada para trx truchas 
 *
 *
 * -----------------------------------------------------------------------------------
 */


char	*gnh_fd2(tr,idx)
trnxptr	tr;
int	idx;
{
	static	char	hora[16];
	int	h,m,s;

	int	h1,m1;
	int	ext;

	char	b[16];

	
	memset (b,0,16);
	strncpy(b,(*tr).tx_trx_hora,2);
	h = atoi(b);

	memset (b,0,16);
	strncpy(b,(*tr).tx_trx_hora+2,2);
	m = atoi(b);

	memset (b,0,16);
	strncpy(b,(*tr).tx_trx_hora+4,2);
	s = atoi(b);

	ext = h * 60 + m + gna(50,90);
	
	h = ext / 60;
	m = ext % 60;
	s = gna(1,59);	

	/* si cambio la fecha, vamos a un horario mas temprano !!!  */
	if (idx > 6)
	{
		h = gna(8,10);
		m = gna(1,20);
		s = gna(1,59);
	}

	sprintf (hora,"%02d%02d%02d",h,m,s);

#if 0
	printf ("fnh_fd2:  %4d %4d %4d   ext: %3d %2d %2d  %s \n",h,m,s,ext,h1,m1,hora);
#endif

	return hora;
}



/*
 * -----------------------------------------------------------------------------------
 *
 *	gsf 
 *
 *	fecha en formato mas 'readabily' para imprimir
 *
 * -----------------------------------------------------------------------------------
 */



char	*gsf(f)
char	*f;
{
	static	char	fecha[16];
	int	a,m,d;

	char	s[16];

	
	memset (s,0,16);
	strncpy(s,f,4);
	a = atoi(s);

	memset (s,0,16);
	strncpy(s,f+4,2);
	m = atoi(s);

	memset (s,0,16);
	strncpy(s,f+6,2);
	d = atoi(s);


	sprintf (fecha,"%04d/%02d/%02d",a,m,d);

	return fecha;
}





/*
 * -----------------------------------------------------------------------------------
 *
 *	gsh
 *
 *	Genera string con hora para imprimir
 *
 *
 * -----------------------------------------------------------------------------------
 */


char	*gsh(sh)
char	*sh;
{
	static	char	hora[16];
	int	h,m,s;

	int	h1,m1;
	int	ext;

	char	b[16];

	
	memset (b,0,16);
	strncpy(b,sh,2);
	h = atoi(b);

	memset (b,0,16);
	strncpy(b,sh+2,2);
	m = atoi(b);

	memset (b,0,16);
	strncpy(b,sh+4,2);
	s = atoi(b);

	sprintf (hora,"%02d:%02d:%02d",h,m,s);

	return hora;
}






/*
 * -----------------------------------------------------------------------------------
 *
 *	azar0200
 *
 *	genera 70 numeros al azar en lista
 *
 * -----------------------------------------------------------------------------------
 */


int	azar0200(ptr)
int	ptr;
{
	int	n[200];
	int	i,j,k;
	int	nro,q;
	int	f1,f2;

	int	tp=200;


	for (i=0; i<tp; i++)
		n[i] = -1;

	k  = 0;
	f2 = 1;

	while (f2)
	{
		nro = rand() % tp;

		for (j=0, f1=1; f1 & j<k; j++)
			if (nro == n[j])
				f1 = 0;
		if (f1)
		{	n[k++] = nro;
		}

		if (k == tp)
			f2 = 0;
	}

	for (i=0; i<tp; i++)
		rn200[ptr][i] = n[i];

#if 0
	for (i=0; i<tp; i++)
		printf ("%2d: %2d\n",i,n[i]);
	printf ("\n\n");
#endif

}



#if 0
/*
 * -----------------------------------------------------------------------------------
 *
 *	azar070
 *
 *	genera 70 numeros al azar en lista
 *
 * -----------------------------------------------------------------------------------
 */


int	azar070()
{
	int	n[70];
	int	i,j,k;
	int	nro,q;
	int	f1,f2;

	int	tp=70;

#if 0
	/* miscelaneas gobales */
	srand (time(NULL));
#endif

	for (i=0; i<tp; i++)
		n[i] = -1;

	k  = 0;
	f2 = 1;

	while (f2)
	{
		nro = rand() % tp;

		for (j=0, f1=1; f1 & j<k; j++)
			if (nro == n[j])
				f1 = 0;
		if (f1)
		{	n[k++] = nro;
		}

		if (k == tp)
			f2 = 0;
	}

	for (i=0; i<tp; i++)
		rn070[i] = n[i];

#if 0
	for (i=0; i<tp; i++)
		printf ("%2d: %2d\n",i,n[i]);
	printf ("\n\n");
#endif

}



/*
 * -----------------------------------------------------------------------------------
 *
 *	azar020
 *
 *	genera 20 numeros al azar en lista
 *
 * -----------------------------------------------------------------------------------
 */


int	azar020()
{
	int	n[20];
	int	i,j,k;
	int	nro,q;
	int	f1,f2;

	int	tp=20;

#if 0
	/* miscelaneas gobales */
	srand (time(NULL));
#endif

	for (i=0; i<tp; i++)
		n[i] = -1;

	k  = 0;
	f2 = 1;

	while (f2)
	{
		nro = rand() % tp;

		for (j=0, f1=1; f1 & j<k; j++)
			if (nro == n[j])
				f1 = 0;
		if (f1)
		{	n[k++] = nro;
		}

		if (k == tp)
			f2 = 0;
	}

	for (i=0; i<tp; i++)
		rn020[i] = n[i];

#if 0
	for (i=0; i<tp; i++)
		printf ("%2d: %2d\n",i,n[i]);
	printf ("\n\n");
#endif

}



/*
 * -----------------------------------------------------------------------------------
 *
 *	azar010
 *
 *	genera 10 numeros al azar en lista
 *
 * -----------------------------------------------------------------------------------
 */


int	azar010()
{
	int	n[10];
	int	i,j,k;
	int	nro,q;
	int	f1,f2;

	int	tp=10;

#if 0
	/* miscelaneas gobales */
	srand (time(NULL));
#endif

	for (i=0; i<tp; i++)
		n[i] = -1;

	k  = 0;
	f2 = 1;

	while (f2)
	{
		nro = rand() % tp;

		for (j=0, f1=1; f1 & j<k; j++)
			if (nro == n[j])
				f1 = 0;
		if (f1)
		{	n[k++] = nro;
		}

		if (k == tp)
			f2 = 0;
	}

	for (i=0; i<tp; i++)
		rn010[i] = n[i];

#if 0
	for (i=0; i<tp; i++)
		printf ("%2d: %2d\n",i,n[i]);
	printf ("\n\n");

#endif
}


#endif


/*
 * -----------------------------------------------------------------------------------
 *
 *	pruebas
 *
 *	codigo aparte para pruebas
 *	(solo si hace falta ... sino, en -pru=1 2 3 ... )
 *
 * -----------------------------------------------------------------------------------
 */


/*
 * -----------------------------------------------------------------------------------
 *
 *	nnparse
 *
 *	parser con reglas tipo negex ampliado
 *
 * -----------------------------------------------------------------------------------
 */


char	formex[1024];


int	nnparse()
{

	int	i,j,k;

	int	hay_mas_reglas;
	int	la_linea_sirve;
	int	regla_actual_match;
	int	ptl,ptk,ptl_ant;
	int	chances;
	int	nro_regla;
	int	sigo,condiciones;
	int	alguna;
	int	tipo;
	int	match;
	int	busco;
	int	regla_que_matcheo;
	int	f1,f_pr;
	int	q_tk_ref;
	int	ref_2p;

	strcpy (formex,"%snro_re: %d  tip: %d  ptk: %2d  tr: |%10.10s|  ptl: %2d  sig: %d  match: %d  cha: %d  cond:  %d  tk:  |%s|\n");

	/* util para debug --- pero en condiciones
	 * normales, demasiada info en el log
	 */
	if (gp_fverbose("d3"))
		m_estado();


	/* set inicial de variables del parser */

	hay_mas_reglas     = 1;
	la_linea_sirve     = 1;
	regla_actual_match = 1;


	nro_regla = 0;
	regla_que_matcheo = 0;

	match = 0;
	busco = 1;
	condiciones = 0;

	q_tk_ref = q_tk;

	/* 
	 * mejorar ...
	 * si puso dos palabras en la referencia a buscar en el scope,
	 * limito la busqueda a la primer palabra de las dos.
	 */

	ref_2p = 0;
	for (i=0; i<strlen(segmento[2])-1; i++)
		if (*(segmento[2] + i) == ' ')
		{	ref_2p = 1;
			segmento[2][i]=0;

			if (gp_fverbose("d3"))
				printf ("\n\nSeparo referencia de busqueda en 2 |%s| \n",
						segmento[2]);
		}


	/*
	 * Mientras flag 'busco' esta encendido, y hay mas reglas para aplicar,
	 * sigo ...
	 */

	match = 0;
	while (  busco && hay_mas_reglas )
	{
		chances = q_tk-1 ;
		ptl = 0;
		ptk = 0;
		ptl_ant = 0;

		rpa = (reglaptr *) &tbr[nro_regla][ptk];

		if (gp_fverbose("d3"))
			printf ("\nAplico Regla: |%s| \n",(**rpa).nm);

		while ( busco && la_linea_sirve )
		{
			if (gp_fverbose("d3"))
				printf ("Entre a la_linea_sirve     \n\n\n");

			chances = q_tk-1 ;
			ptl = ptl_ant;	/* tengo que guardar donde quede ... */
			ptk = 0;
			regla_actual_match = 1;
			f_pr = 1;

			while ( busco && regla_actual_match )
			{
				sigo = 1;
				rpa = (reglaptr *) &tbr[nro_regla][ptk];
				tipo = (**rpa).mrk;
				match = 0;

				if (gp_fverbose("d3"))
				{
					printf (formex,"\nAntes del switch    ",
						nro_regla,tipo, ptk,(*tbr[nro_regla][ptk]).wrd,ptl, sigo,match,chances,condiciones,tk[ptl]);

				}

				switch ( tipo  )
				{
					/* es una palabra sola */
					case 1:	
						if ( (**rpa).tip == 1)
						{	if (strcmp ( (**rpa).wrd , tk[ptl] ) != 0)
								sigo = 0;  
						}
						if ( (**rpa).tip == 2)
						{	if (strncmp ( (**rpa).wrd, tk[ptl], strlen( (**rpa).wrd)  ) != 0 )
								sigo = 0;
						}

						if (sigo)
							match=1;
						break;

					/* numero de tokens intermedios posibles */  
					case 2:
						chances = (**rpa).num;
						break;

					/* es un OR de palabras alternativas */
					case 3:
						alguna = 0;
						rpb = rpa;

						for (j=0; !alguna && j< (**rpa).num; j++)
						{

							if (gp_fverbose("d3"))
								printf ("\nloop case 3 ... sigo:  %d j: %2d  tkn:  |%s|  alt: |%s|  tip: %d  num:  %d   cha:  %d \n\n",
									sigo,j,tk[ptl],(**rpb).wrd,(**rpb).tip,(**rpb).num,chances);

							/* recorro la lista de palabras alternativas */
							if ( (**rpb).tip == 1)
							{	if (strcmp ( (**rpb).wrd , tk[ptl] ) == 0)
									alguna = 1;  
							}
							if ( (**rpb).tip == 2)
							{	if (strncmp ( (**rpb).wrd, tk[ptl], strlen( (**rpb).wrd) ) == 0 )
									alguna = 1;
							}

							rpb = (reglaptr *) & (*rpb)->nx;
						}

						if (alguna == 1 )
							match = 1;

						if (alguna == 0 )
							sigo = 0;

						if (gp_fverbose("d3"))
							printf ("\nCASE 3:  alguna: %d  sigo: %d  \n",alguna,sigo);

						break;

					/* es la palabra de referencia ... una .. o varias !!! (AND) */
					case 4:
#if 0
						if ( strcmp(segmento[2],tk[ptl]) != 0 )
							sigo = 0; 
#endif
						/* mejorar ...
						 * en varios casos, la palabra de referencia no era encontrada en el scope
						 * porque en referencia estaba en singula y en la frase en plurar.
						 * Ej:  'imagen'   'no se encontraron imagenes .... '
						 * En esta version, comparo segun la longitud de la palabra de referencia,
						 * esto soluciona la busqueda de singular / plural 
						 */

						if ( strncmp(segmento[2],tk[ptl],strlen(segmento[2]) ) != 0 )
							sigo = 0; 


						if (sigo)
							match = 1;

						if (gp_fverbose("d3"))
							printf ("Entre CASE 4  seg: |%s|  tk: |%s|  ptl: %2d  ptk: %2d sigo: %d  wrd |%s| \n\n",
								segmento[2], tk[ptl], ptl,ptk,sigo, (**rpa).wrd);
						break;

				}  /* switch ... */

				/*
				 * Guardo la posicion en la linea en la que comienzo a matchear la regla.
				 * Si no termina de matchear toda la regla, es necesario seguir analizando
				 * la misma linea, con la misma regla, pero a partir del token siguiente
				 * al primer token que matcheo en la vuelta anterior 
				 */

				if (f_pr && match == 1 )
				{	f_pr = 0;
					ptl_ant = ptl + 1;

					if (gp_fverbose("d3"))
						printf ("\nSET ptl_ant: %2d\n",ptl_ant);
				}

				if (match == 1)
					chances = 0;

				/* tengo distancia igual a 0, no hubo matchs, y se acabo la linea */
				if (chances == 0 && sigo == 0 && ptl == q_tk_ref )
					condiciones = 0;
				
				/* tengo distancia igual a 0, no hubo matchs, pero todavia hay linea */
				if (chances == 0 && sigo == 0 && ptl < q_tk_ref )
				{	condiciones = 0;
					ptl = nxt_tkn(ptl);
				}
				
				/* tengo distancia igual a 0, hubo matchs y queda linea */
				if (chances == 0 && sigo == 1 && ptl < q_tk_ref )
				{
					condiciones = 1;
					ptl = nxt_tkn(ptl);
					ptk++;
					if (gp_fverbose("d3"))
						printf ("VVV1 ptk: %d\n",ptk);
				}

				/* tengo distancia igual a 0, hubo matchs pero no queda linea  */
				if (chances == 0 && sigo == 1 && ptl == q_tk_ref  )
				{
					condiciones = 0;
				}

				/* llegue al fin de la linea y no paso la primer accion de la regla */
				if (ptk == 0 && ptl == q_tk )
				{
					condiciones = 0;
					if (gp_fverbose("d3"))
						printf ("SET1 ... cond: %d\n\n",condiciones );
					regla_actual_match = 0;
					la_linea_sirve  = 0;
				}


				/* si el token suma chances continuo */
				if (tipo == 2)
				{	condiciones = 1;
					ptk++;
					if (gp_fverbose("d3"))
						printf ("VVV2 ptk: %d\n",ptk);
				}

				/* si el token no suma chances, y no sigo , restar una chance  */
				if (tipo != 2 && sigo == 0 && chances > 0)
				{
					chances--;
					condiciones = 1;
					ptl = nxt_tkn(ptl);
				}
					
				/* si el token no suma chances, y sigo, borrar chanches */
				if (tipo != 2 && sigo == 1 && chances > 0)
				{
					chances=0;
					condiciones = 1;
					ptl = nxt_tkn(ptl);
				}
					
				/* se termino la linea */
				if (tipo !=2 && chances > 0 && ptl == q_tk_ref )
				{
					condiciones = 0;
					if (gp_fverbose("d3"))
						printf ("SET2 ... cond: %d\n\n",condiciones );
					regla_actual_match = 0;
					la_linea_sirve  = 0;

				}


				/* determinar condiciones para seguir con regla actual */
				if ( condiciones == 0 )
				{
					regla_actual_match = 0;
					if (gp_fverbose("d3"))
						printf ("=====>  corta !! \n");
				}

				if (gp_fverbose("d3"))
				{
					if ( tbr[nro_regla][ptk] != (reglaptr) NULL )
					{
						printf (formex,"\nPie w reg act matc  ",
							nro_regla,tipo, ptk,(*tbr[nro_regla][ptk]).wrd,ptl, sigo,match,chances,condiciones,tk[ptl]);
					}
					else
					{
						printf (formex,"\nPie w reg act matc  ",
							nro_regla,tipo, ptk,"vacio",ptl, sigo,match,chances,condiciones,tk[ptl]);
					}

				}

				/* si termino la regla y estamos en 'sigo' ... entonces tenemos match */
				if ( tbr[nro_regla][ptk] == (reglaptr) NULL )
				{
					if (gp_fverbose("d3"))
					{
						printf ("ZZZ lleguamos al final de la regla ... \n");
						printf ("sigo: %d  match: %d  ptk: %d  cha:  %d \n\n\n",sigo,match,ptk,chances);
					}

					regla_actual_match = 0;
					busco = 0;
					regla_que_matcheo = nro_regla + 1;
				}

			} /* while  regla_actual_match ... */

			if (gp_fverbose("d3"))
				printf ("\n\n\nSali del loop de la regla ... ptl_ant: %2d \n\n\n",ptl_ant);

		} /* while  la_linea_sirve ... */

		nro_regla++;
		if (nro_regla == q_grm)
			hay_mas_reglas = 0;

		la_linea_sirve     = 1;
		regla_actual_match = 1;

	} /* while hay_mas_reglas .. */

	if (regla_que_matcheo)
	{	for (f1=1,i=0; f1 && i<q_tk; i++)
			if ( strncmp(segmento[2],tk[i],strlen(segmento[2]) ) == 0 )
				f_pe_lin=1, f1=0;

	}


	return regla_que_matcheo;
}




/*
 * 	nxt_tkn
 *
 * 	necesitamos resolver el tema de que en la lista de tokens,
 * 	aparecen los elementos de los tagg ...
 * 	y esto "entorpece" aplicar las raglas !!!
 *	La resolucion de esto depende mas de la expectativa de uso
 *	de la herramienta, mas que de una cuestion tecnica.
 *	Se espera que los tags puestos en el texto por un tagger
 *	especifico, sean "invisibles" para el analizador de la 
 *	gramatica negex ???
 *
 *	
 *
 */


int	nxt_tkn(l)
int	l;
{
	int	i,j,k;
	int	new_l;
	int	f1,f2;

	new_l	= l+1;

	f1=1;

	while (f1)
	{
		f2 = 0;

		if ( strcmp( tk[new_l],"_" ) == 0 )
			f2=1;

		if ( strcmp( tk[new_l],"NEG") == 0 )
			f2=1;

		if ( strcmp( tk[new_l],"VSE") == 0 )
			f2=1;

		if ( strcmp( tk[new_l],"TNEG") == 0 )
			f2=1;

		if ( strcmp( tk[new_l],"CNEG") == 0 )
			f2=1;

		if ( strcmp( tk[new_l],"ENDNEG") == 0 )
			f2=1;

		if ( strcmp( tk[new_l],"OBS") == 0 )
			f2=1;

		if ( strcmp( tk[new_l],"VIS") == 0 )
			f2=1;

		if ( strcmp( tk[new_l],"EVI") == 0 )
			f2=1;

		if ( strcmp( tk[new_l],"IDE") == 0 )
			f2=1;

		if ( strcmp( tk[new_l],"DET") == 0 )
			f2=1;

		if ( strcmp( tk[new_l],",") == 0 )
			f2=1;

		if ( strcmp( tk[new_l],"COM") == 0 )
			f2=1;



		if (f2)
			new_l++;
		else
			f1=0;
	}

	return new_l;
}

	


/*
 * -----------------------------------------------------------------------------------
 *
 *	separar_seg
 *
 *	separa la linea del corpus en segmentos ...
 *
 * -----------------------------------------------------------------------------------
 */



int	separar_seg(buffer1)
char	*buffer1;
{
	int	i,j,k;
	int	p1,p2,p3,p4;
	int	f1,f2,f3,f4;
	int	m1;

	char	b3[MAXB];


	p1=0;
	p2=0;
	m1=0;

	/* set de marcas */
	if (tipo_marca == 0)
	{	for (i=0, !tipo_marca; i<strlen(buffer1); i++)
		{
			if (!tipo_marca && strncmp(buffer1+i," t1 ",4) == 0)
			{
				strcpy(marcas[0]," t1 ");
				strcpy(marcas[1]," t2 ");
				strcpy(marcas[2]," t3 ");
				tipo_marca = 1;
			}

			if (!tipo_marca && strncmp(buffer1+i," T1 ",4) == 0)
			{
				strcpy(marcas[0]," T1 ");
				strcpy(marcas[1]," T2 ");
				strcpy(marcas[2]," T3 ");
				tipo_marca = 2;
			}

			if (!tipo_marca && strncmp(buffer1+i," TAB1 ",6) == 0)
			{
				strcpy(marcas[0]," TAB1 ");
				strcpy(marcas[1]," TAB2 ");
				strcpy(marcas[2]," TAB3 ");
				tipo_marca = 3;
			}
		}


		if (gp_fverbose("d3"))
		{
			printf ("\n");
			printf ("Tipo de marca :  %d   |%s|  |%s|  |%s|  \n\n",
				tipo_marca,marcas[0],marcas[1],marcas[2]);
		}
	}




	/* Busco primer segmento           */
	while ( p1 < MAXB - strlen(marcas[m1])  &&  strncmp(buffer1+p1,marcas[m1],strlen(marcas[m1]))  )
		p1++;

	if ( p1 == MAXB - strlen(marcas[m1]) )
	{	error(701);
	}

	strncpy(segmento[0],buffer1+p2,p1-p2);
	segmento[0][p1-p2]=0;
	p2 = p1 + strlen(marcas[m1]);



	/* Busco segundo segmento           */
	m1=1;

	while ( p1 < MAXB - strlen(marcas[m1])  &&  strncmp(buffer1+p1,marcas[m1],strlen(marcas[m1]))  )
		p1++;

	if ( p1 == MAXB - strlen(marcas[m1]) )
	{	error(702);
	}

	strncpy(segmento[1],buffer1+p2,p1-p2);
	segmento[1][p1-p2]=0;
	p2 = p1 + strlen(marcas[m1]);



	/* Busco tercer segmento           */
	m1=2;

	while ( p1 < MAXB - strlen(marcas[m1])  &&  strncmp(buffer1+p1,marcas[m1],strlen(marcas[m1]))  )
		p1++;

	if ( p1 == MAXB - strlen(marcas[m1]) )
	{	error(703);
	}

	strncpy(segmento[2],buffer1+p2,p1-p2);
	segmento[2][p1-p2]=0;
	p2 = p1 + strlen(marcas[m1]);


	/* Busco cuarto segmento 
 	 * OJO: en cuarto segmento NO hago strlen ... -1 como 
	 * en los anteriors, sino ... saco el ultimo new line 
	 * y tecnicamente modifico el contenido del registro
	 */

	strcpy(segmento[3],buffer1+p2);
	segmento[3][strlen(segmento[3])]=0;

	/* ojo !!
	 * se cuelan blancos al segmentar la palabra de ref a buscar
	 * algo anda mal en el parser original ...
	 */

	for (i=strlen(segmento[2])-1; segmento[2][i] == ' '; i--)
		segmento[2][i] = 0;


	/* Pasar a minuscula la palabra buscada, para compatibilidad en las comparaciones */
	strcpy (b3,segmento[2]);
	strcpy (segmento[2],pasar_a_minusc(b3));



	/* tengo los 4 segmentos */
	if (gp_fverbose("d5"))
	{
		printf ("Separar: \n");
		printf ("0: |%s|\n",segmento[0]);
		printf ("1: |%s|\n",segmento[1]);
		printf ("2: |%s|\n",segmento[2]);
		printf ("3: |%s|\n",segmento[3]);
	}

}



/*
 * -----------------------------------------------------------------------------------
 *
 *	m_estado
 *
 *	muestra estado de tokens procesados y reglas en memoria
 *
 * -----------------------------------------------------------------------------------
 */


int	m_estado()
{

	int i,j,k,m9;


	printf ("Parsing linea con NGA \n\n");
	
	printf ("Separar: \n");
	printf ("0: |%s|\n",segmento[0]);
	printf ("1: |%s|\n",segmento[1]);
	printf ("2: |%s|\n",segmento[2]);
	printf ("3: |%s|\n",segmento[3]);
		
	printf ("\n\n");
	printf ("Cantidad de tokens ... %3d\n",q_tk);
	for (i=0; i< q_tk; i++)
		printf ("(%3d) %s\n",i,tk[i] );
	printf ("\n\n");

	printf ("Cantidad de reglas ... %3d\n",q_grm);
	for (i=0; i < q_grm; i++)
	{
		j=0;
		while ( tbr[i][j] != (reglaptr) NULL )
		{
			printf ("Reg: %2d  tk: (%2d)          tip: %3d    num: %3d   mrk:  %d |%s|\n",
				i,j,
				(*tbr[i][j]).tip, (*tbr[i][j]).num,(*tbr[i][j]).mrk,(*tbr[i][j]).wrd   );

			if ( (*tbr[i][j]).nx != (reglaptr) NULL )
			{
				rpa = (reglaptr *) &tbr[i][j];
				m9 = ( **rpa).num;

				for ( k=0; k< m9; k++)
				{	
					if (k)
					{
						printf ("                           tip: %3d    num: %3d   mrk:  %d |%s|\n",
							(**rpa).tip, (**rpa).num, (**rpa).mrk, (**rpa).wrd   );
					}

					rpa = (reglaptr *) & (*rpa)->nx;
				}
			}
			j++;

		}  /* while tbr ... */

		printf ("\n");
	}
	printf ("\n\n");
}




/*
 * -----------------------------------------------------------------------------------
 *
 *	bpmp_load
 *	carga la base de datos de mapa
 *
 * -----------------------------------------------------------------------------------
 */

int	bpmp_load()
{
	char	b1[MAXB];
	char	b2[MAXB];
	char	b3[MAXB];
	int	flag;
	int	p1,p2;
	int	i,j,k;
	int	f1;
	int	px,py;
	int	com_offset;

	com_offset=0;

	f1=0;
	q_pmp=0;
	k=0;

	/* proceso */
	if (gp_fverbose("d1"))
	{
		printf ("\nEntra a carga de elementos de mapa  \n\n");
	}

	for (i=0; i<MAX_FCM; i++)
		for (j=0; j<MAX_FCM; j++)
			mmapa[i][j] = (mapaptr) NULL;


	while (fgets(b1,MAXB,hfpmp) != NULL)
	{
		if (gp_fverbose("d3"))
		{
			printf ("%3d %s",q_pmp,b1);
		}

		if (!linea_vacia(b1)  && b1[0] != '#' )
		{
			f1=1;
		}

		if (f1)
		{
			p1=0;
			p2=0;
			k=0;

			while (b1[p1] )
			{	
				b2[k++] = b1[p1];
				
				if (b1[p1] == ',' || b1[p1] == '\n')
				{	
					b2[k-1]=0;

					strcpy(b3,b2);
					if (gp_fverbose("d4"))
						printf ("=%02d= %s\n",p2,b3);

					if (p2 == 0)
					{	
						px = atoi(b3);
					}

					if (p2 == 1)
					{	
						py = atoi(b3);

						mmapa[px][py] = (mapaptr) malloc(sizeof(mapa)) ;
						(*mmapa[px][py]).px = px;
						(*mmapa[px][py]).py = py;

					}

					if (p2 == 2)
					{	
						strcpy( (*mmapa[px][py]).nm,b2);
					}

					if (p2 == 3)
					{	
						strcpy( (*mmapa[px][py]).den,b2);
						(*mmapa[px][py]).iden = densidad(b2);
					}

					if (p2 == 4)
					{	
						strcpy( (*mmapa[px][py]).nse,b2);
						(*mmapa[px][py]).inse = densidad(b2) - 2;
					}

					if (p2 == 5)
					{	
						(*mmapa[px][py]).qneg=atoi(b3);
						
						(*mmapa[px][py]).ptr_p = 0;
						(*mmapa[px][py]).ptr_u = 0;
					}

					p2++;
					k=0;
				}

				p1++;

			}
		

			/* next ... */
			if (q_pmp >= MAX_QEM)
				error(302);
			q_pmp++;
			f1=0;

		}
	}

	if (gp_fverbose("d1"))
	{
		printf ("Cantidad de elementos cargados en mapa : %6d\n\n",q_pmp);
	}


	if (gp_fverbose("d4"))
	{
		printf ("\n\nComprobando integridad de mapa \n\n");

		bpmp_print();
	}


	/* proceso */
	if (gp_fverbose("d1"))
	{
		printf ("\nSale de carga de elementos de mapa  \n\n");
	}

}

/*
 * -----------------------------------------------------------------------------------
 *
 *	bpmp_print
 *
 *	muestra carga de archivo de mapa
 *
 * -----------------------------------------------------------------------------------
 */

int	bpmp_print()
{
	int	px,py;

	for (px=0; px<MAX_FCM ; px++)
		for (py=0; py<MAX_FCM ; py++)
			if ( mmapa[px][py] != (mapaptr ) NULL )
			{
				printf ("%3d %3d %10s %s %s %05d %8d %8d \n",
					(*mmapa[px][py]).px,
					(*mmapa[px][py]).py,
					(*mmapa[px][py]).nm,
					(*mmapa[px][py]).den,
					(*mmapa[px][py]).nse,
					(*mmapa[px][py]).qneg,
					(*mmapa[px][py]).ptr_p,
					(*mmapa[px][py]).ptr_u
					);
			}

}


/*
 * -----------------------------------------------------------------------------------
 *
 *	bcde_load
 *
 *	carga el archivo de definicion de comercios
 *
 * -----------------------------------------------------------------------------------
 */

int	bcde_load()
{
	char	b1[MAXB];
	char	b2[MAXR];
	char	b3[MAXD];
	int	flag;
	int	p1,p2;
	int	i,j,k;
	int	f1;
	int	px,py;
	int	com_offset;

	com_offset=0;

	f1=0;
	q_cde=0;
	k=0;

	if (gp_fverbose("d1"))
	{
		printf ("\nCargando definiciones de comercios  \n\n");
	}

	/* definicion de comercio */

	for (i=0; i<MAX_CDE ; i++)  
		mlcde[i] = (lcdeptr ) NULL;


	while (fgets(b1,MAXB,hfcde) != NULL)
	{
		if (gp_fverbose("d3"))
		{
			printf ("%3d %s",q_cde,b1);
		}

		if (!linea_vacia(b1)  && b1[0] != '#' )
		{
			f1=1;
		}

		if (f1)
		{
			p1=0;
			p2=0;
			k=0;

			while (b1[p1] )
			{	
				b2[k++] = b1[p1];

				if (b1[p1] == ',' || b1[p1] == '\n')
				{	
					b2[k-1]=0;

					if (p2 == 0)
					{	
						strcpy(b3,b2);
						px = atoi(b3);

						if (gp_fverbose("d3"))
							printf ("=%2d= %s\n",p2,b2);
					}

					if (p2 == 1)
					{	
						strcpy(b3,b2);
						py = atoi(b3);

						if (gp_fverbose("d3"))
							printf ("=%2d= %s\n",p2,b2);

						mlcde[q_cde] = (lcdeptr) malloc(sizeof(lcde)) ; 
						(*mlcde[q_cde]).px = px;
						(*mlcde[q_cde]).py = py;

					}



					if (p2 == 2)
					{	
						strcpy(b3,b2);
						strcpy( (*mlcde[q_cde]).nzon,b3);

						if (gp_fverbose("d3"))
							printf ("=%2d= %s\n",p2,b2);

					}

					if (p2 == 3)
					{	
						strcpy(b3,b2);
						strcpy( (*mlcde[q_cde]).mden,b3);


						(*mlcde[q_cde]).iden = densidad( b3 );

						if (gp_fverbose("d3"))
							printf ("=%2d= %s\n",p2,b2);

					}

					if (p2 == 4)
					{	
						strcpy(b3,b2);
						strcpy( (*mlcde[q_cde]).mnse,b3);

						(*mlcde[q_cde]).inse = nivelsocioeconomico( b3 ) -2;

						if (gp_fverbose("d3"))
							printf ("=%2d= %s\n",p2,b2);

					}


					if (p2 == 5)
					{	
						strcpy( (*mlcde[q_cde]).nact,b2);

						if (gp_fverbose("d3"))
							printf ("=%2d= %s\n",p2,b2);

					}

					if (p2 == 6)
					{	
						strcpy( (*mlcde[q_cde]).den,b2);

						if (gp_fverbose("d3"))
							printf ("=%2d= %s\n",p2,b2);

					}

					if (p2 == 7)
					{	
						strcpy( (*mlcde[q_cde]).nse,b2);

						if (gp_fverbose("d3"))
							printf ("=%2d= %s\n",p2,b2);

					}

					if (p2 == 8)
					{	
						strcpy(b3,b2);
						(*mlcde[q_cde]).qcom_min=atoi(b3);
						
						if (gp_fverbose("d3"))
							printf ("=%2d= %s %5d \n",p2,b2, (*mlcde[q_cde]).qcom_min );

					}

					if (p2 == 9)
					{	
						strcpy(b3,b2);
						(*mlcde[q_cde]).qcom_max=atoi(b3);
						
						if (gp_fverbose("d3"))
							printf ("=%2d= %s %5d \n",p2,b2, (*mlcde[q_cde]).qcom_max );

					}

					if (p2 == 10)
					{	
						strcpy(b3,b2);
						(*mlcde[q_cde]).qnt=atoi(b3);
						
						if (gp_fverbose("d3"))
							printf ("=%2d= %s %5d \n",p2,b2, (*mlcde[q_cde]).qnt );

					}

					if (p2 == 11)
					{	
						strcpy(b3,b2);
						(*mlcde[q_cde]).tic=atoi(b3);
						
						if (gp_fverbose("d3"))
							printf ("=%2d= %s %5d \n",p2,b2, (*mlcde[q_cde]).tic );

					}


					if (p2 == 12)
					{	
						strcpy(b3,b2);
						(*mlcde[q_cde]).valido=atoi(b3);
						
						if (gp_fverbose("d3"))
							printf ("=%2d= %s %5d \n",p2,b2, (*mlcde[q_cde]).valido );

					}

					p2++;
					k=0;
				}

				p1++;

			}
		
			/* next ... */
			q_cde++;
			f1=0;

			if (q_cde >= MAX_CDE)
				error(303);
		}
	}

	if (gp_fverbose("d1"))
	{
		printf ("Cantidad en tabla de definicion de comercios : %6d\n\n",q_cde);
	}


	if (gp_fverbose("d3"))
	{
		printf ("\n\nComprobando integridad de denicion de comercios \n\n");

		for (i=0; i< q_cde ; i++)
			if ( mlcde[i] !=  (lcdeptr) NULL )
			{

					printf ("%06d: %3d %3d %s %s %s %s %s %s %05d %05d %3d %02d %d\n",
						i,
						(*mlcde[i]).px,
						(*mlcde[i]).py,
						(*mlcde[i]).nzon,
						(*mlcde[i]).mden,
						(*mlcde[i]).mnse,
						(*mlcde[i]).nact,
						(*mlcde[i]).den,
						(*mlcde[i]).nse,
						(*mlcde[i]).qcom_min,
						(*mlcde[i]).qcom_max,
						(*mlcde[i]).qnt,
						(*mlcde[i]).tic,
						(*mlcde[i]).valido

						);
				}

		printf ("\n");
	}

/* definicion de comercio */

}





/*
 * -----------------------------------------------------------------------------------
 *
 *	btb1_load
 *
 *	carga la tabla con los factores de multip de definicion de comercios
 *
 * -----------------------------------------------------------------------------------
 */

int	btb1_load()
{
	char	b1[MAXB];
	char	b2[MAXR];
	char	b3[MAXD];
	int	flag;
	int	p1,p2;
	int	i,j,k;
	int	m,n;
	int	f1;
	int	nro;

	int	q_lin;
	int	i_den;
	int	i_nse;
	int	i_tic;
	int	o_den;
	int	o_nse;
	int	o_tic;

	/* esto ya caga la 'generalizacion' de carga de la tabla ... */
	int	vn[32];		

	f1 = 0;
	k  = 0;
	q_lin=0;

	if (gp_fverbose("d1"))
	{
		printf ("\nCargando tabla de factor de mult de def de com  \n\n");
	}

	/* inicializo tabla fm */
	for ( i_den=0; i_den < MAX_DEN; i_den++)
		for ( i_nse=0; i_nse < MAX_NSE; i_nse++)
			for ( i_tic=0; i_tic < MAX_TIC; i_tic++)
			{
				fdmd[i_den][i_nse][i_tic][0]=0;
				fdmd[i_den][i_nse][i_tic][1]=0;
			}




	while (fgets(b1,MAXB,hftb1) != NULL)
	{
		if (gp_fverbose("d3"))
		{
			printf ("Linea: %3d %s",q_lin,b1);
		}

		if (!linea_vacia(b1)  && b1[0] != '#' )
		{
			f1=1;
		}

		if (f1)
		{
			p1=0;
			p2=0;
			k=0;

			while (b1[p1] )
			{	
				b2[k++] = b1[p1];

				if (b1[p1] == ',' || b1[p1] == '\n')
				{	
					b2[k-1]=0;

					/* Start ... codigo particular de load */

					if ( 0 == 0 )
					{	
						if (gp_fverbose("d3"))
							printf ("=%2d= %s\n",p2,b2);

						strcpy(b3,b2);

						if (p2)
						{
							nro = atoi(b3);
							vn[p2-1] = nro;
						}

					}


					/* End   ... codigo particular de load */

					p2++;
					k=0;
				}

				p1++;

			}
		
			/* si hay algo que hacer al finalizar la linea ... es aca !! */
			if (gp_fverbose("d3"))
				printf ("\nlinea:  (%d) q en vector: (%d) ultimo vn (%3d) \n",q_lin,p2-1,vn[p2-2]);


			/* tengo que pasar vector vn a matriz fdmc ... */
			i_den = q_lin;

			for ( i_nse=0; i_nse < MAX_NSE; i_nse++)
				for ( i_tic=0; i_tic < MAX_TIC; i_tic++)
				{
					fdmd[i_den][i_nse][i_tic][0]= vn[ (i_nse) * 6 + i_tic * 2 + 0];
					fdmd[i_den][i_nse][i_tic][1]= vn[ (i_nse) * 6 + i_tic * 2 + 1];
				}


			/* next line */
			if (q_lin >= MAX_TB1)
				error(306);

			q_lin++;
			f1=0;

		}
	}
	/* while */




	if (gp_fverbose("d1"))
	{
		printf ("Cantidad en lineas en tabla de fm de def com : %6d\n\n",q_lin);
	}


	if (gp_fverbose("d4"))
	{
		btb1_print();
	}

}



/*
 * -----------------------------------------------------------------------------------
 *
 *	btb1_print
 *
 *	print de la tabla con los factores de multiplicacion de comercios
 *
 * -----------------------------------------------------------------------------------
 */



int	btb1_print()
{
	int	i_den;
	int	i_nse;
	int	i_tic;

	printf ("\n\nDatos cargados en tabla de fdmd \n\n");

	for (i_den=0; i_den < MAX_DEN; i_den++ )
	{
		printf ("Densidad: %2d  ", i_den);

		for ( i_nse=0; i_nse < MAX_NSE; i_nse++)
		{
			for ( i_tic=0; i_tic < MAX_TIC; i_tic++)
			{
				printf ("%3d, %3d, ",
					fdmd[i_den][i_nse][i_tic][0],
					fdmd[i_den][i_nse][i_tic][1]
					);
			}
		}

		printf ("\n");
	}

	printf ("\n\n");
}




/*
 * -----------------------------------------------------------------------------------
 *
 *	btb2_load
 *
 *	carga la tabla con los factores de multiplicacion de comercios
 *
 * -----------------------------------------------------------------------------------
 */

int	btb2_load()
{
	char	b1[MAXB];
	char	b2[MAXR];
	char	b3[MAXD];
	int	flag;
	int	p1,p2;
	int	i,j,k;
	int	m,n;
	int	f1;
	int	nro;

	int	q_lin;
	int	i_den;
	int	i_nse;
	int	i_tic;
	int	o_den;
	int	o_nse;
	int	o_tic;

	/* esto ya caga la 'generalizacion' de carga de la tabla ... */
	int	vn[32];		

	f1 = 0;
	k  = 0;
	q_lin=0;

	if (gp_fverbose("d1"))
	{
		printf ("\nCargando tabla de factor de mult de comercios  \n\n");
	}

	/* inicializo tabla fm */
	for ( i_den=0; i_den < MAX_DEN; i_den++)
		for ( i_nse=0; i_nse < MAX_NSE; i_nse++)
			for ( i_tic=0; i_tic < MAX_TIC; i_tic++)
			{
				fdmc[i_den][i_nse][i_tic][0]=0;
				fdmc[i_den][i_nse][i_tic][1]=0;
			}




	while (fgets(b1,MAXB,hftb2) != NULL)
	{
		if (gp_fverbose("d3"))
		{
			printf ("Linea: %3d %s",q_lin,b1);
		}

		if (!linea_vacia(b1)  && b1[0] != '#' )
		{
			f1=1;
		}

		if (f1)
		{
			p1=0;
			p2=0;
			k=0;

			while (b1[p1] )
			{	
				b2[k++] = b1[p1];

				if (b1[p1] == ',' || b1[p1] == '\n')
				{	
					b2[k-1]=0;

					/* Start ... codigo particular de load */

					if ( 0 == 0 )
					{	
						if (gp_fverbose("d3"))
							printf ("=%2d= %s\n",p2,b2);

						strcpy(b3,b2);

						if (p2)
						{
							nro = atoi(b3);
							vn[p2-1] = nro;
						}

					}


					/* End   ... codigo particular de load */

					p2++;
					k=0;
				}

				p1++;

			}
		
			/* si hay algo que hacer al finalizar la linea ... es aca !! */
			if (gp_fverbose("d3"))
				printf ("\nlinea:  (%d) q en vector: (%d) ultimo vn (%3d) \n",q_lin,p2-1,vn[p2-2]);


			/* tengo que pasar vector vn a matriz fdmc ... */
			i_den = q_lin;

			for ( i_nse=0; i_nse < MAX_NSE; i_nse++)
				for ( i_tic=0; i_tic < MAX_TIC; i_tic++)
				{
					fdmc[i_den][i_nse][i_tic][0]= vn[ (i_nse) * 6 + i_tic * 2 + 0];
					fdmc[i_den][i_nse][i_tic][1]= vn[ (i_nse) * 6 + i_tic * 2 + 1];
				}


			/* next line */
			if (q_lin >= MAX_TB2)
				error(305);

			q_lin++;
			f1=0;

		}
	}
	/* while */




	if (gp_fverbose("d1"))
	{
		printf ("Cantidad en lineas en tabla de fm de comercios : %6d\n\n",q_lin);
	}


	if (gp_fverbose("d3"))
	{
		btb2_print();
	}

}



/*
 * -----------------------------------------------------------------------------------
 *
 *	btb2_print
 *	
 *	imprime la tabla de factor de multiplicacion de comercios
 *
 * -----------------------------------------------------------------------------------
 */



int	btb2_print()
{
	int	i_den;
	int	i_nse;
	int	i_tic;

	printf ("\n\nDatos cargados en tabla de fdmc \n\n");


	for (i_den=0; i_den < MAX_DEN; i_den++ )
	{
		printf ("Densidad: %2d  ", i_den);

		for ( i_nse=0; i_nse < MAX_NSE; i_nse++)
		{
			for ( i_tic=0; i_tic < MAX_TIC; i_tic++)
			{
				printf ("%3d, %3d, ",
					fdmc[i_den][i_nse][i_tic][0],
					fdmc[i_den][i_nse][i_tic][1]
					);
			}
		
		}

		printf ("\n");
	}

	printf ("\n\n");
}





/*
 * -----------------------------------------------------------------------------------
 *
 *	btb3_load
 *
 *	carga la tabla con parametros para definicion de perfiles
 *
 * -----------------------------------------------------------------------------------
 */

int	btb3_load()
{
	char	b1[MAXB];
	char	b2[MAXR];
	char	b3[MAXD];
	int	flag;
	int	p1,p2;
	int	i,j,k;
	int	m,n;
	int	f1;
	int	nro;

	int	q_lin;
	int	i_den;
	int	i_nse;
	int	i_tic;
	int	o_den;
	int	o_nse;
	int	o_tic;

	/* esto ya caga la 'generalizacion' de carga de la tabla ... */
	int	vn[32];		

	f1 = 0;
	k  = 0;
	q_lin=0;

	if (gp_fverbose("d1"))
	{
		printf ("\nCargando tabla de param de def de perfiles  \n\n");
	}

	/* inicializo tabla fm */
	for ( i_den=0; i_den < MAX_DEN; i_den++)
		for ( i_nse=0; i_nse < MAX_NSE; i_nse++)
			for ( i_tic=0; i_tic < MAX_TIC; i_tic++)
			{
				fdmp[i_den][i_nse][i_tic][0]=0;
				fdmp[i_den][i_nse][i_tic][1]=0;
			}




	while (fgets(b1,MAXB,hftb3) != NULL)
	{
		if (gp_fverbose("d3"))
		{
			printf ("Linea: %3d %s",q_lin,b1);
		}

		if (!linea_vacia(b1)  && b1[0] != '#' )
		{
			f1=1;
		}

		if (f1)
		{
			p1=0;
			p2=0;
			k=0;

			while (b1[p1] )
			{	
				b2[k++] = b1[p1];

				if (b1[p1] == ',' || b1[p1] == '\n')
				{	
					b2[k-1]=0;

					/* Start ... codigo particular de load */

					if ( 0 == 0 )
					{	
						if (gp_fverbose("d3"))
							printf ("=%2d= %s\n",p2,b2);

						strcpy(b3,b2);

						if (p2)
						{
							nro = atoi(b3);
							vn[p2-1] = nro;
						}

					}


					/* End   ... codigo particular de load */

					p2++;
					k=0;
				}

				p1++;

			}
		
			/* si hay algo que hacer al finalizar la linea ... es aca !! */
			if (gp_fverbose("d3"))
				printf ("\nlinea:  (%d) q en vector: (%d) ultimo vn (%3d) \n",q_lin,p2-1,vn[p2-2]);


			/* tengo que pasar vector vn a matriz fdmp ... */
			i_den = q_lin;

			for ( i_nse=0; i_nse < MAX_NSE; i_nse++)
				for ( i_tic=0; i_tic < MAX_TIC; i_tic++)
				{
					fdmp[i_den][i_nse][i_tic][0]= vn[ (i_nse) * 6 + i_tic * 2 + 0];
					fdmp[i_den][i_nse][i_tic][1]= vn[ (i_nse) * 6 + i_tic * 2 + 1];
				}


			/* next line */
			if (q_lin >= MAX_TB3)
				error(309);

			q_lin++;
			f1=0;

		}
	}
	/* while */




	if (gp_fverbose("d1"))
	{
		printf ("Cantidad en lineas en tabla de fm de comercios : %6d\n\n",q_lin);
	}


	if (gp_fverbose("d3"))
	{
		btb3_print();
	}

}




/*
 * -----------------------------------------------------------------------------------
 *
 *	btb3_print
 *	
 *	imprime la tabla de parametros de defincion de perfiles
 *
 * -----------------------------------------------------------------------------------
 */



int	btb3_print()
{
	int	i_den;
	int	i_nse;
	int	i_tic;

	printf ("\n\nDatos cargados en tabla de fdmp perfiles \n\n");


	for (i_den=0; i_den < MAX_DEN; i_den++ )
	{
		printf ("Densidad: %2d  ", i_den);

		for ( i_nse=0; i_nse < MAX_NSE; i_nse++)
		{
			for ( i_tic=0; i_tic < MAX_TIC; i_tic++)
			{
				printf ("%3d, %3d, ",
					fdmp[i_den][i_nse][i_tic][0],
					fdmp[i_den][i_nse][i_tic][1]
					);
			}
		
		}

		printf ("\n");
	}

	printf ("\n\n");
}











/*
 * -----------------------------------------------------------------------------------
 *
 *	brub_load
 *	carga el archivo de rubros
 *
 * -----------------------------------------------------------------------------------
 */

int	brub_load()
{
	char	b1[MAXB];
	char	b2[MAXR];
	char	b3[MAXR];
	int	flag;
	int	p1,p2;
	int	i,j,k;
	int	f1;
	int	px,py;
	int	com_offset;

	com_offset=0;

	f1=0;
	q_rub=0;
	k=0;

	if (gp_fverbose("d1"))
	{
		printf ("\nCargando tabla de rubros  \n\n");
	}


	for (i=0; i<MAX_RUB ; i++)  
		mlrub[i] = (lrubptr ) NULL;


	while (fgets(b1,MAXB,hfrub) != NULL)
	{
		if (gp_fverbose("d3"))
		{
			printf ("%3d %s",q_rub,b1); 
		}

		if (!linea_vacia(b1)  && b1[0] != '#' )
		{
			f1=1;
		}

		if (f1)
		{
			p1=0;
			p2=0;
			k=0;

			while (b1[p1] )
			{	
				b2[k++] = b1[p1];

				if (b1[p1] == ',' || b1[p1] == '\n')
				{	
					b2[k-1]=0;

					if (p2 == 0)
					{	
						mlrub[q_rub] = (lrubptr) malloc(sizeof(lrub)) ; 

						strcpy(b3,b2);
						(*mlrub[q_rub]).ru_nro = atoi(b3);

						if (gp_fverbose("d3"))
							printf ("=%02d= %s\n",p2,b2);
					}

					if (p2 == 1)
					{	
						strcpy(b3,b2);
						(*mlrub[q_rub]).ru_tic = atoi(b3);

						if (gp_fverbose("d3"))
							printf ("=%02d= %s\n",p2,b2);
					}

					if (p2 == 2)
					{	
						strcpy(b3,b2);
						strcpy( (*mlrub[q_rub]).ru_mtic,b3); 

						if (gp_fverbose("d3"))
							printf ("=%02d= %s\n",p2,b2);
					}



					if (p2 == 3)
					{	
						strcpy(b3,b2);
						strcpy( (*mlrub[q_rub]).ru_nom,b2);

						if (gp_fverbose("d3"))
							printf ("=%02d= %s\n",p2,b2);

					}

					if (p2 == 4)
					{	
						strcpy(b3,b2);
						strcpy( (*mlrub[q_rub]).ru_desc,b2);

						if (gp_fverbose("d3"))
							printf ("=%02d= %s\n",p2,b2);

					}

					p2++;
					k=0;
				}

				p1++;

			}
		
			/* next ... */
			if (q_rub >= MAX_RUB)
				error(304);

			q_rub++;
			f1=0;

			/* temita .... 
			 * Si trabajo con una tabla de MAX_RUB ...
			 * y en el archivo hubiese mas que MAX_RUB eleementos ...
			 * esto pincha !! 
			 * tal vez arrancando con -1 y sumando 1 al principio !! 
			 * por eso probamos el if ... >= antes que incrementar 
			 */
		}
	}

	if (gp_fverbose("d1"))
	{
		printf ("Cantidad en tabla de rubros : %6d\n\n",q_rub);
	}


	if (gp_fverbose("d3"))
	{
		printf ("\n\nComprobando integridad de rubros \n\n");

		for (i=0; i<MAX_RUB ; i++)
			if ( mlrub[i] !=  (lrubptr) NULL )
			{
					printf ("nro: %4d %2d %s %s %s \n",
						(*mlrub[i]).ru_nro,
						(*mlrub[i]).ru_tic,
						(*mlrub[i]).ru_mtic,
						(*mlrub[i]).ru_nom,
						(*mlrub[i]).ru_desc
						);
				}

		printf ("\n");
	}

}



/*
 * -----------------------------------------------------------------------------------
 *
 *	bcom_load
 *
 *	carga el archivo de comercios
 *
 * -----------------------------------------------------------------------------------
 */

int	bcom_load()
{
	char	b1[MAXB];
	char	b2[MAXB];
	char	b3[MAXB];
	int	flag;
	int	p1,p2;
	int	i,j,k;
	int	f1;
	int	px,py;
	int	pxa,pya;
	int	q;


	f1=0;
	q_com=0;
	k=0;

	if (gp_fverbose("d1"))
	{
		printf ("\nCargando tabla de comercios  \n\n");
	}


	while (fgets(b1,MAXB,hfcom) != NULL)
	{
		if (gp_fverbose("d3"))
		{
			printf ("%06d: >%s< \n",q_com,b1); 
		}

		if (!linea_vacia(b1)  && b1[0] != '#' )
		{
			f1=1;
		}

		if (f1)
		{
			p1=0;
			p2=0;
			k=0;

			while (b1[p1] )
			{	
				b2[k++] = b1[p1];

				if (b1[p1] == ',' || b1[p1] == '\n')
				{	
					b2[k-1]=0;

					strcpy(b3,b2);
					if (gp_fverbose("d3"))
						printf ("=%02d= %s\n",p2,b3);

					if (p2 == 0)
					{
						mcome[q_com] = (comeptr ) malloc ( sizeof (come) );
						(*mcome[q_com]).ncom = atoi(b3);
					}

					if (p2 == 1)
					{
						(*mcome[q_com]).zon_px = atoi(b3);
					}

					if (p2 == 2)
					{
						(*mcome[q_com]).zon_py = atoi(b3);
					}

					if (p2 == 3)
					{	
						strcpy ( (*mcome[q_com]).z_nom,b3);
					}

					if (p2 == 4)
					{	
						strcpy ( (*mcome[q_com]).z_den,b3);
					}

					if (p2 == 5)
					{	
						strcpy ( (*mcome[q_com]).z_nse,b3);
					}

					if (p2 == 6)
					{	
						(*mcome[q_com]).i_den = atoi(b3);
					}

					if (p2 == 7)
					{	
						(*mcome[q_com]).i_nse = atoi(b3);
					}

					if (p2 == 8)
					{	
						strcpy ( (*mcome[q_com]).nact,b3);
					}

					if (p2 == 9)
					{	
						strcpy ( (*mcome[q_com]).r_nom,b3);
					}

					if (p2 == 10)
					{	
						(*mcome[q_com]).min = atoi(b3);
					}

					if (p2 == 11)
					{	
						(*mcome[q_com]).max = atoi(b3);
					}

					if (p2 == 12)
					{	
						(*mcome[q_com]).qnt = atoi(b3);
					}

					if (p2 == 13)
					{	
						(*mcome[q_com]).i_tic = atoi(b3);
					}

					if (p2 == 14)
					{	
						(*mcome[q_com]).px = atoi(b3);
					}

					if (p2 == 15)
					{	
						(*mcome[q_com]).py = atoi(b3);
					}

					if (p2 == 16)
					{	
						strcpy ( (*mcome[q_com]).nse,b3);
					}

					if (p2 == 17)
					{	
						(*mcome[q_com]).c_nse = atoi(b3);
					}

					if (p2 == 18)
					{	
						(*mcome[q_com]).min2 = atoi(b3);
					}

					if (p2 == 19)
					{	
						(*mcome[q_com]).max2 = atoi(b3);
					}

					if (p2 == 20)
					{	
						(*mcome[q_com]).qnt2 = atoi(b3);
					}

					if (p2 == 21)
					{	
						(*mcome[q_com]).n_rub = atoi(b3);
					}

					if (p2 == 22)
					{	
						(*mcome[q_com]).p_rub = atoi(b3);
					}



					p2++;
					k=0;
				}

				p1++;

			}
		
			/* next ... */
			if (q_com >= 409600 )
				error(307);

			q_com++;
			f1=0;

		}
	}



	/* determino primer elemento, y cantidad, de negocios x zona */
	pxa = -1;	
	pya = -1;

	for (i=0; i<q_com; i++)
	{
		px = (*mcome[i]).zon_px;
		py = (*mcome[i]).zon_py;

		if ( ( pxa != (*mcome[i]).zon_px) || (pya != (*mcome[i]).zon_py) )
		{
			zlmap[px][py][0] = i;
			zlmap[px][py][1] = 1;
		}
		else
			zlmap[px][py][1]++;

		pxa = px;
		pya = py;
	}

	
	if (gp_fverbose("d2"))
	{
		for (px=0; px<MAX_FCM; px++)	
		{
			for (py=0; py<MAX_FCM; py++)
			{
				printf ("zlmap: %3d,%3d   %6d - %6d \n",
					px,py,zlmap[px][py][0],zlmap[px][py][1]);
			}
			printf ("\n");
		}
	}


	/* verifico que ninguna zona tenga mas de MAX_ZCO comercios */
	for (px=0; px<MAX_FCM; px++)	
	{
		for (py=0; py<MAX_FCM; py++)
		{
			if ( zlmap[px][py][1] > MAX_ZCO )
			{	printf ("error: zona(%2d,%2d) supera MAX_ZCO  %d\n",
					px,py,zlmap[px][py][1] );
				error(602);
			}
		}
	}

	


	if (gp_fverbose("d1"))
	{
		printf ("Cantidad en tabla de comercios : %6d\n\n",q_com);
	}

	if (gp_fverbose("d3"))
	{
		for (i=0; i<q_com; i++)
		{
			printf ("%06d,%02d,%02d,%s,%s,%s,%d,%d,%s,%s,%03d,%03d,%3d,%d,%3d,%3d,%s,%d,%3d,%3d,%3d,%3d,%3d\n",
				(*mcome[i]).ncom,
				(*mcome[i]).zon_px,
				(*mcome[i]).zon_py,
				(*mcome[i]).z_nom,
				(*mcome[i]).z_den,
				(*mcome[i]).z_nse,
				(*mcome[i]).i_den,
				(*mcome[i]).i_nse,
				(*mcome[i]).nact,
				(*mcome[i]).r_nom,
				(*mcome[i]).min,
				(*mcome[i]).max,
				(*mcome[i]).qnt,
				(*mcome[i]).i_tic,
				(*mcome[i]).px,
				(*mcome[i]).py,
				(*mcome[i]).nse,
				(*mcome[i]).c_nse,
				(*mcome[i]).min2,
				(*mcome[i]).max2,
				(*mcome[i]).qnt2,
				(*mcome[i]).n_rub,
				(*mcome[i]).p_rub
				);
		}
	}
	

	if (gp_fverbose("d3"))
	{
		printf ("revision de comercios, ingresando por la zona ... \n");

		for (px =0; px < MAX_FCM; px++)
		{
			for (py = 0; py < MAX_FCM; py++)
			{
				i=zlmap[px][py][0];			
				q=zlmap[px][py][1];			

				for (j=0; j<q; j++)
					printf ("%4d (%2d,%2d) %6d %2d %2d \n",
						j,
						px,
						py,
						(*mcome[i+j]).ncom,
						(*mcome[i+j]).zon_px,
						(*mcome[i+j]).zon_py
					);

			}

			printf ("\n");
		}
	}

}



/*
 * -----------------------------------------------------------------------------------
 *
 *	btb4_load
 *
 *	carga la tablad de tipos de perfiles x zona
 *
 * -----------------------------------------------------------------------------------
 */


int	btb4_load()
{

	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra a btb4_load \n\n",gp_tm());
	}

	q_tpf =0;

	bcvs_load("Cargando tipo de perfiles \n", hftb4, fx_tb4 );

	if (gp_fverbose("d1"))
	{	printf ("Cantidad de tipos de perfiles cargados %d \n\n",q_tpf);
	}

	if (gp_fverbose("d3"))
	{
		printf ("Tabla de tipos de perfiles por zona  \n\n");

		btb4_print();
	}

	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de btp4_load \n\n",gp_tm());
	}
}


/*
 * -----------------------------------------------------------------------------------
 *
 *	fx_tba
 *
 *	funcion para bcvs_load para cargar la tablad de tipos de perfiles x zona
 *
 * -----------------------------------------------------------------------------------
 */



int	fx_tb4(act,pos,str)
int	act;
int	pos;
char	*str;
{
	int	vn[32];		
	int	i_fam, i_nse;

	if (act == 0)
	{
		if (gp_fverbose("d3"))
		{
			printf ("act 0: %2d %s \n",pos,str);
		}

		if ( pos == 0 )
		{
			mtper[q_tpf] = (tperptr ) malloc ( sizeof (tper) );

			(*mtper[q_tpf]).tp_nro = q_tpf;    
			sprintf( (*mtper[q_tpf]).tp_nom , "zon_tpf_%02d", q_tpf );

			strcpy ( (*mtper[q_tpf]).tp_den,str);
			(*mtper[q_tpf]).tp_iden = densidad(str) -2; 
		}

		if ( pos == 1 )
		{
			strcpy ( (*mtper[q_tpf]).tp_nse,str);
			(*mtper[q_tpf]).tp_inse = densidad(str) -2;   /* seguro ????? OJO */
		}

		if ( pos > 1 )
		{
			/* los 9 numeros */
			vn[pos-2] = atoi(str);;
		}
	}

	if (act == 1)
	{
		/* si hay que hacer algo al finalizar el registro, es ACA (antes de q_tpf++ ) */

		/* paso el vector vn a la matriz */
		for (i_fam = 0; i_fam < 3; i_fam++)
			for (i_nse = 0; i_nse < 3; i_nse++)
				(*mtper[q_tpf]).tp_por[i_fam][i_nse] = vn[i_fam * 3 + i_nse ];

		q_tpf++;  

		if (q_tpf >= MAX_TB4 )
			error( 310 );     
	}

	if (act == 2)
	{
		/* si hay algo que hacer al final de todo el archivo, es ACA !! */
	}
}



/*
 * -----------------------------------------------------------------------------------
 *
 *	btb4_print
 *
 *	imprime  la tablad de tipos de perfiles x zona
 *
 * -----------------------------------------------------------------------------------
 */



int	btb4_print()
{
	int	i;

	printf ("\n\nDatos cargados en tabla tipos de perf  \n\n");

	for (i=0; i<q_tpf; i++)
	{
		printf ("%3d %s %s %s %3d %3d %3d %3d %3d %3d %3d %3d %3d  \n",
			(*mtper[i]).tp_nro,
			(*mtper[i]).tp_nom,
			(*mtper[i]).tp_den,
			(*mtper[i]).tp_nse,
			(*mtper[i]).tp_por[0][0],
			(*mtper[i]).tp_por[0][1],
			(*mtper[i]).tp_por[0][2],
			(*mtper[i]).tp_por[1][0],
			(*mtper[i]).tp_por[1][1],
			(*mtper[i]).tp_por[1][2],
			(*mtper[i]).tp_por[2][0],
			(*mtper[i]).tp_por[2][1],
			(*mtper[i]).tp_por[2][2]

			);
	}
	
	printf ("\n");

}






/*
 * -----------------------------------------------------------------------------------
 *
 *	btrx_load
 *
 *	carga la base de transacciones
 *
 * -----------------------------------------------------------------------------------
 */



int	btrx_load()
{

	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra a  btrx_load \n\n",gp_tm());
	}

	q_trx =0;

	bcvs_load("Cargando transacciones \n", hftrx, fx_trx );

	if (gp_fverbose("d1"))
	{	printf ("Cantidad de transacciones cargadas %d \n\n",q_trx);
	}

	if (gp_fverbose("d3"))
	{
		printf ("Tabla de transacciones  \n\n");

		btrx_print();
	}

	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de btrx_load \n\n",gp_tm());
	}
}







/*
 * -----------------------------------------------------------------------------------
 *
 *	btrx_print 
 *
 *	imprime  la tablad de transacciones 
 *
 * -----------------------------------------------------------------------------------
 */

int	btrx_print()
{
	int	i;

	for (i=0; i<q_trx; i++)
	{
		printf ("|%s %s %10d %5d | %2d | %2d %2d %d %d | %6d %5d %5d %d %d %3d | %8d %2d %2d %d %d | %d %d | %s\n",
			(*mtrax[i]).tx_trx_fecha,
			(*mtrax[i]).tx_trx_hora,
			(*mtrax[i]).tx_trx_nro,
			(*mtrax[i]).tx_trx_monto,
			(*mtrax[i]).tx_art_desc,
			(*mtrax[i]).tx_zon_px,
			(*mtrax[i]).tx_zon_py,
			(*mtrax[i]).tx_zon_iden,
			(*mtrax[i]).tx_zon_inse,
			(*mtrax[i]).tx_com_nro,
			(*mtrax[i]).tx_com_px,
			(*mtrax[i]).tx_com_py,
			(*mtrax[i]).tx_com_inse,
			(*mtrax[i]).tx_com_itic,
			(*mtrax[i]).tx_com_nrub,
			(*mtrax[i]).tx_per_nro,
			(*mtrax[i]).tx_per_px,
			(*mtrax[i]).tx_per_py,
			(*mtrax[i]).tx_per_ifam,
			(*mtrax[i]).tx_per_inse,
			(*mtrax[i]).tx_cal_n1,
			(*mtrax[i]).tx_cal_n2,
			" "
			);
		
	}

	printf ("\n");
}




/*
 * -----------------------------------------------------------------------------------
 *
 *	fx_trx  
 *
 *	funcion para bcvs_load para carga de transacciones
 *
 * -----------------------------------------------------------------------------------
 */



int	fx_trx(act,pos,str)
int	act;
int	pos;
char	*str;
{
	if (act == 0)
	{
		if (gp_fverbose("d4"))
		{
			printf ("act 0: %2d %s \n",pos,str);
		}

		if ( pos == 0 )
		{
			mtrax[q_trx] = (traxptr ) malloc (sizeof (trax));
			if (mtrax[q_trx] == NULL )
				error(904);   

			strcpy ( (*mtrax[q_trx]).tx_trx_fecha , str);
		}

		if ( pos == 1 )
		{
			strcpy( (*mtrax[q_trx]).tx_trx_hora , str);
		}

		if ( pos == 2 )
		{
			(*mtrax[q_trx]).tx_trx_nro  = atoi(str);
		
		}

		if ( pos == 3 )
		{
			(*mtrax[q_trx]).tx_trx_monto  = atoi(str);
		
		}

		if ( pos == 4 )
		{
			(*mtrax[q_trx]).tx_art_desc  = atoi(str); 
	
		}

		if ( pos == 5 )
		{
			(*mtrax[q_trx]).tx_zon_px    = atoi(str); 

		}

		if ( pos == 6 )
		{
			(*mtrax[q_trx]).tx_zon_py    = atoi(str); 

		}

		if ( pos == 7 )
		{
			(*mtrax[q_trx]).tx_zon_iden  = atoi(str); 

		}

		if ( pos == 8 )
		{
			(*mtrax[q_trx]).tx_zon_inse  = atoi(str); 

		}

		if ( pos == 9 )
		{
			(*mtrax[q_trx]).tx_com_nro   = atoi(str); 

		}

		if ( pos == 10 )
		{
			(*mtrax[q_trx]).tx_com_px    = atoi(str); 

		}

		if ( pos == 11 )
		{
			(*mtrax[q_trx]).tx_com_py    = atoi(str); 

		}

		if ( pos == 12 )
		{
			(*mtrax[q_trx]).tx_com_inse  = atoi(str); 

		}

		if ( pos == 13 )
		{
			(*mtrax[q_trx]).tx_com_itic  = atoi(str); 

		}

		if ( pos == 14 )
		{
			(*mtrax[q_trx]).tx_com_nrub  = atoi(str); 

		}

		if ( pos == 15 )
		{
			(*mtrax[q_trx]).tx_per_nro   = atoi(str); 

		}

		if ( pos == 16 )
		{
			(*mtrax[q_trx]).tx_per_px    = atoi(str); 

		}

		if ( pos == 17 )
		{
			(*mtrax[q_trx]).tx_per_py    = atoi(str); 

		}

		if ( pos == 18 )
		{
			(*mtrax[q_trx]).tx_per_ifam  = atoi(str); 

		}

		if ( pos == 19 )
		{
			(*mtrax[q_trx]).tx_per_inse  = atoi(str); 

		}

		if ( pos == 20 )
		{
			(*mtrax[q_trx]).tx_cal_n1    = atoi(str); 

		}

		if ( pos == 21 )
		{
			(*mtrax[q_trx]).tx_cal_n2    = atoi(str); 

		}

	}

	if (act == 1)
	{
		q_trx++;

		if (q_trx >= MAX_TR2 )
			error( 313 );
	}

	if (act == 2)
	{
		/* nothing to do here !! */
	}
}






/*
 * -----------------------------------------------------------------------------------
 *
 *	bper_load
 *
 *	carga la base de perfiles
 *
 * -----------------------------------------------------------------------------------
 */



int	bper_load()
{

	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra a  bper_load \n\n",gp_tm());
	}

	q_per =0;

	bcvs_load("Cargando perfiles \n", hfper, fx_per );

	if (gp_fverbose("d1"))
	{	printf ("Cantidad de perfiles cargados %d \n\n",q_per);
	}

	if (gp_fverbose("d3"))
	{
		printf ("Tabla de perfiles  \n\n");

		bper_print();
	}

	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de bper_load \n\n",gp_tm());
	}
}

#if 0

typedef	struct	tperf
{		
	int	pe_nro;			/* numero de perfil */
	char	pe_nom[64];		/* nombre de perfil */
	char	pe_fam[4];		/* familia numerosa Baja Media Alta */
	char	pe_nse[4];		/* nivel socio economico B M A */
	int	pe_ifam;
	int	pe_inse;
	int	pe_cxm[2];		/* compras por mes min max */
	int	pe_dis[3][3];		/* distribucion de gastos ... 3 ternas, porc,min,max */
	int	re_sp1;			/* spare */
}	perf;




#endif





/*
 * -----------------------------------------------------------------------------------
 *
 *	bper_print 
 *
 *	imprime  la tablad de perfiles 
 *
 * -----------------------------------------------------------------------------------
 */

int	bper_print()
{
	int	i;

	for (i=0; i<q_per; i++)
	{
		printf ("%3d %s %s %s %d %d %5d %5d %5d %5d %5d %5d %5d %5d %5d\n",
			(*mperf[i]).pe_nro,
			(*mperf[i]).pe_nom,
			(*mperf[i]).pe_fam,
			(*mperf[i]).pe_nse,
			(*mperf[i]).pe_ifam,
			(*mperf[i]).pe_inse,
			(*mperf[i]).pe_dis[0][0],
			(*mperf[i]).pe_dis[0][1],
			(*mperf[i]).pe_dis[0][2],
			(*mperf[i]).pe_dis[1][0],
			(*mperf[i]).pe_dis[1][1],
			(*mperf[i]).pe_dis[1][2],
			(*mperf[i]).pe_dis[2][0],
			(*mperf[i]).pe_dis[2][1],
			(*mperf[i]).pe_dis[2][2]
			);
	}

	printf ("\n");
}


/*
 * -----------------------------------------------------------------------------------
 *
 *	fx_per  
 *
 *	funcion para bcvs_load para carga de perfiles
 *
 * -----------------------------------------------------------------------------------
 */



int	fx_per(act,pos,str)
int	act;
int	pos;
char	*str;
{
	if (act == 0)
	{
		if (gp_fverbose("d4"))
		{
			printf ("act 0: %2d %s \n",pos,str);
		}

		if ( pos == 0 )
		{
			mperf[q_per] = (perfptr ) malloc ( sizeof (perf) );
			(*mperf[q_per]).pe_nro = q_per;
			strcpy ( (*mperf[q_per]).pe_nom,str);
		}

		if ( pos == 1 )
		{
			strcpy( (*mperf[q_per]).pe_fam,str);
			(*mperf[q_per]).pe_ifam = densidad(str) -2; 
		}

		if ( pos == 2 )
		{
			strcpy( (*mperf[q_per]).pe_nse,str);
			(*mperf[q_per]).pe_inse = densidad(str) -2; 
		}

		if ( pos == 3 )
		{
			(*mperf[q_per]).pe_cxm[0] = atoi(str);
		}

		if ( pos == 4 )
		{
			(*mperf[q_per]).pe_cxm[1] = atoi(str);
		}

		if ( pos == 5 )
		{
			(*mperf[q_per]).pe_dis[0][0] = atoi(str);
		}

		if ( pos == 6 )
		{
			(*mperf[q_per]).pe_dis[0][1] = atoi(str);
		}

		if ( pos == 7 )
		{
			(*mperf[q_per]).pe_dis[0][2] = atoi(str);
		}

		if ( pos == 8 )
		{
			(*mperf[q_per]).pe_dis[1][0] = atoi(str);
		}

		if ( pos == 9 )
		{
			(*mperf[q_per]).pe_dis[1][1] = atoi(str);
		}

		if ( pos == 10 )
		{
			(*mperf[q_per]).pe_dis[1][2] = atoi(str);
		}

		if ( pos == 11 )
		{
			(*mperf[q_per]).pe_dis[2][0] = atoi(str);
		}

		if ( pos == 12 )
		{
			(*mperf[q_per]).pe_dis[2][1] = atoi(str);
		}

		if ( pos == 13 )
		{
			(*mperf[q_per]).pe_dis[2][2] = atoi(str);
		}

	}

	if (act == 1)
	{
		q_per++;

		if (q_per >= MAX_PFL )
			error( 308 );
	}

	if (act == 2)
	{
		/* nothing to do here !! */
	}
}



/*
 * -----------------------------------------------------------------------------------
 *
 *	bcpe_load
 *
 *	carga la tabla de personas
 *
 * -----------------------------------------------------------------------------------
 */



int	bcpe_load()
{

	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra a  bcpe_load \n\n",gp_tm());
	}

	q_gpe =0;

	bcvs_load("Cargando personas \n", hfcpe, fx_cpe );

	if (gp_fverbose("d1"))
	{	printf ("Cantidad de personas cargadas %d \n\n",q_gpe);
	}

	if (gp_fverbose("d3"))
	{
		printf ("Tabla de personas  \n\n");

		bcpe_print();
	}

	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de bcpe_load \n\n",gp_tm());
	}
}




/*
 * -----------------------------------------------------------------------------------
 *
 *	fx_cpe  
 *
 *	funcion para bcvs_load para carga de personas
 *
 * -----------------------------------------------------------------------------------
 */



int	fx_cpe(act,pos,str)
int	act;
int	pos;
char	*str;
{
	if (act == 0)
	{
		if (gp_fverbose("d3"))
		{
			printf ("act 0: %2d %s \n",pos,str);
		}

		if ( pos == 0 )
		{
			mpers[q_gpe] = (persptr ) malloc ( sizeof (pers) );  
			if (mpers[q_gpe] == NULL)
				error(910);

			(*mpers[q_gpe]).ps_nro = atoi(str);
		}


		if ( pos == 1 )
		{
			strcpy(	(*mpers[q_gpe]).ps_nom , str);
		}

		if ( pos == 2 )
		{
			(*mpers[q_gpe]).ps_zon_px = atoi(str);   
		}

		if ( pos == 3 )
		{
			(*mpers[q_gpe]).ps_zon_py = atoi(str);
		}

		if ( pos == 4 )
		{
			strcpy(	(*mpers[q_gpe]).ps_zon_den, str);
		}

		if ( pos == 5 )
		{
			strcpy(	(*mpers[q_gpe]).ps_zon_nse, str);
		}

		if ( pos == 6 )
		{
			(*mpers[q_gpe]).ps_zon_i_den = atoi(str);
		}

		if ( pos == 7 )
		{
			(*mpers[q_gpe]).ps_zon_i_nse = atoi(str);
		}

		if ( pos == 8 )
		{
			(*mpers[q_gpe]).ps_i_tpf = atoi(str);
		}

		if ( pos == 9 )
		{
			strcpy(	(*mpers[q_gpe]).ps_n_tpf,str);
		}

		if ( pos == 10 )
		{
			strcpy(	(*mpers[q_gpe]).ps_pe_nom,str);
		}

		if ( pos == 11 )
		{
			strcpy(	(*mpers[q_gpe]).ps_fam,str);
		}

		if ( pos == 12 )
		{
			strcpy(	(*mpers[q_gpe]).ps_nse,str);
		}

		if ( pos == 13 )
		{
			(*mpers[q_gpe]).ps_ifam = atoi(str);
		}

		if ( pos == 14 )
		{
			(*mpers[q_gpe]).ps_inse = atoi(str);
		}

		if ( pos == 15 )
		{
			(*mpers[q_gpe]).ps_cxm[0] = atoi(str);
		}

		if ( pos == 16 )
		{
			(*mpers[q_gpe]).ps_cxm[1] = atoi(str);
		}

		if ( pos == 17 )
		{
			(*mpers[q_gpe]).ps_dis[0][0] = atoi(str);
		}

		if ( pos == 18 )
		{
			(*mpers[q_gpe]).ps_dis[0][1] = atoi(str);
		}

		if ( pos == 19 )
		{
			(*mpers[q_gpe]).ps_dis[0][2] = atoi(str);
		}

		if ( pos == 20 )
		{
			(*mpers[q_gpe]).ps_dis[1][0] = atoi(str);
		}

		if ( pos == 21 )
		{
			(*mpers[q_gpe]).ps_dis[1][1] = atoi(str);
		}

		if ( pos == 22 )
		{
			(*mpers[q_gpe]).ps_dis[1][2] = atoi(str);
		}

		if ( pos == 23 )
		{
			(*mpers[q_gpe]).ps_dis[2][0] = atoi(str);
		}

		if ( pos == 24 )
		{
			(*mpers[q_gpe]).ps_dis[2][1] = atoi(str);
		}

		if ( pos == 25 )
		{
			(*mpers[q_gpe]).ps_dis[2][2] = atoi(str);
		}


	}

	if (act == 1)
	{
		q_gpe++;

		if (q_gpe >= MAX_PER )
			error( 312 ); 
	}

	if (act == 2)
	{
		/* nothing to do here !! */
	}
}





/*
 * -----------------------------------------------------------------------------------
 *
 *	bcpe_print 
 *
 *	imprime  la tablad de personas 
 *
 * -----------------------------------------------------------------------------------
 */

int	bcpe_print()
{
	int	i;

	for (i=0; i<q_gpe; i++)
	{
			printf ("%06d,%s,%02d,%02d,%s,%s,%d,%d,%02d,%s,%s,%s,%s,%d,%d,%3d,%3d,%2d,%4d,%5d,%2d,%4d,%5d,%2d,%4d,%5d\n",
				(*mpers[i]).ps_nro,
				(*mpers[i]).ps_nom, 
				(*mpers[i]).ps_zon_px,
				(*mpers[i]).ps_zon_py,
				(*mpers[i]).ps_zon_den,
				(*mpers[i]).ps_zon_nse,
				(*mpers[i]).ps_zon_i_den,
				(*mpers[i]).ps_zon_i_nse,
				(*mpers[i]).ps_i_tpf,
				(*mpers[i]).ps_n_tpf,
				(*mpers[i]).ps_pe_nom,
				(*mpers[i]).ps_fam,
				(*mpers[i]).ps_nse,
				(*mpers[i]).ps_ifam,
				(*mpers[i]).ps_inse,
				(*mpers[i]).ps_cxm[0],
				(*mpers[i]).ps_cxm[1],
				(*mpers[i]).ps_dis[0][0], 
				(*mpers[i]).ps_dis[0][1], 
				(*mpers[i]).ps_dis[0][2], 
				(*mpers[i]).ps_dis[1][0], 
				(*mpers[i]).ps_dis[1][1], 
				(*mpers[i]).ps_dis[1][2], 
				(*mpers[i]).ps_dis[2][0], 
				(*mpers[i]).ps_dis[2][1], 
				(*mpers[i]).ps_dis[2][2] 
				);
	}

	printf ("\n");
}









/*
 * -----------------------------------------------------------------------------------
 *
 *	bfxa_load
 *
 *	carga los registros de fix a
 *
 * -----------------------------------------------------------------------------------
 */

int	bfxa_load()
{
	char	b1[MAXB];
	char	b2[MAXB];
	char	b3[MAXB];
	int	flag;
	int	p1,p2;
	int	i,j,k;
	int	f1;


	f1=0;
	q_fxa=0;
	k=0;

	/* proceso */
	if (gp_fverbose("d1"))
	{
		printf ("\nEntra a carga de elementos de fix a  \n\n");
	}


	while (fgets(b1,MAXB,hffxa) != NULL)
	{
		if (gp_fverbose("d3"))
		{
			printf ("%3d: %s",q_fxa,b1);
		}

		if (!linea_vacia(b1)  && b1[0] != '#' && b1[0] != 'p' && b1[0] != 'i' )
		{
			f1=1;
		}

		if (f1)
		{
			p1=0;
			p2=0;
			k=0;

			while (b1[p1] )
			{	
				b2[k++] = b1[p1];
				
				if (b1[p1] == ',' || b1[p1] == '\n')
				{	
					b2[k-1]=0;

					strcpy(b3,b2);
					if (gp_fverbose("d3"))
						printf ("=%02d= %s\n",p2,b3);

#if 0
	int	fxa_nreg;		/* numero de registro en trx */
	int	fxa_per;		/* numero de persona */
	char	fxa_fecha[16];		/* fecha de la trx */
	char	fxa_hora[16];		/* hora */
	int	fxa_cal_n1;		/* 0 no trux 1 si trux */
	int	fxa_cal_n2;		/* 0 todavia no la vio como trux 1 si lo vio */
#endif

					if (p2 == 0)
					{	
						mrfxa[q_fxa] = (rfxaptr) malloc(sizeof(rfxa)) ;
						if (mrfxa[q_fxa] == NULL )
							error(905);

						(*mrfxa[q_fxa]).fxa_nreg = atoi(b3);  
					}

					if (p2 == 1)
					{	
						(*mrfxa[q_fxa]).fxa_per = atoi(b3);  
					}

					if (p2 == 2)
					{	
						strcpy ( (*mrfxa[q_fxa]).fxa_fecha , b3 ); 
					}

					if (p2 == 3)
					{	
						strcpy ( (*mrfxa[q_fxa]).fxa_hora , b3 ); 
					}

					if (p2 == 4)
					{	
						(*mrfxa[q_fxa]).fxa_cal_n1 = atoi(b3);  
					}

					if (p2 == 5)
					{	
						(*mrfxa[q_fxa]).fxa_cal_n2 = atoi(b3);  
					}

					p2++;
					k=0;
				}

				p1++;

			}
		

			/* next ... */
			if (q_fxa >= MAX_FXA)
				error(314);   
			q_fxa++;
			f1=0;

		}
	}

	if (gp_fverbose("d1"))
	{
		printf ("Cantidad de elementos cargados en fix a : %6d\n\n",q_fxa);
	}


	if (gp_fverbose("d3"))
	{
		printf ("\n\nComprobando integridad de fix a \n\n");

		bfxa_print();
	}


	/* proceso */
	if (gp_fverbose("d1"))
	{
		printf ("\nSale de carga de elementos de fix a  \n\n");
	}

}


int	bfxa_print()
{
	int	i;


	for (i=0; i<q_fxa; i++)
	{
		printf ("%8d %10d %10d %s %s %d %d\n",
			i,
			(*mrfxa[i]).fxa_nreg,
			(*mrfxa[i]).fxa_per,
			(*mrfxa[i]).fxa_fecha,
			(*mrfxa[i]).fxa_hora,
			(*mrfxa[i]).fxa_cal_n1,
			(*mrfxa[i]).fxa_cal_n2
			);
	}

	printf ("\n");
}

/*
 * -----------------------------------------------------------------------------------
 *
 *	espero_enter
 *
 *	esperar enter por pantalla
 *
 * -----------------------------------------------------------------------------------
 */


char	*espero_enter(m)
int	m;
{
	static	char	s[16];

	if (m)
		printf ("%sContinuar .. ",
			"                                                                                    ");

	gets(s);
	return s;
}


int	clear_screen()
{
	printf("\e[1;1H\e[2J");
}


/*
 * -----------------------------------------------------------------------------------
 *
 *	ATENTI ....
 *	de aca en mas .... las cosas que quedaron del viejo ces_tpar01 .... 
 *
 * -----------------------------------------------------------------------------------
 */









/*
 * -----------------------------------------------------------------------------------
 *
 *	bg_load
 *	carga la base de datos de gramatica nga
 *
 * -----------------------------------------------------------------------------------
 */

/*
 *	2018-09-27
 *
 *	bg_load
 *	carga la base de datos de la gramatica nga
 *
 */

int	bg_load()
{
	char	b1[MAXB];
	char	b2[MAXB];
	char	b3[MAXB];
	int	flag,f1,f2,f3,f4;
	int	i,j,k;
	int	p1,p2,q1,q2,q3;
	int	qtr;
	int	m9;

		/* wrd exacta, wrd parcial, barra, pipe, asterisco, mas, numero, tipo */
	int	flg_e,flg_p,flg_b,flg_pp,flg_a,flg_m,flg_num,flg_tip;


	if (gp_fverbose("d1"))
	{
		printf ("Cargando tabla de reglas  \n");
	}



	/* cantidad de palabras en el diccionario */
	q_grm = 0;

	while (fgets(b1,MAXB,hfgrm) != NULL)
	{
		if (!linea_vacia(b1)  && b1[0] != '#' )
		{
			/* quitamos blancos, tabs y new lines ... y terminamos con una ',' */
			j=strlen(b1)-1;
			while ( j >=0 && ( b1[j] == ' ' || b1[j] == '\n' || b1[j] == '\t' ) )
				j--;
			b1[j+1]=',';
			b1[j+2]=0;

			if (gp_fverbose("d3"))
			{
				printf ("%3d |%s|\n",q_grm,b1);
			}

			qtr=0;

			f1 = 1;
			p1 = 0;
			p2 = 0;

			while (f1)
			{	

				flg_e = flg_p = flg_b = flg_pp = flg_a = flg_m = flg_num = flg_tip = 0;

				while (b1[p1] != ',' )
				{
					if (b1[p1] != ' ')
					{
						b2[p2] = b1[p1];

						if (b2[p2] >= '0' && b2[p2] <= '9' )
							flg_num = (int) b2[p2] - '0';

						if (flg_b && b2[p2] == 'E')
							flg_tip = 1;
						if (flg_b && b2[p2] == 'P')
							flg_tip = 2;
							
						if (b2[p2] == '/')
							flg_b = p2;
						if (b2[p2] == '|')
							flg_pp = 1;
						if (b2[p2] == '+')
							flg_m = 3;
						if (b2[p2] == '*')
							flg_m = 4;

						p2++;
					}
					p1++;
				}
				b2[p2]=0;
				p2=0;
				

				if (gp_fverbose("d3"))
				{
					printf ("Regla: %3d  token: (%3d) |%s|\n",q_grm,qtr,b2);
				}

				/* tengo regla q_grm, token qtr, txt del token b2 */
				f2 = 0;
				tbr[q_grm][qtr] = (reglaptr) malloc(sizeof(regla)) ;

				/* XXX identificar la regla !! */
				sprintf ( (*tbr[q_grm][qtr]).nm,"%3d",q_grm);

				/* es una palabra sola */
				if (!f2 && flg_b && !flg_pp )
				{
					(*tbr[q_grm][qtr]).wrd = (char *) malloc (strlen(b2)+1);
					strncpy ( (*tbr[q_grm][qtr]).wrd,b2,flg_b );
					(*tbr[q_grm][qtr]).wrd[flg_b]=0;
					(*tbr[q_grm][qtr]).tip=flg_tip;
					(*tbr[q_grm][qtr]).num=1;
					(*tbr[q_grm][qtr]).mrk=1;
					f2=1;
				}


				/* es un contador de tokens ... + es min 1 max NUM ... * es min 0 max NUM */
				if (!f2 && flg_m )
				{
					(*tbr[q_grm][qtr]).wrd = (char *) malloc (strlen(b2)+1);
					strcpy ( (*tbr[q_grm][qtr]).wrd,b2 );
					(*tbr[q_grm][qtr]).tip=flg_m;
					(*tbr[q_grm][qtr]).num=flg_num;
					(*tbr[q_grm][qtr]).mrk=2;
					f2=1;
				}


				/* es un OR de palabras !! */
				if (!f2 && flg_pp )
				{	if (b2[strlen(b2)-1] != '|')
						strcat(b2,"|");
					for ( m9=0, k=0; k<strlen(b2); k++ )
						if (b2[k] == '|')
							m9++;

					/* rpa ... puntero al primer puntero de la cadena de OR en la matriz */
					(*tbr[q_grm][qtr]).mrk=3;
					rpa = (reglaptr *) &tbr[q_grm][qtr];

					f3 = 1;
					q2 = 0;
					q3 = 0;

					while (f3)
					{
						flg_b = flg_tip = 0;
		
						while (b2[q2] != '|' )
						{
							if (b2[q2] != ' ')
							{
								b3[q3] = b2[q2];
		
								if (flg_b && b2[q2] == 'E')
									flg_tip = 1;
								if (flg_b && b2[q2] == 'P')
									flg_tip = 2;
									
								if (b2[q2] == '/')
									flg_b = q3;
		
								q3++;
							}
							q2++;
						}

						b3[flg_b]=0;
						q3=0;

						/* guardo los datos de la palabra N-sima */

						(**rpa).wrd = (char *) malloc(strlen(b3)+1);
						sprintf ( (**rpa).wrd,"%s",b3);	
						(**rpa).tip=flg_tip;
						(**rpa).num=m9;	
						(**rpa).nx = (reglaptr) NULL;
				

						q2++;
						if (b2[q2] == 0)
						{	f3=0;
						}
						else
						{
							(**rpa).nx = (reglaptr) malloc ( sizeof ( regla ));
							rpa = (reglaptr *) & (*rpa)->nx;
							
						}


					} /* while f3 .. */


					f2=1;
				}




				/* es REF */
				if (!f2)
				{
					(*tbr[q_grm][qtr]).wrd = (char *) malloc (strlen(b2)+1);
					strcpy ( (*tbr[q_grm][qtr]).wrd,b2 );
					(*tbr[q_grm][qtr]).mrk=4;
					f2=1;
				}
					



				p1++;
				if (b1[p1] == 0)
				{	f1=0;
					tbr[q_grm][qtr+1] = (reglaptr) NULL;
				}

				qtr++;
				
			} /* while f1 ... */


			/* nueva regla */
			q_grm++;

		} /* if not linea vacia .. */

	} /* while fgets ... */


	if (gp_fverbose("d1"))
	{
		printf ("\n");
		printf ("Cantidad de lineas en gramatica      : %6d\n\n",q_grm);
	}

	if (gp_fverbose("d3"))
	{	
		printf ("\n");
		printf ("Comprobando integridad de matriz de reglas \n\n");
		
		for (i=0; i < q_grm; i++)
		{
			j=0;
			while ( tbr[i][j] != (reglaptr) NULL )
			{
				printf ("Reg: %2d  tk: (%2d)          tip: %3d    num: %3d   mrk:  %d |%s|\n",
					i,j,
					(*tbr[i][j]).tip, (*tbr[i][j]).num,(*tbr[i][j]).mrk,(*tbr[i][j]).wrd   );


				if ( (*tbr[i][j]).nx != (reglaptr) NULL )
				{
					rpa = (reglaptr *) &tbr[i][j];
					m9 = ( **rpa).num;

					for ( k=0; k< m9; k++)
					{	
						if (k)
						{
							printf ("                           tip: %3d    num: %3d   mrk:  %d |%s|\n",
								(**rpa).tip, (**rpa).num, (**rpa).mrk, (**rpa).wrd   );
						}

						rpa = (reglaptr *) & (*rpa)->nx;
					}

				}
							
				j++;

			}  /* while tbr ... */
			
			printf ("\n");
		}

		printf ("\n\n");
	}
	
	return 0;

}








/*
 * -----------------------------------------------------------------------------------
 *
 *	bd_load
 *	carga la base de datos del diccionario
 *
 * -----------------------------------------------------------------------------------
 */


/*
 *	2018-04-21
 *
 *	bd_load
 *	carga la base de datos del diccionario
 *	diccio base: diccio01.txt
 *
 */

int	bd_load()
{
	char	b1[MAXB];
	int	flag,f1;
	int	i,j,k;
	int	ql;

	if (gp_fverbose("d1"))
	{
		printf ("\nCargando diccionario de palabras \n\n");
	}

	/* cantidad de palabras en el diccionario */
	q_wrd = 0;

	/* ptr al contenedor de direccion del ultimo nodo de la cadena */
	npa = (nodeptr *) &np1;

	ql=0;
	while (fgets(b1,MAXB,hfdic) != NULL)
	{
		if (!linea_vacia(b1)  && b1[0] != '#' )
		{
			for ( flag=0, j=strlen(b1); !flag && j; j--)
				if (b1[j] == '\n')
					b1[j]=0,flag=1;

			if (gp_fverbose("d3"))
			{
				printf ("%3d |%s|\n",ql,b1);
			}

			*npa = (nodeptr ) malloc ( sizeof (node));
			(**npa).wrd = ( char *) malloc(strlen(b1)+1);
			sprintf ( (**npa).wrd,"%s",b1);
			(**npa).num = q_wrd+1;
			(**npa).nx = (nodeptr) NULL;
			npa = (nodeptr *) & (*npa)->nx;

			q_wrd++;
			ql++;
		}
	}

	if (gp_fverbose("d1"))
	{
		printf ("Cantidad de palabras en diccionario  : %6d\n",q_wrd);
	}


	if (gp_fverbose("d3"))
	{
		printf ("\n\nComprobando integridad del diccionario : \n\n");
	
		i=0;
		npa = (nodeptr *) &np1;

		while ( (*npa) != (nodeptr) NULL )
		{
			printf ("i: %3d  (**npa).num: %3d  (**npa).wrd: |%s|\n",i,(**npa).num,(**npa).wrd);
			npa = (nodeptr *) & (*npa)->nx;
			i++;
		}

		printf ("\n");
	}
}







/*
 * -----------------------------------------------------------------------------------
 *
 *	bm_load
 *	carga la base de datos de marcas
 *
 * -----------------------------------------------------------------------------------
 */

int	bm_load()
{
	char	b1[MAXB];
	char	b2[MAXM];
	int	flag;
	int	p1,p2;
	int	i,j,k;
	int	f1;

	f1=0;
	q_bm=0;
	k=0;

	if (gp_fverbose("d1"))
	{
		printf ("\nCargando tabla de marcas  \n\n");
	}


	while (fgets(b1,MAXB,hfmrk) != NULL)
	{
		if (gp_fverbose("d3"))
		{
			printf ("%3d %s",q_bm,b1);
		}

		if (!linea_vacia(b1)  && b1[0] != '#' )
		{
			f1=1;
		}

		if (f1)
		{
			p1=0;
			p2=0;
			k=0;

			while (b1[p1] )
			{	
				b2[k++] = b1[p1];

				if (b1[p1] == ' ' || b1[p1] == '\n')
				{	
					b2[k-1]=0;

					if (p2 == 0)
					{	strcpy(bm_key[q_bm],b2);

					}

					if (p2 == 4)
					{	strcpy(bm_mrk[q_bm],b2);

					}

					p2++;
					k=0;
				}

				p1++;

			}
		
			/* next ... */
			q_bm++;
			f1=0;

			if (q_bm >= MAXK)
				error(301);
		}
	}

	if (gp_fverbose("d1"))
	{
		printf ("Cantidad de marcas cargadas en tabla : %6d\n\n",q_bm);
	}

	if (gp_fverbose("d3"))
	{
		printf ("\n\nComprobando integridad tabla de marcas \n\n");

		for (j=0; j<q_bm; j++)
			printf ("%3d,%-10.10s,%s\n",j,bm_mrk[j],bm_key[j]);

		printf ("\n");
	}

}







/*
 * -----------------------------------------------------------------------------------
 *
 * 	bm_tag
 *
 * 	Concatena a string, el tag correspondiente, si coincide la palabra clave
 *
 * -----------------------------------------------------------------------------------
 */


char	*bm_tag(s)
char	*s;
{
	static char b1[MAXB];

	char	b2[MAXM];
	int	i,j,k;
	int	f1,f2;


	strcpy(b1,s);

	f1=1;

	for ( j=0; f1 && j< q_bm; j++)
	{
		if (strcmp(s,bm_key[j]) == 0)
		{
			f1=0;
			sprintf (b2,"_%s",bm_mrk[j]);
			strcat(b1,b2);
		}
	}

	return b1;
}



/*
 * -----------------------------------------------------------------------------------
 *
 *	gp_print
 *
 * -----------------------------------------------------------------------------------
 */

int	gp_print()
{
	int i;

	printf ("\n");
	printf ("Cantidad de par   : %2d\n",gp_fq(GP_GET,0) - 1 );
	for (i=0; i<3; i++)
		printf ("Cantidad de par %d : %2d\n",i+1,gp_tpar[i]);
	printf ("\n");

}




/*
 * -----------------------------------------------------------------------------------
 *
 *	gp_parser
 *
 *	parser general de parametros de input al programa
 * 
 *
 * -----------------------------------------------------------------------------------
 */

int	gp_parser()
{

	int i,j;
	char	prm[MAXV];

	/* excepcion con verbose, por si lo pusieron al final de la linea, por defecto d5 !!! */

	for (i=0; i < gp_fq(GP_GET,0); i++  )
	{	if ( *( gp_fp(GP_GET,i,(char **)0) + 1) == 'v' )
#if 0
	/* si hay parametro -vac=algo ...
	 * esto no funca
	 * aca hay que agregar el 'y no tiene igual. ... '
	 */
		if ( *( gp_fp(GP_GET,i,(char **)0) + 1) == 'v' && !tiene_igual( gp_fp(GP_GET,i,(char **)0) ) )
#endif
		{	gp_verbose = 1;
			strcpy(gp_opciones,"d5");
		}
		if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"opciones",6) )
		{	strcpy(gp_opciones,desde_igual( gp_fp(GP_GET,i,(char **)0)));
		}


	}

	for (i=0; i < gp_fq(GP_GET,0);  )
	{

		/* parameter type 1 ... "name" */
		if ( i &&  *( gp_fp(GP_GET,i,(char **)0) + 0) != '-')
		{
			gp_q_partype1++;
			gp_tpar[0]++;

			if (gp_q_partype1 == 1)
			{
				strcpy(finp, gp_fp(GP_GET,i,(char **)0));
				ffinp=1;
			}

			if (gp_q_partype1 == 2)
			{
				strcpy(fout, gp_fp(GP_GET,i,(char **)0));
				ffout=1;
			}

			if (gp_fverbose("d5"))
			{
				printf ("Param tipo 1: %s\n", gp_fp(GP_GET,i,(char **)0 ) );
			}
		}

		/* parameter type 2 ... "-something" */
		if ( i && *( gp_fp(GP_GET,i,(char **)0) + 0) == '-' && !tiene_igual( gp_fp(GP_GET,i,(char **)0) ) )
		{
			gp_q_partype2++;
			gp_tpar[1]++;

			if ( *( gp_fp(GP_GET,i,(char **)0) + 1) == 'v'  )
				gp_verbose = 1;

			if ( *( gp_fp(GP_GET,i,(char **)0) + 1) == 'h'  )
				gp_help = 1;

			if ( *( gp_fp(GP_GET,i,(char **)0) + 1) == 'm'  )
				gp_minusculas = 1;

			if ( *( gp_fp(GP_GET,i,(char **)0) + 1) == 'f'  )
				gp_fsentencia = 1;

			if ( *( gp_fp(GP_GET,i,(char **)0) + 1) == 'e'  )
				gp_eol = 1;

			if ( *( gp_fp(GP_GET,i,(char **)0) + 1) == 'i'  )
				gp_reidx = 1;

			if ( *( gp_fp(GP_GET,i,(char **)0) + 1) == 'p'  )
				gp_pause = 1;

			if (gp_fverbose("d5"))
			{
				printf ("Param tipo 2: %s\n", gp_fp(GP_GET,i,(char **)0 ) );
			}
		}

		/* parameter type 3 ... "-someoption=somename" */
		if ( i && *( gp_fp(GP_GET,i,(char **)0) + 0) == '-' && tiene_igual( gp_fp(GP_GET,i,(char **)0) ) )
		{
			gp_q_partype3++;
			gp_tpar[2]++;

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"inp",3) )
			{
				strcpy(finp,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				ffinp=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"out",3) )
			{
				strcpy(fout,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				ffout=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"aux",3) )
			{	
				strcpy(faux,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				ffaux=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"log",3) )
			{	
				strcpy(flog,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				fflog=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"dic",3) )
			{	gp_diccionario=1;
				strcpy(fdic,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				ffdic=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"mrk",3) )
			{	gp_tabmrk=1;
				strcpy(fmrk,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				ffmrk=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"pmp",3) )
			{	
				strcpy(fpmp,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				ffpmp=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"cde",3) )
			{	
				strcpy(fcde,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				ffcde=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"gco",3) )
			{	
				strcpy(fgco,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				ffgco=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"com",3) )
			{	
				strcpy(fcom,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				ffcom=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"gru",3) )
			{	
				strcpy(fgru,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				ffgru=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"rub",3) )
			{	
				strcpy(frub,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				ffrub=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"gdc",3) )
			{	
				strcpy(fgdc,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				ffgdc=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"tst",3) )
			{	
				strcpy(ftst,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				fftst=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"tb1",3) )
			{	
				strcpy(ftb1,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				fftb1=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"tb2",3) )
			{	
				strcpy(ftb2,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				fftb2=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"tb3",3) )
			{	
				strcpy(ftb3,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				fftb3=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"tb4",3) )
			{	
				strcpy(ftb4,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				fftb4=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"per",3) )
			{	
				strcpy(fper,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				ffper=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"gdp",3) )
			{	
				strcpy(fgdp,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				ffgdp=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"gpe",3) )
			{	
				strcpy(fgpe,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				ffgpe=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"cpe",3) )
			{	
				strcpy(fcpe,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				ffcpe=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"gtx",3) )
			{	
				strcpy(fgtx,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				ffgtx=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"trx",3) )
			{	
				strcpy(ftrx,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				fftrx=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"tru",3) )
			{	
				strcpy(ftru,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				fftru=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"sta",3) )
			{	
				strcpy(fsta,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				ffsta=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"wek",3) )
			{	
				strcpy(fwek,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				ffwek=1;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"fxa",3) )
			{	
				strcpy(ffxa,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				fffxa=1;
			}




			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"nvd",3) )
				gp_niveldes = *desde_igual( gp_fp(GP_GET,i,(char **)0)) - '0';

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"dng",3) )
			{	gp_detneg = 1;
				gp_tipneg = 1;
				strcpy(prm,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				if (strncmp(pasar_a_minusc(prm),"lex",3) == 0)
					gp_tipneg = 1;
				if (strncmp(pasar_a_minusc(prm),"nga",3) == 0)
					gp_tipneg = 2;
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"grm",3) )
			{	
				strcpy(fgrm,desde_igual( gp_fp(GP_GET,i,(char **)0)));
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"opciones",6) )
			{	
				strcpy(gp_opciones,desde_igual( gp_fp(GP_GET,i,(char **)0)));
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"prueba",6) )
			{	
				strcpy(gp_pruebas,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				ffprb=gp_pruebas[0]-'0';
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"exec",4) )
			{	
				strcpy(gp_exec,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				ffexc=gp_exec[0]-'0';
			}

			if (!strncmp(gp_fp(GP_GET,i,(char **)0)+1,"proc",4) )
			{	
				strcpy(gp_proc,desde_igual( gp_fp(GP_GET,i,(char **)0)));
				ffpro=gp_proc[0]-'0';
			}



			if (gp_fverbose("d5"))
			{
				printf ("Param tipo 3: %s\n", gp_fp(GP_GET,i,(char **)0 ) );
			}
		}

		/* program name */
		if (i==0)
		{	
			if (gp_fverbose("d5"))
			{
				printf ("Programa    : %s\n", gp_fp(GP_GET,i,(char **)0 ) );
			}
		}

		/* next ... */
		i++;
	}

	/* futuro ...
	 * Hacer una gramatica para la combinacion de comandos
	 * posibles !!
	 *
	 * Por ahora, revisamos combinaciones basicas que
	 * son necesarias entre parametros ...
	 *
	 */

	/* si no pone parametros ... mandar a uso */
	if ( gp_q_partype1 + gp_q_partype2 + gp_q_partype3 == 0 )
		gp_help = 1;

#if 0
	/* si no indico ningun  archivo ... mandar a uso */
	if ( gp_q_partype1 + gp_q_partype3 == 0 )
		gp_help = 1;
#endif 

	if (gp_fverbose("d5"))
	{
		printf ("\n\n");
	}
} 




/*
 * -----------------------------------------------------------------------------------
 *
 *	tiene_igual
 *
 * -----------------------------------------------------------------------------------
 */


int	tiene_igual(s)
char	*s;
{
	int i,flag;

	for (i=0, flag=0; i< strlen(s) && !flag; i++ )
		if ( *(s+i) == '=')
			flag=1;

	return flag;
}



/*
 * -----------------------------------------------------------------------------------
 *
 *	desde_igual
 *
 * -----------------------------------------------------------------------------------
 */


char	*desde_igual(s)
char	*s;
{
	int i,flag;

	for (i=0, flag=0; i< strlen(s) && !flag; i++ )
		if ( *(s+i) == '=')
			flag=1;

	return s+i;
}




/*
 * -----------------------------------------------------------------------------------
 *
 *	gp_init
 *
 * -----------------------------------------------------------------------------------
 */

int	gp_init(vpar_q,vpar_p)
int	vpar_q;
char	**vpar_p;
{
	int i;

	gp_fq(GP_SET,vpar_q);
	gp_fp(GP_SET,0,vpar_p);


}


/*
 * -----------------------------------------------------------------------------------
 *
 *	gp_fp
 *
 * -----------------------------------------------------------------------------------
 */

char	*gp_fp(action,offset,value)
int	action;
int	offset;
char	**value;
{
	static char **gp_value;

	if (action == GP_SET)
	{	
		gp_value=value;
	}

	return 	*(gp_value+offset);
}



/*
 * -----------------------------------------------------------------------------------
 *
 *	gp_fq(action,value)
 *	
 *	int action
 *	int value
 *	
 *	value:
 *	on fist call, value is a number (typically argc )
 *	after first call, value can be any number, and is not used
 *
 *	returns:
 *	value
 *
 * -----------------------------------------------------------------------------------
 */

int	gp_fq(action,value)
int	action;
int	value;
{
	static int gp_value=0;

	if (action == GP_SET)
	{	
		gp_value=value;
	}

	return gp_value;
}



/*
 * -----------------------------------------------------------------------------------
 *
 *	gp_test 
 *
 *	
 *
 * -----------------------------------------------------------------------------------
 */


int	gp_test()
{
	int i;

	printf ("Cantidad de par   : %2d\n",gp_fq(GP_GET,0) - 1 );
	for (i=0; i<3; i++)
		printf ("Cantidad de par %d : %2d\n",i+1,gp_tpar[i]);

	printf ("\n");
	for (i=0; i< gp_fq(GP_GET,0); i++)
		printf ("Parametro  %2d     : %s\n",i,gp_fp(GP_GET,i,(char **)0)  );
			
	printf ("\n");
	printf ("\n");
}

/*
 * -----------------------------------------------------------------------------------
 *
 * 	linea_vacia
 *
 *	Determina si una linea esta vacia (generalmente, para lineas leidas de files)
 *	La linea NO esta vacia si contiene al menos 1 caracter distinto de
 *	' ' 	blanco
 *	'\t' 	tab
 *	'\n'	new line
 *
 *
 * -----------------------------------------------------------------------------------
 */


int	linea_vacia(s)
char	*s;
{
	int i,flag;

	i=0;
	flag=1;

	for (i=0; flag && i< strlen(s); i++)
		if (s[i] != ' ' && s[i] != '\t' && s[i] != '\n' )
			flag=0;

	return flag;
}



/*
 * -----------------------------------------------------------------------------------
 *
 * 	error
 *
 *	display de errores
 *	en esta version es display de un numero
 *
 *
 * -----------------------------------------------------------------------------------
 */


int	error(value)
int	value;
{
	printf ("Error: %d\n",value);
	printf ("usar -h para help\n");
	exit(0);
}





/*
 * -----------------------------------------------------------------------------------
 *
 *	tipo_char
 *
 *	ATENCION:
 *	Falta contemplar que hacemos con el caracter '_' !!!!
 *	Por ahora lo incluyo en letras, porque si no, me separa el tag de las palabras
 *	Si voy a parsear corpus sin tag ... lo tengo que sacar de la lista !!!
 *
 *	0	fin de linea
 *	1	blanco o tab
 *	2	otra cosa ( x ahora ... )
 *
 * -----------------------------------------------------------------------------------
 */



int	tipo_char(c)
char	c;
{
	int x;

	x=TC_RST;

	if (c == '\n' )
		x = TC_EOL;

	if (c == ' ' || c == '\t' )
		x = TC_BLA;

	if (c == '_' )
		x = TC_GBJ;

	if (c == ',' || c == ';' || c == ':' || c == '-' || c == '/' )
		x = TC_CCE;

	if (c == '.' )
		x = TC_PNT;

	if (c == '(' || c == '[' || c == '{' )
		x = TC_PAA;

	if (c == ')' || c == ']' || c == '}' )
		x = TC_PAC;

#if 0
	if (c >= 'a' && c <= 'z' || c>= 'A' && c <= 'Z' || c == '_')
		x = TC_LET;
#endif
	if (c >= 'a' && c <= 'z' || c>= 'A' && c <= 'Z' )
		x = TC_LET;

	if (c >= '0' && c <= '9' )
		x = TC_NUM;

	if (c == '\'' || c == '%' )
		x = TC_CVR;

	return x;
}





/*
 * -----------------------------------------------------------------------------------
 *
 * 	es_numero
 *
 *	funcion para determinar si caracter es numero
 *
 *
 * -----------------------------------------------------------------------------------
 */




int	es_numero(c)
char	c;
{
	int x;

	x=0;
	if ( c >= '0' && c <= '9' )
		x=1;

	return x;
}


/*
 * -----------------------------------------------------------------------------------
 *
 * 	es_num_tk
 *
 *	funcion para determinar si string es numero
 *
 *
 * -----------------------------------------------------------------------------------
 */


int	es_num_tk(s)
char	*s;
{
	int i,x;

	x=1;
	for (i=0; x && i<strlen(s); i++)
	{	if (s[i] < '0' || s[i] > '9' )
			x=0;

	}

	return x;
}




/*
 * -----------------------------------------------------------------------------------
 *
 * 	es_puntuacion
 *
 *	funcion para determinar si es simbolo de puntuacion
 *
 *
 * -----------------------------------------------------------------------------------
 */




int	es_puntuacion(s)
char	*s;
{
	int i,x;

	x=1;

	for (i=0; x && i<strlen(s); i++)
	{	if (s[i] != '.' && s[i] != ',' && s[i] != ':' && s[i] != ';' )
			x=0;
	}

	return x;

}




/*
 * -----------------------------------------------------------------------------------
 *
 * 	es_word
 *
 *	funcion para determinar si es word o no
 *
 *
 * -----------------------------------------------------------------------------------
 */


int	es_word(s)
char	*s;
{
	int i,x;

	x=1;

	for (i=0; x && i<strlen(s); i++)
	{	if (s[i] < 'A' || s[i] > 'z' || ( s[i] > 'Z' && s[i] < 'a')  )
			x=0;
	}

	return x;

}

/*
 * -----------------------------------------------------------------------------------
 *
 * 	char_demed
 *
 *	caracteres utilizados normalmente en frases de medidas 
 *	1 cm x 1 cm ( 'x' ) y similares
 *
 *
 * -----------------------------------------------------------------------------------
 */


int	char_demed(c)
char	c;
{
	int	x;

	x=0;
	if ( c == 'e' || c == 'E' || c == 'x' || c == 'X' )
		x=1;

	return x;
}


/*
 * -----------------------------------------------------------------------------------
 *
 * 	mostrar
 *
 *	rutina para facilitar impresiones en debug
 *
 *
 * -----------------------------------------------------------------------------------
 */


int	mostrar(t_case,n_token,s_token)
int	t_case;
int	n_token;
char	*s_token;
{

	if (gp_fverbose("d3"))
	{
		printf ("t: %3d (%2d)   %s \n", n_token,t_case,s_token);
	}
}


/*
 * -----------------------------------------------------------------------------------
 *
 * 	uso
 *
 *	texto para describir el uso de la herramienta
 *
 *
 * -----------------------------------------------------------------------------------
 */

int	uso(x)
int	x;
{
	printf ("Usage: \n\n");
	printf ("inp_file out_file                   inp_file: texto input  out_file: archivo de salida         \n");
	printf ("-inp=inp_file -out=out_file         inp_file: texto input  out_file: archivo de salida         \n");
	printf ("-h                                  help                                                       \n");
	printf ("-v                                  verbose ... muestra cierta informacion de proceso          \n");
	printf ("                                                                                               \n");
	printf ("-pmp=fil                            carga archivo de mapa                                      \n");
	printf ("-pmp=fil -cde=fil                   carga archivo de mapa y de definicion de comercios         \n");
	printf ("-pmp=fil -cde=fil -gco=fil          carga mapa y def de comercios y genera base comercios      \n");
	printf ("-pmp=fil -com=fil                   carga mapa y comercios                                     \n");
	printf ("                                                                                               \n");
	printf ("-gru=fil                            genera archivo de rubros (para editar a mano)              \n");
	printf ("                                                                                               \n");
	printf ("-cde=fil                            carga archivo de personas                                  \n");
	printf ("-gtx=fil req: -cpe                  genera archivo de transacciones                            \n");
	printf ("                                                                                               \n");
	printf ("                                                                                               \n");
	printf ("-inp=f1 -out=f2 -pruebas=6          f1 es ordenado por PER - Va cargando todas las trx x per   \n");
	printf ("                                                                                               \n");
	printf ("-opciones=AABBCC...                 AA es del formato: letra-numero  ej:  -opciones=d4         \n");
	printf ("          dN     (va con -v )       (debug) Muestra distintos niveles de informacion           \n");
	printf ("                                    0 no imprime, 1 basico, 2 y 3 debug, 4 full debug          \n");
	printf ("\n\n\n");

	exit(x);
}


/*
 * -----------------------------------------------------------------------------------
 *
 * 	gp_default
 *
 * 	inicializa parametros de funcionamiento default
 *
 * -----------------------------------------------------------------------------------
 */

int	gp_default()
{
	gp_help=0;
	gp_verbose=0;
	gp_diccionario=0;
	gp_minusculas=0;
	gp_fsentencia=0;
	gp_eol=0;
	gp_niveldes=0;
	gp_tabmrk=0;
	gp_detneg=0;
	gp_tipneg=0;

	gp_reidx=0;
	gp_pause=0;

	ffinp=0;
	ffout=0;
	ffou2=0;
	ffdic=0;
	ffmrk=0;
	ffgrm=0;
	
	ffaux=0;

	fftst=0;
	ffprb=0;
	ffexc=0;
	ffpro=0;

	fftb1=0;
	fftb2=0;
	fftb3=0;
	fftb4=0;

	ffpmp=0;
	ffper=0;

	ffgdc=0;
	ffcde=0;

	ffgco=0;
	ffcom=0;

	ffgru=0;
	ffrub=0;

	ffper=0;
	ffgdp=0;
	
	ffgpe=0;
	ffcpe=0;

	ffgtx=0;
	fftrx=0;

	fftru=0;
	ffsta=0;
	ffwek=0;
	fffxa=0;

	sprintf (gp_opciones,"%s","______");
	sprintf (gp_pruebas, "%s","______");
	sprintf (gp_exec   , "%s","______");
	sprintf (gp_proc   , "%s","______");

	memset(gp_tpar,0,sizeof(gp_tpar));

}




/*
 * -----------------------------------------------------------------------------------
 *
 * 	pasar_a_minusc
 *
 *
 * -----------------------------------------------------------------------------------
 */

char	*pasar_a_minusc(s)
char	*s;
{
	static char b[MAXB];

	int i,j,k;

	strcpy(b,s);

	for (i=0; i<MAXB && b[i]; i++)
	{
		if ( b[i] >= 'A' && b[i] <= 'Z' )
			b[i] += 32;
	}

	return b;
}












/*
 * -----------------------------------------------------------------------------------
 *
 *	ATENCION !!
 *
 *	Aqui van las rutinas de soporte a las actividades de lex & yacc
 *
 *
 * -----------------------------------------------------------------------------------
 *
 */






/*
 * -----------------------------------------------------------------------------------
 *
 * 	readInputForLexer
 *
 *	ajusta funcionamiento del lexer para poder ler de strings
 *
 *
 * -----------------------------------------------------------------------------------
 */



int readInputForLexer( char *buffer, int *numBytesRead, int maxBytesToRead ) 
{
    int numBytesToRead = maxBytesToRead;
    int bytesRemaining = strlen(globalInputText)-globalReadOffset;

    int i;


    if ( numBytesToRead > bytesRemaining ) 
    {    
        numBytesToRead = bytesRemaining; 
    }

    for ( i = 0; i < numBytesToRead; i++ ) 
    {
        buffer[i] = globalInputText[globalReadOffset+i];
    }

    *numBytesRead = numBytesToRead;
    globalReadOffset += numBytesToRead;
    return 0;
}


/*
 * -----------------------------------------------------------------------------------
 *
 * 	gramatik
 *
 *	algunas funciones de apoyo al parser
 *
 *
 * -----------------------------------------------------------------------------------
 */


/*
 *	gramatik
 *
 *	Conexion entre el programa y los pasos que da 'yacc' analizando la gramatica
 *	Esta rutina debe ser llamada por cada shift o reduce que da el yacc
 *	de manera de detectar el momento que hace la reduccion de una frase con negacion.
 *
 *	f_cb		flag de 'continuo buscando'
 *	f_pe_lin	flag de 'palabra encontrada en la linea'
 *	f_pe_ora	flag de 'palabra encontrada en una oracion' 
 *
 */

int	gramatik(s,reduccion_neg)
char	*s;
int	reduccion_neg;
{
	int	f1;

	f1=0;

	if ( strcmp(s,global_ref) == 0)
	{	
		f_pe_lin = 1;
		f1=1;
	}


	/*
	 * si todavia sigo buscando la negacion ...
	 * y el parser acaba de reducir a una oracion con formato de negacion
	 * y la palabra fue encontrada en algun lugar del arbol antes de reducir ...
	 * => entonces la palabra buscada esta en el scope de esta negacion ...
	 * ( y no busco mas )
	 *
	 */

	if (f_cb)
	{
		if (reduccion_neg == 1)
		{
			if (f_pe_lin)
			{
				f_pe_ora = 1;
				f_cb = 0;
			}
		}
	}

	return f1;
}


/*
 * -----------------------------------------------------------------------------------
 *
 * 	grama_nga
 *
 *	algunas funciones de apoyo al parser de gramatica nga
 *
 *
 * -----------------------------------------------------------------------------------
 */



/*
 *	grama_nga
 *
 *	Conexion entre el programa y los pasos que da nnparse analizando la gramatica
 *
 *	f_cb		flag de 'continuo buscando'
 *	f_pe_lin	flag de 'palabra encontrada en la linea'
 *	f_pe_ora	flag de 'palabra encontrada en una oracion' 
 *
 */

int	grama_nga(s,reduccion_neg)
char	*s;
int	reduccion_neg;
{
	int	f1;

	f1=0;

	if ( strcmp(s,global_ref) == 0)
	{	
		f_pe_lin = 1;
		f1=1;
	}


	/*
	 * si todavia sigo buscando la negacion ...
	 * y el parser acaba de reducir a una oracion con formato de negacion
	 * y la palabra fue encontrada en algun lugar del arbol antes de reducir ...
	 * => entonces la palabra buscada esta en el scope de esta negacion ...
	 * ( y no busco mas )
	 *
	 */

	if (f_cb)
	{
		if (reduccion_neg == 1)
		{
			if (f_pe_lin)
			{
				f_pe_ora = 1;
				f_cb = 0;
			}
		}
	}

	return f1;
}





/*
 * -----------------------------------------------------------------------------------
 *
 * cocina / experimentos / almacen / guarda tuti ...
 *
 * -----------------------------------------------------------------------------------
 */


#if 0
/*	nnparse	*/

			printf ("\nAntes del switch    nro_re: %d  tip: %d  ptk: %2d  tr: |%10.10s|\  ptl: %2d  sig: %d  match: %d  cha: %d  cond:  %d  tk:  |%s|\n",
				nro_regla,tipo, ptk,(*tbr[nro_regla][ptk]).wrd,ptl, sigo,match,chances,condiciones,tk[ptl]);

				printf ("\nPie w reg act matc  nro_re: %d  tip: %d  ptk: %2d  tr: |%10.10s|  ptl: %2d  sig: %d  match: %d  cha: %d  cond:  %d  tk:  |%s| \n",
					nro_regla,tipo, ptk,(*tbr[nro_regla][ptk]).wrd,ptl, sigo,match,chances,condiciones,tk[ptl]);

				printf ("\nPie w reg act matc  nro_re: %d  tip: %d  ptk: %2d  tr: |%10.10s|  ptl: %2d  sig: %d  match: %d  cha: %d  cond:  %d  tk:  |%s| \n",
					nro_regla,tipo, ptk,"vacio",ptl, sigo,match,chances,condiciones,tk[ptl]);


#endif


#if 1

/*
 *	gp_fverbose
 *
 *
 *	Devuelve 1 o 0 si aplica la condicion de verbose
 *	
 *	Las opciones involucradas son:
 *	-v               activa el verbose
 *	-opciones=str    string de condiciones de verbose  (requiere -v )
 *	                 
 *	El string de opciones es del tipo "Ln...(1 o 3)"
 *	Donde:
 *	L       Es la letra (I informativo, D debug, y E ... no me acuerdo
 *	n       Es el nivel
 *	        0 no imprime nada   ( es lo mismo que no poner nada !!! )
 *	        1    imprime cosas basicos, por que rutinas paso etc
 *	        2,3  niveles de debug intermedios
 *	        4    full debug
 *	        5    incluye debug de gp_parse
 *
 * 
 *
 *	
 */


int	gp_fverbose(situacion)
char	*situacion;
{
	int	i,j,k;
	int	sit;

	static	int	f_p=1;
	static	int	f_i=0;
	static	int	f_e=0;
	static	int	f_d=0;

	if ( f_p == 1 && gp_opciones[0] != '_' )
	{	f_p = 0;

		for (i=0; i<strlen(gp_opciones)-1; i=i+2)
		{
			if (*(gp_opciones+i) == 'i' || *(gp_opciones+i) == 'I' )
				f_i = (int) *(gp_opciones+i+1) - '0';
			if (*(gp_opciones+i) == 'e' || *(gp_opciones+i) == 'E' )
				f_e = (int) *(gp_opciones+i+1) - '0';
			if (*(gp_opciones+i) == 'd' || *(gp_opciones+i) == 'D' )
				f_d = (int) *(gp_opciones+i+1) - '0';
		}
	}


	sit = 0;

	if (gp_verbose)
	{

		/* determino si hay que devolver 0 o 1
		 * en base a situacion informada, y 
		 * conjunto de flags y '-opcion=vN'   
		 *
		 */
	
		for (sit=0, i=0; i<strlen(situacion); i=i+2)
		{


#if 0
			printf ("X1 f_P: %d i:%d situaci: %s  opcion: %s f_d: %d sit: %d\n",
				f_p,i,situacion,gp_opciones,f_d,sit);
#endif
	
			if (*(situacion+i) == 'i')
				if ( *(situacion+i+1)-'0' <= f_i )
					sit = 1;
	
			if (*(situacion+i) == 'e')
				if ( *(situacion+i+1)-'0' <= f_e )
					sit = 1;
	
			if (*(situacion+i) == 'd')
				if ( *(situacion+i+1)-'0' <= f_d )
					sit = 1;

#if 0
			printf ("X2 f_P: %d i:%d situaci: %s  opcion: %s f_d: %d sit: %d\n",
				f_p,i,situacion,gp_opciones,f_d,sit);
#endif
		}
	}

	return	sit;
}




#endif


#if 1

char	*gp_tm()
{
	time_t t;
  	struct tm *tm;

	t=time(NULL);
	tm=localtime(&t);
	strftime(gp_fyh, MAXR , "[%Y/%m/%d %H:%M:%S]", tm);

	return(gp_fyh);
}


#endif



#if 1

char	*df(s)
char	*s;
{
	static	char	nf[MAXF];

	int	i,j,k,f1;

	for ( f1=1, i=0; f1 && i<strlen(s); i++)
		if (s[i] != '.')
			nf[i]=s[i];
		else
			nf[i]=0,f1=0;

	strcat(nf,".dif");


	return nf;
}


#endif


#if 1


/*
 * -----------------------------------------------------------------------------------
 *
 *	bd_buscar
 *
 *	buscar una palabra en la base de datos del diccionario
 *
 * -----------------------------------------------------------------------------------
 */


/*
 *	Fri Oct 12 12:59:04 -03 2018
 *
 *	bd_buscar
 *	buscar una palabra en la base de datos del diccionario
 *	diccio base: diccio01.txt
 *
 *	q_wrd	cantidad de palabras en el diccionario
 *
 *	Mejorar:
 *	Busca linealmente. Esto no es muy eficiente, pero en la etapa de pruebas,
 *	es util agregar y sacar palabras o marcas del diccionario sin preocuparse
 *	de ordenar el archivo antes de procesar...
 *	Con un diccionario ya estable, se puede implementar busqueda binaria
 *
 *
 *
 *
 */

int	bd_buscar(palabra)
char	*palabra;

{
	char	b1[MAXB];
	int	flag,f1;
	int	i,j,k;
	int	ql;


	if (gp_fverbose("d4"))
	{
		printf ("busco |%s| \n",palabra);
	}


	
	flag=0;
	i=0;
	npa = (nodeptr *) &np1;

	while ( !flag && (*npa) != (nodeptr) NULL )
	{
		if (strcmp(palabra, (**npa).wrd ) == 0)
			flag=1;

#if 0
		printf ("i: %3d  (**npa).num: %3d  (**npa).wrd: |%s|\n",i,(**npa).num,(**npa).wrd);
#endif

		npa = (nodeptr *) & (*npa)->nx;
		i++;
	}



	return flag;
}



#endif

/* -------------- end of source --------------------- */
/* -------------- end of source --------------------- */
/* -------------- end of source --------------------- */
/* -------------- end of source --------------------- */
/* -------------- end of source --------------------- */
/* -------------- end of source --------------------- */
/* -------------- end of source --------------------- */















int	bpru_load()
{
	int	i;

	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra a  bpru_load \n\n",gp_tm());
	}

	bcvs_load("Cargando pru ", hftst, fx );

	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale  de bpru_load \n\n",gp_tm());
	}
}



int	fx(act,pos,str)
int	act;
int	pos;
char	*str;
{

	if (act == 0)
	{
		printf ("act 0: %2d %s \n",pos,str);
	}

	if (act == 1)
	{
		printf ("act 1: %2d  \n",pos);
	}

	if (act == 2)
	{
		printf ("act 2: %2d  \n",pos);
	}

}









/*
 * -----------------------------------------------------------------------------------
 *
 *	bcvs_load
 *
 *	
 *	carga archivo de campos separados por coma
 *
 * -----------------------------------------------------------------------------------
 */


/*
 *	
 *	msg1		mensage al comienzo ...
 *	fh		file handler archivo a procesar 
 *	fx		funcion a llamar con cada campo procesado
 *
 */


int	bcvs_load(msg1,fh,fx)
char	*msg1;
FILE	*fh;
int	(*fx)(int,int,char *);
{
	char	b1[MAXB];
	char	b2[MAXB];
	char	b3[MAXB];
	int	flag;
	int	p1,p2;
	int	i,j,k;
	int	f1;
	int	px,py;
	int	q_lin;


	f1=0;
	q_lin=0;
	k=0;

	if (gp_fverbose("d1"))
	{
		printf ("\n%s\n",msg1);
	}


	while (fgets(b1,MAXB,fh) != NULL)
	{

		if (gp_fverbose("d3"))
		{
			printf ("%06d: %s \n",q_lin,b1); 
		}

		if (!linea_vacia(b1)  && b1[0] != '#' )
		{
			f1=1;
		}

		if (f1)
		{
			p1=0;
			p2=0;
			k=0;

			while (b1[p1] )
			{	
				b2[k++] = b1[p1];

				if (b1[p1] == ',' || b1[p1] == '\n')
				{	
					b2[k-1]=0;

					strcpy(b3,b2);
					if (gp_fverbose("d4"))
						printf ("=%02d= %s\n",p2,b3);

					fx(0,p2,b3);

					p2++;
					k=0;
				}

				p1++;
			}
		
			/* next ... */

			q_lin++;
			f1=0;

			fx(1,q_lin," ");
		}
	}

	fx(2,q_lin," ");
}







/*
 * -----------------------------------------------------------------------------------
 *
 *	csv_parser
 *
 *	
 *	parser string tipo csv
 *
 * -----------------------------------------------------------------------------------
 */


/*
 *	
 *	fx		funcion a llamar con cada campo procesado
 *
 */


int	csv_parser(str,fx)
char	*str;
int	(*fx)(int,int,char *);
{
	char	b1[MAXB];
	char	b2[MAXB];
	char	b3[MAXB];

	int	p1,p2;
	int	k;
	int	f1;


	strcpy(b1,str);

	k=0;

		
	f1=1;

	if (f1)
	{
		p1=0;
		p2=0;
		k=0;

		while (b1[p1] )
		{	
			b2[k++] = b1[p1];

			if (b1[p1] == ',' || b1[p1] == '\n')
			{	
				b2[k-1]=0;

				strcpy(b3,b2);
				if (gp_fverbose("d4"))
					printf ("=%02d= %s\n",p2,b3);

				fx(0,p2,b3);

				p2++;
				k=0;
			}

			p1++;
		}
		
		/* termino la linea */
		fx(1,p2," ");
	}
}








/*
 * -----------------------------------------------------------------------------------
 *
 *	fp_trx  
 *
 *	funcion para csv_parser para parsear un registro de transaccion
 *
 * -----------------------------------------------------------------------------------
 */



int	fp_trx(act,pos,str)
int	act;
int	pos;
char	*str;
{
	if (act == 0)
	{
		if (gp_fverbose("d4"))
		{
			printf ("act 0: %2d %s \n",pos,str);
		}

		if ( pos == 0 )
		{
			strcpy ( tr_s.tx_trx_fecha , str);
		}

		if ( pos == 1 )
		{
			strcpy( tr_s.tx_trx_hora , str);
		}

		if ( pos == 2 )
		{
			tr_s.tx_trx_nro  = atoi(str);
		}

		if ( pos == 3 )
		{
			tr_s.tx_trx_monto  = atoi(str);
		}

		if ( pos == 4 )
		{
			tr_s.tx_art_desc  = atoi(str); 
		}

		if ( pos == 5 )
		{
			tr_s.tx_zon_px    = atoi(str); 
		}

		if ( pos == 6 )
		{
			tr_s.tx_zon_py    = atoi(str); 
		}

		if ( pos == 7 )
		{
			tr_s.tx_zon_iden  = atoi(str); 
		}

		if ( pos == 8 )
		{
			tr_s.tx_zon_inse  = atoi(str); 
		}

		if ( pos == 9 )
		{
			tr_s.tx_com_nro   = atoi(str); 
		}

		if ( pos == 10 )
		{
			tr_s.tx_com_px    = atoi(str); 
		}

		if ( pos == 11 )
		{
			tr_s.tx_com_py    = atoi(str); 
		}

		if ( pos == 12 )
		{
			tr_s.tx_com_inse  = atoi(str); 
		}

		if ( pos == 13 )
		{
			tr_s.tx_com_itic  = atoi(str); 
		}

		if ( pos == 14 )
		{
			tr_s.tx_com_nrub  = atoi(str); 
		}

		if ( pos == 15 )
		{
			tr_s.tx_per_nro   = atoi(str); 
		}

		if ( pos == 16 )
		{
			tr_s.tx_per_px    = atoi(str); 
		}

		if ( pos == 17 )
		{
			tr_s.tx_per_py    = atoi(str); 
		}

		if ( pos == 18 )
		{
			tr_s.tx_per_ifam  = atoi(str); 
		}

		if ( pos == 19 )
		{
			tr_s.tx_per_inse  = atoi(str); 
		}

		if ( pos == 20 )
		{
			tr_s.tx_cal_n1    = atoi(str); 
		}

		if ( pos == 21 )
		{
			tr_s.tx_cal_n2    = atoi(str); 
		}

	}

	if (act == 1)
	{
		/* fin de la linea */
	}
}






/*
 * -----------------------------------------------------------------------------------
 *
 *	fp_trx2  
 *
 *	funcion para csv_parser para parsear un registro de transaccion (pruebas8)
 *
 * -----------------------------------------------------------------------------------
 */



int	fp_trx2(act,pos,str)
int	act;
int	pos;
char	*str;
{
	if (act == 0)
	{
		if (gp_fverbose("d4"))
		{
			printf ("act 0: %2d %s \n",pos,str);
		}

#if 0
00000000,20180102,170013,000485,99,02,01,3,1,044422,2140,1404,2,2,124,0000000002,00,00,0,0,0,0
#endif
		if ( pos == 0 )
		{
			tr_s.tx_per_nro   = atoi(str); 
		}

		if ( pos == 1 )
		{
			strcpy ( tr_s.tx_trx_fecha , str);
		}

		if ( pos == 2 )
		{
			strcpy( tr_s.tx_trx_hora , str);
		}

#if 0
		if ( pos == 2 )
		{
			tr_s.tx_trx_nro  = atoi(str);
		}
#endif

		if ( pos == 3 )
		{
			tr_s.tx_trx_monto  = atoi(str);
		}

		if ( pos == 4 )
		{
			tr_s.tx_art_desc  = atoi(str); 
		}

		if ( pos == 5 )
		{
			tr_s.tx_zon_px    = atoi(str); 
		}

		if ( pos == 6 )
		{
			tr_s.tx_zon_py    = atoi(str); 
		}

		if ( pos == 7 )
		{
			tr_s.tx_zon_iden  = atoi(str); 
		}

		if ( pos == 8 )
		{
			tr_s.tx_zon_inse  = atoi(str); 
		}

		if ( pos == 9 )
		{
			tr_s.tx_com_nro   = atoi(str); 
		}

		if ( pos == 10 )
		{
			tr_s.tx_com_px    = atoi(str); 
		}

		if ( pos == 11 )
		{
			tr_s.tx_com_py    = atoi(str); 
		}

		if ( pos == 12 )
		{
			tr_s.tx_com_inse  = atoi(str); 
		}

		if ( pos == 13 )
		{
			tr_s.tx_com_itic  = atoi(str); 
		}

		if ( pos == 14 )
		{
			tr_s.tx_com_nrub  = atoi(str); 
		}

		if ( pos == 15 )
		{
			tr_s.tx_trx_nro  = atoi(str);
		}

		if ( pos == 16 )
		{
			tr_s.tx_per_px    = atoi(str); 
		}

		if ( pos == 17 )
		{
			tr_s.tx_per_py    = atoi(str); 
		}

		if ( pos == 18 )
		{
			tr_s.tx_per_ifam  = atoi(str); 
		}

		if ( pos == 19 )
		{
			tr_s.tx_per_inse  = atoi(str); 
		}

		if ( pos == 20 )
		{
			tr_s.tx_cal_n1    = atoi(str); 
		}

		if ( pos == 21 )
		{
			tr_s.tx_cal_n2    = atoi(str); 
		}

	}

	if (act == 1)
	{
		/* fin de la linea */
	}
}








/*
 * -----------------------------------------------------------------------------------
 *
 *	fp_trx3  
 *
 *	funcion para csv_parser para parsear un registro de transaccion (pruebas2)
 *
 * -----------------------------------------------------------------------------------
 */



int	fp_trx3(act,pos,str)
int	act;
int	pos;
char	*str;
{
	if (act == 0)
	{
		if (gp_fverbose("d4"))
		{
			printf ("act 0: %2d %s \n",pos,str);
		}

#if 0
00000000,20180102,170013,000485,99,02,01,3,1,044422,2140,1404,2,2,124,0000000002,00,00,0,0,0,0
#endif
		if ( pos == 0 )
		{
			strcpy ( tr_x.tx_trx_fecha , str);
		}

		if ( pos == 1 )
		{
			strcpy ( tr_x.tx_trx_hora , str);
		}

		if ( pos == 2 )
		{
			tr_x.tx_trx_nro  = atoi(str); 
		}

		if ( pos == 3 )
		{
			tr_x.tx_trx_monto  = atoi(str);
		}

		if ( pos == 4 )
		{
			tr_x.tx_art_desc  = atoi(str); 
		}

		if ( pos == 5 )
		{
			tr_x.tx_zon_px    = atoi(str); 
		}

		if ( pos == 6 )
		{
			tr_x.tx_zon_py    = atoi(str); 
		}

		if ( pos == 7 )
		{
			tr_x.tx_zon_iden  = atoi(str); 
		}

		if ( pos == 8 )
		{
			tr_x.tx_zon_inse  = atoi(str); 
		}

		if ( pos == 9 )
		{
			tr_x.tx_com_nro   = atoi(str); 
		}

		if ( pos == 10 )
		{
			tr_x.tx_com_px    = atoi(str); 
		}

		if ( pos == 11 )
		{
			tr_x.tx_com_py    = atoi(str); 
		}

		if ( pos == 12 )
		{
			tr_x.tx_com_inse  = atoi(str); 
		}

		if ( pos == 13 )
		{
			tr_x.tx_com_itic  = atoi(str); 
		}

		if ( pos == 14 )
		{
			tr_x.tx_com_nrub  = atoi(str); 
		}

		if ( pos == 15 )
		{
			tr_x.tx_per_nro  = atoi(str);
		}

		if ( pos == 16 )
		{
			tr_x.tx_per_px    = atoi(str); 
		}

		if ( pos == 17 )
		{
			tr_x.tx_per_py    = atoi(str); 
		}

		if ( pos == 18 )
		{
			tr_x.tx_per_ifam  = atoi(str); 
		}

		if ( pos == 19 )
		{
			tr_x.tx_per_inse  = atoi(str); 
		}

		if ( pos == 20 )
		{
			tr_x.tx_cal_n1    = atoi(str); 
		}

		if ( pos == 21 )
		{
			tr_x.tx_cal_n2    = atoi(str); 
		}

	}

	if (act == 1)
	{
		/* fin de la linea */
	}
}








/*
 * -----------------------------------------------------------------------------------
 *
 *	fp_idx1  
 *
 *	funcion para csv_parser para parsear un registro de indice1 
 *
 * -----------------------------------------------------------------------------------
 */



int	fp_idx1(act,pos,str)
int	act;
int	pos;
char	*str;
{
	if (act == 0)
	{
		if (gp_fverbose("d4"))
		{
			printf ("act 0: %2d %s \n",pos,str);
		}

		if ( pos == 0 )
		{
			s_idx1.ix_nro = atoi(str);
		}

		if ( pos == 1 )
		{
			s_idx1.ix_trx = atoi(str);
		}

	}

	if (act == 1)
	{
		/* fin de la linea */
	}
}











/*
 * -----------------------------------------------------------------------------------
 *
 *	end of source  
 *
 * -----------------------------------------------------------------------------------
 */

/*
 *    end of source
 *    end of source
 *    end of source
 *    end of source
 *    end of source
 *    end of source
 *    end of source
 *    end of source
 *    end of source
 *    end of source
 *    end of source
 */


/*
 *    end of source
 *    end of source
 *    end of source
 *    end of source
 *    end of source
 *    end of source
 *    end of source
 *    end of source
 *    end of source
 *    end of source
 *    end of source
 */


/*
 * -----------------------------------------------------------------------------------
 *
 *	xxxxx
 *
 *	obtiene el vector de negocios mas cercanos al indicado
 *
 * -----------------------------------------------------------------------------------
 */


int	xxxxx()
{
	int	i,j;
	int 	px,py;
	int	n1;

	px = tr1.tx_per_px;
	py = tr1.tx_per_py;




	for (i=0; i< zlmap[px][py][1]; i++)
	{

		zo_come[i][0] = (*mcome[ zlmap[px][py][0] + i ]).ncom ;
		zo_come[i][1] = abs ( (*mcome[i]).px - px ) + abs ( (*mcome[i]).py - py );

		if (zo_come[i][1] == 0 )
			zo_come[i][1] = 10000;
					
#if 1
		printf ("i: %4d zon (%d,%d)  zlmap: %5d %5d  zo_come[i][0]: %5d  ncom: %5d (%2d,%2d) dist: %6d \n",
			i,
			px,py, zlmap[px][py][0],zlmap[px][py][1],
			zo_come[i][0],
			(*mcome[ zo_come[i][0] ]).ncom,
			(*mcome[ zo_come[i][0] ]).px,
			(*mcome[ zo_come[i][0] ]).py,
			zo_come[i][1]
			);

#endif
	}


}





/* ex bcom_load ... con problemas */
#if 0






/*
 * -----------------------------------------------------------------------------------
 *
 *	bco1_load
 *
 *	carga la base de datos de comercios
 *
 * -----------------------------------------------------------------------------------
 */

int	bco1_load()
{
	char	b1[MAXB];
	char	b2[MAXB];
	char	b3[MAXB];
	int	flag;
	int	p1,p2;
	int	i,j,k,l;
	int	f1;
	int	px,py;
	int	pxa,pya;
	int	n1,n2,n3;


	f1=0;
	q_com=0;	
	k=0;


	if (gp_fverbose("d1"))
	{
		printf ("\nCargando elementos de comercio  \n\n");
	}


	while (fgets(b1,MAXB,hfcom) != NULL)
	{
		if (gp_fverbose("d3"))
		{
			printf ("%06d: %s",q_com,b1);
		}

		if (!linea_vacia(b1)  && b1[0] != '#' )
		{
			f1=1;
		}

		if (f1)
		{
			p1=0;
			p2=0;
			k=0;

			while (b1[p1] )
			{	
				b2[k++] = b1[p1];

				if (b1[p1] == ',' || b1[p1] == '\n')
				{	
					b2[k-1]=0;

					strcpy(b3,b2);
					if (gp_fverbose("d3"))
						printf ("=%02d= >%s<\n",p2,b3);

					if (p2 == 0 )
					{
						n1 = atoi(b3);
					}
					if (p2 == 1 )
					{
						n2 = atoi(b3);
					}
					if (p2 == 2 )
					{
						n3 = atoi(b3);
					}
#if 0
					if (p2 == 0)
					{	
						mcome[q_com] = (comeptr ) malloc(sizeof(come));
printf ("chequeo malloc ... \n");
/*
 *	estaba muuuy dormido se ve,
 *	porque el problema era en
 *	' if ( mcome[q_com] = NULL .... '
 */
						if (mcome[q_com] == NULL )
							error(901);
printf ("chequeo malloc ok ! ... \n");
#if 0
						memset (mcome[q_com],0,sizeof(come));
#endif

		printf ("voy a printf q_com: %d  b3: %s \n",q_com,b3);
						aux=0;
						printf ("p2==0 1.. =%s= aux: %d  \n",b3,aux);

					aux = (*mcome[q_com]).ncom;

						printf ("p2==0 2.. =%s= %d  \n",b3,aux);
	
						printf ("p2==0 3.. =%s= y %010d \n",b3,(*mcome[q_com]).ncom);
		printf ("voy a printf ok !! .\n");

						(*mcome[q_com]).ncom = atoi(b3);

						printf ("p2==0 ... '%s' y %010d \n",b3,(*mcome[q_com]).ncom);
					}

					if (p2 == 1)
					{	
					strcpy(b3,b2);
					if (gp_fverbose("d3"))
						printf ("=%02d= %s\n",p2,b3);
						printf ("p2==1 ... '%s' y %010d \n",b3,(*mcome[q_com]).zon_px);
						(*mcome[q_com]).zon_px = atoi(b3);
						printf ("p2==1 ... '%s' y %010d \n",b3,(*mcome[q_com]).zon_px);
					}

					if (p2 == 2)
					{	
					strcpy(b3,b2);
					if (gp_fverbose("d3"))
						printf ("=%02d= %s\n",p2,b3);
						(*mcome[q_com]).zon_px = atoi(b3);
					}

					if (p2 == 3)
					{	
					strcpy(b3,b2);
					if (gp_fverbose("d3"))
						printf ("=%02d= %s\n",p2,b3);
						strcpy ( (*mcome[q_com]).z_nom,b3);
					}

					if (p2 == 4)
					{	
					strcpy(b3,b2);
					if (gp_fverbose("d3"))
						printf ("=%02d= %s\n",p2,b3);
						strcpy ( (*mcome[q_com]).z_den,b3);
					}

					if (p2 == 5)
					{	
					strcpy(b3,b2);
					if (gp_fverbose("d3"))
						printf ("=%02d= %s\n",p2,b3);
						strcpy ( (*mcome[q_com]).z_nse,b3);
					}

					if (p2 == 6)
					{	
						(*mcome[q_com]).i_den = atoi(b3);
					}

					if (p2 == 7)
					{	
						(*mcome[q_com]).i_nse = atoi(b3);
					}

					if (p2 == 8)
					{	
						strcpy ( (*mcome[q_com]).nact,b3);
					}

					if (p2 == 9)
					{	
						strcpy ( (*mcome[q_com]).r_nom,b3);
					}

					if (p2 == 10)
					{	
						(*mcome[q_com]).min = atoi(b3);
					}

					if (p2 == 11)
					{	
						(*mcome[q_com]).max = atoi(b3);
					}

					if (p2 == 12)
					{	
						(*mcome[q_com]).qnt = atoi(b3);
					}

					if (p2 == 13)
					{	
						(*mcome[q_com]).i_tic = atoi(b3);
					}

					if (p2 == 14)
					{	
						(*mcome[q_com]).px = atoi(b3);
					}

					if (p2 == 15)
					{	
						(*mcome[q_com]).py = atoi(b3);
					}

					if (p2 == 16)
					{	
						strcpy ( (*mcome[q_com]).nse,b3);
					}

					if (p2 == 17)
					{	
						(*mcome[q_com]).c_nse = atoi(b3);
					}

					if (p2 == 18)
					{	
						(*mcome[q_com]).min2 = atoi(b3);
					}

					if (p2 == 19)
					{	
						(*mcome[q_com]).max2 = atoi(b3);
					}

					if (p2 == 20)
					{	
						(*mcome[q_com]).qnt2 = atoi(b3);
					}

					if (p2 == 21)
					{	
						(*mcome[q_com]).n_rub = atoi(b3);
					}

					if (p2 == 22)
					{	
						(*mcome[q_com]).p_rub = atoi(b3);
					}
#endif

					p2++;
					k=0;
				}

				p1++;

			}
		
			/* next ... */
			
			mcome[q_com] = (comeptr ) malloc(sizeof(come));
			(*mcome[q_com]).ncom = n1;
			(*mcome[q_com]).zon_px = n2;
			(*mcome[q_com]).zon_py = n3;

	printf ("reviso en loop... \n");
	for (l=0; l<q_com; l++)		
	{
		printf ("%6d: %d\n",
			(*mcome[l]).ncom );
	}


			q_com++;
			f1=0;

			if (q_com >= MAX_FCM * MAX_FCM * MAX_QCM  )
				error(307);
		}
	}


	printf ("reviso en loop... \n");
	for (l=0; l<q_com; l++)		
	{
		printf ("%6d: %d\n",
			(*mcome[l]).ncom );
	}

#if 0


	pxa = -1;	
	pya = -1;

	for (i=0; i<q_com; i++)
	{
		px = (*mcome[i]).zon_px;
		py = (*mcome[i]).zon_py;

		if ( ( pxa != (*mcome[i]).zon_px) || (pya != (*mcome[i]).zon_py) )
		{
			zlmap[px][py][0] = i;
			zlmap[px][py][1] = 1;
		}
		else
			zlmap[px][py][1]++;

		pxa = px;
		pya = py;
	}

	
	if (gp_fverbose("d2"))
	{
	for (px=0; px<MAX_FCM; px++)	
	{
		for (py=0; py<MAX_FCM; py++)
		{
			printf ("zlmap: %3d,%3d   %6d - %6d \n",
				px,py,zlmap[px][py][0],zlmap[px][py][1]);
		}
		printf ("\n");
	}
	}


	printf ("Elemento2 0,0 ... %6d %6d \n\n", (*mcome[0]).zon_py , (*mcome[0]).zon_py );

	if (gp_fverbose("d1"))
	{
		printf ("Cantidad de elementos cargados en comercio : %6d\n\n",q_com);
	}


	printf ("Elemento3 0,0 ... %6d %6d \n\n", (*mcome[0]).zon_py , (*mcome[0]).zon_py );

	if (gp_fverbose("d3"))
	{
		printf ("\n\nComprobando integridad de comercio \n\n");

		for (i=0; i<q_com; i++)
		{	
			printf ("%06d,%03d,%03d,%s,%s,%s,%d,%d,%s,%s,%03d,%03d,%3d,%d,%3d,%3d,%s,%d,%3d,%3d,%3d,%3d,%3d\n",
				(*mcome[i]).ncom,
				(*mcome[i]).zon_px,
				(*mcome[i]).zon_py,
				(*mcome[i]).z_nom,
				(*mcome[i]).z_den,
				(*mcome[i]).z_nse,
				(*mcome[i]).i_den,
				(*mcome[i]).i_nse,
				(*mcome[i]).nact,
				(*mcome[i]).r_nom,
				(*mcome[i]).min,
				(*mcome[i]).max,
				(*mcome[i]).qnt,
				(*mcome[i]).i_tic,
				(*mcome[i]).px,
				(*mcome[i]).py,
				(*mcome[i]).nse,
				(*mcome[i]).c_nse,
				(*mcome[i]).min2,
				(*mcome[i]).max2,
				(*mcome[i]).qnt2,
				(*mcome[i]).n_rub,
				(*mcome[i]).p_rub
				);
		}
		printf ("\n");
	}

	printf ("Elemento4 0,0 ... %6d %6d \n\n", (*mcome[0]).zon_py , (*mcome[0]).zon_py );
#endif

}




#endif
/* ex bcom_load ... con problemas */






/* registro de trx en file */
#if 0

/*
 *	Registro de trx en files
 *	Para generar las 'fd' ....
 * 
 *	1 - tomo un conjunto de transacciones que se vean generando, al azar 
 *	    (otra opcion ... seria 'clonar' generate transacciones !!! )
 *
 *	2 - x cada transaccion ...
 *	    a) tomar el 'comercio'
 *	    
 *	      
 *
 */

				/* Campos que tenemos que cambar en el
				 * nuevo registro !!
				 */


					/* datos de la transaccion */
		tr1.tx_trx_fecha  1	misma fecha
		tr1.tx_trx_hora   1     horas consecutivas, con poca diferencia
		tr1.tx_trx_monto  1     nuevos montos al azar
		tr1.tx_art_desc   no
					/* datos de la zona */
		tr1.tx_zon_px     no .. debe ser en la misma zona
		tr1.tx_zon_py     no 
		tr1.tx_zon_iden   no
		tr1.tx_zon_inse   no
					/* datos del comercio  */
		tr1.tx_com_nro    1     copiar los datos del comercio elegido
		tr1.tx_com_px     1
		tr1.tx_com_py     1
		tr1.tx_com_inse	  1
		tr1.tx_com_itic	  1
		tr1.tx_com_nrub   1
					/* datos de la persona  */
		tr1.tx_per_nro	  no
		tr1.tx_per_px	  no
		tr1.tx_per_py     no
		tr1.tx_per_ifam	  no
		tr1.tx_per_inse	  no
					/* datos para entrenamiento de ML */
		tr1.tx_cal_n1	
		tr1.tx_cal_n2 
#endif
/* registro de trx en file */




/* generar_fraude ?? */
#if 1




/*
 * -----------------------------------------------------------------------------------
 *
 *	genera_fraudes
 *
 *	genera tabla de transacciones con fraude
 *
 * -----------------------------------------------------------------------------------
 */

/*
 *	generar fraudes ...
 *
 *	caso 1
 *	Una tarjeta robada que genera muchas transacciones en corto tiempo
 *	antes de que el sistema la bloquee.
 *
 *	- genero al azar UNA transaccion, como las validas
 *	- a esa transaccion, le agrego un numero aleatorio de transacciones siguientes
 *	- los comercios deben ser cercanos
 *	- las fechas deben ser, o la misma, o a lo sumo un dia mas.
 *	- los horarios deben ser con poca diferencia
 *
 */


int	genera_fraudes()
{
	int	i,j,k;

	int	i_mes;
	int	i_per;
	int	i_trx; 

	int	v_tru[10000];		/* cantidad maxima ... subir esto ... */
	int	m_tru;			/* cantidad de 'paquete' de transacciones truchas a generar */
	int	q_tru;			/* count de ... */
	int	f_tru;			/* flag */
	int	i_tru;			/* index */
	int	j_tru;			/* index */

	int	m_dis[3][3];
	int	i_dis,j_dis;

	int	pe_tx_min;
	int	pe_tx_max;

	int	tx_rn_min;
	int	tx_rn_max;

	int	co_nro;
	int	co_ptr;

	int	i1,i2,i3,i4;
	int	q1,q2,q3,q4;
	int	n_aux;

	int	q_t[3];

	int	tx_monto;
	char	tx_fecha[16];
	char	tx_hora[16];

	int	px,py;
	int	pxa,pya;

	int	f1,f4;
	int	hasta;		/* hasta que numero de persona .. default q_gpe */

	int	i_per_ant;
	int	f_cambio_persona;
	int	q_cambio_persona;

	int	tr2_idx;


	/* proceso  */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra a genera_fraude \n\n",gp_tm());
	}

	/* si no cargo comercios no se puede generar transacciones */
	if (!ffcom)
		error(810);

	/* si no cargo personas  no se puede generar transacciones */
	if (!ffcpe)
		error(809);

	
	/* para detectar que cambio la persona */
	i_per_ant = -1;
	f_cambio_persona = -1;
	q_cambio_persona = 0;

	/* para detectar cambio de zona de la persona */
	pxa = -1;
	pya = -1;

	/* sub indice de transaccion */
	i_trx = 0;
	if (ffgtx)
		i_trx = t_trx;


#if 1
	q_tru = 0;
	m_tru = 5000;				/* 5000 trx 'truchas' de cabecera */
	hasta = m_tru;
#endif


	cfml = 3;		/* distancias '3' de los negocios */

	/* reporte  */
	if (gp_fverbose("d1"))
	{	printf ("Genera transacciones fd para %d personas \n\n",hasta);
	}


	/* por cada persona ... voy a generar sus transacciones */
	for (j_tru = 0; j_tru < hasta; j_tru++ )
	{

		/* borro datos de la trx */
		memset ( &tr1 , 0 , sizeof ( tr1) );

		/* genero persona al azar (verifico que no duplique)  */
		f_tru = 1;
		while (f_tru)
		{
			f_tru = 0;
			i_per = gna(0,q_gpe);			/* una persona al azar */
			for (i_tru=0; i_tru< q_tru; i_tru++)
				if (i_per == v_tru[i_tru])
					f_tru = 1;

			v_tru[q_tru]=i_per;
			if (!f_tru)
				q_tru++;
		}

			

		/* obtengo todos los datos de la persona que necesito */
		pe_tx_min = (*mpers[i_per]).ps_cxm[0];
		pe_tx_max = (*mpers[i_per]).ps_cxm[1];

		/* copio en reg trx -- datos de la persona */
		tr1.tx_per_nro   = (*mpers[i_per]).ps_nro;
		tr1.tx_per_px    = (*mpers[i_per]).ps_zon_px;
		tr1.tx_per_py    = (*mpers[i_per]).ps_zon_py;
		tr1.tx_per_ifam  = (*mpers[i_per]).ps_ifam;
		tr1.tx_per_inse  = (*mpers[i_per]).ps_inse;

		strcpy( tr1.tx_per_nom, (*mpers[i_per]).ps_nom );
		strcpy( tr1.tx_per_fam, (*mpers[i_per]).ps_fam );
		strcpy( tr1.tx_per_nse, (*mpers[i_per]).ps_nse );
		

		/* armo la matriz de porcentajes / rangos de gastos */
		for (i_dis = 0 ; i_dis < 3; i_dis++ )
		{
			for (j_dis = 0; j_dis < 3; j_dis++ )
			{
				m_dis[i_dis][j_dis] = (*mpers[i_per]).ps_dis[i_dis][j_dis];
			}
		}

#if 0
		printf ("Matriz de rangos ... \n");
		for (i=0; i<3; i++)
		{	printf ("rango %d :   ",i);

			for (j=0; j<3; j++)
				printf ("%5d ",m_dis[i][j]);
			printf ("\n");
		}
#endif


		if (pxa != tr1.tx_per_px || pya != tr1.tx_per_py )
		{

			/* inits varios */
			for (px=0; px<MAX_FCM; px++)
				for (py=0; py<MAX_FCM; py++)
					el_mapa[px][py]=0;

			/* matriz de zonas eligibles */
			determina_elegibles();                  

			/* vector de negocios elegibles */
			determina_vector_elegibles();
		
			pxa = tr1.tx_per_px;
			pya = tr1.tx_per_py;
		}

		/* selecciono el mes de la transaccion al azar */
		i_mes = gna(0,11);



			/* q1 es la cant de trx x mes de la persona, en el rango indicado */
			q1 = gna(pe_tx_min,pe_tx_max);

			/* q_t[x] es la proporcion de trx a realizar x montos en el rango x */
			for (i_dis =0; i_dis < 3; i_dis++)
				q_t[i_dis] = q1 * m_dis[i_dis][0] / 100;


#if 0
			printf ("tot trx: %2d   r1: %2d  r2: %2d  r3: %2d \n",q1,q_t[0],q_t[1],q_t[2] );
#endif

	
			/* transacciones de la persona x mes */	
			/* eligo uno de los grupos al azar. (antes ... for (i1=0; i1<3; i1++)  */
			i1 = gna(0,2);


				/* solo necesitamos 1 transaccion. por cada uno de los rang ind , gen trx */
				for (i2=0; i2 < 1; i2++)
				{

					/*
					 *	loop principal que genera LA transaccion
					 *	"gtrx"
					 */

					/* genero fecha y hora de la trx */
					strcpy (tx_fecha,gnf_fd(i_mes));		/* fecha tuneada para trx trucha */
					strcpy (tx_hora,gnh(8,10));			/* hora  tuneada para trx trucha */
					
					/* genero el monto */
					tx_rn_min = m_dis[i1][1];
					tx_rn_max = m_dis[i1][2];
					n_aux  = gna(tx_rn_min,tx_rn_max) / 10;
					n_aux = n_aux * 10;
					if (gna(1,10) >= 5)
						n_aux += 5;

					tx_monto = n_aux;

					
					/* selecciono comercio entre los elegibles ! */
					co_nro = gna(0,q_vec - 1);
					co_ptr = el_come[co_nro];

					/* copio en reg trx -- datos del comercio */
					tr1.tx_com_nro  = (*mcome[co_ptr]).ncom;         
					tr1.tx_com_px   = (*mcome[co_ptr]).px;	/* coord px py del comercio !! no de la zona */
					tr1.tx_com_py   = (*mcome[co_ptr]).py;
					tr1.tx_com_itic = (*mcome[co_ptr]).i_tic;
					tr1.tx_com_inse = (*mcome[co_ptr]).c_nse;
					tr1.tx_com_nrub = (*mcome[co_ptr]).n_rub;

					strcpy( tr1.tx_com_nom , (*mcome[co_ptr]).co_nom );		
					strcpy( tr1.tx_com_rub , (*mcome[co_ptr]).r_nom  );
					strcpy( tr1.tx_com_nse , (*mcome[co_ptr]).nse    );


					/* copio en reg trx -- datos de la zona de la transaccion */
					tr1.tx_zon_px   = (*mcome[co_ptr]).zon_px;
					tr1.tx_zon_py   = (*mcome[co_ptr]).zon_py;
					tr1.tx_zon_iden = (*mcome[co_ptr]).i_den;
					tr1.tx_zon_inse = (*mcome[co_ptr]).i_nse;

					strcpy( tr1.tx_zon_nom , (*mcome[co_ptr]).z_nom );
					strcpy( tr1.tx_zon_den , (*mcome[co_ptr]).z_den );
					strcpy( tr1.tx_zon_nse , (*mcome[co_ptr]).z_nse );

					/* copio en reg trx -- datos del articulo de la transaccion */	/* refinar a fururo */
					tr1.tx_art_desc = 99;
    

					/* reservo espacio para guardar datos de la trx
					 * una corrida full, genera 135 M de trx ...
					 * no tiene sentido por ahora, guardarla en memoria
					 */
#if 0
					mtrnx[i_per] = (trnxptr ) malloc(sizeof(trnx));
					if (mtrnx[i_per] == NULL )
						error(903);
#endif

					/* guardo rn reg trx -- datos de la trx */
					tr1.tx_trx_nro   = i_trx;
					tr1.tx_trx_dia   = 0;			/* refinar .. calcular dia de la semana */
					tr1.tx_trx_monto = tx_monto;		/* refinar .. ajustar a 0 o 5 el ultimo digito */
					strcpy ( tr1.tx_trx_fecha, tx_fecha );
					strcpy ( tr1.tx_trx_hora , tx_hora  );

					/* guardo en reg trx -- datos para usar en entrenaiento  */
					tr1.tx_cal_n1 = 0;
					tr1.tx_cal_n2 = 1;		/* trx fraude */



					/* impresion para debugging ... */
#if 1
					if ( j_tru % 1000 == 0)
#endif
	printf ("T%s %s j_tru %10d i_per %6d i_mes %2d i1 %3d i2 %3d  (%5d,%5d,%5d) co: %6d pos: %4d %4d zon: %2d %2d \n",
							tx_fecha,tx_hora,j_tru,i_per,i_mes,
							i1,i2,tx_rn_min,tx_rn_max,tx_monto,
							co_nro,		
							tr1.tx_com_px,
							tr1.tx_com_py,
							tr1.tx_zon_px,
							tr1.tx_zon_py
							);

					/* grabamos registro en file */

					/* super parche del momento, porque hacer un merge
					 * con un archivo de 135 millones no es tan facil,
					 * si esta generando transacciones con -gtx ...
					 * agrego tambien al final del archivo de trx
					 * las truchas 
					 */
#if 1
					if (ffgtx)
						graba_gtx(hfgtx,&tr1 );

					graba_gtx(hftru,&tr1 );

#endif



					/* incremento x la trx 'cabecera' */
					i_trx++;


					/* generar las restantes asociadas a la generada ... ACA
					 * Recordemos ...	
					 * caso 1 - muchas en corto tiempo, cerca del comercio inicial
					 * - fechas de la siguientes, igual o a lo sumo, un dia mas
					 * - horarios, con poca diferencia uno de otro
					 */

					/* genero el vector con los comercios que estan cerca de la trx 'cabecera' */
					create_fdt(&tr1);		
					
					/* elijo cantidad de trx truchas a agregar a la 'cabecera' */
					q3 = gna(5,7);
					q4 = gna(2,4);


					/* para la cantidade elegida, tomos los siguientes q3
					 * comercios mas cercanos 
					 * y genero la transaccion trucha
					 */

					/* copio el registro cabecera a la trx trucha */
					memcpy ( &tr2,&tr1, sizeof (trnx) );

					i4 = 0;
					f4 = 1;

					for (i3=0; i3<q3; i3++)
					{
		/*
		 *	que me falta ???
		 *
		 *	la fecha ... si la genera es 28 o 30 o 31 ... generar de nuevo con otro rango
		 *	para que sea mas facil generar las 'consecutivas' (o a los efectos del programa,
		 *	directamente generar una nueva ... )
		 *	
		 *	idem hora !!!
		 *
		 *	atenti !
		 *	la eleccion del negocio 'mas cerca' .....
		 *	podria ser mas 'distanciados' ... (cada 2 o 3 ... ) 
		 *	porque estan 'muy' cerca
		 *
		 *
		 */
						if (f4 && i3>2)
						{	i4 = gna(1,10);		/* index de probabilidad de cambio de fecha */
							if (i4 > 6)
								f4=0;
						}



						/* seleciono el proximo comercio mas cerca ... */
						tr2_idx = zo_come[i3*cfml][0];

#if 0
						tr2.tx_trx_fecha	... deberia estar pegada a la 'anterior'
						tr2.tx_trx_fecha	... deberia estar pegada a la 'anterior'
#endif

						strcpy (tr2.tx_trx_fecha,gnf_fd2(&tr1,i4));
						strcpy (tr2.tx_trx_hora ,gnh_fd2(&tr1,i4));
						i4=0;


						tr2.tx_trx_monto   = gna(700,3500);		/* refinar ... */
						tr2.tx_trx_nro     = i_trx;
						
						/* copio en reg trx -- datos del comercio */
						tr2.tx_com_nro  = (*mcome[tr2_idx]).ncom;         
						tr2.tx_com_px   = (*mcome[tr2_idx]).px;	/* coord px py del comercio !! no de la zona */
						tr2.tx_com_py   = (*mcome[tr2_idx]).py;
						tr2.tx_com_itic = (*mcome[tr2_idx]).i_tic;
						tr2.tx_com_inse = (*mcome[tr2_idx]).c_nse;
						tr2.tx_com_nrub = (*mcome[tr2_idx]).n_rub;

						if (i3 >= q4)
							tr2.tx_cal_n1 = 1;

						if (ffgtx)
							graba_gtx(hfgtx,&tr2 );
	
						graba_gtx(hftru,&tr2 );



						/* copio el ultimo registro como proxima cabecera  */
						memcpy ( &tr1,&tr2, sizeof (trnx) );

						/* siguiente trx */
						i_trx++;

					}
					/* for (i3 ... */

				fprintf (hftru,"%s","\n");


				i4 = 0;
				f4 = 0;


				}
				/* for (i2= ... */


		/* si vamos a agregar transacciones para fraud detection .. es ACA 
		 * cada ... 500 ... personas, agarramos la ultima de esa persona
		 * y generamos un lote de trx 'fraudulentas' al algoritmo de ML
		 */

	}	
	/* for (j_tru ...   */



	printf ("Cantidad de trx generadas .... %d \n\n",i_trx);

	/* proceso  */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de genera_fraudes \n\n",gp_tm());
	}

}



#endif
/* generar_fraude ?? */



int	graba_gtx(fh,tr)
FILE	*fh;
trnxptr	tr;
{
	
	fprintf (fh,"%s,%s,%010d,%06d,%02d,%02d,%02d,%d,%d,%06d,%04d,%04d,%d,%d,%03d,%08d,%02d,%02d,%d,%d,%d,%d\n",   
		(*tr).tx_trx_fecha,
		(*tr).tx_trx_hora,
		(*tr).tx_trx_nro,		
		(*tr).tx_trx_monto,
		(*tr).tx_art_desc,
		(*tr).tx_zon_px,
		(*tr).tx_zon_py,
		(*tr).tx_zon_iden,
		(*tr).tx_zon_inse,
		(*tr).tx_com_nro,
		(*tr).tx_com_px,
		(*tr).tx_com_py,
		(*tr).tx_com_inse,	
		(*tr).tx_com_itic,	
		(*tr).tx_com_nrub,
		(*tr).tx_per_nro,	
		(*tr).tx_per_px,	
		(*tr).tx_per_py,
		(*tr).tx_per_ifam,	
		(*tr).tx_per_inse,	
		(*tr).tx_cal_n1,	
		(*tr).tx_cal_n2 
		);
}



int	graba_trx(fh,tr)
FILE	*fh;
traxptr	tr;
{
	
	fprintf (fh,"%s,%s,%010d,%06d,%02d,%02d,%02d,%d,%d,%06d,%04d,%04d,%d,%d,%03d,%08d,%02d,%02d,%d,%d,%d,%d\n",   
		(*tr).tx_trx_fecha,
		(*tr).tx_trx_hora,
		(*tr).tx_trx_nro,		
		(*tr).tx_trx_monto,
		(*tr).tx_art_desc,
		(*tr).tx_zon_px,
		(*tr).tx_zon_py,
		(*tr).tx_zon_iden,
		(*tr).tx_zon_inse,
		(*tr).tx_com_nro,
		(*tr).tx_com_px,
		(*tr).tx_com_py,
		(*tr).tx_com_inse,	
		(*tr).tx_com_itic,	
		(*tr).tx_com_nrub,
		(*tr).tx_per_nro,	
		(*tr).tx_per_px,	
		(*tr).tx_per_py,
		(*tr).tx_per_ifam,	
		(*tr).tx_per_inse,	
		(*tr).tx_cal_n1,	
		(*tr).tx_cal_n2 
		);
}














#if 0




/*
 * -----------------------------------------------------------------------------------
 *
 *	fx_trx  
 *
 *	funcion para bcvs_load para carga de transacciones
 *
 * -----------------------------------------------------------------------------------
 */



int	fx_trx(act,pos,str)
int	act;
int	pos;
char	*str;
{
	if (act == 0)
	{
		if (gp_fverbose("d3"))
		{
			printf ("act 0: %2d %s \n",pos,str);
		}

		if ( pos == 0 )
		{
			strcpy ( (*tr_s).tx_trx_fecha , str);
		}

		if ( pos == 1 )
		{
			strcpy( (*tr_s).tx_trx_hora , str);
		}

		if ( pos == 2 )
		{
			(*tr_s).tx_trx_nro  = atoi(str);
		
		}

		if ( pos == 3 )
		{
			(*tr_s).tx_trx_monto  = atoi(str);
		
		}

		if ( pos == 4 )
		{
			(*tr_s).tx_art_desc  = atoi(str); 
	
		}

		if ( pos == 5 )
		{
			(*tr_s).tx_zon_px    = atoi(str); 

		}

		if ( pos == 6 )
		{
			(*tr_s).tx_zon_py    = atoi(str); 

		}

		if ( pos == 7 )
		{
			(*tr_s).tx_zon_iden  = atoi(str); 

		}

		if ( pos == 8 )
		{
			(*tr_s).tx_zon_inse  = atoi(str); 

		}

		if ( pos == 9 )
		{
			(*tr_s).tx_com_nro   = atoi(str); 

		}

		if ( pos == 10 )
		{
			(*tr_s).tx_com_px    = atoi(str); 

		}

		if ( pos == 11 )
		{
			(*tr_s).tx_com_py    = atoi(str); 

		}

		if ( pos == 12 )
		{
			(*tr_s).tx_com_inse  = atoi(str); 

		}

		if ( pos == 13 )
		{
			(*tr_s).tx_com_itic  = atoi(str); 

		}

		if ( pos == 14 )
		{
			(*tr_s).tx_com_nrub  = atoi(str); 

		}

		if ( pos == 15 )
		{
			(*tr_s).tx_per_nro   = atoi(str); 

		}

		if ( pos == 16 )
		{
			(*tr_s).tx_per_px    = atoi(str); 

		}

		if ( pos == 17 )
		{
			(*tr_s).tx_per_py    = atoi(str); 

		}

		if ( pos == 18 )
		{
			(*tr_s).tx_per_ifam  = atoi(str); 

		}

		if ( pos == 19 )
		{
			(*tr_s).tx_per_inse  = atoi(str); 

		}

		if ( pos == 20 )
		{
			(*tr_s).tx_cal_n1    = atoi(str); 

		}

		if ( pos == 21 )
		{
			(*tr_s).tx_cal_n2    = atoi(str); 

		}

	}

	if (act == 1)
	{
		q_trx++;

		if (q_trx >= MAX_TR2 )
			error( 313 );
	}

	if (act == 2)
	{
		/* nothing to do here !! */
	}
}





#endif



/* proceso_weka */
#if 0


/*
 * -----------------------------------------------------------------------------------
 *
 *	COPIA_pro_pruebas 6
 *
 *	pruebas ...
 *
 * -----------------------------------------------------------------------------------
 */

/*
 *	Al generar las transacciones 'buenas' por un lado, y las 'truchas' por otro
 *	se dan situaciones en donde, al ordenar todo por fecha, quedan algunas
 *	trx 'buenas' intercaladas entre las 'malas' y necesitamos que estas esten
 *	todas contiguas
 *
 *	Este proceso sive para leer un archivo de trx, ordenado por usuario 
 *	y por fecha , ir cargando en memoria todas las trx de un usuario
 *	y tenerlas disponibles para cualquier arreglo que haya que hacer.
 *	
 *	tambien seria la base de cualquier proceso (weka por ejemplo),
 *	que necesite generar un analis de 'las ultimas N trx ' de un usuario
 *
 */


int	COPIA_pro_pruebas6()
{
	char	b1[MAXB];
	char	b2[MAXB];

	int	tnv[10];		/* tabla 'no van' */

	int	i,j,k;
	int	i1,i2,i3;
	int	j1,j2,j3;
	int	q0,q1,q2,q3,q7;

	int	f1,f2,f3,f4,f5,f6,f7,f8;
	int	n1,n2;

	int	flag;

	char	*x;


	int	i_idx;
	int	i_per;

	int	t_trx_per;		/* total de trx de la persona */
	int	i_per_ant;		/* persona anterior */


	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra proceso prueba 6 \n\n",gp_tm());
	}

	/* necesitamos transacciones indicadas en -inp ...  */
	if ( !ffinp)
		error(813);

	/* necesitamos archivo de output -out ...  */
	if ( !ffout)
		error(814);



	/* algunos inits */
	for (i=0; i< MAX_TRX; i++)
	{
		mtrnx[i] = (trnxptr ) malloc(sizeof(trnx));
			if (mtrnx[i] == NULL )
				error(906);
	}



	i_idx      =  0;
	i_per_ant  = -1;
	flag       =  1;
	f4         =  0;


	/* leo reg x reg del archivo de input */
	while (flag)
	{
		x = fgets(b1,MAXB,hfinp);

		if ( x == NULL )
		{	
			f4 = 1;
			flag = 0;
		}

		if ( x != NULL)
		{
			memset ( &tr_s , 0 , sizeof ( tr_s) );
			csv_parser(b1,fp_trx);
			i_per = tr_s.tx_per_nro;


			if ( i_per_ant == i_per )
			{
				memcpy ( mtrnx[i_idx], &tr_s, sizeof (trnx) );
				i_idx++;
				f4 = 0;
			}

			if ( i_per_ant != i_per  )
			{
				if ( i_per_ant == -1 )
				{
					f4 = 0;
				}

				if ( i_per_ant != -1 )
				{
					f4 = 1;
				}
			}
		}

		/* 
		 *	En este punto ....
		 *	tengo TODAS las transacciones de una persona 
		 *	ordenadas por fecha - hora
		 *	A partir aca ... x cada registro, tomando de
		 *	a N ... hago el calculo de atributos !!! 
		 */

		if (f4)
		{
#if 0
			/* tengo que ordenarlos en la memoria (segun se requiera)  */
			q0 = i_idx;

			for (i1=0; i1<q0-1; i1++)
				for (j1=i1+1; j1<q0; j1++)
					if ( compara ( mtrnx[j1],mtrnx[i1] ) > 0 ) 
					{
						mtrnx[q0] = mtrnx[j1];
						mtrnx[j1] = mtrnx[i1];
						mtrnx[i1] = mtrnx[q0];
					}
			}
#endif

			/* verifico tiene trx con fraude */
			f5 = 0;
			for (i=0; i< i_idx; i++)
			{
				if ( (*mtrnx[i]).tx_cal_n2 == 1)
					f5++;
			}
			
			/* verifico si todas las truchas estan seguidas !! */
			i  = 0;
			f6 = 1;
			f7 = 0;		/* asumo que estan todas consecutivas */
			f8 = 1;
			q7 = 0;

			for (j=0; f6 && j< i_idx; j++)
			{
				if ( (*mtrnx[i+j]).tx_cal_n2 == 1 )
				{
					k=0;
					while (f8)
					{
						if ( (*mtrnx[i+j+k]).tx_cal_n2 == 1  )
							q7++;
						if ( (*mtrnx[i+j+k]).tx_cal_n2 == 0  )
						{	tnv[f7] = i+j+k;
							f7++;
						}

						if (q7 == f5)
							f8=0;
						k++;
						if (k >100)
						{
							error(604);  
						}
					}
				}

			}

		
			/* prueba */
			if (gp_fverbose("d3"))
			{	printf ("WEKA: per %10d con %10d regs (%10d fra) mod? %d  \n",i_per_ant,i_idx,f5,f7);
			}



			/* grabo ... */
			if (ffout)
			{

			}
			if (ffaux)
			{

			}
			
		}
		/* if (f4 ... */


		if ( i_per_ant != i_per )
		{
			memcpy ( mtrnx[0], &tr_s, sizeof (trnx) );
			i_per_ant = i_per;
			i_idx =  1;
		}

	}




	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de proceso prueba 6 \n\n",gp_tm());
	}
}





#endif
/* proceso_weka */





/* OJOJOJOJOJ ... son los numeros de personas que tienen trx truchas !!!!
 * no perder esta lista ...
 * corresponde al trx10 ...
 */


#if	XX_PER


char	xx_per[5000][10]  = {
"00000013",
"00000044",
"00000047",
"00000054",
"00000168",
"00000261",
"00000385",
"00000386",
"00000388",
"00000420",
"00000452",
"00000464",
"00000478",
"00000552",
"00000571",
"00000717",
"00000753",
"00000790",
"00000817",
"00000819",
"00000836",
"00000837",
"00000913",
"00000965",
"00000994",
"00001114",
"00001221",
"00001248",
"00001258",
"00001278",
"00001292",
"00001296",
"00001307",
"00001328",
"00001342",
"00001405",
"00001425",
"00001472",
"00001554",
"00001564",
"00001582",
"00001589",
"00001657",
"00001679",
"00001850",
"00001865",
"00001921",
"00002136",
"00002191",
"00002249",
"00002258",
"00002263",
"00002280",
"00002297",
"00002387",
"00002393",
"00002442",
"00002453",
"00002481",
"00002516",
"00002538",
"00002672",
"00002683",
"00002722",
"00002725",
"00002921",
"00002934",
"00003128",
"00003130",
"00003138",
"00003226",
"00003270",
"00003288",
"00003324",
"00003344",
"00003390",
"00003414",
"00003472",
"00003491",
"00003495",
"00003564",
"00003604",
"00003605",
"00003608",
"00003669",
"00003684",
"00003737",
"00003749",
"00003765",
"00003767",
"00003852",
"00003885",
"00003968",
"00004020",
"00004112",
"00004125",
"00004168",
"00004175",
"00004203",
"00004236",
"00004405",
"00004412",
"00004519",
"00004545",
"00004570",
"00004600",
"00004613",
"00004643",
"00004673",
"00004690",
"00004707",
"00004742",
"00004757",
"00004789",
"00004894",
"00004901",
"00004908",
"00004962",
"00005054",
"00005056",
"00005074",
"00005114",
"00005118",
"00005140",
"00005220",
"00005240",
"00005353",
"00005361",
"00005381",
"00005390",
"00005429",
"00005507",
"00005597",
"00005678",
"00005706",
"00005725",
"00005728",
"00005744",
"00005792",
"00005824",
"00005877",
"00005929",
"00006004",
"00006051",
"00006127",
"00006148",
"00006212",
"00006240",
"00006298",
"00006337",
"00006500",
"00006555",
"00006592",
"00006626",
"00006637",
"00006642",
"00006656",
"00006659",
"00006872",
"00006913",
"00006943",
"00007000",
"00007068",
"00007131",
"00007136",
"00007142",
"00007188",
"00007196",
"00007298",
"00007348",
"00007405",
"00007422",
"00007465",
"00007530",
"00007534",
"00007536",
"00007556",
"00007591",
"00007650",
"00007663",
"00007723",
"00007726",
"00007747",
"00007761",
"00007850",
"00007865",
"00007866",
"00007897",
"00007966",
"00007977",
"00008021",
"00008030",
"00008043",
"00008084",
"00008184",
"00008213",
"00008261",
"00008304",
"00008375",
"00008493",
"00008525",
"00008612",
"00008624",
"00008639",
"00008649",
"00008683",
"00008710",
"00008771",
"00008774",
"00008849",
"00008886",
"00008892",
"00008907",
"00008909",
"00009074",
"00009102",
"00009173",
"00009259",
"00009299",
"00009317",
"00009321",
"00009332",
"00009422",
"00009481",
"00009505",
"00009514",
"00009592",
"00009605",
"00009657",
"00009690",
"00009706",
"00009802",
"00009847",
"00009892",
"00009956",
"00009979",
"00010017",
"00010066",
"00010074",
"00010144",
"00010174",
"00010220",
"00010313",
"00010342",
"00010352",
"00010375",
"00010380",
"00010460",
"00010468",
"00010484",
"00010492",
"00010515",
"00010566",
"00010727",
"00010745",
"00010751",
"00010797",
"00010983",
"00011006",
"00011007",
"00011072",
"00011174",
"00011227",
"00011327",
"00011355",
"00011378",
"00011415",
"00011422",
"00011453",
"00011544",
"00011557",
"00011560",
"00011577",
"00011611",
"00011669",
"00011675",
"00011719",
"00011729",
"00011730",
"00011813",
"00011814",
"00011898",
"00011930",
"00011977",
"00012020",
"00012038",
"00012064",
"00012129",
"00012244",
"00012262",
"00012335",
"00012391",
"00012409",
"00012455",
"00012502",
"00012566",
"00012617",
"00012715",
"00012757",
"00012820",
"00012903",
"00012915",
"00012935",
"00012951",
"00012965",
"00012988",
"00013049",
"00013073",
"00013100",
"00013170",
"00013309",
"00013343",
"00013415",
"00013450",
"00013500",
"00013513",
"00013537",
"00013596",
"00013607",
"00013657",
"00013674",
"00013791",
"00013827",
"00013871",
"00013948",
"00013969",
"00014050",
"00014131",
"00014191",
"00014279",
"00014311",
"00014399",
"00014464",
"00014519",
"00014572",
"00014585",
"00014589",
"00014607",
"00014618",
"00014649",
"00014673",
"00014716",
"00014730",
"00014736",
"00014742",
"00014749",
"00014799",
"00014821",
"00014856",
"00014918",
"00014925",
"00014968",
"00014988",
"00015011",
"00015019",
"00015064",
"00015086",
"00015173",
"00015184",
"00015186",
"00015234",
"00015263",
"00015321",
"00015323",
"00015382",
"00015389",
"00015418",
"00015429",
"00015464",
"00015488",
"00015590",
"00015689",
"00015690",
"00015693",
"00015694",
"00015761",
"00015781",
"00015806",
"00015874",
"00015939",
"00015954",
"00015969",
"00015977",
"00015985",
"00016042",
"00016056",
"00016064",
"00016229",
"00016311",
"00016349",
"00016360",
"00016377",
"00016378",
"00016394",
"00016396",
"00016458",
"00016539",
"00016595",
"00016638",
"00016670",
"00016723",
"00016745",
"00016778",
"00016854",
"00016901",
"00016919",
"00016942",
"00016957",
"00016989",
"00017037",
"00017049",
"00017171",
"00017212",
"00017265",
"00017299",
"00017309",
"00017337",
"00017341",
"00017361",
"00017383",
"00017392",
"00017431",
"00017474",
"00017578",
"00017618",
"00017626",
"00017649",
"00017696",
"00017703",
"00017785",
"00017821",
"00017825",
"00017879",
"00017996",
"00018016",
"00018040",
"00018049",
"00018069",
"00018235",
"00018239",
"00018398",
"00018434",
"00018473",
"00018474",
"00018546",
"00018548",
"00018572",
"00018653",
"00018671",
"00018790",
"00018815",
"00018930",
"00018963",
"00018977",
"00019064",
"00019091",
"00019195",
"00019247",
"00019320",
"00019332",
"00019453",
"00019552",
"00019595",
"00019644",
"00019839",
"00019848",
"00019939",
"00019958",
"00019960",
"00019961",
"00019972",
"00020042",
"00020058",
"00020064",
"00020106",
"00020109",
"00020255",
"00020358",
"00020373",
"00020377",
"00020410",
"00020422",
"00020470",
"00020472",
"00020570",
"00020608",
"00020639",
"00020642",
"00020781",
"00020791",
"00020798",
"00020882",
"00020909",
"00020982",
"00021013",
"00021019",
"00021070",
"00021098",
"00021106",
"00021170",
"00021176",
"00021185",
"00021221",
"00021224",
"00021299",
"00021385",
"00021402",
"00021426",
"00021547",
"00021550",
"00021564",
"00021580",
"00021632",
"00021650",
"00021679",
"00021765",
"00021794",
"00021824",
"00021858",
"00021865",
"00021882",
"00022048",
"00022199",
"00022218",
"00022227",
"00022237",
"00022243",
"00022274",
"00022295",
"00022313",
"00022321",
"00022382",
"00022398",
"00022406",
"00022422",
"00022488",
"00022504",
"00022505",
"00022528",
"00022672",
"00022713",
"00022720",
"00022738",
"00022774",
"00022790",
"00022838",
"00022907",
"00022914",
"00022931",
"00023008",
"00023052",
"00023065",
"00023182",
"00023231",
"00023237",
"00023289",
"00023293",
"00023305",
"00023328",
"00023426",
"00023462",
"00023479",
"00023481",
"00023487",
"00023533",
"00023579",
"00023753",
"00023786",
"00023789",
"00023796",
"00023941",
"00023971",
"00023990",
"00024053",
"00024058",
"00024227",
"00024237",
"00024267",
"00024303",
"00024424",
"00024461",
"00024558",
"00024607",
"00024608",
"00024685",
"00024763",
"00024819",
"00024834",
"00024849",
"00024892",
"00024938",
"00024960",
"00024963",
"00025031",
"00025091",
"00025213",
"00025229",
"00025254",
"00025280",
"00025320",
"00025340",
"00025353",
"00025354",
"00025380",
"00025490",
"00025500",
"00025554",
"00025558",
"00025610",
"00025617",
"00025623",
"00025630",
"00025636",
"00025678",
"00025694",
"00025706",
"00025732",
"00025777",
"00025793",
"00025866",
"00025926",
"00025943",
"00025945",
"00025956",
"00026036",
"00026172",
"00026295",
"00026330",
"00026411",
"00026421",
"00026434",
"00026445",
"00026519",
"00026595",
"00026647",
"00026649",
"00026685",
"00026793",
"00026860",
"00026891",
"00026928",
"00026930",
"00026971",
"00026996",
"00027076",
"00027082",
"00027144",
"00027252",
"00027255",
"00027386",
"00027389",
"00027412",
"00027452",
"00027524",
"00027543",
"00027544",
"00027608",
"00027631",
"00027652",
"00027660",
"00027675",
"00027686",
"00027732",
"00027755",
"00027760",
"00027819",
"00027840",
"00027890",
"00027891",
"00027919",
"00027920",
"00027956",
"00028025",
"00028041",
"00028094",
"00028220",
"00028234",
"00028245",
"00028248",
"00028262",
"00028354",
"00028405",
"00028528",
"00028619",
"00028647",
"00028705",
"00028750",
"00028819",
"00028849",
"00028890",
"00028970",
"00028977",
"00028983",
"00029002",
"00029036",
"00029075",
"00029113",
"00029164",
"00029166",
"00029246",
"00029267",
"00029288",
"00029298",
"00029301",
"00029310",
"00029315",
"00029322",
"00029381",
"00029387",
"00029430",
"00029452",
"00029460",
"00029471",
"00029487",
"00029583",
"00029706",
"00029711",
"00029750",
"00029781",
"00029839",
"00029864",
"00029882",
"00029899",
"00029909",
"00029966",
"00029983",
"00030004",
"00030005",
"00030041",
"00030096",
"00030155",
"00030178",
"00030200",
"00030218",
"00030279",
"00030364",
"00030397",
"00030399",
"00030421",
"00030436",
"00030466",
"00030483",
"00030609",
"00030661",
"00030695",
"00030718",
"00030730",
"00030750",
"00030751",
"00030757",
"00030766",
"00030800",
"00030838",
"00030863",
"00030904",
"00030924",
"00031001",
"00031062",
"00031064",
"00031065",
"00031067",
"00031085",
"00031090",
"00031149",
"00031197",
"00031210",
"00031246",
"00031275",
"00031357",
"00031460",
"00031528",
"00031534",
"00031579",
"00031634",
"00031681",
"00031726",
"00031772",
"00031830",
"00031840",
"00031860",
"00031926",
"00031946",
"00032003",
"00032024",
"00032043",
"00032053",
"00032059",
"00032146",
"00032215",
"00032279",
"00032286",
"00032345",
"00032392",
"00032419",
"00032426",
"00032449",
"00032513",
"00032536",
"00032707",
"00032724",
"00032771",
"00032793",
"00032824",
"00032853",
"00032887",
"00032895",
"00032933",
"00032990",
"00033048",
"00033082",
"00033115",
"00033146",
"00033160",
"00033246",
"00033368",
"00033409",
"00033425",
"00033509",
"00033511",
"00033674",
"00033677",
"00033808",
"00033839",
"00033995",
"00034031",
"00034044",
"00034096",
"00034101",
"00034158",
"00034166",
"00034171",
"00034195",
"00034287",
"00034368",
"00034429",
"00034487",
"00034490",
"00034515",
"00034549",
"00034636",
"00034745",
"00034752",
"00034785",
"00034822",
"00034828",
"00034908",
"00034940",
"00034943",
"00034969",
"00034974",
"00035025",
"00035194",
"00035420",
"00035422",
"00035454",
"00035558",
"00035594",
"00035620",
"00035627",
"00035679",
"00035717",
"00035739",
"00035807",
"00035824",
"00035863",
"00035896",
"00035937",
"00035949",
"00036069",
"00036166",
"00036225",
"00036279",
"00036284",
"00036312",
"00036438",
"00036439",
"00036537",
"00036847",
"00036850",
"00036885",
"00036949",
"00036969",
"00036973",
"00036981",
"00036987",
"00037130",
"00037185",
"00037201",
"00037214",
"00037351",
"00037415",
"00037512",
"00037562",
"00037573",
"00037601",
"00037673",
"00037709",
"00037729",
"00037734",
"00037782",
"00037787",
"00037799",
"00037811",
"00037833",
"00037835",
"00037934",
"00037983",
"00038068",
"00038102",
"00038165",
"00038225",
"00038242",
"00038259",
"00038279",
"00038298",
"00038334",
"00038433",
"00038445",
"00038462",
"00038477",
"00038509",
"00038600",
"00038615",
"00038632",
"00038633",
"00038657",
"00038702",
"00038785",
"00038807",
"00038824",
"00038834",
"00038871",
"00038877",
"00038892",
"00038937",
"00039113",
"00039210",
"00039219",
"00039290",
"00039298",
"00039324",
"00039440",
"00039456",
"00039477",
"00039491",
"00039581",
"00039667",
"00039680",
"00039750",
"00039862",
"00039889",
"00039903",
"00039975",
"00040019",
"00040126",
"00040129",
"00040189",
"00040196",
"00040270",
"00040454",
"00040505",
"00040571",
"00040695",
"00040696",
"00040699",
"00040714",
"00040819",
"00040820",
"00040846",
"00040904",
"00041054",
"00041111",
"00041202",
"00041237",
"00041277",
"00041293",
"00041350",
"00041370",
"00041429",
"00041430",
"00041490",
"00041503",
"00041649",
"00041666",
"00041819",
"00041843",
"00041851",
"00041853",
"00041929",
"00041970",
"00042013",
"00042026",
"00042050",
"00042102",
"00042140",
"00042178",
"00042182",
"00042188",
"00042189",
"00042235",
"00042274",
"00042292",
"00042304",
"00042335",
"00042406",
"00042432",
"00042452",
"00042475",
"00042489",
"00042491",
"00042495",
"00042517",
"00042569",
"00042580",
"00042593",
"00042595",
"00042658",
"00042690",
"00042713",
"00042718",
"00042733",
"00042738",
"00042751",
"00042771",
"00042836",
"00042846",
"00042873",
"00042931",
"00042942",
"00042958",
"00042994",
"00043032",
"00043060",
"00043063",
"00043082",
"00043128",
"00043131",
"00043139",
"00043199",
"00043216",
"00043243",
"00043250",
"00043282",
"00043287",
"00043302",
"00043328",
"00043336",
"00043411",
"00043453",
"00043525",
"00043542",
"00043562",
"00043666",
"00043782",
"00043811",
"00043848",
"00043861",
"00043872",
"00043958",
"00043966",
"00044010",
"00044053",
"00044076",
"00044077",
"00044160",
"00044216",
"00044276",
"00044285",
"00044286",
"00044423",
"00044447",
"00044602",
"00044620",
"00044624",
"00044729",
"00044737",
"00044767",
"00044787",
"00044805",
"00044860",
"00044930",
"00044944",
"00044957",
"00044973",
"00045023",
"00045079",
"00045094",
"00045112",
"00045120",
"00045151",
"00045292",
"00045310",
"00045313",
"00045374",
"00045460",
"00045518",
"00045522",
"00045583",
"00045590",
"00045790",
"00045804",
"00045808",
"00045862",
"00045873",
"00045935",
"00045971",
"00045988",
"00046022",
"00046058",
"00046072",
"00046087",
"00046107",
"00046129",
"00046135",
"00046189",
"00046210",
"00046235",
"00046272",
"00046308",
"00046324",
"00046479",
"00046498",
"00046499",
"00046501",
"00046546",
"00046547",
"00046581",
"00046585",
"00046586",
"00046661",
"00046752",
"00046855",
"00046883",
"00046940",
"00046944",
"00046945",
"00046966",
"00046972",
"00046977",
"00046998",
"00047082",
"00047094",
"00047166",
"00047167",
"00047184",
"00047254",
"00047260",
"00047401",
"00047402",
"00047451",
"00047499",
"00047526",
"00047547",
"00047551",
"00047607",
"00047624",
"00047652",
"00047690",
"00047719",
"00047746",
"00047756",
"00047923",
"00047987",
"00048067",
"00048082",
"00048153",
"00048229",
"00048311",
"00048359",
"00048369",
"00048373",
"00048381",
"00048443",
"00048457",
"00048491",
"00048507",
"00048516",
"00048518",
"00048544",
"00048578",
"00048579",
"00048626",
"00048627",
"00048638",
"00048709",
"00048896",
"00048980",
"00049015",
"00049128",
"00049153",
"00049177",
"00049181",
"00049275",
"00049307",
"00049365",
"00049430",
"00049441",
"00049524",
"00049528",
"00049537",
"00049574",
"00049638",
"00049710",
"00049766",
"00049781",
"00049859",
"00049950",
"00049997",
"00050017",
"00050020",
"00050027",
"00050143",
"00050170",
"00050190",
"00050213",
"00050217",
"00050256",
"00050308",
"00050320",
"00050401",
"00050424",
"00050543",
"00050630",
"00050654",
"00050746",
"00050765",
"00050777",
"00050809",
"00050810",
"00050823",
"00050965",
"00050972",
"00051005",
"00051120",
"00051190",
"00051196",
"00051256",
"00051374",
"00051422",
"00051436",
"00051495",
"00051530",
"00051583",
"00051608",
"00051671",
"00051690",
"00051813",
"00051858",
"00051911",
"00051954",
"00051963",
"00052042",
"00052088",
"00052146",
"00052214",
"00052244",
"00052311",
"00052571",
"00052577",
"00052632",
"00052646",
"00052723",
"00052740",
"00052822",
"00052840",
"00052858",
"00052872",
"00052892",
"00052919",
"00052934",
"00052946",
"00052974",
"00052997",
"00053026",
"00053127",
"00053131",
"00053191",
"00053196",
"00053233",
"00053449",
"00053603",
"00053618",
"00053655",
"00053729",
"00053812",
"00053858",
"00053894",
"00053940",
"00053958",
"00053960",
"00053969",
"00053982",
"00053990",
"00054002",
"00054085",
"00054112",
"00054113",
"00054154",
"00054183",
"00054266",
"00054329",
"00054447",
"00054470",
"00054502",
"00054541",
"00054553",
"00054629",
"00054665",
"00054682",
"00054704",
"00054711",
"00054767",
"00054787",
"00054794",
"00054821",
"00054926",
"00055079",
"00055159",
"00055174",
"00055191",
"00055238",
"00055241",
"00055299",
"00055320",
"00055328",
"00055350",
"00055415",
"00055428",
"00055439",
"00055450",
"00055483",
"00055519",
"00055525",
"00055614",
"00055615",
"00055657",
"00055687",
"00055696",
"00055773",
"00055797",
"00055815",
"00055833",
"00055838",
"00055854",
"00055880",
"00056025",
"00056031",
"00056035",
"00056112",
"00056122",
"00056123",
"00056133",
"00056138",
"00056204",
"00056207",
"00056255",
"00056256",
"00056280",
"00056285",
"00056289",
"00056296",
"00056302",
"00056305",
"00056414",
"00056415",
"00056483",
"00056513",
"00056524",
"00056532",
"00056538",
"00056541",
"00056567",
"00056606",
"00056653",
"00056664",
"00056671",
"00056692",
"00056707",
"00056751",
"00056756",
"00056793",
"00056803",
"00056942",
"00056999",
"00057003",
"00057007",
"00057009",
"00057051",
"00057106",
"00057124",
"00057132",
"00057181",
"00057219",
"00057271",
"00057293",
"00057317",
"00057338",
"00057352",
"00057389",
"00057407",
"00057420",
"00057427",
"00057448",
"00057455",
"00057523",
"00057563",
"00057566",
"00057628",
"00057734",
"00057810",
"00057824",
"00057837",
"00057904",
"00057913",
"00057940",
"00057960",
"00057996",
"00058089",
"00058201",
"00058204",
"00058211",
"00058277",
"00058281",
"00058331",
"00058359",
"00058427",
"00058453",
"00058526",
"00058538",
"00058564",
"00058569",
"00058593",
"00058595",
"00058619",
"00058645",
"00058694",
"00058726",
"00058882",
"00058926",
"00059000",
"00059001",
"00059037",
"00059079",
"00059084",
"00059216",
"00059224",
"00059260",
"00059285",
"00059297",
"00059347",
"00059389",
"00059442",
"00059520",
"00059547",
"00059570",
"00059578",
"00059596",
"00059600",
"00059638",
"00059692",
"00059734",
"00059759",
"00059816",
"00059857",
"00059916",
"00059953",
"00059978",
"00060033",
"00060129",
"00060148",
"00060198",
"00060199",
"00060210",
"00060313",
"00060360",
"00060370",
"00060387",
"00060429",
"00060465",
"00060474",
"00060479",
"00060488",
"00060498",
"00060511",
"00060631",
"00060673",
"00060703",
"00060705",
"00060706",
"00060748",
"00060783",
"00060842",
"00060844",
"00060865",
"00060878",
"00060941",
"00060964",
"00060972",
"00060987",
"00060988",
"00061053",
"00061058",
"00061161",
"00061208",
"00061256",
"00061257",
"00061287",
"00061307",
"00061388",
"00061421",
"00061423",
"00061425",
"00061466",
"00061493",
"00061510",
"00061572",
"00061594",
"00061702",
"00061714",
"00061751",
"00061813",
"00061863",
"00061896",
"00061902",
"00061944",
"00062016",
"00062076",
"00062090",
"00062097",
"00062138",
"00062144",
"00062213",
"00062249",
"00062258",
"00062306",
"00062341",
"00062371",
"00062388",
"00062395",
"00062427",
"00062474",
"00062503",
"00062537",
"00062589",
"00062596",
"00062623",
"00062669",
"00062721",
"00062783",
"00062811",
"00062923",
"00062954",
"00062958",
"00062987",
"00063051",
"00063131",
"00063192",
"00063196",
"00063229",
"00063283",
"00063314",
"00063348",
"00063375",
"00063450",
"00063465",
"00063551",
"00063570",
"00063573",
"00063577",
"00063600",
"00063643",
"00063659",
"00063678",
"00063684",
"00063714",
"00063764",
"00063857",
"00063891",
"00063894",
"00064044",
"00064122",
"00064164",
"00064196",
"00064265",
"00064314",
"00064321",
"00064339",
"00064352",
"00064389",
"00064400",
"00064462",
"00064474",
"00064531",
"00064536",
"00064545",
"00064577",
"00064586",
"00064648",
"00064659",
"00064676",
"00064685",
"00064691",
"00064715",
"00064817",
"00064857",
"00064870",
"00064891",
"00064938",
"00064950",
"00064991",
"00064996",
"00065013",
"00065025",
"00065033",
"00065051",
"00065108",
"00065112",
"00065139",
"00065188",
"00065244",
"00065259",
"00065287",
"00065289",
"00065427",
"00065531",
"00065572",
"00065624",
"00065652",
"00065671",
"00065688",
"00065712",
"00065747",
"00065784",
"00065832",
"00065841",
"00065859",
"00065879",
"00065897",
"00065951",
"00066026",
"00066062",
"00066100",
"00066116",
"00066165",
"00066250",
"00066297",
"00066346",
"00066482",
"00066485",
"00066581",
"00066622",
"00066643",
"00066688",
"00066709",
"00066710",
"00066725",
"00066790",
"00066874",
"00067005",
"00067026",
"00067054",
"00067089",
"00067096",
"00067298",
"00067308",
"00067324",
"00067382",
"00067415",
"00067519",
"00067604",
"00067627",
"00067700",
"00067716",
"00067789",
"00067802",
"00067938",
"00067951",
"00068158",
"00068174",
"00068216",
"00068303",
"00068406",
"00068436",
"00068556",
"00068640",
"00068727",
"00068745",
"00068746",
"00068759",
"00068772",
"00068791",
"00068831",
"00068851",
"00068886",
"00068905",
"00068921",
"00068987",
"00069034",
"00069112",
"00069118",
"00069133",
"00069154",
"00069177",
"00069221",
"00069239",
"00069250",
"00069258",
"00069315",
"00069394",
"00069409",
"00069440",
"00069480",
"00069503",
"00069508",
"00069510",
"00069529",
"00069531",
"00069596",
"00069605",
"00069650",
"00069669",
"00069714",
"00069778",
"00069792",
"00069803",
"00069806",
"00069809",
"00069857",
"00069867",
"00069952",
"00070026",
"00070043",
"00070065",
"00070101",
"00070116",
"00070146",
"00070148",
"00070178",
"00070262",
"00070325",
"00070463",
"00070512",
"00070544",
"00070599",
"00070611",
"00070831",
"00070865",
"00070908",
"00070937",
"00070994",
"00071027",
"00071034",
"00071056",
"00071079",
"00071089",
"00071135",
"00071171",
"00071227",
"00071251",
"00071273",
"00071292",
"00071310",
"00071329",
"00071371",
"00071384",
"00071405",
"00071439",
"00071459",
"00071541",
"00071544",
"00071575",
"00071581",
"00071721",
"00071815",
"00071864",
"00071933",
"00071968",
"00072013",
"00072146",
"00072185",
"00072188",
"00072209",
"00072221",
"00072238",
"00072324",
"00072354",
"00072365",
"00072412",
"00072482",
"00072504",
"00072645",
"00072666",
"00072725",
"00072745",
"00072791",
"00072806",
"00072825",
"00072848",
"00072877",
"00072928",
"00072953",
"00072957",
"00073137",
"00073209",
"00073218",
"00073302",
"00073352",
"00073424",
"00073437",
"00073441",
"00073448",
"00073452",
"00073483",
"00073500",
"00073506",
"00073527",
"00073530",
"00073537",
"00073542",
"00073589",
"00073663",
"00073710",
"00073769",
"00073871",
"00073959",
"00073965",
"00074015",
"00074017",
"00074038",
"00074041",
"00074052",
"00074055",
"00074092",
"00074110",
"00074333",
"00074363",
"00074636",
"00074731",
"00074768",
"00074774",
"00074809",
"00074922",
"00075140",
"00075202",
"00075302",
"00075420",
"00075530",
"00075541",
"00075546",
"00075590",
"00075597",
"00075629",
"00075648",
"00075708",
"00075748",
"00075750",
"00075788",
"00075809",
"00075868",
"00075970",
"00076117",
"00076169",
"00076276",
"00076433",
"00076468",
"00076472",
"00076525",
"00076537",
"00076665",
"00076703",
"00076741",
"00076765",
"00076840",
"00076915",
"00076926",
"00076965",
"00077008",
"00077103",
"00077129",
"00077165",
"00077184",
"00077218",
"00077269",
"00077308",
"00077349",
"00077375",
"00077400",
"00077435",
"00077504",
"00077597",
"00077650",
"00077728",
"00077795",
"00077808",
"00077818",
"00077850",
"00077855",
"00077914",
"00077926",
"00077961",
"00078045",
"00078077",
"00078089",
"00078109",
"00078124",
"00078196",
"00078205",
"00078247",
"00078318",
"00078328",
"00078395",
"00078427",
"00078509",
"00078580",
"00078626",
"00078671",
"00078864",
"00078968",
"00078970",
"00079072",
"00079231",
"00079253",
"00079314",
"00079359",
"00079378",
"00079530",
"00079531",
"00079550",
"00079556",
"00079586",
"00079619",
"00079673",
"00079695",
"00079698",
"00079717",
"00079785",
"00079885",
"00079891",
"00079899",
"00079901",
"00079956",
"00080019",
"00080027",
"00080047",
"00080067",
"00080078",
"00080104",
"00080135",
"00080137",
"00080159",
"00080170",
"00080211",
"00080218",
"00080223",
"00080231",
"00080269",
"00080281",
"00080378",
"00080395",
"00080403",
"00080408",
"00080458",
"00080463",
"00080525",
"00080576",
"00080604",
"00080634",
"00080801",
"00080830",
"00080850",
"00080917",
"00080929",
"00080950",
"00080977",
"00081009",
"00081017",
"00081090",
"00081180",
"00081232",
"00081329",
"00081346",
"00081348",
"00081368",
"00081439",
"00081487",
"00081690",
"00081709",
"00081777",
"00081829",
"00081836",
"00081877",
"00081974",
"00082117",
"00082162",
"00082208",
"00082213",
"00082269",
"00082320",
"00082337",
"00082398",
"00082438",
"00082447",
"00082471",
"00082498",
"00082510",
"00082531",
"00082540",
"00082564",
"00082586",
"00082618",
"00082631",
"00082666",
"00082677",
"00082787",
"00082817",
"00082872",
"00082882",
"00082892",
"00083001",
"00083067",
"00083091",
"00083110",
"00083153",
"00083253",
"00083261",
"00083265",
"00083275",
"00083290",
"00083305",
"00083353",
"00083393",
"00083454",
"00083496",
"00083504",
"00083539",
"00083679",
"00083686",
"00083832",
"00083892",
"00083942",
"00083968",
"00083992",
"00084047",
"00084088",
"00084098",
"00084100",
"00084161",
"00084209",
"00084276",
"00084304",
"00084317",
"00084337",
"00084352",
"00084393",
"00084413",
"00084465",
"00084478",
"00084540",
"00084560",
"00084561",
"00084566",
"00084577",
"00084588",
"00084650",
"00084720",
"00084768",
"00084830",
"00084844",
"00084855",
"00084873",
"00085070",
"00085131",
"00085149",
"00085151",
"00085164",
"00085233",
"00085288",
"00085395",
"00085526",
"00085547",
"00085598",
"00085609",
"00085620",
"00085657",
"00085686",
"00085772",
"00085794",
"00085806",
"00085841",
"00085854",
"00085855",
"00085936",
"00086017",
"00086036",
"00086042",
"00086074",
"00086096",
"00086129",
"00086179",
"00086261",
"00086285",
"00086342",
"00086350",
"00086372",
"00086386",
"00086413",
"00086418",
"00086432",
"00086456",
"00086470",
"00086521",
"00086552",
"00086629",
"00086666",
"00086730",
"00086911",
"00086920",
"00086927",
"00086934",
"00086989",
"00087063",
"00087108",
"00087156",
"00087176",
"00087184",
"00087258",
"00087273",
"00087306",
"00087319",
"00087321",
"00087411",
"00087431",
"00087588",
"00087614",
"00087686",
"00087692",
"00087721",
"00087743",
"00087784",
"00087813",
"00087847",
"00087887",
"00087888",
"00087913",
"00087933",
"00087965",
"00088011",
"00088012",
"00088018",
"00088069",
"00088090",
"00088095",
"00088113",
"00088127",
"00088138",
"00088174",
"00088176",
"00088188",
"00088279",
"00088381",
"00088388",
"00088398",
"00088428",
"00088458",
"00088507",
"00088557",
"00088567",
"00088571",
"00088604",
"00088616",
"00088618",
"00088622",
"00088690",
"00088728",
"00088749",
"00088776",
"00088819",
"00088840",
"00088901",
"00088920",
"00088947",
"00088958",
"00089067",
"00089109",
"00089166",
"00089175",
"00089204",
"00089230",
"00089233",
"00089299",
"00089303",
"00089304",
"00089369",
"00089381",
"00089532",
"00089550",
"00089624",
"00089654",
"00089666",
"00089668",
"00089697",
"00089743",
"00089791",
"00089801",
"00089817",
"00089819",
"00089826",
"00089843",
"00089949",
"00090032",
"00090062",
"00090073",
"00090089",
"00090135",
"00090208",
"00090220",
"00090223",
"00090285",
"00090322",
"00090371",
"00090419",
"00090477",
"00090526",
"00090556",
"00090560",
"00090607",
"00090638",
"00090648",
"00090662",
"00090683",
"00090686",
"00090735",
"00090766",
"00090770",
"00090782",
"00090789",
"00090920",
"00090924",
"00090944",
"00091071",
"00091103",
"00091120",
"00091122",
"00091143",
"00091145",
"00091185",
"00091221",
"00091323",
"00091510",
"00091657",
"00091788",
"00091799",
"00091845",
"00091916",
"00091952",
"00091986",
"00092002",
"00092053",
"00092093",
"00092135",
"00092218",
"00092233",
"00092278",
"00092299",
"00092326",
"00092330",
"00092332",
"00092348",
"00092378",
"00092397",
"00092439",
"00092545",
"00092550",
"00092572",
"00092740",
"00092793",
"00092837",
"00092849",
"00092913",
"00092940",
"00092946",
"00093034",
"00093040",
"00093104",
"00093114",
"00093137",
"00093182",
"00093183",
"00093186",
"00093191",
"00093216",
"00093219",
"00093246",
"00093249",
"00093251",
"00093264",
"00093274",
"00093347",
"00093426",
"00093461",
"00093556",
"00093726",
"00093733",
"00093824",
"00093837",
"00093933",
"00094096",
"00094161",
"00094216",
"00094338",
"00094435",
"00094565",
"00094586",
"00094599",
"00094675",
"00094759",
"00094762",
"00095057",
"00095208",
"00095315",
"00095332",
"00095368",
"00095399",
"00095478",
"00095511",
"00095570",
"00095604",
"00095726",
"00095752",
"00095796",
"00095831",
"00095926",
"00096071",
"00096265",
"00096281",
"00096317",
"00096342",
"00096387",
"00096520",
"00096699",
"00096772",
"00096786",
"00096805",
"00096832",
"00096839",
"00096861",
"00096877",
"00096881",
"00096887",
"00096900",
"00097067",
"00097170",
"00097203",
"00097220",
"00097266",
"00097356",
"00097375",
"00097473",
"00097495",
"00097531",
"00097591",
"00097654",
"00097671",
"00097725",
"00097758",
"00097832",
"00097840",
"00097853",
"00097887",
"00097898",
"00097941",
"00097999",
"00098042",
"00098113",
"00098138",
"00098150",
"00098273",
"00098302",
"00098316",
"00098397",
"00098450",
"00098469",
"00098491",
"00098492",
"00098546",
"00098595",
"00098611",
"00098659",
"00098683",
"00098691",
"00098695",
"00098728",
"00098739",
"00098760",
"00098764",
"00098786",
"00098795",
"00098799",
"00098815",
"00098819",
"00098876",
"00098938",
"00098957",
"00098968",
"00099053",
"00099082",
"00099110",
"00099124",
"00099149",
"00099233",
"00099251",
"00099308",
"00099403",
"00099501",
"00099522",
"00099553",
"00099584",
"00099586",
"00099606",
"00099623",
"00099628",
"00099711",
"00099846",
"00099926",
"00099982",
"00100001",
"00100004",
"00100019",
"00100260",
"00100315",
"00100326",
"00100409",
"00100432",
"00100473",
"00100525",
"00100535",
"00100549",
"00100560",
"00100628",
"00100668",
"00100734",
"00100747",
"00100760",
"00100769",
"00100794",
"00100886",
"00100915",
"00101033",
"00101143",
"00101200",
"00101218",
"00101223",
"00101239",
"00101267",
"00101269",
"00101275",
"00101276",
"00101344",
"00101394",
"00101406",
"00101411",
"00101455",
"00101489",
"00101495",
"00101503",
"00101514",
"00101683",
"00101696",
"00101780",
"00101855",
"00101893",
"00101934",
"00101955",
"00101980",
"00102102",
"00102112",
"00102113",
"00102147",
"00102171",
"00102185",
"00102205",
"00102209",
"00102288",
"00102292",
"00102299",
"00102354",
"00102439",
"00102463",
"00102473",
"00102512",
"00102547",
"00102559",
"00102618",
"00102682",
"00102687",
"00102720",
"00102733",
"00102757",
"00102810",
"00102853",
"00102897",
"00102903",
"00102906",
"00102915",
"00102940",
"00103040",
"00103087",
"00103113",
"00103146",
"00103154",
"00103175",
"00103195",
"00103233",
"00103234",
"00103256",
"00103275",
"00103284",
"00103328",
"00103462",
"00103510",
"00103567",
"00103648",
"00103650",
"00103727",
"00103791",
"00103829",
"00103831",
"00103875",
"00103919",
"00104053",
"00104061",
"00104068",
"00104118",
"00104127",
"00104178",
"00104233",
"00104285",
"00104294",
"00104341",
"00104387",
"00104417",
"00104432",
"00104539",
"00104575",
"00104584",
"00104587",
"00104605",
"00104608",
"00104698",
"00104733",
"00104766",
"00104768",
"00104796",
"00104893",
"00104899",
"00104923",
"00104939",
"00104994",
"00105049",
"00105113",
"00105180",
"00105203",
"00105231",
"00105248",
"00105271",
"00105284",
"00105364",
"00105415",
"00105559",
"00105584",
"00105619",
"00105647",
"00105725",
"00105811",
"00105825",
"00105893",
"00106055",
"00106076",
"00106078",
"00106092",
"00106105",
"00106130",
"00106166",
"00106208",
"00106341",
"00106439",
"00106471",
"00106510",
"00106529",
"00106535",
"00106651",
"00106691",
"00106897",
"00106902",
"00106941",
"00106976",
"00106995",
"00107000",
"00107087",
"00107094",
"00107149",
"00107155",
"00107157",
"00107167",
"00107214",
"00107241",
"00107251",
"00107257",
"00107279",
"00107314",
"00107316",
"00107351",
"00107387",
"00107412",
"00107420",
"00107443",
"00107463",
"00107485",
"00107523",
"00107544",
"00107547",
"00107647",
"00107656",
"00107713",
"00107715",
"00107720",
"00107761",
"00107769",
"00107785",
"00107811",
"00107849",
"00107867",
"00107883",
"00107943",
"00108006",
"00108011",
"00108034",
"00108217",
"00108220",
"00108322",
"00108327",
"00108439",
"00108489",
"00108496",
"00108640",
"00108725",
"00108741",
"00108758",
"00108812",
"00108925",
"00108967",
"00108969",
"00109079",
"00109249",
"00109262",
"00109303",
"00109394",
"00109457",
"00109459",
"00109497",
"00109536",
"00109654",
"00109676",
"00109681",
"00109717",
"00109745",
"00109810",
"00109827",
"00109855",
"00109914",
"00109935",
"00109977",
"00110008",
"00110058",
"00110131",
"00110165",
"00110194",
"00110266",
"00110295",
"00110308",
"00110332",
"00110346",
"00110362",
"00110388",
"00110430",
"00110528",
"00110560",
"00110578",
"00110596",
"00110597",
"00110607",
"00110617",
"00110657",
"00110658",
"00110678",
"00110706",
"00110757",
"00110883",
"00110894",
"00110917",
"00110944",
"00110950",
"00110982",
"00111030",
"00111051",
"00111063",
"00111071",
"00111084",
"00111086",
"00111135",
"00111157",
"00111164",
"00111194",
"00111269",
"00111276",
"00111328",
"00111393",
"00111421",
"00111425",
"00111526",
"00111551",
"00111562",
"00111649",
"00111742",
"00111828",
"00111840",
"00111842",
"00111868",
"00111899",
"00111909",
"00111942",
"00112004",
"00112099",
"00112166",
"00112181",
"00112199",
"00112228",
"00112229",
"00112234",
"00112284",
"00112316",
"00112391",
"00112500",
"00112525",
"00112540",
"00112592",
"00112593",
"00112673",
"00112703",
"00112750",
"00112793",
"00112810",
"00112819",
"00112826",
"00112898",
"00112915",
"00112977",
"00112983",
"00113063",
"00113066",
"00113087",
"00113090",
"00113136",
"00113240",
"00113254",
"00113255",
"00113271",
"00113309",
"00113318",
"00113324",
"00113332",
"00113391",
"00113456",
"00113478",
"00113489",
"00113521",
"00113556",
"00113581",
"00113631",
"00113674",
"00113721",
"00113726",
"00113742",
"00113745",
"00113775",
"00113809",
"00113867",
"00113871",
"00113897",
"00113959",
"00114001",
"00114063",
"00114079",
"00114148",
"00114155",
"00114174",
"00114181",
"00114215",
"00114232",
"00114252",
"00114283",
"00114322",
"00114356",
"00114423",
"00114425",
"00114530",
"00114654",
"00114724",
"00114729",
"00114754",
"00114820",
"00114855",
"00114863",
"00114867",
"00114957",
"00114962",
"00114964",
"00114966",
"00114989",
"00114998",
"00115007",
"00115048",
"00115076",
"00115107",
"00115126",
"00115133",
"00115151",
"00115158",
"00115166",
"00115191",
"00115206",
"00115301",
"00115434",
"00115469",
"00115603",
"00115609",
"00115612",
"00115647",
"00115675",
"00115679",
"00115700",
"00115716",
"00115724",
"00115727",
"00115856",
"00115865",
"00115900",
"00115970",
"00116021",
"00116070",
"00116107",
"00116123",
"00116185",
"00116248",
"00116586",
"00116596",
"00116600",
"00116611",
"00116627",
"00116673",
"00116685",
"00116751",
"00116857",
"00117032",
"00117115",
"00117130",
"00117168",
"00117192",
"00117259",
"00117343",
"00117401",
"00117480",
"00117518",
"00117553",
"00117667",
"00117712",
"00117748",
"00117770",
"00117777",
"00117805",
"00117836",
"00118025",
"00118123",
"00118210",
"00118263",
"00118293",
"00118336",
"00118388",
"00118409",
"00118459",
"00118599",
"00118643",
"00118646",
"00118675",
"00118710",
"00118742",
"00118903",
"00119081",
"00119085",
"00119094",
"00119128",
"00119147",
"00119172",
"00119208",
"00119211",
"00119217",
"00119227",
"00119234",
"00119255",
"00119279",
"00119291",
"00119292",
"00119341",
"00119351",
"00119367",
"00119377",
"00119418",
"00119438",
"00119451",
"00119496",
"00119582",
"00119613",
"00119615",
"00119640",
"00119773",
"00119809",
"00119911",
"00119929",
"00119987",
"00120038",
"00120121",
"00120125",
"00120127",
"00120155",
"00120194",
"00120195",
"00120320",
"00120374",
"00120428",
"00120469",
"00120534",
"00120546",
"00120587",
"00120598",
"00120632",
"00120732",
"00120736",
"00120816",
"00120828",
"00120835",
"00120841",
"00120868",
"00120993",
"00121123",
"00121165",
"00121214",
"00121241",
"00121295",
"00121298",
"00121304",
"00121364",
"00121419",
"00121438",
"00121533",
"00121562",
"00121570",
"00121575",
"00121693",
"00121850",
"00121853",
"00121874",
"00121877",
"00121903",
"00121978",
"00122026",
"00122138",
"00122154",
"00122197",
"00122216",
"00122229",
"00122249",
"00122258",
"00122315",
"00122330",
"00122372",
"00122379",
"00122396",
"00122429",
"00122673",
"00122677",
"00122739",
"00122780",
"00122801",
"00122872",
"00122918",
"00122930",
"00122943",
"00123020",
"00123028",
"00123030",
"00123080",
"00123114",
"00123182",
"00123187",
"00123188",
"00123223",
"00123269",
"00123307",
"00123328",
"00123363",
"00123394",
"00123412",
"00123442",
"00123460",
"00123522",
"00123700",
"00123741",
"00123778",
"00123807",
"00123869",
"00123880",
"00123903",
"00124099",
"00124188",
"00124198",
"00124214",
"00124239",
"00124244",
"00124293",
"00124302",
"00124343",
"00124368",
"00124440",
"00124496",
"00124514",
"00124596",
"00124681",
"00124727",
"00124825",
"00124878",
"00124900",
"00124915",
"00124919",
"00124936",
"00124945",
"00125028",
"00125047",
"00125131",
"00125156",
"00125190",
"00125191",
"00125193",
"00125257",
"00125262",
"00125297",
"00125324",
"00125326",
"00125330",
"00125373",
"00125380",
"00125393",
"00125460",
"00125502",
"00125582",
"00125583",
"00125585",
"00125686",
"00125699",
"00125738",
"00125854",
"00125903",
"00125937",
"00125940",
"00126002",
"00126042",
"00126062",
"00126170",
"00126218",
"00126300",
"00126313",
"00126352",
"00126390",
"00126404",
"00126488",
"00126506",
"00126530",
"00126563",
"00126570",
"00126576",
"00126603",
"00126643",
"00126688",
"00126691",
"00126772",
"00126822",
"00126862",
"00126877",
"00126889",
"00126911",
"00126930",
"00126961",
"00126963",
"00126993",
"00127053",
"00127113",
"00127137",
"00127145",
"00127152",
"00127165",
"00127204",
"00127244",
"00127305",
"00127314",
"00127388",
"00127473",
"00127503",
"00127528",
"00127579",
"00127624",
"00127647",
"00127690",
"00127729",
"00127766",
"00127789",
"00127790",
"00127817",
"00127826",
"00127900",
"00127925",
"00127928",
"00127935",
"00127999",
"00128045",
"00128079",
"00128098",
"00128148",
"00128200",
"00128257",
"00128293",
"00128306",
"00128325",
"00128354",
"00128453",
"00128455",
"00128563",
"00128586",
"00128659",
"00128733",
"00128754",
"00128823",
"00128881",
"00128923",
"00129015",
"00129031",
"00129053",
"00129119",
"00129284",
"00129464",
"00129533",
"00129579",
"00129595",
"00129662",
"00129711",
"00129826",
"00129854",
"00130100",
"00130222",
"00130250",
"00130267",
"00130275",
"00130426",
"00130431",
"00130483",
"00130519",
"00130677",
"00130695",
"00130697",
"00130783",
"00130802",
"00131032",
"00131039",
"00131074",
"00131095",
"00131103",
"00131105",
"00131168",
"00131196",
"00131203",
"00131232",
"00131279",
"00131423",
"00131436",
"00131568",
"00131612",
"00131617",
"00131621",
"00131662",
"00131666",
"00131706",
"00131723",
"00131771",
"00131782",
"00131840",
"00131850",
"00131897",
"00132002",
"00132035",
"00132045",
"00132049",
"00132123",
"00132130",
"00132131",
"00132158",
"00132263",
"00132301",
"00132373",
"00132435",
"00132460",
"00132503",
"00132529",
"00132530",
"00132550",
"00132723",
"00132724",
"00132877",
"00132894",
"00132897",
"00132978",
"00132987",
"00133025",
"00133063",
"00133072",
"00133095",
"00133096",
"00133119",
"00133134",
"00133234",
"00133250",
"00133366",
"00133507",
"00133528",
"00133571",
"00133574",
"00133587",
"00133648",
"00133669",
"00133721",
"00133723",
"00133772",
"00133791",
"00133803",
"00133811",
"00133819",
"00133839",
"00133852",
"00133885",
"00133932",
"00133943",
"00133966",
"00133990",
"00134013",
"00134033",
"00134059",
"00134094",
"00134188",
"00134213",
"00134253",
"00134407",
"00134458",
"00134507",
"00134515",
"00134565",
"00134611",
"00134662",
"00134786",
"00134798",
"00134799",
"00134818",
"00134827",
"00134840",
"00134854",
"00134862",
"00134905",
"00134941",
"00134976",
"00134990",
"00135010",
"00135064",
"00135091",
"00135096",
"00135115",
"00135116",
"00135171",
"00135181",
"00135185",
"00135234",
"00135301",
"00135365",
"00135382",
"00135402",
"00135419",
"00135427",
"00135528",
"00135582",
"00135597",
"00135649",
"00135677",
"00135678",
"00135685",
"00135716",
"00135816",
"00135840",
"00135861",
"00135864",
"00135867",
"00135914",
"00135918",
"00136046",
"00136183",
"00136239",
"00136250",
"00136281",
"00136292",
"00136336",
"00136342",
"00136352",
"00136403",
"00136450",
"00136466",
"00136500",
"00136514",
"00136526",
"00136554",
"00136668",
"00136691",
"00136771",
"00136783",
"00136785",
"00136859",
"00136996",
"00137031",
"00137043",
"00137052",
"00137055",
"00137115",
"00137167",
"00137173",
"00137178",
"00137214",
"00137219",
"00137293",
"00137356",
"00137358",
"00137430",
"00137478",
"00137514",
"00137542",
"00137543",
"00137549",
"00137682",
"00137740",
"00137765",
"00137795",
"00137798",
"00137820",
"00137863",
"00137873",
"00137891",
"00137900",
"00137901",
"00137923",
"00137939",
"00137967",
"00137981",
"00138014",
"00138114",
"00138128",
"00138193",
"00138197",
"00138207",
"00138258",
"00138284",
"00138302",
"00138324",
"00138409",
"00138433",
"00138546",
"00138549",
"00138560",
"00138564",
"00138576",
"00138591",
"00138666",
"00138668",
"00138722",
"00138786",
"00138806",
"00138920",
"00138936",
"00138944",
"00138962",
"00138971",
"00139055",
"00139095",
"00139120",
"00139129",
"00139139",
"00139206",
"00139275",
"00139300",
"00139465",
"00139539",
"00139543",
"00139573",
"00139612",
"00139641",
"00139685",
"00139770",
"00139857",
"00139930",
"00139931",
"00139944",
"00139963",
"00140007",
"00140091",
"00140239",
"00140264",
"00140576",
"00140617",
"00140699",
"00140885",
"00140887",
"00141001",
"00141077",
"00141116",
"00141143",
"00141191",
"00141206",
"00141244",
"00141256",
"00141365",
"00141407",
"00141416",
"00141519",
"00141535",
"00141637",
"00141665",
"00141676",
"00141751",
"00141757",
"00141761",
"00141789",
"00141861",
"00141921",
"00141997",
"00142019",
"00142076",
"00142271",
"00142291",
"00142326",
"00142359",
"00142374",
"00142493",
"00142643",
"00142652",
"00142667",
"00142683",
"00142692",
"00142697",
"00142717",
"00142751",
"00142769",
"00142773",
"00142780",
"00142838",
"00142899",
"00142923",
"00142925",
"00142930",
"00142952",
"00143050",
"00143070",
"00143170",
"00143176",
"00143178",
"00143261",
"00143294",
"00143353",
"00143386",
"00143450",
"00143461",
"00143543",
"00143667",
"00143669",
"00143730",
"00143731",
"00143748",
"00143764",
"00143836",
"00143884",
"00143912",
"00143930",
"00143977",
"00144004",
"00144131",
"00144235",
"00144258",
"00144292",
"00144357",
"00144365",
"00144557",
"00144614",
"00144630",
"00144666",
"00144787",
"00144822",
"00144864",
"00144866",
"00144901",
"00144917",
"00145021",
"00145048",
"00145065",
"00145066",
"00145080",
"00145163",
"00145186",
"00145214",
"00145237",
"00145245",
"00145312",
"00145351",
"00145379",
"00145392",
"00145418",
"00145454",
"00145471",
"00145487",
"00145537",
"00145552",
"00145574",
"00145586",
"00145595",
"00145625",
"00145680",
"00145682",
"00145736",
"00145771",
"00145866",
"00145883",
"00145915",
"00145932",
"00145946",
"00145975",
"00145977",
"00146037",
"00146048",
"00146092",
"00146107",
"00146149",
"00146175",
"00146183",
"00146209",
"00146349",
"00146371",
"00146377",
"00146390",
"00146392",
"00146450",
"00146453",
"00146458",
"00146529",
"00146570",
"00146578",
"00146594",
"00146801",
"00146837",
"00146933",
"00146945",
"00146994",
"00147115",
"00147128",
"00147140",
"00147172",
"00147328",
"00147347",
"00147377",
"00147398",
"00147408",
"00147472",
"00147523",
"00147533",
"00147562",
"00147591",
"00147620",
"00147656",
"00147664",
"00147676",
"00147732",
"00147734",
"00147891",
"00147953",
"00148198",
"00148228",
"00148239",
"00148275",
"00148318",
"00148332",
"00148384",
"00148403",
"00148428",
"00148453",
"00148471",
"00148486",
"00148506",
"00148527",
"00148601",
"00148675",
"00148693",
"00148741",
"00148828",
"00148850",
"00148858",
"00148963",
"00148981",
"00149013",
"00149023",
"00149045",
"00149049",
"00149120",
"00149162",
"00149167",
"00149175",
"00149184",
"00149220",
"00149248",
"00149273",
"00149285",
"00149290",
"00149367",
"00149442",
"00149472",
"00149474",
"00149553",
"00149557",
"00149694",
"00149732",
"00149777",
"00149792",
"00149893",
"00149899",
"00149904",
"00149964",
"00150014",
"00150017",
"00150060",
"00150144",
"00150167",
"00150169",
"00150173",
"00150213",
"00150216",
"00150303",
"00150317",
"00150329",
"00150337",
"00150340",
"00150379",
"00150391",
"00150393",
"00150591",
"00150596",
"00150614",
"00150670",
"00150735",
"00150750",
"00150833",
"00150880",
"00150911",
"00150975",
"00151006",
"00151044",
"00151061",
"00151091",
"00151136",
"00151205",
"00151323",
"00151421",
"00151426",
"00151445",
"00151469",
"00151475",
"00151485",
"00151492",
"00151536",
"00151546",
"00151571",
"00151577",
"00151606",
"00151688",
"00151729",
"00151763",
"00151808",
"00151918",
"00151939",
"00151978",
"00152068",
"00152085",
"00152114",
"00152121",
"00152178",
"00152214",
"00152344",
"00152468",
"00152494",
"00152514",
"00152650",
"00152748",
"00152792",
"00152979",
"00152996",
"00153003",
"00153014",
"00153018",
"00153050",
"00153055",
"00153125",
"00153126",
"00153166",
"00153196",
"00153207",
"00153247",
"00153274",
"00153326",
"00153357",
"00153381",
"00153411",
"00153423",
"00153436",
"00153468",
"00153470",
"00153475",
"00153492",
"00153561",
"00153600",
"00153668",
"00153682",
"00153694",
"00153724",
"00153732",
"00153743",
"00153746",
"00153767",
"00153802",
"00153906",
"00153916",
"00153953",
"00154003",
"00154052",
"00154118",
"00154138",
"00154145",
"00154272",
"00154291",
"00154389",
"00154407",
"00154483",
"00154539",
"00154565",
"00154600",
"00154684",
"00154829",
"00154851",
"00154860",
"00154965",
"00154979",
"00155010",
"00155042",
"00155048",
"00155070",
"00155141",
"00155153",
"00155168",
"00155224",
"00155335",
"00155372",
"00155391",
"00155517",
"00155519",
"00155593",
"00155611",
"00155665",
"00155670",
"00155738",
"00155783",
"00155815",
"00155838",
"00155884",
"00155892",
"00155907",
"00155944",
"00155945",
"00155948",
"00156003",
"00156121",
"00156150",
"00156168",
"00156186",
"00156229",
"00156284",
"00156321",
"00156328",
"00156413",
"00156491",
"00156524",
"00156534",
"00156538",
"00156577",
"00156699",
"00156714",
"00156759",
"00156766",
"00156782",
"00156834",
"00156867",
"00156873",
"00157012",
"00157028",
"00157039",
"00157075",
"00157091",
"00157217",
"00157222",
"00157253",
"00157308",
"00157359",
"00157366",
"00157483",
"00157489",
"00157494",
"00157633",
"00157634",
"00157647",
"00157648",
"00157691",
"00157736",
"00157885",
"00157920",
"00157960",
"00158036",
"00158046",
"00158066",
"00158103",
"00158109",
"00158254",
"00158257",
"00158423",
"00158468",
"00158523",
"00158570",
"00158597",
"00158665",
"00158714",
"00158727",
"00158745",
"00158822",
"00158829",
"00158841",
"00158886",
"00158890",
"00158891",
"00158900",
"00158931",
"00158956",
"00159009",
"00159020",
"00159030",
"00159087",
"00159133",
"00159200",
"00159242",
"00159256",
"00159272",
"00159280",
"00159425",
"00159470",
"00159549",
"00159563",
"00159586",
"00159662",
"00159747",
"00159748",
"00159770",
"00159786",
"00159816",
"00159853",
"00159951",
"00159966",
"00160014",
"00160076",
"00160107",
"00160109",
"00160124",
"00160220",
"00160226",
"00160338",
"00160444",
"00160474",
"00160508",
"00160573",
"00160592",
"00160679",
"00160717",
"00160783",
"00160794",
"00160911",
"00160921",
"00160976",
"00161101",
"00161104",
"00161109",
"00161124",
"00161182",
"00161186",
"00161214",
"00161257",
"00161269",
"00161287",
"00161333",
"00161348",
"00161398",
"00161409",
"00161414",
"00161415",
"00161439",
"00161486",
"00161683",
"00161725",
"00161759",
"00161785",
"00161867",
"00161873",
"00161877",
"00161908",
"00161972",
"00161989",
"00162000",
"00162005",
"00162047",
"00162164",
"00162177",
"00162228",
"00162278",
"00162336",
"00162362",
"00162441",
"00162546",
"00162549",
"00162551",
"00162580",
"00162878",
"00162891",
"00162943",
"00162990",
"00163108",
"00163173",
"00163186",
"00163246",
"00163271",
"00163273",
"00163301",
"00163326",
"00163370",
"00163437",
"00163479",
"00163582",
"00163619",
"00163627",
"00163694",
"00163702",
"00163925",
"00163937",
"00163967",
"00164064",
"00164072",
"00164115",
"00164144",
"00164158",
"00164160",
"00164174",
"00164177",
"00164231",
"00164263",
"00164297",
"00164310",
"00164462",
"00164517",
"00164528",
"00164565",
"00164573",
"00164627",
"00164696",
"00164777",
"00164800",
"00164825",
"00164891",
"00164976",
"00165114",
"00165143",
"00165211",
"00165288",
"00165361",
"00165413",
"00165476",
"00165505",
"00165525",
"00165536",
"00165545",
"00165570",
"00165572",
"00165596",
"00165598",
"00165643",
"00165659",
"00165660",
"00165716",
"00165762",
"00165804",
"00165847",
"00165860",
"00165871",
"00165941",
"00165952",
"00166012",
"00166015",
"00166024",
"00166027",
"00166055",
"00166067",
"00166072",
"00166080",
"00166150",
"00166240",
"00166241",
"00166372",
"00166386",
"00166442",
"00166543",
"00166554",
"00166582",
"00166721",
"00166734",
"00166737",
"00166827",
"00166832",
"00166887",
"00166920",
"00166942",
"00166981",
"00166996",
"00167051",
"00167085",
"00167096",
"00167110",
"00167115",
"00167128",
"00167144",
"00167243",
"00167299",
"00167315",
"00167316",
"00167321",
"00167373",
"00167406",
"00167417",
"00167480",
"00167493",
"00167511",
"00167553",
"00167561",
"00167771",
"00167875",
"00167967",
"00167990",
"00168000",
"00168030",
"00168182",
"00168201",
"00168239",
"00168318",
"00168321",
"00168403",
"00168404",
"00168443",
"00168447",
"00168470",
"00168544",
"00168610",
"00168633",
"00168637",
"00168716",
"00168752",
"00168757",
"00168760",
"00168821",
"00168833",
"00168863",
"00168887",
"00168938",
"00168983",
"00169023",
"00169041",
"00169061",
"00169092",
"00169108",
"00169158",
"00169178",
"00169209",
"00169219",
"00169223",
"00169250",
"00169309",
"00169336",
"00169365",
"00169371",
"00169379",
"00169401",
"00169491",
"00169513",
"00169543",
"00169564",
"00169565",
"00169583",
"00169593",
"00169680",
"00169697",
"00169708",
"00169719",
"00169720",
"00169724",
"00169799",
"00169839",
"00169880",
"00169890",
"00169946",
"00169962",
"00169968",
"00169980",
"00170031",
"00170064",
"00170069",
"00170085",
"00170103",
"00170189",
"00170272",
"00170287",
"00170352",
"00170409",
"00170492",
"00170588",
"00170591",
"00170667",
"00170686",
"00170716",
"00170718",
"00170766",
"00170775",
"00170782",
"00170912",
"00170941",
"00170953",
"00170967",
"00170976",
"00171023",
"00171062",
"00171086",
"00171129",
"00171144",
"00171150",
"00171168",
"00171184",
"00171195",
"00171313",
"00171327",
"00171328",
"00171406",
"00171413",
"00171508",
"00171514",
"00171529",
"00171567",
"00171672",
"00171726",
"00171774",
"00171776",
"00171803",
"00171806",
"00171813",
"00171904",
"00171947",
"00171960",
"00171966",
"00171977",
"00171978",
"00172011",
"00172017",
"00172039",
"00172146",
"00172168",
"00172170",
"00172245",
"00172249",
"00172334",
"00172335",
"00172423",
"00172456",
"00172478",
"00172528",
"00172582",
"00172586",
"00172634",
"00172670",
"00172706",
"00172712",
"00172723",
"00172735",
"00172801",
"00172859",
"00172921",
"00172938",
"00172962",
"00173002",
"00173003",
"00173124",
"00173136",
"00173159",
"00173186",
"00173275",
"00173290",
"00173396",
"00173489",
"00173500",
"00173551",
"00173606",
"00173611",
"00173649",
"00173783",
"00173793",
"00173820",
"00173829",
"00173840",
"00173857",
"00173881",
"00173954",
"00173993",
"00174126",
"00174190",
"00174197",
"00174212",
"00174225",
"00174319",
"00174407",
"00174452",
"00174500",
"00174592",
"00174601",
"00174628",
"00174647",
"00174685",
"00174698",
"00174758",
"00174809",
"00174856",
"00174885",
"00174970",
"00175012",
"00175111",
"00175119",
"00175142",
"00175159",
"00175160",
"00175182",
"00175210",
"00175274",
"00175310",
"00175317",
"00175318",
"00175378",
"00175566",
"00175587",
"00175619",
"00175649",
"00175684",
"00175732",
"00175775",
"00175782",
"00175852",
"00175863",
"00175910",
"00176084",
"00176087",
"00176191",
"00176247",
"00176251",
"00176275",
"00176277",
"00176279",
"00176372",
"00176381",
"00176400",
"00176443",
"00176467",
"00176535",
"00176543",
"00176579",
"00176626",
"00176629",
"00176649",
"00176682",
"00176699",
"00176711",
"00176717",
"00176724",
"00176816",
"00176826",
"00176850",
"00176884",
"00176899",
"00176931",
"00177179",
"00177277",
"00177280",
"00177400",
"00177423",
"00177430",
"00177432",
"00177436",
"00177439",
"00177483",
"00177583",
"00177641",
"00177648",
"00177693",
"00177785",
"00177828",
"00177843",
"00177871",
"00177872",
"00177903",
"00177909",
"00177940",
"00178022",
"00178024",
"00178108",
"00178276",
"00178342",
"00178355",
"00178376",
"00178392",
"00178407",
"00178429",
"00178466",
"00178535",
"00178670",
"00178674",
"00178725",
"00178730",
"00178735",
"00178768",
"00178816",
"00178846",
"00178879",
"00178889",
"00178895",
"00178975",
"00178985",
"00179004",
"00179128",
"00179184",
"00179218",
"00179227",
"00179250",
"00179280",
"00179334",
"00179336",
"00179340",
"00179351",
"00179354",
"00179398",
"00179415",
"00179471",
"00179501",
"00179568",
"00179597",
"00179603",
"00179616",
"00179664",
"00179765",
"00179824",
"00179838",
"00179841",
"00179845",
"00179882",
"00179887",
"00179891",
"00179893",
"00179946",
"00179953",
"00179988",
"00179996",
"00180085",
"00180153",
"00180204",
"00180244",
"00180299",
"00180332",
"00180407",
"00180424",
"00180432",
"00180440",
"00180502",
"00180506",
"00180545",
"00180547",
"00180575",
"00180596",
"00180624",
"00180669",
"00180765",
"00180787",
"00180847",
"00180922",
"00180937",
"00180946",
"00180962",
"00180963",
"00180975",
"00180982",
"00181058",
"00181087",
"00181267",
"00181322",
"00181396",
"00181432",
"00181435",
"00181447",
"00181457",
"00181494",
"00181573",
"00181576",
"00181578",
"00181582",
"00181595",
"00181663",
"00181696",
"00181738",
"00181771",
"00181783",
"00181826",
"00181832",
"00181866",
"00181882",
"00182006",
"00182037",
"00182102",
"00182103",
"00182119",
"00182236",
"00182266",
"00182330",
"00182335",
"00182344",
"00182350",
"00182379",
"00182405",
"00182416",
"00182479",
"00182485",
"00182505",
"00182622",
"00182761",
"00182773",
"00182794",
"00182819",
"00182919",
"00182957",
"00182979",
"00182982",
"00183061",
"00183065",
"00183080",
"00183096",
"00183123",
"00183179",
"00183214",
"00183215",
"00183217",
"00183224",
"00183228",
"00183276",
"00183331",
"00183364",
"00183369",
"00183455",
"00183480",
"00183514",
"00183516",
"00183532",
"00183592",
"00183638",
"00183662",
"00183742",
"00183753",
"00183755",
"00183834",
"00183842",
"00183883",
"00183908",
"00183960",
"00183969",
"00183983",
"00184003",
"00184012",
"00184074",
"00184127",
"00184159",
"00184163",
"00184166",
"00184184",
"00184198",
"00184210",
"00184240",
"00184246",
"00184290",
"00184302",
"00184325",
"00184333",
"00184394",
"00184420",
"00184431",
"00184579",
"00184614",
"00184620",
"00184704",
"00184853",
"00184894",
"00184900",
"00184914",
"00184924",
"00185020",
"00185064",
"00185076",
"00185101",
"00185117",
"00185119",
"00185159",
"00185203",
"00185339",
"00185433",
"00185462",
"00185532",
"00185597",
"00185641",
"00185760",
"00185766",
"00185807",
"00185832",
"00185975",
"00186022",
"00186086",
"00186235",
"00186236",
"00186272",
"00186351",
"00186354",
"00186375",
"00186444",
"00186458",
"00186481",
"00186525",
"00186532",
"00186596",
"00186674",
"00186714",
"00186846",
"00186943",
"00186960",
"00187013",
"00187051",
"00187121",
"00187146",
"00187147",
"00187187",
"00187200",
"00187223",
"00187235",
"00187243",
"00187247",
"00187281",
"00187282",
"00187308",
"00187314",
"00187353",
"00187399",
"00187455",
"00187496",
"00187518",
"00187677",
"00187684",
"00187686",
"00187732",
"00187757",
"00187779",
"00187792",
"00187803",
"00187882",
"00187927",
"00187949",
"00188013",
"00188029",
"00188049",
"00188051",
"00188074",
"00188078",
"00188234",
"00188281",
"00188285",
"00188335",
"00188360",
"00188423",
"00188468",
"00188533",
"00188540",
"00188585",
"00188669",
"00188840",
"00188913",
"00189238",
"00189291",
"00189305",
"00189325",
"00189352",
"00189367",
"00189398",
"00189476",
"00189505",
"00189631",
"00189650",
"00189764",
"00189809",
"00189841",
"00189972",
"00189987",
"00189996",
"00190038",
"00190081",
"00190092",
"00190102",
"00190104",
"00190212",
"00190221",
"00190291",
"00190299",
"00190418",
"00190425",
"00190487",
"00190496",
"00190518",
"00190555",
"00190571",
"00190585",
"00190633",
"00190654",
"00190767",
"00190790",
"00190851",
"00191092",
"00191098",
"00191104",
"00191144",
"00191260",
"00191289",
"00191434",
"00191446",
"00191461",
"00191466",
"00191478",
"00191483",
"00191500",
"00191512",
"00191527",
"00191548",
"00191586",
"00191591",
"00191598",
"00191599",
"00191772",
"00191780",
"00191787",
"00192005",
"00192036",
"00192057",
"00192158",
"00192176",
"00192280",
"00192299",
"00192308",
"00192322",
"00192388",
"00192396",
"00192418",
"00192427",
"00192518",
"00192522",
"00192527",
"00192533",
"00192541",
"00192582",
"00192604",
"00192615",
"00192684",
"00192703",
"00192728",
"00192759",
"00192760",
"00192783",
"00192852",
"00192860",
"00192908",
"00193107",
"00193145",
"00193149",
"00193186",
"00193194",
"00193310",
"00193339",
"00193348",
"00193415",
"00193428",
"00193432",
"00193503",
"00193533",
"00193551",
"00193594",
"00193608",
"00193633",
"00193681",
"00193707",
"00193713",
"00193718",
"00193788",
"00193793",
"00193822",
"00193863",
"00193875",
"00193904",
"00194014",
"00194086",
"00194143",
"00194312",
"00194315",
"00194504",
"00194520",
"00194547",
"00194625",
"00194682",
"00194715",
"00194819",
"00194842",
"00194855",
"00194868",
"00194916",
"00194985",
"00195009",
"00195026",
"00195030",
"00195125",
"00195132",
"00195142",
"00195176",
"00195205",
"00195215",
"00195274",
"00195361",
"00195363",
"00195386",
"00195459",
"00195460",
"00195470",
"00195495",
"00195540",
"00195541",
"00195557",
"00195596",
"00195646",
"00195682",
"00195812",
"00195824",
"00195906",
"00195941",
"00195970",
"00196100",
"00196102",
"00196175",
"00196235",
"00196236",
"00196246",
"00196262",
"00196292",
"00196377",
"00196385",
"00196435",
"00196453",
"00196540",
"00196642",
"00196644",
"00196676",
"00196800",
"00196802",
"00196813",
"00196814",
"00196857",
"00196873",
"00196888",
"00196911",
"00197009",
"00197010",
"00197063",
"00197084",
"00197143",
"00197264",
"00197353",
"00197367",
"00197490",
"00197514",
"00197613",
"00197631",
"00197645",
"00197663",
"00197678",
"00197692",
"00197758",
"00197859",
"00197863",
"00197876",
"00197942",
"00197968",
"00197992",
"00198034",
"00198164",
"00198200",
"00198300",
"00198332",
"00198350",
"00198367",
"00198467",
"00198562",
"00198604",
"00198629",
"00198684",
"00198749",
"00198777",
"00198817",
"00198829",
"00198839",
"00198930",
"00198987",
"00199136",
"00199175",
"00199239",
"00199275",
"00199422",
"00199461",
"00199522",
"00199544",
"00199630",
"00199672",
"00199681",
"00199808",
"00199831",
"00199890",
"00199979",
"00200094",
"00200141",
"00200178",
"00200227",
"00200320",
"00200424",
"00200455",
"00200569",
"00200571",
"00200578",
"00200674",
"00200728",
"00200773",
"00200778",
"00200875",
"00200888",
"00200899",
"00200911",
"00200976",
"00201001",
"00201129",
"00201157",
"00201184",
"00201192",
"00201194",
"00201275",
"00201324",
"00201383",
"00201384",
"00201500",
"00201551",
"00201631",
"00201687",
"00201693",
"00201781",
"00201805",
"00201969",
"00201990",
"00202015",
"00202120",
"00202270",
"00202326",
"00202358",
"00202424",
"00202540",
"00202553",
"00202584",
"00202589",
"00202651",
"00202721",
"00202821",
"00202834",
"00202836",
"00202861",
"00202893",
"00202899",
"00202942",
"00202990",
"00203012",
"00203021",
"00203073",
"00203076",
"00203184",
"00203205",
"00203219",
"00203297",
"00203448",
"00203453",
"00203528",
"00203531",
"00203586",
"00203611",
"00203635",
"00203649",
"00203686",
"00203735",
"00203745",
"00203797",
"00203834",
"00203890",
"00203896",
"00203901",
"00203920",
"00203996",
"00203998",
"00204010",
"00204025",
"00204038",
"00204128",
"00204159",
"00204244",
"00204276",
"00204278",
"00204280",
"00204287",
"00204323",
"00204363",
"00204381",
"00204402",
"00204418",
"00204487",
"00204490",
"00204564",
"00204702",
"00204736",
"00204835",
"00204841",
"00204883",
"00204912",
"00204918",
"00204923",
"00204994",
"00204997",
"00205037",
"00205127",
"00205133",
"00205286",
"00205388" 
};

#endif
/* define XX_PER */


#if 0
          1         2         3         4         5         6         7         8         9
0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

20181231,200917,0133250320,003590,99,07,06,3,1,182956,7081,6994,1,0,119,00205399,09,08,2,2,0,0
20181231,110340,0133250355,011175,99,07,08,3,1,194459,7286,8744,1,2,199,00205399,09,08,2,2,0,0
20181231,093459,0133250202,000425,99,09,06,1,0,229331,9631,6860,1,1,187,00205399,09,08,2,2,0,0
20181230,193810,0133250275,002185,99,08,07,1,0,210862,8361,7941,1,1,036,00205399,09,08,2,2,0,0
20181230,183038,0133250286,007010,99,09,08,1,0,231306,9182,8870,1,1,124,00205399,09,08,2,2,0,0
20181230,165814,0133250184,001060,99,07,06,3,1,183702,7425,6770,1,0,057,00205399,09,08,2,2,0,0
20181230,114507,0133250200,001265,99,07,06,3,1,183408,7004,6375,1,0,088,00205399,09,08,2,2,0,0
20181230,110250,0133250187,000930,99,07,07,3,1,189854,7678,7972,1,2,090,00205399,09,08,2,2,0,0
20181230,103931,0133250292,004320,99,07,08,3,1,191527,7097,8887,0,0,055,00205399,09,08,2,2,0,0
20181229,205141,0133250337,002425,99,08,06,3,1,207760,8623,6968,1,0,075,00205399,09,08,2,2,0,0

#endif

#if 0




#include <stdio.h>
 
struct Envase
{
    int id; //Identificador
    int volumen; //Volumen en mililitros
    float precio; //Precio en euros
};
 
/* Metodo de ordenacion I: ordenar por identificador*/
int ordenarID (const void * a, const void * b)
{
    /*Se hace un casting para convertir los punteros void a punteros de Envases
      y se guarda su identificador*/
    int id_a = ((struct Envase*)a)->id;
    int id_b = ((struct Envase*)b)->id;
 
    //Se comparan los dos identificadores
    if(id_a > id_b)
        return 1;
 
    else
        return -1;
}
 
 
/* Metodo de ordenacion II: ordenar por precio/unidad de volumen */
int ordenar_precio_volumen (const void * a, const void * b)
{
    /*Se hace un casting para convertir los punteros void a punteros de Envases
      y se guarda su volumen y precio*/
    float precio_a = ((struct Envase*)a)->precio;
    int volumen_a = ((struct Envase*)a)->volumen;
    float precio_b = ((struct Envase*)b)->precio;
    int volumen_b = ((struct Envase*)b)->volumen;
 
    //Se calcula precio/volumen
    float val_a = precio_a/volumen_a;
    float val_b = precio_b/volumen_b;
 
    //Se comparan los dos valores
    if(val_a > val_b)
        return 1;
 
    else
        return -1;
}
 
 
int main()
{
    int dimension = 4; //Dimension del vector
 
    struct Envase array[4]; //Vector de envases
    array[0].id = 5;
    array[0].volumen = 600;
    array[0].precio = 12;
 
    array[1].id = 6;
    array[1].volumen = 550;
    array[1].precio = 7.99;
 
    array[2].id = 1;
    array[2].volumen = 400;
    array[2].precio = 25;
 
    array[3].id = 3;
    array[3].volumen = 600;
    array[3].precio = 9.5;
 
 
     
    //Ordenar el vector por ID
    qsort((void*)array, dimension, sizeof(struct Envase), ordenarID);
 
 
    //Mostrar el vector por ID. El output es {1 3 5 6}
    printf("\nOrdenado por ID: {");
    for(int i = 0; i < dimension; i++)
    {
        printf("%d ", array[i].id);
    }
    printf("}\n");
 
 
    //Ordenar el vector por precio/volumen
    qsort((void*)array, dimension, sizeof(struct Envase), ordenar_precio_volumen);
 
 
    //Mostrar el vector por ID. El output ahora es {6 3 5 1}
    printf("\nOrdenado por precio/vol: {");
    for(int i = 0; i < dimension; i++)
    {
        printf("%d ", array[i].id);
    }
    printf("}\n\n");
 
     
    return 0;
}




#endif

