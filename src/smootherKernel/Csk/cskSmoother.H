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
    Foam::smoothers::cskSmoother

Description
    Slow down the smoothing by taking the Csk-weighted average
    of a smoothed field and the non-smoothed field of the previous iterations.

    The cskSmoother uses another smoother to do the actual smoothing.
    Csk = 0: no smoothing
    Csk = 1: pure smoothing (then this intermediate class does in effect nothing)

    See e.g. Eq. (12) of [1].

    [1] Raeini, Ali Q., Martin J. Blunt, and Branko Bijeljic.
    "Modelling two-phase flow in porous media at the pore scale using
    the volume-of-fluid method."
    Journal of Computational Physics 231, no. 17 (2012): 5653-5668

SourceFiles
    cskSmoother.C

Author (community contribution)
	Kevin van As/floquation
	openfoam@kevinvanas.nl

\*---------------------------------------------------------------------------*/

#ifndef smoothers_cskSmoother_H
#define smoothers_cskSmoother_H

#include "smootherKernel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace smoothers
{

/*---------------------------------------------------------------------------*\
                           Class normal Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class cskSmoother
:
    public smootherKernel<Type>
{
    // Private data

		int numSmoothingIterations_;
		scalar Csk_;
		autoPtr<Foam::smoothers::smootherKernel<Type>> smoother_;

	// Private Member Functions

public:

    //- Runtime type information
        TypeName("Csk");

    // Constructors

        //- Construct from components
        cskSmoother
        (
			const word& name,
			const dictionary & dict
        );


    //- Destructor
		virtual ~cskSmoother(){}


    // Member Functions

		virtual tmp<GeometricField<Type, fvPatchField, volMesh>> smoothen(const GeometricField<Type, fvPatchField, volMesh>& fld) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace smoothers
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "cskSmoother.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
