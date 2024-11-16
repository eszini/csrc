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
 *	cnv_f02.c
 *	
 *
 *	2024_04_04
 *
 *
 *	conversores de codigo fuente de fortran
 *
 * 	Opciones:
 * 	-h   		forma de uso
 *	
 *	-v		verbose
 *
 *
 *	Changes pending...
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
int	gp_uso(int);		/* usage */
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
 *
 *	Estructuras y variables para prueba de malloc de vector de ptr
 *
 */


#define	MAX_NOBJ		1000

typedef	struct	tip_nobj	*ptr_nobj;

typedef	struct	tip_nobj
{		
	char	nobj_c1[64];		/* c1 */
	int	nobj_c2;		/* c2 */
	int	nobj_c3;		/* c3 */

}	str_nobj;


ptr_nobj	pn1_nobj, *pn2_nobj;

ptr_nobj	vec_nobj[MAX_OBJ];

str_nobj	st1_nobj;











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
 *	y cargar trx10_frd.txt en el vector
 */

#if 0
#define	XX_PER	1
#endif

#if	XX_PER
char	xx_per[5000][10];
#endif





/* definicion de funcion como parametro */
#if 0

int	csv_parser(char *, int (*)(int,int,char *));

int	bcvs_load(char *, FILE *, int (*)(int,int,char *));
int	fx(int,int,char *);

#endif
/* definicion de funcion como parametro */









/*
 * -----------------------------------------------------------------------------------
 *
 *	(MMM)
 *
 *	main
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
	 	gp_uso(0);


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
 *	(MMM)
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
	{	printf ("%s Sale de mostrar reportes \n\n",gp_tm());
	}
}


/*
 * -----------------------------------------------------------------------------------
 *
 *	(MMM)
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



	if ( ffprb)
	{
		if (ffprb == 1)
			pro_pruebas1();
		if (ffprb == 2)
			pro_pruebas2();
	}	

	if ( ffexc)
	{
		if (ffexc == 1)	
			pro_exec1();
		if (ffexc == 2)
			pro_exec2();
	}



	/* proceso */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de proceso principal \n\n",gp_tm());
	}

}




/*
 * -----------------------------------------------------------------------------------
 *
 *	(MMM)
 *
 *	pro_exec 1
 *
 *	exec aparte ...
 *
 * -----------------------------------------------------------------------------------
 */

/*
 *
 *
 *
 */



#if 1

int	pro_exec1()
{

	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra proceso exec 1 \n\n",gp_tm());
	}
		

	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale proceso exec 1 \n\n",gp_tm());
	}
}


#endif




/*
 * -----------------------------------------------------------------------------------
 *
 *	(MMM)
 *
 *	pro_exec 2
 *
 *	exec aparte ...
 *
 * -----------------------------------------------------------------------------------
 */


/* exec2  */

#if 1

int	pro_exec2()
{

	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra a proceso exec 2 \n\n",gp_tm());
	}
		


	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale de proceso exec 2 \n\n",gp_tm());
	}

}


#endif

/* exec2  */









/*
 * -----------------------------------------------------------------------------------
 *
 *	(MMM)
 *
 *	pro_prueba 1
 *
 *	pruebas ...
 *
 * -----------------------------------------------------------------------------------
 */

/*
 *
 *
 */


#if 1


int	pro_pruebas1()
{

	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra proceso prueba 1 \n\n",gp_tm());
	}
		

	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale proceso prueba 1 \n\n",gp_tm());
	}
}


#endif

/*
 * -----------------------------------------------------------------------------------
 *
 *	(MMM)
 *
 *	pro_prueba 2
 *
 *	pruebas ...
 *
 * -----------------------------------------------------------------------------------
 */



#if 1

int	pro_pruebas2()
{

	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Entra proceso prueba 2\n\n",gp_tm());
	}
		
	/* prueba */
	if (gp_fverbose("d1"))
	{	printf ("%s Sale proceso prueba 2 \n\n",gp_tm());
	}
}


#endif





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
 * -----------------------------------------------------------------------------------
 *
 *	(MMM)
 *
 *	Rutinas generales gp_xxxxxx
 *
 *
 * -----------------------------------------------------------------------------------
 */


/* general gp algo */
#if 1




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
 * 	uso
 *
 *	texto para describir el uso de la herramienta
 *
 *
 * -----------------------------------------------------------------------------------
 */

int	gp_uso(x)
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
 * 	gp_fverbose
 *
 * -----------------------------------------------------------------------------------
 */




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




/*
 * -----------------------------------------------------------------------------------
 *
 * 	gp_tm
 *
 * -----------------------------------------------------------------------------------
 */




char	*gp_tm()
{
	time_t t;
  	struct tm *tm;

	t=time(NULL);
	tm=localtime(&t);
	strftime(gp_fyh, MAXR , "[%Y/%m/%d %H:%M:%S]", tm);

	return(gp_fyh);
}



/*
 * -----------------------------------------------------------------------------------
 *
 * 	df
 *
 * -----------------------------------------------------------------------------------
 */




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
/* general gp algo */




/*
 * -----------------------------------------------------------------------------------
 *
 *	end of source
 *	end of source
 *	end of source
 *	end of source
 *	end of source
 *
 * -----------------------------------------------------------------------------------
 */







