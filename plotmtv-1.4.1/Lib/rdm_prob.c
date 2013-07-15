/*
 * rdm_prob.c - routines to read probability data from an MTV-format.
 * 
 * The data is assumed to be sandwiched between $DATA=xxx lines
 */

/*
 * Probability data can be specified in one of 2 ways:
 *   (A) List of unordered data-points
 *          x1
 *          x2
 *          ...
 *          xn
 *       In this case, we would first sort in rank order and then
 *       figure out the cumulative probability using a simple function,
 *       yi = (2i - 1)/(2n)
 *
 *  (B) List of unordered x,y pairs 
 *          x1 y1
 *          x2 y2
 *          ...
 *          xn yn
 *      where xi is the number, and yi is the cumulative probability (0-1)
 *
 * To simplify the checking, use a flag to indicate if the probability is
 * to be read in...
 *
 * Thus the first format looks like
 * $ DATA=PROBABILITY
 *   %read_prob = False
 *   x1
 *   x2...
 *
 * And the second format looks like
 * $ DATA=PROBABILITY
 *   %read_prob = True
 *   x1 y1
 *   x2 y2...
 *   
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "CNplot.h"

int CNmtv_read_probab_data();
static void read_probab_options();
static void probab_itemno_err();

/* 
 * Read data in probability format
 * Return an EOF if such is found during the read; otherwise return 0
 */
int CNmtv_read_probab_data(Dptr,
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
   CNpointptr         P;
   CNcurveptr         curvehead=NULL, curvetail=NULL, Cptr;
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
   int         pointID=1, npoints, i;
   double      xmin, xmax, ymin, ymax, zmin, zmax;
   double      x,y,z;
   int         readprob=CN_FALSE;

   /* Print info */
   (void) fprintf(stdout,"\n   Reading PROBABILITY data...(line %d)\n",*lineno);

   /* Initialize */
   *Dptr  = NULL;
   xmin   =  CN_LARGE;
   xmax   = -CN_LARGE;
   ymin   =  0.0001;
   ymax   =  0.9999;
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
   (void) CNparse_curve_property(&cv_property, "linetype", "0", 0);
   (void) CNparse_curve_property(&cv_property, "markertype", "4", 0);

   /* Allocate a curve data structure */
   Cptr = CNinsert_curve(&curvehead, &curvetail, 0);

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
         read_probab_options("PROBAB  ",line,
                             view_pr,&pt_property,&ds_property,&cv_property,
                             &readprob,verbose);
         if ((ds_property.contours) && ((ds_property.flag1 & CNctrlevel)!=0))
         CNset_contstep_levels(&cstephead, &csteptail, &ds_property);

         /* Apply the curve options to the last curve */
         if (Cptr!=NULL) CNset_curve_property(&(Cptr->curv_pr),&cv_property);

      } else if (!invalid_grid) {
         /* Must be ascii data */ 

         if (readprob) {
            /* Grab 2 numbers from the line */
            if (sscanf(line,"%lf %lf",&x,&y) != 2){
               /* Read 2 numbers from the line */
               probab_itemno_err("PROBAB  ",(*lineno),2,ierr++);
            } else {
               /* Get min and max */
               if (x < xmin) xmin = x;
               if (x > xmax) xmax = x;
                    
               /* Store the number */
               z = 0.0;
               (void) CNinsert_point(&(Cptr->pointhead), &(Cptr->pointtail),
                                   x,y,z,pointID++);
            }
         } else {
            /* Grab 1 numbers from the line */
            if (sscanf(line,"%lf",&x) != 1){
               /* Read 1 numbers from the line */
               probab_itemno_err("PROBAB  ",(*lineno),1,ierr++);
            } else {
               /* Get min and max */
               if (x < xmin) xmin = x;
               if (x > xmax) xmax = x;
                    
               /* Store the number */
               y = 0.0;
               z = 0.0;
               (void) CNinsert_point(&(Cptr->pointhead), &(Cptr->pointtail),
                                   x,y,z,pointID++);
            }
         }
      }
   }

   /* Go thru the points and report the findings */
   npoints = CNcount_points(Cptr->pointhead, Cptr->pointtail);
   if (verbose) (void) fprintf(stdout,"   Read %d point(s)\n",npoints);


   /* If readprob=False then we need to order the points */
   if (!readprob && (npoints > 0)) {
      if (verbose) (void) fprintf(stdout,"Sorting the points...\n");
      CNdo_quick_sort_xpoints(&(Cptr->pointhead), &(Cptr->pointtail));
      /* Calculate the y-values */
      i = 1;
      for (P=Cptr->pointhead; P!=NULL; P=P->next) {
         P->y  = (2*i - 1)/(double)(2*npoints); 
         P->ID = i;
         i++;
      }
   }

   /* Print out the points */
#ifdef DEBUG
   CNprint_curve(Cptr, 1);
#endif

   /* Now create the dataset */
   if (curvehead!=NULL && curvehead->pointhead!=NULL) {

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
      *Dptr = CNmake_dataset(filename,dataname,CN_PROBAB,
                               xmin,xmax,ymin,ymax,zmin,zmax,
                               xmin,xmax,ymin,ymax,zmin,zmax,*dataID);
      if ((*Dptr) != NULL) (*dataID)++;

      if ((*Dptr) != NULL) {

         (*Dptr)->curvehead = curvehead;
         (*Dptr)->curvetail = curvetail;
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

      }
   }

   /* Failed */
   if ((*Dptr)==NULL) {
      CNdelete_curve_list(&curvehead, &curvetail);
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
   (void) fprintf(stdout,"   PROBAB   : Read %d points\n",npoints);
   } else
   (void) fprintf(stdout,"   PROBAB   : Format error - couldn't read data!\n");

   /* return */
   if (!header_found) 
      return(EOF);
   else    
      return(header_found);
}


/* 
 * Read a line and apply options to a dataset or a curve
 */
static void read_probab_options(format,header,
                                view_pm,pt_prop,ds_prop,cv_prop,
                                readprob,vbs)
char               *format;
char               *header;
CNviewptr          view_pm;
CNplotset_property *pt_prop;
CNdataset_property *ds_prop;
CNcurve_property   *cv_prop;
int                *readprob;
int                vbs;
{
   char *argtbl[CN_MAXWORDS], *valtbl[CN_MAXWORDS];
   char newheader[CN_MAXCHAR];
   int  nargs = 0, nvals = 0;
   int  argfound, i;
   short sval;

   /* CNparse_line wants "command arg=val arg=val" so create a new header */
   (void) sprintf(newheader, "datafile %s",header);

   /* Get the argument-value pairs from the line */
   if (CNparse_line(newheader, CN_MAXCHAR,
                    &nargs, argtbl, CN_MAXWORDS,
                    &nvals, valtbl, CN_MAXWORDS)) {

      /* Go thru the arguments and find specialized matches */
      i = 0;
      while (i < nargs) {

         /* Go thru the arguments and find a match */
         argfound = CN_TRUE;

         if ((strcmp(argtbl[i],"readprob")==0) ||
             (strcmp(argtbl[i],"read_prob")==0) ) {
            CNassign_boolean_keyword(&sval,valtbl[i],"readprob",0);
            *readprob = sval;
            if (vbs)
            (void)fprintf(stdout,"   %8s: %-14s= %s\n",format, "readprob",
                          BOOLEAN_VALUE(readprob));

         } else {
            argfound = CN_FALSE;
         }

         /* Reset the table if a match was found; otherwise increment index */
         CNdownshift(argfound,&i,argtbl,valtbl,&nargs,&nvals);
      }

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
static void probab_itemno_err(format,lineno,itemno,ierr)
char *format;
int  lineno,itemno,ierr;
{
   (void) fprintf(stderr,"   warning (line %3d) %8s: ",lineno,format);
   (void) fprintf(stderr,"need %d items\n",itemno);
}

