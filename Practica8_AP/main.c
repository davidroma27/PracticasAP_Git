#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <emmintrin.h>
#include <time.h>

#define ALTO 16
#define ANCHO 16

int compararBloquesSSE2(int i, int j, IplImage*ImgOrigen, int k, int l, IplImage*ImgDestino){
    int row, col;
    int suma = 0;
    __m128i A, B, C, D, E;
    int valor=0;
    
    for (row = 0; row < ALTO; row++) {
        __m128i *pFrutas = (__m128i*) (ImgOrigen->imageData + (row + i) * ImgOrigen->widthStep + j * ImgOrigen->nChannels);
        __m128i *pAves = (__m128i*) (ImgDestino->imageData + (row + k) * ImgDestino->widthStep + l * ImgDestino->nChannels);

        for (col = 0; col < ANCHO * ImgDestino->nChannels; col+=16) {
            A = _mm_load_si128(pFrutas++); //Cargamos las 2 imagenes
            B = _mm_load_si128(pAves++);
            C = _mm_sad_epu8(A, B); //realiza las sumas de 8 bits
            D = _mm_srli_si128(C, 8); //desplaza la operación de C
            E = _mm_add_epi32(C, D); //realiza la suma de C y D
            valor = _mm_cvtsi128_si32(E); //copia el menor valor y lo almacenamos
            suma += valor;
        }
    }
    return suma;
}


int compararSSE2Optimizado(int i, int j, IplImage * ImgOrigen, int k, int l, IplImage * ImgDestino) {
    int row, col;
    __m128i A, B, C, D, E, suma;
    int valor=0;
    suma = _mm_set1_epi8(0);
    
    for (row = 0; row < ALTO; row++) {
        __m128i *pFrutas = (__m128i*) (ImgOrigen->imageData + (row + i) * ImgOrigen->widthStep + j * ImgOrigen->nChannels);
        __m128i *pAves = (__m128i*) (ImgDestino->imageData + (row + k) * ImgDestino->widthStep + l * ImgDestino->nChannels);

        for (col = 0; col < ANCHO * ImgDestino->nChannels; col+=16) {
            A = _mm_load_si128(pFrutas++); //Cargamos las 2 imagenes
            B = _mm_load_si128(pAves++);
            C = _mm_sad_epu8(A, B); //realiza las sumas de 8 bits
            suma = _mm_adds_epu8 (C, suma); //suma la nueva diferencia
        }
    }
        //Sacamos fuera del bucle las operaciones para un codigo mas eficiente
        D = _mm_srli_si128(suma, 8); //desplaza la operación de suma
        E = _mm_add_epi32(suma, D); //realiza la suma de suma y D
        return _mm_cvtsi128_si32(E); //Devuelve el menor valor de la suma
}

void copiarBloqueSSE2(int i, int j, IplImage * ImgOrigen, int k, int l, IplImage * ImgDestino) {

    int row, col;

    for (row = 0; row < ALTO; row++) {
        __m128i *pFrutas = (__m128i*) (ImgOrigen->imageData + (row + i) * ImgOrigen->widthStep + j * ImgOrigen->nChannels);
        __m128i *pAves = (__m128i*) (ImgDestino->imageData + (row + k) * ImgDestino->widthStep + l * ImgDestino->nChannels);
        
        for (col = 0; col < (ANCHO * ImgDestino->nChannels); col+=ANCHO) {
            __m128i A = _mm_load_si128(pFrutas++);
            _mm_store_si128(pAves++, A);
        }
    }
}

int main(int argc, char** argv) {

    if (argc != 3) {
        printf("Error: Usage %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    IplImage* Img1 = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
    IplImage* Img2 = cvLoadImage(argv[2], CV_LOAD_IMAGE_COLOR);
    IplImage* ImgRes = cvCreateImage(cvSize(Img1->width, Img1->height), 8, 3);

    if (!Img1) {
        printf("Error: file %s not found\n", argv[1]);
        return EXIT_FAILURE;
    }
    if (!Img2) {
        printf("Error: file %s not found\n", argv[2]);
        return EXIT_FAILURE;
    }
    
    struct timespec start, finish;
    float elapsed;
    
    //Empezamos a medir el tiempo que lleva realizar todos los calculos
    clock_gettime(CLOCK_MONOTONIC, &start);

    int row1, col1, row2, col2;
    int dif, maxPar, filMax, colMax;

    for (row1 = 0; row1 < Img1->height; row1 += ALTO) {
        for (col1 = 0; col1 < Img1->width; col1 += ANCHO) {
            maxPar = INT_MAX;
            filMax = 0;
            colMax = 0;
            for (row2 = 0; row2 < (Img2->height); row2 += ALTO) {
                for (col2 = 0; col2 < (Img2->width); col2 += ANCHO) {
                    dif = compararSSE2Optimizado(row1, col1, Img2, row2, col2, Img1);
                    if (dif < maxPar) {
                        maxPar = dif;
                        filMax = row2;
                        colMax = col2;
                    }
                }
            }
            copiarBloqueSSE2(filMax, colMax, Img1, row1, col1, ImgRes);
        }
    }
    
    //Detenemos la medición del tiempo una vez finaliza el bucle
    clock_gettime(CLOCK_MONOTONIC, &finish);
    
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    
    //Mostramos por pantalla el tiempo transcurrido
    printf("Tiempo transcurrido: %f", elapsed);

    cvNamedWindow("Resultado", CV_WINDOW_AUTOSIZE);
    cvShowImage("Resultado", ImgRes);
    cvWaitKey(0);
    cvDestroyWindow("Resultado");

    cvReleaseImage(&Img1);
    cvReleaseImage(&Img2);
    cvReleaseImage(&ImgRes);

    return EXIT_SUCCESS;
}

