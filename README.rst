|Logo|

===============
 OpenCTR Tools 
===============

|Release| |Status| |License|

OpenCTR is a free, open-source, and cross-platform SDK for developing
Nintendo 3DS homebrew.

This repository contains the free and open-source tools used in the OpenCTR
SDK.

.. contents::
   :local:
   :depth: 1
   :backlinks: none

Getting Started
---------------

Most users will not need to use this repo directly, and should instead use
https://github.com/OpenCTR/OpenCTR.

When building OpenCTR-Tools from source, CMake is used as the build system.
Configure and build OpenCTR-Tools like any other CMake project, but take
care to install OpenCTR-Tools to the same location as the OpenCTR SDK.

Documentation
-------------

Documentation for OpenCTR is available online at https://openctr.github.io/

License
-------

**OpenCTR-Tools** is licensed under version 3 of the `GNU General Public
License`_. See `COPYING`_ for terms.

Acknowledgments
---------------

``3dsxtool`` was imported from https://github.com/devkitPro/3dstools, and was 
originally authored by smealum and/or fincs. The ``3dsxtool`` program in this 
repository is licensed under the same terms as the original ``3dsxtool``.

.. |Logo| image:: https://openctr.github.io/_static/logo.svg
   :alt: OpenCTR Logo
   :width: 250px
   :target: https://openctr.github.io/

.. |Status| image:: https://img.shields.io/travis/OpenCTR/OpenCTR-Tools.svg?style=flat-square&label=Build
   :alt: TravisCI status.
   :target: https://travis-ci.org/OpenCTR/OpenCTR-Tools

.. |Release| image:: https://img.shields.io/github/release/OpenCTR/OpenCTR-Tools.svg?style=flat-square&label=Release
   :alt: Latest Release
   :target: https://github.com/OpenCTR/OpenCTR-Tools/releases/latest

.. |License| image:: https://img.shields.io/github/license/OpenCTR/OpenCTR-Tools.svg?style=flat-square&label=License
   :alt: GNU General Public License v3.0
   :target: http://choosealicense.com/licenses/gpl-3.0/

.. _GNU General Public License: http://www.gnu.org/licenses/gpl-3.0.html

.. _COPYING: ./COPYING.txt: 
