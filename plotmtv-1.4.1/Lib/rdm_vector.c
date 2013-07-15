/*
 * rdm_vector.c - routines to read vector data from an MTV-format.
 * 
 * The data is assumed to be sandwiched between $DATA=xxx lines
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "CNplot.h"

int CNmtv_read_vector_data();
static void read_vector_options();
static void vector_itemno_err();

static CNdouble def_mval = {0.0, 0};

/* 
 * Read data in vector format
 * Return an EOF if such is found during the read; otherwise return 0
 */
int CNmtv_read_vector_data(Dptr,
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
   CNvecboxptr        vector_box = NULL;
   CNvecptr           vectorhead = NULL, vectortail = NULL;
   CNannotptr         annothead=NULL, annottail=NULL;
   CNcontstepptr      cstephead=NULL, csteptail=NULL;
   CNviewptr          view_pr;
   CNcurve_property   cv_property;
   CNdataset_property ds_property;
   CNplotset_property pt_property;
   char        line[BUFSIZ];
   int         header_found = CN_FALSE;
   int         invalid_grid = CN_FALSE;
   int         len, ierr=0, vectorID=0, nvectors, npts, i;
   double      xmin, xmax, ymin, ymax, zmin, zmax;
   double      *xarr=NULL, *yarr=NULL, *zarr=NULL;
   double      *vxarr=NULL, *vyarr=NULL, *vzarr=NULL;
   double      x,y,z,vx,vy,vz;
   double      vlen_max, vlen_min;
   double      vscale, vlogscale;
   CNdouble    rnpts; 
   int         binary;

   /* Print info */
   (void) fprintf(stdout,"\n   Reading VECTOR data...(line %d)\n",*lineno);

   /* Initialize */
   *Dptr  = NULL;
   xmin   =  CN_LARGE;
   xmax   = -CN_LARGE;
   ymin   =  CN_LARGE;
   ymax   = -CN_LARGE;
   zmin   =  CN_LARGE;
   zmax   = -CN_LARGE;
   rnpts  = def_mval;

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
         read_vector_options("VECTOR  ",line,
                         view_pr,&pt_property,&ds_property,&cv_property,
                         &rnpts,&binary,verbose);
         if ((ds_property.contours) && ((ds_property.flag1 & CNctrlevel)!=0))
         CNset_contstep_levels(&cstephead, &csteptail, &ds_property);


         /* Binary format - data follows on next line */
         if (binary) {
            /* Set the number of grid points */
            invalid_grid = CNmtv_check_points(&rnpts,&npts,"npts",
                                              "VECTOR  ",*lineno);
            if (!invalid_grid) {

               /* Read the data */
               xarr  = CNmtv_read_binary_dbl_array(fp,npts);
               yarr  = CNmtv_read_binary_dbl_array(fp,npts);
               zarr  = CNmtv_read_binary_dbl_array(fp,npts);
               vxarr = CNmtv_read_binary_dbl_array(fp,npts);
               vyarr = CNmtv_read_binary_dbl_array(fp,npts);
               vzarr = CNmtv_read_binary_dbl_array(fp,npts);
               rnpts= def_mval;

               if (xarr == NULL || yarr == NULL || zarr == NULL || 
                   vxarr== NULL || vyarr== NULL || vzarr== NULL) {
                  CNfree_1D_double_array(xarr);
                  CNfree_1D_double_array(yarr);
                  CNfree_1D_double_array(zarr);
                  CNfree_1D_double_array(vxarr);
                  CNfree_1D_double_array(vyarr);
                  CNfree_1D_double_array(vzarr);
               } else {
                  /* Copy the data into the vector list */
                  for (i=0; i<npts; i++) {
                     x = xarr[i];
                     y = yarr[i];
                     z = zarr[i];
                     vx = vxarr[i];
                     vy = vyarr[i];
                     vz = vzarr[i];

                     /* Get min and max */
                     if (x < xmin) xmin = x;
                     if (x > xmax) xmax = x;
                     if (y < ymin) ymin = y;
                     if (y > ymax) ymax = y;
                     if (z < zmin) zmin = z;
                     if (z > zmax) zmax = z;

                     /* Create a vector */
                     (void) CNinsert_vector(&vectorhead, &vectortail,
                                      x,y,z,vx,vy,vz,vectorID++);
                  }

                  /* Free the arrays */
                  if (xarr != NULL) CNfree_1D_double_array(xarr);
                  if (yarr != NULL) CNfree_1D_double_array(yarr);
                  if (zarr != NULL) CNfree_1D_double_array(zarr);
                  if (vxarr != NULL) CNfree_1D_double_array(vxarr);
                  if (vyarr != NULL) CNfree_1D_double_array(vyarr);
                  if (vzarr != NULL) CNfree_1D_double_array(vzarr);
               }
            }
         }

      } else if (!invalid_grid) {
         /* Must be ascii data */ 

         if (sscanf(line,"%lf %lf %lf %lf %lf %lf",&x,&y,&z,&vx,&vy,&vz) != 6){
            /* Read 6 numbers from the line */
            vector_itemno_err("VECTOR  ",(*lineno),6,ierr++);

         } else {

            /* Get min and max */
            if (x < xmin) xmin = x;
            if (x > xmax) xmax = x;
            if (y < ymin) ymin = y;
            if (y > ymax) ymax = y;
            if (z < zmin) zmin = z;
            if (z > zmax) zmax = z;

            /* Store the vector */
            (void) CNinsert_vector(&vectorhead, &vectortail,
                                   x,y,z,vx,vy,vz,vectorID++);
         }
      }
   }

   /* Go thru the vectors and report the findings */
   nvectors = CNcount_vectors(vectorhead, vectortail);
   if (verbose) (void) fprintf(stdout,"   Read %d vector(s)\n",nvectors);

   /* Put the vectors inside a vector box */
   if (vectorhead != NULL) {

      /* Get the max/min length of the vectors */
      vlen_max = CNmax_vector(vectorhead, vectortail);
      vlen_min = CNmin_vector(vectorhead, vectortail);
      if (verbose) (void) fprintf(stdout,"   Max magnitude = %g\n",vlen_max);
      if (verbose) (void) fprintf(stdout,"   Min magnitude = %g\n",vlen_min);

      /* Figure out the best scale factor */
      vscale    = CNdefault_vector_scale(vectorhead, vectortail,
                                         xmin, xmax,
                                         ymin, ymax,
                                         zmin, zmax,
                                         vlen_max);

      vlogscale = CNdefault_vector_scale(vectorhead, vectortail,
                                         xmin, xmax,
                                         ymin, ymax,
                                         zmin, zmax,
                                         CNveclog10(vlen_max));

      /* Create a vector-box */
      vector_box = CNmake_vectorbox(vectorhead, vectortail,
                                    vlen_min, vlen_max, 0);
      if (vector_box == NULL) {
         (void) fprintf(stderr,"Error! Couldn't create a vector box!\n");
         CNdelete_vector_list(&vectorhead, &vectortail);
      }
   }

   /* Now create the dataset */
   if (vector_box != NULL) {

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
      *Dptr = CNmake_dataset(filename,dataname,CN_VECTOR,
                               xmin,xmax,ymin,ymax,zmin,zmax,
                               xmin,xmax,ymin,ymax,zmin,zmax,*dataID);
      if ((*Dptr) != NULL) (*dataID)++;

      if ((*Dptr) != NULL) {

         (*Dptr)->vecbox = vector_box;
         (*Dptr)->annothead = annothead;
         (*Dptr)->annottail = annottail;

         /* Set the scale-factor */
         (*Dptr)->data_pr.vscale    = vscale;
         (*Dptr)->data_pr.vlogscale = vlogscale;

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

         /* Apply the linecolor/thickness from the curve property */
         if ((cv_property.flag & CNlinetype ) != 0) 
            (*Dptr)->vecbox->linetype  = cv_property.linetype ;
         if ((cv_property.flag & CNlinecolor) != 0) 
            (*Dptr)->vecbox->linecolor = cv_property.linecolor;
         if ((cv_property.flag & CNlinewidth) != 0) 
            (*Dptr)->vecbox->linewidth = cv_property.linewidth;
         if ((cv_property.flag & CNmarktype ) != 0) 
            (*Dptr)->vecbox->marktype  = cv_property.marktype ;
         if ((cv_property.flag & CNmarkcolor) != 0) 
            (*Dptr)->vecbox->markcolor = cv_property.markcolor;

         /* Print out the dataset */
         if (verbose) CNprint_dataset(*Dptr, 0);

#ifdef DEBUG
         /* Print out the vectors */
         CNprint_vector_list((*Dptr)->vectorhead, (*Dptr)->vectortail, 1);
#endif
      }
   }

   /* Failed */
   if ((*Dptr)==NULL) {
      if (vector_box) CNdelete_vectorbox(vector_box);
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
   (void) fprintf(stdout,"   VECTOR   : Read %d vectors\n",nvectors);
   (void) fprintf(stdout,"            : Vector scale factor = %g\n",
                  (*Dptr)->data_pr.vscale);
   } else
   (void) fprintf(stdout,"   VECTOR   : Format error - couldn't read data!\n");

   /* return */
   if (!header_found) 
      return(EOF);
   else    
      return(header_found);
}


/* 
 * Read a line and apply options to a dataset or a curve
 */
static void read_vector_options(format,header,
                                view_pm,pt_prop,ds_prop,cv_prop,
                                rnpts,binary,vbs)
char               *format;
char               *header;
CNviewptr          view_pm;
CNplotset_property *pt_prop;
CNdataset_property *ds_prop;
CNcurve_property   *cv_prop;
CNdouble           *rnpts;
int                *binary;
int                vbs;
{
   char *argtbl[CN_MAXWORDS], *valtbl[CN_MAXWORDS];
   char newheader[CN_MAXCHAR];
   int  nargs = 0, nvals = 0;
   int  argfound, i;

   /* Initialize */
   *binary = CN_FALSE;

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

         if (strcmp(argtbl[i],"npts")==0) {
            rnpts->val = atof(valtbl[i]);
            rnpts->def = CN_TRUE;
            if (vbs) 
            (void)fprintf(stdout,"   %8s: %-14s= %d\n",
                  format,"npts",(int)rnpts->val);

         } else if (strcmp(argtbl[i],"binary")==0) {
            *binary = CN_TRUE;
            if (vbs)
            (void)fprintf(stdout,"   %8s: %-14s= %s\n",format,"Binary","ON");

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
static void vector_itemno_err(format,lineno,itemno,ierr)
char *format;
int  lineno,itemno,ierr;
{
   (void) fprintf(stderr,"   warning (line %3d) %8s: ",lineno,format);
   (void) fprintf(stderr,"need %d items\n",itemno);
}

