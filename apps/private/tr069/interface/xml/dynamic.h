/*
 * Copyright(c) 2008, Works Systems, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source code and binary executable file, with or without modification,
 * are prohibited without prior written permission from Works Systems, Inc.
 * The redistribution may be allowed subject to the terms of the License Agreement with Works Systems, Inc.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/*!
 * \file dynamic.h
 *
 * \author Arthur Echo <arthur_echo@workssys.com.cn>
 *
 * \brief To implement the dynamic library interfaces, compatible with GNU
 *
 */

#ifndef __DYNAMIC_H
#define __DYNAMIC_H

#ifndef __USE_GNU
/*!
 * Some macros definition, but not used
 *
 */

#define RTLD_LAZY       0x00001
#define RTLD_NOW        0x00002
#define RTLD_BINDING_MASK       0x3
#define RTLD_NOLOAD     0x00004
#define RTLD_DEEPBIND       0x00008

#define RTLD_GLOBAL 0x00100

#define RTLD_LOCAL  0

#define RTLD_NODELETE   0x01000

/*!
 * \struct dynamic_library_pair dynamic.h "include/dynamic.h"
 *
 * \section struct_Dl_info
 */

struct dynamic_library_pair {
    const char *dli_fname;/* Filename of defining object */
    void *dli_fbase;      /* Load address of that object */
    const char *dli_sname;/* Name of nearest lower symbol */
    void *dli_saddr;      /* Exact value of nearest symbol */
};

typedef struct dynamic_library_pair Dl_info;

void *dlopen( char *, int );
void *dlsym( void *, const char * );
int dladdr( void *, Dl_info * );
int dlclose( void * );
char *dlerror( void );
#else
#include <dlfcn.h>
#endif /*_USE_GNU*/

#endif /*__DYNAMIC_H*/
