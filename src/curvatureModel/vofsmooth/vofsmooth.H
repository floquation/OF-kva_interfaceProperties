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
    Foam::curvatureModels::vofsmooth

Description
    Provides an algorithm for calculating the interface curvature in
    the VOF formulation. It uses the CSF model of Brackbill,
    with the additional possibility to smooth the VOF field (alpha),
    smooth the curvature, and smooth gradAlpha before calculating
    the interface normal.

    See for example:
    [1] Hoang, D. A., V. Van Steijn, L. M. Portela, M. T. Kreutzer, and C. R. Kleijn.
    "Modeling of low-capillary number segmented flows in microchannels using OpenFOAM."
    In AIP Conference Proceedings, vol. 1479, no. 1, pp. 86-89. AIP, 2012.
    [2] Lafaurie, Bruno, Carlo Nardone, Ruben Scardovelli, Stéphane Zaleski, and Gianluigi Zanetti.
    "Modelling merging and fragmentation in multiphase flows with SURFER."
    Journal of Computational Physics 113, no. 1 (1994): 134-147.
    [3] Raeini, Ali Q., Martin J. Blunt, and Branko Bijeljic.
    "Modelling two-phase flow in porous media at the pore scale using
    the volume-of-fluid method." Journal of Computational Physics 231,
    no. 17 (2012): 5653-5668.

SourceFiles
    vofsmooth.C

Author (community contribution)
	Kevin van As/floquation
	openfoam@kevinvanas.nl

\*---------------------------------------------------------------------------*/

#ifndef curvatureModel_vofsmooth_H
#define curvatureModel_vofsmooth_H

#include "curvatureModel.H"
#include "smootherKernel.H"

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

		volScalarField alphaSmooth_; // Add smoothed alpha field as a member data to keep it in the objectRegistry.

		autoPtr<Foam::smoothers::smootherKernel<scalar>> alphaSmoother_;
		autoPtr<Foam::smoothers::smootherKernel<vector>> gradientAlphaSmoother_;
		autoPtr<Foam::smoothers::smootherKernel<scalar>> curvatureSmoother_;

	// Private Member Functions

		void smoothCurvature(volScalarField& K);

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

		void updateAlphaSmooth();

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
