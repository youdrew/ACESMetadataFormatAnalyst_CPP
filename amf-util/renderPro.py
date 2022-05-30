"""
__author__ = "Patrick Renner"
__copyright__ = "Copyright 2020, Pomfort GmbH"

__license__ = "MIT"
__maintainer__ = "Patrick Renner"
__email__ = "opensource@pomfort.com"
"""

from src.amf import pass_context
from src.util import logger

from src.amf.amf import AmfFileReader
from src.ctl.ctl import TransformsTraverser
import os



def render(ctxfilepath,ctxctlrootpath):

    ctxverbose = False
    if ctxverbose:
        logger.info(f'render for \"{os.path.basename(ctxfilepath)}\"')

    traverser = TransformsTraverser(ctxctlrootpath)

    reader = AmfFileReader(ctxfilepath)
    reader.parse()
    reader.log_render(traverser.transforms, traverser.root_path)

if __name__ == '__main__':
    render("/Volumes/Youdrew_WorkDisk_01/ACESMetadataFormatAnalyst_CPP/TestFile/LogCEI800-Rec.709100nitsdim.amf", "/Users/youdrew/Documents/AMF/Test_file/aces-dev/transforms/ctl")