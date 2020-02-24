import sys
import os

cross_compiling = "_PYTHON_HOST_PLATFORM" in os.environ

linux_platforms = ('linux', 'freebsd', 'gnukfreebsd')
windows_platforms = 'win32'
macosx_platforms = 'darwin'

def get_platform():
    # cross build
    if "_PYTHON_HOST_PLATFORM" in os.environ:
        return os.environ["_PYTHON_HOST_PLATFORM"]
    # Get value of sys.platform
    if sys.platform.startswith('osf1'):
        return 'osf1'
    return sys.platform

def is_linux():
    return host_platform.startswith(linux_platforms)

def is_windows():
    return host_platform == windows_platforms

def is_macos():
    return host_platform == macosx_platforms

host_platform = get_platform()