from chef.logger import logger
import shutil


def clean(args):
    # Simply removes the build directory
    shutil.rmtree('build')
    logger.info('Build directory removed.')
