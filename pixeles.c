#include <stdio.h>
#include <stdlib.h>

typedef unsigned int Uint;

typedef struct coordenada{
	Uint x;
	Uint y;
} Coordenada;

typedef struct color{
	Uint rojo;
	Uint verde;
	Uint azul;
} Color;

typedef struct pixel{
	Color pixel;
} Pixel;

typedef struct bmp{
	Uint altura;
	Uint anchura;
	Pixel **matriz;
} Bmp;

Bmp *crear_bmp(Uint, Uint);
void mostrar_bmp(Bmp);
void set_pixel(Pixel *, Color);
void destruir_bmp(Bmp *);
void dibujar_rectangulo_color_en_imagen(Bmp *, Color, Coordenada, Coordenada);
Color color_predominante(Bmp);
void negativo_imagen(Bmp *);
void invertir_imagen(Bmp *);
int menu();

int main(){
	Uint alto, ancho;			//alto y ancho de nuestra matriz!
	Bmp *mapaDeBits;			//puntero a una estructura Bmp llamado mapaDeBits
	Coordenada supIzq, infDer;	//estructuras de tipo Coordenada
	Color c, predominante;		//estruturas de tipo Color

	printf("\n-------- Mapa de Bits --------");
	printf("\n\nNo hice un menu porque los iba a enredar con la weá, así que el programa funciona así:");
	printf("\n1- Nos piden el alto y el ancho del mapa de bits.");
	printf("\n2- Crea el mapa de bits inicializando todos los colores en blanco (255, 255, 255).");
	printf("\n3- Muestra la direccion en la que se creo el mapa.");
	printf("\n4- Muetra el mapa de bits.");
	printf("\n5- Ahora empieza el proceso para dibujar un rectangulo.");
	printf("\n6- Nos pide las coordenadas X e Y de la esquina sup izq e inf der.");
	printf("\n7- Nos pide el color que tendra el rectangulo a dibujar.");
	printf("\n8- Dibuja el rectangulo y lo muestra por pantalla.");
	printf("\n9- Nos muestra el color predominante en el mapa de bits.");
	printf("\n10- Crea el negativo de la imagen y lo muestra por pantalla.");
	printf("\n11- Crea el espejo horizontal de la imagen y lo muestra por pantalla.");
	printf("\n12- Libera la memoria de la matriz dinamica y de la estructura dinamica.");

	printf("\n\nAlto: ");			//pedimos el alto que tendrá el mapa de bits.
	scanf("%d", &alto);		
	printf("Ancho: ");			//pedimos el ancho que tendrá el mapa de bits.
	scanf("%d", &ancho);

	mapaDeBits = crear_bmp(alto, ancho);	//creamos el mapa de bits. OJO que acá estamos asignando una dirección de memoria, es decir, estructura dinámica!
	printf("\nDireccion donde se creó el mapa de bits: %p\n", mapaDeBits);	//ésta es la dirección del heap donde creamos nuestra estructura dinámica.

	mostrar_bmp(*mapaDeBits);	//mostramos por pantalla el mapa de bits.

	printf("\n");


	// OJO que las coordenadas X e Y no son las mismas que en un mapa al que estamos acostumbrados!
	// X = filas => ALTO de la matriz
	// Y = columnas => ANCHO de la matriz

	do{
		printf("\nIngrese coordernada X de la esquina superior izquierda: ");	//pedimos las coordenadas de la esquina superior izquierda mientras no sea
		scanf("%d", &supIzq.x);													//menor, ni mayor que los límites de nuestra matriz.
		printf("Ingrese coordernada Y de la esquina superior izquierda: ");
		scanf("%d", &supIzq.y);
	}while(supIzq.x<0 || supIzq.x>mapaDeBits->altura || supIzq.y<0 || supIzq.y>mapaDeBits->anchura);	

	do{
		printf("\nIngrese coordernada X de la esquina inferior derecha: ");		//pedimos las coordenadas de la esquina inferior derecha mientras no sea
		scanf("%d", &infDer.x);													//menor, que las coordenadas de la esquina superior izq o mayor
		printf("Ingrese coordernada Y de la esquina inferior derecha: ");		//que la matriz.
		scanf("%d", &infDer.y);
	}while(infDer.x>mapaDeBits->altura || infDer.x<supIzq.x || infDer.y>mapaDeBits->anchura || infDer.y<supIzq.y);	

	do{
		printf("\nIngrese un color en formato RGB\nRojo: ");					//pedimos los colores del pixel a dibujar en el rectángulo
		scanf("%d", &c.rojo);													//mientras no sean menores que 0 o mayores que 255.
		printf("Verde: ");
		scanf("%d", &c.verde);
		printf("Azul: ");
		scanf("%d", &c.azul);
	}while(c.rojo<0 || c.verde<0 || c.azul<0 || c.rojo>255 || c.verde>255 || c.azul>255);

	dibujar_rectangulo_color_en_imagen(mapaDeBits, c, supIzq, infDer);			//se dibuja el rectángulo dentro de la matriz del color antes ingresado.
	mostrar_bmp(*mapaDeBits);													//lo mostramos por pantalla.

	predominante = color_predominante(*mapaDeBits);								//asignamos el color predominante a la estructura de tipo Color llamada "predominante".
	printf("\n\nEl color predominante en nuestro mapa de bits es: (%d, %d, %d)"
			, predominante.rojo, predominante.verde, predominante.azul);		//mostramos el color en formato (R,G,B) por pantalla.

	printf("\n\n");
	negativo_imagen(mapaDeBits);	//Creamos el negativo de la imagen.

	printf("\n");
	invertir_imagen(mapaDeBits);	//Creamos el espejo horizontal de la imagen.

	destruir_bmp(mapaDeBits);		//liberamos la memoria de la matriz y la estructura dinámica.
	printf("\n");

	return 0;
}

/* Esta funcion recibe el alto y el ancho de una imagen; crea un struct dinamica de tipo Bmp, inicializa su altura y anchura, 
dimensiona su matriz dinamica de pixeles de ese alto y ese ancho inicializando todos sus pixeles en el color blanco (255) 
y retorna un puntero ducha struct (Bmp) */
Bmp *crear_bmp(Uint alto, Uint ancho){
	int i, j;
	Bmp *b;		//puntero a una estructura de tipo Bmp

	b = (Bmp *)malloc(sizeof(Bmp));		//asignamos la memoria que tendrá la estructura dinámica
	if(b == NULL){				//verificamos si se pudo asignar memoria a la estructura dinámica,
		printf("\nError");		//sino, mostramos error y terminamos el programa con la función exit(-1)
		exit(-1);
	}

	b->altura = alto;		//asignamos el alto al campo altura de la matriz b (como b es un puntero a una estructura, se necesita el operador FLECHA)
					//que es exactamente lo mismo que escribir (*b).altura = alto; y los paréntesis son NECESARIOS.
	b->anchura = ancho;		//asignamos el ancho al campo anchura de la matriz b exactamente igual que el alto.
	b->matriz = (Pixel **)malloc(alto*sizeof(Pixel *));	//Asignamos la memoria que tendrá nuestra **matriz con el tamaño del contenido de la matriz,
														//es decir, el tamaño del campo de la matriz, que es nuestro (Pixel *).
														//Desglosando un poco sería:
														//b->matriz = la dirección del campo **matriz de nuestra estructura b.
														//(Pixel **) = casteamos la asignación de memoria al tipo doble puntero de un Pixel
														//esto se debe a que la función malloc es así: void *malloc(tamaño de lo que queremos asignar).
														//y ya que estamos trabajando con una matriz (**matriz), se colocan 2 asteriscos.
														//alto = las filas de la matriz, es decir, el alto de ésta.
														//sizeof(Pixel *) = el tamaño del contenido de cada fila, es decir, cada vector dinámico que asignaremos.
	
	if(b->matriz == NULL){	//verificamos si se pudo asignar la memoria
		printf("\nError");
		exit(-1);
	}

	for(i=0; i<alto; i++){		
		b->matriz[i] = (Pixel *)malloc(ancho*sizeof(Pixel));	//asignamos la memoria de cada VECTOR dinámico que vendrían siendo nuestras columnas de la matriz.
		if(b->matriz[i] == NULL){								//verificamos si se pudo asignar memoria a cada uno de los vectores que estamos creando.
			printf("\nError");
			exit(-1);
		} 
	}

	for(i=0; i<alto; i++){						//recorremos toda la matriz asignando el color blanco (255, 255, 255) a cada píxel de ésta.
		for(j=0; j<ancho; j++){
			b->matriz[i][j].pixel.rojo = 255;	//acá vemos que la matriz está indexeada [i][j], para así acceder a cada slot de ésta, que a su vez
			b->matriz[i][j].pixel.verde = 255;	//contiene un píxel, y éste último contiene un color.
			b->matriz[i][j].pixel.azul = 255;
		}
	}

	return b;	// retornamos la dirección de memoria donde se creó nuestra estructura dinámica!
}

/* Esta funcion recibe un mapa de bits (struct de tipo Bmp) y muestra por pantalla, su altura, su anchura y su matriz de pixeles. */
void mostrar_bmp(Bmp b){
	int i, j;

	printf("\nFormato RGB (Red, Green, Blue)");

	for(i=0; i<b.altura; i++){							//recorremos toda la matriz slot, por slot, y los vamos mostrando por pantalla.
		printf("\n");									//para hacerlo bonito al momento de mostrarlo, agregué 0 a los números mayores de 100 y 10
		for(j=0; j<b.anchura; j++){						//cosa de que no se vieran más chicos los píxeles en la matriz, no se compliquen con eso.
			/* 
				En caso de que se compliquen con los ceros al leerlo, es exactamente lo mismo que hacer esto:

				printf("(%d, %d, %d) ", b.matriz[i][j].pixel.rojo, b.matriz[i][j].pixel.verde, b.matriz[i][j].pixel.azul);

				, es decir, pueden borrar todos los if, else y printfs de abajo y dejar esa única línea, pero se va a ver feito 
				si un píxel es (0, 0, 0) y en la misma columna hay un (255, 255, 255) <- nótese que ese ocupa más espacio al tener más números xd.
			*/
			if(b.matriz[i][j].pixel.rojo < 100){
				if(b.matriz[i][j].pixel.rojo < 10){
					printf("(00%d ", b.matriz[i][j].pixel.rojo);
				}else{
					printf("(0%d ", b.matriz[i][j].pixel.rojo);
				}
			}else{
				printf("(%d ", b.matriz[i][j].pixel.rojo);
			}
			
			if(b.matriz[i][j].pixel.verde < 100){
				if(b.matriz[i][j].pixel.verde < 10){
					printf("00%d ", b.matriz[i][j].pixel.verde);
				}else{
					printf("0%d ", b.matriz[i][j].pixel.verde);
				}
			}else{
				printf("%d ", b.matriz[i][j].pixel.verde);
			}

			if(b.matriz[i][j].pixel.azul < 100){
				if(b.matriz[i][j].pixel.azul < 10){
					printf("00%d) ", b.matriz[i][j].pixel.azul);
				}else{
					printf("0%d) ", b.matriz[i][j].pixel.azul);
				}
			}else{
				printf("%d) ", b.matriz[i][j].pixel.azul);
			}
		}
	}
}

/* Esta funcion recibe un puntero a una struct de tipo Pixel (px) y le asigna el color especificado en la variable c. */
void set_pixel(Pixel *px, Color c){ 	//nótese que recibimos un puntero a una estructura de tipo Pixel.
	px->pixel = c;	// Asignamos el color a un píxel que es de tipo Color.
					// Esto se puede hacer sólo si los campos son estáticos, en caso contrario sería así:
					/*
						px->pixel.rojo = c.rojo;
						px->pixel.verde = c.verde;
						px->pixel.azul = c.azul;

						, es decir, campo por campo, si se confunden en el certamen, ocupen la 2da forma (asignación de campo por campo).
					*/
}

/* Esta funcion recibe un puntero a un mapa de bits (struct de tipo Bmp) y libera, la struct dinamica y 
todas las posiciones de memoria reservadas dinamicamente de su matriz de pixeles. */
void destruir_bmp(Bmp *b){
	int i;

	for(i=0; i<b->altura; i++){		//Para liberar la memoria, se tiene que hacer al inverso que cuando la asignamos, por lo tanto,
		free(b->matriz[i]);			//primero liberamos la memoria de los vectores dinámicos que equivalen a las columnas de la matriz.
	}

	free(b->matriz);				//Segundo, liberamos la memoria de las filas de la matriz, que equivale a un vector dinámico.
	free(b);						//Tercero, liberamos la memoria de la estructura dinámica.
}

/* Esta funcion recibe un puntero a una struct de tipo Bmp y dibuja un rectangulo de un cierto color desde la coordenada supIzq hasta la 
coordenada infDer, con un determinado alto y ancho. */
void dibujar_rectangulo_color_en_imagen(Bmp *b, Color c, Coordenada supIzq, Coordenada infDer){
										// recibimos un puntero a una estructura b, un color c, las coordenadas de las esquinas.
	int i, j;

	for(i=supIzq.x; i<=infDer.x; i++){			//Recorremos la matriz columna por columna.
		for(j=supIzq.y; j<=infDer.y; j++){		//recorremos la matriz fila por fila.
			set_pixel(&(b->matriz[i][j]), c);	//asignamos el pixel con el color que recibimos.
												//Nótese que envíamos la dirección de cada píxel, para así modificarlo directamente en la matriz.
												//Me refiero a: &(b->matriz[i][j]) <- incluyan los paréntesis, sí o sí, just in case, a no ser
												//que se sepan las precedencias de los operadores de memoria.
		}
	}
}

/* Esta funcion recibe una struct de tipo Bmp y retorna una struct con el color predominante de la imagen (el color que mas
se repite), si existe mas de un color predominante retorna cualquiera de los ellos. */
Color color_predominante(Bmp b){
	int i, j, k ,l, cont, contAux;					//Ya, aquí los voy a enredar un poquito, así que pongan atención a lo que se hace:
	Color aux, predominante;					//Tenemos un contador, un contador auxiliar, un color auxiliar y el color predominante.
	contAux = 0;

	for(i=0; i<b.altura; i++){					//Acá vamos a pescar todos los slots de la matriz, es decir, todos los [i][j], uno por uno para
		for(j=0; j<b.anchura; j++){				//ir comparando 1 sólo con toooooda la matriz, e ir contando cuántas veces se repite cada color.
			aux = b.matriz[i][j].pixel;			//Asignamos el color del pixel que vamos a comparar con toda la matriz.
			cont = 0;							//inicializamos el contador que hará válida nuestra comparación xd.

			for(k=0; k<b.altura; k++){						//Acá comenzamos a recorrer toda la matriz comparando cada campo con el primer slot, es decir, matriz[0][0]
				for(l=0; l<b.anchura; l++){					//Luego de comparar el primer slot de la matriz, con toda la matriz y generar la cuenta, recién se empieza a comparar
												//el siguiente slot matriz[0][1], con toda la matriz nuevamente -sí, es súper ineficiente-, y así sucesivamente.
					if(aux.rojo == b.matriz[k][l].pixel.rojo && aux.verde == b.matriz[k][l].pixel.verde && aux.azul == b.matriz[k][l].pixel.azul){
						cont++;						//si el color es equivalente, contamos.
						if(cont >= contAux){				//si el contador actual, es mayor al contador auxiliar, que es donde estamos guardando las 
												//comparaciones anteriores, asignamos un nuevo color predominante.
												// OJO que siempre irá guardando el último píxel predominante, aunque hayan varios que sean predominante,
												// ya que las instrucciones (arriba de la función) dicen que se puede mostrar cualquiera, en nuestro caso
												// mostramos el último para no hacernos dramas.
							predominante = b.matriz[k][l].pixel;	// Se asigna el color predominante actual
							contAux = cont;				// nuestro contador del color predominante actual, pasa a ser nuestro contador auxiliar, 
												// para una próxima comparación.
						}
					}
				}				
			}
		}
	}

	return predominante;	// retornamos el color predominante! (una estructura de tipo Color llamada predominante).
}

/* Esta funcion recibe un puntero a una struct de tipo Bmp y genera el negativo de la imagen. En el caso del modelo de color RGB se
determina el valor inverso de un color restando del valor maximo el valor del color original. */
void negativo_imagen(Bmp *b){
	int i, j;

	printf("\nEl negativo de la matriz: ");

	for(i=0; i<b->altura; i++){			//Recorremos la matriz slot, por slot.
		for(j=0; j<b->anchura; j++){
			b->matriz[i][j].pixel.rojo = 255 - b->matriz[i][j].pixel.rojo;		// Asignamos el color negativo uno por uno, ya que el píxel puede tener
			b->matriz[i][j].pixel.verde = 255 - b->matriz[i][j].pixel.verde;	// distintos valores, por ejemplo, (1, 100, 230), lo que nos da un
			b->matriz[i][j].pixel.azul = 255 - b->matriz[i][j].pixel.azul;		// negativo de (254, 125, 25).
												// esto quiere decir que no lo podemos hacer de la forma en la
												// que estábamos asignando los píxeles:
												// b->matriz[i][j].pixel = 255 - b->matriz[i][j].pixel; <- ESTO ESTÁ MALO!!!
		}
	}

	mostrar_bmp(*b); 	//envíamos nuestra estructura de tipo Bmp "negativa" para mostrarla por pantalla.
}

/*Esta funcion recibe un puntero a una strct de tipo Bmp e invierte la imagen que contiene de izquierda a derecha (genera el 
espejo de la imagen de forma horizontal) */
void invertir_imagen(Bmp *b){
	int i, j, auxR, auxG, auxB;

	printf("\nLa matriz invertida horizontalmente: ");

	for(i=0; i<b->altura; i++){						
		for(j=0; j < ((b->anchura)/2); j++){			//Recorremos la matriz pero sólo hasta la mitad de las columnas, para así generar el espejo de éstas.
			auxR = b->matriz[i][j].pixel.rojo;		//Asignamos los colores a los auxiliares, para así reemplazar la columna izquierda por la derecha.
			auxG = b->matriz[i][j].pixel.verde;
			auxB = b->matriz[i][j].pixel.azul;

			b->matriz[i][j].pixel = b->matriz[i][b->anchura - 1 - j].pixel;	//Asignamos los colores de la columna espejo derecha, a la izquierda.

			b->matriz[i][b->anchura - 1 - j].pixel.rojo = auxR;			//Asignamos los colores de la columna espejo izquierda, a la derecha.
			b->matriz[i][b->anchura - 1 - j].pixel.verde = auxG;			//Para hacerlo gráficamente:
			b->matriz[i][b->anchura - 1 - j].pixel.azul = auxB;			// 1 2 3 4 5	el 1 tiene que pasar a ser el 4 y viceversa, y así sucesivamente.
												// a b c d e =>	e d c b a
												// f g h i j =>	j i h g f
												// Pude haber creado una estructura de tipo Color que se llamara aux
												// y hacer la asignación de cada píxel, en vez de color por color.
		}
	}

	mostrar_bmp(*b);	// mostramos nuestro Bitmap espejo!
}
