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
    Foam::diracDeltaModel

Description
    TODO

SourceFiles
    diracDeltaModel.C

\*---------------------------------------------------------------------------*/

#ifndef diracDeltaModels_diracDeltaModel_H
#define diracDeltaModels_diracDeltaModel_H

#include "volFieldsFwd.H"
#include "surfaceFieldsFwd.H"
#include "dictionary.H"
#include "typeInfo.H"
#include "runTimeSelectionTables.H"
#include "tmp.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace diracDeltaModels
{


/*---------------------------------------------------------------------------*\
                           Class diracDeltaModel Declaration
\*---------------------------------------------------------------------------*/

class diracDeltaModel
{
    // Private Member Functions

protected:

    // Protected data

        word name_; // KVA: Added a name, like OpenFoam's "viscosityModel". I don't understand why.

public:

    //- Runtime type information
        TypeName("diracDeltaModel");

    // Declare run-time constructor selection table

		declareRunTimeSelectionTable
		(
			autoPtr,
			diracDeltaModel,
			dict,
			(
				const word& name,
				const dictionary& dict
			),
			(name, dict)
		);


	// Constructors

		diracDeltaModel(
			const word& name
		);

	// Selectors

		//- Select based on given type name
		static autoPtr<diracDeltaModel> New(const word& name, const dictionary& dict, bool quiet = false);

    //- Destructor

		virtual ~diracDeltaModel();

    // Member Functions

		// Returns an approximation to the Dirac delta function, based on a Heaviside field (e.g. the VOF field).
		virtual tmp<surfaceScalarField> delta(const volScalarField& alpha) const = 0;

		//- Return name
		const word& name() const
		{
		    return name_;
		}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace diracDeltaModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
