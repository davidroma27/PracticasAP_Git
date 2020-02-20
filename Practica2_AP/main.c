#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

/**
 * Mostrar 3 imágenes tal como muestra la figura 2, cada una con una componente RGB
 * @param argc
 * @param argv
 * @return 
 */

int main(int argc, char** argv) {
    
    //Se pasa una imagen como parametro. argc = 2 (nombre programa + imagen)
    if (argc != 2) {
        printf("Usage: %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    //CV_LOAD_IMAGE_COLOR = 1 forces the resultant IplImage to be colour.
    //CV_LOAD_IMAGE_GRAYSCALE = 0 forces a greyscale IplImage.
    //CV_LOAD_IMAGE_UNCHANGED = -1
    //Carga la imagen desde memoria 
    IplImage* Img1 = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    
    // Always check if the program can find a file
    if (!Img1) {
        printf("Error: fichero %s no leido\n", argv[1]);
        return EXIT_FAILURE;
    }

    //Creamos 3 imágenes, una para cada componente de color
    IplImage* ImgBLUE = cvCreateImage(cvSize(Img1->width, Img1->height),Img1->depth,Img1->nChannels);
    IplImage* ImgGREEN = cvCreateImage(cvSize(Img1->width, Img1->height),Img1->depth,Img1->nChannels);
    IplImage* ImgRED = cvCreateImage(cvSize(Img1->width, Img1->height),Img1->depth,Img1->nChannels);
    
    if (!ImgBLUE || !ImgGREEN || !ImgRED) {
        printf("Error: Creando nueva imagen\n");
        return EXIT_FAILURE;
    }
    
    //Creamos un bucle for que recorra la imagen original
    unsigned int row, col;
    unsigned char *pImg1;
    unsigned char *pImgBLUE;
    unsigned char *pImgGREEN;
    unsigned char *pImgRED;
    
    for(row = 0; row < Img1->height; row++){
        //Casteamos para que tenga un valor de 256 bits y alineamos la imagen al principio de cada fila con widthStep
        pImg1 = (unsigned char*) (Img1->imageData + (row*Img1->widthStep));
        pImgBLUE = (unsigned char*) (ImgBLUE->imageData + (row*ImgBLUE->widthStep));
        pImgGREEN = (unsigned char*) (ImgGREEN->imageData + (row*ImgGREEN->widthStep));
        pImgRED = (unsigned char*) (ImgRED->imageData + (row*ImgRED->widthStep));
        
        for(col = 0; col < Img1->width; col++){
            //Componente BLUE
            *pImgBLUE++ = *pImg1++;
            *pImgGREEN++ = 0;
            *pImgRED++ = 0;
            pImgBLUE++;
                        
            //Componente GREEN
            *pImgBLUE++ = 0;
            *pImgGREEN++ = *pImg1++;
            *pImgRED++ = 0;
            pImgGREEN++;
            
            //Componente RED  
            *pImgBLUE++ = 0;
            *pImgGREEN++ = 0;
            *pImgRED++ = *pImg1++;
            pImgRED++;
            
        }
    }
    
    //Se muestra la imagen origen
    cvNamedWindow(argv[1], CV_WINDOW_NORMAL);
    // img is shown in 'image' window
    cvShowImage(argv[1], Img1);
    cvWaitKey(0);

    //Se muestra la componente azul
    cvNamedWindow("Componente AZUL", CV_WINDOW_NORMAL);
    cvShowImage("Componente AZUL", ImgBLUE);
    cvWaitKey(0);
    
    //Se muestra la componente verde
    cvNamedWindow("Componente VERDE", CV_WINDOW_NORMAL);
    cvShowImage("Componente VERDE", ImgGREEN);
    cvWaitKey(0);
    
    //Se muestra la componente roja
    cvNamedWindow("Componente ROJA", CV_WINDOW_NORMAL);
    cvShowImage("Componente ROJA", ImgRED);
    cvWaitKey(0);

    // memory release for img before exiting the application
    cvReleaseImage(&Img1);
    cvReleaseImage(&ImgBLUE);
    cvReleaseImage(&ImgGREEN);
    cvReleaseImage(&ImgRED);

    // Self-explanatory
    cvDestroyWindow(argv[1]);
    cvDestroyWindow("Componente AZUL");
    cvDestroyWindow("Componente VERDE");
    cvDestroyWindow("Componente ROJA");

    return EXIT_SUCCESS;

}

