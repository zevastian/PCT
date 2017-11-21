#include<stdexcept>
#include"PCTNetworkWorker.h"

PCTNetworkWorker::PCTNetworkWorker()
    :mExit(false)
{
    CURLcode code = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (code != CURLE_OK) {
        throw std::runtime_error("curl_global_init: " + std::string(curl_easy_strerror(code)));
    }

    mMulti = curl_multi_init();
    if (!mMulti) {
        curl_global_cleanup();
        throw std::runtime_error("curl_multi_init: error");
    }

    CURLMcode mcode = curl_multi_setopt(mMulti, CURLMOPT_MAX_HOST_CONNECTIONS, 4L);
    if (mcode != CURLM_OK) {
        curl_multi_cleanup(mMulti);
        curl_global_cleanup();
        throw std::runtime_error("curl_multi_setopt: " + std::string(curl_multi_strerror(mcode)));
    }

#if LIBCURL_VERSION_MAJOR >= 7 && LIBCURL_VERSION_MINOR >= 43
    mcode = curl_multi_setopt(mMulti, CURLMOPT_PIPELINING, CURLPIPE_HTTP1 | CURLPIPE_MULTIPLEX);
#elif LIBCURL_VERSION_MAJOR >= 7 && LIBCURL_VERSION_MINOR >= 16
    mcode = curl_multi_setopt(mMulti, CURLMOPT_PIPELINING, 1L);
#endif
    if (mcode != CURLM_OK) {
        curl_multi_cleanup(mMulti);
        curl_global_cleanup();
        throw std::runtime_error("curl_multi_setopt: " + std::string(curl_multi_strerror(mcode)));
    }

    mThread = std::thread([&] {
        bool exit = false;
        do {
            {
                std::unique_lock<std::mutex> lock(mMutexQueueItemsWaiting);
                while(mItemsWaiting.empty() && !mExit) {
                    mCondVarItemsWaiting.wait(lock);
                }
            }

            for (;;) {
                CURLMcode mcode;
                while (mItemsWorking.size() < PCT_MAX_COUNT_ITEMS) {
                    std::shared_ptr<PCTNetworkItem> item;

                    mMutexQueueItemsWaiting.lock();
                    bool emptyQueueWaiting = mItemsWaiting.empty();
                    if (!emptyQueueWaiting) {
                        item = std::move(mItemsWaiting.top());
                        mItemsWaiting.pop();
                    } else if (mExit) {
                        exit = true;
                    }
                    mMutexQueueItemsWaiting.unlock();
                    if (emptyQueueWaiting) {
                        break;
                    }

                    CURL* handle = NULL;
                    if (mHandles.empty()) {
                        handle = curl_easy_init();
                    } else {
                        handle = mHandles.front();
                        mHandles.pop();
                        curl_easy_reset(handle);
                    }

                    bool failed = true;
                    if (handle) {
                        if (item->onStart(handle)) {
                            mcode = curl_multi_add_handle(mMulti, handle);
                            if (mcode == CURLM_OK) {
                                mItemsWorking.push_back(std::make_pair(std::move(item), handle));
                                failed = false;
                            }
                        }
                    }

                    if (failed) {
                        item->onFinish(NULL, CURLE_OK);
                        if (handle) {
                            curl_easy_cleanup(handle);
                        }
                    }
                }

                if (mItemsWorking.empty()) {
                    break;
                }

                int running = 0;
                mcode = curl_multi_perform(mMulti, &running);
                if (mcode != CURLM_OK) {
                    throw std::runtime_error("curl_multi_perform: " + std::string(curl_multi_strerror(mcode)));
                }

                int numfds = 0;
                mcode = curl_multi_wait(mMulti, NULL, 0, 100, &numfds);
                if (mcode != CURLM_OK) {
                    throw std::runtime_error("curl_multi_wait: " + std::string(curl_multi_strerror(mcode)));
                }

                int pending = 0;
                CURLMsg* msg;
                while ((msg = curl_multi_info_read(mMulti, &pending))) {
                    if (msg->msg == CURLMSG_DONE) {

                        for (auto it = mItemsWorking.begin(); it != mItemsWorking.end(); it++) {
                            if (it->second == msg->easy_handle) {
                                it->first->onFinish(msg->easy_handle, msg->data.result);
                                mItemsWorking.erase(it);
                                mcode = curl_multi_remove_handle(mMulti, msg->easy_handle);
                                if (msg->data.result != CURLE_OK || mcode != CURLM_OK) {
                                    curl_easy_cleanup(msg->easy_handle);
                                } else {
                                    mHandles.push(msg->easy_handle);
                                }
                                break;
                            }
                        }

                    }
                }

                if (!numfds) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
        } while (!exit);
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

    while (!mHandles.empty()) {
        curl_easy_cleanup(mHandles.front());
        mHandles.pop();
    }
    curl_multi_cleanup(mMulti);
    curl_global_cleanup();
}

void PCTNetworkWorker::addNetworkItem(std::shared_ptr<PCTNetworkItem> item)
{
    mMutexQueueItemsWaiting.lock();
    mItemsWaiting.push(std::move(item));
    mMutexQueueItemsWaiting.unlock();
    mCondVarItemsWaiting.notify_one();
}
