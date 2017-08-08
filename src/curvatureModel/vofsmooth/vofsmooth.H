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
    Foam::curvatureModel::vofsmooth

Description
    TODO

SourceFiles
    vofsmooth.C

\*---------------------------------------------------------------------------*/

#ifndef curvatureModel_vofsmooth_H
#define curvatureModel_vofsmooth_H

#include "curvatureModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace curvatureModels
{

/*---------------------------------------------------------------------------*\
                           Class normal Declaration
\*---------------------------------------------------------------------------*/

class vofsmooth
:
    public curvatureModel
{
    // Private data

		int numSmoothingIterations_;

	// Private Member Functions

		//- vofsmooth smoother function: simple Laplacian smoother
		void smoothen
		(
			volScalarField& smooth_func
		) const;

public:

    //- Runtime type information
        TypeName("vofsmooth");


    // Constructors

        //- Construct from components
        vofsmooth
        (
            const word& name,
            const interfaceProperties& interfaceProperties,
			const word& modelType
        );


    //- Destructor
		~vofsmooth(){}


    // Member Functions

		void calculateK(volScalarField& K, surfaceScalarField& nHatf) const;

        //- Read transportProperties dictionary
        bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace curvatureModel
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
