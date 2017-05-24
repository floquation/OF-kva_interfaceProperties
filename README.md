# OF-kva_interfaceProperties

Extension of OpenFOAM's "interfaceProperties" library.
A run-time selection mechanism was implemented to select the model for calculating the curvature (kappa).
Current options are:
- "normal": The current OpenFOAM implementation, cf. Brackbill.
- "vofsmooth": The curvature is calculated based on a Laplacian-smoothed alpha-field [1,2], which may reduce spurious currents by an order of magnitude. This is a port of [Hoang's [2] OF-1.6-ext code](https://www.cfd-online.com/Forums/openfoam-verification-validation/124363-interfoam-validation-bubble-droplet-flows-microfluidics.html).

## OpenFOAM version

Tested with OF40 and OF41.

## About the author

I (Kevin van As) am (at the time of writing) a PhD student at TUDelft, Faculty of Applied Sciences, Transport Phenomena group.
My work is, qualitatively, on simulating boiling within OpenFOAM.

https://www.linkedin.com/in/kevinvanas/

## Installation

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
Simply compile the code
```bash
wmake
```
__And you're done!__ You can stop reading this section, unless you want to know more.

### Where did it install the code?

The code was installed in $FOAM_USER_LIBBIN, which is OF's standard location for installing user-modified libraries.
This location may be seen in "Make/files", just like any other OpenFOAM library.

The name of the library is "libinterfaceProperties.so", which is the exact same name as OF's interfaceProperties library.
As a consequence, when you execute your solver, it will find kva_interfaceProperties in $FOAM_USER_LIBBIN
before finding interfaceProperties in $FOAM_LIBBIN.
Therefore, this modified library will be dynamically linked when your solver is executed,
overriding the behaviour of OF's interfaceProperties in the process. If you wish, you can check which dynamic library is used by your solver using the following command:
```bash
ldd $(which interFoam) | grep "interfaceProperties"
```

The main advantage of this method is that you can use it with any solver that relies on OF's interfaceProperties library
(interFoam, interDyMFoam, ...), without _any_ effort!
It simply works. That is a _huge_ advantage to me, which also makes it easier to port to newer versions of OF.
The disadvantage is that it, therefore, must use the very same name as OF's interfaceProperties.

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
Then, in order to use it, make sure that its location is added to your LD_LIBRARY_PATH environmental variable
when you execute your solver. For example (assumming libinterfaceProperties.so was moved to your system directory):
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


## References

[1] Lafaurie B, Nardone C, Scardovelli R, Zaleski S, Zanetti G. Modelling merging and fragmentation in multiphase ﬂows with surfer. J Comput Phys 1994;113:134–47.

[2] Hoang DA, van Steijn V, Portela LM, Kreutzer MT, Kleijn CR. Benchmark numerical simulations of segmented two-phase flows in microchannels using the Volume of Fluid method. J Computers & Fluids 2013;86:28-36

