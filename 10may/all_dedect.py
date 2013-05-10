#!/usr/bin/env python

import cv
import math
import ctype *

CvHaarClassifierCascade *cascade,*cascade_e,*cascade_nose,*cascade_mouth;
CvMemStorage            *storage;

 *eye_cascade="./haarcascades/haarcascade_mcs_eyepair_big.xml";


#/*Eyes detection*/
def detectEyes( IplImage *img,CvRect *r)
{
     *eyecascade
    Cv.Seq *eyes
    eye_detect=0


  #  //eye detection starts
  # /* Set the Region of Interest: estimate the eyes' position */

    cv.SetImageROI(img,                    #/* the source image */
          cv.Rect
          (
              r->x,            				# /* x = start from leftmost */
              r->y + (r->height/5.5), 		# /* y = a few pixels from the top */
              r->width,      				# /* width = same width with the face */
              r->height/3.0   				# /* height = 1/3 of face height */
          )
      )

      #/* detect the eyes */
      eyes = cv.HaarDetectObjects( img,           # /* the source image, with the estimated location defined */
                                  cascade_e,      #/* the eye classifier */
                                  storage,        #/* memory buffer */
                                  1.15, 3, 0,     #/* tune for your app */
                                  cv.Size(25, 15)  #/* minimum detection scale */
                                )

    print("\no of eyes detected are %d",eyes->total)


        # /* draw a rectangle for each detected eye */
        for i in (eyes ? eyes->total : 0):
          
              eye_detect=eye_detect + 1
              #/* get one eye */
              Cv.Rect *eye = (Cv.Rect*)cv.GetSeqElem(eyes, i)
              #/* draw a red rectangle */
                        cv.Rectangle(img,
                                    cv.Point(eye->x, eye->y),
                                    cv.Point(eye->x + eye->width, eye->y + eye->height),
                                    CV_RGB(0, 0, 255),
                                    1, 8, 0
                                   )
           


 }