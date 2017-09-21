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

#include "snGradPCDeltaModel.H"
#include "addToRunTimeSelectionTable.H"
#include "fvcSnGrad.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace diracDeltaModels
{
    defineTypeNameAndDebug(snGradPCDeltaModel, 0);
    addToRunTimeSelectionTable
    (
    	diracDeltaModel,
        snGradPCDeltaModel,
		dict
    );
}
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::diracDeltaModels::snGradPCDeltaModel::snGradPCDeltaModel
(
	const word& name,
	const dictionary& dict
)
:
	diracDeltaModel(name),
	Cpc_(dict.lookupOrDefault<scalar>("Cpc",0.5))
{
	scalar CpcDefault = 0.5;
	if (!( 0 <= Cpc_ && Cpc_ < 1 ))
	{
		WarningInFunction
			<< "Specified Cpc = " << Cpc_ << " in " << dict.name() << "." << nl
			<< "    " << "This value must be 0 <= Cpc < 1. Assuming the default value " << CpcDefault << " instead." << endl;
		Cpc_ = CpcDefault;
	}
	if (!dict.found("Cpc"))
	{
		WarningInFunction
			<< "Entry Cpc was not found in " << dict.name() << "." << nl
			<< "    " << "Assuming the default value " << CpcDefault << " instead." << endl;
		Cpc_ = CpcDefault;
	}
}

Foam::diracDeltaModels::snGradPCDeltaModel::snGradPCDeltaModel
(
	const word& name,
	scalar Cpc
)
:
	diracDeltaModel(name),
	Cpc_(Cpc)
{
	scalar CpcDefault = 0.5;
	if (!( 0 <= Cpc_ && Cpc_ < 1 ))
	{
		WarningInFunction
			<< "Specified Cpc = " << Cpc_ << " somewhere in the code (programming logic error!)." << nl
			<< "    " << "This value must be 0 <= Cpc < 1. Assuming the default value " << CpcDefault << " instead." << endl;
		Cpc_ = CpcDefault;
	}
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::diracDeltaModels::snGradPCDeltaModel::~snGradPCDeltaModel()
{}

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

namespace Foam{

tmp<surfaceScalarField> diracDeltaModels::snGradPCDeltaModel::delta(const volScalarField& alpha) const{
	// First compute alphaPC
	volScalarField alphaPC(
		IOobject
		(
				"PC(" + alpha.name() + ")", // new name
				alpha().instance(), // same instance
				alpha().mesh(), // same registry
				IOobject::NO_READ,
				IOobject::NO_WRITE,
				true // register
		),
		(1/(1-Cpc_))
		*
		(
			min(
				max(
					alpha,
					Cpc_/2
				),
				1-Cpc_/2
			)
			-
			Cpc_/2
		)
	);

	// Then calculate the surface-normal gradient based on alphaPC
	return fvc::snGrad<scalar>(alphaPC);
}

} // End namespace Foam


// ************************************************************************* //
