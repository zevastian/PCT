#include"NetworkItem.h"

size_t PCTNetworkItem::onWritePrivate(char* ptr, size_t size, size_t nmemb, PCTNetworkItem* self)
{
    //TODO
    //ESTA BIEN USADO STD::MOVE?
    std::vector<char> buffer(&ptr[0], &ptr[size*nmemb]);
    return self->onWrite(std::move(buffer)) ? size*nmemb : -1;
}
