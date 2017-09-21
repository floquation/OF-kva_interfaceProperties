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
	Cpc_(dict.lookupOrDefault<float>("Csk",0.5))
{
}

Foam::diracDeltaModels::snGradPCDeltaModel::snGradPCDeltaModel
(
	const word& name,
	float Cpc
)
:
	diracDeltaModel(name),
	Cpc_(Cpc)
{
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::diracDeltaModels::snGradPCDeltaModel::~snGradPCDeltaModel()
{}

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

namespace Foam{

tmp<surfaceScalarField> diracDeltaModels::snGradPCDeltaModel::delta(const volScalarField& alpha) const{
	return fvc::snGrad<scalar>(alpha); // TODO: PC
}

} // End namespace Foam


// ************************************************************************* //
