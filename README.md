# Welcome to the 2017 Rohde & Schwarz Engineering Competition

Thank you for joining. We are excited to present you this year's task.

The goal of this competition is to build the most efficient software-based coder for DVB-T2.

You will use the latest master branch of GNU Radio as the basis and do whatever you think is necessary to improve the performance and reduce the runtime.

Each time you submit your code to this repository, it will be automatically compiled and tested for standard compliance and performance. To make the challenge more exciting, we continuously publish a ranking with the current position of all teams. Can you get to first place to come to the final in Munich?

## Reasoning

The output of this coder is I/Q modulation data, which is then used to generate a COFDM waveform. In a transmitter system the resulting RF signal is then fed to an amplifier. To have a fast and efficient software coder allows you to use software based coding instead of relying on FPGAs. This is a more flexible approach and maybe in future your code will be used in production at Rohde & Schwarz.

## Technical details

Your code will be tested for standard compliance. You can do this on your own with the tools in the engcomp/tools repository. The maximum error may not exceed `1e-4`. A copy of the offical standard can be found [here](http://www.etsi.org/deliver/etsi_en/302700_302799/302755/01.04.01_60/en_302755v010401p.pdf).

Please read the GNU Radio Coding Guidelines at <http://wiki.gnuradio.org/index.php/Main_Page#IV._Developing_GNU_Radio>, as we would like to upstream the most effective optimizations to the GNU Radio Project. Also take a look into README.hacking and README.original, as there are quite a lot of useful instructions if you are new to GNU Radio.

The deadline is 2017-05-23 23:59 UTC. Please push your final submission before this deadline, as you will not be able to submit your code after that.

By submitting code you acknowledge that your code stands under the GNU General Public License Version 3. This basically means that whoever distributes something including GNU Radio or gives it away, for free or not, is obligated to make the source code, including all changes, publicly available as well. For more information see the [COPYING](COPYING) file or <https://www.gnu.org/licenses/gpl-3.0.en.html>. We intend to publish all code after the competition ends.

We recommend using the latest Debian testing for your development. If you like to use any other Linux distribution please adapt the following command. To install the dependencies on Debian testing use this command:

```
apt-get install build-essential cmake libboost-all-dev pkg-config python python-dev python-mako python-numpy python-six python-cheetah swig git liborc-0.4-dev libfftw3-dev libgsl0-dev
```

To help you optimize GNU Radio for our specific platform, we collected some information which you can find in the tools repository.

If you have any questions, please do not hesitate to ask them in the FAQ. Please do also consider to subscribe to the official GNU Radio Mailing list at <https://lists.gnu.org/mailman/listinfo/discuss-gnuradio> to get up to date development news for GNU Radio. There you can also get in contact with the main developers of GNU Radio. Please add the following tag to the front of your mail subject if you message the mailing list with something Engineering Competition specific: `[R&S EC2017]`.

A little warning: There is another DVB-T2 project on Github ([here](https://github.com/drmpeg/gr-dvbt2)) which is out of date and already merged into the official GNU Radio. **Please do not use the code in this repository.** The README files may be interesting though.

## Contents of this repository

This repository contains a copy of the official GNU Radio Github repository from 2017-04-25 with the following files added:

- `README.md`: the current file

- `build.sh`: this file contains the instructions for our build system. You can use it to change e.g. the compiler flags. Please note that the build system does not support all components of GNU Radio because it has not all dependencies installed. If you absolutely need other dependencies, please open an issue in the FAQ project.

- `vv002_cr35l.grc`: The GNU Radio Companion file with a fully functioning DVB-T2 Coder. ATTENTION: Do not remove the variables `input_file` and `output_file`. These are required for our automated test system. Always recreate the python file in GNU Radio Companion if you change the .grc file.
- `vv002_cr35l.py`: The corresponding executable python file.

- `engineering-competition/`: this is a directory for you to add a documentation of your changes and optimizations. This is mostly for you, as you may be required to present your approach in the final. Please also keep your code structured and presentable.

## Useful links

- [GNU Radio Homepage](http://gnuradio.org/)

- [GNU Radio Wiki](https://wiki.gnuradio.org/index.php/Main_Page)

- Git Tutorials:
	- [interactive tutorial on GitHub](https://try.github.io)
	- [another good tutorial](http://rypress.com/tutorials/git/index)
	- [yet another tutorial](https://git-scm.com/book/en/v2)

- [DVB-T2 Reference](http://www.etsi.org/deliver/etsi_en/302700_302799/302755/01.04.01_60/en_302755v010401p.pdf)
