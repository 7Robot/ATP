from distutils.core import setup, Extension

#jsread = Extension('jsread', sources = ['jsread.c'])

setup(
    name='jsbot',
    version='0.1.0',
    author='Élie Bouttier',
    author_email='elie+7robot@bouttier.eu.org',
    packages=['jsread'],
    scripts=['jsbot'],
#    ext_modules=[jsread],
    url='http://7robot.fr',
    description='7Robot Eurobot - Joystick ATP connector.',
)
