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
    Foam::weightFactors::weightFactor

Description
    Abstract base class for functions that are commonly used as weighting factors.

    Beware: the child "unweighted" returns an empty tmp to represent that
    no weighting is required. Callers should check:
    	tmp<volScalarField> tresult = weightFactor_.weight(mesh);
		if(tresult.empty()){
			// unweighted
		} else {
			// weighted
		}

SourceFiles
    weightFactor.C

Author (community contribution)
	Kevin van As/floquation
	openfoam@kevinvanas.nl

\*---------------------------------------------------------------------------*/

#ifndef weightFactors_weightFactor_H
#define weightFactors_weightFactor_H

#include "volFields.H"
#include "typeInfo.H"
#include "runTimeSelectionTables.H"
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

protected:

    // Protected data

        word name_;

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
		static autoPtr<weightFactor> New(const word& name, const dictionary& dict, bool quiet = false);

    //- Destructor

		virtual ~weightFactor();

    // Member Functions

		virtual tmp<volScalarField> weight(const fvMesh& mesh) const = 0;

		//- Return name
		const word& name() const
		{
		    return name_;
		}

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace weightFactors
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
