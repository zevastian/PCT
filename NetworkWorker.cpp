#include<stdexcept>
#include<algorithm>
#include"NetworkWorker.h"

PCTNetworkWorker::PCTNetworkWorker()
    :mExit(false)
{
    CURLcode code;
    if ((code = curl_global_init(CURL_GLOBAL_DEFAULT)) != CURLE_OK) {
        throw std::runtime_error(curl_easy_strerror(code));
    }

    mMultiHandle = curl_multi_init();
    if (!mMultiHandle) {
        curl_global_cleanup();
        throw std::runtime_error("curl_multi_init failed");
    }

    mThread = std::thread([&] {
        CURLMcode code;
        while (true)
        {
            int running = 0;
            while ((code = curl_multi_perform(mMultiHandle, &running)) == CURLM_CALL_MULTI_PERFORM);
            if (code != CURLM_OK) {
                throw std::runtime_error(curl_multi_strerror(code));
            }

            int numfds = 0;
            code = curl_multi_wait(mMultiHandle, NULL, 0, 1000, &numfds);
            if (code != CURLM_OK) {
                throw std::runtime_error(curl_multi_strerror(code));
            }

            int pending = 0;
            CURLMsg* msg;
            while ((msg = curl_multi_info_read(mMultiHandle, &pending))) {

                if (msg->msg == CURLMSG_DONE) {
                    for (auto it = mItemsWorking.begin(); it != mItemsWorking.end(); it++) {
                        if (it->second == msg->easy_handle) {
                            if (curl_multi_remove_handle(mMultiHandle, it->second) != CURLM_OK) {
                                throw std::runtime_error(curl_multi_strerror(code));
                            }
                            if (msg->data.result != CURLE_OK) {
                                curl_easy_cleanup(msg->easy_handle);
                                it->first->onFinish(PCT_NET_FAILURE);
                            } else {
                                mCurlHandles.push(msg->easy_handle);
                                it->first->onFinish(PCT_NET_SUCCESS);
                            }
                            mItemsWorking.erase(it);
                            break;
                        }
                    }
                }
            }

            if (running < MAX_SIMULTANEOUS_HANDLES) {
                //ESTA BIEN USAR EL LOCK?
                std::unique_lock<std::mutex> lock(mMutexQueueItemsWaiting);
                if (!mItemsWaiting.size() && !running) {
                    if (mExit) {
                        return;
                    }

                    while(mItemsWaiting.empty() && !mExit) {
                        mCondVarItemsWaiting.wait(lock);
                    }
                }

                if (mItemsWaiting.size() > 0) {
                    size_t iter = std::min((size_t)(MAX_SIMULTANEOUS_HANDLES - running), mItemsWaiting.size());
                    for (size_t i = 0; i < iter; i++) {

                        std::shared_ptr<PCTNetworkItem> item = mItemsWaiting.top();
                        mItemsWaiting.pop();

                        CURL* handle = NULL;
                        if (mCurlHandles.size() < (size_t)(MAX_SIMULTANEOUS_HANDLES - running)) {
                            handle = curl_easy_init();
                            if (!handle) {
                                throw std::runtime_error("curl_easy_init failed");
                            }
                        } else {
                            handle = mCurlHandles.front();
                            mCurlHandles.pop();
                            curl_easy_reset(handle);
                        }

                        if (item->onStartPrivate(handle)) {
                            code = curl_multi_add_handle(mMultiHandle, handle);
                            if (code == CURLM_OK) {
                                mItemsWorking.push_back(std::make_pair(item, handle));
                                continue;
                            }
                            item->onFinish(PCT_NET_FAILURE);
                        }
                        curl_easy_cleanup(handle);
                    }
                    continue;
                }
            }

            //SOLO ESPERO CUANDO NO TENGO NINGUN
            //ITEM QUE AGREGAR
            if (!numfds) {
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }
        }
    });
}

PCTNetworkWorker::~PCTNetworkWorker()
{
    mMutexQueueItemsWaiting.lock();
    mExit = true;
    mMutexQueueItemsWaiting.unlock();
    mCondVarItemsWaiting.notify_one();

    if (mThread.joinable()) {
        mThread.join();
    }
}

bool PCTNetworkWorker::PCTItemComparator::operator()(std::shared_ptr<PCTNetworkItem>& item1, std::shared_ptr<PCTNetworkItem>& item2)
{
    return true;
}

void PCTNetworkWorker::addNetworkItem(std::shared_ptr<PCTNetworkItem> item)
{
    mMutexQueueItemsWaiting.lock();
    mItemsWaiting.push(item);
    mMutexQueueItemsWaiting.unlock();
    mCondVarItemsWaiting.notify_one();
}
