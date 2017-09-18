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

#include "Raeini.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace vofsmooth
{
namespace weightFactors
{
    defineTypeNameAndDebug(Raeini, 0);
    addToRunTimeSelectionTable
    (
    	weightFactor,
        Raeini,
		Istream
    );
}
}
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::vofsmooth::weightFactors::Raeini::Raeini
(
	const word& name,
	const dictionary& dict
)
:
	weightFactor(name),
	alphaName_(dict.lookupOrDefault<word>("alpha","alpha"))
{
//	// First entry of "is" is the alphaName.
//    token entry;
//    is.read(entry);
//    alphaName_ = entry.wordToken();
}

Foam::vofsmooth::weightFactors::Raeini::Raeini
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

Foam::vofsmooth::weightFactors::Raeini::~Raeini()
{}

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

namespace Foam{

tmp<volScalarField> vofsmooth::weightFactors::Raeini::weight(const fvMesh& mesh) const{
	// First lookup the required fields.
	const volScalarField* alphaPtr;
//    if(mesh.foundObject<volScalarField>(alphaName_)){
        alphaPtr = &mesh.lookupObject<volScalarField>(alphaName_);
//    }else{
    	// Try a common name or error.
//    	alphaPtr = &mesh.lookupObject<volScalarField>("alpha");
//    }
    const volScalarField& alpha = *alphaPtr;

//    Info << "(Raeini.C) alpha.instance() = " << alphaPtr->instance() << endl;

    // Limit alpha to prevent FPE
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
				"weightFactor(Raeini)",
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
