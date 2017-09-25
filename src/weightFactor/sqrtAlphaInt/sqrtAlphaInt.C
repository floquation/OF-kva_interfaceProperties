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

#include "sqrtAlphaInt.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace weightFactors
{
    defineTypeNameAndDebug(sqrtAlphaInt, 0);
    addToRunTimeSelectionTable
    (
    	weightFactor,
        sqrtAlphaInt,
		Istream
    );
}
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::weightFactors::sqrtAlphaInt::sqrtAlphaInt
(
	const word& name,
	const dictionary& dict
)
:
	weightFactor(name),
	alphaName_(dict.lookupOrDefault<word>("alpha","alpha"))
{
	if (!dict.found("alpha")){
		WarningInFunction
			<< "Keyword \"alpha\" was not found in " << dict.name() << "." << nl
			<< "    " << "Assuming the default value \"alpha\" instead." << nl
			<< "    " << "But... There is a good chance that there is no field with that name!" << nl
			<< "    " << "In that case you will soon receive a fatal error! Best of luck! ;)" << endl;
	}
}

Foam::weightFactors::sqrtAlphaInt::sqrtAlphaInt
(
	const word& name,
	const word& alphaName
)
:
	weightFactor(name),
	alphaName_(alphaName)
{
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::weightFactors::sqrtAlphaInt::~sqrtAlphaInt()
{}

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

namespace Foam{

tmp<volScalarField> weightFactors::sqrtAlphaInt::weight(const fvMesh& mesh) const{
	// First lookup the required fields.
	const volScalarField* alphaPtr;
//    if(mesh.foundObject<volScalarField>(alphaName_)){
        alphaPtr = &mesh.lookupObject<volScalarField>(alphaName_);
//    }else{
    	// Try a common name or error.
//    	alphaPtr = &mesh.lookupObject<volScalarField>("alpha");
//    }
    const volScalarField& alpha = *alphaPtr; // Initialise via a pointer to deal with the if/then/else above.

    // Limit alpha to prevent FPE in sqrt below
    const volScalarField limitedAlpha
    (
        "limitedAlpha1",
        min(max(alpha, scalar(0)), scalar(1))
    );

    // Then calculate the weight factor.
    return tmp<volScalarField>
    (
		new volScalarField(
			IOobject
			(
				"weightFactor(sqrtAlphaInt)",
				mesh.time().timeName(),
				mesh,
				IOobject::NO_READ,
				IOobject::NO_WRITE
			),
			sqrt(limitedAlpha*(scalar(1)-limitedAlpha) + SMALL)
    	)
    );
}

} // End namespace Foam


// ************************************************************************* //
