#!/usr/bin/env python

from distutils.core import setup, Extension

_yael = Extension('_yael',
                  define_macros = [('FINTEGER', 'int'),
                                   ('HAVE_THREADS', '')],
                  include_dirs = ['/usr/include/python2.7', './yael', '.'],
                  libraries = ['boost_python-mt-py26'],
                  extra_compile_args = ['-fopenmp'],
                  extra_link_args = ['-fopenmp'],
                  sources = ['yael/binheap.c',
                             'yael/eigs.c',
                             'yael/gmm.c',
                             'yael/hkm.c',
                             'yael/kmeans.c',
                             'yael/kmlsh.c',
                             'yael/machinedeps.c',
                             'yael/matrix.c',
                             'yael/nn.c',
                             'yael/sorting.c',
                             'yael/spectral_clustering.c',
                             'yael/vector.c',
                             'yael/vlad.c',
                            ])


setup(name='yael',
    version='3.0.0',
    description='python bindings for yael',
    author='Herve Jegou, Jack Culpepper',
    url="https://central.iqengines.com/cheeseshop/yael/",
    author_email='jack@iqengines.com',
    packages=['yael'],
    ext_modules = [_yael],
#    package_data={'yael':['_yael.so']},
    install_requires=["numexpr>=1.4.1", "tables"],
)

