/*****************************************************************************/
/*  LibreDWG - free implementation of the DWG file format                    */
/*                                                                           */
/*  Copyright (C) 2009, 2018 Free Software Foundation, Inc.                  */
/*  Copyright (C) 2010 Thien-Thi Nguyen                                      */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

/*
 * test.c: reading and loading a DWG file to memory
 * written by Felipe Castro
 * modified by Felipe Corrêa da Silva Sances
 * modified by Thien-Thi Nguyen
 * modified by Reini Urban
 */

#include <stdio.h>
#include "../src/config.h"

#include <dwg.h>
#include "suffix.inc"
static int help(void);
int verbosity(int argc, char **argv, int i, unsigned int *opts);
#include "common.inc"
static char *fmt = NULL;
static char *outfile = NULL;

static int usage(void) {
  printf("\nUsage: dwgread [-v[0-9]] [-O FMT] [-o OUTFILE] DWGFILE\n");
  return 1;
}
static int opt_version(void) {
  printf("dwgread %s\n", PACKAGE_VERSION);
  return 0;
}
static int help(void) {
  printf("\nUsage: dwgread [OPTION]... DWGFILE\n");
  printf("Reads the DWG and prints error, success or verbose internal progress.\n"
         "\n");
  printf("  -v[0-9], --verbose [0-9]  verbosity\n");
  printf("  -O fmt,  --format fmt     fmt: JSON, YAML, XML, DXF, DXFB\n");
  printf("  -o outfile                \n");
  printf("           --help           display this help and exit\n");
  printf("           --version        output version information and exit\n"
         "\n");
  printf("GNU LibreDWG online manual: <https://www.gnu.org/software/libredwg/>\n");
  return 0;
}

int
main(int argc, char *argv[])
{
  unsigned int opts = 1; //loglevel 1
  int i = 1;
  int error;
  Dwg_Data dwg;

  if (argc < 2)
    {
      return usage();
    }
#if defined(USE_TRACING) && defined(HAVE_SETENV)
  setenv("LIBREDWG_TRACE", "1", 0);
#endif
  if (argc > 2 &&
      (!strcmp(argv[i], "--verbose") ||
       !strncmp(argv[i], "-v", 2)))
    {
      int num_args = verbosity(argc, argv, i, &opts);
      argc -= num_args;
      i += num_args;
    }
  if (argc > 2 &&
      (!strcmp(argv[i], "--format") ||
       !strncmp(argv[i], "-O", 2)))
    {
      int num_args;
      if (!strncmp(argv[i], "-O", 2) && strcmp(argv[i], "-O")) //-Ofmt
        {
          fmt = argv[i]+2;
          num_args = 1;
        }
      else //-O fmt | --format fmt
        {
          fmt = argv[i+1];
          num_args = 2;
        }
      argc -= num_args;
      i += num_args;
    }
  if (argc > 1 && !strcmp(argv[i], "--help"))
    return help();
  if (argc > 1 && !strcmp(argv[i], "--version"))
    return opt_version();

  REQUIRE_INPUT_FILE_ARG (argc);
  dwg.opts = opts;
  error = dwg_read_file(argv[i], &dwg);
  if (!fmt)
    {
      if (error)
        printf("\nERROR\n");
      else
        printf("\nSUCCESS\n");
    }
  dwg_free(&dwg);

  return error;
}
