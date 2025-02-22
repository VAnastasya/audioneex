
Example programs
================

The ``examples`` directory contains some programs that demonstrate the functionality 
of the engine, its interfaces and the suggested practices for a correct and efficient 
use. There are currently examples for Linux, Windows and Android. Other platforms
may be supported in the future, but there is no immediate plan for that.


Linux and Windows examples
--------------------------

Example programs for Linux and Windows can be found in the ``examples/winux`` directory.

.. note::

   In order for the examples to build it is necessary to properly set the 
   include and lib directories in the *User Config* section of the main CMake 
   script (CMakeLists.txt) in the root directory.

Database
^^^^^^^^

All of the examples use an implementation of the :class:`DataStore` interface based on
key-value databases. These implementations can be found in the ``DAO`` directory. 
The following drivers are provided:

* ``TCDataStore`` - This driver provides access to `Tokyo Cabinet 
  <http://fallabs.com/tokyocabinet/>`_, a memory-mapped, key-value store management 
  library. It is an “embedded database", so that's the right choice for deploying 
  ACR applications on embedded or mobile devices for on-device recognition (that 
  is without the need to access a database over a network).

* ``CBDataStore`` - This driver provides access to `Couchbase <http://www.couchbase.com>`_, 
  a distributed key-value/document database for high-performance applications. 
  This is the typical database based on a client-server architecture and is suitable 
  for large scale applications (e.g. web services). The examples have been tested 
  on the Community Edition.

The examples need only one of those databases (it is also possible to use both),
which must be specified at build time using a special CMake parameter (see below).
You can download them from their respective websites, then build the libraries 
(*libtokyocabinet* and/or *libcouchbase*) and install the binaries and headers in 
your system (the headers should be put into a ``tcabinet`` and ``libcouchbase``
directory respectively). If you want to use other databases, you can use the
provided drivers as blueprints to write your own.

Audio I/O
^^^^^^^^^

Audio data is read from files or audio devices using the :class:`AudioSource` class, which
by default looks for the FFmpeg audio decoder on the host system. It needs to be 
located somewhere accessible from the PATH environment variable in order for it 
to be found and used. Should you want to use a different audio decoder, you will 
need to modify the :class:`AudioSource` class.

Other
^^^^^

The examples use the Boost library for several processing tasks (filesystem access, 
threading, events handling, etc.) so you need to install that too. At least version 
1.55 is required. The only needed modules are *thread* and *filesystem* (and their 
dependencies), so you can build just those two if you don't want to build the whole 
thing.

Optionally, if you wish to extract metadata from the audio files, to be used in the
identification process, get the TagLib library as well.

Building
^^^^^^^^

The Linux and Windows examples can be built as part of the main library build
process by indicating so in the CMake command line, as follows

.. code-block:: bash

   $ cmake -DWITH_EXAMPLES=ON ..

By default, the programs will use the Tokyo Cabinet database through the 
``TCDataStore`` driver. It is possible to change this option and use the Couchbase
database by specifying the ``DATASTORE_T`` parameter as follows

.. code-block:: bash

   $ cmake -DWITH_EXAMPLES=ON -DDATASTORE_T=CBDataStore ..

If you want to build with ID3 tag support to handle the metadata then include
the ``WITH_ID3`` paramater

.. code-block:: bash

   $ cmake -DWITH_EXAMPLES=ON -DWITH_ID3=ON ..


Android examples
----------------

The ``examples/android`` folder contains demo apps for Android Studio to test the 
engine and show how to integrate it into Android applications. In order for the 
apps to successfully build there are some things to configure according to your
environment.

1. Get the header files for the Tokyo Cabinet drivers and 
   extract the tc*.h files into a ``tcabinet`` folder somewhere 
   in your system.

2. If you don't have it already, get the Boost library
   and install it into a directory somewhere in your system.

3. You need to build the required native libraries and put
   them into a ``/lib`` folder in the source root directory using
   the following scheme

   ``<sources_root>/lib/android-<arch>-<compiler>``

   Running the ``build_android`` script will do just that for
   the Audioneex libraries. External ones, you need to build
   them yourself (you can use the ``android-configure`` script
   for that) and then copy them into the above directory. Patched 
   sources for Android can be found `here  
   <https://www.dropbox.com/s/kg9sn42d80lt0gt/audioneex_android_ext_libs.tar.gz?dl=0>`_

4. Locate the Android.mk file in the ``app/src/main/jni`` directory
   of the Studio projects and set the include paths in the *User Config* 
   section to the folders where you installed the headers at step 1 
   and 2. If other include paths are required just put them there.

You will also have to set the Android Studio SDK and NDK paths
according to your system in *File->Project Structure*. After that
everything should be set to go. An internet connection is likely
to be needed for Gradle to download some dependencies.
The projects have been set to build for armeabi-v7a architectures
only. If you need something different then modify the filter
in the app's Gradle script.

About the Android demo apps
^^^^^^^^^^^^^^^^^^^^^^^^^^^

**Test**

This app is meant to verify that the main functionality of the 
engine work properly on the target device. Just run it and click 
the button to start the test. If everything works well you will 
see a success message.

**OTA**

This app demonstrates how the engine can be used to perform over-the-air 
recognitions. In order for the app to work you will need to put a fingerprint 
database (\*.idx, \*.qfp and \*.met files) into the ``assets`` folder. The database 
can be created using the command line programs. It can also be done 
programmatically in the app, but you will need to write the code for that.

