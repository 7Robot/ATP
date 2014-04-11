from distutils.core import setup

setup(
    name='ATP',
    version='0.1.0',
    author='Élie Bouttier',
    author_email='elie+7robot@bouttier.eu.org',
    packages=['atp'],
    scripts=['bin/atp-encode', 'bin/atp-decode', 'bin/atp-protos'],
    url='http://7robot.fr',
    description='7Robot Eurobot - ATP module.',
)
