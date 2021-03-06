# Copyright © 2018 Rob Clark
# Copyright © 2019 Collabora

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

libpanfrost_bifrost_files = files(
  'disassemble.c',
  'bi_liveness.c',
  'bi_print.c',
  'bi_opt_dce.c',
  'bi_pack.c',
  'bi_ra.c',
  'bi_schedule.c',
  'bi_tables.c',
  'bir.c',
  'bifrost_compile.c',
)

bifrost_nir_algebraic_c = custom_target(
  'bifrost_nir_algebraic.c',
  input : 'bifrost_nir_algebraic.py',
  output : 'bifrost_nir_algebraic.c',
  command : [
    prog_python, '@INPUT@',
    '-p', join_paths(meson.source_root(), 'src/compiler/nir/'),
  ],
  capture : true,
  depend_files : nir_algebraic_py,
)

libpanfrost_bifrost = static_library(
  'panfrost_bifrost',
  [libpanfrost_bifrost_files, bifrost_nir_algebraic_c],
  include_directories : [inc_common, inc_include, inc_src, inc_panfrost_hw],
  dependencies: [idep_nir],
  link_with: [libpanfrost_util],
  c_args : [c_vis_args, no_override_init_args],
  cpp_args : [cpp_vis_args],
  build_by_default : false,
)
