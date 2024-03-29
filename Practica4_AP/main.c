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

/**
 * Ejercicio 4: A partir de una imagen de 4 canales (abierta con la opción CV_LOAD_IMAGE_UNCHANGED), mostrar el
canal alfa como una imagen en escala de grises.
 * @param argc
 * @param argv
 * @return 
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

    //Se crea y muestra la imagen origen
    cvNamedWindow(argv[1], CV_WINDOW_NORMAL);
    cvShowImage(argv[1], ImgOrigen);
    cvWaitKey(0);
    
/*
    __m128i reg1 = _mm_set1_epi32(0x00FF0000); //Mascara para obtener color solo rojo
    __m128i reg2 = _mm_set1_epi32(0x0000FF00); //Mascara para obtener color solo verde
    __m128i reg3 = _mm_set1_epi32(0x000000FF); //Mascara para obtener color solo azul
*/
    
    __m128i alfa = _mm_set1_epi32(0xFF000000); //Mascara alfa
                
    int row, col;
    //Recorremos la imagen origen y declaramos los punteros de imagen origen y destino
    for (row = 0; row < ImgOrigen->height; row++) {
        __m128i *pImgOrigen = (__m128i *) (ImgOrigen->imageData + row * ImgOrigen->widthStep);
        __m128i *pImgDestino = (__m128i *) (ImgDestino->imageData + row * ImgDestino->widthStep);
        __m128i registro;
        
        for (col = 0; col < ImgOrigen->widthStep; col += 16) {
            __m128i origen = _mm_loadu_si128(pImgOrigen++); //Cargo en memoria la imagen a procesar
            __m128i aorigen = _mm_and_si128(origen, alfa); //Se realiza el AND de la imagen y alfa
            
            __m128i arojo = _mm_srli_epi32(aorigen, 8); //Obtengo alfa para rojo
            __m128i averde = _mm_srli_epi32(arojo, 8); //Obtengo alfa para verde
            __m128i aazul = _mm_srli_epi32(averde, 8); //Obtengo alfa para azul
            
            __m128i A = _mm_or_si128(arojo, averde); //Obtengo alfa para rojo y verde
            __m128i B = _mm_or_si128(A,aazul); //Obtengo alfa para rojo, verde y azul
            
            _mm_storeu_si128(pImgDestino++,B);
            
            
        }
    }

    cvNamedWindow("Destino", CV_WINDOW_NORMAL);
    cvShowImage("Destino", ImgDestino);
    cvWaitKey(0);
    
    // memory release for img before exiting the application
    cvReleaseImage(&ImgOrigen);
    cvReleaseImage(&ImgDestino);
    
    // Self-explanatory
    cvDestroyWindow(argv[1]);
    cvDestroyWindow("Destino");

    return EXIT_SUCCESS;

}

