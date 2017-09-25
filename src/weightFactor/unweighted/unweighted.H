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
    Foam::weightFactors::unweighted

Description
    Unweighted weight factor.
    Rather than returning a field filled with ones, this returns an empty tmp
    to save computational power.
    Callers should check:
    	tmp<volScalarField> tresult = weightFactor_.weight(mesh);
		if(tresult.empty()){
			// unweighted
		} else {
			// weighted
		}

SourceFiles
    unweighted.C

Author (community contribution)
	Kevin van As/floquation
	openfoam@kevinvanas.nl

\*---------------------------------------------------------------------------*/

#ifndef weightFactors_unweighted_H
#define weightFactors_unweighted_H

#include "weightFactor.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace weightFactors
{


/*---------------------------------------------------------------------------*\
                           Class weightFactor Declaration
\*---------------------------------------------------------------------------*/

class unweighted
:
	public weightFactor
{
    // Private data

//	const volScalarField& unitWeight;

protected:

    // Protected data

public:

    //- Runtime type information
        TypeName("unweighted");

	// Constructors

		unweighted(
			const word& name,
			const dictionary& dict
		);

		unweighted(
			const word& name
		);

    //- Destructor

		virtual ~unweighted();

    // Member Functions

		virtual tmp<volScalarField> weight(const fvMesh& mesh) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace weightFactors
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
