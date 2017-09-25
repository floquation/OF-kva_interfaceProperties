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
    Interpolates a volField to a surfaceField, while using
    a volScalarField to weight the procedure:
    result = fvc::interpolate(fld*weight) / fvc::interpolate(weight);

SourceFiles
    fvcWeightedSurfaceInterpolate.C

Author (community contribution)
	Kevin van As/floquation
	openfoam@kevinvanas.nl

\*---------------------------------------------------------------------------*/


#ifndef fvcWeightedInterpolate_H
#define fvcWeightedInterpolate_H

#include "surfaceInterpolate.H"
#include "volFieldsFwd.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Namespace fvc functions Declaration
\*---------------------------------------------------------------------------*/

namespace fvc
{
    //- Interpolates a volField to a surfaceField using a weighted interpolation:
	//  interpolate( field * weight )/interpolate( weight )
    template<class Type>
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh>> weightedInterpolate
    (
        const GeometricField<Type, fvPatchField, volMesh>&,
		const volScalarField& weight
    );

    //- Interpolates a volField to a surfaceField using a weighted interpolation:
    //  interpolate( field * weight )/interpolate( weight )
    //- tweight is NOT cleared upon usage, because it is likely that the weightField
    //  is required to be re-used by the caller (e.g. in several iterations).
    template<class Type>
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh>> weightedInterpolate
    (
        const GeometricField<Type, fvPatchField, volMesh>&,
		const tmp<volScalarField>& tweight
    );

    //- Interpolates a volField to a surfaceField using a weighted interpolation:
    //  interpolate( field * weight )/interpolate( weight )
    template<class Type>
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh>> weightedInterpolate
    (
        const tmp<GeometricField<Type, fvPatchField, volMesh>>&,
		const volScalarField& weight
    );

    //- Interpolates a volField to a surfaceField using a weighted interpolation:
    //  interpolate( field * weight )/interpolate( weight )
    //- tweight is NOT cleared upon usage, because it is likely that the weightField
    //  is required to be re-used by the caller (e.g. in several iterations).
    template<class Type>
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh>> weightedInterpolate
    (
        const tmp<GeometricField<Type, fvPatchField, volMesh>>&,
		const tmp<volScalarField>& tweight
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "fvcWeightedSurfaceInterpolate.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
