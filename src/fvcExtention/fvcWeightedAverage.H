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

InNamespace
    Foam::fvc

Description
    Area-weighted average a volField creating a volField, while using
    a volScalarField to weight the averaging procedure.
    result = fvc::average(fld*weight) / fvc::average(weight);

SourceFiles
    fvcWeightedAverage.C

Author (community contribution)
	Kevin van As

\*---------------------------------------------------------------------------*/


#ifndef fvcWeightedAverage_H
#define fvcWeightedAverage_H

#include "fvcAverage.H"
#include "volFieldsFwd.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Namespace fvc functions Declaration
\*---------------------------------------------------------------------------*/

namespace fvc
{
//    //- Area-weighted average a surfaceField creating a volField
//    template<class Type>
//    tmp<GeometricField<Type, fvPatchField, volMesh>> weightedAverage
//    (
//        const GeometricField<Type, fvsPatchField, surfaceMesh>&
//    );
//
//    //- Area-weighted average a surfaceField creating a volField
//    template<class Type>
//    tmp<GeometricField<Type, fvPatchField, volMesh>> weightedAverage
//    (
//        const tmp<GeometricField<Type, fvsPatchField, surfaceMesh>>&
//    );


    //- Area-weighted average a volField creating a volField, using a weight factor.
    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh>> weightedAverage
    (
        const GeometricField<Type, fvPatchField, volMesh>&,
		const volScalarField& weight
    );

    //- Area-weighted average a volField creating a volField, using a weight factor.
    //- tweight is NOT cleared upon usage, because it is likely that the weightField
    //  is required to be re-used by the caller (e.g. in several iterations).
    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh>> weightedAverage
    (
        const GeometricField<Type, fvPatchField, volMesh>&,
		const tmp<volScalarField>& tweight
    );

    //- Area-weighted average a volField creating a volField, using a weight factor.
    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh>> weightedAverage
    (
        const tmp<GeometricField<Type, fvPatchField, volMesh>>&,
		const volScalarField& weight
    );

    //- Area-weighted average a volField creating a volField, using a weight factor.
    //- tweight is NOT cleared upon usage, because it is likely that the weightField
    //  is required to be re-used by the caller (e.g. in several iterations).
    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh>> weightedAverage
    (
        const tmp<GeometricField<Type, fvPatchField, volMesh>>&,
		const tmp<volScalarField>& tweight
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "fvcWeightedAverage.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
