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

#include "weightFactor.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace weightFactors
{
	defineTypeNameAndDebug(weightFactor, 0);
	defineRunTimeSelectionTable(weightFactor, Istream);
}
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::weightFactors::weightFactor::weightFactor
(
	const word& name
)
:
	name_(name)
{
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::weightFactors::weightFactor::~weightFactor()
{}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

Foam::autoPtr< Foam::weightFactors::weightFactor > Foam::weightFactors::weightFactor::New
(
	const word& name,
	const dictionary& dict
)
{
//	// First entry of "is" is the modelType.
//    token entry;
//    is.read(entry);
    word modelType;
//    modelType = entry.wordToken();
	modelType = dict.lookupOrDefault<word>("type","unweighted");
    if(!dict.found("type")){
		WarningInFunction
			<< "Keyword \"type\" not found in dictionary. Using the default (unweighted) instead." << endl;
    }
    // Next entries are the arguments to be passed to the constructor of modelType.

    Info<< "Selecting weightFactor " << modelType << " for " << name << endl;
//    Info<< "label:" << readLabel(is) << endl;

    if (!IstreamConstructorTablePtr_)
    {
        FatalErrorInFunction
            << "weightFactor table is empty"
            << exit(FatalError);
    }

//    Info<< "dictionaryConstructorTablePtr_ exists" << endl;

    typename IstreamConstructorTable::iterator cstrIter =
    		IstreamConstructorTablePtr_->find(modelType);

//    Info<< "type sought for" << endl;

    if (cstrIter == IstreamConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown weightFactor type "
            << modelType << nl << nl
            << "Valid weightFactor types are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

//    Info<< "type found in table" << endl;

    return autoPtr<weightFactor>(cstrIter()(modelType+"("+name+")", dict));
}

// ************************************************************************* //
