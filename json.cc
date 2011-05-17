// Copyright (C) 2011 Chris Double.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// DEVELOPERS AND CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
#include "mozart.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <jansson.h>

OZ_Term json2oz(json_t* json)
{
  OZ_Term r = OZ_nil();
  if (json_is_integer(json)) {
    r = OZ_int(json_integer_value(json));
  }
  else if (json_is_number(json)) {
    r = OZ_float(json_number_value(json));
  }
  else if(json_is_object(json)) {
    void* iter = json_object_iter(json);
    OZ_Term oz = OZ_record(OZ_atom("json"), OZ_nil());
    while(iter) {
      const char* key = json_object_iter_key(iter);
      json_t* value = json_object_iter_value(iter);
      oz = OZ_adjoinAt(oz, OZ_atom(key), json2oz(value));
      iter = json_object_iter_next(json, iter);
    }
    r = oz;
  }
  else if(json_is_array(json)) {
    size_t n = json_array_size(json);
    OZ_Term oz = OZ_nil();
    for(size_t i = 0; i < n; ++i) {
      json_t* value = json_array_get(json, n - i - 1);
      oz = OZ_cons(json2oz(value), oz);
    }
    r = oz;
  }
  else if(json_is_string(json)) {
    r = OZ_string(json_string_value(json));
  }
  else if(json_is_true(json)) {
    r = OZ_true();
  }
  else if(json_is_false(json)) {
    r = OZ_false();
  }
  else if(json_is_null(json)) {
    r = OZ_nil();
  }

  return r;
}

json_t* oz2json(OZ_Term oz)
{
  json_t* r = json_null();
  if (OZ_isNil(oz)) {
    r = json_null();
  }
  else if (OZ_isAtom(oz)) {
    r = json_string(OZ_atomToC(oz));
  }
  else if (OZ_isBool(oz)) {
    r = OZ_boolToC(oz) ? json_true() : json_false();
  }
  else if(OZ_isFloat(oz)) {
    r = json_real(OZ_floatToC(oz));
  }
  else if(OZ_isInt(oz)) {
    r = json_integer(OZ_intToC(oz));
  }
  else if(OZ_isSmallInt(oz)) {
    r = json_integer(OZ_intToC(oz));
  }
  else if(OZ_isBigInt(oz)) {
    r = json_integer(OZ_intToC(oz));
  }
  else if(OZ_isDictionary(oz)) {
    /* TODO */
  }
  else if(OZ_isObject(oz)) {
    /* TODO */
  }
  else if(OZ_isString(oz, NULL)) {
    int n;
    r = json_string(OZ_stringToC(oz, &n));
  }
  else if(OZ_isByteString(oz)) {
    int n;
    r = json_string(OZ_stringToC(oz, &n));
  }
  else if(OZ_isVirtualString(oz, NULL)) {
    int n;
    r = json_string(OZ_virtualStringToC(oz, &n));
  }
  else if(OZ_isList(oz, NULL)) {
    json_t* a = json_array();
    while (!OZ_isNil(oz)) {
      json_array_append_new(a, oz2json(OZ_head(oz)));
      oz = OZ_tail(oz);
    }
    r = a;
  }
  else if(OZ_isTuple(oz)) {
    /* TODO */
  }
  else if(OZ_isRecord(oz)) {
    json_t* a = json_object();
    OZ_Term arity = OZ_arityList(oz);
    while (!OZ_isNil(arity)) {
      OZ_Term name = OZ_head(arity);
      OZ_Term subtree = OZ_subtree(oz, name);
      json_object_set_new(a, OZ_atomToC(name), oz2json(subtree));
      arity = OZ_tail(arity);
    }
    r = a;
  }
  else {
    /* TODO */
    printf("Unknown: %s\n", OZ_atomToC(OZ_termType(oz)));
  }
  return r;
}

OZ_BI_define(JSON_encode,1,1)
{
  OZ_declareDetTerm(0, Oz);

  json_t * json = oz2json(Oz);
  char* s = json_dumps(json, JSON_COMPACT);
  OZ_Term r = OZ_string(s);
  free(s);
  OZ_RETURN(r);
}
OZ_BI_end

OZ_BI_define(JSON_decode,1,1)
{
  OZ_declareString(0, Json);

  json_error_t error;
  json_t* root = json_loads(Json, 0, &error);
  if (!root) {
    OZ_RETURN(OZ_nil());
  }

  OZ_Term oz = json2oz(root);
  json_decref(root);
  OZ_RETURN(oz);
}
OZ_BI_end

static OZ_C_proc_interface oz_interface[] = {
  { "encode", 1, 1, JSON_encode },
  { "decode", 1, 1, JSON_decode },
  { 0,0,0,0}
};

OZ_C_proc_interface *oz_init_module() {
  return oz_interface;
}
