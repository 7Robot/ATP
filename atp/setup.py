from distutils.core import setup

setup(
    name='ATP',
    version='0.1.0',
    author='Élie Bouttier',
    author_email='elie+7robot@bouttier.eu.org',
    packages=['atp'],
    scripts=['atp-encode', 'atp-decode', 'atp-protos', 'atp-gen',
        'atp-channel', 'atp-webserver', 'atp-gui'],
    data_files=[('/etc', ['atp.conf'])],
    url='http://7robot.fr',
    description='7Robot Eurobot - ATP module.',
)
