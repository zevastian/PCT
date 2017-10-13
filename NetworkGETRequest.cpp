#include"NetworkGETRequest.h"

bool PCTNetworkGETRequest::onStartPrivate(CURL* handle)
{
    bool ret = false;
    if (onStart()) {
        ret = true;
        //TEST
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, onWritePrivate);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, this);
        curl_easy_setopt(handle, CURLOPT_ACCEPT_ENCODING, "");
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 1L);
        curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1L);
        curl_easy_setopt(handle, CURLOPT_URL, mURL.c_str());
        curl_easy_setopt(handle, CURLOPT_HTTPGET, 1L);
        //SETEAR OPCIONES
        //SI FALLA LLAMAR A
        //onFinish(PCT_NET_FAILURE);
    }
    return ret;
}

void PCTNetworkGETRequest::setURL(std::string url)
{
    //TEST
    mURL = url;
}

void PCTNetworkGETRequest::setUserAgent(std::string userAgent)
{

}

void PCTNetworkGETRequest::setAcceptEncoding(bool enable)
{

}

void PCTNetworkGETRequest::setFollowLocation(bool enable)
{

}

void PCTNetworkGETRequest::setNoSignal(bool enable)
{

}

void PCTNetworkGETRequest::setUseSSL(bool enable)
{

}

void PCTNetworkGETRequest::setVerifyHost(bool enable)
{

}

void PCTNetworkGETRequest::setVerifyPeer(bool enable)
{

}

void PCTNetworkGETRequest::setTimeout(int seconds)
{

}
