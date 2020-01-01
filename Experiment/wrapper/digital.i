
/* file : digital.i */

/* name of module to use*/

%module digital
%include <std_string.i>
%{
/* Every thing in this file is being copied in
 wrapper file. We include the C header file necessary
 to compile the interface */
#include <string>
#include "../digital.h"

%}

/* explicitly list functions and variables to be interfaced */
std::string serialBytes(double vel_F, double vel_N, double vel_w, int id);

%include "../digital.h"
/* or if we want to interface all functions then we can simply
   include header file like this -
   %include "gfg.h"
*/
