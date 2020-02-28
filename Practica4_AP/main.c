#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <emmintrin.h> //Librerias para las instrucciones SSE2

/*
    __m128i B = _mm_set1_epi8 (char A);
    __m128i A = _mm_load_si128 (__m128i const *P); // P alineado a 16 Bytes
    __m128i A = _mm_loadu_si128 (__m128i const *P); // P no alineado a 16 Bytes
    __m128i C = _mm_subs_epu8 (__m128i A, __m128i B);
    _mm_store_si128 (__m128i *P, __m128i B); // P alineado a 16 Bytes
    _mm_storeu_si128 (__m128i *P, __m128i B); // P no alineado a 16 Bytes
 *  __m128i _mm_and_si128 (__m128i A, __m128iB);
 *  __m128i _mm_or_si128 (__m128i A, __m128iB);
 *  __m128i _mm_set1_epi32 (int A);
 */


int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Usage: %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    //CV_LOAD_IMAGE_COLOR = 1 forces the resultant IplImage to be colour.
    //CV_LOAD_IMAGE_GRAYSCALE = 0 forces a greyscale IplImage.
    //CV_LOAD_IMAGE_UNCHANGED = -1
    IplImage* ImgOrigen = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    IplImage* ImgDestino = cvCreateImage(cvSize(800,600), IPL_DEPTH_8U, 4);
    
    //Creamos una imagen para cada componente
    IplImage* ImgRoja = cvClone(ImgDestino); 
    IplImage* ImgVerde = cvClone(ImgDestino);
    IplImage* ImgAzul = cvClone(ImgDestino);

    //Se crea y muestra la imagen origen
    cvNamedWindow(argv[1], CV_WINDOW_NORMAL);
    cvShowImage(argv[1], ImgOrigen);
    cvWaitKey(0);
    
    __m128i soloRojo = _mm_set1_epi32(0x00FF0000); //Mascara para obtener color solo rojo
    __m128i soloVerde = _mm_set1_epi32(0x0000FF00); //Mascara para obtener color solo verde
    __m128i soloAzul = _mm_set1_epi32(0x000000FF); //Mascara para obtener color solo azul
    
    int row, col;
    //Recorremos la imagen origen y declaramos los punteros de cada componente
    for (row = 0; row < ImgOrigen->height; row++) {
        __m128i *pImgOrigen = (__m128i *) (ImgOrigen->imageData + row * ImgOrigen->widthStep);
        __m128i *pImgRoja = (__m128i *) (ImgRoja->imageData + row * ImgRoja->widthStep);
        __m128i *pImgVerde = (__m128i *) (ImgVerde->imageData + row * ImgVerde->widthStep);
        __m128i *pImgAzul = (__m128i *) (ImgAzul->imageData + row * ImgAzul->widthStep);
        __m128i registro;
        
        //Recorremos cada 16 pixeles y almacenamos en registro la img origen
        //Despues para cada componente hacemos la oprecion AND con registro y la mascara
        //Incrementamos cada componente para recorrer la img entera
        for (col = 0; col < ImgOrigen->widthStep; col += 16) {
            registro = *pImgOrigen++;
            *pImgRoja = _mm_and_si128(registro, soloRojo);
            pImgRoja++;
            
            *pImgVerde = _mm_and_si128(registro, soloVerde);
            pImgVerde++;
        
            *pImgAzul = _mm_and_si128(registro, soloAzul);
            pImgAzul++;
        }
    }

    cvNamedWindow("Solo rojo", CV_WINDOW_NORMAL);
    cvShowImage("Solo rojo", ImgRoja);
    cvWaitKey(0);
    
    cvNamedWindow("Solo verde", CV_WINDOW_NORMAL);
    cvShowImage("Solo verde", ImgVerde);
    cvWaitKey(0);
    
    cvNamedWindow("Solo azul", CV_WINDOW_NORMAL);
    cvShowImage("Solo azul", ImgAzul);
    cvWaitKey(0);
    
    // memory release for img before exiting the application
    cvReleaseImage(&ImgOrigen);
    cvReleaseImage(&ImgRoja);
    cvReleaseImage(&ImgVerde);
    cvReleaseImage(&ImgAzul);
    

    // Self-explanatory
    cvDestroyWindow(argv[1]);
    cvDestroyWindow("Solo roja");
    cvDestroyWindow("Solo verde");
    cvDestroyWindow("Solo azul");

    return EXIT_SUCCESS;

}

