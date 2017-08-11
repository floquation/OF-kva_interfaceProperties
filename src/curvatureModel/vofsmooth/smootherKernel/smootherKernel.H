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
    Foam::smootherKernel

Description
    TODO

SourceFiles
    smootherKernel.C

\*---------------------------------------------------------------------------*/

#ifndef vofsmooth_smootherKernel_H
#define vofsmooth_smootherKernel_H

//#include "IOdictionary.H"
#include "volFields.H"
//#include "surfaceFields.H"
#include "typeInfo.H"
#include "runTimeSelectionTables.H"
#include "GeometricFields.H"
#include "IOstreams.H"
#include "tmp.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace vofsmooth
{


/*---------------------------------------------------------------------------*\
                           Class smootherKernel Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class smootherKernel
{
    // Private Member Functions

protected:

    // Protected data

        word name_; // KVA: Added a name, like OpenFoam's "viscosityModel". I don't understand why.

//		dictionary dict_; // TODO

public:

    //- Runtime type information
    virtual const word& type() const = 0;
//        TypeName("smootherKernel"); // KVA note: cannot do this with a templated base class.

    // Declare run-time constructor selection table

		declareRunTimeSelectionTable
		(
			autoPtr,
			smootherKernel,
			Istream,
			(
				const word& name,
				Istream& is,
				const volScalarField& alpha
			),
			(name, is, alpha)
		);


	// Constructors

		smootherKernel(
			const word& name,
			Istream& is,
			const volScalarField& alpha
		);

	// Selectors

		//- Select based on given type name
		static autoPtr<smootherKernel<Type>> New(const word& name, Istream& is, const volScalarField& alpha);

    //- Destructor

		virtual ~smootherKernel();

    // Member Functions

//		template<class Type> // KVA note: virtual method cannot be templated in C++. Instead, template the entire class.
//		virtual void smoothen(GeometricField<Type, fvPatchField, volMesh>& fld) const = 0;
		virtual tmp<GeometricField<Type, fvPatchField, volMesh>> smoothen(const GeometricField<Type, fvPatchField, volMesh>& fld) const = 0;

//		virtual GeometricField<Type, fvPatchField, volMesh> smoothen(GeometricField<Type, fvPatchField, volMesh>& fld) const{
//			return fld;
//		}

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace vofsmooth
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Add the constructor functions to the hash tables
// Inspired by src/finiteVolume/finiteVolume/snGradSchemes

#define makeSmootherKernelType(SS, Type)                                       \
    defineNamedTemplateTypeNameAndDebug(Foam::vofsmooth::SS<Foam::Type>, 0);   \
                                                                               \
    namespace Foam                                                             \
    {                                                                          \
        namespace vofsmooth                                                    \
        {                                                                      \
            smootherKernel<Type>::addIstreamConstructorToTable<SS<Type>>       \
                add##SS##Type##MeshConstructorToTable_;                        \
        }                                                                      \
    }

#define makeSmootherKernel(SS)                                                 \
                                                                               \
makeSmootherKernelType(SS, scalar)                                             \
makeSmootherKernelType(SS, vector)                                             \
makeSmootherKernelType(SS, sphericalTensor)                                    \
makeSmootherKernelType(SS, symmTensor)                                         \
makeSmootherKernelType(SS, tensor)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "smootherKernel.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


#endif

// ************************************************************************* //
