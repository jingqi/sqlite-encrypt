
import platform

from os.path import dirname, join

from nova.util import file_utils


CWD = dirname(__file__)

ns = globals()['namespace']
ns.set_name('sqlite-encrypt-wks')

app = ns.get_app()
app.import_namespace(join(CWD, 'novaconfig-sqlite.py'))
app.import_namespace(join(CWD, 'novaconfig-sqlite-encrypt.py'))
app.import_namespace(join(CWD, 'novaconfig-sqlitecpp.py'))
app.import_namespace(join(CWD, 'novaconfig-test-sqlite-encrypt.py'))
app.import_namespace(join(CWD, 'novaconfig-test-sqlitecpp.py'))

out_dir = platform.system().lower() + '-' + ('debug' if ns['DEBUG'] == '1' else 'release')
ns.set_default_target(join(CWD, out_dir, 'test-sqlite-encrypt' + ns['PROGRAM_SUFFIX']))

# run
ns.add_dep('@run', 'test-sqlite-encrypt|@run')

# clean
def clean(target):
    file_utils.remove_any(join(CWD, out_dir))
ns.set_recipe('@clean', clean)
ns.add_dep('@clean', 'nut|@clean')
