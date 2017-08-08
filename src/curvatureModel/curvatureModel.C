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

#include "curvatureModel.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
	defineTypeNameAndDebug(curvatureModel, 0);
	defineRunTimeSelectionTable(curvatureModel, dictionary);
}

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

const Foam::volScalarField& Foam::curvatureModel::retrieve_alpha() const{
	return interfaceProperties_.alpha1_;
}

const Foam::dimensionedScalar& Foam::curvatureModel::retrieve_deltaN() const{
	return interfaceProperties_.deltaN_;
}

void Foam::curvatureModel::correctContactAngle
(
	surfaceVectorField::Boundary& nHat,
	const surfaceVectorField::Boundary& gradAlphaf
) const {
	interfaceProperties_.correctContactAngle(nHat,gradAlphaf);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::curvatureModel::curvatureModel(
	const word& name,
	const interfaceProperties& interfaceProperties,
	const word& modelType
)
:
	name_(name),
	interfaceProperties_(interfaceProperties)
{
	read(modelType);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::curvatureModel::~curvatureModel()
{}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

Foam::autoPtr<Foam::curvatureModel> Foam::curvatureModel::New
(
	const word& name,
	const interfaceProperties& interfaceProperties
)
{
	// The type should be defined with an entry "curvatureModel" in transportProperties.
	// For backwards compatibility, if the entry is not specified, use the "normal" model by default.
    const word modelType(interfaceProperties.transportPropertiesDict_.lookupOrDefault<word>("curvatureModel","normal"));
    if(!interfaceProperties.transportPropertiesDict_.found("curvatureModel")){
		WarningInFunction
			<< "Keyword \"curvatureModel\" not found in transportProperties dictionary. Using the default instead." << endl;
    }
    Info<< "Selecting curvatureModel " << modelType << endl;

    if (!dictionaryConstructorTablePtr_)
    {
        FatalErrorInFunction
            << "curvatureModel table is empty"
            << exit(FatalError);
    }

//    Info<< "dictionaryConstructorTablePtr_ exists" << endl;

    typename dictionaryConstructorTable::iterator cstrIter =
    	dictionaryConstructorTablePtr_->find(modelType);

//    Info<< "type sought for" << endl;

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown curvatureModel type "
            << modelType << nl << nl
            << "Valid curvatureModel types are :" << endl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

//    Info<< "type found in table" << endl;


    return autoPtr<curvatureModel>(cstrIter()(name, interfaceProperties, modelType));
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

bool Foam::curvatureModel::read(const word& childType)
{
//	Info << "kva: curvatureModel::read(" << childType << ");" << endl;

	if (childType != "normal"){ // Normal does not need a coeffsDict. Do not enforce is for backwards compatibility.
		coeffsDict_ = interfaceProperties_.transportPropertiesDict_.subDict(childType + "Coeffs");
	}

    return true;
}



// ************************************************************************* //
