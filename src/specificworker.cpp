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

}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{
	
}

bool SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
	
	timer.start(Period);

	return true;
}

void SpecificWorker::compute()
{

    const float threshold = 400; //millimeters -- Limite 
    float der = 0.9;  //rads per second -- Gira a la derecha
    float izq = -0.9;  //rads per second -- Gira a la izquierda
    
    static bool sentido=true;
    
    try
    {
        RoboCompLaser::TLaserData ldata = laser_proxy->getLaserData();  //read laser data 
        std::sort( ldata.begin()+10, ldata.end()-10, [](RoboCompLaser::TData a, RoboCompLaser::TData b){ return     a.dist < b.dist; }) ;  //sort laser data from small to large distances using a lambda function.

    if( (ldata.data()+10)->dist < threshold)
    {
        std::cout << (ldata.data()+10)->dist << std::endl;
        
        if((ldata.data()+10)->angle > 0){
            differentialrobot_proxy->setSpeedBase(5, izq);
        }else{ 
            differentialrobot_proxy->setSpeedBase(5, der);
            
        }
        usleep(rand()%(1500000-100000 + 1) + 100000);  //random wait between 1.5s and 0.1sec
    }
    else
    {
        //float valor=(ldata.data()+10)->dist;  
        
        sentido=!sentido;
        differentialrobot_proxy->setSpeedBase(250, 0);
    }
    
    }
    catch(const Ice::Exception &ex)
    {
        std::cout << ex << std::endl;
    }
    
}



void SpecificWorker::newAprilTag(const tagsList& tags)
{
  for (auto t: tags)
    qDebug() << t.id;
  
}

/*
 * setSpeedBase
 * 
 * @param adv Advance speed in mm/seg
 * @param rot Turn speed in rads/seg
 * 
 */





