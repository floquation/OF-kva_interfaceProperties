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

#ifndef vofsmooth_weightFactors_Raeini_H
#define vofsmooth_weightFactors_Raeini_H

#include "weightFactor.H"
#include "autoPtr.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace vofsmooth
{
namespace weightFactors
{


/*---------------------------------------------------------------------------*\
                           Class weightFactor Declaration
\*---------------------------------------------------------------------------*/

class Raeini
:
		public weightFactor
{

	// Private data

		word alphaName_;
//	autoPtr<volScalarField> alpha_;
//	const volScalarField& alpha_;

public:

    //- Runtime type information
        TypeName("Raeini");

	// Constructors

		Raeini(
			const word& name,
			const dictionary& dict
		);

		Raeini(
			const word& name,
			const word& alphaName
		);

    //- Destructor

		virtual ~Raeini();

    // Member Functions

		virtual tmp<volScalarField> weight(const fvMesh& mesh) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace weightFactors
} // End namespace vofsmooth
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
