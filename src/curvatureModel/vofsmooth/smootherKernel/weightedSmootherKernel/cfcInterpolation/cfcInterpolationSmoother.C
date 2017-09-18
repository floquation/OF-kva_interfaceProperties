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
)
:
	weightedSmootherKernel<Type>(name, dict),
	numSmoothingIterations_(dict.lookupOrDefault<label>("numIts",0))
{
	if (numSmoothingIterations_ <= 0)
	{
		int numSmoothingIterationsDefault = 2;
		WarningInFunction
			<< "Specified numIts = " << numSmoothingIterations_ << "." << nl
			<< "    " << "This value must be positive. Assuming the default value " << numSmoothingIterationsDefault << " instead." << endl;
		numSmoothingIterations_ = numSmoothingIterationsDefault;
	}
}

template<class Type>
Foam::vofsmooth::cfcInterpolationSmoother<Type>::cfcInterpolationSmoother
(
	const word& name,
	int numIts,
	weightFactor* weight
)
:
	weightedSmootherKernel<Type>(name, weight),
	numSmoothingIterations_(numIts)
{
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
	Info << "(cfcInterpolationSmoother) Smoothing(fld) " << fld.name() << "." << endl;
//
//	// Declare return
//	tmp<GeometricField<Type, fvPatchField, volMesh>> fldSmooth(fld);
//
////	Info << "(cfcInterpolationSmoother) &fld == &fldSmooth()" << ( &fld == &fldSmooth() ) << endl; // Yes! So no data is copied. "fvc::average" below constructs new data.

	if(!this->weight_.valid()){
		FatalErrorInFunction
			<< "Attempted to use smoothen(fld) with a smoother instance that does not own its own weightFunction." << nl
			<< "The function smoothen(fld, weight) should have been used instead." << nl
			<< "This indicates an error in the programming logic."
			<< abort(FatalError);
	}

	// Compute weightFactor
	tmp<volScalarField> tweight = this->weight_->weight(fld.mesh());

	// Smoothen field
	return smoothen(fld,tweight);
	// tweight is automatically cleared (out-of-scope)

//
//	// Smoothen field
//	if(tweight.empty()){ // unweighted
//	    for(int iter = 0; iter < numSmoothingIterations_; iter++)
//	    {
//        	fldSmooth = fvc::average(fldSmooth);
//	    }
//	}else{ // weighted
////		const volScalarField weight = tweight(); // Will copy data
////		volScalarField weight = tweight(); // Will copy data.
//		const volScalarField& weight = tweight(); // Will refer pointer.
////		Info << "&weight == &tweight()? " << ( &weight == &tweight() ) << endl;
//	    for(int iter = 0; iter < numSmoothingIterations_; iter++)
//	    {
//        	fldSmooth = fvc::average(weight*fldSmooth) / fvc::average(weight);
//        	Info << "(cfcInterpolationSmoother) fldSmooth->name() = " << fldSmooth->name() << endl;
//	    }
//	}
//
//    return fldSmooth;
//    // tweight is automatically cleared (out-of-scope)
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>>
Foam::vofsmooth::cfcInterpolationSmoother<Type>::smoothen(
	const GeometricField<Type, fvPatchField, volMesh>& fld,
	const tmp<volScalarField>& tweight
) const{
	Info << "(cfcInterpolationSmoother) Smoothing(fld,tweight) " << fld.name() << "." << endl;

	// Declare return as CONST_REF tmp to fld. (No data is copied.)
	tmp<GeometricField<Type, fvPatchField, volMesh>> fldSmooth(fld);

	// Smoothen field
	if(tweight.empty()){ // unweighted
	    for(int iter = 0; iter < numSmoothingIterations_; iter++)
	    {
        	fldSmooth = fvc::average(fldSmooth);
	    }
	}else{ // weighted
//		const volScalarField weight = tweight(); // Will copy data
//		volScalarField weight = tweight(); // Will copy data.
		const volScalarField& weight = tweight(); // Will refer pointer.
//		Info << "&weight == &tweight()? " << ( &weight == &tweight() ) << endl;
	    for(int iter = 0; iter < numSmoothingIterations_; iter++)
	    {
        	fldSmooth = fvc::average(weight*fldSmooth) / fvc::average(weight);
        	Info << "(cfcInterpolationSmoother) fldSmooth->name() = " << fldSmooth->name() << endl;
	    }
	}

    return fldSmooth;
    // tweight is automatically cleared (out-of-scope)
}

// ************************************************************************* //

}

// ************************************************************************* //
