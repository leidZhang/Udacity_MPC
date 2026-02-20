import sys
from pathlib import Path

_this_dir = Path(__file__).resolve().parent
_build_dir = _this_dir.parent / "build" / "Release"

if _build_dir.exists():
    sys.path.insert(0, str(_build_dir))

from _mpc import *

__all__ = ["MPCPolicy"]