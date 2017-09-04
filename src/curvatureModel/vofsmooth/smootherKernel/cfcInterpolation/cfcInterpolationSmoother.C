/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2015 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "cfcInterpolationSmoother.H"
#include "fvcAverage.H"

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
Foam::vofsmooth::cfcInterpolationSmoother<Type>::cfcInterpolationSmoother
(
	const word& name,
	const dictionary& dict
//	Istream& is
)
:
	smootherKernel<Type>(name),
	numSmoothingIterations_(dict.lookupOrDefault<label>("numIts",0)),
	weight_(Foam::vofsmooth::weightFactor::New(name,dict.subDict("weightFactor")))
{
//	Info << "Dict contained numSmoothingIterations_ = " << numSmoothingIterations_ << endl; // works!

	if (numSmoothingIterations_ <= 0)
	{
		int numSmoothingIterationsDefault = 2;
		WarningInFunction
			<< "Specified numIts = " << numSmoothingIterations_ << "." << nl
			<< "    " << "This value must be positive. Assuming the default value " << numSmoothingIterationsDefault << " instead." << endl;
		numSmoothingIterations_ = numSmoothingIterationsDefault;
	}
}

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>>
Foam::vofsmooth::cfcInterpolationSmoother<Type>::smoothen(const GeometricField<Type, fvPatchField, volMesh>& fld) const{
	// Declare return
	tmp<GeometricField<Type, fvPatchField, volMesh>> fldSmooth(fld);

	// Compute weightFactor
	Info << "Calculating weight." << endl;
	tmp<volScalarField> weight = weight_->weight(fldSmooth().mesh());

	// Smoothen field
    for(int iter = 0; iter < numSmoothingIterations_; iter++)
    {
    	Info << "iter = " << iter << endl;

    	// fld = fvc::average(fvc::interpolate(fld)); // via surfaceField
    	// fvc::average(volField) uses "linearInterpolate" automatically to first obtain a surfaceField.
    	if(weight.empty()){ // unweighted
        	fldSmooth = fvc::average(fldSmooth);
    	}else{ // weighted
//    		weight->operator++(); // Need to use the tmp twice
//    		Info << "weight->count() = " << weight->count() << endl;
    		tmp<volScalarField> weight2(weight); // Need to use "weight" twice, so we must use two different tmps.
        	fldSmooth = fvc::average(weight*fldSmooth) / fvc::average(weight2);
//        	Info<< "weight  empty?" << weight.empty()  << endl; // yes!
//        	Info<< "weight2 empty?" << weight2.empty() << endl; // yes!
    	}
    }

    return fldSmooth;
}

// ************************************************************************* //

}

// ************************************************************************* //
