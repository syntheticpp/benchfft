benchfft
========

----------------------------------------------------------------------------
WARNING
=======

THIS BENCHMARK IS PROVIDED FOR YOUR INFORMATION ONLY.  DO NOT EXPECT
IT TO RUN OUT OF THE BOX.  WE DID NOT WRITE MOST OF THE INCLUDED
PROGRAMS.  IT IS NOT OUR FAULT IF NO TWO C++ COMPILERS ACCEPT THE SAME
LANGUAGE.  USE AT YOUR OWN RISK.

----------------------------------------------------------------------------

benchFFT
========

benchFFT is a program to benchmark FFT software, assembled by Matteo
Frigo and Steven G. Johnson at the Massachusetts Institute of
Technology.  You can contact us at benchfft@fftw.org, or visit the
benchFFT home page for benchmark results and other information:

		http://www.fftw.org/benchfft

The benchmark incorporates a large number of publicly available FFT
implementations, in both C and Fortran, and measures their performance
and accuracy over a range of transform sizes. It benchmarks both real
and complex transforms in one, two, and three dimensions.

The FFT implementations in the benchmark (found in the benchees/
subdirectory) were written by various authors over a period of more
than 35 years. Except for small tweaks to get things to compile, we
used the unmodified original codes.

----------------------------------------------------------------------------
			       CONTENTS

libbench/
	-- Timing, accuracy, utilities, and other routines used
	   when benchmarking and testing each FFT code.

benchees/
	-- The routines to be benchmarked.  Each directory contains
	   the original source code, any required patches, and source
	   for one or more "doit" programs that link with libbench and
	   call the FFT routine.  (See "architecture", below.)

scripts/
	-- Scripts to run the benchmark and to process/plot the
	   resulting data.  (See "running" and "plotting" below.)

----------------------------------------------------------------------------
			LICENSE AND COPYRIGHT

Files in the libbench/ directory are Copyright (c) 2003 Matteo Frigo,
Copyright (c) 2003 Massachusetts Institute of Technology, and
distributed under the terms of the GNU General Public License.  See
file COPYING for details.

FFT routines in the benchees/ directory are copyrighted by their
respective authors.  We are distributing only those routines that are
redistributable.  Please be advised that some routines may have
licensing terms that are incompatible with the GPL, in which case you
are not allowed to distribute the executable that results from linking
libbench/ with that FFT routine.  This is not a problem in practice,
because those executables are not meant to be distributed.

The remaining files (in particular, all programs called doit*.[ch],
and all scripts in the scripts/ directory) are hereby placed in the
public domain.

----------------------------------------------------------------------------
			       RUNNING

If you don't have a Unix system (GNU/Linux is fine), you are on your own.

To run the benchmark, you should pick some "good" compilers and
compiler flags and set the appropriate environment variables (e.g. CC
and CFLAGS), as well as setting the LDFLAGS and LD_LIBRARY_PATHS
variables as necessary in order to find any libraries installed on
your system (see below).  You then ./configure (with --enable-single
for single precision), run 'make -k' to build the benchmark and 'make
-k benchmark' to benchmark speed and/or 'make -k accuracy' to
benchmark accuracy.  (The '-k' skips any routines that fail to compile
for whatever reason.)  At the end, run 'make collect' to collect all
of the benchmark output into files `hostname`.speed,
`hostname`.accuracy, etcetera.

We generally put all of the commands to run the benchmark on a
particular machine into a little shell script.  See the scripts/run.*
files for some examples.  You would then run e.g. 'sh
scripts/run.solaris' to run the benchmark and collect the data.

		  Optional FFT libraries to install:

The benchmark does not include source code for all routines; in some
cases, you are expected to install the code separately onto your
machine.  The codes that you need to install separately, if you want
to benchmark them, are:

Free software:
	FFTW 2.x and/or FFTW 3.x (double and/or single precision): www.fftw.org
	GNU Scientific Library: sources.redhat.com/gsl

Hardware/vendor-specific:
	Intel Math Kernel Library: www.intel.com/software/products/mkl
	Intel IPPS
	AMD Core Math Library
	Apple VDSP (Macintosh G4 and higher only)
	IBM ESSL (AIX only)
	sgimath (SGI/MIPS only)
	SUNPERF (SPARC only)
	DXML/CXML (Alpha only)

Proprietary software:
	Numerical Recipes: copy .c and .f files into benchees/nr
	NAG (Numerical Algorithms Group) Fortran Library
	IMSL (International Mathematical and Statistical Library)

----------------------------------------------------------------------------
			       Plotting

The format and meaning of the collected speed and accuracy data are
described in www.fftw.org/speed/method.html and
www.fftw.org/accuracy/method.html.  We include a few useful scripts to
help you analyze and plot this data in the scripts/ directory.  (These
scripts mostly require you to have Perl installed.)

These scripts generally take either the `hostname`.speed or
`hostname`.accuracy, output by the benchmark above, as input.

Because these are plain text files, you can extract certain
information simply via grep; e.g. 'grep dcif' to extract data for
double-precision (d) complex-data (c) in-place (i) forward transforms
(f).  We supplement this with additional scripts like 'perl
grep-rank.pl 3' to extract rank-3 (or whatever) transforms, 'perl
grep-p2.pl' to grep for power-of-two sizes, and so on.

We also include a script, grace-plot.pl, to make plots of the speed
and accuracy data, just as they are plotted on our web pages.  This
script converts the speed and accuracy data into the file format of
the free Grace plotting software:

	http://plasma-gate.weizmann.ac.il/Grace/

So, for example, to plot the power-of-two 1d single-precision
complex-transform speed data to a foo.ps PostScript file, you would do:

	egrep 'sc[io][fb]' foo.speed | perl grep-p2.pl | perl grep-rank.pl 1
		| perl grace-plot.pl --no-dups
		| gracebat -pipe -printfile foo.ps

(gracebat is a batch-processing program that comes with Grace.)  The
--no-dups option selects only the "best" variant of each code (just
the best of forward/backward transforms, etcetera), as described in
www.fftw.org/speed/method.html.  For accuracy data, you should use the
--accuracy option to grace-plot.pl, and we generally also use the
--no-dups and --plot-worst options in this case.

To simplify matters somewhat, we have a standard-plots.sh script to
make a set of plots similar to the ones on our web page.  To use it,
do:

	sh standard-plots.sh foo.speed
	sh standard-plots.sh foo.accuracy

and the output will be a set of PostScript files (which, for the web,
we convert to .png via ImageMagick).

----------------------------------------------------------------------------
			     Architecture

For each routine that we benchmark, we compile a separate "doit"
program that links to libbench.  Not only does this isolate the
routines (which may be buggy) from one another, but it also provides a
compact record of the calling sequence and data formats that have been
used in FFTs over the years.

libbench provides the main program, command-line parsing, timing,
accuracy tests, etcetera, so the doit source only needs to provide the
following routines:

can_do(problem):
	Return whether or not the routine can solve a given FFT problem
	(size, precision, dimensionality, data format, etcetera).

setup(problem):
	Perform any one-time initializations needed for the FFT routine.

doit(numiters, problem):
	Call the FFT routine numiters time on the problem (which includes
	preallocated arrays, initialized to zero for speed benchmarks).

done(problem):
	Deallocate memory, etcetera.

In order to verify the correctness of an FFT routine and to benchmark
its accuracy, however, we also need to convert the input/output data
to/from a standard format: a row-major array of complex numbers.
(This is especially important for real-data transforms, which use
widely differing formats.)  To do this, the doit source code can
optionally also provide one or more of the following routines:

copy_c2c_from(problem, in):
	Copy complex data from input array 'in' to problem's
	complex-data arrays (defaults to a memcpy).

copy_c2c_from(problem, out):
	Copy complex data from problem's complex-data arrays to output
	array 'out' (defaults to a memcpy).

copy_c2r/r2c(problem, in/out)
	Copy real parts of complex data from/to array in/out to/from
	problem's real-data arrays (defaults to copying to/from a
	contiguous row-major array of real data).

copy_c2h/h2c(problem, in/out)
	Copy hermitian-symmetry complex data from/to array in/out to/from
	problem's arrays (e.g. as output by a transform's real-input
	FFT, which can use a variety of formats).

Some routines also return a normalized FFT output, whereas the
benchmark's accuracy tests expect unnormalized output, so some of the
doit programs also call the libbench unnormalize() routine to undo
this.  Note that NONE OF THESE DATA CONVERSIONS ARE PERFORMED DURING
SPEED BENCHMARKING.

The above documentation is not intended to be comprehensive, only to
give you a flavor of the overall structure.  See the many included
benchmark doit.c files (etc.) for examples, and also read the libbench
source code.
----------------------------------------------------------------------------

