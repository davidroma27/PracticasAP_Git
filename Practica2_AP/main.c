#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

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
    IplImage *ImgMod = cvCloneImage(Img1);
    
    if (!ImgMod) {
        printf("Error: Creando nueva imagen\n");
        return EXIT_FAILURE;
    }
    
    //Creamos el bucle que recorre la imagen
    unsigned int row, col;
    unsigned char *pImg;
    unsigned char *pImgMod;
    unsigned int step;
    
    //Para cada fila recorre las columnas de cada imagen
    //La variable step cuenta los ciclos que se hacen para decrementar los valores
    //Se establece en 256 porque es el tamaño que tiene cada componente de la imagen
    for(step=0; step<256; step++){ 
        
        /*for(row=0; row<Img1->height; row++){
            //Creamos el puntero de la nueva imagen para que se cambie cada dato a negro
            unsigned char *pImgMod = (unsigned char*) (ImgMod->imageData + (row*ImgMod->widthStep));
            
            for(col=0; col<Img1->width; col++){
                //Componente azul
                if(*pImgMod>0){
                    (*pImgMod)--; //Decrementa en 1 el valor del dato en cada pasada del bucle
                                  //Esto se repite para cada componente
                }
                pImgMod++;
                
                //Componente verde
                if(*pImgMod>0){
                    (*pImgMod)--;
                }
                pImgMod++;
                
                //Componente roja
                if(*pImgMod>0){
                    (*pImgMod)--;
                }
                pImgMod++;
            }
        }*/
        
        //Tambien se puede hacer con un solo bucle
        unsigned char *pImgMod = (unsigned char*)(ImgMod->imageData);
        for(row=0; row<Img1->imageSize;row++){
            if(*pImgMod>0){
                    (*pImgMod)--;
                }
                pImgMod++;
        }
        
        //Muestra la imagen sobre la que se aplica el fundido
        cvShowImage("Negra", ImgMod);
        cvWaitKey(20); 
    }
    /**
    // a visualization window is created with title 'image'
    cvNamedWindow(argv[1], CV_WINDOW_NORMAL);
    // img is shown in 'image' window
    cvShowImage(argv[1], Img1);
    cvWaitKey(0);*/

    //Creamos la ventana para la imagen para el fundido negro
    cvNamedWindow("Negra", CV_WINDOW_NORMAL);
    /*cvShowImage("Negra", ImgMod);
    cvWaitKey(0);*/
    
    // memory release for img before exiting the application
    /*cvReleaseImage(&Img1);
    cvReleaseImage(&ImgMod);*/
    
    // Self-explanatory
    cvDestroyWindow(argv[1]);
    cvDestroyWindow("Negra");

    return EXIT_SUCCESS;

}

