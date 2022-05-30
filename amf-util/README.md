> #### Current reference frames
>
> Due to large file sizes the created reference files are not part of this repository. If you want do download a set of the latest reference files, use the following link:
>
> * [Samples-2020-03-12-1703.zip](https://download.pomfort.com/aces-amf/Samples-2020-03-12-1703.zip) (162.1 MB): 
>    * 7 source images
>    * 10 AMF files
>    * 11 reference images
>    * 10 ctlrender creation scripts


# ACES AMF utility

This is a command line utility with several commands for aiding the work of the AMF Imp VWG.

## Features of amf-util

With the AMF utility ``amf-util`` you can:

* output information of an AMF file
* create a bash file that uses ``ctlrender`` to render the pipeline from an AMF file
* scan a folder with CTLs and read transformIds from headers

## Repository content

The ACES AMF util repository consists of:

* ``README.md`` – Overview of features, installation, and usage of the utility
* ``amf-util.py`` –  A command line tool for reading and processing AMF files, written in Python 3
* ``src/`` – Additional source code for `amf-util.py`
* ``Material/`` – Sample AMF files
* ``scripts/`` – Shell scripts, e.g. for batch creating reference images with the help of `amf-util.py`

## Getting started

The ``amf-util`` tool requires a few dependencies that need to be installed first. 

For installing system dependencies on macOS [Homebrew](https://brew.sh) is recommended.

### System requirements

Make sure you have Python 3 installed:

```shell
$ brew install python3
$ brew postinstall python3
```

Clone the repository:

```shell
$ git clone https://github.com/pomfort/amf-util.git
$ cd amf-util
```

Install dependencies using a [Virtual Environment](https://docs.python.org/3/tutorial/venv.html):

```shell
$ python3 -m venv env
$ source env/bin/activate
$ pip3 install -r requirements.txt
```

> As of now, this process has only been tested on macOS 10.13.

## Running ``amf-util.py``

Here are the steps to use and learn more about the ``amf-util.py`` tool:

```shell
$ ./amf-util.py --help
$ ./amf-util.py info --help
$ ./amf-util.py render --help
$ ./amf-util.py ctls --help

```

### The ``info`` command

A basic command is the ``info`` command, it displays information about the AMF file:

```shell
$ amf-util.py info --help
Usage: amf-util.py info [OPTIONS] FILEPATH

  Read an AMF file at a given path and display information about the
  contents of the file.

Options:
  -c, --compact  Compact, single-line output
  -v, --verbose  Verbose output
  --help         Show this message and exit.
```

Example:

```shell
$ ./amf-util.py info PATH/TO/AMF-FILE.amf
```

The output will look like this:

```shell
$ amf-util.py info Material/amf_minimal.amf
Material/amf_minimal.amf:
           description: Example Movie
                  uuid: afe122be-59d3-4360-ad69-33c10108fa7a
      creationDateTime: 2020-11-10T13:20:00Z
  modificationDateTime: 2020-11-27T13:20:00Z
                   IDT: IDT.Acme.Camera.a1.v1 [applied="true"] (IDT from Acme Camera Company)
                   RRT: RRT.a1.0.3 (ACES v1.0.3 RRT)
                   ODT: ODT.Academy.P3D60_48nits.a1.0.3 (P3D60 ODT)

```

To list multiple AMF files and their UUID, you can use the following command with the ``-c`` (compact) option:

```shell
$ find ./samples -name "*.amf" -exec ./amf-util.py info -c {} \;
```

The output will look like this:

```shell
$ find ./samples -name "*.amf" -exec ./amf-util.py info -c {} \;
./samples/file1.amf: 81e512f6-daf2-4fb1-a904-90047e766209
./samples/file2.amf: 47ccc858-bc0e-4409-8104-95ae527001dc
./samples/file3.amf: 8e3bd0b3-6aea-4551-b398-53739a3e36d9
```

### The ``render`` command

The ``render`` command outputs a bash file, that allows to render the pipeline from an AMF file.

```shell
$ amf-util.py render --help
Usage: amf-util.py render [OPTIONS] FILEPATH CTLROOTPATH

  Read an AMF file at a given path and output a ctlrender command that
  renders the pipeline described in the AMF file.

Options:
  -v, --verbose  Verbose output
  --help         Show this message and exit.
```

The ``render`` command

* parses the folder structure at ``CTLROOTPATH``, 
* finds all CTL files,
* extracts the ``transformId`` (via the embedded <ACEStransformID> tag) for each CTL file, and
* assembles a ``ctlrender`` command with paths to the CTL files referenced in the AMF via the <transformId> tags.

Example:

```shell
$ amf-util.py render /tmp/aces-dev-1.0.3/transforms/ctl Material/amf_minimal.amf
```

The output looks like this:

```shell
#!/bin/bash

# ../amf/example2.amf
# created by amf-util 0.0.2
# transforms:
#   IDT: IDT.Acme.Camera.a1.v1 (IDT from Acme Camera Company)
#   RRT: RRT.a1.0.3 (ACES v1.0.3 RRT)
#   ODT: ODT.Academy.P3D60_48nits.a1.0.3 (P3D60 ODT)

CTLRENDER=`which ctlrender`

if [ -z "$1" ] || [ -z "$2" ]
then
     echo "Usage: [script name] path/to/input-file.[tiff|dpx|exr] path/to/output-file.[tiff|dpx|exr]"
     echo
     exit 200
fi

INPUTIMAGEPATH=$1
OUTPUTIMAGEPATH=$2

export CTL_MODULE_PATH="../ACES-LUTs/aces-dev-1.1.0/transforms/ctl/utilities/"

$CTLRENDER \
     -ctl ../ACES-LUTs/aces-dev-1.1.0/transforms/ctl/rrt/RRT.ctl \
     -param1 aIn 1.0 \
     -ctl ../ACES-LUTs/aces-dev-1.1.0/transforms/ctl/odt/p3/ODT.Academy.P3D60_48nits.ctl \
     -param1 aIn 1.0 \
     -force \
     "$INPUTIMAGEPATH" \
     "$OUTPUTIMAGEPATH"

# skipping IDT.Acme.Camera.a1.v1 [applied="true"]
```


## Licensing

The code in this project is licensed under MIT license.
