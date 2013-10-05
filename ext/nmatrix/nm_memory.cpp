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
// == nm_memory.c
//
// Helper functions for volatile memory allocation.

#include <ruby.h>

// (see nm_memory.h for explanation)
volatile VALUE no_op(volatile VALUE* volatile in) {
  return (*in) || Qnil;
}

volatile VALUE no_op(volatile VALUE in) {
  return in;
}
