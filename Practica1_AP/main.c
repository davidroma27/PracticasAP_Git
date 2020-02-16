/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: AP
 *
 * Created on 7 de febrero de 2020, 12:12
 */

#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

/**
 * 
 * @param argc -> Indica el numero de parametros que tiene el programa al ejecutarse
 * @param argv -> Es un vector de 2 dimensiones que almacena el contenido de los parametros que se le pasan al programa
 * @return 
 */

int main(int argc, char** argv) {

    if (argc != 3) {
        printf("Usage: %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    //CV_LOAD_IMAGE_COLOR = 1 forces the resultant IplImage to be colour.
    //CV_LOAD_IMAGE_GRAYSCALE = 0 forces a greyscale IplImage.
    //CV_LOAD_IMAGE_UNCHANGED = -1
    IplImage* Img1 = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    IplImage* Img2 = cvLoadImage(argv[2], CV_LOAD_IMAGE_UNCHANGED);
    
    // Always check if the program can find a file
    if (!Img1 && !Img2) {
        printf("Error: fichero %s no leido\n", argv[1]);
        return EXIT_FAILURE;
    }

    //ventana img1
    cvNamedWindow(argv[1], CV_WINDOW_NORMAL);
    //ventana img2
    cvNamedWindow(argv[2], CV_WINDOW_NORMAL);
    
    //Muestra la imagen con los parametros pasados
    //argv[1]->nombre de la imagen(parametro 1)
    //Img1->La propia imagen pasada por parametro
    cvShowImage(argv[1], Img1);
    
    //Mostrar imagen 2
    cvShowImage(argv[2], Img2);
    
    
    cvWaitKey(0); //Espera a que se pulse una tecla
                  //Si es 0 espera indefinidamente
                  //Si es mayor que 0 espera el valor indicado en ms

    // memory release for img before exiting the application
    cvReleaseImage(&Img1);
    cvReleaseImage(&Img2);

    // Self-explanatory
    cvDestroyWindow(argv[1]);
    cvDestroyWindow(argv[2]);
    
    return EXIT_SUCCESS;

}
