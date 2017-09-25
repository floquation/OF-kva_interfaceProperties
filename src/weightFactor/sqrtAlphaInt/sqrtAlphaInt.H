/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2016 OpenFOAM Foundation
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
    Foam::weightFactors::sqrtAlphaInt

Description
    Weightfactor that is zero anywhere but at the interface:
    	sqrt( alpha * (1-alpha) + SMALL )
    where alpha is automatically limited between 0 and 1 using minmax.

	This was was used in Eq. 16 of [1].

    [1] Raeini, Ali Q., Martin J. Blunt, and Branko Bijeljic.
    "Modelling two-phase flow in porous media at the pore scale using
    the volume-of-fluid method."
    Journal of Computational Physics 231, no. 17 (2012): 5653-5668

SourceFiles
    sqrtAlphaInt.C

Author (community contribution)
	Kevin van As/floquation
	openfoam@kevinvanas.nl

\*---------------------------------------------------------------------------*/

#ifndef weightFactors_sqrtAlphaInt_H
#define weightFactors_sqrtAlphaInt_H

#include "weightFactor.H"
#include "autoPtr.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace weightFactors
{


/*---------------------------------------------------------------------------*\
                           Class weightFactor Declaration
\*---------------------------------------------------------------------------*/

class sqrtAlphaInt
:
	public weightFactor
{

	// Private data

		word alphaName_;
//	autoPtr<volScalarField> alpha_;
//	const volScalarField& alpha_;

public:

    //- Runtime type information
        TypeName("sqrtAlphaInt");

	// Constructors

		sqrtAlphaInt(
			const word& name,
			const dictionary& dict
		);

		sqrtAlphaInt(
			const word& name,
			const word& alphaName
		);

    //- Destructor

		virtual ~sqrtAlphaInt();

    // Member Functions

		virtual tmp<volScalarField> weight(const fvMesh& mesh) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace weightFactors
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
