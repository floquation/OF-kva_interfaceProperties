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

#ifndef diracDeltaModels_snGradPC_H
#define diracDeltaModels_snGradPC_H

#include "diracDeltaModel.H"
#include "fieldTypes.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace diracDeltaModels
{


/*---------------------------------------------------------------------------*\
                           Class weightFactor Declaration
\*---------------------------------------------------------------------------*/

class snGradPCDeltaModel
:
	public diracDeltaModel
{

	// Private data members

		scalar Cpc_; // PC-coefficient. 0 = CSF, 0.98 = SSF, 0.5 = FSF.

public:

    //- Runtime type information
        TypeName("snGradPC");

	// Constructors

		snGradPCDeltaModel(
			const word& name,
			const dictionary& dict
		);

		snGradPCDeltaModel(
			const word& name,
			scalar Cpc
		);

    //- Destructor

		virtual ~snGradPCDeltaModel();

    // Member Functions

		virtual tmp<surfaceScalarField> delta(const volScalarField& alpha) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace diracDeltaModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
