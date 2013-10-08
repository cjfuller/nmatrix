/////////////////////////////////////////////////////////////////////
// = NMatrix
//
// A linear algebra library for scientific computation in Ruby.
// NMatrix is part of SciRuby.
//
// NMatrix was originally inspired by and derived from NArray, by
// Masahiro Tanaka: http://narray.rubyforge.org
//
// == Copyright Information
//
// SciRuby is Copyright (c) 2010 - 2013, Ruby Science Foundation
// NMatrix is Copyright (c) 2013, Ruby Science Foundation
//
// Please see LICENSE.txt for additional copyright notices.
//
// == Contributing
//
// By contributing source code to SciRuby, you agree to be bound by
// our Contributor Agreement:
//
// * https://github.com/SciRuby/sciruby/wiki/Contributor-Agreement
//
// == nm_memory.h
//
// Macros for memory allocation and freeing

/**
 * We define these macros, which just call the ruby ones, as this makes 
 * debugging memory issues (particularly those involving interaction with
 * the ruby GC) easier, as it's posssible to add debugging code temporarily.
 */
#ifndef __NM_MEMORY_H__
#define __NM_MEMORY_H__

#include <ruby.h>

#define NM_ALLOC(type) (ALLOC(type))

#define NM_ALLOC_N(type, n) (ALLOC_N(type, n))

#define NM_REALLOC_N(var, type, n) (REALLOC_N(var, type, n))

#define NM_ALLOCA_N(type, n) (ALLOCA_N(type, n))

#define NM_FREE(var) (xfree(var))


/**
 * Macros for marking ruby VALUEs as volatile.
 *
 * Why volatile?
 * VALUE is a typedef for unsigned long, and as such can often get optimized
 * out by the compiler.
 *
 * In addition to ruby objects that are marked manually to avoid GC,
 * ruby will also not collect VALUEs on the stack or VALUEs pointed to
 * by pointers on the stack.  One way to keep the ruby GC from collecting
 * VALUEs in use by the nmatrix C code is to keep them (or a pointer) on the
 * stack.  The use of volatile in this context ensures that this type of use
 * is not optimized out. (This is an idiom used throughout the ruby extension
 * API itself.)
 *
 * GC might potentially occur any time a call to the ruby API occurs,
 * including the ruby macros used for memory allocations, so it ends up being 
 * important to mark VALUEs volatile almost everywhere they are used in the 
 * nmatrix code.  Not declaring these volatile can lead to seemingly random
 * segfaults when the GC occasionally runs during an API call and deletes
 * objects still in use.
 *
 * One exception (?) to where volatile is needed is in methods that only call
 * other non-ruby-API methods and pass any and all VALUEs in use to each one
 * of those other methods.  The volatile marking can then be done in those
 * downstream methods.
 *
 * Additionally, VALUEs associated with a wrapped structure with an associated
 * GC marking function do not need to be volatile.
 */

/**
 * The compiler will warn about unused values for each of the volatile
 * declarations in these macros (since they are in fact unused).  The no-op
 * functions allow suppression of these warnings specifically for these cases
 * without having to turn them off.
 */
volatile VALUE no_op(volatile VALUE* volatile in);

volatile VALUE no_op(volatile VALUE in);

#define NM_VOL_VARNAME(var) __nm_vol_##var

#define NM_VOLATILE(var) volatile VALUE NM_VOL_VARNAME(var) = var; no_op(NM_VOL_VARNAME(var))

//FIXME: is making the pointer volatile necessary?  Are there situations where
// this could problematically optimized out otherwise?
#define NM_VOLATILE_P(var) volatile VALUE* volatile __nm_vol_##var = var; no_op(NM_VOL_VARNAME(var))

#define NM_VOLATILE_ELEM(elem, dtype) \
  volatile VALUE* volatile NM_VOL_VARNAME(elem) = NULL; \
  if (dtype == nm::RUBYOBJ) { \
    NM_VOL_VARNAME(elem) = reinterpret_cast<VALUE*>(elem); \
  } \
  no_op(NM_VOL_VARNAME(elem))

#define NM_VOLATILE_DENSE_ELEM(d_storage) \
  void* __elems_temp_##d_storage = d_storage->elements; \
  NM_VOLATILE_ELEM(__elems_temp_##d_storage, d_storage->dtype)

#define NM_VOLATILE_YALE_ELEM(y_storage)//TODO

#define NM_VOLATILE_LIST_ELEM(l_storage)//TODO



// FIXME: does the pointer actually need to be volatile or just the VALUE it's pointing to?
//TODO: list and yale
#define NM_VOLATILE_STORAGE(stype, storage) \
  volatile VALUE* volatile NM_VOL_VARNAME(storage) = NULL; \
  if (storage && storage->dtype == nm::RUBYOBJ) { \
    if (stype == nm::DENSE_STORE) { \
      NM_VOL_VARNAME(storage) = reinterpret_cast<VALUE*>(((DENSE_STORAGE*) storage)->elements); \
    } else if (stype == nm::LIST_STORE) { \
      \
    } else if (stype == nm::YALE_STORE) { \
      \
    } \
  } \
  no_op(NM_VOL_VARNAME(storage))

#define NM_VOLATILE_NMATRIX(m) \
  volatile VALUE* volatile NM_VOL_VARNAME(m) = NULL; \
  if (m && m->storage && m->storage->dtype == nm::RUBYOBJ) { \
    if (m->stype == nm::DENSE_STORE) { \
      NM_VOL_VARNAME(m) = reinterpret_cast<VALUE*>(((DENSE_STORAGE*) m->storage)->elements); \
    } else if (m->stype == nm::LIST_STORE) { \
      \
    } else if (m->stype == nm::YALE_STORE) { \
      \
    } \
  } \
  no_op(NM_VOL_VARNAME(m))
#endif
