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

#include "smootherKernel.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

//namespace Foam
//{
//	defineTypeNameAndDebug(smootherKernel, 0);
//	defineRunTimeSelectionTable(smootherKernel, Istream);
//}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
Foam::smoothers::smootherKernel<Type>::smootherKernel
(
	const word& name
)
:
	name_(name)//,
//	dict_(dict)
{
//	read();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
Foam::smoothers::smootherKernel<Type>::~smootherKernel()
{}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

template<class Type>
Foam::autoPtr< Foam::smoothers::smootherKernel<Type> >
Foam::smoothers::smootherKernel<Type>::New
(
	const word& name,
	const dictionary& dict
//	Istream& is
)
{

//	alpha.db().lookupObject<volScalarField>("kiwi");

	// First entry of "is" is the modelType.
    word modelType;
//    token entry;
//    is.read(entry);
//    modelType = entry.wordToken();
    modelType = dict.lookupOrDefault<word>("type","none");
    // Next entries are the arguments to be passed to the constructor of modelType.

    Info<< "Selecting smootherKernel " << modelType << " for " << name << endl;
//    Info<< "label:" << readLabel(is) << endl;

    if (!IstreamConstructorTablePtr_)
    {
        FatalErrorInFunction
            << "smootherKernel table is empty"
            << exit(FatalError);
    }

//    Info<< "dictionaryConstructorTablePtr_ exists" << endl;

    typename IstreamConstructorTable::iterator cstrIter =
    		IstreamConstructorTablePtr_->find(modelType);

//    Info<< "type sought for" << endl;

    if (cstrIter == IstreamConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown smootherKernel type "
            << modelType << nl << nl
            << "Valid smootherKernel types are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

//    Info<< "type found in table" << endl;


    return autoPtr<smootherKernel>(cstrIter()(modelType+"("+name+")", dict));
}

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class Type>
Foam::tmp<Foam::GeometricField<Type, Foam::fvPatchField, Foam::volMesh>>
Foam::smoothers::smootherKernel<Type>::smoothen(const tmp<GeometricField<Type, fvPatchField, volMesh>>& tfld) const{
	Info << "(smootherKernel) Using smootherKernel::smoothen(tmp)." << endl;
    tmp<GeometricField<Type, fvPatchField, volMesh>> tReturn
    (
        smoothen(tfld())
    );
    tfld.clear();
    return tReturn;
}

//bool Foam::smoothers::smootherKernel::read()
//{
////    alpha1_.mesh().solverDict(alpha1_.name()).lookup("cAlpha") >> cAlpha_;
////    transportPropertiesDict_.lookup("sigma") >> sigma_;
////
////    transportPropertiesDict_.subDict( wordList(transportPropertiesDict_.lookup("phases"))[0] ).lookup("rho") >> rho1_; // KVA
////    transportPropertiesDict_.subDict( wordList(transportPropertiesDict_.lookup("phases"))[1] ).lookup("rho") >> rho2_; // KVA
////
////    bool result = readSurfaceTensionModel(); // KVA
////
////    curvatureModel_->read(); // KVA
//
//    return result && true;
//}

// ************************************************************************* //
