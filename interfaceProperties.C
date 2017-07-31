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

\*---------------------------------------------------------------------------*/

#include "interfaceProperties.H"
#include "alphaContactAngleFvPatchScalarField.H"
#include "mathematicalConstants.H"
#include "surfaceInterpolate.H"
//#include "fvcDiv.H" // KVA: no longer needed
//#include "fvcGrad.H" // KVA: no longer needed
#include "fvcSnGrad.H"

// * * * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * //

const Foam::scalar Foam::interfaceProperties::convertToRad =
    Foam::constant::mathematical::pi/180.0;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// Correction for the boundary condition on the unit normal nHat on
// walls to produce the correct contact angle.

// The dynamic contact angle is calculated from the component of the
// velocity on the direction of the interface, parallel to the wall.

void Foam::interfaceProperties::correctContactAngle
(
    surfaceVectorField::Boundary& nHatb,
    const surfaceVectorField::Boundary& gradAlphaf
) const
{
    const fvMesh& mesh = alpha1_.mesh();
    const volScalarField::Boundary& abf = alpha1_.boundaryField();

    const fvBoundaryMesh& boundary = mesh.boundary();

    forAll(boundary, patchi)
    {
        if (isA<alphaContactAngleFvPatchScalarField>(abf[patchi]))
        {
            alphaContactAngleFvPatchScalarField& acap =
                const_cast<alphaContactAngleFvPatchScalarField&>
                (
                    refCast<const alphaContactAngleFvPatchScalarField>
                    (
                        abf[patchi]
                    )
                );

            fvsPatchVectorField& nHatp = nHatb[patchi];
            const scalarField theta
            (
                convertToRad*acap.theta(U_.boundaryField()[patchi], nHatp)
            );

            const vectorField nf
            (
                boundary[patchi].nf()
            );

            // Reset nHatp to correspond to the contact angle

            const scalarField a12(nHatp & nf);
            const scalarField b1(cos(theta));

            scalarField b2(nHatp.size());
            forAll(b2, facei)
            {
                b2[facei] = cos(acos(a12[facei]) - theta[facei]);
            }

            const scalarField det(1.0 - a12*a12);

            scalarField a((b1 - a12*b2)/det);
            scalarField b((b2 - a12*b1)/det);

            nHatp = a*nf + b*nHatp;
            nHatp /= (mag(nHatp) + deltaN_.value());

            acap.gradient() = (nf & nHatp)*mag(gradAlphaf[patchi]);
            acap.evaluate();
        }
    }
}


void Foam::interfaceProperties::calculateK()
{
	curvatureModel_->calculateK(K_, nHatf_); // KVA: Forward calculation to the curvatureModel class
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::interfaceProperties::interfaceProperties
(
    const volScalarField& alpha1,
    const volVectorField& U,
    const IOdictionary& dict
)
:
    transportPropertiesDict_(dict),
    cAlpha_
    (
        readScalar
        (
            alpha1.mesh().solverDict(alpha1.name()).lookup("cAlpha")
        )
    ),

    sigmaPtr_(surfaceTensionModel::New(dict, alpha1.mesh())),

	rho1_("rho", dimDensity,
		transportPropertiesDict_.subDict( wordList(transportPropertiesDict_.lookup("phases"))[0] )
	),
	rho2_("rho", dimDensity,
		transportPropertiesDict_.subDict( wordList(transportPropertiesDict_.lookup("phases"))[1] )
	),

    deltaN_
    (
        "deltaN",
        1e-8/pow(average(alpha1.mesh().V()), 1.0/3.0)
    ),

    alpha1_(alpha1),
    U_(U),

    nHatf_
    (
        IOobject
        (
            "nHatf",
            alpha1_.time().timeName(),
            alpha1_.mesh()
        ),
        alpha1_.mesh(),
        dimensionedScalar("nHatf", dimArea, 0.0)
    ),

    K_
    (
        IOobject
        (
            "interfaceProperties:K",
            alpha1_.time().timeName(),
            alpha1_.mesh()
        ),
        alpha1_.mesh(),
        dimensionedScalar("K", dimless/dimLength, 0.0)
    ),

	curvatureModel_ // KVA: Added curvatureModel_ to constructor
	(
		curvatureModel::New
		(
			"Kmodel",
			*this
		)
	) // KVA warning:
	// "curvatureModel_" SHOULD be constructed last, otherwise "this" is only a partially constructed object.
	// In the present implementation, it is *AT LEAST* crucial that "transportPropertiesDict_" is constructed,
	//  because curvatureModel will access it in its constructor.
{
	readSurfaceTensionModel(); // KVA
    calculateK(); // KVA warning: "curvatureModel_" MUST be constructed before this line.
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

Foam::tmp<Foam::volScalarField>
Foam::interfaceProperties::sigmaK() const
{
    return sigmaPtr_->sigma()*K_;
}

Foam::tmp<Foam::surfaceScalarField>
Foam::interfaceProperties::surfaceTensionForce() const
{
	if(densityWeighted_){ // KVA: Added a switch to select between the density-weighted surfaceTensionForce calculation and OF's default one.
		const volScalarField limitedAlpha1
		(
			min(max(alpha1_, scalar(0)), scalar(1))
		);

		const volScalarField rho(limitedAlpha1*rho1_ + (scalar(1) - limitedAlpha1)*rho2_);
		return fvc::interpolate(sigmaK()*rho)*fvc::snGrad(alpha1_) * 2/(rho1_+rho2_);
	}else{
        return fvc::interpolate(sigmaK())*fvc::snGrad(alpha1_);
	}
}


Foam::tmp<Foam::volScalarField>
Foam::interfaceProperties::nearInterface() const
{
    return pos0(alpha1_ - 0.01)*pos0(0.99 - alpha1_);
}


void Foam::interfaceProperties::correct()
{
    calculateK();
}


bool Foam::interfaceProperties::read()
{
    alpha1_.mesh().solverDict(alpha1_.name()).lookup("cAlpha") >> cAlpha_;
    sigmaPtr_->readDict(transportPropertiesDict_);

    transportPropertiesDict_.subDict( wordList(transportPropertiesDict_.lookup("phases"))[0] ).lookup("rho") >> rho1_; // KVA
    transportPropertiesDict_.subDict( wordList(transportPropertiesDict_.lookup("phases"))[1] ).lookup("rho") >> rho2_; // KVA

    bool result = readSurfaceTensionModel(); // KVA

    curvatureModel_->read(); // KVA

    return result && true;
}

bool Foam::interfaceProperties::readSurfaceTensionModel() // KVA
{
    if(transportPropertiesDict_.found("surfaceTensionForceModel")){
    	const dictionary& stfDict = transportPropertiesDict_.subDict("surfaceTensionForceModel");

        if(!stfDict.found("densityWeighted")){
    		WarningInFunction
    			<< "Keyword \"densityWeighted\" not found in surfaceTensionForceModel subdictionary." << nl
    			<< "    " << "Selecting default value " << false << " instead." << endl;
    	}
    	densityWeighted_ = stfDict.lookupOrDefault("densityWeighted",false);
    }else{
		WarningInFunction
			<< "Subdictionary surfaceTensionForceModel not found. Selecting the following default values instead:" << nl
			<< "    " << "densityWeighted = " << false << endl;
    	densityWeighted_ = false;
    }
    Info<< "Selecting surfaceTensionModel CSF(densityWeighted=" << densityWeighted_ << ")" << endl;

    return true;
}


// ************************************************************************* //
