
import platform

from os.path import realpath, dirname, join

from nova.util import file_utils
from nova.builtin import compile_c, file_op


CWD = dirname(__file__)

ns = globals()['namespace']
ns.set_name('sqlite')

## Vars
src_root = join(CWD, '../../src/sqlite-amalgamation-3150100')
out_dir = platform.system().lower() + '-' + ('debug' if ns['DEBUG'] == '1' else 'release')
out_root = join(CWD, out_dir)
obj_root = join(out_root, 'obj/sqlite')
header_root = join(out_root, 'include/sqlite')

## Flags
ns.append_env_flags('CPPFLAGS', '-I' + realpath(header_root))
ns.append_env_flags('CFLAGS', '-std=c11')

if platform.system() != 'Windows':
    ns.append_env_flags('CFLAGS', '-fPIC')
    ns.append_env_flags('CXXFLAGS', '-fPIC')

## Dependencies
so = join(out_root, 'libsqlite' + ns['SHARED_LIB_SUFFIX'])
ns.set_default_target(so)

# Generate headers
for src in file_utils.iterfiles(src_root, '.h'):
    h = src
    ih = file_utils.chproot(src, src_root, header_root)
    ns.set_recipe(ih, file_op.copyfile)
    ns.add_chained_deps('@headers', ih, h)

# Compile .c
ns.set_recipe('@read_c_deps', compile_c.read_deps)
for src in file_utils.iterfiles(src_root, '.c'):
    c = src
    o = file_utils.chproot(src, src_root, obj_root)[:-2] + '.o'
    d = o + '.d'
    ns.add_dep(d, '@headers')
    ns.add_chained_deps(o, '@read_c_deps', d, c)
    ns.add_chained_deps(so, o, c)


# clean
def clean(target):
    file_utils.remove_any(so, obj_root, header_root)
ns.set_recipe('@clean', clean)
