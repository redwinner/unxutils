/*
 * rdm_polygon.c - routines to read polygon data from an MTV-format.
 *                 This routine is similar to the MESH4D format
 *
 * The data is assumed to be sandwiched between $DATA=xxx lines
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "CNplot.h"

int CNmtv_read_polygon_data();
static void read_polygon_options();

static void ugh();
static void itemno_err();
static void read_array();
static void fill_array();
static void check_array();
static CNregionptr read_region();
static CNregionptr insert_region();
static void        insert_poly();

#define NO_DATA     0
#define PCOORD_DATA 1

/*
 * The POLY format represents polygons as
 * points associated with regions. The polygons contains
 * x,y,z coordinate data as well as "t" data.
 *   p npoints mat 
 *   [x1 y1 z1 t1]
 *   [x2 y2 z2 t2]
 *   ..
 *   [xn yn zn tn]
 *   r regID material-name color nocont-flag noplot-flag
 *
 * The POLY format is often derived from the MESH4D format
 */


/* 
 * Read data in polygon format
 * Return an EOF if such is found during the read; otherwise return 0
 */
int CNmtv_read_polygon_data(Dptr,
                            dataID,dataname,filename,fp,lineno,header,verbose)
CNdatasetptr *Dptr;          /* Dataset pointer                      */
int          *dataID;        /* Data ID                              */
char         *dataname;      /* Dataset name                         */
char         *filename;      /* The name of the file/pipe source     */
FILE         *fp;            /* The file/pipe source                 */
int          *lineno;        /* Current line number                  */
char         *header;        /* Header string                        */
int          verbose;        /* Verbosity Flag                       */
{
   CNannotptr         annothead=NULL, annottail=NULL;
   CNcontstepptr      cstephead=NULL, csteptail=NULL;
   CNregionptr        regionhead=NULL, regiontail=NULL;
   CNpolyptr          polyhead=NULL, polytail=NULL;
   CNnodeptr          nodehead=NULL, nodetail=NULL;
   CNpointptr         pointhead=NULL,pointtail=NULL;
   CNviewptr          view_pr;
   CNdataset_property ds_property;
   CNplotset_property pt_property;
   char        line[BUFSIZ];
   int         header_found  = CN_FALSE;
   int         data_type;
   int         len;
   int         nregions;
   int         npolys=0, nnodes=0, npoints=0;
   int         pcount;
   int         npts=0;
   int         regID;
   int         ierr, err;
   double      *poly_arr=NULL;
   double      xmin, xmax, ymin, ymax, zmin, zmax, tmin, tmax;

   /* Initialize */
   ierr        = 0;
   pcount      = 0;
   data_type   = NO_DATA;
   nregions    = 0;

   /* Print info */
   (void) fprintf(stdout,"\n   Reading POLYGON data...(line %d)\n",*lineno);

   /* Set the view parameters */
   view_pr = CNcreate_view();

   /* Set the properties of the plotset */
   CNset_default_plotset_property(&pt_property);

   /* Set the properties of the dataset */
   CNset_default_dataset_property(&ds_property);

   /*
    * This expects one or more polygons and regions
    */

   /* Keep on reading until a "$" is encountered */
   while (!header_found && CNgetucline(fp, line, lineno, BUFSIZ) != EOF) {

      /*EMPTY*/
      if (((len=strlen(line)) == 0) || (len==1 && line[0]=='\n')) {
         /* Ignore newlines */
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
         read_polygon_options(line,view_pr,&pt_property,&ds_property,
                             verbose);
         if ((ds_property.contours) && ((ds_property.flag1 & CNctrlevel)!=0))
         CNset_contstep_levels(&cstephead, &csteptail, &ds_property);

      } else {
         /*
          * Must be ascii mesh data
          * The first non-blank character identifies the element
          * A numeric character indicates data that is to be dumped to an array
          */
 
         switch(line[0]) {

         case 'p' : /* Read the polygon coordinates */
                    if (sscanf(line, "p %d %d", &npts, &regID) != 2)
                       itemno_err(*lineno,2,ierr++);
                    else {
                       /* Start reading a new array */
                       if (poly_arr != NULL) {
                          CNfree_1D_double_array(poly_arr);
                          poly_arr = NULL;
                       }
                       pcount    = 0;
                       data_type = PCOORD_DATA;
                    }
                    break;

         case 'r' : /* Read the region ID and label */
                    (void) read_region(line,*lineno,&ierr,
                                       &regionhead,&regiontail,&nregions);
                    break;

         default  : /* Read the numbers in the line */
                    switch (data_type) {
                    case PCOORD_DATA: 
                         /* Read polygon-coord array */
                         read_array("poly-coords",
                                    &poly_arr,&pcount,npts*4,
                                    &data_type,
                                    line,*lineno,verbose);
                         if (pcount == npts*4) {
                            insert_poly(*lineno,&ierr,
                                        poly_arr,npts,regID,
                                        &polyhead, &polytail, &npolys,
                                        &nodehead, &nodetail, &nnodes,
                                        &pointhead,&pointtail,&npoints);
                            pcount = 0;
                         }
                         break;
                    case NO_DATA    :
                    default         : /* Error */
                                      ugh(*lineno,"unrecognized arguments",
                                          ierr++);
                                      break;
                    }
                    break;
         }
      }
   }
 
   /* Error-check */
   err = CN_FALSE;
   if (polyhead==NULL || nodehead==NULL || pointhead==NULL) err = CN_TRUE;

   /* Now create the dataset */
   if (!err) {
      /* Find the max/min */
      CNget_pointlist_maxmin(pointhead, pointtail,
                             &xmin,&xmax,&ymin,&ymax,&zmin,&zmax);
      CNmtv_adjust_boundaries(&xmin,&xmax,&ymin,&ymax,&zmin,&zmax);

      /* create the dataset */
      *Dptr = CNmake_dataset(filename, dataname, CN_POLYGON,
                             xmin, xmax, ymin, ymax, zmin, zmax,
                             xmin, xmax, ymin, ymax, zmin, zmax,
                             *dataID);
      if ((*Dptr) != NULL) (*dataID)++;

      if ((*Dptr) != NULL) {

         /* Save the points, nodes, polygons */
         (*Dptr)->polyhead  = polyhead;
         (*Dptr)->polytail  = polytail;
         (*Dptr)->nodehead  = nodehead;
         (*Dptr)->nodetail  = nodetail;
         (*Dptr)->pointhead = pointhead;
         (*Dptr)->pointtail = pointtail;

         /* Save the regions */
         (*Dptr)->regionhead = regionhead;
         (*Dptr)->regiontail = regiontail;

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

         /* Special steps for contours */
         CNget_nodelist_tmaxmin(nodehead,nodetail,&tmin,&tmax);
         if (EQUAL(tmin,tmax)) {
            tmin = -0.5;
            tmax =  0.5;
         }
         (*Dptr)->data_pr.cmin = tmin;
         (*Dptr)->data_pr.cmax = tmax;
         (*Dptr)->data_pr.cstep = CNround_to_decimal((tmax-tmin)/
                                                     (double)CN_IDLSTEPS);

         /*
          * Select the contour step size
          * This is done ONLY if the steps have NOT been specified individually
          */
         if ((*Dptr)->data_pr.stepmethod != CN_USERDEFN)
         CNselect_contour_step(&((*Dptr)->data_pr.cmin),
                               &((*Dptr)->data_pr.cmax),
                               (*Dptr)->data_pr.stepmethod,
                               &((*Dptr)->data_pr.cstep),
                               &((*Dptr)->data_pr.nsteps),
                               (*Dptr)->data_pr.logzstep,
                               &((*Dptr)->cstephead), &((*Dptr)->csteptail),
                               verbose);
 
         /* Print out the curve */
         if (verbose) CNprint_dataset((*Dptr),0);
 
      }
   }

   /* Failed */
   if ((*Dptr)==NULL) {
      CNdelete_region_list(&regionhead, &regiontail);
      CNdelete_poly_list(&polyhead, &polytail);
      CNdelete_node_list(&nodehead, &nodetail);
      CNdelete_point_list(&pointhead, &pointtail);
      CNdelete_contstep_list(&cstephead, &csteptail);
      CNdelete_annotation_list(&annothead, &annottail);
   }
 
   /* Reset the property structures */
   CNdelete_plotset_property_fields(&pt_property);
   CNdelete_dataset_property_fields(&ds_property);
 
   /* Free the view structure */
   CNdelete_view(view_pr);
 
   /* Free the arrays */
   if (poly_arr != NULL) CNfree_1D_double_array(poly_arr);

   /* Print out info */
   if ((*Dptr) != NULL) {
      (void) fprintf(stdout,"   POLYGON  : Read %d polygons\n", npolys);
   } else {
      (void) fprintf(stdout,"   POLYGON  : Format error - ");
      (void) fprintf(stdout,"couldn't read data (%d errors)!\n",ierr);
   }

   /* return */
   if (!header_found) 
      return(EOF);
   else    
      return(header_found);
}



/* 
 * Read a line and apply polygon options to a dataset
 */
static void read_polygon_options(header,view_pm,pt_prop,ds_prop,vbs)
char               *header;
CNviewptr          view_pm;
CNplotset_property *pt_prop;
CNdataset_property *ds_prop;
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
                (void) fprintf(stderr,"   warning : Invalid option \"%s=%s\"\n",
                               argtbl[i],valtbl[i]);
      }

      /* Clear the tables */
      CNfreewords(&nargs, argtbl);
      CNfreewords(&nvals, valtbl);
   }
}



/*
 * READ UTILITIES 
 */

/*
 * Print read-error
 */
/*ARGSUSED*/
static void ugh(line,s,ierr)
char *s;
int  line,ierr;
{
   (void) fprintf(stderr,"   %-8s : Input mesh error in line %d: %s\n",
                  "POLYGON",line,s);
}


/*
 * Print item-number error
 */
/*ARGSUSED*/
static void itemno_err(line,itemno,ierr)
int  line,itemno,ierr;
{
   char error[100];
   (void) sprintf(error,"need %d items",itemno);
   ugh(line,error,ierr);
}


/*
 * Read numbers from a line and put them into an array
 */
static void read_array(label, arr, npts, maxpts, data_type, 
                       line, lineno, verbose)
char   *label;
double **arr;     /* container                           */
int    *npts;     /* Number of points currently in array */
int    maxpts;    /* Max array size                      */
int    *data_type;/* Type of data to read                */
char   *line;     /* Current line                        */
int    lineno;    /* Current line number                 */
int    verbose;
{
   double values[MAX_VALS];
   int    pts_in_line=0;

   /* If the array has not yet been created then do so */
   if (*arr == NULL) {
      if (verbose)
         (void) fprintf(stdout,
                        "\n   Reading %d %s...line=%d\n",maxpts,label,lineno);
      *arr = CNcreate_1D_double_array(maxpts);
   }

   /* Read all the points off the line and store */
   if (*arr != NULL) {
      CNmtv_read_line(line,values,MAX_VALS,&pts_in_line);
      CNmtv_filter_array(values,&pts_in_line,*arr,npts,maxpts);
   }

   /* If the array is full, set a flag */
   if (*npts == maxpts) *data_type = NO_DATA;
}


/*
 * Read a region
 */
static CNregionptr read_region(line,lineno,ierr,regionhead,regiontail,nreg)
char        *line;
int         lineno, *ierr;
CNregionptr *regionhead, *regiontail;
int         *nreg;
{
   CNregionptr R;
   int         regID, color;
   short       doplot, docont;
   short       doplot_defined = CN_FALSE;
   short       docont_defined = CN_FALSE;
   short       color_defined = CN_FALSE;
   char        regname[BUFSIZ];
   char        *word[CN_MAXWORDS];
   int         nw=0;
 
   /*
    * The first 2 arguments are necessary, the next 2 are optional
    */
 
   /* Parse line into lots of words */
   if ((nw = CNgetwords(line,word, CN_MAXWORDS)) >= 3) {
 
      /* Skip the first word, Copy the next 2 words */
      CNassign_int_keyword(&regID ,word[1],"regID",0,0,0,0,0,0);
      CNassign_string_keyword(regname,word[2],"material",0);
      if (nw >= 4) {
         CNassign_int_keyword(&color ,word[3],"color",-1,-1,100,1,0,0);
         if (color < -1)
            color = -1;
         else if (color > CN_MAX_LINE_COLORS)
            color = (color - 1) % CN_MAX_LINE_COLORS + 1;
         color_defined = CN_TRUE;
      }
      if (nw >= 5) {
         CNassign_boolean_keyword(&docont,word[4],"docont",0);
         docont_defined = CN_TRUE;
      }
      if (nw >= 6) {
         CNassign_boolean_keyword(&doplot,word[5],"doplot",0);
         doplot_defined = CN_TRUE;
      }
 
      /* Insert the region */
      if ((R=insert_region(regionhead,regiontail,regname,regID))!=NULL) {
         /* Set internal flags */
         if (color_defined ) R->color  = color;
         if (docont_defined) R->nocont = !docont;
         if (doplot_defined) R->isair  = !doplot;
 
         /* Increment the region count */
         (*nreg)++;
      } else {
         ugh (lineno,"insufficient memory",(*ierr)++);
      }
   } else {
      itemno_err(lineno,2,(*ierr)++);
   }
 
   /* Free the words */
   CNfreewords(&nw,word);

   /* Return */
   return(R);
}

/*
 * Insert a region
 */
static CNregionptr insert_region(regionhead,regiontail,regname,regID)
CNregionptr *regionhead, *regiontail;
int         regID;
char        *regname;
{
   CNregionptr R;
 
   R = CNinsert_region(regionhead, regiontail, regname, regID, regID);
   return(R);
}


/*
 * Read a polygon 
 */
static void insert_poly(lineno,ierr,
                        arr, npts, regID, 
                        polyhead,polytail,npoly,
                        nodehead,nodetail,nnode,
                        pointhead,pointtail,npoint)
int         lineno, *ierr;
double      *arr;
int         npts;   /* No of elements in the poly */
int         regID;
CNpolyptr   *polyhead, *polytail;
CNnodeptr   *nodehead, *nodetail;
CNpointptr  *pointhead,*pointtail;
int         *npoly, *nnode, *npoint;
{
   CNnlistptr  nlhead=NULL, nltail=NULL;
   CNnodeptr   nd, nd0=NULL;
   CNpointptr  pt;
   CNpolyptr   P=NULL;
   int         i;
 
   /* Insert the points */
   for (i=0; i<npts; i++) {
      pt = CNinsert_point(pointhead, pointtail, 
                          arr[i*4], arr[i*4+1], arr[i*4+2], (*npoint)++);
      nd = CNinsert_tailnode(nodehead, nodetail, pt, arr[i*4+3], (*nnode)++);
      (void) CNinsert_tailnlist(&nlhead, &nltail, nd);
      if (i==0) nd0 = nd;

   }

   /* Insert the polygon */
   if (nlhead != NULL) {
      /* Add one more node to close the polygon */
      if (nd0 != NULL) (void) CNinsert_tailnlist(&nlhead, &nltail, nd0);

      /* Create the polygon */
      P = CNinsert_poly(polyhead, polytail, nlhead, nltail, regID, (*npoly)++);
   }

   if (P==NULL) {
      ugh (lineno,"couldn't add polygon",(*ierr)++);
   }
}

