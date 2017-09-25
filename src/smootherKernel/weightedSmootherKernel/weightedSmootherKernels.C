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

#include "weightedSmootherKernel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// We cannot use "makeSmootherKernel" here, because weightedSmoothedKernel is also
// an abstract base class that extends smootherKernel.
// However, we still need to declare all its Types.
// Therefore, only call the "defineNamedTemplateTypeNameAndDebug", which
// does not add its constructor to the lookup table.
defineNamedTemplateTypeNameAndDebug(Foam::smoothers::weightedSmootherKernel<Foam::scalar>, 0);
defineNamedTemplateTypeNameAndDebug(Foam::smoothers::weightedSmootherKernel<Foam::vector>, 0);
defineNamedTemplateTypeNameAndDebug(Foam::smoothers::weightedSmootherKernel<Foam::sphericalTensor>, 0);
defineNamedTemplateTypeNameAndDebug(Foam::smoothers::weightedSmootherKernel<Foam::symmTensor>, 0);
defineNamedTemplateTypeNameAndDebug(Foam::smoothers::weightedSmootherKernel<Foam::tensor>, 0);

//makeSmootherKernel(weightedSmootherKernel)

// ************************************************************************* //
