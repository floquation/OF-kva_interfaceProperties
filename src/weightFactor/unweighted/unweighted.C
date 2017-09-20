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

\*---------------------------------------------------------------------------*/

#include "unweighted.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace weightFactors
{
    defineTypeNameAndDebug(unweighted, 0);
    addToRunTimeSelectionTable
    (
    	weightFactor,
        unweighted,
		Istream
    );
}
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::weightFactors::unweighted::unweighted
(
	const word& name,
	const dictionary& dict
)
:
	weightFactor(name)//,
//	unitWeight(
//		new volScalarField(
//			IOobject
//			(
//				"one",
//				mesh.time().timeName(), // instance; unneeded string?
//				mesh, // db
//				IOobject::NO_READ,
//				IOobject::NO_WRITE
//			),
//			mesh,
//			Foam::scalar(1)
//		)
//	)
{
	// Don't have to read anything from dict for unweighted.
}

Foam::weightFactors::unweighted::unweighted
(
	const word& name
)
:
	weightFactor(name)
{}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::weightFactors::unweighted::~unweighted()
{}

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

namespace Foam{

tmp<volScalarField> weightFactors::unweighted::weight(const fvMesh& mesh) const{
	return tmp<volScalarField>(); // Return an empty field (actually, a null pointer to a volScalarField).
}

} // End namespace Foam


// ************************************************************************* //
