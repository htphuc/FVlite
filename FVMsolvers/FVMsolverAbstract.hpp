// FVMsolverAbstract.hpp
//
// Abstract finite volume solver.
// FVM stands for 'Finite Volume Method'

#ifndef FVMABSTRACT_HPP
#define FVMABSTRACT_HPP

#include <string>
#include <iostream>
#include <cstdlib>
#include <libconfig.h++>

#include "Grid/Grid.hpp"
#include "FluxSolvers/FluxSolvers.hpp"
#include "Sources/Sources.hpp"
#include "CutCellManagers/CutCellManagers.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class FVMsolver{

protected:

    Grid*       pGrid;
    FluxSolver* pFlux;
    CutCellManager* pCutCell;

public:

    FVMsolver(){}
    virtual ~FVMsolver();

    void init( Grid* pGrid, Setting& cfg);
    virtual void exec( char dim, double dt) = 0;
    virtual void newTimeStep();
};

// Declare object factory

ObjectFactory<FVMsolver> FVMsolverFactory;

// Function definitions

void FVMsolver::init( Grid* pGrid, Setting& cfg){
    (*this).pGrid = pGrid;

    string fluxType = cfg.lookup("scheme");
    pFlux = FluxSolverFactory.create(fluxType);
    pFlux->init(pGrid,cfg);

    string fvmType, cutCellType;
    fvmType = cfg.lookup("type").c_str();
    if( fvmType == "Std") cutCellType = "None";
    if( fvmType == "CutCell") cutCellType = "Std";
    pCutCell = CutCellManagerFactory.create(cutCellType);
    pCutCell->init(pGrid,pFlux);
    return;
}

FVMsolver::~FVMsolver(){
    delete pFlux;
    delete pCutCell;
}

void FVMsolver::newTimeStep(){
    pCutCell->newTimeStepSetup();
    return;
}

}// Namespace closure
#endif /* FVMABSTRACT_HPP */
