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
    Foam::vofsmooth::smootherKernels::none

Description
    TODO

SourceFiles
    none.C

\*---------------------------------------------------------------------------*/

#ifndef smoothers_weightedSmootherKernel_H
#define smoothers_weightedSmootherKernel_H

#include "smootherKernel.H"
#include "weightFactor.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace smoothers
{

/*---------------------------------------------------------------------------*\
                           Class normal Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class weightedSmootherKernel
:
    public smootherKernel<Type>
{
    // Private data

	// Private Member Functions

protected:

	// Protected data

		autoPtr<weightFactors::weightFactor> weight_;

public:

    //- Runtime type information
//	    virtual const word& type() const = 0;
	    //TypeName("weightedSmootherKernel"); // KVA note: cannot do this with a templated base class.

    // Constructors

        //- Construct from dictionary
        weightedSmootherKernel
        (
			const word& name,
			const dictionary & dict
        );

        //- Construct from components
        weightedSmootherKernel
        (
			const word& name,
			weightFactors::weightFactor* weight
        );


    //- Destructor
		virtual ~weightedSmootherKernel(){}


    // Member Functions

		virtual tmp<GeometricField<Type, fvPatchField, volMesh>> smoothen(const GeometricField<Type, fvPatchField, volMesh>& fld) const = 0;
		virtual tmp<GeometricField<Type, fvPatchField, volMesh>> smoothen(
			const GeometricField<Type, fvPatchField, volMesh>& fld,
			const tmp<volScalarField>& tweight
		) const = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace smoothers
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "weightedSmootherKernel.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
