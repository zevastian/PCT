#include"PCTSingletonApi.h"

PCTSingletonApi::PCTSingletonApi()
{
    //NADA
}

PCTSingletonApi::~PCTSingletonApi()
{
    //NADA
}

PCTApi& PCTSingletonApi::getInstance()
{
    static PCTApi instance;
    return instance;
}
