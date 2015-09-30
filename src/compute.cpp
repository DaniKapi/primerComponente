void SpecificWorker::compute()
{

    const float limite = 400;
    float velocidadAvance = 5
    float velocidadRotacion = 0.5;
    
    try
    {
        RoboCompLaser::TLaserData ldata = laser_proxy->getLaserData();  //read laser data 
        std::sort( ldata.begin()+10, ldata.end()-10, [](RoboCompLaser::TData a, RoboCompLaser::TData b){ return     a.dist < b.dist; }) ;  //sort laser data from small to large distances using a lambda function.

        if( (ldata.data()+10)->dist < limite)
        {
            //Rota
            differentialrobot_proxy->setSpeedBase(getSpeedAdv((ldata.data()+10)->angle,ldata.data()+10)->dist);
            
        }
        else
        {
            //Avanza
            
            differentialrobot_proxy->setSpeedBase(250, 0);
        }
        
    }
    catch(const Ice::Exception &ex)
    {
        std::cout << ex << std::endl;
    }
    
}

float SpecificWorker::getSpeedRot(float angle){
    
}

float SpecificWorker::getSpeedAdv(float dist){
    return 0.5*dist;
}