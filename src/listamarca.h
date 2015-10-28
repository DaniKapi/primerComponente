/*
 * Copyright 2015 <copyright holder> <email>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */

#ifndef LISTAMARCA_H
#define LISTAMARCA_H

#include <genericworker.h>
#include <innermodel/innermodel.h>

class ListaMarca
{

public:
  
  typedef struct 
  {
    int id;
    float distance;
    float tx;
    float ty;
    float tz;
    float rx;
    float ry;
    float rz;
  } Marca;
  
  ListaMarca(); 
  void add(Marca mar);
  Marca get(int id);
  bool exists(int id);
  void borrar(int id);
  void setInner(InnerModel *inner_);
  ~ListaMarca();
  
private:
  QMap<int,Marca> lista;
  QMutex mutex;
  QVec memoria;
  bool hayMemoria;
  InnerModel *inner;

};

#endif // LISTAMARCA_H
