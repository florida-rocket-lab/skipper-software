import unittest
import sys
import os
import numpy as np

sys.path.append(os.path.join(os.path.abspath(__file__), "..", "..", "..", "terminal"))

import skipper

class SkipperTests:
    class CommunicationSuite(unittest.TestSuite):
