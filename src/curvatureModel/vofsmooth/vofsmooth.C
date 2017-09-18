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

#include "regIOobject.H"

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

// * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * * //

void Foam::curvatureModels::vofsmooth::smoothCurvature(volScalarField& K)
{
	Info << "(vofsmooth.C) smoothCurvature(" << K.name() << ")" << endl;

	tmp<volScalarField> tsmoothed(
		curvatureSmoother_->smoothen(K)
	);

    if (tsmoothed.isTmp())
	{
    	// Use GeometricField::operator=(tmp) to transfer the data without copying.
		K = tsmoothed;
	}
	else
	{
		// The above gives a SegFault if tmp is of type CONST_REF, because GeometricField::operator=(tmp) attempts to steal the data from a const.
		// This bug was fixed in OF50.
		// Instead, take a detour by calling GeometricField::operator=(const&).
		const volScalarField& smoothed(tsmoothed());
		if (&smoothed != &K){ // Only copy if the new data is not what we already have. (Which happens with the "noneSmoother".)
		    K = smoothed; // Disadvantage: results in data copy, but we have to.
		}
	}

    // SegFault with the short version in case of "noneSmoother". See "updateAlphaSmooth()" for explanation.
    //	K = curvatureSmoother_->smoothen(K);

    Info << "(vofsmooth.C) Now calling tsmoothed.clear():" << endl;
    tsmoothed.clear();
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::curvatureModels::vofsmooth::vofsmooth
(
    const word& name,
    const interfaceProperties& interfaceProperties,
	const word& modelType
)
:
	curvatureModel(name,interfaceProperties,modelType),

	alphaSmooth_(
		IOobject
		(
				"smooth("+retrieve_alpha().name()+")", // new name
				retrieve_alpha().instance(), // same instance
				retrieve_alpha().mesh(), // same registry
				IOobject::NO_READ,
				IOobject::NO_WRITE,
				true // register
		),
		retrieve_alpha() // copy values as IC
	),

	alphaSmoother_(
//		Foam::vofsmooth::smootherKernel<scalar>::New("alphaSmoother", coeffsDict_.lookup("smoothAlpha"))
		Foam::vofsmooth::smootherKernel<scalar>::New("alphaSmoother", coeffsDict_.subDict("smoothAlpha"))
	),

	curvatureSmoother_(
//		Foam::vofsmooth::smootherKernel<scalar>::New("curvatureSmoother", coeffsDict_.lookup("smoothCurvature"))
		Foam::vofsmooth::smootherKernel<scalar>::New("curvatureSmoother", coeffsDict_.subDict("smoothCurvature"))
	)
{
    read(); // Note: parent's read is called twice, but that doesn't really matter.
    updateAlphaSmooth(); // Init alphaSmooth_ as the smoothed alpha field, such that other classes can immediately use it if they so desire.
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void Foam::curvatureModels::vofsmooth::updateAlphaSmooth()
{
	Info << "(vofsmooth.C) updateAlphaSmooth()" << endl;

	tmp<volScalarField> tsmoothed(
		alphaSmoother_->smoothen(retrieve_alpha())
	);

    if (tsmoothed.isTmp())
	{
    	// Use GeometricField::operator=(tmp) to transfer the data without copying.
		alphaSmooth_ = tsmoothed;
	}
	else
	{
		// The above gives a SegFault if tmp is of type CONST_REF, because GeometricField::operator=(tmp) attempts to steal the data from a const.
		// This bug was fixed in OF50.
		// Instead, take a detour by calling GeometricField::operator=(const&).
		const volScalarField& smoothed(tsmoothed());
	    alphaSmooth_ = smoothed; // Disadvantage: results in data copy, but we have to.
	}

    // SegFault if this (shorter) method is used. This is fixed in OF50.
//    alphaSmooth_ = alphaSmoother_->smoothen(retrieve_alpha()); // Segfault at some future point in the code if alphaSmoother_ returns a tmp of type CONST_REF.
//    alphaSmoother_->smoothen(retrieve_alpha()); // No Segfault without GeometricField::operator=, hence that operator causes it.
}

void Foam::curvatureModels::vofsmooth::calculateK(volScalarField& K, surfaceScalarField& nHatf)
{
	Info << "(vofsmooth.C) calculateK(" << K.name() << "," << nHatf.name() << ")" << endl;

	const volScalarField& alpha1 = retrieve_alpha();
	const dimensionedScalar& deltaN = retrieve_deltaN();

	const fvMesh& mesh = alpha1.mesh();
	const surfaceVectorField& Sf = mesh.Sf();

    Info << alpha1.db().names<volScalarField>() << endl;

	// Define a smoothed version of the alpha field. Initialise it as a copy.
//    volScalarField alpha1_smooth = alpha1;
//    volScalarField alpha1_smooth(
//		IOobject
//		(
//				"smooth("+alpha1.name()+")", // new name
//				alpha1.instance(), // same instance
//				alpha1.mesh(), // same registry
//                IOobject::NO_READ,
//                IOobject::NO_WRITE,
//				true // register
//		),
//		alpha1 // copy values as IC
//    );
//    alphaSmooth_ = alpha1; // copy values as IC

//    Info << alpha1.db().names() << endl;

//    alphaSmooth_ = alphaSmoother_->smoothen(alpha1);
    updateAlphaSmooth();
//    smoothen(alpha1_smooth);

	// Cell gradient of alpha, based on the _smoothed_ alpha field.
	const volVectorField gradAlpha(fvc::grad(alphaSmooth_, "nHat"));

//	Info << "gradAlpha was computed." << endl;

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

//	Info << "Now smoothing curvature." << endl;
	smoothCurvature(K);
//	K = curvatureSmoother_->smoothen(K);
//	Info << "Now smoothing curvature. finished" << endl;

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

    // TODO: Construct new alphaSmoother_ and curvatureSmoother_?

    return result && true;
}


// ************************************************************************* //
