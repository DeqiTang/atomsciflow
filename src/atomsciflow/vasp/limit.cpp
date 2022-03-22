/************************************************************************
MIT License

Copyright (c) 2021 Deqi Tang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
************************************************************************/

/// @file src/atomsciflow/vasp/limit.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Sun 20 Mar 2022 01:58:20 PM CST

#include "atomsciflow/vasp/limit.h"

namespace atomsciflow {

VaspLimit::VaspLimit() {
    this->init();
}

void VaspLimit::init() {
    static_allowed.insert("NWRITE");
    static_allowed.insert("PREC");
    static_allowed.insert("NCORE");
    static_allowed.insert("NPAR");
    static_allowed.insert("KPAR");
    static_allowed.insert("LPLANE");
    static_allowed.insert("NSIM");
    static_allowed.insert("ENCUT");
    static_allowed.insert("EDIFF");
    static_allowed.insert("NELM");
    static_allowed.insert("NFREE");
    static_allowed.insert("ISMEAR");
    static_allowed.insert("SIGMA");
    static_allowed.insert("IVDW");
    static_allowed.insert("LORBIT");
    static_allowed.insert("LOPTICS");
    static_allowed.insert("CSHIFT");
    static_allowed.insert("NEDOS");
    static_allowed.insert("ISPIN");
    static_allowed.insert("MAGMOM" );
    static_allowed.insert("LNONCOLLINEAR");
    static_allowed.insert("LSORBIT");
    static_allowed.insert("SAXIS");
    static_allowed.insert("LMAXMIX");
    static_allowed.insert("LHFCALC");
    static_allowed.insert("HFSCREEN");
    static_allowed.insert("AEXX");
    static_allowed.insert("LSUBROT");
    static_allowed.insert("ALGO");
    static_allowed.insert("IALGO");
    static_allowed.insert("ADDGRID");
    static_allowed.insert("ISYM");
    static_allowed.insert("LREAL");
    static_allowed.insert("LWAVE");
    static_allowed.insert("LCHARG");
    static_allowed.insert("LELF");
    static_allowed.insert("PSTRESS");
    static_allowed.insert("AMIX");
    static_allowed.insert("BMIX");
    static_allowed.insert("LAECHG");
    static_allowed.insert("SYMPREC");
    static_allowed.insert("NELECT");
    static_allowed.insert("KSPACING");
    static_allowed.insert("LPARD");
    static_allowed.insert("LSEPK");
    static_allowed.insert("LSEPB");
    static_allowed.insert("NBMOD");
    static_allowed.insert("EINT");
    static_allowed.insert("LVHAR");

    opt_allowed.insert("NWRITE");
    opt_allowed.insert("PREC");
    opt_allowed.insert("NCORE");
    opt_allowed.insert("NPAR");
    opt_allowed.insert("KPAR");
    opt_allowed.insert("LPLANE");
    opt_allowed.insert("NSIM");
    opt_allowed.insert("ENCUT");
    opt_allowed.insert("EDIFF");
    opt_allowed.insert("NELM");
    opt_allowed.insert("NFREE");
    opt_allowed.insert("ISMEAR");
    opt_allowed.insert("SIGMA");
    opt_allowed.insert("IVDW");
    opt_allowed.insert("ISPIN");
    opt_allowed.insert("MAGMOM");
    opt_allowed.insert("LNONCOLLINEAR");
    opt_allowed.insert("ALGO");
    opt_allowed.insert("IALGO");
    opt_allowed.insert("ADDGRID");
    opt_allowed.insert("ISYM");
    opt_allowed.insert("LREAL");
    opt_allowed.insert("LWAVE");
    opt_allowed.insert("LCHARG");
    opt_allowed.insert("LAECHG");
    opt_allowed.insert("IBRION");
    opt_allowed.insert("ISIF");
    opt_allowed.insert("NSW");
    opt_allowed.insert("POTIM");
    opt_allowed.insert("EDIFFG");
    opt_allowed.insert("PSTRESS");
    opt_allowed.insert("AMIX");
    opt_allowed.insert("BMIX");
    opt_allowed.insert("SYMPREC");
    opt_allowed.insert("NELECT");
    opt_allowed.insert("KSPACING");
    opt_allowed.insert("LVHAR");

    phonopy_allowed.insert("NWRITE");
    phonopy_allowed.insert("PREC");
    phonopy_allowed.insert("NCORE");
    phonopy_allowed.insert("NPAR");
    phonopy_allowed.insert("KPAR");
    phonopy_allowed.insert("LPLANE");
    phonopy_allowed.insert("NSIM");
    phonopy_allowed.insert("ENCUT");
    phonopy_allowed.insert("EDIFF");
    phonopy_allowed.insert("NELM");
    phonopy_allowed.insert("NFREE");
    phonopy_allowed.insert("ISMEAR");
    phonopy_allowed.insert("SIGMA");
    phonopy_allowed.insert("IVDW");
    phonopy_allowed.insert("ISPIN");
    phonopy_allowed.insert("MAGMOM");
    phonopy_allowed.insert("LNONCOLLINEAR");
    phonopy_allowed.insert("ALGO");
    phonopy_allowed.insert("IALGO");
    phonopy_allowed.insert("ADDGRID");
    phonopy_allowed.insert("LAECHG");
    phonopy_allowed.insert("ISYM");
    phonopy_allowed.insert("LREAL");
    phonopy_allowed.insert("LWAVE");
    phonopy_allowed.insert("LCHARG");
    phonopy_allowed.insert("AMIX");
    phonopy_allowed.insert("BMIX");
    phonopy_allowed.insert("SYMPREC");
    phonopy_allowed.insert("NELECT");
    phonopy_allowed.insert("KSPACING");

    neb_allowed.insert("NWRITE");
    neb_allowed.insert("PREC");
    neb_allowed.insert("NCORE");
    neb_allowed.insert("NPAR");
    neb_allowed.insert("KPAR");
    neb_allowed.insert("LPLANE");
    neb_allowed.insert("NSIM");
    neb_allowed.insert("ENCUT");
    neb_allowed.insert("EDIFF");
    neb_allowed.insert("NELM");
    neb_allowed.insert("NFREE");
    neb_allowed.insert("ISMEAR");
    neb_allowed.insert("SIGMA");
    neb_allowed.insert("IVDW");
    neb_allowed.insert("ISPIN");
    neb_allowed.insert("MAGMOM");
    neb_allowed.insert("LNONCOLLINEAR");
    neb_allowed.insert("ALGO");
    neb_allowed.insert("IALGO");
    neb_allowed.insert("ADDGRID");
    neb_allowed.insert("ISYM");
    neb_allowed.insert("LREAL");
    neb_allowed.insert("LWAVE");
    neb_allowed.insert("LCHARG");
    neb_allowed.insert("IBRION");
    neb_allowed.insert("ISIF");
    neb_allowed.insert("NSW");
    neb_allowed.insert("POTIM");
    neb_allowed.insert("EDIFFG");
    neb_allowed.insert("IOPT");
    neb_allowed.insert("LCLIMB");
    neb_allowed.insert("SPRING");
    neb_allowed.insert("IMAGES");
    neb_allowed.insert("LNEBCELL");
    neb_allowed.insert("MAXMOVE");
    neb_allowed.insert("LGLOBAL");
    neb_allowed.insert("LAUTOSCALE");
    neb_allowed.insert("INVCURV");
    neb_allowed.insert("LLINEOPT");
    neb_allowed.insert("FDSTEP");
    neb_allowed.insert("AMIX");
    neb_allowed.insert("BMIX");
    neb_allowed.insert("SYMPREC");
    neb_allowed.insert("NELECT");
    neb_allowed.insert("KSPACING");

    md_allowed.insert("NWRITE");
    md_allowed.insert("PREC");
    md_allowed.insert("NCORE");
    md_allowed.insert("NPAR");
    md_allowed.insert("KPAR");
    md_allowed.insert("LPLANE");
    md_allowed.insert("NSIM");
    md_allowed.insert("ENCUT");
    md_allowed.insert("EDIFF");
    md_allowed.insert("NELM");
    md_allowed.insert("NFREE");
    md_allowed.insert("ISMEAR");
    md_allowed.insert("SIGMA");
    md_allowed.insert("IVDW");
    md_allowed.insert("ISPIN");
    md_allowed.insert("MAGMOM");
    md_allowed.insert("LNONCOLLINEAR");
    md_allowed.insert("ALGO");
    md_allowed.insert("IALGO");
    md_allowed.insert("ADDGRID");
    md_allowed.insert("ISYM");
    md_allowed.insert("LREAL");
    md_allowed.insert("LWAVE");
    md_allowed.insert("LCHARG");
    md_allowed.insert("LAECHG");
    md_allowed.insert("IBRION");
    md_allowed.insert("ISIF");
    md_allowed.insert("NSW");
    md_allowed.insert("POTIM");
    md_allowed.insert("EDIFFG");
    md_allowed.insert("PSTRESS");
    md_allowed.insert("AMIX");
    md_allowed.insert("BMIX");
    md_allowed.insert("SYMPREC");
    md_allowed.insert("NELECT");
    md_allowed.insert("KSPACING");
    md_allowed.insert("MDALGO");
    md_allowed.insert("SMASS");
    md_allowed.insert("ANDERSON_PROB");
    md_allowed.insert("TEBEG");
    md_allowed.insert("TEEND");

    phonon_allowed.insert("NWRITE");
    phonon_allowed.insert("PREC");
    phonon_allowed.insert("NCORE");
    phonon_allowed.insert("NPAR");
    phonon_allowed.insert("KPAR");
    phonon_allowed.insert("LPLANE");
    phonon_allowed.insert("NSIM");
    phonon_allowed.insert("ENCUT");
    phonon_allowed.insert("EDIFF");
    phonon_allowed.insert("NELM");
    phonon_allowed.insert("NFREE");
    phonon_allowed.insert("ISMEAR");
    phonon_allowed.insert("SIGMA");
    phonon_allowed.insert("IVDW");
    phonon_allowed.insert("ISPIN");
    phonon_allowed.insert("MAGMOM");
    phonon_allowed.insert("LNONCOLLINEAR");
    phonon_allowed.insert("ALGO");
    phonon_allowed.insert("IALGO");
    phonon_allowed.insert("ADDGRID");
    phonon_allowed.insert("ISYM");
    phonon_allowed.insert("LREAL");
    phonon_allowed.insert("LWAVE");
    phonon_allowed.insert("LCHARG");
    phonon_allowed.insert("LAECHG");
    phonon_allowed.insert("IBRION");
    phonon_allowed.insert("ISIF");
    phonon_allowed.insert("NSW");
    phonon_allowed.insert("POTIM");
    phonon_allowed.insert("EDIFFG");
    phonon_allowed.insert("AMIX");
    phonon_allowed.insert("BMIX");
    phonon_allowed.insert("SYMPREC");
    phonon_allowed.insert("NELECT");
    phonon_allowed.insert("KSPACING");
}

} // namespace atomsciflow
