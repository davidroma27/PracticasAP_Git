#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <emmintrin.h> //Librerias para la tecnología SSE2

// Para fundido a negro
/*
    __m128i B = _mm_set1_epi8 (char A);
    __m128i A = _mm_load_si128 (__m128i const *P); // P alineado a 16 Bytes
    __m128i A = _mm_loadu_si128 (__m128i const *P); // P no alineado a 16 Bytes
    __m128i C = _mm_subs_epu8 (__m128i A, __m128i B);
    _mm_store_si128 (__m128i *P, __m128i B); // P alineado a 16 Bytes
    _mm_storeu_si128 (__m128i *P, __m128i B); // P no alineado a 16 Bytes
 */


int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Usage: %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    //CV_LOAD_IMAGE_COLOR = 1 forces the resultant IplImage to be colour.
    //CV_LOAD_IMAGE_GRAYSCALE = 0 forces a greyscale IplImage.
    //CV_LOAD_IMAGE_UNCHANGED = -1
    IplImage* Img1 = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
    
    // Always check if the program can find a file
    if (!Img1) {
        printf("Error: fichero %s no leido\n", argv[1]);
        return EXIT_FAILURE;
    }

    //Crea una imagen con las caracteristicas de la imagen de entrada
    IplImage* ImgC= cvCreateImage(cvSize(Img1->width,Img1->height) , Img1->depth, Img1->nChannels);
    
    if (!ImgC) {
        printf("Error: Creando nueva imagen\n");
        return EXIT_FAILURE;
    }
    
    //Creamos el bucle que recorre la imagen
    unsigned int row, col;
    __m128i *pImg1;
    __m128i *pImgC;
    __m128i UNOS = _mm_set1_epi8 (1);
    unsigned int step;
    
    //Para cada fila recorre las columnas de cada imagen
    //La variable step cuenta los ciclos que se hacen para decrementar los valores
    //Se establece en 256 porque es el tamaño que tiene cada componente de la imagen
    for(step=0; step<256; step++){ 
                
        for(row=0; row<Img1->height; row++){
            
            pImg1=(__m128i*)(Img1 ->imageData + (row*Img1->widthStep));
            pImgC =(__m128i*)(ImgC ->imageData + (row*ImgC->widthStep));
            for(col=0; col<Img1->widthStep; col+=16){
                
                __m128i A = _mm_loadu_si128 (pImg1); // P no alineado a 16 Bytes
                __m128i B = _mm_loadu_si128 (pImgC);
                __m128i C = _mm_adds_epu8 (B, UNOS);
                __m128i D = _mm_min_epu8 (A, C);
                _mm_storeu_si128 (pImgC, D); // P no alineado a 16 Bytes
                pImg1++;
                pImgC++;
                 
            }
        }
        
        //Muestra la imagen sobre la que se aplica el fundido
        cvShowImage("Blanca", ImgC);
        cvWaitKey(10);
    }
    
    /**
    // a visualization window is created with title 'image'
    cvNamedWindow(argv[1], CV_WINDOW_NORMAL);
    // img is shown in 'image' window
    cvShowImage(argv[1], Img1);
    cvWaitKey(0);*/

    //Creamos la ventana para la imagen para el fundido negro
    cvNamedWindow("Blanca", CV_WINDOW_NORMAL);
    /*cvShowImage("Negra", ImgMod);
    cvWaitKey(0);*/
    
    // memory release for img before exiting the application
    /*cvReleaseImage(&Img1);
    cvReleaseImage(&ImgMod);*/
    
    // Self-explanatory
    cvDestroyWindow(argv[1]);
    cvDestroyWindow("Blanca");

    return EXIT_SUCCESS;

}