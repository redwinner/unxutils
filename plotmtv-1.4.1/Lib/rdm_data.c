/*
 * rdm_data.c - read MTV-format data from a file 
 *
 * The data is read and saved in the
 * "CNdataset" data-structure, pointed to by "CNdatasetptr".  
 *
 * The datasets are assumed to be separated using a $DATA=xxx line
 */

/*
 * #ifdef INTEL_ONLY are used to denote sections of code that have been
 * disabled.  These are primarily the hierarchical mesh formats which
 * are specialized for process/device modeling
 * Also the XDR machine-independent format has been disabled.
 *
 * The ifdef statements are left here to allow easier comparison with
 * intel specific code which is not available for public distribution
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "CNplot.h"

void CNread_plotmtv();
void CNread_plotmtv_from_file();
void CNread_plotmtv_from_pipe();
void CNread_plotmtv_data();
static int read_datatype();
static int read_data();
static int CNmtv_read_unrecog_data();


/*
 * read the ascii/binary data from a file/pipe
 * return NULL upon failure
 *
 * A single data-file or stream could contain more than one dataset.
 * This collection of routines reads the data and puts each separate
 * set of data into a linked list of datasets.  The global_dataset
 * linked list is used ONLY for reading mesh quantities, which require 
 * a matching mesh structure (presumably inside the global list).
 */
void CNread_plotmtv(filename, fp, source, 
                    global_datahead, global_datatail,
                    datahead, datatail, dataID, verbose)
char         *filename;        /* The name of the file                 */
FILE         *fp;              /* The pipe descriptor                  */
int          source;           /* Data source : CN_FILE or CN_PIPE     */
CNdatasetptr global_datahead;  /* Pointers to existing plot data       */
CNdatasetptr global_datatail;  /* Pointers to existing plot data       */
CNdslistptr  *datahead;        /* Pointers to plot data                */
CNdslistptr  *datatail;        /* Pointers to plot data                */
int          *dataID;          /* data ID                              */
int          verbose;          /* Verbosity flag                       */
{
   /* Initialize */
   *datahead = NULL;
   *datatail = NULL;

   /* Branch according to file/pipe source */
   if (source == CN_FILE) {

      /* Read from file */
      CNread_plotmtv_from_file(filename, 
                               global_datahead, global_datatail,
                               datahead, datatail, dataID, verbose);

   } else {

      /* Read from pipe */
      CNread_plotmtv_from_pipe(fp, 
                               global_datahead, global_datatail,
                               datahead, datatail, dataID, verbose);

   }

}


/*
 * read the data from a file 
 * return NULL upon failure
 */
void CNread_plotmtv_from_file(filename, 
                              global_datahead, global_datatail,
                              datahead, datatail, dataID, verbose)
char         *filename;        /* The name of the file                 */
CNdatasetptr global_datahead;  /* Pointers to existing plot data       */
CNdatasetptr global_datatail;  /* Pointers to existing plot data       */
CNdslistptr  *datahead;        /* Pointers to plot data                */
CNdslistptr  *datatail;        /* Pointers to plot data                */
int          *dataID;          /* data ID                              */
int          verbose;          /* Verbosity flag                       */
{
   FILE         *fp;
   int          fp_ispipe=0;

   /* open the file */
   if (!CNopen_file(filename,&fp,&fp_ispipe)) return;

   /* Now process the file */
   CNread_plotmtv_data(fp,filename,
                       global_datahead, global_datatail,
                       datahead,datatail,dataID,verbose);

   /* close the file */
   CNclose_file(fp, fp_ispipe);

   /* Print out warning message */
   if (*datahead == NULL) 
   (void) fprintf(stderr,"   ***Error! PlotMTV file-read was unsuccessful.\n");
}


/* 
 * read the data from a pipe 
 * return NULL upon failure
 */
void CNread_plotmtv_from_pipe(fp, 
                              global_datahead, global_datatail,
                              datahead, datatail, dataID, verbose)
FILE         *fp;              /* The pipe-descriptor                  */
CNdatasetptr global_datahead;  /* Pointers to existing plot data       */
CNdatasetptr global_datatail;  /* Pointers to existing plot data       */
CNdslistptr  *datahead;        /* Pointers to plot data                */
CNdslistptr  *datatail;        /* Pointers to plot data                */
int          *dataID;          /* data ID                              */
int          verbose;          /* Verbosity flag                       */
{
   /* check the pipe */
   if (fp == NULL) {
      (void) fprintf(stderr,"Error! Cannot read from NULL pipe!\n");
      return;
   }

   /* Now process the pipe */
   CNread_plotmtv_data(fp,"PIPE",
                       global_datahead, global_datatail,
                       datahead,datatail,dataID,verbose);

   /* Print out warning message */
   if (*datahead == NULL) 
   (void) fprintf(stderr,"   ***Error! PlotMTV pipe-read was unsuccessful.\n");
}

#define XDRDATA -3
#define QUIT    -2
#define UNDEFN   0
#define CURVE2D  1
#define CURVE3D  2
#define CONTCURV 3
#define RECTGRID 4
#define GRID4D   5
#define MESH4D   6
#define POLYGON  7
#define VECTOR   8
#define MESH     9
#define COLUMN   10
#define PROBAB   11
#define HISTOG   12
#define BARCHART 13
#define UNRECOG  14

/* 
 * read in the data 
 * return NULL upon failure
 */
void CNread_plotmtv_data(fp, filename, 
                         global_datahead, global_datatail,
                         datahead, datatail, dataID, verbose)
FILE         *fp;              /* File pointer                         */
char         *filename;        /* The name of the file/pipe source     */
CNdatasetptr global_datahead;  /* Pointers to existing plot data       */
CNdatasetptr global_datatail;  /* Pointers to existing plot data       */
CNdslistptr  *datahead;        /* Pointers to plot data                */
CNdslistptr  *datatail;        /* Pointers to plot data                */
int          *dataID;          /* Data ID                              */
int          verbose;          /* Verbosity flag                       */
{
   char         header[BUFSIZ];
   char         dataname[BUFSIZ];
   int          lineno = 0, datatype, status=0;
   int          datacount=0;

   /*
    * The file format consists of :
    *    $ DATA=XX            # header lines
    *    x y                  # data
    *    # comments
    *    % options
    * Everytime a "$" sign is encountered the routine branches to
    * the appropiate reader.  The reader is passed the header arguments 
    * and the file-pointer, and it ends when a new line starting with
    * a "$" sign is encountered.
    */

   /* Print out some information */
   (void) fprintf(stdout,"\nReading MTV data from file \"%s\" : \n",filename);

   /* Count the initial number of datasets - should be an empty list */
   datacount = CNcount_dslists(*datahead, *datatail);

   /*
    * Use CNread_header() to scan through the lines
    * Find the first header ("$") or the first occurence of a '$' or
    * an alphanumeric character is encountered.
    * The '$' and alphanumeric characters are put back on the stream.
    */
   (void) strcpy(header,"");
   while ((status=CNread_header(fp,&lineno,header,BUFSIZ))!=EOF && status)
      /*EMPTY*/
      ;

   while (status != EOF && status != QUIT && status != XDRDATA) {
      /* Read the data-type */
      (void) strcpy(dataname, CN_NONAME);
      status=read_datatype(header,&datatype,dataname,verbose);
      if (status == QUIT) continue;
      if (status == XDRDATA) continue;

      /* Check the dataname */
      if (strcmp(dataname, CN_NONAME)==0) 
      (void) sprintf(dataname,"%s.%d",CNstring_concat(filename),
                     (*dataID)-datacount);

      /* Read the data - this stops at EOF or at another "$ xxx " line */
      status = read_data(fp, filename, &lineno, header,
                         global_datahead, global_datatail,
                         datahead, datatail, dataID,
                         datatype, dataname, verbose);
   } /* End while */

#ifdef INTEL_ONLY
   if (status == XDRDATA) {
      /*
       * Read in XDR format 
       */
      CNread_plotxdr_data(fp,filename,
                       global_datahead, global_datatail,
                       datahead,datatail,dataID,verbose);
   }
#endif

   /* Print more information */
   (void) fprintf(stdout,"Found %d datasets.\n",
                  CNcount_dslists(*datahead, *datatail)-datacount);
}

typedef struct {
  char *label;
  int   value;
} MPdata_format;

static MPdata_format format_pairs[] = {
   {"curve2d"      , CURVE2D },
   {"2d"           , CURVE2D },
   {"twod"         , CURVE2D },
   {"curve3d"      , CURVE3D },
   {"3d"           , CURVE3D },
   {"threed"       , CURVE3D },
   {"contcurv"     , CONTCURV},
   {"contcurve"    , CONTCURV},
   {"contourcurve" , CONTCURV},
   {"contour-curve", CONTCURV},
   {"contour"      , RECTGRID},
   {"rectgrid"     , RECTGRID},
   {"grid4d"       , GRID4D  },
   {"4d"           , GRID4D  },
#ifdef INTEL_ONLY
   {"mesh4d"       , MESH4D  },
   {"stride"       , MESH4D  },
#endif
   {"poly"         , POLYGON },
   {"polygon"      , POLYGON },
   {"vector"       , VECTOR  },
   {"vect"         , VECTOR  },
#ifdef INTEL_ONLY
   {"mesh"         , MESH    },
   {"mtvmesh"      , MESH    },
   {"pifmesh"      , MESH    },
#endif
   {"column"       , COLUMN  },
   {"prob"         , PROBAB  },
   {"probab"       , PROBAB  },
   {"probability"  , PROBAB  },
   {"hist"         , HISTOG  },
   {"histog"       , HISTOG  },
   {"histogram"    , HISTOG  },
   {"bar"          , BARCHART},
   {"barchart"     , BARCHART},
};

/*
 * Read the data-type 
 */
static int read_datatype(header,datatype,dataname,verbose)
char *header;     /* header ($data=xxx) */
int  *datatype;   /* Data type          */
char *dataname;   /* Dataset name/label */
int  verbose;     /* Verbose flag       */
{
   char *argtbl[CN_MAXWORDS], *valtbl[CN_MAXWORDS];
   char newheader[CN_MAXCHAR];
   int  nargs = 0, nvals = 0;
   int  i, j, npairs, status=0, format_found=CN_FALSE, dataspec=CN_FALSE;

   /* Initialize the datatype */
   *datatype = CURVE2D;

   /* CNparse_line wants "command arg=val arg=val" so create a new header */
   (void) sprintf(newheader, "datafile %s",header);

   /* Get the argument-value pairs from the line */
   if (CNparse_line(newheader, CN_MAXCHAR,
                    &nargs, argtbl, CN_MAXWORDS,
                    &nvals, valtbl, CN_MAXWORDS)) {

      /* Go thru the arg-val pairs */
      for (i=0; i<nargs; i++) {
         /*
         (void)fprintf(stdout,"argument=%s value=%s\n",argtbl[i],valtbl[i]);
          */

         if ((strcmp(argtbl[i],"end")==0) || (strcmp(argtbl[i],"quit")==0))
            status = QUIT;

#ifdef INTEL_ONLY
         if (strncmp(argtbl[i],"xdr",3)==0)
            status = XDRDATA;
#endif

         else if (strcmp(argtbl[i],"name")==0) 
            CNassign_string_keyword(dataname,valtbl[i],argtbl[i],verbose);

         else if (strncmp(argtbl[i],"data",4)==0) {
            dataspec = CN_TRUE;
            CNstring_to_lower(valtbl[i]);
            /* Do the comparison */
            format_found = CN_FALSE;
            npairs = sizeof(format_pairs)/sizeof(MPdata_format);
            for (j=0; j<npairs && !format_found; j++) {
               if (strcmp(valtbl[i],format_pairs[j].label)==0) {
                  *datatype = format_pairs[j].value;
                  format_found = CN_TRUE;
               }
            }
            if (!format_found) {
               (void) fprintf(stdout,
               "\n   ***Warning : unrecognized data-type \"%s\"", valtbl[i]);
               *datatype = UNRECOG; 
            }
         }
      }

      /* Clear the tables */
      CNfreewords(&nargs, argtbl);
      CNfreewords(&nvals, valtbl);
   }

   /* Read the first argument-value pair following the % */
   if (verbose && !dataspec && status!=QUIT && status!=XDRDATA) {
      (void) fprintf(stdout,"   Warning : No data-type specified! : ");
      (void) fprintf(stdout,"Assuming CURVE2D format...\n");
      *datatype = CURVE2D;
   }

   /* Return the status (QUIT) */
   return (status);
}

/* 
 * Read the data 
 */
static int read_data(fp, filename, lineno, header,
                     global_datahead, global_datatail,
                     datahead, datatail, dataID,
                     datatype, dataname, verbose)
FILE         *fp;              /* The file/pipe source                 */
char         *filename;        /* The name of the file/pipe source     */
int          *lineno;          /* Current line number                  */
char         *header;          /* Header string                        */
CNdatasetptr global_datahead;  /* Pointers to existing plot data       */
CNdatasetptr global_datatail;  /* Pointers to existing plot data       */
CNdslistptr  *datahead;        /* Pointers to plot data                */
CNdslistptr  *datatail;        /* Pointers to plot data                */
int          *dataID;          /* Data ID                              */
int          datatype;         /* Type of data                         */
char         *dataname;        /* Name/Label of data                   */
int          verbose;          /* Verbosity Flag                       */
{
   CNdatasetptr Dptr=NULL;
   int          status;
   int          stopatE = CN_FALSE;

   /*
    * Branch according to the reader 
    * Mesh-type datasets are treated differently because multiple
    * datasets could be contained in a single data definition
    * (mesh structure and multiple quantities)
    */
   switch (datatype) {
   case CURVE2D  : status = CNmtv_read_curve2D_data  (&Dptr,
                            dataID, dataname, filename,
                            fp,lineno,header,verbose); 
                   break;
   case CURVE3D  : status = CNmtv_read_curve3D_data  (&Dptr,
                            dataID, dataname, filename,
                            fp,lineno,header,verbose); 
                   break;
   case CONTCURV : status = CNmtv_read_contcurve_data(&Dptr,
                            dataID, dataname, filename,
                            fp,lineno,header,verbose); 
                   break;
   case RECTGRID : status = CNmtv_read_contour_data  (&Dptr,
                            dataID, dataname, filename,
                            fp,lineno,header,verbose); 
                   break;
   case GRID4D   : status = CNmtv_read_grid4D_data   (&Dptr,
                            dataID, dataname, filename,
                            fp,lineno,header,verbose); 
                   break;
#ifdef INTEL_ONLY
   case MESH4D   : status = CNmtv_read_mesh4D_data(
                            datahead, datatail,
                            dataID, dataname, filename,
                            fp,lineno,header,verbose); 
                   Dptr   = NULL;  /* Don't do anything more to the data */
                   break;
#endif
   case POLYGON  : status = CNmtv_read_polygon_data(&Dptr,
                            dataID, dataname, filename,
                            fp,lineno,header,verbose); 
                   break;
   case VECTOR   : status = CNmtv_read_vector_data   (&Dptr,
                            dataID, dataname, filename,
                            fp,lineno,header,verbose); 
                   break;
#ifdef INTEL_ONLY
   case MESH     : status = CNmtv_read_mtvmesh_data  (
                            global_datahead, global_datatail,
                            datahead, datatail,
                            dataID, dataname, filename,
                            fp,lineno,header,stopatE,verbose); 
                   Dptr   = NULL;  /* Don't do anything more to the data */
                   break;
#endif
   case COLUMN   : status = CNmtv_read_column_data(
                            datahead, datatail,
                            dataID, dataname, filename,
                            fp,lineno,header,verbose); 
                   Dptr   = NULL;  /* Don't do anything more to the data */
                   break;
   case PROBAB   : status = CNmtv_read_probab_data(&Dptr,
                            dataID, dataname, filename,
                            fp,lineno,header,verbose); 
                   break;
   case HISTOG   : status = CNmtv_read_histogram_data(&Dptr,
                            dataID, dataname, filename,
                            fp,lineno,header,verbose); 
                   break;
   case BARCHART : status = CNmtv_read_barchart_data(&Dptr,
                            dataID, dataname, filename,
                            fp,lineno,header,verbose); 
                   break;
   case UNRECOG  : status = CNmtv_read_unrecog_data(fp,lineno,header);
                   break;
   default       : (void) fprintf(stderr,
                   "Error! Unrecognized data-type \"%d\n",datatype);
                   break;
   }
 
   /* Store the dataset in the local linked list */
   if (Dptr != NULL) (void) CNinsert_dslist(datahead, datatail, Dptr);
 
   /* Return the status */
   return(status);
}


/*
 * Just go thru the lines until we get to a $ or EOF
 */
static int CNmtv_read_unrecog_data(fp,lineno,header)
FILE         *fp;            /* The file/pipe source                 */
int          *lineno;        /* Current line number                  */
char         *header;        /* Header string                        */
{
   char        line[BUFSIZ];
   int         header_found  = CN_FALSE;
   int         len;

   /* Print info */
   (void) fprintf(stdout,"\n   Skipping UNKNOWN data...(line %d)\n",*lineno);

   /* Keep on reading until a "$" is encountered */
   while (!header_found && CNgetucline(fp, line, lineno, BUFSIZ) != EOF) {

      /*EMPTY*/
      if (((len=strlen(line)) == 0) || (len==1 && line[0]=='\n')) {
         /* Ignore newlines */
         ;
 
      } else if (line[0] == '$') {
         /* Header */
         line[0] = ' ';
         (void) strcpy(header, line);
         header_found = CN_TRUE;

      } 
   } 

   /* return */
   if (!header_found)
      return(EOF);
   else
      return(header_found);
}
