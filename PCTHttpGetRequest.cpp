#include"PCTHttpGetRequest.h"

PCTHttpGetRequest::PCTHttpGetRequest(int priority)
    :PCTNetworkItem(priority)
{
    //
}

bool PCTHttpGetRequest::onStart(CURL* handle)
{
    bool ret = false;
    if (onStart()) {
        ret = true;
        //TEST
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, onWritePrivate);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, this);
        curl_easy_setopt(handle, CURLOPT_ACCEPT_ENCODING, "");
        curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0");
        curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, 15L);
        curl_easy_setopt(handle, CURLOPT_LOW_SPEED_TIME, 15L);
        curl_easy_setopt(handle, CURLOPT_LOW_SPEED_LIMIT, 128L);
#if LIBCURL_VERSION_MAJOR >= 7 && LIBCURL_VERSION_MINOR >= 43
        curl_easy_setopt(handle, CURLOPT_PIPEWAIT, 1L);
#endif
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 2L);
#if LIBCURL_VERSION_MAJOR >= 7 && LIBCURL_VERSION_MINOR >= 41
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYSTATUS, 1L);
#endif
        curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 3L);
        curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1L);
        curl_easy_setopt(handle, CURLOPT_URL, mURL.c_str());
        curl_easy_setopt(handle, CURLOPT_HTTPGET, 1L);
#ifdef __DEBUG__
        curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
#endif
        //SETEAR OPCIONES
        //SI FALLA LLAMAR A
        //onFinish(PCT_NET_FAILURE);
    }
    return ret;
}

void PCTHttpGetRequest::onFinish(CURL* handle, CURLcode code)
{
    //EN ESTE CASO FALLO LA CREACION DEL HANDLE
    //RAZON POR LA CUAL SE RECIBE NULO
    if (!handle) {
        onFinish(PCT_NETWORK_CODE::PCT_NET_FAILURE);
    }

    //FALLO LIBCURL O SE CANCELO
    if (code != CURLE_OK) {
        onFinish(PCT_NETWORK_CODE::PCT_NET_FAILURE);
    }

    //NO FALLO PERO VEMOS SI TUVO EXITO
    //EL REQUEST HTTP
    long httpCode = 0;
    code = curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &httpCode);
    if (code != CURLE_OK) {
        onFinish(PCT_NETWORK_CODE::PCT_NET_FAILURE);
    }

    if (httpCode == 200) {
        onFinish(PCT_NETWORK_CODE::PCT_NET_SUCCESS);
    } else {
        onFinish(PCT_NETWORK_CODE::PCT_NET_FAILURE);
    }
}

size_t PCTHttpGetRequest::onWritePrivate(char* ptr, size_t size, size_t nmemb, PCTHttpGetRequest* self)
{
    //TODO
    //ESTA BIEN USADO STD::MOVE?
    std::vector<char> buffer(&ptr[0], &ptr[size*nmemb]);
    return self->onWrite(std::move(buffer)) ? size*nmemb : -1;
}

void PCTHttpGetRequest::setURL(std::string url)
{
    //TEST
    mURL = url;
}
