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
 */
#include "specificworker.h"

/**
* \brief Default constructor
*/
SpecificWorker::SpecificWorker(MapPrx& mprx) : GenericWorker(mprx)
{
  this->estado=0;
  this->currentMark = 0;
  this->inner= new InnerModel("salabeta/robocomp/files/innermodel/simpleworld.xml");
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


/********************************************************************************/

void SpecificWorker::copiar(tag t, ListaMarcas::Marca& y)
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
    qDebug() << t.id;
    markread=t.id;
    ListaMarcas::Marca x;
    this->copiar(t,x);
    marcas.add(x);
  }
  
}



/********************************************************************************/


/*Gira hasta encontrar la marca*/
void SpecificWorker::search()
{
  if( marcas.exists( currentMark ))
  {
    
    /*Parar robot*/
     differentialrobot_proxy->setSpeedBase(0, 0);
     
     
     differentialrobot_proxy->setSpeedBase(0, 0.5);
     
     
     state = State::ADVANCE;
    
    /*Return*/
    return;
    
  }
  
  /*Girar*/ 
   differentialrobot_proxy->setSpeedBase(0, 0.5);
  
}

/*Se mueve hacia la marca*/
void SpecificWorker::movimiento()
{
    static bool sentido=true;
    const float threshold = 400; //millimeters -- Limite 
    float der = 0.9;  //rads per second -- Gira a la derecha
    float izq = -0.9;  //rads per second -- Gira a la izquierda
    std::cout<<"Quiero ir a: "<<currentMark<<std::endl;
    try
    {
    if( marcas.exists(this->currentMark)){
      ListaMarcas::Marca mar=marcas.get(this->currentMark);
      float dist=calcularDist(mar.tx,mar.ty);
      cout<<"Distancia: "<<dist <<endl;
	if(dist<0.3){
	  state = State::STOP;
	  std::cout << "Cambiando a STOP" << std::endl;
	  return;
	}
      }

   
        RoboCompLaser::TLaserData ldata = laser_proxy->getLaserData();  //read laser data 
        std::sort( ldata.begin()+10, ldata.end()-10, [](RoboCompLaser::TData a, RoboCompLaser::TData b){ return     a.dist < b.dist; }) ;  //sort laser data from small to large distances using a lambda function.

    if( (ldata.data()+10)->dist < threshold)
    {   
        if((ldata.data()+10)->angle > 0){
            differentialrobot_proxy->setSpeedBase(5, izq);
        }else{ 
            differentialrobot_proxy->setSpeedBase(5, der);
            
        }
        usleep(rand()%(1500000-100000 + 1) + 100000);  //random wait between 1.5s and 0.1sec
	
    }
    else
    {
        sentido=!sentido;
	if(markread!=currentMark){
	  state = State::SEARCH;
	  cout<<"cambiando a SEACH"<<endl;
	  marcas.borrar(currentMark);
	}else{
	  differentialrobot_proxy->setSpeedBase(250, 0);
	}
    }
    
    }
    catch(const Ice::Exception &ex)
    {
        std::cout << ex << std::endl;
    }
}

/*El robot llega a la marca y se para*/
void SpecificWorker::parar()
{
  differentialrobot_proxy->setSpeedBase(0, 0);
  currentMark++;
  cout<<"cambiando a INIT"<<endl;
  state = State::INIT;
}

/*bucle principal*/
void SpecificWorker::compute()
{
  
  TBaseState tbase;
  differentialrobot_proxy->getBaseState(tbase);
  inner->updateTransformValuesS("goal",tbase.x,0,tbase.z,0,tbase.alpha,0);
  
  switch(state)
  {
    case State::INIT:
      state = State::SEARCH;
      break;
    
    case State::SEARCH:
      search();
      break;

    case State::ADVANCE:
      movimiento();
      break;
      
    case State::STOP:
      parar();
    break;
  }
  
}



/*
 * setSpeedBase
 * 
 * @param adv Advance speed in mm/seg
 * @param rot Turn speed in rads/seg
 * 
 */

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{
	
}


