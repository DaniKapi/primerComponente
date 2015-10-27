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

#include "listamarca.h"

public ListaMarca::ListaMarca() {
  
}

void ListaMarca::setInner(InnerModel* inner_)
{
  inner = inner_;
  hayMemoria = false;
}

void ListaMarca::add(Marca mar)
{
  QMutexLocker ml(&mutex);
  lista.insert(mar.id, mar);
  memoria=inner->transform("world",QVec::vec3(mar.tx,0,mar.tz),"rgbd");
  hayMemoria = true;
}

bool ListaMarca::exists(int id)
{
  QMutexLocker ml(&mutex);
  return lista.contains(id);
}

ListaMarca::Marca ListaMarca::get(int id)
{
  QMutexLocker ml(&mutex);

  if(exists(id)){
    return lista.value(id);
  } else {
    Marca aux;
    inner->transform("rgbd", memoria, "world");
    aux.tx = memoria.x();
    aux.tz = memoria.z();	
    return aux;
  }
}

void ListaMarca::borrar(int id)
{
  QMutexLocker ml(&mutex);
  lista.remove(id);
}

ListaMarca::~ListaMarca()
{

}

