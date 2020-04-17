#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#define ALTO 16
#define ANCHO 16

int compararBloques(int i, int j, IplImage * ImgOrigen, int k, int l, IplImage * ImgDestino) {
    int row, col;
    int diff = 0;
    int suma = 0;
    
    for (row = 0; row < ALTO; row++) {
        unsigned char *pFrutas = (unsigned char *) (ImgOrigen->imageData + (row + i) * ImgOrigen->widthStep + j * ImgOrigen->nChannels);
        unsigned char *pAves = (unsigned char *) (ImgDestino->imageData + (row + k) * ImgDestino->widthStep + l * ImgDestino->nChannels);

        for (col = 0; col < ANCHO; col++) {
            diff += abs(*pFrutas++ - *pAves++);
            diff += abs(*pFrutas++ - *pAves++);
            diff += abs(*pFrutas++ - *pAves++);
            suma += diff;
        }
    }
    return suma;
}

/**
 * FUNCION COPIAR BLOQUE:
 * Se pasa como parametro el punto (i,j) desde donde empezará a copiar pixeles en la imgOrigen.
 * Se pasa como parametro el punto (k,l) desde donde empezará a pegar en la imgDestino el bloque copiado.
 */
void copiarBloque(int i, int j, IplImage * ImgOrigen, int k, int l, IplImage * ImgDestino) {

    int row, col;

    for (row = 0; row < ALTO; row++) {
        unsigned char *pFrutas = (unsigned char*) (ImgOrigen->imageData + (i + row) * ImgOrigen->widthStep + j * ImgOrigen->nChannels);
        unsigned char *pAves = (unsigned char*) (ImgDestino->imageData + (k + row) * ImgDestino->widthStep + l * ImgDestino->nChannels);
        for (col = 0; col < ANCHO; col++) {
            *pAves++ = *pFrutas++;
            *pAves++ = *pFrutas++;
            *pAves++ = *pFrutas++;
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

    int fil1, col1, fil2, col2;
    int dif, maxPar, filMax, colMax;

    for (fil1 = 0; fil1 < Img1->height; fil1 += ALTO) {
        for (col1 = 0; col1 < Img1->width; col1 += ANCHO) {
            maxPar = INT_MAX;
            filMax = 0;
            colMax = 0;
            for (fil2 = 0; fil2 < (Img2->height); fil2 += ALTO) {
                for (col2 = 0; col2 < (Img2->width); col2 += ANCHO) {
                    dif = compararBloques(fil1, col1, Img1, fil2, col2, Img2);
                    if (dif < maxPar) {
                        maxPar = dif;
                        filMax = fil2;
                        colMax = col2;
                    }
                }
            }
            copiarBloque(filMax, colMax, Img2, fil1, col1, ImgRes);
        }
    }

    cvNamedWindow("Resultado", CV_WINDOW_AUTOSIZE);
    cvShowImage("Resultado", ImgRes);
    cvWaitKey(0);
    cvDestroyWindow("Resultado");

    cvReleaseImage(&Img1);
    cvReleaseImage(&Img2);
    cvReleaseImage(&ImgRes);

    return EXIT_SUCCESS;
}