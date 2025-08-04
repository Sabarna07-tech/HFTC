from setuptools import setup, Extension

ext_modules = [
    Extension(
        "hftc._hftcmodule",
        sources=["src/hftc/_hftcmodule.c"],
        extra_compile_args=["-O3", "-march=native"],
    )
]

setup(
    name="hftc",
    version="0.1.0",
    packages=["hftc"],
    package_dir={"": "src"},
    ext_modules=ext_modules,
)