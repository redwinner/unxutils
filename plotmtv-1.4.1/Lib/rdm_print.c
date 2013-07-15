/*
 * rdm_print.c - write MTV-format data to a file 
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

static void modify_dataset_list();
static void modify_mesh_dataset_list();
static void modify_mesh4D_dataset_list();
static void write_curve_data();
static void write_gdcontour_data();
static void write_cvcontour_data();
static void write_grid4D_data();
static void write_vector_data();
static void write_probab_data();
static void write_histogram_data();
static void write_barchart_data();
static void write_polygon_data();
static void write_mesh4D_data();
static void write_mesh_data();
static void write_quant_data();
static void write_quantity_data();

/*
 * write the ascii/binary data to a file/pipe
 * return NULL upon failure
 *
 * A single data-file or stream could contain more than one dataset.
 * This collection of routines reads the data and puts each separate
 * set of data into a linked list of datasets.  The global_dataset
 * linked list is used ONLY for reading mesh quantities, which require
 * a matching mesh structure (presumably inside the global list).
 */
void CNwrite_plotmtv(filename, fp, source,
                     global_datahead, global_datatail, 
                     datahead, datatail, binary, xdrbinary, verbose)
char         *filename;        /* The name of the file                 */
FILE         *fp;              /* The pipe descriptor                  */
int          source;           /* Data source : CN_FILE or CN_PIPE     */
CNdatasetptr global_datahead;  /* Pointers to existing plot data       */
CNdatasetptr global_datatail;  /* Pointers to existing plot data       */
CNdslistptr  *datahead;        /* Pointers to plot data                */
CNdslistptr  *datatail;        /* Pointers to plot data                */
int          binary;           /* Write in binary format               */
int          xdrbinary;        /* Write in xdrbinary format            */
int          verbose;          /* Verbosity flag                       */
{
   /* Branch according to file/pipe source */
   if (source == CN_FILE) {
 
      /* Write to file */
      CNwrite_plotmtv_to_file(filename,
                              global_datahead, global_datatail,
                              datahead, datatail, 
                              binary, xdrbinary, verbose);
 
   } else {
 
      /* Write to pipe */
      CNwrite_plotmtv_to_pipe(fp,
                              global_datahead, global_datatail,
                              datahead, datatail, 
                              binary, xdrbinary, verbose);
   }
}

/*
 * Write the data to a file
 * return NULL upon failure
 */
void CNwrite_plotmtv_to_file(filename, 
                             global_datahead, global_datatail,
                             datahead, datatail, binary, xdrbinary, verbose)
char         *filename;        /* The name of the file                 */
CNdatasetptr global_datahead;  /* Pointers to existing plot data       */
CNdatasetptr global_datatail;  /* Pointers to existing plot data       */
CNdslistptr  *datahead;        /* Pointers to plot data                */
CNdslistptr  *datatail;        /* Pointers to plot data                */
int          binary;           /* Write in binary format               */
int          xdrbinary;        /* Write in xdrbinary format            */
int          verbose;          /* Verbosity flag                       */
{
   FILE *fp, *fopen();
 
   /* Open the outputfile */
   if ((fp =  fopen(filename,"w")) == NULL) {
      (void) fprintf(stderr,"cat: can't open %s\n", filename);
      return;
   }
 
   /* Now process the file */
   CNwrite_plotmtv_data(fp,filename,
                        global_datahead, global_datatail,
                        datahead,datatail,
                        binary, xdrbinary, verbose);
 
   /* close the file */
   (void) fclose(fp);
}

/*
 * Write the data to a pipe
 * return NULL upon failure
 */
void CNwrite_plotmtv_to_pipe(fp,
                             global_datahead, global_datatail,
                             datahead, datatail, binary, xdrbinary, verbose)
FILE         *fp;              /* File pointer                         */
CNdatasetptr global_datahead;  /* Pointers to existing plot data       */
CNdatasetptr global_datatail;  /* Pointers to existing plot data       */
CNdslistptr  *datahead;        /* Pointers to plot data                */
CNdslistptr  *datatail;        /* Pointers to plot data                */
int          binary;           /* Write in binary format               */
int          xdrbinary;        /* Write in xdrbinary format            */
int          verbose;          /* Verbosity flag                       */
{
   /* check the pipe */
   if (fp == NULL) {
      (void) fprintf(stderr,"Error! Cannot write to NULL pipe!\n");
      return;
   }
 
   /* Now process the pipe */
   CNwrite_plotmtv_data(fp,"PIPE",
                        global_datahead, global_datatail,
                        datahead,datatail,
                        binary, xdrbinary, verbose);
}

/*
 * read the ascii/binary data from a file/pipe
 * return NULL upon failure
 */
/*ARGSUSED*/
void CNwrite_plotmtv_data(fp, filename, 
                          global_datahead, global_datatail, 
                          datahead, datatail, binary, xdrbinary, verbose)
FILE         *fp;              /* File pointer                         */
char         *filename;        /* The name of the file                 */
CNdatasetptr global_datahead;  /* Pointers to existing plot data       */
CNdatasetptr global_datatail;  /* Pointers to existing plot data       */
CNdslistptr  *datahead;        /* Pointers to plot data                */
CNdslistptr  *datatail;        /* Pointers to plot data                */
int          binary;           /* Write in binary format               */
int          xdrbinary;        /* Write in xdrbinary format            */
int          verbose;          /* Verbosity flag                       */
{
   char         time[CN_MAXCHAR];
   char         sysname[CN_MAXCHAR];
   char         hostname[CN_MAXCHAR];
   char         datafile[CN_MAXCHAR];
   char         dataname[CN_MAXCHAR];
   int          dataID=0;
   CNdslistptr  new_datahead=NULL, new_datatail=NULL, ds;

   /* Print out initial message */
   CNget_localtime(time,CN_MAXCHAR);
   CNget_hostname(hostname,CN_MAXCHAR);
   CNget_sysname(sysname);
   (void) fprintf(fp,"# \n");
   (void) fprintf(fp,"# MTVDAT Plot Format, generated on:\n");
   (void) fprintf(fp,"#    System   : %s\n",sysname);
   (void) fprintf(fp,"#    Machine  : %s\n",hostname);
   (void) fprintf(fp,"#    Run Date : %s\n",time);
   (void) fprintf(fp,"# \n");

   /* Modify the dataset list */
   modify_dataset_list(global_datahead, global_datatail,
                       *datahead, *datatail,
                       &new_datahead, &new_datatail, verbose);

#ifdef INTEL_ONLY
   /*
    * Write in XDR binary
    */
   if (xdrbinary) {
      /* Print header */
      (void) fprintf(fp,"$ XDR\n");

      /* Now process the file */
      CNwrite_plotxdr_data(fp,filename,
                       global_datahead, global_datatail,
                       &new_datahead, &new_datatail, &dataID, verbose);

      /* Remove the new dataset list */
      CNdelete_dslist_list(&new_datahead, &new_datatail);

      /* Done */
      return;
   }
#endif

   /*
    * Normal write (Ascii and machine-dependent binary)
    */

   /* Print out the datasets one at a time */
   for (ds=new_datahead; ds!=NULL; ds=ds->next) {
      if (ds->Dptr == NULL) continue;

      /* Set the dataname */
      if (ds->Dptr->label) {
         (void) strcpy(dataname, ds->Dptr->label);
         if (ds->Dptr->filename) {
            (void) strcpy(datafile, CNstring_concat(ds->Dptr->filename));
            if (strncmp(dataname, datafile, strlen(datafile))==0) 
               (void) strcpy(dataname, "");
         }   
      } else {
         (void) strcpy(dataname, "");
      }

      switch(ds->Dptr->datatype) {
      case CN_PLOT2D    : write_curve_data(fp,ds->Dptr,dataname,1,"CURVE2D",
                                           binary, verbose);
                          break;
      case CN_PLOT3D    : write_curve_data(fp,ds->Dptr,dataname,0,"CURVE3D",
                                           binary, verbose);
                          break;
      case CN_CONTOUR   : /* do pif-quantities differently */
                          if (ds->Dptr->parent)
                          write_quant_data(fp,ds->Dptr,"MTVMESH",
                                           binary,verbose);
                          else if (ds->Dptr->grid) 
                          write_gdcontour_data(fp,ds->Dptr,dataname,"CONTOUR",
                                           binary, verbose);
                          else
                          write_cvcontour_data(fp,ds->Dptr,dataname,"CONTCURVE",
                                           binary, verbose);
                          break;
      case CN_GRID4D    : write_grid4D_data(fp,ds->Dptr,dataname,"GRID4D",
                                           binary, verbose);
                          break;
      case CN_VECTOR    : /* do pif-quantities differently */
                          if (ds->Dptr->parent)
                          write_quant_data(fp,ds->Dptr,"MTVMESH",
                                           binary,verbose);
                          else
                          write_vector_data(fp,ds->Dptr,dataname,"VECTOR",
                                           binary, verbose);
                          break;
      case CN_PROBAB    : write_probab_data(fp,ds->Dptr,dataname,"PROBABILITY",
                                           binary, verbose);
                          break;
      case CN_HISTOGRAM : write_histogram_data(fp,ds->Dptr,dataname,"HISTOGRAM",
                                           binary, verbose);
                          break;
      case CN_BARCHART  : write_barchart_data(fp,ds->Dptr,dataname,"BARCHART",
                                           binary, verbose);
                          break;
      case CN_POLYGON   : write_polygon_data(fp,ds->Dptr,dataname,"POLYGON",
                                           binary,verbose);
                          break;
      case CN_MESH4D_P  : write_mesh4D_data(fp,ds->Dptr,dataname,"MESH4D",
                                           binary,verbose);
                          break;
      case CN_PIF_PARENT: write_mesh_data(fp,ds->Dptr,"MTVMESH",
                                           binary,verbose);
                          break;
      case CN_PIF_CHILD : write_quant_data(fp,ds->Dptr,"MTVMESH",
                                           binary,verbose);
                          break;
      default           : break;
      }
   }

   /* Write the tail*/
   (void) fprintf(fp,"$ END\n");

   /* Remove the new dataset list */
   CNdelete_dslist_list(&new_datahead, &new_datatail);
}


/*
 * Create a new dslist of datasets to be saved.
 * This is necessary because several MESH-based datasets can be derived from
 * a single quantity.
 */
/*ARGSUSED*/
static void modify_dataset_list(global_datahead, global_datatail,
                                old_datahead, old_datatail, 
                                new_datahead, new_datatail, verbose)
CNdatasetptr global_datahead;  /* Pointers to existing plot data       */
CNdatasetptr global_datatail;  /* Pointers to existing plot data       */
CNdslistptr  old_datahead;     /* Pointers to plot data                */
CNdslistptr  old_datatail;     /* Pointers to plot data                */
CNdslistptr  *new_datahead;    /* Pointers to plot data                */
CNdslistptr  *new_datatail;    /* Pointers to plot data                */
int          verbose;          /* Verbosity flag                       */
{
   CNdslistptr tmp_datahead=NULL, tmp_datatail=NULL;

   /* Modify the mesh dataset list */
   modify_mesh_dataset_list(global_datahead, global_datatail,
                            old_datahead, old_datatail,
                            &tmp_datahead, &tmp_datatail, verbose);

   /* Modify the mesh4D dataset list */
   modify_mesh4D_dataset_list(global_datahead, global_datatail,
                            tmp_datahead, tmp_datatail,
                            new_datahead, new_datatail, verbose);

   /* Remove the tmp dslist */
   CNdelete_dslist_list(&tmp_datahead, &tmp_datatail);
}


/*
 * Create a new dslist of datasets to be saved.
 * This is necessary because several MESH-based datasets can be derived from
 * a single quantity.
 * This relies on the relationship between mesh and quantity datasets
 */
/*ARGSUSED*/
static void modify_mesh_dataset_list(global_datahead, global_datatail,
                                     old_datahead, old_datatail, 
                                     new_datahead, new_datatail, verbose)
CNdatasetptr global_datahead;  /* Pointers to existing plot data       */
CNdatasetptr global_datatail;  /* Pointers to existing plot data       */
CNdslistptr  old_datahead;     /* Pointers to plot data                */
CNdslistptr  old_datatail;     /* Pointers to plot data                */
CNdslistptr  *new_datahead;    /* Pointers to plot data                */
CNdslistptr  *new_datatail;    /* Pointers to plot data                */
int          verbose;          /* Verbosity flag                       */
{
   CNdatasetptr dptr;
   CNdslistptr  ds, dsa, dsmesh;
   CNdslistptr  mesh_dshead=NULL, mesh_dstail=NULL;
   int          KEEP_MESH=CN_FALSE;
   int          QUANT_FOUND=CN_FALSE;

   /* Clean up the new list */
   CNdelete_dslist_list(new_datahead, new_datatail);

   /* Reset the flags on the dslist */
   for (ds=old_datahead; ds!=NULL; ds=ds->next) ds->flag = 0;

   /* 
    * Search the global list for mesh-datasets 
    * and add to the list
    */
   for (dptr=global_datahead; dptr!=NULL; dptr=dptr->next) {
      if (dptr->datatype == CN_PIF_PARENT) {
         dsmesh = CNinsert_dslist(&mesh_dshead, &mesh_dstail, dptr);

         /* Save quantity datasets in the dslist that match the mesh */
         KEEP_MESH = CN_FALSE;
         for (ds=old_datahead; ds!=NULL; ds=ds->next) {
            if ((ds->flag==0) && 
                (ds->Dptr->parent != NULL) &&
                (ds->Dptr->parent == dptr)) {

               /* This element has been checked */
               ds->flag = 1;

               /* Make sure the dptr's quantity hasn't already been saved */
               QUANT_FOUND = CN_FALSE;
               for (dsa=mesh_dshead; dsa!=NULL && !QUANT_FOUND; dsa=dsa->next)
                  if (ds->Dptr->quantity && 
                      (ds->Dptr->quantity == dsa->Dptr->quantity))
                     QUANT_FOUND = CN_TRUE;

               if (!QUANT_FOUND) {
                  (void) CNinsert_dslist(&mesh_dshead, &mesh_dstail, ds->Dptr);
                  KEEP_MESH = CN_TRUE;
               }
            }
         }
        
         /* Check to see if the mesh is to be saved */
         for (ds=old_datahead; ds!=NULL; ds=ds->next) {
            if ((ds->Dptr->datatype==CN_PIF_PARENT) && (ds->Dptr==dptr)) {
               ds->flag = 1;
               if (!KEEP_MESH) KEEP_MESH = CN_TRUE;
            }
         }

         /*
          * If no quant-datasets were found that match the mesh, delete
          * the mesh from the "must-save" list 
          */
         if (!KEEP_MESH)
         CNdelete_dslist(&mesh_dshead, &mesh_dstail, dsmesh);
      }
   }

   /* Now print the mesh-datasets */
   if (verbose)
   CNprint_dslist_list(mesh_dshead, mesh_dstail);

   /* 
    * Now combine the mesh-dataset list and the dslist 
    */
   for (ds=old_datahead; ds!=NULL; ds=ds->next) {
      if (ds->Dptr && ds->flag==0) 
      (void) CNinsert_dslist(new_datahead, new_datatail, ds->Dptr);
   }
   for (ds=mesh_dshead; ds!=NULL; ds=ds->next) {
      if (ds->Dptr) 
      (void) CNinsert_dslist(new_datahead, new_datatail, ds->Dptr);
   }

   /* Now print the new dataset list */
   if (verbose)
   CNprint_dslist_list(*new_datahead, *new_datatail);

   /* Delete the mesh list */
   CNdelete_dslist_list(&mesh_dshead, &mesh_dstail);
}


/*
 * Create a new dslist of datasets to be saved.
 * This is necessary because several MESH-based datasets can be derived from
 * a single quantity.
 * This relies on the relationship between MESH4D_P and MESH4D_C datasets
 */
/*ARGSUSED*/
static void modify_mesh4D_dataset_list(global_datahead, global_datatail,
                                       old_datahead, old_datatail, 
                                       new_datahead, new_datatail, verbose)
CNdatasetptr global_datahead;  /* Pointers to existing plot data       */
CNdatasetptr global_datatail;  /* Pointers to existing plot data       */
CNdslistptr  old_datahead;     /* Pointers to plot data                */
CNdslistptr  old_datatail;     /* Pointers to plot data                */
CNdslistptr  *new_datahead;    /* Pointers to plot data                */
CNdslistptr  *new_datatail;    /* Pointers to plot data                */
int          verbose;          /* Verbosity flag                       */
{
   CNdatasetptr dptr;
   CNdslistptr  ds;
   CNdslistptr  mesh_dshead=NULL, mesh_dstail=NULL;
   int          KEEP_MESH=CN_FALSE;

   /* Clean up the new list */
   CNdelete_dslist_list(new_datahead, new_datatail);

   /* Reset the flags on the dslist */
   for (ds=old_datahead; ds!=NULL; ds=ds->next) ds->flag = 0;

   /* 
    * Search the global list for mesh-datasets 
    * and add to the list
    */
   for (dptr=global_datahead; dptr!=NULL; dptr=dptr->next) {
      if (dptr->datatype == CN_MESH4D_P) {

         /*
          * The mesh is saved if it is in the to-save list or
          * if one of its children is in the to-save list
          */

         /* Flag child datasets in the dslist that match the mesh */
         KEEP_MESH = CN_FALSE;
         for (ds=old_datahead; ds!=NULL; ds=ds->next) {
            if ((ds->flag==0) && 
                (ds->Dptr->parent != NULL) &&
                (ds->Dptr->parent == dptr) &&
                (ds->Dptr->datatype == CN_MESH4D_C)) {

               /* This element has been checked */
               ds->flag = 1;

               /* Flag this mesh (to be saved) */
               KEEP_MESH = CN_TRUE;
            }
         }
        
         /* Check to see if the mesh is to be saved */
         for (ds=old_datahead; ds!=NULL; ds=ds->next) {
            if ((ds->Dptr->datatype==CN_MESH4D_P) && (ds->Dptr==dptr)) {
               ds->flag = 1;
               if (!KEEP_MESH) KEEP_MESH = CN_TRUE;
            }
         }

         /*
          * Save the mesh
          */
         if (KEEP_MESH)
         (void) CNinsert_dslist(&mesh_dshead, &mesh_dstail, dptr);
      }
   }

   /* Now print the mesh-datasets */
   if (verbose)
   CNprint_dslist_list(mesh_dshead, mesh_dstail);

   /* 
    * Now combine the mesh-dataset list and the dslist 
    */
   for (ds=old_datahead; ds!=NULL; ds=ds->next) {
      if (ds->Dptr && ds->flag==0) 
      (void) CNinsert_dslist(new_datahead, new_datatail, ds->Dptr);
   }
   for (ds=mesh_dshead; ds!=NULL; ds=ds->next) {
      if (ds->Dptr) 
      (void) CNinsert_dslist(new_datahead, new_datatail, ds->Dptr);
   }

   /* Now print the new dataset list */
   if (verbose)
   CNprint_dslist_list(*new_datahead, *new_datatail);

   /* Delete the mesh list */
   CNdelete_dslist_list(&mesh_dshead, &mesh_dstail);
}


/*
 * INDIVIDUAL PRINT ROUTINES
 */

/* 
 * Write out the data in curve format 
 */
static void write_curve_data(fp, Dptr, dataname, 
                             twod, datatype, binary, verbose)
FILE         *fp;
CNdatasetptr Dptr;
char         *dataname;
int          twod;
char         *datatype;
int          binary;
int          verbose;
{
   CNcurveptr C;
   CNpointptr P;
   double     *xarr, *yarr, *zarr;
   int        npoints=0, npts=0, i;
   int        ncurves=0;

   if (Dptr->curvehead == NULL) {
      (void) fprintf(stderr,"Error : No curves in the %s dataset!\n",datatype);
      return;
   }

   /* Write the header - Append the name if it is available */
   (void) fprintf(fp,"$ DATA=%s",datatype);
   if (strlen(dataname)>0) (void) fprintf(fp," NAME=\"%s\"\n",dataname);
   else                    (void) fprintf(fp,"\n");
   
   /* Write plot/view options */
   CNwrite_view_options   (fp, Dptr->view_pr);
   CNwrite_plotset_options(fp, &(Dptr->plot_pr));
   CNwrite_dataset_options(fp, &(Dptr->data_pr));
   if ((Dptr->data_pr.stepmethod == CN_USERDEFN) && Dptr->cstephead)
   CNwrite_contsteps(fp, Dptr->cstephead, Dptr->csteptail);
   if (Dptr->annothead)
   CNwrite_annotations(fp, Dptr->annothead, Dptr->annottail);

   for (C=Dptr->curvehead; C!=NULL; C=C->next) {
      /* Count no of points in the curve */
      npts = CNcount_points(C->pointhead, C->pointtail);
      if (npts <= 0) continue;

      /* Print newline */
      (void) fprintf(fp,"\n");
      CNwrite_curve_options(fp, &(C->curv_pr));

      /* If the curveID does not agree with the current curve count, print it */
      if (C->ID != ncurves)
      (void) fprintf(fp,"%% curveno   = %d\n", C->ID);

      if (!binary) {
         /*
          * Write in ascii 
          */

         /* Print the coordinates */
         for (P=C->pointhead; P!=NULL; P=P->next) {
            if (twod)
            (void) fprintf(fp,"%g %g %d\n",P->x,P->y,P->ID);
            else
            (void) fprintf(fp,"%g %g %g %d\n",P->x,P->y,P->z,P->ID);
            npoints++;
         }
      } else {
         /*
          * Write in binary
          */

         /* Create and Fill the coordinate arrays */
         xarr = CNcreate_1D_double_array(npts);
         yarr = CNcreate_1D_double_array(npts);
         zarr = CNcreate_1D_double_array(npts);
         i=0;
         for (P=C->pointhead; P!=NULL; P=P->next) {
            CNset_1D_double_array_value(xarr,i,npts,&(P->x));
            CNset_1D_double_array_value(yarr,i,npts,&(P->y));
            CNset_1D_double_array_value(zarr,i,npts,&(P->z));
            i++;
         } 

         /* Write out binary data */
         (void) fprintf(fp,"%% BINARY=True npts=%d\n",npts);
         if (fwrite((char *)xarr,sizeof(double),npts,fp) != npts)
            (void) fprintf(stderr,"   ***Binary write error of data array!\n");
         if (fwrite((char *)yarr,sizeof(double),npts,fp) != npts) 
            (void) fprintf(stderr,"   ***Binary write error of data array!\n");
         if (!twod) {
            if (fwrite((char *)zarr,sizeof(double),npts,fp) != npts) 
            (void) fprintf(stderr,"   ***Binary write error of data array!\n");
         }

         /* Free the arrays */
         CNfree_1D_double_array(xarr);
         CNfree_1D_double_array(yarr);
         CNfree_1D_double_array(zarr);

         /* Increment the point count */
         npoints += npts;
      }
      /* Increment the curve count */
      ncurves++;
   }
   (void) fprintf(fp,"\n");

   if (verbose) 
      (void) fprintf(stdout,"   Printed %s data with %d points in %d curves\n",
                     datatype,npoints,ncurves);
}

/* 
 * Write out the data in grid-contour format 
 */
/*ARGSUSED*/
static void write_gdcontour_data(fp, Dptr, dataname, datatype, binary, verbose)
FILE         *fp;
CNdatasetptr Dptr;
char         *dataname;
char         *datatype;
int          binary;
int          verbose;
{
   double *arr;
   int    npts, i;

   if (Dptr->grid == NULL) {
      (void) fprintf(stderr,
      "Error : Couldn't find rectangular grid for the %s dataset!\n",datatype);
      return;
   }

   /* Write the header - Append the name if it is available */
   (void) fprintf(fp,"$ DATA=%s",datatype);
   if (strlen(dataname)>0) (void) fprintf(fp," NAME=\"%s\"\n",dataname);
   else                    (void) fprintf(fp,"\n");
   
   /* Write plot/view options */
   CNwrite_view_options   (fp, Dptr->view_pr);
   CNwrite_plotset_options(fp, &(Dptr->plot_pr));
   CNwrite_dataset_options(fp, &(Dptr->data_pr)); 
   if ((Dptr->data_pr.stepmethod == CN_USERDEFN) && Dptr->cstephead)
   CNwrite_contsteps(fp, Dptr->cstephead, Dptr->csteptail);

   /* Write x-array info */
   (void) fprintf(fp,"%% NX=%d\n",Dptr->grid->nx);
   (void) fprintf(fp,"%% XGRID\n");
   if (!binary) {
      /* Write in ascii */
      for (i=0; i<Dptr->grid->nx; i++) {
         (void) fprintf(fp,"%g ",Dptr->grid->xarray[i]);
         if ((i+1)%5 == 0) (void) fprintf(fp,"\n");
      }
   } else {
      /* Write in binary */
      arr  = Dptr->grid->xarray;
      npts = Dptr->grid->nx;
      (void) fprintf(fp,"%% BINARY=True\n");
      if (fwrite((char *)arr,sizeof(double),npts,fp) != npts)
      (void) fprintf(stderr,"   ***Binary write error of data array!\n");
   }
   (void) fprintf(fp,"\n");

   /* Write y-array info */
   (void) fprintf(fp,"%% NY=%d\n",Dptr->grid->ny);
   (void) fprintf(fp,"%% YGRID\n");
   if (!binary) {
      /* Write in ascii */
      for (i=0; i<Dptr->grid->ny; i++) {
         (void) fprintf(fp,"%g ",Dptr->grid->yarray[i]);
         if ((i+1)%5 == 0) (void) fprintf(fp,"\n");
      }
   } else {
      /* Write in binary */
      arr  = Dptr->grid->yarray;
      npts = Dptr->grid->ny;
      (void) fprintf(fp,"%% BINARY=True\n");
      if (fwrite((char *)arr,sizeof(double),npts,fp) != npts) 
      (void) fprintf(stderr,"   ***Binary write error of data array!\n");
   }
   (void) fprintf(fp,"\n");

   /* Write z-array information */
   (void) fprintf(fp,"# z-array\n");
   if (!binary) {
      /* Write in ascii */
      for (i=0; i<Dptr->grid->nz; i++) {
         (void) fprintf(fp,"%g ",Dptr->grid->zarray[i]);
         if ((i+1)%5 == 0) (void) fprintf(fp,"\n");
      }
   } else {
      /* Write in binary */
      arr  = Dptr->grid->zarray;
      npts = Dptr->grid->nz;
      (void) fprintf(fp,"%% BINARY=True\n");
      if (fwrite((char *)arr,sizeof(double),npts,fp) != npts)
      (void) fprintf(stderr,"   ***Binary write error of data array!\n");
   }
   (void) fprintf(fp,"\n");
   (void) fprintf(fp,"\n");
   
   if (verbose)
      (void) fprintf(stdout,"   Printed %s data with %d x %d grid-points \n",
                     datatype,Dptr->grid->nx,Dptr->grid->ny);
}

/* 
 * Write out the data in curve-contour format 
 */
/*ARGSUSED*/
static void write_cvcontour_data(fp, Dptr, dataname, datatype, binary, verbose)
FILE         *fp;
CNdatasetptr Dptr;
char         *dataname;
char         *datatype;
int          binary;
int          verbose;
{
   CNrectptr  R;
   CNtriaptr  T;
   int        ntrias=0;
   int        nrects=0;

   if (Dptr->triahead == NULL && Dptr->recthead == NULL) {
      (void) fprintf(stderr,
      "Error : No triangles/rectangles in the %s dataset!\n",datatype);
      return;
   }

   /* Write the header - Append the name if it is available */
   (void) fprintf(fp,"$ DATA=%s",datatype);
   if (strlen(dataname)>0) (void) fprintf(fp," NAME=\"%s\"\n",dataname);
   else                    (void) fprintf(fp,"\n");
   
   /* Write plot/view options */
   CNwrite_view_options   (fp, Dptr->view_pr);
   CNwrite_plotset_options(fp, &(Dptr->plot_pr));
   CNwrite_dataset_options(fp, &(Dptr->data_pr));
   if ((Dptr->data_pr.stepmethod == CN_USERDEFN) && Dptr->cstephead)
   CNwrite_contsteps(fp, Dptr->cstephead, Dptr->csteptail);

   /* Print the triangle-list */
   for (T=Dptr->triahead; T!=NULL; T=T->next) {
      /* Print newline */
      (void) fprintf(fp,"\n");

      /* Print the coordinates */
      (void) fprintf(fp,"%g %g %g\n",
                     T->n1->coord->x, T->n1->coord->y, T->n1->t);
      (void) fprintf(fp,"%g %g %g\n",
                     T->n2->coord->x, T->n2->coord->y, T->n2->t);
      (void) fprintf(fp,"%g %g %g\n",
                     T->n3->coord->x, T->n3->coord->y, T->n3->t);
      ntrias++;
   }

   /* Print the rectangle-list */
   for (R=Dptr->recthead; R!=NULL; R=R->next) {
      /* Print newline */
      (void) fprintf(fp,"\n");

      /* Print the coordinates */
      (void) fprintf(fp,"%g %g %g\n",
                     R->n1->coord->x, R->n1->coord->y, R->n1->t);
      (void) fprintf(fp,"%g %g %g\n",
                     R->n2->coord->x, R->n2->coord->y, R->n2->t);
      (void) fprintf(fp,"%g %g %g\n",
                     R->n3->coord->x, R->n3->coord->y, R->n3->t);
      (void) fprintf(fp,"%g %g %g\n",
                     R->n4->coord->x, R->n4->coord->y, R->n4->t);
      nrects++;
   }
   (void) fprintf(fp,"\n");

   if (verbose) 
      (void) fprintf(stdout,"   Printed %s data with %d trias %d rects\n",
                     datatype,ntrias,nrects);
}


/* 
 * Write out the data in grid format 
 */
static void write_grid4D_data(fp, Dptr, dataname, datatype, binary, verbose)
FILE         *fp;
CNdatasetptr Dptr;
char         *dataname;
char         *datatype;
int          binary;
int          verbose;
{
   double *arr;
   int    i, npts;

   if (Dptr->grid == NULL) {
      (void) fprintf(stderr,"Error : No 4D grid in the %s dataset!\n",datatype);
      return;
   }

   /* Write the header - Append the name if it is available */
   (void) fprintf(fp,"$ DATA=%s",datatype);
   if (strlen(dataname)>0) (void) fprintf(fp," NAME=\"%s\"\n",dataname);
   else                    (void) fprintf(fp,"\n");

   /* Write plot/view options */
   CNwrite_view_options   (fp, Dptr->view_pr);
   CNwrite_plotset_options(fp, &(Dptr->plot_pr));
   CNwrite_dataset_options(fp, &(Dptr->data_pr));
   if ((Dptr->data_pr.stepmethod == CN_USERDEFN) && Dptr->cstephead)
   CNwrite_contsteps(fp, Dptr->cstephead, Dptr->csteptail);

   /* Write the data */
   
   /* Write x-array info */
   (void) fprintf(fp,"%% NX=%d\n",Dptr->grid->nx);
   (void) fprintf(fp,"%% XGRID\n");
   if (!binary) {
      /* Write in ascii */
      for (i=0; i<Dptr->grid->nx; i++) {
         (void) fprintf(fp,"%g ",Dptr->grid->xarray[i]);
         if ((i+1)%5 == 0) (void) fprintf(fp,"\n");
      }
   } else {
      /* Write in binary */
      arr  = Dptr->grid->xarray;
      npts = Dptr->grid->nx;
      (void) fprintf(fp,"%% BINARY=True\n");
      if (fwrite((char *)arr,sizeof(double),npts,fp) != npts) 
      (void) fprintf(stderr,"   ***Binary write error of data array!\n");
   }
   (void) fprintf(fp,"\n");

   /* Write y-array info */
   (void) fprintf(fp,"%% NY=%d\n",Dptr->grid->ny);
   (void) fprintf(fp,"%% YGRID\n");
   if (!binary) {
      /* Write in ascii */
      for (i=0; i<Dptr->grid->ny; i++) {
         (void) fprintf(fp,"%g ",Dptr->grid->yarray[i]);
         if ((i+1)%5 == 0) (void) fprintf(fp,"\n");
      }
   } else {
      /* Write in binary */
      arr  = Dptr->grid->yarray;
      npts = Dptr->grid->ny;
      (void) fprintf(fp,"%% BINARY=True\n");
      if (fwrite((char *)arr,sizeof(double),npts,fp) != npts) 
      (void) fprintf(stderr,"   ***Binary write error of data array!\n");
   }
   (void) fprintf(fp,"\n");

   /* Write z-array info */
   (void) fprintf(fp,"%% NZ=%d\n",Dptr->grid->nz);
   (void) fprintf(fp,"%% ZGRID\n");
   if (!binary) {
      /* Write in ascii */
      for (i=0; i<Dptr->grid->nz; i++) {
         (void) fprintf(fp,"%g ",Dptr->grid->zarray[i]);
         if ((i+1)%5 == 0) (void) fprintf(fp,"\n");
      }
   } else {
      /* Write in binary */
      arr  = Dptr->grid->zarray;
      npts = Dptr->grid->nz;
      (void) fprintf(fp,"%% BINARY=True\n");
      if (fwrite((char *)arr,sizeof(double),npts,fp) != npts) 
      (void) fprintf(stderr,"   ***Binary write error of data array!\n");
   }
   (void) fprintf(fp,"\n");

   /* Write t-array information */
   (void) fprintf(fp,"# t-array\n");
   if (!binary) {
      /* Write in ascii */
      for (i=0; i<Dptr->grid->nt; i++) {
         (void) fprintf(fp,"%g ",Dptr->grid->tarray[i]);
         if ((i+1)%5 == 0) (void) fprintf(fp,"\n");
      }
   } else {
      /* Write in binary */
      arr  = Dptr->grid->tarray;
      npts = Dptr->grid->nt;
      (void) fprintf(fp,"%% BINARY=True\n");
      if (fwrite((char *)arr,sizeof(double),npts,fp) != npts)
      (void) fprintf(stderr,"   ***Binary write error of data array!\n");
   }
   (void) fprintf(fp,"\n");
   (void) fprintf(fp,"\n");
   
   if (verbose)
      (void) fprintf(stdout,
                     "   Printed %s data with %d x %d x %d grid-points\n",
                     datatype,Dptr->grid->nx,Dptr->grid->ny,Dptr->grid->nz);
}


/* 
 * Write out the data in vector format 
 */
/*ARGSUSED*/
static void write_vector_data(fp, Dptr, dataname, datatype, binary, verbose)
FILE         *fp;
CNdatasetptr Dptr;
char         *dataname;
char         *datatype;
int          binary;
int          verbose;
{
   CNvecptr   V;
   double     *xarr, *yarr, *zarr, *vxarr, *vyarr, *vzarr;
   int        nvecs=0, i;

   if (Dptr->vecbox == NULL) {
      (void) fprintf(stderr,"Error : No vectors in the %s dataset!\n",datatype);
      return;
   }

   /* Count the number of vectors */
   nvecs = CNcount_vectors(Dptr->vecbox->vectorhead,
                           Dptr->vecbox->vectortail);
   if (nvecs <= 0) {
      (void) fprintf(stderr,"Error : No vectors in the %s dataset!\n",datatype);
      return;
   }

   /* Write the header - Append the name if it is available */
   (void) fprintf(fp,"$ DATA=%s",datatype);
   if (strlen(dataname)>0) (void) fprintf(fp," NAME=\"%s\"\n",dataname);
   else                    (void) fprintf(fp,"\n");

   /* Write plot/view options */
   CNwrite_view_options   (fp, Dptr->view_pr);
   CNwrite_plotset_options(fp, &(Dptr->plot_pr));
   CNwrite_dataset_options(fp, &(Dptr->data_pr));
   if ((Dptr->data_pr.stepmethod == CN_USERDEFN) && Dptr->cstephead)
   CNwrite_contsteps(fp, Dptr->cstephead, Dptr->csteptail);

   /* Print the vectors */
   if (!binary) {
      /*
       * Write in ascii 
       */
      for (V=Dptr->vecbox->vectorhead; V!=NULL; V=V->next) {
         /* Print the coordinates */
         (void) fprintf(fp,"%g %g %g  ",V->x ,V->y ,V->z);
         (void) fprintf(fp,"%g %g %g\n",V->vx,V->vy,V->vz);
      }
   } else {
      /*
       * Write in binary
       */

      /* Create and Fill the coordinate arrays */
      xarr = CNcreate_1D_double_array(nvecs);
      yarr = CNcreate_1D_double_array(nvecs);
      zarr = CNcreate_1D_double_array(nvecs);
      vxarr = CNcreate_1D_double_array(nvecs);
      vyarr = CNcreate_1D_double_array(nvecs);
      vzarr = CNcreate_1D_double_array(nvecs);
      i=0;
      for (V=Dptr->vecbox->vectorhead; V!=NULL; V=V->next) {
         CNset_1D_double_array_value(xarr,i,nvecs,&(V->x));
         CNset_1D_double_array_value(yarr,i,nvecs,&(V->y));
         CNset_1D_double_array_value(zarr,i,nvecs,&(V->z));
         CNset_1D_double_array_value(vxarr,i,nvecs,&(V->vx));
         CNset_1D_double_array_value(vyarr,i,nvecs,&(V->vy));
         CNset_1D_double_array_value(vzarr,i,nvecs,&(V->vz));
         i++;
      } 

      /* Write out binary data */
      (void) fprintf(fp,"%% BINARY=True npts=%d\n",nvecs);
      if (fwrite((char *)xarr,sizeof(double),nvecs,fp) != nvecs) 
         (void) fprintf(stderr,"   ***Binary write error of data array!\n");
      if (fwrite((char *)yarr,sizeof(double),nvecs,fp) != nvecs) 
         (void) fprintf(stderr,"   ***Binary write error of data array!\n");
      if (fwrite((char *)zarr,sizeof(double),nvecs,fp) != nvecs) 
         (void) fprintf(stderr,"   ***Binary write error of data array!\n");
      if (fwrite((char *)vxarr,sizeof(double),nvecs,fp) != nvecs) 
         (void) fprintf(stderr,"   ***Binary write error of data array!\n");
      if (fwrite((char *)vyarr,sizeof(double),nvecs,fp) != nvecs) 
         (void) fprintf(stderr,"   ***Binary write error of data array!\n");
      if (fwrite((char *)vzarr,sizeof(double),nvecs,fp) != nvecs) 
         (void) fprintf(stderr,"   ***Binary write error of data array!\n");

      /* Free the arrays */
      CNfree_1D_double_array(xarr);
      CNfree_1D_double_array(yarr);
      CNfree_1D_double_array(zarr);
      CNfree_1D_double_array(vxarr);
      CNfree_1D_double_array(vyarr);
      CNfree_1D_double_array(vzarr);
   }
   (void) fprintf(fp,"\n");

   if (verbose) 
      (void) fprintf(stdout,"   Printed %s data with %d vectors\n",
                     datatype,nvecs);
}


/* 
 * Write out the data in probability format 
 */
/*ARGSUSED*/
static void write_probab_data(fp, Dptr, dataname, datatype, binary, verbose)
FILE         *fp;
CNdatasetptr Dptr;
char         *dataname;
char         *datatype;
int          binary;
int          verbose;
{
   CNcurveptr C;
   CNpointptr P;
   int        npts=0;

   /* Count the number of points */
   for (C=Dptr->curvehead; C!=NULL; C=C->next)
      npts += CNcount_points(C->pointhead, C->pointtail);
   if (npts <= 0) {
      (void) fprintf(stderr,"Error : No points in the %s dataset!\n",datatype);
      return;
   }

   /* Write the header - Append the name if it is available */
   (void) fprintf(fp,"$ DATA=%s",datatype);
   if (strlen(dataname)>0) (void) fprintf(fp," NAME=\"%s\"\n",dataname);
   else                    (void) fprintf(fp,"\n");

   /* Write plot/view options */
   CNwrite_view_options   (fp, Dptr->view_pr);
   CNwrite_plotset_options(fp, &(Dptr->plot_pr));
   CNwrite_dataset_options(fp, &(Dptr->data_pr));
   if ((Dptr->data_pr.stepmethod == CN_USERDEFN) && Dptr->cstephead)
   CNwrite_contsteps(fp, Dptr->cstephead, Dptr->csteptail);

   /*
    * Write in ascii 
    */
   (void) fprintf(fp,"%% readprob=True\n");
   for (C=Dptr->curvehead; C!=NULL; C=C->next)
      for (P=C->pointhead; P!=NULL; P=P->next) 
         /* Print the coordinates */
         (void) fprintf(fp,"%g %g\n",P->x,P->y);
   (void) fprintf(fp,"\n");

   if (verbose) 
      (void) fprintf(stdout,"   Printed %s data with %d points\n",
                     datatype,npts);
}


/* 
 * Write out the data in histogram format 
 */
/*ARGSUSED*/
static void write_histogram_data(fp, Dptr, dataname, datatype, 
                                 binary, verbose)
FILE         *fp;
CNdatasetptr Dptr;
char         *dataname;
char         *datatype;
int          binary;
int          verbose;
{
   CNpointptr P;
   int        npts=0;

   /* Error check */
   if (Dptr->histogram == NULL) {
      (void) fprintf(stderr,
                     "Error : No histogram in the %s dataset!\n",datatype);
      return;
   }

   /* Count the number of points */
   npts = CNcount_points(Dptr->histogram->pointhead,Dptr->histogram->pointtail);
   if (npts <= 0) {
      (void) fprintf(stderr,"Error : No points in the %s dataset!\n",datatype);
      return;
   }

   /* Write the header - Append the name if it is available */
   (void) fprintf(fp,"$ DATA=%s",datatype);
   if (strlen(dataname)>0) (void) fprintf(fp," NAME=\"%s\"\n",dataname);
   else                    (void) fprintf(fp,"\n");

   /* Write plot/view options */
   CNwrite_view_options   (fp, Dptr->view_pr);
   CNwrite_plotset_options(fp, &(Dptr->plot_pr));
   CNwrite_dataset_options(fp, &(Dptr->data_pr));
   if ((Dptr->data_pr.stepmethod == CN_USERDEFN) && Dptr->cstephead)
   CNwrite_contsteps(fp, Dptr->cstephead, Dptr->csteptail);

   /* Write special options */
   if (Dptr->histogram->filltype != CN_FILL_SOLID)
      (void) fprintf(fp, "%% filltype = %d\n",Dptr->histogram->filltype);
   if (Dptr->histogram->fillcolor != 2)
      (void) fprintf(fp, "%% fillcolor = %d\n",Dptr->histogram->fillcolor);

   /*
    * Write in ascii 
    */
   /* Print the coordinates */
   for (P=Dptr->histogram->pointhead; P!=NULL; P=P->next) 
         (void) fprintf(fp,"%g\n",P->x);
   (void) fprintf(fp,"\n");

   if (verbose) 
      (void) fprintf(stdout,"   Printed %s data with %d points\n",
                     datatype,npts);
}


/* 
 * Write out the data in barchart format 
 */
/*ARGSUSED*/
static void write_barchart_data(fp, Dptr, dataname, datatype, 
                                 binary, verbose)
FILE         *fp;
CNdatasetptr Dptr;
char         *dataname;
char         *datatype;
int          binary;
int          verbose;
{
   CNbinptr Binptr;
   CNbarptr Barptr;
   int      nbars=0;

   /* Error check */
   if (Dptr->barchart == NULL) {
      (void) fprintf(stderr,
                     "Error : No barchart in the %s dataset!\n",datatype);
      return;
   }

   /* Count the number of points */
   nbars=0;
   for (Binptr=Dptr->barchart->binhead; Binptr!=NULL; Binptr=Binptr->next)
      for (Barptr=Binptr->barhead; Barptr!=NULL; Barptr=Barptr->next)
         nbars++;
   if (nbars <= 0) {
      (void) fprintf(stderr,"Error : No bars in the %s dataset!\n",datatype);
      return;
   }

   /* Write the header - Append the name if it is available */
   (void) fprintf(fp,"$ DATA=%s",datatype);
   if (strlen(dataname)>0) (void) fprintf(fp," NAME=\"%s\"\n",dataname);
   else                    (void) fprintf(fp,"\n");

   /* Write plot/view options */
   CNwrite_view_options   (fp, Dptr->view_pr);
   CNwrite_plotset_options(fp, &(Dptr->plot_pr));
   CNwrite_dataset_options(fp, &(Dptr->data_pr));
   if ((Dptr->data_pr.stepmethod == CN_USERDEFN) && Dptr->cstephead)
   CNwrite_contsteps(fp, Dptr->cstephead, Dptr->csteptail);

   /*
    * Write in ascii 
    */

   /* Print the bar labels */
   Binptr = Dptr->barchart->binhead;
   for (Barptr=Binptr->barhead; Barptr!=NULL; Barptr=Barptr->next)
      (void) fprintf(fp,"\"%s\" ",Barptr->name);
   (void) fprintf(fp,"\n");

   /* Print the bins */
   for (Binptr=Dptr->barchart->binhead; Binptr!=NULL; Binptr=Binptr->next) {
      (void) fprintf(fp,"\"%s\" ",Binptr->name);
      for (Barptr=Binptr->barhead; Barptr!=NULL; Barptr=Barptr->next)
         (void) fprintf(fp,"%g ",Barptr->value);
      (void) fprintf(fp,"\n");
   }

   /* Print the filltypes/colors */
   Binptr = Dptr->barchart->binhead;
   (void) fprintf(fp,"%% filltype ");
   for (Barptr=Binptr->barhead; Barptr!=NULL; Barptr=Barptr->next)
      (void) fprintf(fp,"%d ",Barptr->filltype);
   (void) fprintf(fp,"\n");
   (void) fprintf(fp,"%% fillcolor ");
   for (Barptr=Binptr->barhead; Barptr!=NULL; Barptr=Barptr->next)
      (void) fprintf(fp,"%d ",Barptr->fillcolor);
   (void) fprintf(fp,"\n");

   if (verbose) 
      (void) fprintf(stdout,"   Printed %s data with %d bars\n",
                     datatype,nbars);
}


/* 
 * Write out the data in polygon format 
 */
/*ARGSUSED*/
static void write_polygon_data(fp, Dptr, dataname, datatype, 
                               binary, verbose)
FILE         *fp;
CNdatasetptr Dptr;
char         *dataname;
char         *datatype;
int          binary;
int          verbose;
{
   CNpolyptr   P;
   CNnlistptr  Nd;
   CNregionptr R;
   int         pcount, wraparound, npts;

   /* Error check */
   if (Dptr->polyhead == NULL) {
      (void) fprintf(stderr,
                     "Error : No polygons in the %s dataset!\n",datatype);
      return;
   }

   /* Write the header - Append the name if it is available */
   (void) fprintf(fp,"$ DATA=%s",datatype);
   if (strlen(dataname)>0) (void) fprintf(fp," NAME=\"%s\"\n",dataname);
   else                    (void) fprintf(fp,"\n");

   /* Write plot/view options */
   CNwrite_view_options   (fp, Dptr->view_pr);
   CNwrite_plotset_options(fp, &(Dptr->plot_pr));
   CNwrite_dataset_options(fp, &(Dptr->data_pr));
   if ((Dptr->data_pr.stepmethod == CN_USERDEFN) && Dptr->cstephead)
   CNwrite_contsteps(fp, Dptr->cstephead, Dptr->csteptail);

   /*
    * Write in ascii 
    */

   /* Print out each polygon one at a time */
   pcount = 0;
   for (P=Dptr->polyhead; P!=NULL; P=P->next) {
      /* Current polygon count */
      pcount++;

      /* Count no of points */
      npts = CNcount_nlists(P->nlisthead, P->nlisttail);
      wraparound = CN_FALSE;
      if ((npts > 1) && (P->nlisthead->N == P->nlisttail->N)) {
         npts--;
         wraparound = CN_TRUE;
      }

      /* Print out the polygon */
      if (npts > 0) {
         /* Print header */
         (void) fprintf(fp,"# Polygon %d\n",pcount);
         (void) fprintf(fp,"p %d %d\n",npts, P->region);
         
         /* Print the points in the list */
         for (Nd=P->nlisthead; Nd!=NULL; Nd=Nd->next) {
            /* Don't do the last point in some cases */
            if (Nd->next == NULL && wraparound) continue;
            (void) fprintf(fp,"%g %g %g %g\n",
                           Nd->N->coord->x,
                           Nd->N->coord->y,
                           Nd->N->coord->z,
                           Nd->N->t);
         }
         (void) fprintf(fp,"\n");
      }
   }

   /* Print the regions */
   if (Dptr->regionhead != NULL) 
      (void) fprintf(fp,"# Regions\n");
   for (R=Dptr->regionhead; R!=NULL; R=R->next) {
      (void) fprintf(fp,"r %d \"%s\" %d %s %s\n",
                     R->ID, R->matname, R->color, 
                     BOOLEAN_VALUE(!(R->nocont)), 
                     BOOLEAN_VALUE(!(R->isair)));
   }

   if (verbose) 
      (void) fprintf(stdout,"   Printed %s data with %d polygons\n",
                     datatype,pcount);
}


/* 
 * Write out the data in mesh4D format 
 */
/*ARGSUSED*/
static void write_mesh4D_data(fp, Dptr, 
                              dataname, datatype, 
                              binary, verbose)
FILE         *fp;
CNdatasetptr Dptr;
char         *dataname;
char         *datatype;
int          binary;
int          verbose;
{
   CNregionptr R;
   CNquant4Dptr Q;
   double *arr;
   int    i, npts, nquants;

   /* Error check */
   if (Dptr->mesh4D == NULL) {
      (void) fprintf(stderr,
                     "Error : No mesh4D in the %s dataset!\n",datatype);
      return;
   }

   /* Write the header - Append the name if it is available */
   (void) fprintf(fp,"$ DATA=%s",datatype);
   if (strlen(dataname)>0) (void) fprintf(fp," NAME=\"%s\"\n",dataname);
   else                    (void) fprintf(fp,"\n");

   /* Write plot/view options */
   CNwrite_view_options   (fp, Dptr->view_pr);
   CNwrite_plotset_options(fp, &(Dptr->plot_pr));
   CNwrite_dataset_options(fp, &(Dptr->data_pr));
   if ((Dptr->data_pr.stepmethod == CN_USERDEFN) && Dptr->cstephead)
   CNwrite_contsteps(fp, Dptr->cstephead, Dptr->csteptail);

   /* Write binary flag */
   if (binary) 
   (void) fprintf(fp,"%% binary=True\n");

   /* Write x-array info */
   (void) fprintf(fp,"\n# x-array\n");
   (void) fprintf(fp,"x %d\n",Dptr->mesh4D->nx);
   if (!binary) {
      /* Write in ascii */
      for (i=0; i<Dptr->mesh4D->nx; i++) {
         (void) fprintf(fp,"%g ",Dptr->mesh4D->xarray[i]);
         if ((i+1)%5 == 0) (void) fprintf(fp,"\n");
      }
   } else {
      /* Write in binary */
      arr  = Dptr->mesh4D->xarray;
      npts = Dptr->mesh4D->nx;
      if (fwrite((char *)arr,sizeof(double),npts,fp) != npts)
      (void) fprintf(stderr,"   ***Binary write error of data array!\n");
   }
   (void) fprintf(fp,"\n");

   /* Write y-array info */
   (void) fprintf(fp,"\n# y-array\n");
   (void) fprintf(fp,"y %d\n",Dptr->mesh4D->ny);
   if (!binary) {
      /* Write in ascii */
      for (i=0; i<Dptr->mesh4D->ny; i++) {
         (void) fprintf(fp,"%g ",Dptr->mesh4D->yarray[i]);
         if ((i+1)%5 == 0) (void) fprintf(fp,"\n");
      }
   } else {
      /* Write in binary */
      arr  = Dptr->mesh4D->yarray;
      npts = Dptr->mesh4D->ny;
      if (fwrite((char *)arr,sizeof(double),npts,fp) != npts)
      (void) fprintf(stderr,"   ***Binary write error of data array!\n");
   }
   (void) fprintf(fp,"\n");

   /* Write z-array info */
   (void) fprintf(fp,"\n# z-array\n");
   (void) fprintf(fp,"z %d\n",Dptr->mesh4D->nz);
   if (!binary) {
      /* Write in ascii */
      for (i=0; i<Dptr->mesh4D->nz; i++) {
         (void) fprintf(fp,"%g ",Dptr->mesh4D->zarray[i]);
         if ((i+1)%5 == 0) (void) fprintf(fp,"\n");
      }
   } else {
      /* Write in binary */
      arr  = Dptr->mesh4D->zarray;
      npts = Dptr->mesh4D->nz;
      if (fwrite((char *)arr,sizeof(double),npts,fp) != npts)
      (void) fprintf(stderr,"   ***Binary write error of data array!\n");
   }
   (void) fprintf(fp,"\n");

   /* Write mat1 coding */
   (void) fprintf(fp,"\n# mat1-array\n");
   (void) fprintf(fp,"m1\n");
   if (!binary) {
      /* Write in ascii */
      npts = (Dptr->mesh4D->nx-1)*(Dptr->mesh4D->ny-1)*(Dptr->mesh4D->nz-1); 
      for (i=0; i<npts; i++) {
         (void) fprintf(fp,"%g ",Dptr->mesh4D->mat1_array[i]);
         if ((i+1)%5 == 0) (void) fprintf(fp,"\n");
      }
   } else {
      /* Write in binary */
      arr  = Dptr->mesh4D->mat1_array;
      npts = (Dptr->mesh4D->nx-1)*(Dptr->mesh4D->ny-1)*(Dptr->mesh4D->nz-1); 
      if (fwrite((char *)arr,sizeof(double),npts,fp) != npts)
      (void) fprintf(stderr,"   ***Binary write error of data array!\n");
   }
   (void) fprintf(fp,"\n");

   /* Write mat2 coding */
   (void) fprintf(fp,"\n# mat2-array\n");
   (void) fprintf(fp,"m2\n");
   if (!binary) {
      /* Write in ascii */
      npts = (Dptr->mesh4D->nx-1)*(Dptr->mesh4D->ny-1)*(Dptr->mesh4D->nz-1); 
      for (i=0; i<npts; i++) {
         (void) fprintf(fp,"%g ",Dptr->mesh4D->mat2_array[i]);
         if ((i+1)%5 == 0) (void) fprintf(fp,"\n");
      }
   } else {
      /* Write in binary */
      arr  = Dptr->mesh4D->mat2_array;
      npts = (Dptr->mesh4D->nx-1)*(Dptr->mesh4D->ny-1)*(Dptr->mesh4D->nz-1); 
      if (fwrite((char *)arr,sizeof(double),npts,fp) != npts)
      (void) fprintf(stderr,"   ***Binary write error of data array!\n");
   }
   (void) fprintf(fp,"\n");

   /* Write mat-prism coding */
   (void) fprintf(fp,"\n# mat-prism-coding array\n");
   (void) fprintf(fp,"mp\n");
   if (!binary) {
      /* Write in ascii */
      npts = (Dptr->mesh4D->nx-1)*(Dptr->mesh4D->ny-1)*(Dptr->mesh4D->nz-1); 
      for (i=0; i<npts; i++) {
         (void) fprintf(fp,"%g ",Dptr->mesh4D->prism_array[i]);
         if ((i+1)%5 == 0) (void) fprintf(fp,"\n");
      }
   } else {
      /* Write in binary */
      arr  = Dptr->mesh4D->prism_array;
      npts = (Dptr->mesh4D->nx-1)*(Dptr->mesh4D->ny-1)*(Dptr->mesh4D->nz-1); 
      if (fwrite((char *)arr,sizeof(double),npts,fp) != npts)
      (void) fprintf(stderr,"   ***Binary write error of data array!\n");
   }
   (void) fprintf(fp,"\n");

   /* Print out all the quantities associated with this dataset */
   nquants = 0;
   for (Q=Dptr->mesh4D->quant4Dhead; Q!=NULL; Q=Q->next) {
      nquants++;

      /* Write mat-prism coding */
      (void) fprintf(fp,"\n# quantity %d\n",nquants);
      (void) fprintf(fp,"q \"%s\"\n",Q->name);
      if (!binary) {
         /* Write in ascii */
         npts = Q->npts;
         for (i=0; i<Q->npts; i++) {
            (void) fprintf(fp,"%g ",Q->qarray[i]);
            if ((i+1)%5 == 0) (void) fprintf(fp,"\n");
         }
      } else {
         /* Write in binary */
         arr  = Q->qarray;
         npts = Q->npts;
         if (fwrite((char *)arr,sizeof(double),npts,fp) != npts)
         (void) fprintf(stderr,"   ***Binary write error of data array!\n");
      }
      (void) fprintf(fp,"\n");
   }

   /* Print the regions */
   if (Dptr->mesh4D->regionhead != NULL) 
      (void) fprintf(fp,"\n# Regions\n");
   for (R=Dptr->mesh4D->regionhead; R!=NULL; R=R->next) {
      (void) fprintf(fp,"r %d \"%s\" %d %s %s\n",
                     R->ID, R->matname, R->color, 
                     BOOLEAN_VALUE(!(R->nocont)), 
                     BOOLEAN_VALUE(!(R->isair)));
   }

   if (verbose) {
      (void) fprintf(stdout,
      "   Printed %s data with %d x %d x %d grid-points and %d quantities\n",
      datatype,Dptr->mesh4D->nx,Dptr->mesh4D->ny,Dptr->mesh4D->nz,nquants);
   }
}


/* 
 * Write out the data in mesh format 
 */
static void write_mesh_data(fp, Dptr, datatype, binary, verbose)
FILE         *fp;
CNdatasetptr Dptr;
char         *datatype;
int          binary;
int          verbose;
{
   CNpointptr  P;
   CNnodeptr   N;
   CNtriaptr   T;
   CNregionptr R;
   CNquantptr  Q, Qdop=NULL;
   double      *xarr, *yarr, *zarr;
   int         *parr, *narr, *tarr, *n1arr, *n2arr, *n3arr, *rgarr;
   int         ndID, ptID, trID, n1ID, n2ID, n3ID, rgID;
   int         npoints=0, nnodes=0, ntrias=0, nregions=0, nquants=0;
   int         ndvals=0;
   int         DOP_FOUND=CN_FALSE, i;

   /* Write the header - Append the name if it is available */
   (void) fprintf(fp,"$ DATA=%s\n",datatype);
   
   /* Write plot/view options */
   CNwrite_view_options   (fp, Dptr->view_pr);
   CNwrite_plotset_options(fp, &(Dptr->plot_pr));
   CNwrite_dataset_options(fp, &(Dptr->data_pr));
   if ((Dptr->data_pr.stepmethod == CN_USERDEFN) && Dptr->cstephead)
   CNwrite_contsteps(fp, Dptr->cstephead, Dptr->csteptail);

   /* Print the point-list */
   npoints = CNcount_points(Dptr->pointhead, Dptr->pointtail);
   if (!binary) {

      /* 
       * Print the point-list in ASCII 
       */

      /* Print the coordinates */
      for (P=Dptr->pointhead; P!=NULL; P=P->next) 
         (void) fprintf(fp,"c %d  %g %g %g\n",P->ID, P->x, P->y, P->z);

   } else if (npoints > 0) {

      /* 
       * Print the point-list in Binary
       */

      /* Create and fill coordinate arrays */
      parr = CNcreate_1D_int_array   (npoints);
      xarr = CNcreate_1D_double_array(npoints);
      yarr = CNcreate_1D_double_array(npoints);
      zarr = CNcreate_1D_double_array(npoints);
      i=0;
      for (P=Dptr->pointhead; P!=NULL; P=P->next) {
         ptID = P->ID;
         CNset_1D_int_array_value   (parr,i,npoints,&(ptID));
         CNset_1D_double_array_value(xarr,i,npoints,&(P->x));
         CNset_1D_double_array_value(yarr,i,npoints,&(P->y));
         CNset_1D_double_array_value(zarr,i,npoints,&(P->z));
         i++;
      }

      /* Write out binary data */
      (void) fprintf(fp,"bc %d\n",npoints);
      if (fwrite((char *)parr,sizeof(int   ),npoints,fp) != npoints)
         (void) fprintf(stderr,"   ***Binary write error of data array!\n");
      if (fwrite((char *)xarr,sizeof(double),npoints,fp) != npoints)
         (void) fprintf(stderr,"   ***Binary write error of data array!\n");
      if (fwrite((char *)yarr,sizeof(double),npoints,fp) != npoints)
         (void) fprintf(stderr,"   ***Binary write error of data array!\n");
      if (fwrite((char *)zarr,sizeof(double),npoints,fp) != npoints)
         (void) fprintf(stderr,"   ***Binary write error of data array!\n");
      (void) fprintf(fp,"\n");

      /* Free the arrays */
      CNfree_1D_int_array   (parr);
      CNfree_1D_double_array(xarr);
      CNfree_1D_double_array(yarr);
      CNfree_1D_double_array(zarr);

   }

   /* Print the node-list */
   nnodes = CNcount_nodes(Dptr->nodehead, Dptr->nodetail);
   if (!binary) {
       
      /* 
       * Print the node-list in ASCII
       */

      /* Print the node and point ID */
      for (N=Dptr->nodehead; N!=NULL; N=N->next) 
         (void) fprintf(fp,"n %d  %d\n",N->ID, N->coord->ID);

   } else if (nnodes > 0) {

      /*
       * Print the node-list in Binary
       */
 
      /* Create and fill coordinate arrays */
      parr = CNcreate_1D_int_array(nnodes);
      narr = CNcreate_1D_int_array(nnodes);
      i=0;
      for (N=Dptr->nodehead; N!=NULL; N=N->next) {
         ndID = N->ID;
         ptID = N->coord->ID;
         CNset_1D_int_array_value(narr,i,nnodes,&(ndID));
         CNset_1D_int_array_value(parr,i,nnodes,&(ptID));
         i++;
      }

      /* Write out binary data */
      (void) fprintf(fp,"bn %d\n",nnodes);
      if (fwrite((char *)narr,sizeof(int),nnodes,fp) != nnodes)
         (void) fprintf(stderr,"   ***Binary write error of data array!\n");
      if (fwrite((char *)parr,sizeof(int),nnodes,fp) != nnodes)
         (void) fprintf(stderr,"   ***Binary write error of data array!\n");
      (void) fprintf(fp,"\n");

      /* Free the arrays */
      CNfree_1D_int_array(narr);
      CNfree_1D_int_array(parr);
   }

   /* Print the triangle-list */
   ntrias = CNcount_trias(Dptr->triahead, Dptr->triatail);
   if (!binary) {

      /* 
       * Print the triangle list in ASCII
       */

      /* Print the triangle and node IDs */
      for (T=Dptr->triahead; T!=NULL; T=T->next) 
         (void) fprintf(fp,"t %d  %d %d %d  %d\n",
                     T->ID, T->n1->ID, T->n2->ID, T->n3->ID, T->region);

   } else {

      /*
       * Print the triangle list in Binary
       */
 
      /* Create and fill coordinate arrays */
      tarr  = CNcreate_1D_int_array(ntrias);
      n1arr = CNcreate_1D_int_array(ntrias);
      n2arr = CNcreate_1D_int_array(ntrias);
      n3arr = CNcreate_1D_int_array(ntrias);
      rgarr = CNcreate_1D_int_array(ntrias);
      i=0;
      for (T=Dptr->triahead; T!=NULL; T=T->next) {
         trID = T->ID;
         n1ID = T->n1->ID;
         n2ID = T->n2->ID;
         n3ID = T->n3->ID;
         rgID = T->region;
         CNset_1D_int_array_value(tarr ,i,ntrias,&(trID));
         CNset_1D_int_array_value(n1arr,i,ntrias,&(n1ID));
         CNset_1D_int_array_value(n2arr,i,ntrias,&(n2ID));
         CNset_1D_int_array_value(n3arr,i,ntrias,&(n3ID));
         CNset_1D_int_array_value(rgarr,i,ntrias,&(rgID));
         i++;
      }

      /* Write out binary data */
      (void) fprintf(fp,"bt %d\n",ntrias);
      if (fwrite((char *)tarr ,sizeof(int),ntrias,fp) != ntrias)
         (void) fprintf(stderr,"   ***Binary write error of data array!\n");
      if (fwrite((char *)n1arr,sizeof(int),ntrias,fp) != ntrias)
         (void) fprintf(stderr,"   ***Binary write error of data array!\n");
      if (fwrite((char *)n2arr,sizeof(int),ntrias,fp) != ntrias)
         (void) fprintf(stderr,"   ***Binary write error of data array!\n");
      if (fwrite((char *)n3arr,sizeof(int),ntrias,fp) != ntrias)
         (void) fprintf(stderr,"   ***Binary write error of data array!\n");
      if (fwrite((char *)rgarr,sizeof(int),ntrias,fp) != ntrias)
         (void) fprintf(stderr,"   ***Binary write error of data array!\n");
      (void) fprintf(fp,"\n");
 
      /* Free the arrays */
      CNfree_1D_int_array(tarr);
      CNfree_1D_int_array(n1arr);
      CNfree_1D_int_array(n2arr);
      CNfree_1D_int_array(n3arr);
      CNfree_1D_int_array(rgarr);
   }

   for (R=Dptr->regionhead; R!=NULL; R=R->next) {
      /* Print the region ID and name */
      (void) fprintf(fp,"r %d \"%s\"\n",R->ID,R->matname);
      nregions++; 
   }
   (void) fprintf(fp,"\n");

   /* Search for first "doping" quantity */
   DOP_FOUND = CN_FALSE;
   Qdop = NULL;
   for (Q=Dptr->quanthead; Q!=NULL && !DOP_FOUND; Q=Q->next) {
      if ( (strcmp(Q->label,"doping")==0) ||
           (strcmp(Q->label,"Doping")==0) ) {
         DOP_FOUND = CN_TRUE;
         Qdop      = Q;
      }
   }

   if (Qdop != NULL) {
      /* Write the quantity and fields */
      write_quantity_data(fp, Qdop, binary, &ndvals);
   }

   if (verbose) {
      (void) fprintf(stdout,"   Printed %s data with ", datatype);
      (void) fprintf(stdout," %d points", npoints);
      (void) fprintf(stdout," %d nodes", nnodes);
      (void) fprintf(stdout," %d triangles", ntrias);
      (void) fprintf(stdout," %d regions", nregions);
      (void) fprintf(stdout," %d quantities\n", nquants);
   }
}


/* 
 * Write out the data in quant format 
 */
static void write_quant_data(fp, Dptr, datatype, binary, verbose)
FILE         *fp;
CNdatasetptr Dptr;
char         *datatype;
int          binary;
int          verbose;
{
   CNquantptr Q=NULL;
   int        npoints=0;

   if (Dptr->parent == NULL) return;
   if ((Q=Dptr->quantity) == NULL) return;

   if ( (strcmp(Q->label,"doping")==0) || (strcmp(Q->label,"Doping")==0) ) 
      return;

   /* Write the header - Append the name if it is available */
   (void) fprintf(fp,"$ DATA=%s\n",datatype);
   
   /* Write plot/view options */
   CNwrite_view_options   (fp, Dptr->view_pr);
   CNwrite_plotset_options(fp, &(Dptr->plot_pr));
   CNwrite_dataset_options(fp, &(Dptr->data_pr));
   if ((Dptr->data_pr.stepmethod == CN_USERDEFN) && Dptr->cstephead)
   CNwrite_contsteps(fp, Dptr->cstephead, Dptr->csteptail);

   /* Write the quantity name and no of fields */
   write_quantity_data(fp, Q, binary, &npoints);

   if (verbose) {
      (void) fprintf(stdout,"   Printed %s data with ", datatype);
      (void) fprintf(stdout," 1 quantity %d node-values\n", npoints);
   }
}


/*
 * Write out quantity data
 */
static void write_quantity_data(fp, Q, binary, npoints)
FILE       *fp;
CNquantptr Q;
int        binary;
int        *npoints;
{
   CNndvalptr Nv;
   int        ndvals, i, j;
   int        *narr;
   int        ndID;
   double     *xarr;

   if (Q==NULL) return;

   /* Write the quantity name and no of fields */
   (void) fprintf(fp,"q  \"%s\" %d\n",Q->label,Q->nfields);
 
   /* Write the node-values */
   ndvals = CNcount_ndvals(Q->ndvalhead, Q->ndvaltail);
   if (!binary) {

      /*
       * Write in ASCII format 
       */

      /* Write the node-values */
      for (Nv=Q->ndvalhead; Nv!=NULL; Nv=Nv->next) {
         (void) fprintf(fp,"v %d ",Nv->nodeID);
         for (i=0; i<Nv->nfields && i<CN_MAXFIELDS; i++)
            (void) fprintf(fp,"%g ",Nv->field[i]);
         (void) fprintf(fp,"\n");
      }

   } else if (ndvals > 0) {

      /*
       * Print the node-value-list in Binary
       */
 
      /* Create and fill coordinate arrays with node-IDs */
      narr = CNcreate_1D_int_array   (ndvals);
      i=0;
      for (Nv=Q->ndvalhead; Nv!=NULL; Nv=Nv->next) {
         ndID = Nv->nodeID;
         CNset_1D_int_array_value   (narr,i,ndvals,&(ndID));
         i++;
      }

      /* Print the header and nodeID arrays first */
      (void) fprintf(fp,"bv %d %d\n",ndvals,Q->nfields);
      if (fwrite((char *)narr ,sizeof(int),ndvals,fp) != ndvals)
         (void) fprintf(stderr,"   ***Binary write error of data array!\n");

      /* Do the fields */
      xarr = CNcreate_1D_double_array(ndvals);
      for (j=0; j<Q->nfields && j<CN_MAXFIELDS; j++) {
         /* Fill the array */
         i=0;
         for (Nv=Q->ndvalhead; Nv!=NULL; Nv=Nv->next) {
            CNset_1D_double_array_value(xarr,i,ndvals,&(Nv->field[j]));
            i++;
         }
         /* Write the array */
         if (fwrite((char *)xarr,sizeof(double),ndvals,fp) != ndvals)
         (void) fprintf(stderr,"   ***Binary write error of data array!\n");
      }

      /* Free the arrays */
      CNfree_1D_int_array   (narr);
      CNfree_1D_double_array(xarr);
   }
   (void) fprintf(fp,"\n");
   *npoints = ndvals;
}



