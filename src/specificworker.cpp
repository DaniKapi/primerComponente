/*
 *    Copyright (C) 2015 by YOUR NAME HERE
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * 	COMPONENT
 * 
 */
#include "specificworker.h"

/**
* \brief Default constructor
*/
SpecificWorker::SpecificWorker(MapPrx& mprx) : GenericWorker(mprx)
{
  this->currentMark = 0;
  this->inner= new InnerModel("/home/salabeta/robocomp/files/innermodel/simpleworld.xml");
  marcas.setInner(inner);
}

float SpecificWorker::calcularDist(float x,float y){
  float suma=(x*x)+(y*y);
  return sqrt(suma);
}

bool SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
  timer.start(Period);
  return true;
}


/***************************** USANDO APRILTAGS ********************************/

void SpecificWorker::copiar(tag t, ListaMarca::Marca& y)
{

  y.id=t.id;
  y.tx=t.tx;
  y.ty=t.ty;
  y.tz=t.tz;
  y.rx=t.rx;
  y.ry=t.ry;
  y.rz=t.rz;

}

void SpecificWorker::newAprilTag(const tagsList& tags)
{
  for (auto t: tags){
    ListaMarca::Marca x;
    this->copiar(t,x);
    marcas.add(x);
   
  }
  
}

/********************************************************************************/

/********************** MÉTODOS DE LA MÁQUINA DE ESTADOS ************************/

void SpecificWorker::controller()
{
    try{
      NavState estadoController = controller_proxy->getState();
      if(estadoController.state=="IDLE"){
	    qDebug("Enviando marcas al controlador");
	    ListaMarca::Marca marcaMundo = marcas.get(currentMark);
	    inner->transform("world","base").print("base en el mundo");
	    QVec m = QVec::vec3(marcaMundo.tx,marcaMundo.ty,marcaMundo.tz);
	    m.print("marca");
	    QVec vec=inner->transform("world", m, "rgbd");
	    vec.print("marca en mundo");
	    
	    TargetPose tp = {vec.x(), vec.y(), vec.z()};
 	    controller_proxy->go(tp);
      }
      else if(estadoController.state=="FINISH"){
	state = State::STOP;
      }
    }catch(const Ice::Exception &ex){
        std::cout << ex << std::endl;
    }
}

/*Gira hasta encontrar la marca*/
void SpecificWorker::search()
{
  static bool primeraVuelta = true;
  
  if( marcas.exists(currentMark)) {
     differentialrobot_proxy->setSpeedBase(0, 0);	// Parar el robot
     state = State::CONTROLLER;	
     qDebug() <<"Cambiado a estado: Controller";
     primeraVuelta = true;
     return;
  }
  
  if(primeraVuelta){
    differentialrobot_proxy->setSpeedBase(0, 0.5);	// Girar mientras no encuentre la marca
    primeraVuelta = false;
  }
  
}

/*El robot llega a la marca y se para*/
void SpecificWorker::parar()
{
  differentialrobot_proxy->setSpeedBase(0, 0);	//Para el robot
  currentMark = (currentMark+1)%4;		//Actualiza la marca que está buscando
  state = State::INIT;				//Vuelve al primer estado
}


/*bucle principal*/
void SpecificWorker::compute()
{
  
  TBaseState tbase;
  
  try
  {  
    differentialrobot_proxy->getBaseState(tbase);
    inner->updateTransformValuesS("base",tbase.x,0,tbase.z,0,tbase.alpha,0);
  }
  catch(const Ice::Exception e)
  {
      std::cout << e << std::endl;
  }
    
  switch(state)
  {
    case State::INIT:
      state = State::SEARCH;
      break;
    
    case State::SEARCH:
      search();
      break;
    
    case State::CONTROLLER:
      controller();
      break;

    case State::STOP:
      parar();
      break;
  }
  
}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{
	
}


