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

#include "diracDeltaModel.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace diracDeltaModels
{
	defineTypeNameAndDebug(diracDeltaModel, 0);
	defineRunTimeSelectionTable(diracDeltaModel, dict);
}
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::diracDeltaModels::diracDeltaModel::diracDeltaModel
(
	const word& name
)
:
	name_(name)
{
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::diracDeltaModels::diracDeltaModel::~diracDeltaModel()
{}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

Foam::autoPtr< Foam::diracDeltaModels::diracDeltaModel > Foam::diracDeltaModels::diracDeltaModel::New
(
	const word& name,
	const dictionary& dict
)
{
	word modelType = dict.lookupOrDefault<word>("type","snGrad");
    if(!dict.found("type")){
		WarningInFunction
			<< "Keyword \"type\" not found in dictionary. Using the default (snGrad) instead." << endl;
    }
    // Next entries are the arguments to be passed to the constructor of modelType.

    Info<< "Selecting diracDeltaModel " << modelType << " for " << name << endl;

    if (!dictConstructorTablePtr_)
    {
        FatalErrorInFunction
            << "diracDeltaModel table is empty"
            << exit(FatalError);
    }

    typename dictConstructorTable::iterator cstrIter =
    		dictConstructorTablePtr_->find(modelType);

    if (cstrIter == dictConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown diracDeltaModel type "
            << modelType << nl << nl
            << "Valid diracDeltaModel types are :" << endl
            << dictConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<diracDeltaModel>(cstrIter()("delta="+modelType+"("+name+")", dict));
}

// ************************************************************************* //
