/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2017 OpenFOAM Foundation
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
    Foam::interfaceProperties

Description
    Contains the interface properties.

    Properties to aid interFoam:
    -# Correct the alpha boundary condition for dynamic contact angle.
    -# Calculate interface curvature.

KVA_Modifications
	.H/.C: Added field curvatureModel_
	.H: curvatureModel is a friend class
	.C: Replaced calculateK()'s content by a forward call to curvatureModel.
	.C: Removed some #includes, as now curvatureModel's child's .C needs those.
	.C: Forward the read() call to the curvatureModel.

    .H/.C: Added rho1_ and rho2_ to constructor/read
    .C: Added densityWeighted surface tension calculation to "surfaceTensionForce()".
    .H/.C: Added "densityWeighted_" switch as private variable and in read method

SourceFiles
    interfaceProperties.C

\*---------------------------------------------------------------------------*/

#ifndef interfaceProperties_H
#define interfaceProperties_H

#include "IOdictionary.H"
#include "surfaceTensionModel.H"
#include "volFields.H"
#include "surfaceFields.H"
#include "curvatureModel.H" // KVA

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class curvatureModel; // KVA

/*---------------------------------------------------------------------------*\
                           Class interfaceProperties Declaration
\*---------------------------------------------------------------------------*/

class interfaceProperties
{
    // Private data

        //- Keep a reference to the transportProperties dictionary
        const dictionary& transportPropertiesDict_;

        //- Compression coefficient
        scalar cAlpha_;

        //- Surface tension
        autoPtr<surfaceTensionModel> sigmaPtr_;
    	bool densityWeighted_ = false; // KVA

        //- Density // KVA: For simplicity sake, assume immiscible such that rho = alpha * rho1_ + (1-alpha) * rho2_
        dimensionedScalar rho1_; // KVA
        dimensionedScalar rho2_; // KVA

        //- Stabilisation for normalisation of the interface normal
        const dimensionedScalar deltaN_;

        const volScalarField& alpha1_;
        const volVectorField& U_;
        surfaceScalarField nHatf_;
        volScalarField K_;

        friend class curvatureModel; // KVA: Allow curvatureModel to access private members (alpha1_, deltaN_, ...)
        autoPtr<curvatureModel> curvatureModel_; // KVA: class that computes the curvature (K_)


    // Private Member Functions

        //- Disallow default bitwise copy construct and assignment
        interfaceProperties(const interfaceProperties&);
        void operator=(const interfaceProperties&);

        //- Correction for the boundary condition on the unit normal nHat on
        //  walls to produce the correct contact dynamic angle
        //  calculated from the component of U parallel to the wall
        void correctContactAngle
        (
            surfaceVectorField::Boundary& nHat,
            const surfaceVectorField::Boundary& gradAlphaf
        ) const;

        //- Re-calculate the interface curvature
        void calculateK();

        bool readSurfaceTensionModel(); // KVA

public:

    //- Conversion factor for degrees into radians
    static const scalar convertToRad;


    // Constructors

        //- Construct from volume fraction field gamma and IOdictionary
        interfaceProperties
        (
            const volScalarField& alpha1,
            const volVectorField& U,
            const IOdictionary&
        );


    // Member Functions

        scalar cAlpha() const // KVA note: Used by solvers for interfacial compression
        {
            return cAlpha_;
        }

        const dimensionedScalar& deltaN() const // KVA note: Not called by anyone??
        {
            return deltaN_;
        }

        const surfaceScalarField& nHatf() const // KVA note: Used by solvers for interfacial compression
        {
            return nHatf_;
        }

        tmp<volScalarField> sigmaK() const;

        tmp<surfaceScalarField> surfaceTensionForce() const; // KVA note: Used by solvers in UEqn.H

        //- Indicator of the proximity of the interface
        //  Field values are 1 near and 0 away for the interface.
        tmp<volScalarField> nearInterface() const; // KVA note: Used by solvers in alphaCourantNo.H

        void correct();

        //- Read transportProperties dictionary
        bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
