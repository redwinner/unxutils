/*
 * rdm_curve.c - routines to read 2D/3D curve data from an MTV-format.
 *
 * The data is assumed to be sandwiched between $DATA=xxx lines
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "CNplot.h"

#define BOUNDARY 0.025

int         CNmtv_read_curve2D_data();
int         CNmtv_read_curve3D_data();
int         CNmtv_read_contcurve_data();
int         CNmtv_read_curve_data();
static int  read_curve_data();
static void read_cv_options();
static void curve_itemno_err();

static CNdouble def_mval = {0.0, 0};

/* 
 * Read data in 2D format
 * Return an EOF if such is found during the read; otherwise return 0
 */
int CNmtv_read_curve2D_data(Dptr,
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
   int status;
   status = CNmtv_read_curve_data(CN_FALSE,
                                  Dptr,dataID,dataname,
                                  filename,fp,lineno,header,verbose);
   return(status);
}

/* 
 * Read data in 3D format
 * Return an EOF if such is found during the read; otherwise return 0
 */
int CNmtv_read_curve3D_data(Dptr,
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
   int status;
   status = CNmtv_read_curve_data(CN_TRUE,
                                  Dptr,dataID,dataname,
                                  filename,fp,lineno,header,verbose);
   return(status);
}

/* 
 * Read data in 3D-contour format
 * Return an EOF if such is found during the read; otherwise return 0
 */
int CNmtv_read_contcurve_data(Dptr,
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
   CNcontstepptr      cstephead=NULL, csteptail=NULL;
   CNviewptr          view_pr;
   CNcurve_property   cv_property;
   CNdataset_property ds_property;
   CNplotset_property pt_property;
   CNcurveptr  curvehead=NULL, curvetail=NULL, C;
   CNannotptr  annothead=NULL, annottail=NULL;
   CNpointptr  pointhead=NULL, pointtail=NULL, P, pt;
   CNnodeptr   nodehead =NULL, nodetail =NULL, nd0, nd1, nd2;
   CNtriaptr   triahead =NULL, triatail =NULL;
   int         ndID=0, trID=0, npoints;
   int         header_found = CN_FALSE;
   int         boundary = CN_FALSE;
   double      xmin, xmax, ymin, ymax, zmin, zmax;

   /* Print info */
   (void) fprintf(stdout,"\n   Reading %s data...(line %d)\n",
                  "CONTCURVE", *lineno); 

   /* Initialize */
   *Dptr = NULL;

   /* Set the view parameters */
   view_pr = CNcreate_view();

   /* Set the properties of the plotset */
   CNset_default_plotset_property(&pt_property);

   /* Set the properties of the dataset */
   CNset_default_dataset_property(&ds_property);

   /* Set the properties of the curve */
   CNset_default_curve_property(&cv_property);

   /* Read curve data */
   header_found = read_curve_data(fp,lineno,header,CN_TRUE,
                                  "CONTCURVE",
                                  &curvehead, &curvetail,
                                  &xmin, &xmax, &ymin, &ymax, &zmin, &zmax,
                                  &annothead, &annottail,
                                  view_pr, &pt_property,
                                  &ds_property, &cv_property,
                                  &cstephead, &csteptail,
                                  &boundary,
                                  verbose);

   /* Go thru the curves and convert them to triangles/rectangles */
   for (C=curvehead; C!=NULL; C=C->next) {
      npoints = CNcount_points(C->pointhead,C->pointtail);
      if (npoints == 3) {
         P    = C->pointhead;
         pt   = CNinsert_point(&pointhead,&pointtail,P->x,P->y,P->z,P->ID);
         nd0  = CNinsert_tailnode(&nodehead,&nodetail,pt,P->z,ndID++);
         P    = P->next;
         pt   = CNinsert_point(&pointhead,&pointtail,P->x,P->y,P->z,P->ID);
         nd1  = CNinsert_tailnode(&nodehead,&nodetail,pt,P->z,ndID++);
         P    = P->next;
         pt   = CNinsert_point(&pointhead,&pointtail,P->x,P->y,P->z,P->ID);
         nd2  = CNinsert_tailnode(&nodehead,&nodetail,pt,P->z,ndID++);
         (void)CNinsert_tria(&triahead,&triatail,nd0,nd1,nd2,0,trID++);
      } else if (npoints >=4) {
         CNtriangulate_polygon(C->pointhead, C->pointtail, 
                               &pointhead, &pointtail,
                               &nodehead, &nodetail, 
                               &triahead, &triatail, &trID);
      } else {
         (void) fprintf(stdout,"Warning! Cannot contour %d-sided polygon\n",
                        npoints);
      }
   }

   /* Delete all the curves */
   CNdelete_curve_list(&curvehead, &curvetail);

   /* slice and dice the triangles and place results in a linked list */
   if (triahead!=NULL) {
      *Dptr = CNget_triangular_contour_data(filename,dataname,
                                            xmin,xmax,ymin,ymax,zmin,zmax,
                                            triahead,triatail, (*dataID));

      if ((*Dptr) != NULL) (*dataID)++;

      if ((*Dptr) != NULL) {
         /* Save the point, nodelist */
         (*Dptr)->pointhead = pointhead;
         (*Dptr)->pointtail = pointtail;
         (*Dptr)->nodehead  = nodehead;
         (*Dptr)->nodetail  = nodetail;

         /* Save the annotations */
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

         /* Slice the contours */
         CNslice_contours((*Dptr),verbose);

         /* Print out the curve */
         if (verbose) CNprint_dataset((*Dptr),0);
      }
   }

   /* Failed */
   if ((*Dptr)==NULL) {
      CNdelete_tria_list (&triahead , &triatail);
      CNdelete_node_list (&nodehead , &nodetail);
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
   if ((*Dptr) != NULL)
   (void) fprintf(stdout,"   %8s : Read %d triangles\n",
                  "CONTCURV", trID);
   else
   (void) fprintf(stdout,"   %8s : Format error - couldn't read data!\n",
                  "CONTCURV");
 
   /* return */
   if (!header_found)
      return(EOF);
   else
      return(header_found);
}


/* 
 * Read data in 2D/3D format
 * Return an EOF if such is found during the read; otherwise return 0
 */
int CNmtv_read_curve_data(threed,
                          Dptr,
                          dataID,dataname,filename,fp,lineno,header,verbose)
int          threed;         /* 3D data flag                         */
CNdatasetptr *Dptr;          /* The dataset pointer                  */
int          *dataID;        /* Data ID                              */
char         *dataname;      /* Dataset name                         */
char         *filename;      /* The name of the file/pipe source     */
FILE         *fp;            /* The file/pipe source                 */
int          *lineno;        /* Current line number                  */
char         *header;        /* Header string                        */
int          verbose;        /* Verbosity Flag                       */
{
   CNcontstepptr      cstephead=NULL, csteptail=NULL;
   CNviewptr          view_pr;
   CNcurve_property   cv_property;
   CNdataset_property ds_property;
   CNplotset_property pt_property;
   CNcurveptr  curvehead=NULL, curvetail=NULL;
   CNannotptr  annothead=NULL, annottail=NULL;
   int         header_found = CN_FALSE;
   int         boundary = CN_FALSE;
   double      xmin, xmax, ymin, ymax, zmin, zmax;
   double      delta;

   /* Print info */
   (void) fprintf(stdout,"\n   Reading %s data...(line %d)\n",
                  ((threed) ? "CURVE3D " : "CURVE2D "), *lineno); 

   /* Initialize */
   *Dptr = NULL;

   /* Set the view parameters */
   view_pr = CNcreate_view();

   /* Set the properties of the plotset */
   CNset_default_plotset_property(&pt_property);

   /* Set the properties of the dataset */
   CNset_default_dataset_property(&ds_property);

   /* Set the properties of the curve */
   CNset_default_curve_property(&cv_property);

   /* Read curve data */
   header_found = read_curve_data(fp,lineno,header,threed,
                                  ((threed) ? "CURVE3D " : "CURVE2D "),
                                  &curvehead, &curvetail,
                                  &xmin, &xmax, &ymin, &ymax, &zmin, &zmax,
                                  &annothead, &annottail,
                                  view_pr, &pt_property,
                                  &ds_property, &cv_property,
                                  &cstephead, &csteptail,
                                  &boundary,
                                  verbose);


   /* Now create the dataset */
   if (curvehead != NULL) {
      /* Adjust xmin, xmax, ymin, ymax, zmin, zmax */
      CNmtv_adjust_boundaries(&xmin, &xmax, &ymin, &ymax, &zmin, &zmax);

      /* Provide a boundary layer */
      if (boundary) {
         delta = xmax - xmin;
         xmin -= BOUNDARY*delta;
         xmax += BOUNDARY*delta;
         delta = ymax - ymin;
         ymin -= BOUNDARY*delta;
         ymax += BOUNDARY*delta;
         delta = zmax - zmin;
         zmin -= BOUNDARY*delta;
         zmax += BOUNDARY*delta;
      }
      
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
      *Dptr = CNmake_dataset(filename,dataname,
                             ((threed) ? CN_PLOT3D : CN_PLOT2D),
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

#ifdef DEBUG
         /* Print out the curves */
         CNprint_curve_list((*Dptr)->curvehead, (*Dptr)->curvetail, 1);
#endif

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
   if ((*Dptr) != NULL)
   (void) fprintf(stdout,"   %8s : Read %d curves\n",
                  ((threed) ? "CURVE3D " : "CURVE2D "), 
                  CNcount_curves((*Dptr)->curvehead, (*Dptr)->curvetail));
   else
   (void) fprintf(stdout,"   %8s : Format error - couldn't read data!\n",
                  ((threed) ? "CURVE3D " : "CURVE2D "));

   /* return */
   if (!header_found) 
      return(EOF);
   else    
      return(header_found);
}

/*
 * Read data in 2D/3D format
 * Return an EOF if such is found during the read; otherwise return 0
 */
static int read_curve_data(fp,lineno,header,
                           threed, format,
                           curvehead, curvetail,
                           xmin, xmax, ymin, ymax, zmin, zmax,
                           annothead, annottail,
                           view_pr, pt_property,
                           ds_property, cv_property,
                           cstephead, csteptail,
                           boundary,
                           verbose)
FILE         *fp;                              /* The file/pipe source */
int          *lineno;                          /* Current line number  */
char         *header;                          /* Header string        */
int          threed;                           /* Flag for 3D data     */
char         *format;                          /* Format for 2D/3D data*/
CNcurveptr   *curvehead, *curvetail;           /* Curve list           */
double       *xmin, *xmax;                     /* x-limits             */
double       *ymin, *ymax;                     /* y-limits             */
double       *zmin, *zmax;                     /* z-limits             */
CNannotptr   *annothead, *annottail;           /* Annotations          */
CNviewptr          view_pr;                    /* View parameters      */
CNplotset_property *pt_property;               /* Plot property        */
CNdataset_property *ds_property;               /* Data property        */
CNcurve_property   *cv_property;               /* Curve property       */
CNcontstepptr      *cstephead, *csteptail;     /* Contour step list    */
int          *boundary;                        /* Add boundary flag    */
int          verbose;                          /* Verbosity Flag       */
{
   CNgbcurve_property gb_property;             /* GlobalCurve property */

   CNcurveptr  Cptr=NULL;
   double      *xarr=NULL, *yarr=NULL, *zarr=NULL;
   int         len, ierr=0, curveID=0, ptID=0, ncurves, npts, i, n, ID;
   char        line[BUFSIZ];
   int         header_found = CN_FALSE;
   int         newcurve     = CN_TRUE;
   int         invalid_grid = CN_FALSE;
   int         read_err     = CN_FALSE;
   double      x,y,z=0.0;
   CNdouble    rnpts;
   CNdouble    curveNo;
   int         ID_FOUND = CN_FALSE;
   int         binary;

   /* Initialize */
   *xmin =  CN_LARGE;
   *xmax = -CN_LARGE;
   *ymin =  CN_LARGE;
   *ymax = -CN_LARGE;
   *zmin =  CN_LARGE;
   *zmax = -CN_LARGE;
   rnpts = def_mval;
   curveNo = def_mval;

   /* Set the global properties of the curve */
   CNset_default_gbcurve_property(&gb_property);

   /* Keep on reading until a "$" is encountered */
   while (!header_found && CNgetucline(fp, line, lineno, BUFSIZ) != EOF) {

      if (((len=strlen(line)) == 0) || (len==1 && line[0]=='\n')) {

         /* Apply the curve options to the old curve */
         if (Cptr!=NULL) CNset_curve_property(&(Cptr->curv_pr),cv_property);

         /* If the curve ID has been specified, use it */
         if (Cptr && curveNo.def) Cptr->ID = (int) curveNo.val;

         /* New curve */
         newcurve = CN_TRUE;
         Cptr     = NULL;
         ptID     = 0;
         curveNo  = def_mval;
         CNdelete_curve_property_fields(cv_property);
         CNset_default_curve_property(cv_property);

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
                                 annothead, annottail,
                                 verbose);

      } else if (line[0] == '%') {
         /* Option  */
         line[0] = ' ';
         read_cv_options(format,line,
                         view_pr,pt_property,ds_property,cv_property,
                         &gb_property,
                         &rnpts,&binary,boundary,&curveNo,verbose);
         if ((ds_property->contours) && ((ds_property->flag1 & CNctrlevel)!=0))
         CNset_contstep_levels(cstephead, csteptail, ds_property);

         /* Binary format - data follows on next line */
         if (binary) {
            /* Set the number of grid points */
            invalid_grid = CNmtv_check_points(&rnpts,&npts,"npts",
                                              format,*lineno);
            if (!invalid_grid) {

               /* Read the data */
               xarr = CNmtv_read_binary_dbl_array(fp,npts);
               yarr = CNmtv_read_binary_dbl_array(fp,npts);
               if (threed) 
               zarr = CNmtv_read_binary_dbl_array(fp,npts);
               rnpts= def_mval;
               if (xarr == NULL || yarr == NULL || (threed && zarr==NULL)) {
                  CNfree_1D_double_array(xarr);
                  CNfree_1D_double_array(yarr);
                  if (threed)
                  CNfree_1D_double_array(zarr);
               } else {
                  /* Save the data */

                  if (newcurve) {
                     /* Allocate a curve data structure */
                     Cptr = CNinsert_curve(curvehead, curvetail, curveID++);
 
                     /* Start collecting points in this curve */
                     newcurve = CN_FALSE;
                  }

                  if (Cptr!=NULL) {
                     for (i=0; i<npts; i++) {
                        /* Insert the point */
                        x = xarr[i];
                        y = yarr[i];
                        z = (threed) ? zarr[i] : 0.0;
                        if (x < *xmin) *xmin = x;
                        if (x > *xmax) *xmax = x;
                        if (y < *ymin) *ymin = y;
                        if (y > *ymax) *ymax = y;
                        if (z < *zmin) *zmin = z;
                        if (z > *zmax) *zmax = z;
                        (void) CNinsert_point(&(Cptr->pointhead),
                                              &(Cptr->pointtail),
                                              x,y,z,i);
                     }
                  }
               }
               /* Free the arrays */
               if (xarr != NULL) CNfree_1D_double_array(xarr);
               if (yarr != NULL) CNfree_1D_double_array(yarr);
               if (zarr != NULL) CNfree_1D_double_array(zarr);
            }
         }

      } else if (!invalid_grid) {
         /* Must be ascii data */ 

         /* Read the data */
         read_err = CN_TRUE;
         ID_FOUND = CN_FALSE;
         if (threed) {
            /* Read 3 numbers plus optional ID from the line */
            if ((n=sscanf(line,"%lf %lf %lf %d",&x,&y,&z,&ID)) != 3 && n!=4){
               curve_itemno_err(format,(*lineno),3,ierr++);
            } else {
               if (n==4) ID_FOUND=CN_TRUE;
               read_err = CN_FALSE;
            }
         } else {
            /* Read 2 numbers plus optional ID from the line */
            if ((n=sscanf(line,"%lf %lf %d",&x,&y,&ID)) != 2 && n!=3){
               curve_itemno_err(format,(*lineno),2,ierr++);
            } else {
               if (n==3) ID_FOUND=CN_TRUE;
               read_err = CN_FALSE;
            }
         }

         if (!read_err) {
            if (newcurve) {
               /* Allocate a curve data structure */
               Cptr = CNinsert_curve(curvehead, curvetail, curveID++);

               /* Start collecting points in this curve */
               newcurve = CN_FALSE;
            }
            if (Cptr != NULL) {
               /* Insert the point */
               if (x < *xmin) *xmin = x;
               if (x > *xmax) *xmax = x;
               if (y < *ymin) *ymin = y;
               if (y > *ymax) *ymax = y;
               if (z < *zmin) *zmin = z;
               if (z > *zmax) *zmax = z;
               (void) CNinsert_point(&(Cptr->pointhead),&(Cptr->pointtail),
                                     x,y,z,((ID_FOUND) ? ID : (ptID++)));
            }
         }
      }
   }

   /* Apply the curve options to the last curve */
   if (Cptr!=NULL) CNset_curve_property(&(Cptr->curv_pr),cv_property);

   /* If the curve ID has been specified, use it */
   if (Cptr && curveNo.def) Cptr->ID = (int) curveNo.val;

   /* Go thru the curves and report the findings */
   ncurves = CNcount_curves(*curvehead, *curvetail);
   if (verbose) {
      (void) fprintf(stdout,"   Read %d curve(s)\n",ncurves);
      for (Cptr=(*curvehead); Cptr!= NULL; Cptr=Cptr->next) {
         npts = CNcount_points(Cptr->pointhead, Cptr->pointtail);
         (void) fprintf(stdout,"      Curve #%2d : %4d points\n",Cptr->ID,npts);
      }
   }

   /* Apply the global-curve dataset properties to the individual curves */
   if (gb_property.flag != 0)
   CNreset_curves(*curvehead, *curvetail, &gb_property, 0);

   /* Reset the property structures */
   CNdelete_gbcurve_property_fields(&gb_property);

   /* return */
   if (!header_found)
      return(EOF);
   else
      return(header_found);
}



/*
 * READING UTILITIES FOR 2D/3D CURVES
 */

/* 
 * Read a line and apply options to a dataset or a curve
 */
static void read_cv_options(format,header,
                            view_pm,pt_prop,ds_prop,cv_prop,
                            gb_prop,rnpts,binary,boundary,curveNo,vbs)
char               *format;
char               *header;
CNviewptr          view_pm;
CNplotset_property *pt_prop;
CNdataset_property *ds_prop;
CNcurve_property   *cv_prop;
CNgbcurve_property *gb_prop;
CNdouble           *rnpts;
int                *binary;
int                *boundary;
CNdouble           *curveNo;
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

         } else if (strcmp(argtbl[i],"boundary")==0) {
            *boundary = CN_TRUE;
            if (vbs)
            (void)fprintf(stdout,"   %8s: %-14s= %s\n",format,"Boundary","ON");

         } else if (strcmp(argtbl[i],"curveno")==0) {
            curveNo->val = atof(valtbl[i]);
            curveNo->def = CN_TRUE;
            if (vbs) 
            (void)fprintf(stdout,"   %8s: %-14s= %d\n",
                  format,"curveno",(int)rnpts->val);

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
               if (!CNparse_gbcurve_property(gb_prop,argtbl[i],valtbl[i],vbs))
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
static void curve_itemno_err(format,lineno,itemno,ierr)
char *format;
int  lineno,itemno,ierr;
{
   (void) fprintf(stderr,"   warning (line %3d) %8s: ",lineno,format);
   (void) fprintf(stderr,"need %d items\n",itemno);
}

