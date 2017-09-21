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

#ifndef diracDeltaModels_snGrad_H
#define diracDeltaModels_snGrad_H

#include "diracDeltaModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace diracDeltaModels
{


/*---------------------------------------------------------------------------*\
                           Class weightFactor Declaration
\*---------------------------------------------------------------------------*/

class snGradDeltaModel
:
	public diracDeltaModel
{

public:

    //- Runtime type information
        TypeName("snGrad");

	// Constructors

		snGradDeltaModel(
			const word& name,
			const dictionary& dict
		);

		snGradDeltaModel(
			const word& name
		);

    //- Destructor

		virtual ~snGradDeltaModel();

    // Member Functions

		virtual tmp<surfaceScalarField> delta(const volScalarField& alpha) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace diracDeltaModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
