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

\*---------------------------------------------------------------------------*/

#include "fvcWeightedSurfaceInterpolate.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fvc
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh>>
weightedInterpolate
(
    const GeometricField<Type, fvPatchField, volMesh>& vtf,
	const volScalarField& weight
)
{
	Info << "weightedInterpolate(vtf=" << vtf.name() << ",weight=" << weight.name() << ")" << endl;
    return fvc::interpolate(vtf*weight) / fvc::interpolate(weight);
}


template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh>>
weightedInterpolate
(
    const GeometricField<Type, fvPatchField, volMesh>& vtf,
	const tmp<volScalarField>& tweight
)
{
	if(tweight.empty()){ // unweighted
		Info << "weightedInterpolate(vtf=" << vtf.name() << ",tweight=unweighted)" << endl;
		return fvc::interpolate(vtf);
	}else{ // weighted
		Info << "weightedInterpolate(vtf=" << vtf.name() << ",tweight=" << tweight->name() << ")" << endl;
		return fvc::weightedInterpolate(vtf,tweight());
	}
}

template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh>>
weightedInterpolate
(
    const tmp<GeometricField<Type, fvPatchField, volMesh>>& tvtf,
	const volScalarField& weight
)
{
	Info << "weightedInterpolate(tvtf=" << tvtf->name() << ",weight=" << weight.name() << ")" << endl;
	// Note: It is more efficient to re-use tvtf's memory,
	// hence we do not forward this function call to weightedInterpolate(vtf,weight).
    return fvc::interpolate(tvtf*weight) / fvc::interpolate(weight);
}

template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh>>
weightedInterpolate
(
    const tmp<GeometricField<Type, fvPatchField, volMesh>>& tvtf,
	const tmp<volScalarField>& tweight
)
{
	if(tweight.empty()){ // unweighted
		Info << "weightedInterpolate(tvtf=" << tvtf->name() << ",tweight=unweighted)" << endl;
		return fvc::interpolate(tvtf);
	}else{ // weighted
		Info << "weightedInterpolate(tvtf=" << tvtf->name() << ",tweight=" << tweight->name() << ")" << endl;
	    return fvc::weightedInterpolate(tvtf,tweight());
	}
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fvc

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
