/*
Copyright (C) 2023,  Marco Chieppa | crap0101

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3 of the License,
or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <https://www.gnu.org/licenses>.
*/

/*
 * Description: utilities for using regex in gawk.
 */

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "gawkapi.h"

// define these before include awk_extensions.h
#define _DEBUGLEVEL 0
#define __module__ "regexmix"

#include "awk_extensions.h"
// https://github.com/crap0101/laundry_basket/blob/master/awk_extensions.h

static awk_value_t * do_make(int nargs, awk_value_t *result, struct awk_ext_func *finfo);


/* ----- boilerplate code ----- */
int plugin_is_GPL_compatible;
static const gawk_api_t *api;

static awk_ext_id_t ext_id;
static const char *ext_version = "0.1";

static awk_ext_func_t func_table[] = {
  { "make", do_make, 1, 1, awk_false, NULL },
};

__attribute__((unused)) static awk_bool_t (*init_func)(void) = NULL;


int dl_load(const gawk_api_t *api_p, void *id) {
  api = api_p;
  ext_id = (awk_ext_id_t) &id;
  int errors = 0;
  long unsigned int i;
  
  if (api->major_version < 3) { //!= GAWK_API_MAJOR_VERSION
      //    || api->minor_version < GAWK_API_MINOR_VERSION) {
    eprint("incompatible api version:  %d.%d != %d.%d (extension/gawk version)\n",
	   GAWK_API_MAJOR_VERSION, GAWK_API_MINOR_VERSION, api->major_version, api->minor_version);
    exit(1);
  }
  
  for (i=0; i < sizeof(func_table) / sizeof(awk_ext_func_t); i++) {
    if (! add_ext_func("regexmix", & func_table[i])) {
      eprint("can't add extension function <%s>\n", func_table[0].name);
      errors++;
    }
  }
  if (ext_version != NULL) {
    register_ext_version(ext_version);
  }
  return (errors == 0);
}

/* ---------------------------- */

/*********************/
/* UTILITY FUNCTIONS */
/*********************/
		       
/***********************/
/* EXTENSION FUNCTIONS */
/***********************/

static awk_value_t * do_make(int nargs, awk_value_t *result, __attribute__((unused)) struct awk_ext_func *finfo) {
  /*
   * Returns a  strongly typed regexp value from $nargs[0].
   */
  awk_value_t value;
  assert(result != NULL);

  if (nargs != 1) {
    eprint("this function accepts *one* argument\n");
    result = make_null_string(result);
    goto out;
  }
  if (! get_argument(0, AWK_STRING, & value)) {
    eprint("can't retrieve the function arg\n");
    result = make_null_string(result);
    goto out;
  }
  result = make_const_regex(value.str_value.str, value.str_value.len, result);

 out:
  return result;
}


/* COMPILE WITH (me, not necessary you):
gcc -ggdb -fPIC -shared -DHAVE_CONFIG_H -c -O -g -I/usr/include -iquote ~/local/include/awk -Wall -Wextra regexmix.c && gcc -o regexmix.so -shared regexmix.o && cp regexmix.so ~/local/lib/awk/
*/
