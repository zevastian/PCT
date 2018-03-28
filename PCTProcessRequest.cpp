#include<cstring>
#include"PCTProcessRequest.h"
#include"PCTSingletonThreadWorker.h"
#include"PCTSingletonNetworkWorker.h"
#include"PCTSingletonApi.h"
#include"PCTLoadCover.h"
#include"PCTLoadInformation.h"

PCTProcessRequest::PCTProcessRequest(std::vector<char> requestData, std::shared_ptr<PCTCancellationToken> token, PCTCommon common, int priority)
    :PCTWorkItem(priority), mRequestData(requestData), mToken(token), mCommon(common)
{
    //NADA
}

void PCTProcessRequest::run()
{
    if (!mToken->isCancelled()) {
        std::vector<PCTMovie> movies;
        //ACA CREO QUE DEBERIA DISTINGUIR AQUELLOS ERRORES
        //DE RED DE AQUELLOS DE LA API (EJ: ESTA MAL PARSEADO,
        //LA API RETORNA CODIGO DE ERROR, ETC)
        if (PCTSingletonApi::getInstance().process(mRequestData, movies) == PCT_APICODE::PCT_API_SUCCESS) {
            for (auto it = movies.begin(); it != movies.end(); it++) {
                //EVITAR QUE HAYA DOSPCTProcessRequest
                //CUANDO INSERTO UN ITEM LO HAGO BAJO UN MUTEX
                //LO MISMO OCURRE CUANDO LE AGREGO EL POSTER
                //PARA QUE CUANDO YO LO CANCELE SE QUE LUEGO
                //NO SE VA A INSERTAR NINGUN ITEM MAS O AGREGAR UNA TEXTURA
                //A UN ITEM EXISTENTE
                mCommon.mutexTextures->lock();
                OGLTexture text;
                text.available = false;
                int indx = mCommon.textures->size();
                mCommon.textures->push_back(text);
                mCommon.mutexTextures->unlock();

                if (it->small_cover_image_url.size() && it->medium_cover_image_url.size() && it->large_cover_image_url.size()) {
                    //mToken->getMutex()->lock();
                    if (!mToken->isCancelled()) {
                        std::shared_ptr<PCTLoadCover> task = std::make_shared<PCTLoadCover>(*it, mToken, indx, mCommon, 0);
                        PCTSingletonThreadWorker::getInstance().addWorkItem(std::move(task));
                    }
                    //mToken->getMutex()->unlock();
                } else {
                    //mToken->getMutex()->lock();
                    if (!mToken->isCancelled()) {
                        std::shared_ptr<PCTLoadInformation> task = std::make_shared<PCTLoadInformation>(*it, mToken, indx, mCommon, 1);
                        PCTSingletonThreadWorker::getInstance().addWorkItem(std::move(task));
                    }
                    //mToken->getMutex()->unlock();
                }
            }
            XClientMessageEvent event;
            std::memset(&event, 0, sizeof(XClientMessageEvent));
            event.type = ClientMessage;
            event.format = 32;
            event.data.s[0] = 0;
            XSendEvent(mCommon.display, mCommon.window, 0, 0, (XEvent*)&event);
            XFlush(mCommon.display);
        }
    }
}
