/*
 * DCE05968 - Estruturas de Dados I
 * Funções responsaveis pelos filtros que podem ser aplicados na imagem.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Imagem.h"  /* Tipos e protótipos de funções que manipulam imagens */
#include "Filtros.h" /* Protótipo das funções responsáveis pelos filtros das imagens */

/* Função responsável por escurecer uma imagem
 * A função recebe um ponteiro para um struct Imagem e altera/escurece 
 * cada um de seus pixels em cada canal de cor (RGB).
 * 
 * Analise essa função com bastante atenção. Ela irá te auxiliar no entendimento 
 * do TAD Imagem e como outros filtros podem ser implementados.
 *
 * Você não precisa modificar essa função 
 */
void escurecerImagem(Imagem *img){
    int v;
    printf("Digite o fator de escurecimento: ");
    scanf("%d", &v);
    /* Cada canal de cor (RGB) de cada pixel é reduzido 'v' do valor.
     * Note que devemos garantir que o valor esteja entre 0 e 255.   
     * Como estamos subtraindo, verificamos apenas se o valor é >= 0 
     * Note também a utilização de expressão ternária e o cast (conversão) 
     * entre os valores Byte (unsigned int) e int. Esse cast evita erros nas 
     * operações matemáticas.
     */ 
    for (int h = 0; h < obtemAltura(img); h++) {
        for (int w = 0; w < obtemLargura(img); w++) {
            //Obtém o pixel da posição (h, w) da imagem 
            Pixel pixel = obtemPixel(img, h, w);
            //Modifica cada canal de cor do pixel
            pixel.cor[RED]   = (((int)pixel.cor[RED]   - v) >= 0 ? ((int)pixel.cor[RED] - v) : 0);
            pixel.cor[GREEN] = (((int)pixel.cor[GREEN] - v) >= 0 ? ((int)pixel.cor[GREEN] - v) : 0);
            pixel.cor[BLUE]  = (((int)pixel.cor[BLUE]  - v) >= 0 ? ((int)pixel.cor[BLUE] - v) : 0);
            //Grava o novo pixel na posição (h, w) da imagem
            recolorePixel(img, h, w, pixel);
        }
    }
}

/* Função responsável por clarear uma imagem
 * A função recebe um ponteiro para um struct Imagem e altera/clareia 
 * cada um de seus pixels em cada canal de cor (RGB).
 */
void clarearImagem(Imagem *img){

    int v;
    printf("Digite o fator de clareamento: ");
    scanf("%d", &v);

    for (int h = 0; h < obtemAltura(img); h++) {
        for (int w = 0; w < obtemLargura(img); w++) {
            Pixel pixel = obtemPixel(img, h, w);
            pixel.cor[RED]   = (((int)pixel.cor[RED]   + v) <= 255 ? (pixel.cor[RED]   + v) : 255);
            pixel.cor[GREEN] = (((int)pixel.cor[GREEN] + v) <= 255 ? (pixel.cor[GREEN] + v) : 255);
            pixel.cor[BLUE]  = (((int)pixel.cor[BLUE]  + v) <= 255 ? (pixel.cor[BLUE]  + v) : 255);

            recolorePixel(img, h, w, pixel);
        }
    }

}

/* Função responsável por converter uma imagem colorida em escala de cinza
 * A função recebe um ponteiro para um struct Imagem e converte 
 * cada um de seus pixels em cada canal de cor (RGB) em uma tonalidade de cinza.
 * 
 * No pdf de descrição do EP existem duas estretégias para a implementação 
 * desse filtro. Você deve implementar uma delas ou pesquisar por outras alternativas. 
 */
void escalaDeCinzaImagem(Imagem *img){

    //Calculamos a media dos valores R-G-B de um Pixel, e atribuimos essa media a cada um desses Pixels
    int media;
	for (int i = 0 ; i <obtemAltura(img) ; i ++) {
		for (int j = 0 ; j <obtemLargura(img) ; j ++) {
            
            Pixel pixel = obtemPixel(img,i,j);

			media=((int)(pixel.cor[RED]) + (pixel.cor[GREEN]) + (pixel.cor[BLUE]))/3;
			pixel.cor[RED]   = media;
            pixel.cor[GREEN] = media;
            pixel.cor[BLUE]  = media;

            recolorePixel(img, i, j, pixel);

		}
	}

}

/* Função responsável por aplicar o filtro de Sobel na imagem. 
 * Leia o pdf do EP para obter mais informações de como o filtro é implementado. 
 */
void filtroSobel(Imagem *img){

    Imagem *copia=copiaImagem(img);
    int A = obtemAltura(img);
    int L = obtemLargura(img);
    int i, j;
    //Percorre cada pixel da matriz, dado um determinado indice, percorremos os elementos vizinhos
    for (i=1;i<A-1;i++){
        for(j=1 ;j<L-1;j++){
            Pixel pixel  = obtemPixel(img,i,j);
            Pixel pix1 = obtemPixel(copia,i-1,j-1);
            Pixel pix2 = obtemPixel(copia,i-1,j);
            Pixel pix3 = obtemPixel(copia,i-1,j+1);
            Pixel pix4 = obtemPixel(copia,i,j-1);
            Pixel pix5 = obtemPixel(img,i,j);
            Pixel pix6 = obtemPixel(copia,i,j+1);
            Pixel pix7 = obtemPixel(copia,i+1,j-1);
            Pixel pix8 = obtemPixel(copia,i+1,j);
            Pixel pix9 = obtemPixel(copia,i+1,j+1);

            int gxred=   (((int)pix1.cor[RED])   + (2 * ((int)pix4.cor[RED]))   + ((int)pix7.cor[RED])   - ((int)pix3.cor[RED])    - (2 * ((int)pix6.cor[RED]))   - ((int)pix9.cor[RED]));
            int gxgreen= (((int)pix1.cor[GREEN]) + (2 * ((int)pix4.cor[GREEN])) + ((int)pix7.cor[GREEN]) - ((int)pix3.cor[GREEN])  - (2 * ((int)pix6.cor[GREEN])) - ((int)pix9.cor[GREEN]));
            int gxblue=  (((int)pix1.cor[BLUE])  + 2 * (((int)pix4.cor[BLUE]))  + ((int)pix7.cor[BLUE])  - ((int)pix3.cor[BLUE])   - (2 * ((int)pix6.cor[BLUE]))  - ((int)pix9.cor[BLUE]));

            int gyred=   (((int)pix1.cor[RED])   + (2 * ((int)pix2.cor[RED]))   + ((int)pix3.cor[RED])   - ((int)pix7.cor[RED])   - (2 * ((int)pix8.cor[RED]))   - ((int)pix9.cor[RED]));
            int gygreen= (((int)pix1.cor[GREEN]) + (2 * ((int)pix2.cor[GREEN])) + ((int)pix3.cor[GREEN]) - ((int)pix7.cor[GREEN]) - (2 * ((int)pix8.cor[GREEN])) - ((int)pix9.cor[GREEN]));
            int gyblue=  (((int)pix1.cor[BLUE])  + (2 * ((int)pix2.cor[BLUE]))  + ((int)pix3.cor[BLUE])  - ((int)pix7.cor[BLUE])  - (2 * ((int)pix8.cor[BLUE]))  - ((int)pix9.cor[BLUE]));

            int pixelred   =   ((gyred)+(gxred))/2 ;
            int pixelgreen =   ((gygreen)+(gxgreen))/2;
            int pixelblue  =   ((gyblue)+(gxblue))/2;

            pixelred=   ((pixelred)   >=0   ? (pixelred)   :0);
            pixelgreen= ((pixelgreen) >=0   ? (pixelgreen) :0);
            pixelblue=  ((pixelblue)  >=0   ? (pixelblue)  :0);

            pixelblue=  ((pixelblue)   <= 255 ?  (pixelblue ) : 255);
            pixelred=   ((pixelred)    <=255  ?  (pixelred)   : 255);
            pixelgreen=  ((pixelgreen) <=255  ?  (pixelgreen) : 255);

            pixel.cor[RED]   = pixelred;
            pixel.cor[GREEN] = pixelgreen;
            pixel.cor[BLUE]  = pixelblue ;

            recolorePixel(img, i, j, pixel);
        }

    }
    liberaImagem(copia);

    
    /* Dicas:
     * 1) Se quiser, pode ignorar as bordas das imagens
     * 2) Para o pixel na posição (h, w), obtenha os outros 8 pixels vizinhos e aplique a matriz gx e gy 
     *    em cada banda de cor do pixel. Combine o valor obtido por gx e gy e modifique o pixel de img.
     *    Lembre-se que a obtenção dos pixels deve ser a partir de uma cópia da imagem. 
     * 3) Verifique se o novo valor obtido para a banda de cor é um valor válido (entre 0 e 255).
    */
}

/* Função responsável por aplicar a detecçõa de bordas de Laplace na imagem. 
 * Leia o pdf do EP para obter mais informações de como o filtro é implementado. 
 */
void deteccaoBordasLaplace(Imagem *img) {

    Imagem *copia=copiaImagem(img);
    int A = obtemAltura(img);
    int L = obtemLargura(img);
    int i, j;
    //Percorre cada pixel da matriz, dado um determinado indice, percorre os elementos vizinhos, e aplica o efeito, de forma analoga ao filtro Sobel
    for (i=1;i<A-1;i++){
        for(j=1 ;j<L-1;j++){

            Pixel pixel = obtemPixel(img,i,j);
            Pixel pix1 = obtemPixel(copia,i-1,j-1);
            Pixel pix2 = obtemPixel(copia,i-1,j);
            Pixel pix3 = obtemPixel(copia,i-1,j+1);
            Pixel pix4 = obtemPixel(copia,i,j-1);
            Pixel pix5 = obtemPixel(copia,i,j);
            Pixel pix6 = obtemPixel(copia,i,j+1);
            Pixel pix7 = obtemPixel(copia,i+1,j-1);
            Pixel pix8 = obtemPixel(copia,i+1,j);
            Pixel pix9 = obtemPixel(copia,i+1,j+1);

            int pixelred   =   (4 * (((int)pix5.cor[RED]))   -((int)pix2.cor [RED] )   -((int)pix4.cor [RED] )   -((int)pix6.cor [RED] )   - ((int)pix8.cor [RED] ))  ;
            int pixelgreen =   (4 * (((int)pix5.cor[GREEN])) -((int)pix2.cor[GREEN])   -((int)pix4.cor[GREEN])   -((int)pix6.cor[GREEN])   - ((int)pix8.cor[GREEN]))  ;
            int pixelblue  =   (4 * (((int)pix5.cor[BLUE]))  -((int)pix2.cor [BLUE])   -((int)pix4.cor [BLUE])   -((int)pix6.cor [BLUE])   - ((int)pix8.cor [BLUE]))  ;

            pixelred=   ((pixelred)   > 0   ? (pixelred)   :0);
            pixelgreen= ((pixelgreen) > 0   ? (pixelgreen) :0);
            pixelblue=  ((pixelblue)  > 0   ? (pixelblue)  :0);

            pixelblue=   ((pixelblue)   < 255  ?  (pixelblue ) : 255);
            pixelred=    ((pixelred)    < 255  ?  (pixelred)   : 255);
            pixelgreen=  ((pixelgreen)  < 255  ?  (pixelgreen) : 255);




            pixel.cor[RED]   = (pixelred)  ;
            pixel.cor[GREEN] = (pixelgreen);
            pixel.cor[BLUE]  = (pixelblue) ;

            recolorePixel(img, i, j, pixel);

        }}
    liberaImagem(copia);
    /* Siga as mesmas dicas do filtro de Sobel */
}

/* Função responsável por aplicar na imagem o filtro definido por você. Seja criativo! 
 * Filtros que apenas fazem algumas modificações simples em cada pixel 
 * (por exemplo, negativo, espelhar a imagem) não serão considerados.
 */


void meuFiltro(Imagem *img){

    Imagem *copia=copiaImagem(img);
    int A = obtemAltura(img);
    int L = obtemLargura(img);
    int i, j;
    //Percorre cada pixel da matriz, dado um determinado indice, percorre os elementos vizinhos
    for (i=1;i<A-1;i++){
        for(j=1 ;j<L-1;j++){

            Pixel pixel = obtemPixel(img,i,j);
            Pixel pix1 = obtemPixel(copia,i-1,j-1);
            Pixel pix2 = obtemPixel(copia,i-1,j);
            Pixel pix3 = obtemPixel(copia,i-1,j+1);
            Pixel pix4 = obtemPixel(copia,i,j-1);
            Pixel pix5 = obtemPixel(copia,i,j);
            Pixel pix6 = obtemPixel(copia,i,j+1);
            Pixel pix7 = obtemPixel(copia,i+1,j-1);
            Pixel pix8 = obtemPixel(copia,i+1,j);
            Pixel pix9 = obtemPixel(copia,i+1,j+1);


            int pixelred   = ((0.299 * ((int)pix1.cor[RED])) + (0.587 * ((int)pix2.cor[RED]))  + (0.144 * ((int)pix3.cor[RED]))) * 2;
            int pixelgreen = ((0.596 * ((int)pix4.cor[RED])) - (0.0247 * ((int)pix5.cor[RED])) - (0.322 * ((int)pix6.cor[RED]))) * 4;
            int pixelblue  = ((0.211 * ((int)pix7.cor[RED])) - (0.523 * ((int)pix8.cor[RED]))  + (0.311 * ((int)pix9.cor[RED]))) * 9;

    

            pixelred=   ((pixelred)   > 0   ? (pixelred)   :0);
            pixelgreen= ((pixelgreen) > 0   ? (pixelgreen) :0);
            pixelblue=  ((pixelblue)  > 0   ? (pixelblue)  :0);

            pixelblue=   ((pixelblue)   < 255  ?  (pixelblue ) : 255);
            pixelred=    ((pixelred)    < 255  ?  (pixelred)   : 255);
            pixelgreen=  ((pixelgreen)  < 255  ?  (pixelgreen) : 255);


            int media=((int)(pixel.cor[RED]) + (pixel.cor[GREEN]) + (pixel.cor[BLUE]))/2;

			pixel.cor[RED]   = (pixelred) + media;
            pixel.cor[GREEN] = (pixelgreen) + media;
            pixel.cor[BLUE]  = (pixelblue) + media;

            recolorePixel(img, i, j, pixel);

        }
    }
    liberaImagem(copia);

}