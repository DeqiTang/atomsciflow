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

/// @file src/atomsciflow/vasp/incar_grouping.cpp
/// @author: DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Sun 20 Mar 2022 02:27:35 PM CST

#include "atomsciflow/vasp/incar_grouping.h"

namespace atomsciflow {

IncarGrouping::IncarGrouping() {
    this->init();
}

void IncarGrouping::init() {
    start_incharge.insert("NWRITE");
    start_incharge.insert("PREC");
    start_incharge.insert("ISTART");
    start_incharge.insert("ICHARG");
    start_incharge.insert("ISPIN");
    start_incharge.insert("LNONCOLLINEAR");
    start_incharge.insert("LSORBIT");
    start_incharge.insert("INIWAV");
    start_incharge.insert("LASPH");
    start_incharge.insert("METAGGA");

    xc_incharge.insert("GGA");
    xc_incharge.insert("LEXCH");
    xc_incharge.insert("VOSKOWN");
    xc_incharge.insert("LHFCALC");
    xc_incharge.insert("HFSCREEN");
    xc_incharge.insert("LHFONE");
    xc_incharge.insert("AEXX");

    write_incharge.insert("LWAVE");
    write_incharge.insert("LDOWNSAMPLE");
    write_incharge.insert("LCHARG");
    write_incharge.insert("LVTOT");
    write_incharge.insert("LVHAR");
    write_incharge.insert("LELF");
    write_incharge.insert("LORBIT");

    orbitalmagnet_incharge.insert("ORBITALMAG");
    orbitalmagnet_incharge.insert("LCHIMAG");
    orbitalmagnet_incharge.insert("DQ");
    orbitalmagnet_incharge.insert("LLRAUG");

    lr_incharge.insert("LEPSILON");
    lr_incharge.insert("LRPA");
    lr_incharge.insert("LNABLA");
    lr_incharge.insert("LVEL");
    lr_incharge.insert("LINTERFAST");
    lr_incharge.insert("KINTER");
    lr_incharge.insert("OMEGAMAX");
    lr_incharge.insert("DEG_THRESHOLD");
    lr_incharge.insert("RTIME");
    lr_incharge.insert("DFIELD");

    ions_incharge.insert("EDIFFG");
    ions_incharge.insert("NSW");
    ions_incharge.insert("NBLOCK");
    ions_incharge.insert("IBRION");
    ions_incharge.insert("NFREE");
    ions_incharge.insert("ISIF");
    ions_incharge.insert("IWAVPR");
    ions_incharge.insert("ISYM");
    ions_incharge.insert("LCORR");
    ions_incharge.insert("POTIM");
    ions_incharge.insert("TEIN");
    ions_incharge.insert("TEBEG");
    ions_incharge.insert("SMASS");
    ions_incharge.insert("SCALEE");
    ions_incharge.insert("NPACO");
    ions_incharge.insert("PSTRESS");
    ions_incharge.insert("MDALGO");

    intraband_incharge.insert("WEIMIN");
    intraband_incharge.insert("EBREAK");
    intraband_incharge.insert("DEPER");
    intraband_incharge.insert("TIME");

    electrons_incharge.insert("ENCUT");
    electrons_incharge.insert("ENINI");
    electrons_incharge.insert("ENAUG");
    electrons_incharge.insert("NELM");
    electrons_incharge.insert("EDIFF");
    electrons_incharge.insert("LREAL");
    electrons_incharge.insert("NLSPLINE");
    electrons_incharge.insert("LCOMPAT");
    electrons_incharge.insert("GGA_COMPAT");
    electrons_incharge.insert("LMAXPAW");
    electrons_incharge.insert("LMAXMIX");
    electrons_incharge.insert("VOSKOWN");
    electrons_incharge.insert("ROPT");
    electrons_incharge.insert("IALGO");
    electrons_incharge.insert("LDIAG");
    electrons_incharge.insert("LSUBROT");
    electrons_incharge.insert("TURBO");
    electrons_incharge.insert("IRESTART");
    electrons_incharge.insert("NREBOOT");
    electrons_incharge.insert("NMIN");
    electrons_incharge.insert("EREF");
    electrons_incharge.insert("IMIX");
    electrons_incharge.insert("EMIN");
    electrons_incharge.insert("EMAX");
    electrons_incharge.insert("EFERMI");
    electrons_incharge.insert("ISMEAR");
    electrons_incharge.insert("SIGMA");

    dipolecorrection_incharge.insert("LDIPOL");

    neb_incharge.insert("IOPT");
    neb_incharge.insert("SPRING");
    neb_incharge.insert("IMAGES");
    neb_incharge.insert("LCIMB");
    neb_incharge.insert("ICHAIN");
    neb_incharge.insert("LNEBCELL");
    neb_incharge.insert("MAXMOVE");
    neb_incharge.insert("LGLOBAL");
    neb_incharge.insert("LAUTOSCALE");
    neb_incharge.insert("INVCURV");
    neb_incharge.insert("LLINEOPT");
    neb_incharge.insert("FDSTEP");
}

} // namespace atomsciflow
