/*
 * rdm_histogram.c - routines to read histogram data from an MTV-format.
 * 
 * The data is assumed to be sandwiched between $DATA=xxx lines
 */

/*
 * Histogram data is specified as a list of unordered x points, i.e.
 *          x1
 *          x2 
 *          ...
 *          xn 
 *
 * The user also specifies bin-sizes and bin-locations which are used to
 * sort out the data into boxes.
 *
 * Thus the format looks like
 * $ DATA=HISTOGRAM
 *   % bin_size = 0.3  
 *   % bin_start= 0.1  
 *   x1 
 *   x2 ...
 *   
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "CNplot.h"

int CNmtv_read_histogram_data();
static void read_histogram_options();
static void histogram_itemno_err();

/* 
 * Read data in histogram format
 * Return an EOF if such is found during the read; otherwise return 0
 */
int CNmtv_read_histogram_data(Dptr,
                           dataID,dataname,filename,fp,lineno,header,verbose)
CNdatasetptr *Dptr;          /* The dataset pointer                  */
int          *dataID;        /* Data ID                              */
char         *dataname;      /* Dataset name                         */
char         *filename;      /* The name of the file/pipe source     */
FILE         *fp;            /* The file/pipe source                 */
int          *lineno;        /* Current line number                  */
char         *header;        /* Header string                        */
int          verbose;        /* Verbosity Flag                       */
{
   CNhistogramptr     histogram=NULL;
   CNpointptr         pointhead=NULL, pointtail=NULL;
   CNannotptr         annothead=NULL, annottail=NULL;
   CNcontstepptr      cstephead=NULL, csteptail=NULL;
   CNviewptr          view_pr;
   CNcurve_property   cv_property;
   CNdataset_property ds_property;
   CNplotset_property pt_property;
   char        line[BUFSIZ];
   int         header_found = CN_FALSE;
   int         invalid_grid = CN_FALSE;
   int         len, ierr=0;
   int         pointID=1, npoints;
   double      xmin, xmax, ymin, ymax, zmin, zmax;
   double      x,y,z;

   /* Print info */
   (void) fprintf(stdout,"\n   Reading HISTOGRAM data...(line %d)\n",*lineno);

   /* Initialize */
   *Dptr  = NULL;
   xmin   =  CN_LARGE;
   xmax   = -CN_LARGE;
   ymin   = -0.5;
   ymax   =  0.5;
   zmin   = -0.5;
   zmax   =  0.5;

   /* Set the view parameters */
   view_pr = CNcreate_view();

   /* Set the properties of the plotset */
   CNset_default_plotset_property(&pt_property);

   /* Set the properties of the dataset */
   CNset_default_dataset_property(&ds_property);

   /* Set the properties of the curve */
   CNset_default_curve_property(&cv_property);

   /* Keep on reading until a "$" is encountered */
   while (!header_found && CNgetucline(fp, line, lineno, BUFSIZ) != EOF) {

      /*EMPTY*/
      if (((len=strlen(line)) == 0) || (len==1 && line[0]=='\n')) {
         /* Blank line - ignore */
         ;

      /*EMPTY*/
      } else if (line[0] == '#') {
         /* Comment */
         ;

      } else if (line[0] == '$') {
         /* Header */
         line[0] = ' ';
         (void) strcpy(header, line);
         header_found = CN_TRUE;

      } else if (line[0] == '@') {
         /* Annotation */
         line[0] = ' ';
         CNparse_annotation_line(line, BUFSIZ,
                                 &annothead, &annottail,
                                 verbose);

      } else if (line[0] == '%') {
         /* Option  */
         line[0] = ' ';
         read_histogram_options("HISTOG  ",line,
                             view_pr,&pt_property,&ds_property,&cv_property,
                             verbose);
         if ((ds_property.contours) && ((ds_property.flag1 & CNctrlevel)!=0))
         CNset_contstep_levels(&cstephead, &csteptail, &ds_property);

      } else if (!invalid_grid) {
         /* Must be ascii data */ 

         /* Grab 1 number from the line */
         if (sscanf(line,"%lf",&x) != 1){
            /* Read 1 numbers from the line */
            histogram_itemno_err("HISTOG  ",(*lineno),1,ierr++);
         } else {
            /* Get min and max */
            if (x < xmin) xmin = x;
            if (x > xmax) xmax = x;
                    
            /* Store the number */
            y = 0.0;
            z = 0.0;
            (void) CNinsert_point(&pointhead, &pointtail,
                                x,y,z,pointID++);
         }
      }
   }

   /* Go thru the points and report the findings */
   npoints = CNcount_points(pointhead, pointtail);
   if (verbose) (void) fprintf(stdout,"   Read %d point(s)\n",npoints);

   /* Put the points inside a histogram structure */
   if (pointhead != NULL) {

      /* Create a histogram */
      histogram = CNmake_histogram(xmin, xmax, pointhead, pointtail);
      if (histogram == NULL) {
         (void) fprintf(stderr,"Error! Couldn't create histogram!\n");
         CNdelete_point_list(&pointhead, &pointtail);
      }

      /* Apply curve properties to the histogram */
      if (histogram != NULL) {
         if ((cv_property.flag & CNfillcolor) != 0) 
            histogram->fillcolor = cv_property.fillcolor;
         if ((cv_property.flag & CNfilltype) != 0) 
            histogram->filltype  = cv_property.filltype ;
      }
   }


   /* Now create the dataset */
   if (histogram != NULL) {

      /* 
       * If the plot-boundary has been set in the options then
       * use that as the true plot boundary
       */
      if ((pt_property.flag1 & CNvxmin) != 0) xmin = pt_property.vxmin;
      if ((pt_property.flag1 & CNvymin) != 0) ymin = pt_property.vymin;
      if ((pt_property.flag1 & CNvzmin) != 0) zmin = pt_property.vzmin;
      if ((pt_property.flag1 & CNvxmax) != 0) xmax = pt_property.vxmax;
      if ((pt_property.flag1 & CNvymax) != 0) ymax = pt_property.vymax;
      if ((pt_property.flag1 & CNvzmax) != 0) zmax = pt_property.vzmax;

      /* Store the curves in a dataset */
      *Dptr = CNmake_dataset(filename,dataname,CN_HISTOGRAM,
                               xmin,xmax,ymin,ymax,zmin,zmax,
                               xmin,xmax,ymin,ymax,zmin,zmax,*dataID);
      if ((*Dptr) != NULL) (*dataID)++;

      if ((*Dptr) != NULL) {

         (*Dptr)->histogram = histogram;
         (*Dptr)->annothead = annothead;
         (*Dptr)->annottail = annottail;

         /* Apply the view options to the dataset */
         CNset_view_property((*Dptr)->view_pr,view_pr);

         /* Apply the plotset options to the dataset */
         CNset_plotset_property(&((*Dptr)->plot_pr),&pt_property);

         /* Apply the dataset options to the dataset */
         CNset_dataset_property(&((*Dptr)->data_pr),&ds_property);

         /* Apply the contsteps */
         if (cstephead != NULL) {
         (*Dptr)->cstephead = cstephead;
         (*Dptr)->csteptail = csteptail;
         }

         /* Print out the dataset */
         if (verbose) CNprint_dataset(*Dptr, 0);

         /* Generate curves of the histogram */
         CNsort_histogram(*Dptr, 0);

#ifdef DEBUG
         /* Print out the points */
         CNprint_histogram(histogram);
#endif
      }
   }

   /* Failed */
   if ((*Dptr)==NULL) {
      CNdelete_point_list(&pointhead, &pointtail);
      CNdelete_contstep_list(&cstephead, &csteptail);
      CNdelete_annotation_list(&annothead, &annottail);
   }

   /* Reset the property structures */
   CNdelete_plotset_property_fields(&pt_property);
   CNdelete_dataset_property_fields(&ds_property);
   CNdelete_curve_property_fields(&cv_property);

   /* Free the view structure */
   CNdelete_view(view_pr);

   /* Print out info */
   if ((*Dptr) != NULL) {
   (void) fprintf(stdout,"   HISTOG   : Read %d points\n",npoints);
   } else
   (void) fprintf(stdout,"   HISTOG   : Format error - couldn't read data!\n");

   /* return */
   if (!header_found) 
      return(EOF);
   else    
      return(header_found);
}


/* 
 * Read a line and apply options to a dataset or a curve
 */
/*ARGSUSED*/
static void read_histogram_options(format,header,
                                view_pm,pt_prop,ds_prop,cv_prop,
                                vbs)
char               *format;
char               *header;
CNviewptr          view_pm;
CNplotset_property *pt_prop;
CNdataset_property *ds_prop;
CNcurve_property   *cv_prop;
int                vbs;
{
   char *argtbl[CN_MAXWORDS], *valtbl[CN_MAXWORDS];
   char newheader[CN_MAXCHAR];
   int  nargs = 0, nvals = 0;
   int  i;

   /* CNparse_line wants "command arg=val arg=val" so create a new header */
   (void) sprintf(newheader, "datafile %s",header);

   /* Get the argument-value pairs from the line */
   if (CNparse_line(newheader, CN_MAXCHAR,
                    &nargs, argtbl, CN_MAXWORDS,
                    &nvals, valtbl, CN_MAXWORDS)) {

      /* Look for plotset/dataset/curve arguments */
      for (i=0; i<nargs; i++) {
         if (!CNparse_view_property       (view_pm,argtbl[i],valtbl[i],vbs))
           if (!CNparse_plotset_property  (pt_prop,argtbl[i],valtbl[i],vbs))
             if (!CNparse_dataset_property(ds_prop,argtbl[i],valtbl[i],vbs))
               if (!CNparse_curve_property(cv_prop,argtbl[i],valtbl[i],vbs))
                 (void) fprintf(stderr,
                                "   warning : Invalid option \"%s=%s\"\n",
                                argtbl[i],valtbl[i]);
      }

      /* Clear the tables */
      CNfreewords(&nargs, argtbl);
      CNfreewords(&nvals, valtbl);
   }
}


/*
 * Print item-number error
 */
/*ARGSUSED*/
static void histogram_itemno_err(format,lineno,itemno,ierr)
char *format;
int  lineno,itemno,ierr;
{
   (void) fprintf(stderr,"   warning (line %3d) %8s: ",lineno,format);
   (void) fprintf(stderr,"need %d items\n",itemno);
}

