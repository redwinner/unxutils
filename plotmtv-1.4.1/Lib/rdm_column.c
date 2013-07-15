/*
 * rdm_column.c - routines to read multi-column curve data from an MTV-format.
 *
 * The data is assumed to be sandwiched between $DATA=xxx lines
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include "CNplot.h"


/*
 * Data is organized in columns:
 *    Row 1:   Column_1   Column_2 ... Column_n
 *    Row 2:   Column_1   Column_2 ... Column_n
 *    ...
 *    Row M:   Column_1   Column_2 ... Column_n
 *
 * Each column contains a number of row-sets.  A row-set contains a
 * number of rows; each rowset is separated by a blank line in the file.
 * A row-set corresponds to a single curve.
 *     Column_1
 *       row1   | row-set-1 
 *       row2   | 
 *       row3   | 
 *
 *       row4   | row-set-2 
 *       row5   | 
 *       row6   | 
 *
 *       ...    | ...    
 *
 *       ...    | row-set-m
 *
 * Each row-set contains several rows of double-precision values
 *     Column_1
 *       row-set-1 
 *          row1
 *          row2
 *          ...
 *          row-n
 *
 *       row-set-2 
 *          row1
 *          row2
 *          ...
 *          row-n
 *
 *       ...           
 *
 *       row-set-m
 *          row1
 *          row2
 *          ...
 *          row-n
 *
 * WARNING:  THE MAX NO OF COLUMNS IS HARDCODED TO 100!!!
 */    

/* Row Data-structure */
typedef struct CLrow_strct {
   double value;
   short  invalid;
   struct CLrow_strct *next;
   struct CLrow_strct *prev;
} CLrow;
typedef struct CLrow_strct *CLrowptr;

/* Row-Set Data-structure */
typedef struct CLrowset_strct {
   CNcurve_property *curv_pr;
   struct CLrow_strct    *rowhead;
   struct CLrow_strct    *rowtail;
   struct CLrowset_strct *next;
   struct CLrowset_strct *prev;
} CLrowset;
typedef struct CLrowset_strct *CLrowsetptr;
    
/* Column Data-structure */
typedef struct CLcolumn_strct {
   char   name[CN_MAXCHAR];
   struct CLrowset_strct *rowsethead;
   struct CLrowset_strct *rowsettail;
   struct CLcolumn_strct *next;
   struct CLcolumn_strct *prev;
} CLcolumn;
typedef struct CLcolumn_strct *CLcolumnptr;

int CNmtv_read_column_data();
static void        read_cl_options();
static void        column_itemno_err();

static CLcolumnptr make_column();
static CLcolumnptr insert_column();
static void        delete_column();
static void        delete_column_list();
static int         count_columns();

static CLrowsetptr make_rowset();
static CLrowsetptr insert_rowset();
static void        delete_rowset();
static void        delete_rowset_list();
static void        apply_curvpr_to_rowset();

static CLrowptr    make_row();
static CLrowptr    insert_row();
static void        delete_row();
static void        delete_row_list();

#define MAXCOLS 100   /* Max no of rows per line */

/* 
 * Read data in multicolumn format
 * Return an EOF if such is found during the read; otherwise return 0
 */
/*ARGSUSED*/
int CNmtv_read_column_data(datahead, datatail, dataID, dataname,
                           filename, fp, lineno, header, verbose)
CNdslistptr  *datahead;        /* Pointers to plot data                */
CNdslistptr  *datatail;        /* Pointers to plot data                */
int          *dataID;          /* Data ID                              */
char         *dataname;        /* Dataset name                         */
char         *filename;        /* The name of the file/pipe source     */
FILE         *fp;              /* The file/pipe source                 */
int          *lineno;          /* Current line number                  */
char         *header;          /* Header string                        */
int          verbose;          /* Verbosity Flag                       */
{
   char               *CNstrip_keyword();
   CNcontstepptr      cstephead=NULL, csteptail=NULL;
   CNannotptr         annothead=NULL, annottail=NULL;
   CLcolumnptr        column_listhead=NULL, column_listtail=NULL, C, CX;
   CLrowsetptr        RS, RSx, RSy;
   CLrowptr           Rx, Ry;
   CNcurveptr         curvehead, curvetail, Cptr;
   CNpointptr         P;
   CNdatasetptr       dptr;
   CNviewptr          view_pr;
   CNcurve_property   cv_property[MAXCOLS];
   CNgbcurve_property gb_property[MAXCOLS];
   CNdataset_property ds_property;
   CNplotset_property pt_property;
   char               line[BUFSIZ];
   char               xcolumn[CN_MAXCHAR];
   char               name[CN_MAXCHAR];
   char               *word[CN_MAXWORDS];
   int                header_found = CN_FALSE;
   int                nw=0, len, ierr=0, i, FOUND=CN_FALSE;
   int                ncolumns = 0;
   int                invalid, break_curve;
   int                first_dataset = CN_TRUE;
   int                pointID, curveID;
   double             value, x, y, z;
   double             xmin, xmax, ymin, ymax, zmin=0.0, zmax=0.0;
 
   /* Print info */
   (void) fprintf(stdout,"\n   Reading COLUMN data...(line %d)\n",*lineno);

   /* Set the view parameters */
   view_pr = CNcreate_view();

   /* Set the properties of the plotset */
   CNset_default_plotset_property(&pt_property);

   /* Set the properties of the dataset */
   CNset_default_dataset_property(&ds_property);

   /* Set the properties of the curve */
   for (i=0; i<MAXCOLS; i++) 
   CNset_default_curve_property(&(cv_property[i]));

   /* Set the properties of the global curve */
   for (i=0; i<MAXCOLS; i++) 
   CNset_default_gbcurve_property(&(gb_property[i]));

   /* Initalize */
   (void) strcpy(xcolumn,"");

   /* Keep on reading until a "$" is encountered */
   while (!header_found && CNgetucline(fp, line, lineno, BUFSIZ) != EOF) {

      if (((len=strlen(line)) == 0) || (len==1 && line[0]=='\n')) {
         /* Create new row-sets */
         i=0;
         for (C=column_listhead; C!=NULL; C=C->next) {
            i++;
            if (C->rowsettail != NULL && C->rowsettail->rowtail != NULL) {
               /* Apply the curve properties to the rowset */
               if (i < MAXCOLS)
               apply_curvpr_to_rowset(C->rowsettail, &(cv_property[i]));

               /* Create a new rowset */
               (void) insert_rowset(&(C->rowsethead), &(C->rowsettail));
            }
         }

         /* New curve */
         for (i=0; i<MAXCOLS; i++) {
            CNdelete_curve_property_fields(&(cv_property[i]));
            CNset_default_curve_property(&(cv_property[i]));
         }

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
         read_cl_options(line,view_pr,&pt_property,&ds_property,
                         cv_property, gb_property, 
                         xcolumn, ncolumns, verbose);
         if ((ds_property.contours) && ((ds_property.flag1 & CNctrlevel)!=0))
         CNset_contstep_levels(&cstephead, &csteptail, &ds_property);

      } else {
         /* Read column data */
         
         if ((nw = CNgetwords(line,word, CN_MAXWORDS)) >= 2) {
            if (column_listhead == NULL) {
               /* make columns */
               for (i=0; i<nw; i++)
                  (void) insert_column(&column_listhead, &column_listtail, 
                                         CNstrip_keyword(word[i]));
               ncolumns = count_columns(column_listhead, column_listtail);

            } else {
               /* Add to the columns */
               i = 0;
               for (C=column_listhead; C!=NULL; C=C->next) {
                  /* Get the value of the word */
                  value = 0.0;
                  invalid = CN_TRUE;
                  if ( (i < nw) && (strlen(word[i])>0) && !isalpha(word[i][0])){
                     value   = atof(word[i]);
                     invalid = CN_FALSE;
                  }

                  /* Create a rowset if necessary */
                  if ((RS=C->rowsettail) == NULL) {
                     /* Create a rowset */
                     RS = insert_rowset(&(C->rowsethead), &(C->rowsettail));
                  }

                  /* Insert a row and place the value into the row */
                  if (RS != NULL) {
                     /* Put the row into this rowset */
                     (void) insert_row(&(RS->rowhead), &(RS->rowtail),
                                       value, invalid);
                  }
                  i++;
               }      
            }
         } else {
            column_itemno_err(*lineno,2,ierr++);
         }

         /* Free the words */
         CNfreewords(&nw,word);
      }
   }

   /* 
    * Apply the curve properties to the last rowsets.
    * This is necessary if there is no blank line at the end of the data
    */
   i=0;
   for (C=column_listhead; C!=NULL; C=C->next) {
      i++;
      if (C->rowsettail != NULL && C->rowsettail->rowtail != NULL) {
         /* Apply the curve properties to the rowset */
         if (i < MAXCOLS)
         apply_curvpr_to_rowset(C->rowsettail, &(cv_property[i]));
      }
   } 

   /* Create the various datasets */
   if (column_listhead != NULL) {

      /* Find the x-column */
      CX = column_listhead;
      if (strlen(xcolumn) > 0) {
         for (C=column_listhead; C!=NULL && !FOUND; C=C->next) {
            if (strcmp(C->name,xcolumn)==0) {
               FOUND = CN_TRUE;
               CX = C;
            }
         }
      }

      /* Separate out the curves */
      i=0;
      for (C=column_listhead; C!=NULL; C=C->next) {
         i++;
         if (C==CX) continue;
         if (C->rowsethead==NULL) continue;

         curveID   = 0;
         pointID   = 0;
         curvehead = NULL;
         curvetail = NULL;
         (void) sprintf(name,"%s vs %s",C->name,CX->name);
         RSy = C->rowsethead;
         for (RSx=CX->rowsethead; RSx!=NULL; RSx=RSx->next) {

            /* Don't create the curve if the rowset is empty */
            if (RSx->rowhead == NULL) continue;

            /* Create a curve */
            Cptr = CNinsert_curve(&curvehead, &curvetail, curveID++);

            /* Set curve properties */
            if (RSy->curv_pr)
            CNset_curve_property(&(Cptr->curv_pr), RSy->curv_pr);

            /*
             * If the label on the curve has not been set, set it 
             * Do this only for the first curve 
             */
            if ((Cptr == curvehead) && 
                ((Cptr->curv_pr.flag & CNlinelabel) == 0)) {
               (void) CNparse_curve_property(&(Cptr->curv_pr),
                                             "linelabel", C->name, 0);
            }

            /* Fill the curve */
            break_curve = CN_FALSE;
            Ry = RSy->rowhead;
            for (Rx=RSx->rowhead; Rx!=NULL; Rx=Rx->next) {
               /* x,y,z values */
               x = Rx->value; 
               y = (Ry!=NULL) ? Ry->value : 0.0;
               z = 0.0;

               /* 
                * Break up the curve at invalid points 
                * The points are inserted ONLY if both X and Y points are valid
                */
               if ( (Rx && Rx->invalid==CN_FALSE) &&
                    (Ry && Ry->invalid==CN_FALSE) ) {
                  if (break_curve) {
                     /* Create a new curve but only if prev curve is empty */ 
                     if (Cptr->pointhead != NULL) {
                        /* Create a curve */
                        Cptr=CNinsert_curve(&curvehead, &curvetail, curveID++);

                        /* Set curve properties */
                        if (RSy->curv_pr)
                        CNset_curve_property(&(Cptr->curv_pr), RSy->curv_pr);
                     }
                     break_curve = CN_FALSE;
                  }
                  (void) CNinsert_point(&(Cptr->pointhead), &(Cptr->pointtail), 
                                        x, y, z, pointID++);
               } else {
                  break_curve = CN_TRUE;
               }

               /* Next y-point */
               if (Ry != NULL) Ry = Ry->next;
            }
            if (RSy != NULL) RSy = RSy->next;
         } 

         /* In this scheme it is possible that the last curve will be empty */
         if (Cptr->pointhead == NULL) 
            CNdelete_curve(&curvehead, &curvetail, Cptr);

         /* Now create the dataset */
         if (curvehead != NULL) {

            /* 
             * Apply the global-curve dataset properties to 
             * the individual curves 
             */
            if (i<MAXCOLS && (gb_property[i].flag != 0))
            CNreset_curves(curvehead, curvetail, &(gb_property[i]), 0);

            /* Get the min and max of the curves */
            xmin =  CN_LARGE;
            xmax = -CN_LARGE;
            ymin =  CN_LARGE;
            ymax = -CN_LARGE;
            for (Cptr=curvehead; Cptr!=NULL; Cptr=Cptr->next) {
               for (P=Cptr->pointhead; P!=NULL; P=P->next) {
                  if (P->x < xmin) xmin = P->x;
                  if (P->x > xmax) xmax = P->x;
                  if (P->y < ymin) ymin = P->y;
                  if (P->y > ymax) ymax = P->y;
               }
            }

            /* Adjust xmin, xmax, ymin, ymax, zmin, zmax */
            CNmtv_adjust_boundaries(&xmin, &xmax, &ymin, &ymax, &zmin, &zmax);

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

            /* Create a dataset containing the curves */
            dptr = CNmake_dataset(filename,name,CN_PLOT2D,
                                  xmin,xmax,ymin,ymax,zmin,zmax,
                                  xmin,xmax,ymin,ymax,zmin,zmax,*dataID);
            if (dptr != NULL) (*dataID)++;

            if (dptr != NULL) {
               dptr->curvehead = curvehead;
               dptr->curvetail = curvetail;

               /* Set overlay */
               /*
               (void) CNparse_plotset_property(&(dptr->plot_pr),
                                            "overlay", "FALSE", 0);
                */

               /* Set name */
               (void) CNparse_plotset_property(&(dptr->plot_pr),
                                            "toplabel", name, 0);

               /* Apply the view options to the dataset */
               CNset_view_property(dptr->view_pr,view_pr);
 
               /* Apply the plotset options to the dataset */
               CNset_plotset_property(&(dptr->plot_pr),&pt_property);
 
               /* Apply the dataset options to the dataset */
               CNset_dataset_property(&(dptr->data_pr),&ds_property);

               /* Apply the contsteps */
               if (cstephead != NULL) {
                  if (first_dataset) {
                     dptr->cstephead = cstephead;
                     dptr->csteptail = csteptail;
                     first_dataset = CN_FALSE;
                  } else {
                     CNcopy_contstep_list(&(dptr->cstephead),
                                          &(dptr->csteptail),
                                          cstephead, csteptail);
                  }
               }

               /* Print out the dataset */
               if (verbose) CNprint_dataset(dptr, 0);
 
#ifdef DEBUG
               /* Print out the curves */
               CNprint_curve_list(dptr->curvehead, dptr->curvetail, 1);
#endif

               /* Store the dataset in the dataset linked list */
               (void) CNinsert_dslist(datahead, datatail, dptr);
            }
         }
      }
   }
         
   /* Delete all the columns */
   delete_column_list(&column_listhead, &column_listtail);

   /* Reset the property structures */
   CNdelete_plotset_property_fields(&pt_property);
   CNdelete_dataset_property_fields(&ds_property);
   for (i=0; i<MAXCOLS; i++) 
   CNdelete_gbcurve_property_fields(&(gb_property[i]));
   for (i=0; i<MAXCOLS; i++) 
   CNdelete_curve_property_fields(&(cv_property[i]));

   /* Free the view structure */
   CNdelete_view(view_pr);

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
static void read_cl_options(header,
                            view_pr,pt_prop,ds_prop,cv_prop,gb_prop,
                            xcol, ncols, vbs)
char               *header;
CNviewptr          view_pr;
CNplotset_property *pt_prop;
CNdataset_property *ds_prop;
CNcurve_property    cv_prop[];
CNgbcurve_property  gb_prop[];
char               *xcol;
int                ncols;
int                vbs;
{
   char *argtbl[CN_MAXWORDS], *valtbl[CN_MAXWORDS];
   char *words[CN_MAXWORDS];
   char value[CN_MAXCHAR];
   char newheader[CN_MAXCHAR];
   int  nargs = 0, nvals = 0;
   int  argfound, i, len, ncolumns=0;

   /* No of columns */
   ncolumns = (ncols > 0) ? ncols : MAXCOLS;

   /* 
    * The COLUMN format uses its own format for CURVE and GBCURVE options.
    * Screen first for curve options of the form "%linecolor 1 2 3 ..." 
    * Then screen for gbcurve options of the form "%dlinecolor 1 2 3 ..." 
    */
   (void) strcpy(newheader, header);
   len = strlen(newheader);
   if (len > 0) len = CNuncomment_line(newheader, CN_MAXCHAR);
   if (len > 0) {
      if ((nargs = CNgetwords(header,words,CN_MAXWORDS)) >= 2) {

         /*
          * IF there are less words than columns, the later columns
          * take on the value of the last word, i.e.
          * 3 columns: % linetype= 1 2 (column1=1 column2=2 column3=2)
          *
          * Below, arg[0] = argument
          *        arg[1] = value => applies to cv_prop[1]
          *        arg[n] = value => applies to cv_prop[n]
          * cv_prop[0] and gb_prop[0] are ALWAYS empty
          */
         argfound = CN_TRUE;

         if (strcmp(words[0],"linelabel")==0) {
            for (i=1; i< LARGER_OF(nargs, ncolumns+1) && i<MAXCOLS; i++) {
               if (i < nargs) (void) strcpy(value, words[i]);
               else           (void) strcpy(value, words[nargs-1]);
               (void) CNparse_curve_property(&(cv_prop[i]),"linelabel",
                                             value,vbs);
            }

         } else if ( (strcmp(words[0],"linetype")==0) ||
                     (strcmp(words[0],"lt")==0) ) {
            for (i=1; i< LARGER_OF(nargs, ncolumns+1) && i<MAXCOLS; i++) {
               if (i < nargs) (void) strcpy(value, words[i]);
               else           (void) strcpy(value, words[nargs-1]);
               (void) CNparse_curve_property(&(cv_prop[i]),"linetype",
                                             value,vbs);
            }

         } else if ( (strcmp(words[0],"linecolor")==0) ||
                     (strcmp(words[0],"lc")==0) ) {
            for (i=1; i< LARGER_OF(nargs, ncolumns+1) && i<MAXCOLS; i++) {
               if (i < nargs) (void) strcpy(value, words[i]);
               else           (void) strcpy(value, words[nargs-1]);
               (void) CNparse_curve_property(&(cv_prop[i]),"linecolor",
                                             value,vbs);
            }

         } else if ( (strcmp(words[0],"linewidth")==0) ||
                     (strcmp(words[0],"lw")==0) ) {
            for (i=1; i< LARGER_OF(nargs, ncolumns+1) && i<MAXCOLS; i++) {
               if (i < nargs) (void) strcpy(value, words[i]);
               else           (void) strcpy(value, words[nargs-1]);
               (void) CNparse_curve_property(&(cv_prop[i]),"linewidth",
                                             value,vbs);
            }

         } else if ( (strcmp(words[0],"marktype")==0) ||
                     (strcmp(words[0],"markertype")==0) ||
                     (strcmp(words[0],"mt")==0) ) {
            for (i=1; i< LARGER_OF(nargs, ncolumns+1) && i<MAXCOLS; i++) {
               if (i < nargs) (void) strcpy(value, words[i]);
               else           (void) strcpy(value, words[nargs-1]);
               (void) CNparse_curve_property(&(cv_prop[i]),"markertype",
                                             value,vbs);
            }

         } else if ( (strcmp(words[0],"markcolor")==0) ||
                     (strcmp(words[0],"markercolor")==0) ||
                     (strcmp(words[0],"mc")==0) ) {
            for (i=1; i< LARGER_OF(nargs, ncolumns+1) && i<MAXCOLS; i++) {
               if (i < nargs) (void) strcpy(value, words[i]);
               else           (void) strcpy(value, words[nargs-1]);
               (void) CNparse_curve_property(&(cv_prop[i]),"markercolor",
                                             value,vbs);
            }

         } else if ( (strcmp(words[0],"marksize")==0) ||
                     (strcmp(words[0],"markersize")==0) ||
                     (strcmp(words[0],"ms")==0) ) {
            for (i=1; i< LARGER_OF(nargs, ncolumns+1) && i<MAXCOLS; i++) {
               if (i < nargs) (void) strcpy(value, words[i]);
               else           (void) strcpy(value, words[nargs-1]);
               (void) CNparse_curve_property(&(cv_prop[i]),"markersize",
                                             value,vbs);
            }

         } else if ( (strcmp(words[0],"filltype")==0) ||
                     (strcmp(words[0],"ft")==0) ) {
            for (i=1; i< LARGER_OF(nargs, ncolumns+1) && i<MAXCOLS; i++) {
               if (i < nargs) (void) strcpy(value, words[i]);
               else           (void) strcpy(value, words[nargs-1]);
               (void) CNparse_curve_property(&(cv_prop[i]),"filltype",
                                             value,vbs);
            }

         } else if ( (strcmp(words[0],"fillcolor")==0) ||
                     (strcmp(words[0],"fc")==0) ) {
            for (i=1; i< LARGER_OF(nargs, ncolumns+1) && i<MAXCOLS; i++) {
               if (i < nargs) (void) strcpy(value, words[i]);
               else           (void) strcpy(value, words[nargs-1]);
               (void) CNparse_curve_property(&(cv_prop[i]),"fillcolor",
                                             value,vbs);
            }

         /* GBCURVE properties */

         } else if (strcmp(words[0],"dlinelabel")==0) {
            for (i=1; i< LARGER_OF(nargs, ncolumns+1) && i<MAXCOLS; i++) {
               if (i < nargs) (void) strcpy(value, words[i]);
               else           (void) strcpy(value, words[nargs-1]);
               (void) CNparse_gbcurve_property(&(gb_prop[i]),"dlinelabel",
                                             value,vbs);
            }

         } else if ( (strcmp(words[0],"dlinetype")==0) ||
                     (strcmp(words[0],"dlt")==0) ) {
            for (i=1; i< LARGER_OF(nargs, ncolumns+1) && i<MAXCOLS; i++) {
               if (i < nargs) (void) strcpy(value, words[i]);
               else           (void) strcpy(value, words[nargs-1]);
               (void) CNparse_gbcurve_property(&(gb_prop[i]),"dlinetype",
                                             value,vbs);
            }

         } else if ( (strcmp(words[0],"dlinecolor")==0) ||
                     (strcmp(words[0],"dlc")==0) ) {
            for (i=1; i< LARGER_OF(nargs, ncolumns+1) && i<MAXCOLS; i++) {
               if (i < nargs) (void) strcpy(value, words[i]);
               else           (void) strcpy(value, words[nargs-1]);
               (void) CNparse_gbcurve_property(&(gb_prop[i]),"dlinecolor",
                                             value,vbs);
            }

         } else if ( (strcmp(words[0],"dlinewidth")==0) ||
                     (strcmp(words[0],"dlw")==0) ) {
            for (i=1; i< LARGER_OF(nargs, ncolumns+1) && i<MAXCOLS; i++) {
               if (i < nargs) (void) strcpy(value, words[i]);
               else           (void) strcpy(value, words[nargs-1]);
               (void) CNparse_gbcurve_property(&(gb_prop[i]),"dlinewidth",
                                             value,vbs);
            }

         } else if ( (strcmp(words[0],"dmarktype")==0) ||
                     (strcmp(words[0],"dmarkertype")==0) ||
                     (strcmp(words[0],"dmt")==0) ) {
            for (i=1; i< LARGER_OF(nargs, ncolumns+1) && i<MAXCOLS; i++) {
               if (i < nargs) (void) strcpy(value, words[i]);
               else           (void) strcpy(value, words[nargs-1]);
               (void) CNparse_gbcurve_property(&(gb_prop[i]),"dmarkertype",
                                             value,vbs);
            }

         } else if ( (strcmp(words[0],"dmarkcolor")==0) ||
                     (strcmp(words[0],"dmarkercolor")==0) ||
                     (strcmp(words[0],"dmc")==0) ) {
            for (i=1; i< LARGER_OF(nargs, ncolumns+1) && i<MAXCOLS; i++) {
               if (i < nargs) (void) strcpy(value, words[i]);
               else           (void) strcpy(value, words[nargs-1]);
               (void) CNparse_gbcurve_property(&(gb_prop[i]),"dmarkercolor",
                                             value,vbs);
            }

         } else if ( (strcmp(words[0],"dmarksize")==0) ||
                     (strcmp(words[0],"dmarkersize")==0) ||
                     (strcmp(words[0],"dms")==0) ) {
            for (i=1; i< LARGER_OF(nargs, ncolumns+1) && i<MAXCOLS; i++) {
               if (i < nargs) (void) strcpy(value, words[i]);
               else           (void) strcpy(value, words[nargs-1]);
               (void) CNparse_gbcurve_property(&(gb_prop[i]),"dmarkersize",
                                             value,vbs);
            }

         } else if ( (strcmp(words[0],"dfilltype")==0) ||
                     (strcmp(words[0],"dft")==0) ) {
            for (i=1; i< LARGER_OF(nargs, ncolumns+1) && i<MAXCOLS; i++) {
               if (i < nargs) (void) strcpy(value, words[i]);
               else           (void) strcpy(value, words[nargs-1]);
               (void) CNparse_gbcurve_property(&(gb_prop[i]),"dfilltype",
                                             value,vbs);
            }

         } else if ( (strcmp(words[0],"dfillcolor")==0) ||
                     (strcmp(words[0],"dfc")==0) ) {
            for (i=1; i< LARGER_OF(nargs, ncolumns+1) && i<MAXCOLS; i++) {
               if (i < nargs) (void) strcpy(value, words[i]);
               else           (void) strcpy(value, words[nargs-1]);
               (void) CNparse_gbcurve_property(&(gb_prop[i]),"dfillcolor",
                                             value,vbs);
            }

         } else {
            argfound = CN_FALSE;
         }
      }
 
      /* Free the word table */
      if (nargs > 0) CNfreewords(&nargs,words);
 
   }
   /* Skip if a match was found */
   if (argfound) return;

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
 
         if (strncmp(argtbl[i],"xcol",4)==0) {
            CNassign_string_keyword(xcol,valtbl[i],"xcolumn",0);
            if (vbs)
            (void)fprintf(stdout,"   Column  : %-14s= %s\n","xcol",xcol);

         } else {
            argfound = CN_FALSE;
         }
 
         /* Reset the table if a match was found; otherwise increment index */
         CNdownshift(argfound,&i,argtbl,valtbl,&nargs,&nvals);
      }

      /* Look for plotset/dataset/curve arguments */
      for (i=0; i<nargs; i++) {
         if (!CNparse_view_property       (view_pr,argtbl[i],valtbl[i],vbs))
           if (!CNparse_plotset_property  (pt_prop,argtbl[i],valtbl[i],vbs))
             if (!CNparse_dataset_property(ds_prop,argtbl[i],valtbl[i],vbs))
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
static void column_itemno_err(lineno,itemno,ierr)
int  lineno,itemno,ierr;
{
   (void) fprintf(stderr,"   warning (line %3d) %8s: ",lineno,"COLUMN");
   (void) fprintf(stderr,"need at least %d items\n",itemno);
}


/*
 * COLUMN DATA_STRUCT ALLOCATION
 */
 
/*
 * make a column
 */
static CLcolumnptr make_column(name)
char *name;
{
   CLcolumnptr newptr;
   unsigned int size = sizeof(CLcolumn);
 
   if ((newptr = (CLcolumnptr)malloc(size))!=NULL) {
      if (name != NULL) (void) strcpy(newptr->name,name);
      newptr->rowsethead = NULL;
      newptr->rowsettail = NULL;
      newptr->next       = NULL;
      newptr->prev       = NULL;
   }
 
   return(newptr);
}
 
/*
 * Insert a column at the tail of the current column list
 */
static
CLcolumnptr insert_column(column_listhead,column_listtail,name)
CLcolumnptr *column_listhead, *column_listtail;
char     *name;
{
   CLcolumnptr next,A,B;
 
   A = *column_listtail;
   if ((B=make_column(name))!=NULL) {
      if (A==NULL) {
         *column_listhead = B;
         *column_listtail = B;
      } else {
         next = A->next;
         B->next = next;
         B->prev = A;
         A->next = B;
         if (next    != NULL) next->prev = B;
         if (B->next == NULL) *column_listtail = B;
      }
   }
   return(B);
}
 
/*
 * Delete column
 */
static void delete_column(column_listhead, column_listtail, C)
CLcolumnptr *column_listhead, *column_listtail;
CLcolumnptr C;
{
   CLcolumnptr prev,next;
 
   /* Delete the rowset */
   delete_rowset_list(&(C->rowsethead), &(C->rowsettail));

   prev = C->prev;
   next = C->next;
   if (prev!=NULL) prev->next = next;
   if (next!=NULL) next->prev = prev;
   if (C==*column_listhead) *column_listhead = next;
   if (C==*column_listtail) *column_listtail = prev;
 
   /* Now delete C */
   free ((char*)C);
}

 
/*
 * Delete all the columns in the list
 */
static void delete_column_list(column_listhead, column_listtail)
CLcolumnptr *column_listhead, *column_listtail;
{
   CLcolumnptr C;
 
   while ((C = *column_listhead) != NULL)
      delete_column(column_listhead, column_listtail, C);
}

/*
 * Count the number of columns in the list
 */
/*ARGSUSED*/
static int count_columns(column_listhead, column_listtail)
CLcolumnptr column_listhead, column_listtail;
{
   CLcolumnptr C;
   int        count = 0;
 
   for (C=column_listhead; C!=NULL; C=C->next) count++;
 
   return(count);
}


/*
 * ROW-SET DATA_STRUCT ALLOCATION
 */
 
/*
 * make a rowset
 */
static CLrowsetptr make_rowset()
{
   CLrowsetptr newptr;
   unsigned int size = sizeof(CLrowset);
 
   if ((newptr = (CLrowsetptr)malloc(size))!=NULL) {
      newptr->curv_pr   = NULL;
      newptr->rowhead   = NULL;
      newptr->rowtail   = NULL;
      newptr->next      = NULL;
      newptr->prev      = NULL;
   }
 
   return(newptr);
}
 
/*
 * Insert a rowset at the tail of the current rowset list
 */
static
CLrowsetptr insert_rowset(rowset_listhead,rowset_listtail)
CLrowsetptr *rowset_listhead, *rowset_listtail;
{
   CLrowsetptr next,A,B;
 
   A = *rowset_listtail;
   if ((B=make_rowset())!=NULL) {
      if (A==NULL) {
         *rowset_listhead = B;
         *rowset_listtail = B;
      } else {
         next = A->next;
         B->next = next;
         B->prev = A;
         A->next = B;
         if (next    != NULL) next->prev = B;
         if (B->next == NULL) *rowset_listtail = B;
      }
   }
   return(B);
}
 
/*
 * Delete rowset
 */
static void delete_rowset(rowset_listhead, rowset_listtail, R)
CLrowsetptr *rowset_listhead, *rowset_listtail;
CLrowsetptr R;
{
   CLrowsetptr prev,next;
 
   /* Free the curve property if that exists */
   if (R->curv_pr != NULL) CNdelete_curve_property(R->curv_pr);

   /* Free the row list */
   delete_row_list(&(R->rowhead), &(R->rowtail));

   prev = R->prev;
   next = R->next;
   if (prev!=NULL) prev->next = next;
   if (next!=NULL) next->prev = prev;
   if (R==*rowset_listhead) *rowset_listhead = next;
   if (R==*rowset_listtail) *rowset_listtail = prev;
 
   /* Now delete R */
   free ((char*)R);
}

 
/*
 * Delete all the rowsets in the list
 */
static void delete_rowset_list(rowset_listhead, rowset_listtail)
CLrowsetptr *rowset_listhead, *rowset_listtail;
{
   CLrowsetptr R;
 
   while ((R = *rowset_listhead) != NULL)
      delete_rowset(rowset_listhead, rowset_listtail, R);
}


/* 
 * Apply the curve properties to the rowset 
 */
static void apply_curvpr_to_rowset(R, curv_pr)
CLrowsetptr R;
CNcurve_property *curv_pr;
{

   /* Error check */
   if (R==NULL || curv_pr==NULL || curv_pr->flag==0) return;

   /* Create a curve-property structure */
   if (R->curv_pr == NULL) {
      R->curv_pr = CNmake_curve_property();
   }

   /* Apply the property */
   if (R->curv_pr != NULL) {
      CNset_curve_property(R->curv_pr, curv_pr);
   } 
}



/*
 * ROW DATA_STRUCT ALLOCATION
 */
 
/*
 * make a row
 */
static CLrowptr make_row(value, invalid)
double value;
int    invalid;
{
   CLrowptr newptr;
   unsigned int size = sizeof(CLrow);
 
   if ((newptr = (CLrowptr)malloc(size))!=NULL) {
      newptr->value         = value;
      newptr->invalid       = invalid;
      newptr->next          = NULL;
      newptr->prev          = NULL;
   }
 
   return(newptr);
}
 
/*
 * Insert a row at the tail of the current row list
 */
static
CLrowptr insert_row(row_listhead,row_listtail,value,invalid)
CLrowptr *row_listhead, *row_listtail;
double   value;
int      invalid;
{
   CLrowptr next,A,B;
 
   A = *row_listtail;
   if ((B=make_row(value,invalid))!=NULL) {
      if (A==NULL) {
         *row_listhead = B;
         *row_listtail = B;
      } else {
         next = A->next;
         B->next = next;
         B->prev = A;
         A->next = B;
         if (next    != NULL) next->prev = B;
         if (B->next == NULL) *row_listtail = B;
      }
   }
   return(B);
}
 
/*
 * Delete row
 */
static void delete_row(row_listhead, row_listtail, R)
CLrowptr *row_listhead, *row_listtail;
CLrowptr R;
{
   CLrowptr prev,next;
 
   prev = R->prev;
   next = R->next;
   if (prev!=NULL) prev->next = next;
   if (next!=NULL) next->prev = prev;
   if (R==*row_listhead) *row_listhead = next;
   if (R==*row_listtail) *row_listtail = prev;
 
   /* Now delete R */
   free ((char*)R);
}

 
/*
 * Delete all the rows in the list
 */
static void delete_row_list(row_listhead, row_listtail)
CLrowptr *row_listhead, *row_listtail;
{
   CLrowptr R;
 
   while ((R = *row_listhead) != NULL)
      delete_row(row_listhead, row_listtail, R);
}

