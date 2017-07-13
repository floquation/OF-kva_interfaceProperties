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

\*---------------------------------------------------------------------------*/

#include "vofsmooth.H"
#include "addToRunTimeSelectionTable.H"

#include "surfaceInterpolate.H"
#include "fvcDiv.H"
#include "fvcGrad.H"
#include "fvcSnGrad.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace curvatureModels
{
    defineTypeNameAndDebug(vofsmooth, 0);
    addToRunTimeSelectionTable
    (
    	curvatureModel,
        vofsmooth,
        dictionary
    );
}
}

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

void Foam::curvatureModels::vofsmooth::smoothen
(
    volScalarField& smooth_func
) const
{
    const fvMesh& mesh = smooth_func.mesh();
    const surfaceVectorField& Sf = mesh.Sf();

    const labelList& own = mesh.faceOwner();
    const labelList& nei = mesh.faceNeighbour();

    for(int iter = 0; iter < numSmoothingIterations_; iter++)
    {
    	scalarField smooth_cal(mesh.nCells(),scalar(0));

    	scalarField sum_area(mesh.nCells(),scalar(0));

		surfaceScalarField smoothF = fvc::interpolate(smooth_func);

		for(int facei = 0; facei < nei.size(); facei++) //KVA note: should be own???
		{
			smooth_cal[own[facei]] += smoothF[facei]*mag(Sf[facei]);
			sum_area[own[facei]] += mag(Sf[facei]);
		}

		forAll(nei,facei)
		{
			smooth_cal[nei[facei]] += smoothF[facei]*mag(Sf[facei]);
			sum_area[nei[facei]] += mag(Sf[facei]);
		}

		forAll(mesh.boundary(), patchi)
		{
			const unallocLabelList& pFaceCells = mesh.boundary()[patchi].faceCells();

			const fvsPatchScalarField& pssf = smoothF.boundaryField()[patchi];

			forAll(mesh.boundary()[patchi], facei)
			{
			   smooth_cal[pFaceCells[facei]] += pssf[facei]*mag(Sf[facei]);
			   sum_area[pFaceCells[facei]] += mag(Sf[facei]);
			}
		}

		forAll(mesh.cells(),celli)
		{
			smooth_func[celli] = smooth_cal[celli]/sum_area[celli];
		}

		smooth_func.correctBoundaryConditions();

    }
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::curvatureModels::vofsmooth::vofsmooth
(
    const word& name,
    const interfaceProperties& interfaceProperties,
	const word& modelType
)
:
	curvatureModel(name,interfaceProperties,modelType)
{
    read(); // Note: parent's read is called twice, but that doesn't really matter.
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void Foam::curvatureModels::vofsmooth::calculateK(volScalarField& K, surfaceScalarField& nHatf) const
{
	const volScalarField& alpha1 = retrieve_alpha();
	const dimensionedScalar& deltaN = retrieve_deltaN();

	const fvMesh& mesh = alpha1.mesh();
	const surfaceVectorField& Sf = mesh.Sf();

	// Define a smoothed version of the alpha field. Initialise it as a copy.
    volScalarField alpha1_smooth = alpha1;
    smoothen(alpha1_smooth);

	// Cell gradient of alpha, based on the _smoothed_ alpha field.
	const volVectorField gradAlpha(fvc::grad(alpha1_smooth, "nHat"));

	// Interpolated face-gradient of alpha
	surfaceVectorField gradAlphaf(fvc::interpolate(gradAlpha));

	//gradAlphaf -=
	//    (mesh.Sf()/mesh.magSf())
	//   *(fvc::snGrad(alpha1_) - (mesh.Sf() & gradAlphaf)/mesh.magSf());

	// Face unit interface normal
	surfaceVectorField nHatfv(gradAlphaf/(mag(gradAlphaf) + deltaN));
	// surfaceVectorField nHatfv
	// (
	//     (gradAlphaf + deltaN_*vector(0, 0, 1)
	//    *sign(gradAlphaf.component(vector::Z)))/(mag(gradAlphaf) + deltaN_)
	// );
	correctContactAngle(nHatfv.boundaryFieldRef(), gradAlphaf.boundaryField());

	// Face unit interface normal flux
	nHatf = nHatfv & Sf;

	// Simple expression for curvature
	K = -fvc::div(nHatf);

	// Complex expression for curvature.
	// Correction is formally zero but numerically non-zero.
	/*
	volVectorField nHat(gradAlpha/(mag(gradAlpha) + deltaN_));
	forAll(nHat.boundaryField(), patchi)
	{
		nHat.boundaryField()[patchi] = nHatfv.boundaryField()[patchi];
	}

	K_ = -fvc::div(nHatf_) + (nHat & fvc::grad(nHatfv) & nHat);
	*/

}



bool Foam::curvatureModels::vofsmooth::read()
{
//	Info << "kva: curvatureModels::vofsmooth::read();" << endl;

	// Always call the parent first
    bool result = curvatureModel::read(this->type());

    // Read model parameters
//    numSmoothingIterations_ = readInt(coeffsDict_.lookup("numSmoothingIterations"));
    int numSmoothingIterationsDefault = 2;
    numSmoothingIterations_ = coeffsDict_.lookupOrDefault<int>("numSmoothingIterations",numSmoothingIterationsDefault);

    if(!coeffsDict_.found("numSmoothingIterations")){
		WarningInFunction
			<< "Keyword \"numSmoothingIterations\" not found in vofsmoothCoeffs subdictionary." << nl
			<< "    " << "Selecting default value " << numSmoothingIterationsDefault << " instead." << endl;
	}

    if (numSmoothingIterations_ <= 0)
    {
    	if (numSmoothingIterations_ == 0){
			WarningInFunction
				<< "Specified numSmoothingIterations = 0." << nl
				<< "    " << "Therefore \"vofsmooth\" will operate in \"normal\" mode without a smoother." << nl
				<< "    " << "The recommended value is 2." << endl;
    	}else{
			WarningInFunction
				<< "Specified numSmoothingIterations = " << numSmoothingIterations_ << "." << nl
				<< "    " << "This value must be non-negative. Assuming the default value " << numSmoothingIterationsDefault << " instead." << endl;
			numSmoothingIterations_ = numSmoothingIterationsDefault;
    	}
    }

    return result && true;
}


// ************************************************************************* //
