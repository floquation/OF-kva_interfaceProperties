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

Class
    Foam::vofsmooth::smootherKernels::none

Description
    TODO

SourceFiles
    none.C

\*---------------------------------------------------------------------------*/

#ifndef vofsmooth_cfcInterpolationSmoother_H
#define vofsmooth_cfcInterpolationSmoother_H

#include "weightedSmootherKernel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace vofsmooth
{

/*---------------------------------------------------------------------------*\
                           Class normal Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class cfcInterpolationSmoother
:
    public weightedSmootherKernel<Type>
{
    // Private data

		int numSmoothingIterations_;

	// Private Member Functions

public:

    //- Runtime type information
        TypeName("interpolate");

    // Constructors

        //- Construct from dictionary
        cfcInterpolationSmoother
        (
			const word& name,
			const dictionary & dict
        );

        //- Construct from components
        cfcInterpolationSmoother
        (
			const word& name,
			int numIts,
			weightFactor* weight
        );


    //- Destructor
		virtual ~cfcInterpolationSmoother(){}


    // Member Functions

		virtual tmp<GeometricField<Type, fvPatchField, volMesh>> smoothen(const GeometricField<Type, fvPatchField, volMesh>& fld) const;
		virtual tmp<GeometricField<Type, fvPatchField, volMesh>> smoothen(
			const GeometricField<Type, fvPatchField, volMesh>& fld,
			const tmp<volScalarField>& tweight
		) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace vofsmooth
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "cfcInterpolationSmoother.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
