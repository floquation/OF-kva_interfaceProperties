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
	Istream& is,
	const volScalarField& alpha
)
:
	smootherKernel<Type>(name,is,alpha),
	numSmoothingIterations_(readLabel(is))
{
//	Info << "is contained numSmoothingIterations_ = " << numSmoothingIterations_ << endl; // works!
	// TODO: Read "weightFactor" class from "is".

	if (numSmoothingIterations_ <= 0)
	{
		int numSmoothingIterationsDefault = 2;
		WarningInFunction
			<< "Specified numSmoothingIterations = " << numSmoothingIterations_ << "." << nl
			<< "    " << "This value must be positive. Assuming the default value " << numSmoothingIterationsDefault << " instead." << endl;
		numSmoothingIterations_ = numSmoothingIterationsDefault;
	}
}

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>>
Foam::vofsmooth::cfcInterpolationSmoother<Type>::smoothen(const GeometricField<Type, fvPatchField, volMesh>& fld) const{
	tmp<GeometricField<Type, fvPatchField, volMesh>> fldSmooth(fld);
	Info << "fldSmooth = tmp?: " << fldSmooth.isTmp() << endl;
    for(int iter = 0; iter < numSmoothingIterations_; iter++)
    {
//    	fld = fvc::average(fvc::interpolate(fld));
    	fldSmooth = fvc::average(fldSmooth); // uses "linearInterpolate" automatically
    	Info << "fldSmooth = tmp?: " << fldSmooth.isTmp() << endl;
    	// TODO: Use a weightFactor
    }

    return fldSmooth;
}

// ************************************************************************* //

}

// ************************************************************************* //
