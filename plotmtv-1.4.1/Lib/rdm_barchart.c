/*
 * rdm_barchart.c - routines to read multi-column barchart data 
 *                  from an MTV-format.
 *
 * The data is assumed to be sandwiched between $DATA=xxx lines
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include "CNplot.h"


/*
 * Bar Chart data has the following format:
 *             "Bar_A"   "Bar_B"   "Bar_C"
 *   "value1"    xa1       xb1       xc1
 *   "value2"    xa2       xb2       xc2
 *   "value3" ...
 *
 * The resulting chart is obtained:
 *
 *         |      C
 *         |  A  |=|                  C
 *         | |=|B| |    A B C       B|=|
 *         | | |=| |   |=|=|=|    A|=| | 
 *         | | | | |   | | | |   |=| | |
 *         | | | | |   | | | |   | | | |
 *         ----------|---------|---------|
 *           value1    value2    value3
 *
 * Each of the intervals "value1", "value2" etc are defined as bins,
 * and each bin contains 1 or more bars.
 *
 * Thus the data-structure contains a linked list of bins
 * Each bin has a name (e.g. "value1" and a linked list of bars
 * Each bar in turn contains a point and a pointer to a name (e.g. "A")
 */    

int CNmtv_read_barchart_data();
static void       read_barchart_options();
static void       barchart_itemno_err();

#define MAXBARS 100    /* Max no of bars per bin */

/* 
 * Read data in barchart format
 * Return an EOF if such is found during the read; otherwise return 0
 */
/*ARGSUSED*/
int CNmtv_read_barchart_data(Dptr, dataID, dataname,
                             filename, fp, lineno, header, verbose)
CNdatasetptr *Dptr;            /* The dataset pointer                  */
int          *dataID;          /* Data ID                              */
char         *dataname;        /* Dataset name                         */
char         *filename;        /* The name of the file/pipe source     */
FILE         *fp;              /* The file/pipe source                 */
int          *lineno;          /* Current line number                  */
char         *header;          /* Header string                        */
int          verbose;          /* Verbosity Flag                       */
{
   char               *CNstrip_keyword();
   CNbinptr           bin_listhead=NULL, bin_listtail=NULL, Binptr;
   CNbinptr           barnamehead=NULL, barnametail=NULL, nameptr;
   CNbarptr           Barptr;
   CNbarchartptr      barchart=NULL;
   CNcontstepptr      cstephead=NULL, csteptail=NULL;
   CNannotptr         annothead=NULL, annottail=NULL;
   CNviewptr          view_pr;
   CNcurve_property   cv_property;
   CNdataset_property ds_property;
   CNplotset_property pt_property;
   char               line[BUFSIZ];
   char               *word[CN_MAXWORDS];
   int                header_found = CN_FALSE;
   int                nargs, nw=0, len, ierr=0, i;
   int                bar_count;
   double             bar_value, bar_min, bar_max;
   char               *bar_name;
   int                fillcolor[MAXBARS], filltype[MAXBARS];
   double             xmin, xmax, ymin, ymax, zmin, zmax;
 
   /* Print info */
   (void) fprintf(stdout,"\n   Reading BARCHART data...(line %d)\n",*lineno);

   /* Initialize */
   *Dptr   = NULL;
   xmin    =  CN_LARGE;
   xmax    = -CN_LARGE;
   ymin    =  0.0;
   ymax    = -CN_LARGE;
   zmin    = -0.5;
   zmax    =  0.5;
   bar_min =  CN_LARGE;
   bar_max = -CN_LARGE;
   for (i=0; i<MAXBARS; i++) filltype[i] =CN_FILL_SOLID;
   for (i=0; i<MAXBARS; i++) fillcolor[i]=2+i;

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
         /* Ignore blanks */
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
         read_barchart_options("BARCHART",line,
                          view_pr,&pt_property,&ds_property, &cv_property,
                          filltype, fillcolor, verbose);
         if ((ds_property.contours) && ((ds_property.flag1 & CNctrlevel)!=0))
         CNset_contstep_levels(&cstephead, &csteptail, &ds_property);

      } else {
         /* Read column data */
         
         nargs = 1;
         if (barnamehead != NULL) nargs = 2;

         if ((nw = CNgetwords(line,word, CN_MAXWORDS)) >= nargs) {
            if (barnamehead == NULL) {
               /* make columns of names */
               for (i=0; i<nw; i++)
                  (void) CNinsert_bin(&barnamehead, &barnametail, 
                                      CNstrip_keyword(word[i]));
            } else {
               /* Allocate a bin */
               Binptr = CNinsert_bin(&bin_listhead, &bin_listtail, 
                                   CNstrip_keyword(word[0]));

               /* Save points as bars */
               bar_count = 1;
               for (nameptr=barnamehead; nameptr!=NULL; nameptr=nameptr->next){
                  /* The name of the bar - only the 1st bin is named */
                  bar_name = nameptr->name;
                  if (bin_listhead != bin_listtail) bar_name = (char *)NULL;
 
                  /* Figure out the value of the bar */
                  bar_value = 0.0;
                  if (bar_count < nw) bar_value = atof(word[bar_count]);
                  if (bar_value < bar_min) bar_min = bar_value;
                  if (bar_value > bar_max) bar_max = bar_value;
                  (void) CNinsert_bar(&(Binptr->barhead),&(Binptr->bartail),
                                      bar_name, bar_value);

                  /* Increment the barcount */
                  bar_count++;
               }
            }
         } else {
            barchart_itemno_err(*lineno,nargs,ierr++);
         }

         /* Free the words */
         CNfreewords(&nw,word);
      }
   }

   /*
   for (i=0; i<10; i++) {
      (void) fprintf(stdout,"i=%d  fillcolor=%d  filltype=%d\n",
                     i,fillcolor[i], filltype[i]);
   }
    */

   /* Now create the barchart */
   if (bin_listhead != NULL) {

      /* Apply the fillcolors and filltypes to the bins/bars */
      for (Binptr=bin_listhead; Binptr!=NULL; Binptr=Binptr->next) {
         i=0;
         for (Barptr=Binptr->barhead; Barptr!=NULL; Barptr=Barptr->next) {
            if (i < MAXBARS) {
               Barptr->fillcolor = fillcolor[i];
               Barptr->filltype  = filltype[i];
            }
            i++;
         }
      }

      /* Create the barchart */
      barchart = CNmake_barchart(bin_listhead, bin_listtail);
      if (barchart == NULL) {
         (void) fprintf(stderr,"Error! Couldn't create barchart!\n");
         CNdelete_bin_list(&bin_listhead, &bin_listtail);
      }

      /* Figure out ymin, ymax */
      if (ds_property.barmin <= bar_min) {    
         /* Probably bar_min >= 0, def-barmin=0 */
         ymin = ds_property.barmin;
         ymax = bar_max + 0.05*(bar_max - bar_min);
      } else if (ds_property.barmin >= bar_max) {  
         /* Probably bar_max <= 0, def-barmin=0 */
         ymin = bar_min - 0.05*(bar_max - bar_min);
         ymax = ds_property.barmin;
      } else {
         ymin = bar_min - 0.05*(bar_max - bar_min);
         ymax = bar_max + 0.05*(bar_max - bar_min);
      }
   }

   /* Now create the dataset */
   if (barchart != NULL) {

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
      *Dptr = CNmake_dataset(filename,dataname,CN_BARCHART,
                            xmin,xmax,ymin,ymax,zmin,zmax,
                            xmin,xmax,ymin,ymax,zmin,zmax,*dataID);
      if ((*Dptr) != NULL) (*dataID)++;

      if ((*Dptr) != NULL) {
         (*Dptr)->barchart  = barchart;
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
         if (verbose) CNprint_dataset((*Dptr), 0);
 
         /* Generate curves of the barchart */
         CNsort_barchart(*Dptr, 0);
      }
   }
         
   /* Failed */
   if ((*Dptr)==NULL) {
      CNdelete_contstep_list(&cstephead, &csteptail);
      CNdelete_annotation_list(&annothead, &annottail);
   }

   /* Delete all the temp structures */
   CNdelete_bin_list(&barnamehead, &barnametail);

   /* Reset the property structures */
   CNdelete_plotset_property_fields(&pt_property);
   CNdelete_dataset_property_fields(&ds_property);
   CNdelete_curve_property_fields(&cv_property);

   /* Free the view structure */
   CNdelete_view(view_pr);

   /* Print out info */
   if ((*Dptr) != NULL)
   (void) fprintf(stdout,"   BARCHART : Read %d bars\n",
                  CNcount_curves((*Dptr)->curvehead, (*Dptr)->curvetail));
   else
   (void) fprintf(stdout,"   BARCHART : Format error - couldn't read data!\n");
   
   /* return */
   if (!header_found)
      return(EOF);
   else
      return(header_found);
}



/*
 * READING UTILITIES FOR BARCHARTS
 */

/* 
 * Read a line and apply options to a the bars
 */
/*ARGSUSED*/
static void read_barchart_options(format,header,
                                  view_pr,pt_prop,ds_prop,cv_prop,
                                  filltype, fillcolor, vbs)
char               *format;
char               *header;
CNviewptr          view_pr;
CNplotset_property *pt_prop;
CNdataset_property *ds_prop;
CNcurve_property   *cv_prop;
int                *filltype, *fillcolor;
int                vbs;
{
   char *argtbl[CN_MAXWORDS], *valtbl[CN_MAXWORDS];
   char *words[CN_MAXWORDS];
   char newheader[CN_MAXCHAR];
   int  nargs = 0, nvals = 0;
   int  argfound, i, len;

   /* Screen first for "%fillcolor 1 2 3..." lines */
   (void) strcpy(newheader, header);
   len = strlen(newheader);
   if (len > 0) len = CNuncomment_line(newheader, CN_MAXCHAR);
   if (len > 0) {
      
      if ((nargs = CNgetwords(header,words,CN_MAXWORDS)) >= 2) {
         argfound = CN_TRUE;
         if (strcmp(words[0],"fillcolor")==0) {
            for (i=1; i<nargs && i<MAXBARS; i++)
               fillcolor[i-1] = atoi(words[i]);
         } else if (strcmp(words[0],"filltype")==0) {
            for (i=1; i<nargs && i<MAXBARS; i++)
               filltype[i-1] = atoi(words[i]);
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

      /* Look for plotset/dataset/curve arguments */
      for (i=0; i<nargs; i++) {
         if (!CNparse_view_property       (view_pr,argtbl[i],valtbl[i],vbs))
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
static void barchart_itemno_err(lineno,itemno,ierr)
int  lineno,itemno,ierr;
{
   (void) fprintf(stderr,"   warning (line %3d) %8s: ",lineno,"BARCHART");
   (void) fprintf(stderr,"need at least %d items\n",itemno);
}


