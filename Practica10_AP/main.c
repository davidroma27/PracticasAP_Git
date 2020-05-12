#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <emmintrin.h> //Librerias para la tecnología SSE2

int main(int argc, char** argv) {

    if (argc != 3) {
        printf("Usage: %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    //CV_LOAD_IMAGE_COLOR = 1 forces the resultant IplImage to be colour.
    //CV_LOAD_IMAGE_GRAYSCALE = 0 forces a greyscale IplImage.
    //CV_LOAD_IMAGE_UNCHANGED = -1
    IplImage* imagenInicial = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
    IplImage* imagenFinal = cvLoadImage(argv[2], CV_LOAD_IMAGE_COLOR);

    //Convertimos la imagen inicial a IPL_DEPTH_32F creando una nueva imagen
    IplImage* imagenInicialFloat = cvCreateImage(cvSize(imagenInicial->width,
            imagenInicial->height), IPL_DEPTH_32F, imagenInicial->nChannels);

    IplImage* imagenTransicionFloat = cvCreateImage(cvSize(imagenInicial->width,
            imagenInicial->height), IPL_DEPTH_32F, imagenInicial->nChannels);

    IplImage* imagenFinalFloat = cvCreateImage(cvSize(imagenFinal->width,
            imagenFinal->height), IPL_DEPTH_32F, imagenFinal->nChannels);

    //Se aplica la funcion cvConvertScale para realizar el escalado a float
    cvConvertScale(imagenInicial, imagenInicialFloat, 1.0 / 255.0, 0);
    cvConvertScale(imagenInicial, imagenTransicionFloat, 1.0 / 255.0, 0);
    cvConvertScale(imagenFinal, imagenFinalFloat, 1.0 / 255.0, 0);
    
    //Configuramos el fichero para el video de la transicion
    CvVideoWriter* pWriter;
    pWriter = cvCreateVideoWriter("tran.avi", -1, 10, cvSize(imagenInicial->width, imagenInicial->height), 1);
    
    if (!pWriter) {
        printf("Video Writer Wrong!\n");
        return EXIT_FAILURE;
    }


    // Always check if the program can find a file
    if (!imagenInicial) {
        printf("Error: fichero %s no leido\n", argv[1]);
        return EXIT_FAILURE;
    }

    if (!imagenFinal) {
        printf("Error: fichero %s no leido\n", argv[2]);
        return EXIT_FAILURE;
    }

    int iteraciones = 25;
    int iteracion;

    //Bucle para fundir a negro la imagenInicialFloat
    for (iteracion == 0; iteracion <= iteraciones; iteracion++) {
        float alfa = ((float) iteraciones - iteracion) / (float) (iteraciones);
        __m128 alfaSSE = _mm_set_ps1(alfa);
        __m128 ccInicial, ccFinal, ccTrans1, ccTrans2;
        __m128 ccTransicion;

        int fila, cc;
        for (fila = 0; fila < imagenInicialFloat->height; fila++) {
            __m128 *pImagenInicialFloat = (__m128 *) (imagenInicialFloat->imageData
                    + fila * imagenInicialFloat->widthStep);
            __m128 *pImagenFinalFloat = (__m128 *) (imagenFinalFloat->imageData
                        + fila * imagenFinalFloat->widthStep);
            __m128 *pImagenTransicionFloat = (__m128 *) (imagenTransicionFloat->imageData
                        + fila * imagenTransicionFloat->widthStep);
            
            for (fila = 0; fila < imagenFinalFloat->height; fila++) {
                
                //Recorremos las imagenes inicial y final al mismo tiempo
                //En cada iteracion se aplica la operacion para el alfa de cada imagen
                //Alfa imagenInicial incrementa
                //Alfa imagenFinal decrementa (1-alfa)
                for (cc = 0; cc < imagenInicialFloat->width * imagenInicialFloat->nChannels; cc = cc + 4) {
                    for (cc = 0; cc < imagenFinalFloat->width * imagenFinalFloat->nChannels; cc = cc + 4) {
                        ccInicial = *pImagenInicialFloat++;
                        ccTrans1 = _mm_mul_ps(ccInicial, alfaSSE);
                        
                        ccInicial = *pImagenFinalFloat++;
                        ccTrans2 = _mm_mul_ps(ccInicial, 1 - alfaSSE);
                        
                        //Los dos resultados se suman y se almacenan en el puntero de la imagen resultante
                        ccTransicion = ccTrans1 + ccTrans2;                        
                        *pImagenTransicionFloat++ = ccTransicion;
                        
                    }
                }
            }
        }
        
        //Visualizamos la imagen
        cvNamedWindow("imagenTransicionFloat", CV_WINDOW_NORMAL);
        cvShowImage("imagenTransicionFloat", imagenTransicionFloat);
        cvWaitKey(100);
        
        //Se captura cada imagen y se crea un nuevo frame para el video 
        cvWriteFrame(pWriter, imagenTransicionFloat);
    }
    //Se cierra la creacion del video
    cvReleaseVideoWriter(&pWriter);
    
    //Se liberan las imagenes de memoria
    cvDestroyWindow("imagenTransicionFloat");
    
    cvReleaseImage(&imagenInicialFloat);
    cvReleaseImage(&imagenFinalFloat);
    cvReleaseImage(&imagenTransicionFloat);

    return EXIT_SUCCESS;
}
