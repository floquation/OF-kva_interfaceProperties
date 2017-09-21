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

#include "cskSmoother.H"

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
Foam::smoothers::cskSmoother<Type>::cskSmoother
(
	const word& name,
	const dictionary& dict
//	Istream& is
)
:
	smootherKernel<Type>(name),
	numSmoothingIterations_(dict.lookupOrDefault<label>("numIts",0)),
	Csk_(dict.lookupOrDefault<scalar>("Csk",0.5)),
	smoother_(
		Foam::smoothers::smootherKernel<Type>::New(name,dict.subDict("smoother"))
	)
{
	if (numSmoothingIterations_ <= 0)
	{
		int numSmoothingIterationsDefault = 2;
		WarningInFunction
			<< "Specified numIts = " << numSmoothingIterations_ << " in " << dict.name() << "." << nl
			<< "    " << "This value must be positive. Assuming the default value " << numSmoothingIterationsDefault << " instead." << endl;
		numSmoothingIterations_ = numSmoothingIterationsDefault;
	}

	scalar CskDefault = 0.5;
	if (Csk_ < 0 || Csk_ > 1){
		WarningInFunction
			<< "Specified Csk = " << Csk_ << " in " << dict.name() << "." << nl
			<< "    " << "This value must be between 0 and 1. Assuming the default value " << CskDefault << " instead." << endl;
		Csk_ = CskDefault;
	}
	if (!dict.found("Csk"))
	{
		WarningInFunction
			<< "Entry Csk was not found in " << dict.name() << "." << nl
			<< "    " << "Assuming the default value " << CskDefault << " instead." << endl;
		Csk_ = CskDefault;
	}
}

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>>
Foam::smoothers::cskSmoother<Type>::smoothen(const GeometricField<Type, fvPatchField, volMesh>& fld) const{
	Info << "(cskSmoother) Smoothing " << fld.name() << "." << endl;

//	Info << "fld.count() = " << fld.count() << endl;

	// Declare return; initialise as pointer to not-smoothed field. After the first iteration, it will be replaced by a new data field.
	tmp<GeometricField<Type, fvPatchField, volMesh>> fldPrev(fld);
//	if(fldSmooth.isTmp()) Info << "fldSmooth->count() = " << fldSmooth->count() << endl;

	// Smoothen field
    for(int iter = 0; iter < numSmoothingIterations_; iter++)
    {
//    	Info << "(cskSmoother) it" << iter << endl;

    	// Put the previous field in a tmp (pointer copy: operator++) to pass to smoother_->smoothen().
//    	tmp<GeometricField<Type, fvPatchField, volMesh>> tfld(fldPrev);

//    	Info << "(cskSmoother) &tfld == &fldPrev? " << ( &tfld() == &fldPrev() ) << endl; // Yes! So no data being copied unnecessarily.

//    	if(!fldPrev.valid()){
//    		Info << "fldPrev is invalid." << endl;
//    	}else if(fldPrev.isTmp()){
//    		Info << "fldPrev->count() = " << fldPrev->count() << endl;
//    	}else{
//    		Info << "fldPrev is not TMP." << endl;
//    	}
//    	if(!tfld.valid()){
//    		Info << "tfld is invalid." << endl;
//    	}else if(tfld.isTmp()){
//    		Info << "tfld->count() = " << tfld->count() << endl;
//    	}else{
//    		Info << "tfld is not TMP." << endl;
//    	}

//    	Info << "Calculating fldSmooth." << endl;
    	// Calculate the smoothed field. Automatically calls operator-- on tfld.
    	tmp<GeometricField<Type, fvPatchField, volMesh>> fldSmooth(
    		smoother_->smoothen( fldPrev() )
		);

//    	if(!fldPrev.valid()){
//    		Info << "fldPrev is invalid." << endl;
//    	}else if(fldPrev.isTmp()){
//    		Info << "fldPrev->count() = " << fldPrev->count() << endl;
//    	}else{
//    		Info << "fldPrev is not TMP." << endl;
//    	}
//    	if(!tfld.valid()){
//    		Info << "tfld is invalid." << endl;
//    	}else if(tfld.isTmp()){
//    		Info << "tfld->count() = " << tfld->count() << endl;
//    	}else{
//    		Info << "tfld is not TMP." << endl;
//    	}
//    	if(!fldSmooth.valid()){
//    		Info << "fldSmooth is invalid." << endl;
//    	}else if(fldSmooth.isTmp()){
//    		Info << "fldSmooth->count() = " << fldSmooth->count() << endl;
//    	}else{
//    		Info << "fldSmooth is not TMP." << endl;
//    	}
//
//    	const GeometricField<Type, fvPatchField, volMesh>* ref = &fldPrev();
//    	Info << "(cskSmoother) ref == &fldPrev? " << ( ref == &fldPrev() ) << endl;
//
//    	Info << "Test operator* on tmp." << endl;
//		tmp<GeometricField<Type, fvPatchField, volMesh>> test = Csk_ * fldPrev;
//    	Info << "(cskSmoother) ref == &test? " << ( ref == &test() ) << endl; // if fldPrev is CONST_REF, then new field is created. If fldPrev is a tmp, same field is used; ownership transfered.
//		if(!fldPrev.valid()){
//			Info << "fldPrev is invalid." << endl;
//		}else{
//			if(!fldPrev.isTmp()){
//				Info << "fldPrev is not TMP." << endl;
//			}
//			Info << "(cskSmoother) &test2 == &fldPrev? " << ( &test() == &fldPrev() ) << endl;
//		}

//    	Info << "Calculating fldNext." << endl;
    	// Calculate fldNext and immediately overwrite fldPrev.
    	// The arithmetics creates a new data field.
    	fldPrev = Csk_*fldSmooth + (1-Csk_)*fldPrev;

//    	if(!fldPrev.valid()){
//    		Info << "fldPrev is invalid." << endl;
//    	}else if(fldPrev.isTmp()){
//    		Info << "fldPrev->count() = " << fldPrev->count() << endl;
//    	}else{
//    		Info << "fldPrev is not TMP." << endl;
//    	}
//    	if(!tfld.valid()){
//    		Info << "tfld is invalid." << endl;
//    	}else if(tfld.isTmp()){
//    		Info << "tfld->count() = " << tfld->count() << endl;
//    	}else{
//    		Info << "tfld is not TMP." << endl;
//    	}
//    	if(!fldSmooth.valid()){
//    		Info << "fldSmooth is invalid." << endl;
//    	}else if(fldSmooth.isTmp()){
//    		Info << "fldSmooth->count() = " << fldSmooth->count() << endl;
//    	}else{
//    		Info << "fldSmooth is not TMP." << endl;
//    	}

//    	if(fldSmooth.isTmp()) Info << "fldSmooth->count() = " << fldSmooth->count() << endl;
//    	if(tfld.isTmp()) Info << "tfld->count() = " << tfld->count() << endl;

//    	Info << "tfld=tmp? " << tfld.isTmp() << " and empty? " << tfld.empty() << endl;
//    	tfld.clear();
//    	Info << "(clear) tfld=tmp? " << tfld.isTmp() << " and empty? " << tfld.empty() << endl;
//    	Info << "fldSmooth=tmp? " << fldSmooth.isTmp() << " and empty? " << fldSmooth.empty() << endl;
    }

    return fldPrev;
}

// ************************************************************************* //

}

// ************************************************************************* //
