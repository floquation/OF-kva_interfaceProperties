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
    Foam::curvatureModel

Description
    Class responsible for calculating the curvature (K) based on the VOF field.

SourceFiles
    curvatureModel.C

\*---------------------------------------------------------------------------*/

#ifndef curvatureModel_H
#define curvatureModel_H

#include "dictionary.H"
#include "runTimeSelectionTables.H"
#include "volFields.H"
#include "surfaceFields.H"
#include "interfaceProperties.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class interfaceProperties;

/*---------------------------------------------------------------------------*\
                           Class curvatureModel Declaration
\*---------------------------------------------------------------------------*/

class curvatureModel
{

protected:

    // Protected data

        word name_; // KVA: Added a name, like OpenFoam's "viscosityModel". I don't understand why.

        const interfaceProperties& interfaceProperties_;

		dictionary coeffsDict_; // The parameter subdict of the present model

    // Protected Member Functions

        // Allow children to obtain these fields from interfaceProperties:
        // KVA note: Implementation must be in the C file, because of the class forward declaration.
        const volScalarField& retrieve_alpha() const;
        const dimensionedScalar& retrieve_deltaN() const;

        // Allow children to call these functions from interfaceProperties:
        void correctContactAngle
        (
            surfaceVectorField::Boundary& nHat,
            const surfaceVectorField::Boundary& gradAlphaf
        ) const;

public:

    //- Runtime type information

        TypeName("curvatureModel");

    // Declare run-time constructor selection table

		declareRunTimeSelectionTable
		(
			autoPtr,
			curvatureModel,
			dictionary,
			(
				const word& name,
				const interfaceProperties& interfaceProperties,
				const word& modelType
			),
			(name, interfaceProperties, modelType)
		);

    // Constructors

		curvatureModel(
			const word& name,
			const interfaceProperties& interfaceProperties,
			const word& modelType
		);

	// Selectors

		//- Select based on given type name
		static autoPtr<curvatureModel> New(const word& type, const interfaceProperties& interfaceProperties);


	//- Destructor

		virtual ~curvatureModel(); // abstract class needs a virtual destructor

	// Member Functions

		//- Forward curvature calculation to a child class
		virtual void calculateK(volScalarField& K, surfaceScalarField& nHatf) const = 0;

        //- Read transportProperties.coeffs dictionary
        virtual bool read() = 0; // update all model parameters

        // Updates typenameCoeffs dict
        // I've had the child pass its typename, such that the parent could manage the dictionary.
        // Otherwisely every child would have to manage its own dictionary in an identical way, which is code duplication.
        bool read(const word& childType);
};



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
