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
    Foam::weightFactor

Description
    TODO

SourceFiles
    weightFactor.C

\*---------------------------------------------------------------------------*/

#ifndef weightFactors_weightFactor_H
#define weightFactors_weightFactor_H

#include "volFields.H"
#include "typeInfo.H"
#include "runTimeSelectionTables.H"
//#include "GeometricFields.H"
//#include "IOstreams.H"
#include "tmp.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace weightFactors
{


/*---------------------------------------------------------------------------*\
                           Class weightFactor Declaration
\*---------------------------------------------------------------------------*/

class weightFactor
{
    // Private Member Functions

protected:

    // Protected data

        word name_; // KVA: Added a name, like OpenFoam's "viscosityModel". I don't understand why.

public:

    //- Runtime type information
        TypeName("weightFactor");

    // Declare run-time constructor selection table

		declareRunTimeSelectionTable
		(
			autoPtr,
			weightFactor,
			Istream,
			(
				const word& name,
				const dictionary& dict
			),
			(name, dict)
		);


	// Constructors

		weightFactor(
			const word& name
		);

	// Selectors

		//- Select based on given type name
		static autoPtr<weightFactor> New(const word& name, const dictionary& dict);

    //- Destructor

		virtual ~weightFactor();

    // Member Functions

		virtual tmp<volScalarField> weight(const fvMesh& mesh) const = 0;

//		template<class Type> // KVA note: virtual method cannot be templated in C++. Instead, template the entire class.
//		virtual void smoothen(GeometricField<Type, fvPatchField, volMesh>& fld) const = 0;
//		virtual tmp<GeometricField<Type, fvPatchField, volMesh>> smoothen(const GeometricField<Type, fvPatchField, volMesh>& fld) const = 0;

//		virtual GeometricField<Type, fvPatchField, volMesh> smoothen(GeometricField<Type, fvPatchField, volMesh>& fld) const{
//			return fld;
//		}

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace weightFactors
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
