# setup.py
import os
from setuptools import setup, Extension

extra_compile_args = []
# on *nix you can keep -O3 etc.
if os.name != 'nt':
    extra_compile_args = ['-O3', '-march=native']
else:
    # MSVC: optimization flag is /O2
    extra_compile_args = ['/O2']

ext_modules = [
    Extension(
        'hftc._hftcmodule',
        sources=[
            'src/hftc/_hftcmodule.c',
            'src/hftc/buffer_ring.c',     # ← include your ring-buffer impl
            # add market_feed.c here when ready
        ],
        include_dirs=['src/hftc'],        # so it can find buffer_ring.h
        extra_compile_args=extra_compile_args,
    )
]

setup(
    name='hftc',
    version='0.1.0',
    packages=['hftc'],
    package_dir={'': 'src'},
    ext_modules=ext_modules,
)
