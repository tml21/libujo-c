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
 *
 *  Homepage:
 *    http://www.libujo.org
 *
 *  For professional support contact us:
 *
 *    wobe-systems GmbH
 *    support@libujo.org
 */

#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include "ujo.h"
#include "testujo_helper.h"
#include "tests/ujo_tests.h"

// --------------------------- begin tests ---------------------------------

/**
 * Run a test and return success or failure.
 */
ujoBool run_test(int no)
{
	switch(no) {
	case 1: 
		if (test01()) {
			printf ("Test 01: library test [   OK   ]\n");
		}else {
			printf ("Test 01: library test [ FAILED ]\n");
			return ujoFalse;
		}; break;
		
	case 2: 
		if (test02()) {
			printf ("Test 02: reading and writing integer types [   OK   ]\n");
		}else {
			printf ("Test 02: reading and writing integer types [ FAILED ]\n");
			return ujoFalse;
		}; break;
	case 3: 
		if (test03()) {
			printf ("Test 03: reading and writing date/time types [   OK   ]\n");
		}else {
			printf ("Test 03: reading and writing date/time types [ FAILED ]\n");
			return ujoFalse;
		}; break;
	case 4: 
		if (test04()) {
			printf ("Test 04: basic string type access [   OK   ]\n");
		}else {
			printf ("Test 04: basic string type access [ FAILED ]\n");
			return ujoFalse;
		}; break;
	case 5: 
		if (test05()) {
			printf ("Test 05: test parse function with callback [   OK   ]\n");
		}else {
			printf ("Test 05: test parse function with callback [ FAILED ]\n");
			return ujoFalse;
		}; break;
	case 6: 
		if (test06()) {
			printf ("Test 06: basic table test [   OK   ]\n");
		}else {
			printf ("Test 06: basic table test [ FAILED ]\n");
			return ujoFalse;
		}; break;
	case 7: 
		if (test07()) {
			printf ("Test 07: basic binary test [   OK   ]\n");
		}else {
			printf ("Test 07: basic binary test [ FAILED ]\n");
			return ujoFalse;
		}; break;
	case 8: 
		if (test08()) {
			printf ("Test 08: format integrity [   OK   ]\n");
		}else {
			printf ("Test 08: format integrity [ FAILED ]\n");
			return ujoFalse;
		}; break;
	case 9: 
		if (test09()) {
			printf ("Test 09: file access [   OK   ]\n");
		}else {
			printf ("Test 09: file access [ FAILED ]\n");
			return ujoFalse;
		}; break;
	default:
		printf ("Test with no %d not found! [ FAILED ]\n", no);
		return ujoFalse;
	}
	return ujoTrue;
};

// --------------------------- end tests ---------------------------------

/**
 * Main function.
 */
int main(int argc, char **argv)
{
	int      testno;

	printf ("** LibUJO: UJO Data Object Notation (regression test).\n");
	printf ("** Copyright (C) 2015 wobe-systems GmbH\n**\n");
	printf ("** UJO regression tests: version=%s\n**\n",
		VERSION);
	printf ("** To gather information about time performance you can use:\n**\n");
	printf ("**     >> time ./testujo\n**\n");
	printf ("** To gather information about memory consumed (and leaks) use:\n**\n");
	printf ("**     >> libtool --mode=execute valgrind --leak-check=yes --error-limit=no ./testujo\n**\n");

	if (argc == 2) {
		if(sscanf(argv[1], "%d", &testno) == EOF) {
		fprintf(stderr, "WARNING: Incorrect value for device\n");
		return -1;
		}
		if (!run_test(testno)) {
			return -1;
		}
	}
	else {
		for (testno = 1; testno <= 9; testno++)
		{
			if (!run_test(testno)) {
			return -1;
			}
		}
	}
	return 0;
}

