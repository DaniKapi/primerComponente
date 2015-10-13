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

/**
       \brief
       @author authorname
*/







#ifndef SPECIFICWORKER_H
#define SPECIFICWORKER_H

#include <genericworker.h>
#include <innermodel/innermodel.h>


struct ListaMarcas
  {
    typedef struct 
    {
      int id;
      float tx;
      float ty;
      float tz;
      float rx;
      float ry;
      float rz;
    } Marca;
    
    QMap<int,Marca> lista;
    QMutex mutex;
    void add(Marca mar)
    {
      QMutexLocker ml(&mutex);
      lista.insert(mar.id, mar);
	
    };
    bool exists(int id)
    {
      return lista.contains(id);
    }
    Marca get(int id){
      return lista.value(id);
    };
  };

class SpecificWorker : public GenericWorker
{
Q_OBJECT
public:
	SpecificWorker(MapPrx& mprx);	
	~SpecificWorker();
	bool setParams(RoboCompCommonBehavior::ParameterList params);
	void newAprilTag(const tagsList &tags);
	

public slots:
	void compute(); 	
	void movimiento();
	void moverAcero();
	void moverAuno();
	void moverAdos();
	void moverAtres();
	void copiar(tag t, ListaMarcas::Marca &y);

private:
  int estado;
  
  
  ListaMarcas marcas;
   enum class State {INIT, SEARCH, ADVANCE, STOP};
   State state = State::INIT;
    int currentMark;
   
   void search();
  
};

#endif

