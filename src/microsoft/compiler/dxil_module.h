/*
 * Copyright 2019 Collabora Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * on the rights to use, copy, modify, merge, publish, distribute, sub
 * license, and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHOR(S) AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef DXIL_MODULE_H
#define DXIL_MODULE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "dxil.h"

#include "util/blob.h"
#include "util/list.h"

enum dxil_standard_block {
   DXIL_BLOCKINFO = 0,
   DXIL_FIRST_APPLICATION_BLOCK = 8
};

enum dxil_llvm_block {
   DXIL_MODULE = DXIL_FIRST_APPLICATION_BLOCK,
   DXIL_PARAMATTR = DXIL_FIRST_APPLICATION_BLOCK + 1,
   DXIL_PARAMATTR_GROUP = DXIL_FIRST_APPLICATION_BLOCK + 2,
   DXIL_CONST_BLOCK = DXIL_FIRST_APPLICATION_BLOCK + 3,
   DXIL_FUNCTION_BLOCK = DXIL_FIRST_APPLICATION_BLOCK + 4,
   DXIL_VALUE_SYMTAB_BLOCK = DXIL_FIRST_APPLICATION_BLOCK + 6,
   DXIL_TYPE_BLOCK = DXIL_FIRST_APPLICATION_BLOCK + 9,
};

enum dxil_fixed_abbrev {
   DXIL_END_BLOCK = 0,
   DXIL_ENTER_SUBBLOCK = 1,
   DXIL_DEFINE_ABBREV = 2,
   DXIL_UNABBREV_RECORD = 3,
   DXIL_FIRST_APPLICATION_ABBREV = 4
};

enum dxil_module_code {
   DXIL_MODULE_CODE_VERSION = 1,
   DXIL_MODULE_CODE_TRIPLE = 2,
   DXIL_MODULE_CODE_DATALAYOUT = 3,
   DXIL_MODULE_CODE_ASM = 4,
   DXIL_MODULE_CODE_SECTIONNAME = 5,
   DXIL_MODULE_CODE_DEPLIB = 6,
   DXIL_MODULE_CODE_GLOBALVAR = 7,
   DXIL_MODULE_CODE_FUNCTION = 8,
   DXIL_MODULE_CODE_ALIAS = 9,
   DXIL_MODULE_CODE_PURGEVALS = 10,
   DXIL_MODULE_CODE_GCNAME = 11,
   DXIL_MODULE_CODE_COMDAT = 12,
};

enum dxil_blockinfo_code {
  DXIL_BLOCKINFO_CODE_SETBID = 1,
  DXIL_BLOCKINFO_CODE_BLOCKNAME = 2,
  DXIL_BLOCKINFO_CODE_SETRECORDNAME = 3
};

enum dxil_attr_kind {
  DXIL_ATTR_KIND_NO_UNWIND = 18,
  DXIL_ATTR_KIND_READ_NONE = 20,
  DXIL_ATTR_KIND_READ_ONLY = 21,
};

struct dxil_attrib {
   enum {
      DXIL_ATTR_ENUM
   } type;

   union {
      enum dxil_attr_kind kind;
   };
};

struct dxil_abbrev {
   struct {
      enum {
         DXIL_OP_LITERAL = 0,
         DXIL_OP_FIXED = 1,
         DXIL_OP_VBR = 2,
         DXIL_OP_ARRAY = 3,
         DXIL_OP_CHAR6 = 4,
         DXIL_OP_BLOB = 5
      } type;
      union {
         uint64_t value;
         uint64_t encoding_data;
      };
   } operands[7];
   size_t num_operands;
};

struct dxil_type;

struct dxil_function_module_info {
   const struct dxil_type *type;
   bool decl;
   int attr_set;
};

struct dxil_const {
   const struct dxil_type *type;
   bool undef;
   union {
      int64_t int_value;
   };
};

struct dxil_module {
   enum dxil_shader_kind shader_kind;
   unsigned major_version, minor_version;
   struct blob module;

   unsigned abbrev_width;

   struct {
      unsigned abbrev_width;
      intptr_t offset;
   } blocks[16];
   size_t num_blocks;

   uint64_t buf;
   unsigned buf_bits;

   struct dxil_abbrev const_abbrevs[4];
   struct dxil_abbrev func_abbrevs[8];
   struct dxil_abbrev type_table_abbrevs[6];

   struct list_head type_list;
   unsigned next_type_id;
};

void
dxil_module_init(struct dxil_module *m);

bool
dxil_module_emit_bits(struct dxil_module *m, uint32_t data, unsigned width);

bool
dxil_module_emit_vbr_bits(struct dxil_module *m, uint64_t data,
                          unsigned width);

bool
dxil_module_align(struct dxil_module *m);

static bool
dxil_module_emit_abbrev_id(struct dxil_module *m, uint32_t id)
{
   return dxil_module_emit_bits(m, id, m->abbrev_width);
}

bool
dxil_module_enter_subblock(struct dxil_module *m, unsigned id,
                           unsigned abbrev_width);

bool
dxil_module_exit_block(struct dxil_module *m);

bool
dxil_module_emit_record(struct dxil_module *m, unsigned code,
                        const uint64_t *data, size_t size);

static inline bool
dxil_module_emit_record_int(struct dxil_module *m, unsigned code, int value)
{
   uint64_t data = value;
   return dxil_module_emit_record(m, code, &data, 1);
}

bool
dxil_module_emit_blockinfo(struct dxil_module *m, int type_index_bits);

bool
dxil_emit_attrib_group_table(struct dxil_module *m,
                             const struct dxil_attrib **attrs,
                             const size_t *sizes, size_t num_attrs);

bool
dxil_emit_attribute_table(struct dxil_module *m,
                          const unsigned *attrs, size_t num_attrs);

bool
dxil_emit_module_info(struct dxil_module *m,
                      const struct dxil_function_module_info *funcs,
                      size_t num_funcs);

bool
dxil_module_emit_type_table(struct dxil_module *m, int type_index_bits);

bool
dxil_emit_module_consts(struct dxil_module *m,
                        const struct dxil_const *consts, size_t num_consts);

bool
dxil_module_emit_symtab_entry(struct dxil_module *m, unsigned value,
                              const char *name);

struct dxil_type *
dxil_module_add_void_type(struct dxil_module *m);

struct dxil_type *
dxil_module_add_int_type(struct dxil_module *m, unsigned bit_size);

struct dxil_type *
dxil_module_add_pointer_type(struct dxil_module *m,
                             const struct dxil_type *target);

struct dxil_type *
dxil_module_add_struct_type(struct dxil_module *m,
                            const char *name,
                            const struct dxil_type **elem_types,
                            size_t num_elem_types);

struct dxil_type *
dxil_module_add_function_type(struct dxil_module *m,
                              const struct dxil_type *ret_type,
                              const struct dxil_type **arg_types,
                              size_t num_arg_types);

#ifdef __cplusplus
}
#endif

#endif
