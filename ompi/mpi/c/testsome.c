/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2006 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 */
#include "ompi_config.h"
#include <stdio.h>

#include "ompi/mpi/c/bindings.h"
#include "ompi/mca/pml/pml.h"
#include "ompi/request/request.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILING_DEFINES
#pragma weak MPI_Testsome = PMPI_Testsome
#endif

#if OMPI_PROFILING_DEFINES
#include "ompi/mpi/c/profile/defines.h"
#endif

static const char FUNC_NAME[] = "MPI_Testsome";


int MPI_Testsome(int incount, MPI_Request requests[],
                 int *outcount, int indices[],
                 MPI_Status statuses[]) 
{
    int rc, index, completed;
    ompi_status_public_t *pstatus;

    if ( MPI_PARAM_CHECK ) {
        int rc = MPI_SUCCESS;
        OMPI_ERR_INIT_FINALIZE(FUNC_NAME);
        if( 0 != incount ) {
            if( NULL == requests) {
                rc = MPI_ERR_REQUEST;
            } else if (NULL == indices) {
                rc = MPI_ERR_ARG;
            }
        }
        OMPI_ERRHANDLER_CHECK(rc, MPI_COMM_WORLD, rc, FUNC_NAME);
    }

    if( MPI_STATUSES_IGNORE != statuses ) {
        pstatus = statuses;
    } else {
        pstatus = MPI_STATUS_IGNORE;
    }
    /* optimize this in the future */
    rc = ompi_request_test_any(incount, requests, &index, &completed, pstatus);
    if(completed) {
        *outcount = (index == MPI_UNDEFINED) ? MPI_UNDEFINED : 1;
        indices[0] = index;
    } else {
        *outcount = 0;
    }
    OMPI_ERRHANDLER_RETURN(rc, MPI_COMM_WORLD, rc, FUNC_NAME);
}

