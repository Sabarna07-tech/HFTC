from setuptools import setup, Extension
import os

extra_args = ['/O2'] if os.name=='nt' else ['-O3','-march=native']

ext_modules = [
    Extension(
        'hftc._hftcmodule',
        sources=[
            'src/hftc/_hftcmodule.c',
            'src/hftc/buffer_ring.c',
            'src/hftc/market_feed.c',
            "src/hftc/order_encoder.c",# ← include it here
        ],
        include_dirs=['src/hftc'],
        extra_compile_args=extra_args,
    )
]

setup(
    name='hftc',
    version='0.1.0',
    packages=['hftc'],
    package_dir={'': 'src'},
    ext_modules=ext_modules,
)
