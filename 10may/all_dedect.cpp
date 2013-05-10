/* program to detect facial features using opencv*/

/* detectFacialfeatures() is the main function that detects the face followed by Eyes,nose and mouth detection.*/

// #include "stdafx.h"


/**
 * Loads static images from database and detect faces
 */
#include <stdio.h>
// #include<conio.h>
#include "cv.h"
#include "highgui.h"
#include "cvaux.h"

CvHaarClassifierCascade *cascade,*cascade_e,*cascade_nose,*cascade_mouth;
CvMemStorage            *storage;
char *face_cascade="./haarcascades/haarcascade_frontalface_alt2.xml";
char *eye_cascade="./haarcascades/haarcascade_mcs_eyepair_big.xml";
char *nose_cascade="./haarcascades/haarcascade_mcs_nose.xml";
char *mouth_cascade="./haarcascades/haarcascade_mcs_mouth.xml";

/*Mouth detect ion*/
void detectMouth( IplImage *img,CvRect *r)
{
   CvSeq *mouth;
   //mouth detecetion - set ROI
   cvSetImageROI(img,/* the source image */
                 cvRect(r->x,            /* x = start from leftmost */
                        r->y+(r->height *2/3), /* y = a few pixels from the top */
                        r->width,        /* width = same width with the face */
                        r->height/3    /* height = 1/3 of face height */
                       )
                );
    mouth = cvHaarDetectObjects(img,/* the source image, with the estimated location defined */
                                cascade_mouth,      /* the eye classifier */
                                storage,        /* memory buffer */
                                1.15, 4, 0,     /* tune for your app */
                                cvSize(25, 15)  /* minimum detection scale */
                               );

        for( int i = 0; i < (mouth ? mouth->total : 0); i++ )
        {

          CvRect *mouth_cord = (CvRect*)cvGetSeqElem(mouth, i);
          /* draw a red rectangle */
          cvRectangle(img,
                      cvPoint(mouth_cord->x, mouth_cord->y),
                      cvPoint(mouth_cord->x + mouth_cord->width, mouth_cord->y + mouth_cord->height),
                      CV_RGB(255,255, 255),
                      1, 8, 0
                    );
        }
     //end mouth detection

}

/*Nose detection*/
void detectNose( IplImage *img,CvRect *r)
{
  CvSeq *nose;

  //nose detection- set ROI
  cvSetImageROI(img,                    /* the source image */
                cvRect(r->x,            /* x = start from leftmost */
                       r->y , /* y = a few pixels from the top */
                       r->width,        /* width = same width with the face */
                       r->height  /* height = 1/3 of face height */
                      )
               );


  nose = cvHaarDetectObjects(img, /* the source image, with the estimated location defined */
                             cascade_nose,      /* the eye classifier */
                             storage,        /* memory buffer */
                             1.15, 3, 0,     /* tune for your app */
                             cvSize(25, 15)  /* minimum detection scale */
                            );

  for( int i = 0; i < (nose ? nose->total : 0); i++ )
      {
          CvRect *nose_cord = (CvRect*)cvGetSeqElem(nose, i);

          /* draw a red rectangle */
          cvRectangle(img,
                      cvPoint(nose_cord->x, nose_cord->y),
                      cvPoint(nose_cord->x + nose_cord->width, nose_cord->y + nose_cord->height),
                      CV_RGB(0,255, 0),
                      1, 8, 0
                    );

      }
}

/*Eyes detection*/
void detectEyes( IplImage *img,CvRect *r)
{
    char *eyecascade;
    CvSeq *eyes;
    int eye_detect=0;


   //eye detection starts
  /* Set the Region of Interest: estimate the eyes' position */

    cvSetImageROI(img,                    /* the source image */
          cvRect
          (
              r->x,            /* x = start from leftmost */
              r->y + (r->height/5.5), /* y = a few pixels from the top */
              r->width,        /* width = same width with the face */
              r->height/3.0    /* height = 1/3 of face height */
          )
      );

      /* detect the eyes */
      eyes = cvHaarDetectObjects( img,            /* the source image, with the estimated location defined */
                                  cascade_e,      /* the eye classifier */
                                  storage,        /* memory buffer */
                                  1.15, 3, 0,     /* tune for your app */
                                  cvSize(25, 15)  /* minimum detection scale */
                                );

      printf("\no of eyes detected are %d",eyes->total);


        /* draw a rectangle for each detected eye */
        for( int i = 0; i < (eyes ? eyes->total : 0); i++ )
          {
              eye_detect++;
              /* get one eye */
              CvRect *eye = (CvRect*)cvGetSeqElem(eyes, i);
              /* draw a red rectangle */
                        cvRectangle(img,
                                    cvPoint(eye->x, eye->y),
                                    cvPoint(eye->x + eye->width, eye->y + eye->height),
                                    CV_RGB(0, 0, 255),
                                    1, 8, 0
                                   );
           }


}
void detectFacialFeatures( IplImage *img,IplImage *temp_img,int img_no)
{

    char image[100],msg[100],temp_image[100];
    float m[6];
    double factor = 1;
    CvMat M = cvMat( 2, 3, CV_32F, m );
    int w = (img)->width;
    int h = (img)->height;
    CvSeq* faces;
    CvRect *r;

    m[0] = (float)(factor*cos(0.0));
    m[1] = (float)(factor*sin(0.0));
    m[2] = w*0.5f;
    m[3] = -m[1];
    m[4] = m[0];
    m[5] = h*0.5f;

    cvGetQuadrangleSubPix(img, temp_img, &M);
    CvMemStorage* storage=cvCreateMemStorage(0);
    cvClearMemStorage( storage );

    if( cascade )
        faces = cvHaarDetectObjects(img,cascade, storage, 1.2, 2, CV_HAAR_DO_CANNY_PRUNING, cvSize(20, 20));
    else
        printf("\nFrontal face cascade not loaded\n");

    printf("\n no of faces detected are %d",faces->total);


    /* for each face found, draw a red box */
    for(int i = 0 ; i < ( faces ? faces->total : 0 ) ; i++ )
    {
        r = ( CvRect* )cvGetSeqElem( faces, i );
        cvRectangle( img,cvPoint( r->x, r->y ),cvPoint( r->x + r->width, r->y + r->height ),
                     CV_RGB( 255, 0, 0 ), 1, 8, 0 );

        printf("\n face_x=%d face_y=%d wd=%d ht=%d",r->x,r->y,r->width,r->height);

        detectEyes(img,r);
        /* reset region of interest */
        cvResetImageROI(img);
        // detectNose(img,r);
        // cvResetImageROI(img);
        // detectMouth(img,r);
        // cvResetImageROI(img);
    }
    /* reset region of interest */
      cvResetImageROI(img);

      if(faces->total>0)
        {
            sprintf(image,"a%d.jpg",img_no);
            cvSaveImage( image, img );
        }
}

int main( int argc, char** argv )
{
    CvCapture *capture;
    IplImage  *img,*temp_img;
    int       key;

    char image[100],temp_image[100];


    /* load the classifier
       note that I put the file in the same directory with
       this code */
    storage = cvCreateMemStorage( 0 );
        cascade = ( CvHaarClassifierCascade* )cvLoad( face_cascade, 0, 0, 0 );
    cascade_e = ( CvHaarClassifierCascade* )cvLoad( eye_cascade, 0, 0, 0 );
    cascade_nose = ( CvHaarClassifierCascade* )cvLoad( nose_cascade, 0, 0, 0 );
    cascade_mouth = ( CvHaarClassifierCascade* )cvLoad( mouth_cascade, 0, 0, 0 );



    if( !(cascade || cascade_e ||cascade_nose||cascade_mouth) )
        {
        fprintf( stderr, "ERROR: Could not load classifier cascade\n" );
        return -1;
        }

    for(int j=1;j<3;j++)
    {

         sprintf(image,"%d.jpg",j);

        img=cvLoadImage(image);
        temp_img=cvLoadImage(image);

        if(!img)
        {
        printf("Could not load image file and trying once again: %s\n",image);
        }
        printf("\n curr_image = %s",image);

        detectFacialFeatures(img,temp_img,j);
    }


    cvReleaseHaarClassifierCascade( &cascade );
    cvReleaseHaarClassifierCascade( &cascade_e );

    cvReleaseHaarClassifierCascade( &cascade_nose );
    cvReleaseHaarClassifierCascade( &cascade_mouth );
    cvReleaseMemStorage( &storage );

     cvReleaseImage(&img);
     cvReleaseImage(&temp_img);


    return 0;
}
