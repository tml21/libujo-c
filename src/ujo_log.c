/* 
 *  LibUjo:  An UJO binaray data object notation library.
 *  Copyright (c) 2015 by wobe-systems GmbH
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this program; if not, write to the Free
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *  
 *  You may find a copy of the license under this software is released
 *  at COPYING file. This is LGPL software: you are welcome to develop
 *  proprietary applications using this library without any royalty or
 *  fee but returning back any change, improvement or addition in the
 *  form of source code, project image, documentation patches, etc.
 */

/* global includes */
#include "ujo_log.h"

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#ifndef OS_X
  #include <malloc.h>
#endif // OS_X
#include <string.h>
 

void log_print(char* filename, int line, char *fmt,...)
{
    
#ifdef WRITE_DEBUG_LOG	
	va_list list;
    char *p, *r;
    int e;
	time_t t;
    
	time(&t); 
    printf("%s ", ctime(&t));
    va_start( list, fmt );
 
    for ( p = fmt ; *p ; ++p )
    {
        if ( *p != '%' )//If simple string
        {
            fputc( *p, stdout );
        }
        else
        {
            switch ( *++p )
            {
                /* string */
            case 's':
            {
                r = va_arg( list, char * );
 
                fprintf(stdout, "%s", r);
                continue;
            }
 
            /* integer */
            case 'd':
            {
                e = va_arg( list, int );
 
                fprintf(stdout, "%d", e);
                continue;
            }
 
            default:
                fputc( *p, stdout );
            }
        }
    }
    va_end( list );
    fprintf(stdout, " [%s][line: %d] ",filename,line);
    fprintf(stdout, "\n");
	fflush(stdout);
#endif /* WRITE_DEBUG_LOG */ 
}