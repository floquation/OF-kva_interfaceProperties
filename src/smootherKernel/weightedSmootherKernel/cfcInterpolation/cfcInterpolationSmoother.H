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
    Foam::smootherKernel::cfcInterpolationSmoother

Description
    Smoothes the field using a simple "cell -> face -> cell" interpolation.

	See for example:
    [1] Hoang, D. A., V. Van Steijn, L. M. Portela, M. T. Kreutzer, and C. R. Kleijn.
    "Modeling of low-capillary number segmented flows in microchannels using OpenFOAM."
    In AIP Conference Proceedings, vol. 1479, no. 1, pp. 86-89. AIP, 2012.
    [2] Lafaurie, Bruno, Carlo Nardone, Ruben Scardovelli, Stéphane Zaleski, and Gianluigi Zanetti.
    "Modelling merging and fragmentation in multiphase flows with SURFER."
    Journal of Computational Physics 113, no. 1 (1994): 134-147.

SourceFiles
    cfcInterpolationSmoother.C

Author (community contribution)
	Kevin van As/floquation
	openfoam@kevinvanas.nl

\*---------------------------------------------------------------------------*/

#ifndef smoothers_cfcInterpolationSmoother_H
#define smoothers_cfcInterpolationSmoother_H

#include "weightedSmootherKernel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace smoothers
{

/*---------------------------------------------------------------------------*\
                           Class normal Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class cfcInterpolationSmoother
:
    public weightedSmootherKernel<Type>
{
    // Private data

		int numSmoothingIterations_;

	// Private Member Functions

public:

    //- Runtime type information
        TypeName("interpolate");

    // Constructors

        //- Construct from dictionary
        cfcInterpolationSmoother
        (
			const word& name,
			const dictionary & dict
        );

        //- Construct from components
        cfcInterpolationSmoother
        (
			const word& name,
			int numIts,
			weightFactors::weightFactor* weight
        );


    //- Destructor
		virtual ~cfcInterpolationSmoother(){}


    // Member Functions

		virtual tmp<GeometricField<Type, fvPatchField, volMesh>> smoothen(const GeometricField<Type, fvPatchField, volMesh>& fld) const;
		virtual tmp<GeometricField<Type, fvPatchField, volMesh>> smoothen(
			const GeometricField<Type, fvPatchField, volMesh>& fld,
			const tmp<volScalarField>& tweight
		) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace smoothers
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "cfcInterpolationSmoother.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
