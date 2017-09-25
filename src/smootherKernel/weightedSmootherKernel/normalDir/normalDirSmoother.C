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

#include "normalDirSmoother.H"
#include "sqrtAlphaInt.H"
#include "cfcInterpolationSmoother.H"

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
Foam::smoothers::normalDirSmoother<Type>::normalDirSmoother
(
	const word& name,
	const dictionary& dict
)
:
	weightedSmootherKernel<Type>(
		name,
		new weightFactors::sqrtAlphaInt(
			"sqrtAlphaIntWeight(normalDirSmoother)",
			dict.lookupOrDefault<word>("alpha","alpha")
		)
	),
	numSmoothingIterations_(dict.lookupOrDefault<label>("numIts",0)),
	smoother_(
		new cfcInterpolationSmoother<Type>("interpolate(normalDirSmoother)", 1, NULL)
	)
{
	if (numSmoothingIterations_ <= 0)
	{
		int numSmoothingIterationsDefault = 2;
		WarningInFunction
			<< "Specified numIts = " << numSmoothingIterations_ << "." << nl
			<< "    " << "This value must be positive. Assuming the default value " << numSmoothingIterationsDefault << " instead." << endl;
		numSmoothingIterations_ = numSmoothingIterationsDefault;
	}

	if (!dict.found("alpha")){
		WarningInFunction
			<< "Keyword \"alpha\" was not found in " << dict.name() << "." << nl
			<< "    " << "Assuming the default value \"alpha\" instead." << nl
			<< "    " << "But... There is a good chance that there is no field with that name!" << nl
			<< "    " << "In that case you will soon receive a fatal error! Best of luck! ;)" << endl;
	}
}

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>>
Foam::smoothers::normalDirSmoother<Type>::smoothen(const GeometricField<Type, fvPatchField, volMesh>& fld) const{
	Info << "(normalDirSmoother) Smoothing(fld) " << fld.name() << "." << endl;

	// Compute weightFactor
	tmp<volScalarField> tweight = this->weight_->weight(fld.mesh());

	return smoothen(fld,tweight);
	// tweight is cleared automatically (out-of-scope)
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>>
Foam::smoothers::normalDirSmoother<Type>::smoothen(
	const GeometricField<Type, fvPatchField, volMesh>& fld,
	const tmp<volScalarField>& tweight
) const{
	Info << "(normalDirSmoother) Smoothing(fld,tweight) " << fld.name() << "." << endl;

	// Declare return; initialise as pointer to not-smoothed field. After the first iteration, it will be replaced by a new data field.
	tmp<GeometricField<Type, fvPatchField, volMesh>> fldPrev(fld);

	// Obtain weightFactor
	const volScalarField& weight = tweight(); // Copy pointer to const&. tmp is not cleared.
//	const volScalarField& weight = weight_->weight(fld.mesh())(); // SegFault! This creates a tmp. Gets a constant reference to its data. Then the tmp goes out of scope immediately, and we are left with a reference to freed memory!

	// Smoothen field
    for(int iter = 0; iter < numSmoothingIterations_; iter++)
    {
    	// Calculate the smoothed field.
    	tmp<GeometricField<Type, fvPatchField, volMesh>> fldSmooth(
    		smoother_->smoothen( fldPrev(), tweight ) // pass weight-field to prevent recomputing it numSmoothingIterations_ times.
		);

//    	Info << "(normalDirSmoother) Now calculating fldPrev." << endl;
//    	Info << "(normalDirSmoother) weight_.valid()? " << weight_.valid() << endl;
//    	Info << "(normalDirSmoother) tweight.valid()? " << tweight.valid() << endl;
//    	Info << "(normalDirSmoother) fldSmooth.valid()? " << fldSmooth.valid() << endl;

    	// Calculate fldNext and immediately overwrite fldPrev.
    	// The arithmetics creates a new data field.
    	fldPrev = (1-2*weight)*fldSmooth + 2*weight*fld;
    }

    return fldPrev;
}

// ************************************************************************* //

}

// ************************************************************************* //
