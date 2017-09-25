/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2015 OpenFOAM Foundation
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
    Foam::curvatureModels::normal

Description
    Provides an algorithm for calculating the interface curvature in
    the VOF formulation. It uses the CSF model of Brackbill,
    just as was originally implemented in OpenFOAM.

SourceFiles
    normal.C

Author (community contribution)
	Kevin van As/floquation
	openfoam@kevinvanas.nl
	Note: the algorithm is just a copy-paste from OpenFOAM's implementation
	in interfaceProperties.C. I only moved it to a RTS class.

\*---------------------------------------------------------------------------*/

#ifndef curvatureModel_normal_H
#define curvatureModel_normal_H

#include "curvatureModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace curvatureModels
{

/*---------------------------------------------------------------------------*\
                           Class normal Declaration
\*---------------------------------------------------------------------------*/

class normal
:
    public curvatureModel
{
    // Private data


public:

    //- Runtime type information
        TypeName("normal");


    // Constructors

        //- Construct from components
        normal
        (
            const word& name,
            const interfaceProperties& interfaceProperties,
			const word& modelType
        );


    //- Destructor
		~normal(){}


    // Member Functions

		void calculateK(volScalarField& K, surfaceScalarField& nHatf);

        //- Read transportProperties dictionary
        bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace curvatureModel
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
