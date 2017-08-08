# kva_interfaceProperties

Extension of OpenFOAM's "interfaceProperties" library.
A run-time selection mechanism was implemented to select the model for calculating the curvature (kappa).
Current options are:
- "normal": The current OpenFOAM implementation, cf. Brackbill.
- "vofsmooth": The curvature is calculated based on a Laplacian-smoothed alpha-field [1,2], which may reduce spurious currents by an order of magnitude. This is a port of [Hoang's [2] OF-1.6-ext code](https://www.cfd-online.com/Forums/openfoam-verification-validation/124363-interfoam-validation-bubble-droplet-flows-microfluidics.html).

## OpenFOAM version support

Tested with OF40, OF41 and OF50.
(Disclaimer: no thorough tests have been performed for OF50.)

## OpenFOAM solvers support

At present, the code was only tested with the interFoam solver, without any adaptations.

Theoretically, it should work with __any__ OF solver that relies on OF's interfaceProperties,
as kva_interfaceProperties merely overrides OF's interfaceProperties' behaviour:
* inter(DyM)Foam
* compressibleInter(DyM)Foam
* multiphaseInter(DyM)Foam
* interPhaseChange(DyM)Foam
* very likely some more!

## Installation

### Compile kva_interfaceProperties

Source your OpenFOAM installation and go to an appropriate location (you may choose a different location if you like).
```bash
of40
mkdir -p $FOAM_RUN/../libraries
cd $FOAM_RUN/../libraries
```
Download the source code
```bash
git clone https://github.com/floquation/OF-kva_interfaceProperties
mv OF-kva_interfaceProperties kva_interfaceProperties
cd kva_interfaceProperties
```
Then choose the correct branch amongst these options:
```bash
git checkout of40
git checkout of50
```

Then all that's left is to compile the code:
```bash
cd src
wmake
```

### Recompile your solver

Finally, it is also necessary to recompile the solver that you'd like to use (or rather, to link your solver against
my kva_interfaceProperties library).
This library comes with a script that can automatically do this for you (on UNIX).

#### Using the script
You can use this script as follows:
```bash
cd recompileSolvers
```
Open the file "list_of_solvers", and specify the FULL/PATH to the solvers you wish to recompile (one solver per line).
You may optionally add a new name for each solver.
"list_of_solvers" might then look like:
```bash
# FULL_PATH_to_solver; optional_new_solver_name
$FOAM_SOLVERS/multiphase/interFoam; kva_interFoam
$FOAM_SOLVERS/multiphase/interFoam/interDyMFoam; kva_interDyMFoam
```
Then, run the "get" script followed by the "make" script:
```bash
./Allget && ./Allwmake
```
(_Explanation of what this does if you're interested_: the "get" script obtains a copy of the "Make" folder of every solver
specified in the "list_of_solvers" file. Then, "Make/files" and "Make/options" are modified
such that they refer to the original solver's source code (i.e., the source code is not copied),
and such that the new solver is linked against my kva_interfaceProperties library.
Finally, the "make" script compiles all these solvers, which effectively is recompiling your solvers
with an optional different name. These new solvers are put in $FOAM_USER_APPBIN.
I recommend using a different name, as the recompiled solver may potentially overwrite
your original solver if your original solver also resides in $FOAM_USER_APPBIN.)

Now verify that all your (recompiled) solvers are indeed located in the directory $FOAM_USER_APPBIN:
```bash
ls $FOAM_USER_APPBIN
```

Then, to use the newly build solver, you must relog; or you can simply type the following
(replace "interFoam" by the name of your (new) solver; repeat for every solver). This is equivalent to what relogging would do:
```bash
hash interFoam
```

If something did not work, feel free to ask (see the "Support" section below),
or continue with the "doing it yourself" instructions in case you cannot use the UNIX script.

#### Doing it yourself
To recompile the solver yourself, you have to edit the Make/files and Make/options files from the solver's source directory.
(You might want to copy the solver, rather than modifying the original solver!
That's what the automatic script above does as well.
I also recommend renaming the recompiled solver, from e.g. "interFoam" to "kva_interFoam".
Then you can distinguish between the original solver, and the solver that uses kva_interfaceProperties.)
Please follow my instructions in
[this bug report](https://github.com/floquation/OF-kva_interfaceProperties/issues/2)
for a step-by-step guide of what should be changed.
(Or open the bash script "recompileSolvers/getSolverMake.sh" and try to make sense of what it changes.)

#### Installation successful!
And then you're done! You can stop reading this section, unless you want to know more.

### Where did it install the code?

The code (kva_interfaceProperties) was installed in $FOAM_USER_LIBBIN, which is OF's standard location for installing user-modified libraries.
This location may be seen in "Make/files", just like any other OpenFOAM library.

The name of the library is "libkva_interfaceProperties.so".
Your recompiled solver is linked against this library, and the library has precedence over OF's original "libinterfaceProperties.so".
Therefore, when you execute your recompiled solver, it will find kva_interfaceProperties in $FOAM_USER_LIBBIN
before finding interfaceProperties in $FOAM_LIBBIN.
Then, it will use kva_interfaceProperties' code --> kva_interfaceProperties will be dynamically linked,
overriding the behaviour of OF's interfaceProperties in the process. If you wish, you can check which dynamic library is used by your solver using the following command (modify for your solver name):
```bash
ldd $(which interFoam) | grep "interfaceProperties"
```

The main advantage of this method is that you can use it with any solver that relies on OF's interfaceProperties library
(interFoam, interDyMFoam, ...), without _any_ effort! That is, you do not have to change the source code of your solver!
It simply works. That is a _huge_ advantage to me, which also makes it easier to port to newer versions of OF.
So far, I have not spotted a disadvantage of using this methodology.

#### Doesn't that conflict with my (old) existing case?

The library was written in such a way that if dictionary entries are missing (see below), it will operate using the "normal"
curvature model, which is identical to what OF's interfaceProperties is doing.
Hence, it is completely safe having kva_interfaceProperties overriding OF's interfaceProperties behaviour:
it is completely *identical*. No worries!

Well, there is one small difference: it will print a message to your log-file that is uses the default "normal" curvature model
to inform you about which curvature model is being used.

#### But... But... I don't want it there...

Well, you can always just copy/move the library to a different directory (e.g. your case) if that makes you happier
(but why would you...?).
You'll still have to recompile your solver, so make sure you correctly compile it against the right library version.
This is difficult if you keep a different copy in every case.

Then, in order to use it, make sure that its location is added to your LD_LIBRARY_PATH environmental variable
when you execute your solver. For example (assumming libkva_interfaceProperties.so was moved to your system directory):
```bash
LD_LIBRARY_PATH=./system:$LD_LIBRARY_PATH
interFoam
```

However, this will change LD_LIBRARY_PATH inside your shell --- __not__ merely for this one execution.
This is bad practice, as unexpected things might happen.
If you wish to only update LD_LIBRARY_PATH for one execution, then use the following instead:
```bash
(LD_LIBRARY_PATH=./system:$LD_LIBRARY_PATH && interFoam)
```

Evidently, you can incorporate this in a ./runCase.sh or ./Allrun script if you do not wish to type this all the time.
But honestly, let me ask you again, why wouldn't you just use $FOAM_USER_LIBBIN?


## Using the library in your case

Simply add the following to your constant/transportProperties dictionary:
```
curvatureModel      vofsmooth; // normal;
vofsmoothCoeffs
{
    numSmoothingIterations 2; // default: 2
}
```
And you're ready to go!!

You do not need to import the library in your case. This is done automagically by the dynamic linker.

If the entry "curvatureModel" is missing, "normal" is assumed.
This will ensure that in the absence of this keyword, the library will operate in OpenFOAM's "normal" modus,
as if kva_interfaceProperties was not there.

## Support
You may ask questions in this topic on the CFD forum:
https://www.cfd-online.com/Forums/openfoam-verification-validation/124363-interfoam-validation-bubble-droplet-flows-microfluidics.html#post650088
That is most suitable for generic questions.

Or you can add a bug report under "Issues" on GitHub.
That is most suitable for errors that should not occur, i.e. bugs.
For example, a Segmentation Fault while running the code.
Please make sure that the case that you use _does_ work for the original solver
as to ensure that kva_interfaceProperties is the problem.

## Recommended Read
[Deshpande et al.[3]](http://dx.doi.org/10.1088/1749-4699/5/1/014016) have evaluated the performance
of the interFoam solver.
The paper contains many important hints for anyone using interFoam, like
the capillary time step constraint.
Without it, your simulation could very well become non-physical, despite it converging!

## References

[1] Lafaurie B, Nardone C, Scardovelli R, Zaleski S, Zanetti G. Modelling merging and fragmentation in multiphase ﬂows with surfer. J Comput Phys 1994;113:134–47.

[2] Hoang DA, van Steijn V, Portela LM, Kreutzer MT, Kleijn CR. Benchmark numerical simulations of segmented two-phase flows in microchannels using the Volume of Fluid method. J Computers & Fluids 2013;86:28-36

[3] Deshpande, S. S., Anumolu, L., & Trujillo, M. F. (2012). Evaluating the performance of the two-phase flow solver interFoam. Computational science & discovery, 5(1), 014016.

## About the author

I (Kevin van As) am (at the time of writing) a PhD student at TUDelft, Faculty of Applied Sciences, Transport Phenomena group.
My work is, qualitatively, on simulating boiling within OpenFOAM.

https://www.linkedin.com/in/kevinvanas/

