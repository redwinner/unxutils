/*
 * rdm_util.c - Reading utilities for reading mtv-format data
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include "CNplot.h"

static void reset_maxmin();

/* 
 * Check to see if the number of points has been defined
 */
int CNmtv_check_points(rnpts,npts,type,format,lineno)
CNdouble *rnpts;
int      *npts;
char *type;
char *format;
int lineno;
{
   int error = CN_FALSE;

   /* Set the number of grid points */
   *npts = (int) rnpts->val;
   if (!rnpts->def) {
      (void) fprintf(stderr,"   warning (line %3d) %8s: ",lineno,format);
      (void) fprintf(stderr, "\"%s\" has not been defined!\n",type);
      error = CN_TRUE;

   } else if ((*npts) <= 0) {
      (void) fprintf(stderr,"   warning (line %3d) %8s: ",lineno,format);
      (void) fprintf(stderr,
      "No of array points is less than or equal to 0 : ");
      (void) fprintf(stderr,"%s=%d\n",type,npts);
      error = CN_TRUE;
   }

   /* Return */
   return(error);
}


/* 
 * Read an int array in binary format
 */
int *CNmtv_read_binary_int_array(fp,npts)
FILE   *fp;
int npts;
{
   int *xarr=NULL;

   /* Error checking */
   if (fp==NULL) {
      (void) fprintf(stderr,"Error - Cannot read from NULL file/pipe!\n");
      return(NULL);
   }
   if (npts<=0) {
      (void) fprintf(stderr,"Error - Invalid size (npts=%d)\n",npts);
      return(NULL);
   }

   /* allocate space for the data in a 1D array */
   xarr = CNcreate_1D_int_array(npts);

   /*
    * read the intensity values and save in a 1D array
    */
   if (fread((char *)xarr,sizeof(int),npts,fp) != npts) {
      (void) fprintf(stderr,"   ***Binary read error of data array!\n");
      CNfree_1D_int_array(xarr);
      return(NULL);
   }

   /*
    * Return the array pointer - it is up to the calling routine to
    * free this array 
    */
   return(xarr);
}


/* 
 * Read an double array in binary format
 */
double *CNmtv_read_binary_dbl_array(fp,npts)
FILE   *fp;
int npts;
{
   double *xarr=NULL;

   /* Error checking */
   if (fp==NULL) {
      (void) fprintf(stderr,"Error - Cannot read from NULL file/pipe!\n");
      return(NULL);
   }
   if (npts<=0) {
      (void) fprintf(stderr,"Error - Invalid size (npts=%d)\n",npts);
      return(NULL);
   }

   /* allocate space for the data in a 1D array */
   xarr = CNcreate_1D_double_array(npts);

   /*
    * read the intensity values and save in a 1D array
    */
   if (fread((char *)xarr,sizeof(double),npts,fp) != npts) {
      (void) fprintf(stderr,"   ***Binary read error of data array!\n");
      CNfree_1D_double_array(xarr);
      return(NULL);
   }

   /*
    * Return the array pointer - it is up to the calling routine to
    * free this array 
    */
   return(xarr);
}


/* 
 * Read a line and get the numbers off the line
 */
void CNmtv_read_line(line,arrptr,lim,npts)
char   *line;
double *arrptr;
int    lim, *npts;
{
   char   *word[CN_MAXWORDS];
   int    nw=0, i;
   double z;

   /* Parse line into lots of words */
   if ((nw = CNgetwords(line,word, CN_MAXWORDS)) >= 0) {
      /* Go thru each word */
      for (i=0; i<nw; i++) {
         z = atof(word[i]);
         if (*npts < lim) arrptr[(*npts)++] = z;
      }
   }

   /* Free the words */
   CNfreewords(&nw,word);
}


/*
 * Go thru the array of values and fill the first array
 */
void CNmtv_filter_array(valarr,nvals,grid_arr,ngrid,max_grid)
double *valarr;      /* The source array                 */
int    *nvals;       /* No of points in the source array */
double *grid_arr;    /* The array to be filled           */
int    *ngrid;       /* No of points in the target array */
int    max_grid;     /* Size of the target array         */
{
   int needed, ntake, i;
   double z;

   /* No of points needed */
   needed = max_grid - (*ngrid);
   if (needed <= 0) return;
 
   /* No of points to be taken from the source array */
   ntake = (needed > (*nvals)) ? (*nvals) : needed;

   /* Do the actual transfer */
   for (i=0; i<ntake; i++) {
      z = valarr[i];
      CNset_1D_double_array_value(grid_arr,(*ngrid)++,max_grid,&z);
   }

   /* Reset the source array */
   for (i=0; i<((*nvals)-ntake); i++)
      valarr[i] = valarr[i+ntake];
   (*nvals) = (*nvals) - ntake; 
}


/*
 * Adjust xmin, xmax, ymin, ymax, zmin, zmax 
 */
void CNmtv_adjust_boundaries(xmin, xmax, ymin, ymax, zmin, zmax)
double *xmin, *xmax, *ymin, *ymax, *zmin, *zmax;
{
   double dx, dy, dz, dl, tmp;
   int    xsmall, ysmall, zsmall;
#define INTERV 0.5e-5;

   /* Switch max and min if necessary */
   if (*xmax < *xmin) {tmp = *xmin;  *xmin = *xmax;  *xmax = tmp;}
   if (*ymax < *ymin) {tmp = *ymin;  *ymin = *ymax;  *ymax = tmp;}
   if (*zmax < *zmin) {tmp = *zmin;  *zmin = *zmax;  *zmax = tmp;}

   /* Find the intervals */
   dx = (*xmax)-(*xmin);
   dy = (*ymax)-(*ymin);
   dz = (*zmax)-(*zmin);
   zsmall = dz < 1e-99;
   ysmall = dy < 1e-99;
   xsmall = dx < 1e-99;
 
   /*EMPTY*/
   if (!xsmall && !ysmall && !zsmall) {
      /* The ranges are all big */
      ;
   } else if (xsmall && ysmall && zsmall) {
      /* All the numbers are very small */
      *xmax = *xmax + INTERV;
      *xmin = *xmin - INTERV;
      *ymax = *ymax + INTERV;
      *ymin = *ymin - INTERV;
      *zmax = *zmax + INTERV;
      *zmin = *zmin - INTERV;
   } else if (xsmall && ysmall && !zsmall) {
      /* Set x,y intervals to the z-interval */
      reset_maxmin(xmin, xmax, dz);
      reset_maxmin(ymin, ymax, dz);
   } else if (xsmall && !ysmall && zsmall) {
      /* Set x,z intervals to the y-interval */
      reset_maxmin(xmin, xmax, dy);
      reset_maxmin(zmin, zmax, dy);
   } else if (!xsmall && ysmall && zsmall) {
      /* Set y,z intervals to the x-interval */
      reset_maxmin(ymin, ymax, dx);
      reset_maxmin(zmin, zmax, dx);
   } else if (xsmall) {
      /* Set x interval the the smaller of dy, dz */
      dl    = SMALLER_OF(dy,dz);
      reset_maxmin(xmin, xmax, dl);
   } else if (ysmall) {
      /* Set y interval the the smaller of dx, dz */
      dl    = SMALLER_OF(dx,dz);
      reset_maxmin(ymin, ymax, dl);
   } else if (zsmall) {
      /* Set z interval the the smaller of dx, dy */
      dl    = SMALLER_OF(dx,dy);
      reset_maxmin(zmin, zmax, dl);
   }
} 


/*
 * Reset the boundaries using dy as a guide
 * Assumes xmin=xmax, dy>0
 */
static void reset_maxmin(xmin, xmax, dy)
double *xmin, *xmax, dy;
{ 
   if (fabs((*xmax)/dy) > 1.0e5) {
      /* Vastly different order of magnitude */
      *xmax = *xmax * 10.0;
      *xmin = *xmin * 0.10;
   } else {
      /* Same order of magnitude */
      *xmax = *xmax + 0.5*dy; 
      *xmin = *xmin - 0.5*dy; 
   }
}


/*
 * Triangulate a polygon
 */
void CNtriangulate_polygon(opointhead, opointtail, 
                           pointhead, pointtail,
                           nodehead,  nodetail,
                           triahead,  triatail, trID)
CNpointptr opointhead, opointtail;
CNpointptr *pointhead, *pointtail;
CNnodeptr  *nodehead,  *nodetail;
CNtriaptr  *triahead,  *triatail;
int        *trID;
{
   int        idtang();
   double     *xarr, *yarr;
   double     xmin, xmax, ymin, ymax, ratio;
   CNnodeptr  *narr;
   CNpointptr P, pt;
   int        npoints;
   int        *ipt, *ipl, nt, nl, i;
   int        p1, p2, p3;

   /* Check the no of points */
   npoints = CNcount_points(opointhead, opointtail);
   if (npoints < 4) {
      (void) fprintf(stderr,
                     "Error - triangulation requires 4 or more points!\n");
      return;
   }

   /* Allocate arrays based on no of points in point list */
   xarr    = (double *)malloc((unsigned int)(npoints*sizeof(double)));
   yarr    = (double *)malloc((unsigned int)(npoints*sizeof(double)));
   narr    = (CNnodeptr  *)malloc((unsigned int)(npoints*sizeof(CNnodeptr)));
   if (!xarr || !yarr || !narr) {
      (void) fprintf(stderr,"Error - insufficent space for work arrays!\n");
      if (xarr) free((char *)xarr);
      if (yarr) free((char *)yarr);
      if (narr) free((char *)narr);
      return;
   }
   
   /* Fill the arrays */
   npoints = 0;
   for (P=opointhead; P!=NULL; P=P->next) {
      if (P==opointhead) {
         xmin = P->x;
         xmax = P->x;
         ymin = P->y;
         ymax = P->y;
      }
      if (P->x < xmin) xmin = P->x;
      if (P->x > xmax) xmax = P->x;
      if (P->y < ymin) ymin = P->y;
      if (P->y > ymax) ymax = P->y;
      xarr[npoints] = P->x;
      yarr[npoints] = P->y;
      npoints++;
   }

   /* Triangulation fails if the x-bounds are greater than y-bounds
    * by several orders of magnitude (or vice versa).  Scale the
    * x,y arrays arrordingly 
    */
   if ((xmin == xmax) || (ymin == ymax)) {
      (void) fprintf(stderr,"***Error! All collinear points!\n");
      (void) fprintf(stderr,"***Error! Triangulation failed!\n");
      if (xarr) free((char *)xarr);
      if (yarr) free((char *)yarr);
      if (narr) free((char *)narr);
      return;
   }
   ratio = fabs((xmax-xmin)/(ymax-ymin));
   if ((ratio > 1.0e3) || (ratio < 1.0e-3)) {
      for (i=0; i<npoints; i++) 
         yarr[i] = yarr[i]*ratio; 
   }

   /* Do the triangulation */
   if (idtang(npoints, xarr, yarr, &nt, &ipt, &nl, &ipl)==0) {
      (void) fprintf(stderr,"   ***Error! Triangulation failed!\n");
   } else {
      /* Create nodes connected to points */
      npoints = 0;
      for (P=opointhead; P!=NULL; P=P->next) {
         pt = CNinsert_point(pointhead,pointtail,P->x,P->y,P->z,P->ID);
         narr[npoints] = CNinsert_tailnode(nodehead,nodetail,pt,P->z,P->ID);
         npoints++;
      }

      /* Create the triangles */
      for (i=1; i<=nt; i++) {
         p1 = ipt[i*3-2];
         p2 = ipt[i*3-1];
         p3 = ipt[i*3];
         if ((p1>=0 && p1<npoints) && 
             (p2>=0 && p2<npoints) && 
             (p3>=0 && p3<npoints))
            (void) CNinsert_tria(triahead,triatail,
                                 narr[p1], narr[p2], narr[p3], 0, (*trID)++);
         else
            (void)fprintf(stderr,"   Warning! Array element out of bounds!\n");
      }
   }
  
   /* Free the arrays */
   if (xarr) free((char *)xarr);
   if (yarr) free((char *)yarr);
   if (narr) free((char *)narr);
   if (ipt ) free((char *)ipt );
   if (ipl ) free((char *)ipl );
}
