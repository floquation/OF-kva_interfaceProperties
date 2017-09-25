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
    Foam::smoothers::normalDirSmoother

Description
    Applies a weighted smoothing filter suitable for VOF.
    The smoothing favors smoothing the field only in the interface-normal direction:
    it diffuses the field _away_ from the interface while having a minimal effect on
    the field in the interface region.
    The used smoothing operation is a simple "cell -> face -> cell" interpolation method
    (although this class may be easily adapted to read the desired smoother from a dict).

    See Eqs. 15-16 of [1].

    [1] Raeini, Ali Q., Martin J. Blunt, and Branko Bijeljic.
    "Modelling two-phase flow in porous media at the pore scale using
    the volume-of-fluid method."
    Journal of Computational Physics 231, no. 17 (2012): 5653-5668

SourceFiles
    normalDirSmoother.C

Author (community contribution)
	Kevin van As/floquation
	openfoam@kevinvanas.nl

\*---------------------------------------------------------------------------*/

#ifndef smoothers_normalDirSmoother_H
#define smoothers_normalDirSmoother_H

#include "weightedSmootherKernel.H"
#include "weightFactor.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace smoothers
{

/*---------------------------------------------------------------------------*\
                           Class normal Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class normalDirSmoother
:
    public weightedSmootherKernel<Type>
{
    // Private data

		int numSmoothingIterations_;
		autoPtr<Foam::smoothers::weightedSmootherKernel<Type>> smoother_;

	// Private Member Functions

public:

    //- Runtime type information
        TypeName("normalDir");

    // Constructors

        //- Construct from components
        normalDirSmoother
        (
			const word& name,
			const dictionary & dict
        );


    //- Destructor
		virtual ~normalDirSmoother(){}


    // Member Functions

		virtual tmp<GeometricField<Type, fvPatchField, volMesh>> smoothen(const GeometricField<Type, fvPatchField, volMesh>& fld) const;
		virtual tmp<GeometricField<Type, fvPatchField, volMesh>> smoothen(
			const GeometricField<Type, fvPatchField, volMesh>& fld,
			const tmp<volScalarField>& tweight
		) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace smoothers
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "normalDirSmoother.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
