#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char** argv) {

    if (argc != 3) {
        printf("Usage: %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    //CV_LOAD_IMAGE_COLOR = 1 forces the resultant IplImage to be colour.
    //CV_LOAD_IMAGE_GRAYSCALE = 0 forces a greyscale IplImage.
    //CV_LOAD_IMAGE_UNCHANGED = -1
    IplImage* Personajes = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    IplImage* Fondo = cvLoadImage(argv[2], CV_LOAD_IMAGE_UNCHANGED);
    
    // Always check if the program can find a file
    if (!Personajes && !Fondo) {
        printf("Error: fichero %s no leido\n", argv[1]);
        printf("Error: fichero %s no leido\n", argv[2]);
        return EXIT_FAILURE;
    }
    
    unsigned char *pPersonajes, *pFondo, *pSalida = 0;
    
    int row, col;
    
    for(row=0; row < 48; row++){
        pFondo = (unsigned char*) (Fondo->imageData + (row*Fondo->widthStep));
        //Los personajes se empiezan a recorrer en la fila 908 y columna 780*4(Bytes)
        pPersonajes = (unsigned char*) (Personajes->imageData + ((row+908)*Personajes->widthStep)+(780*Personajes->nChannels));
        
        for(col=0; col < 56; col++){
            if(*(pPersonajes+3) == 255){ //Se obtiene el valor del canal alfa que sea 255(Donde aparece el personaje)
                *pFondo++ = *pPersonajes++;
                *pFondo++ = *pPersonajes++;
                *pFondo++ = *pPersonajes++;
                pFondo++;
                pPersonajes++;
            }
            else{
                //Si la mascara de los personajes no es 255 se salta al siguiente pixel
                pFondo += 4;
                pPersonajes += 4;
            }
        }
    }
    
/*
    // a visualization window is created with title 'image'
    cvNamedWindow(argv[1], CV_WINDOW_NORMAL);
    // img is shown in 'image' window
    cvShowImage(argv[1], Personajes);
    cvWaitKey(0);    
*/

    // a visualization window is created with title 'image'
    cvNamedWindow(argv[2], CV_WINDOW_NORMAL);
    // img is shown in 'image' window
    cvShowImage(argv[2], Fondo);
    cvWaitKey(0);


    // memory release for img before exiting the application
    cvReleaseImage(&Personajes);
    cvReleaseImage(&Fondo);

    // Self-explanatory
    cvDestroyWindow(argv[1]);
    cvDestroyWindow(argv[2]);
    
    return EXIT_SUCCESS;

}

