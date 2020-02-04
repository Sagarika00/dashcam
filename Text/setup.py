from distutils.core import setup, Extension

module1 = Extension('libmain',
                    define_macros = [('MAJOR_VERSION', '1'),
                                     ('MINOR_VERSION', '0')],
                    include_dirs = ['/home/aswin/Documents/Courses/Udacity/Intel-Edge/Repository/tbb/include',
                    '/home/aswin/Documents/Courses/Udacity/Intel-Edge/Repository/boost_1_65_1',
                    '/home/aswin/Documents/Courses/Udacity/Intel-Edge/Repository/numpy/numpy/core/include/',
                    '/home/aswin/Documents/Courses/Udacity/Intel-Edge/Repository/opencv/include/',
                    '/home/aswin/Documents/Courses/Udacity/Intel-Edge/Work/EdgeApp/License_Plate_Recognition/SOLID-Project-Framework/app/include',
                    '/home/aswin/Documents/Courses/Udacity/Intel-Edge',
                    '/home/aswin/Documents/Courses/Udacity/Intel-Edge/Repository/Python-3.6.1/Doc/includes', 
                    '/home/aswin/Documents/Courses/Udacity/Intel-Edge/Repository/Python-3.6.1/Include'],
                    libraries = ['python3', 'boost_python-mt', 'boost_numpy-mt', 'boost_thread-mt', 'boost_system-mt', 
                    'opencv_gapi', 'opencv_core', 'opencv_text', 'opencv_ml', 'opencv_ts', 'opencv_objdetect'],
                    library_dirs = ['/home/aswin/Documents/Courses/Udacity/Intel-Edge/Repository/opencv/build/lib', 
                    '/home/aswin/Documents/Courses/Udacity/Intel-Edge/Repository/boost_1_65_1/stage/lib5',
                    '/home/aswin/anaconda3/envs/enscalo_test/lib'],
                    sources = ['libmain.cpp'])

setup (name = 'PackageName',
       version = '1.0',
       description = 'This is a demo package',
       author = '',
       author_email = '',
       url = '',
       long_description = '''
This is really just a demo package.
''',
       ext_modules = [module1])