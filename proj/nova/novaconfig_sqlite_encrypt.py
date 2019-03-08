
import platform

from os.path import realpath, dirname, join, basename

from nova.util import file_utils
from nova.builtin import compile_c, file_op


CWD = dirname(__file__)

ns = globals()['namespace']
ns.set_name('sqlite_encrypt')

nut_proj_root = join(ns.getenv('NUT_PATH', join(CWD, '../../lib/nut.git')), 'proj/nova')
ns.get_app().import_namespace(join(nut_proj_root, 'novaconfig_nut.py'))

## Vars
src_root = join(CWD, '../../src/sqlite_encrypt')
sqlite_src_root = join(CWD, '../../src/sqlite-amalgamation-3150100')
out_dir = platform.system().lower() + '-' + ('debug' if ns['DEBUG'] == '1' else 'release')
out_root = join(CWD, out_dir)
obj_root = join(out_root, 'obj/sqlite_encrypt')
header_root = join(out_root, 'include/sqlite_encrypt')

## Flags
ns.append_env_flags('CPPFLAGS', '-DSQLITE_HAS_CODEC',
                    '-I' + realpath(src_root),
                    '-I' + realpath(sqlite_src_root),
                    '-I' + realpath(header_root),
                    '-I' + realpath(join(nut_proj_root, out_dir, 'include')))
ns.append_env_flags('CFLAGS', '-std=c11')
ns.append_env_flags('CXXFLAGS', '-std=c++11')

if platform.system() == 'Darwin':
    ns.append_env_flags('CXXFLAGS', '-stdlib=libc++')
    ns.append_env_flags('LDFLAGS', '-lc++')
else:
    ns.append_env_flags('LDFLAGS', '-lstdc++')

# if platform.system() == 'Linux':
#     ns.append_env_flags('LDFLAGS', '-lpthread', '-ldl', '-latomic')

if platform.system() != 'Windows':
    ns.append_env_flags('CFLAGS', '-fPIC')
    ns.append_env_flags('CXXFLAGS', '-fPIC')

ns.append_env_flags('LDFLAGS', '-L' + out_root, '-lnut')

## Dependencies
so = join(out_root, 'libsqlite_encrypt' + ns['SHARED_LIB_SUFFIX'])

# Generate headers
for src in file_utils.iterfiles(sqlite_src_root, '.h'):
    ih = file_utils.chproot(src, sqlite_src_root, header_root)
    ns.set_recipe(ih, file_op.copyfile)
    ns.add_chained_deps('@headers', ih, src)

# Compile .c
ns.set_recipe('@read_c_deps', compile_c.read_deps)
for src in file_utils.iterfiles(src_root, '.c', '.cpp'):
    o = file_utils.chproot(src, src_root, obj_root)[:-2] + '.o'
    d = o + '.d'
    ns.add_dep(d, '@headers')
    ns.add_chained_deps(o, '@read_c_deps', d, src)
    ns.add_chained_deps(so, o, src)

for src in file_utils.iterfiles(sqlite_src_root, '.c'):
    if basename(src) == 'sqlite3.c': # HACK "sqlite3.c" 被包含在了 "codecext.c" 中
        continue
    o = file_utils.chproot(src, sqlite_src_root, obj_root)[:-2] + '.o'
    d = o + '.d'
    ns.add_dep(d, '@headers')
    ns.add_chained_deps(o, '@read_c_deps', d, src)
    ns.add_chained_deps(so, o, src)

nut_dup = join(out_root, 'libnut' + ns['SHARED_LIB_SUFFIX'])
ns.set_recipe(nut_dup, file_op.copyfile)
ns.add_chained_deps(so, nut_dup, join(nut_proj_root, out_dir, 'libnut' + ns['SHARED_LIB_SUFFIX']))
ns.set_default_target(so)

# clean
def clean(target):
    file_utils.remove_any(so, obj_root, header_root)
ns.set_recipe('@clean', clean)
