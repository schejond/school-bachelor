#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <climits>
#include <cfloat>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <vector>
#include <set>
#include <list>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>
#include <deque>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "progtest_solver.h"
#include "sample_tester.h"
using namespace std;
#endif /* __PROGTEST__ */ 

class CWeldingCompany {
public:
    static void SeqSolve(APriceList priceList, COrder &order) {
      std::vector<COrder> cOrders;
      cOrders.push_back(order);
      ProgtestSolver(cOrders, priceList);
      order = cOrders.front();
    };
    
    //tohle je worker
    void AddProducer(AProducer prod) {
      this->producers.push_back(prod);
    };

    ///tohle je producer
    void AddCustomer(ACustomer cust) {
      this->customers.push_back(cust);
    };

    //bude probihat asynchronne i synchrone!
    void AddPriceList(AProducer prod, APriceList priceList)  {
      const unsigned materialId = priceList->m_MaterialID;
      unique_lock<mutex> mapLock(mtxMaps);
      materialIdToPriceListsUndone[materialId].push_back(priceList);
      if (materialIdToPriceListsUndone[materialId].size() == producers.size()) {
        map<unsigned, map<unsigned, CProd>> pricedProducts;

        for (vector<APriceList>::iterator priceListIterator = materialIdToPriceListsUndone[materialId].begin();
             priceListIterator != materialIdToPriceListsUndone[materialId].end();
             priceListIterator++) {
          for (vector<CProd>::iterator orderIt = (*priceListIterator)->m_List.begin();
               orderIt != (*priceListIterator)->m_List.end();
               orderIt++) {

            //get lower value as first key
            unsigned lowerValue;
            unsigned higherValue;
            if (orderIt->m_W > orderIt->m_H) {
              lowerValue = orderIt->m_H;
              higherValue = orderIt->m_W;
            } else {
              lowerValue = orderIt->m_W;
              higherValue = orderIt->m_H;
            }
            //neni tam
            if (pricedProducts.find(lowerValue) == pricedProducts.end()
                || (pricedProducts.find(lowerValue) != pricedProducts.end()
                   && pricedProducts[lowerValue].find(higherValue) == pricedProducts[lowerValue].end())
                ) {
              pricedProducts[lowerValue].insert(std::pair<unsigned, CProd>(higherValue, *orderIt));
              continue;
            }else if (pricedProducts[lowerValue].find(higherValue)->second.m_Cost > orderIt->m_Cost) {
              pricedProducts[lowerValue].find(higherValue)->second.m_Cost = orderIt->m_Cost;
              continue;
            }
          }
        }

        mapLock.unlock();
        // ted z nich vytvorim jeden priceList
        CPriceList bestPriceList(materialId);
        map<unsigned, map<unsigned, CProd>>::iterator mapIt = pricedProducts.begin();
        for ( ; mapIt != pricedProducts.end() ; mapIt++) {
          map<unsigned, CProd>::iterator map2It = mapIt->second.begin();
          for ( ; map2It != mapIt->second.end() ; map2It++) {
            bestPriceList.Add(map2It->second);
          }
        }

        APriceList aPrice = make_shared<CPriceList>(bestPriceList);
        mapLock.lock();
        materialIdToPriceListsDone[materialId] = aPrice;
        cv_WaitingForPriceList.notify_all();
        mapLock.unlock();//redundant
      }
    };

    void MakeDemands (ACustomer cust, unsigned tid) {
      while(true) {
        AOrderList orderList = cust->WaitForDemand();
        if (orderList == nullptr) {
          break;
        }
        unique_lock<mutex> ul (mtxQueue);
        cv_QueueFull.wait(ul, [this] () {return (customersDemands.size() < MAX_SIZE);});
        this->customersDemands.push(make_pair(cust, orderList));
        cv_QueueEmpty.notify_one();
      }
    }

    void FulfillOrders(unsigned tid) {
      //pokud nebude queue prazdna, nebo neni konec, tak vzdy popni front
      while (!customersDemands.empty() || !creationOfDemandsFinished) {
        unique_lock<mutex> ul (mtxQueue);
        //pokud se po uplynuti timeoutu nesplni podminka tak jed cyklus znova
        if (!cv_QueueEmpty.wait_for(ul, chrono::milliseconds(100),  [ this ] {return (!customersDemands.empty());})) {
          continue;
        }
        pair<ACustomer, AOrderList> firstElem = customersDemands.front();
        customersDemands.pop();
        cv_QueueFull.notify_one();// todo zamysli se nad poradim, jestli netreba prohodit s tim unlockem

        const unsigned materialIdOfFirstElem = firstElem.second->m_MaterialID;
        //pokud jeste neni hotovy tak spust for cyklus aby zhotovil a cekej
        unique_lock<mutex> mapLock(mtxMaps);//zamknu tady abych mohl ifem checknout mapu
        if (materialIdToPriceListsDone.find(materialIdOfFirstElem) == materialIdToPriceListsDone.end()) {
          mapLock.unlock();
          for (AProducer prod : producers) {
            prod->SendPriceList(materialIdOfFirstElem);
          }
          mapLock.lock();
          cv_WaitingForPriceList.wait(mapLock, [this, materialIdOfFirstElem] () {return materialIdToPriceListsDone.find(materialIdOfFirstElem) != materialIdToPriceListsDone.end();});
        }
        APriceList finalizedPriceList = materialIdToPriceListsDone[materialIdOfFirstElem];
        mapLock.unlock();//nechci mit mapu zamcenou behem plneni cen
        ul.unlock();//jiz nebudu pracovat s queue takze nepotrebuju drzet zamek
        ProgtestSolver(firstElem.second->m_List, finalizedPriceList);
        firstElem.first->Completed(firstElem.second);
      }
    }

    void Start(unsigned thrCount);

    void Stop(void);

    queue<pair<ACustomer, AOrderList>> customersDemands;

    map<unsigned, vector<APriceList>> materialIdToPriceListsUndone;
    map<unsigned, APriceList> materialIdToPriceListsDone;

    vector<thread> workersThreads;//pracovni vlakna
    vector<thread> consumersThreads;//pocet produceru
    vector<ACustomer> customers;
    vector<AProducer> producers;

    unsigned MAX_SIZE = 10;

    mutex mtxQueue;
    mutex mtxMaps;

    condition_variable cv_QueueEmpty;
    condition_variable cv_QueueFull;
    condition_variable cv_WaitingForPriceList;

    bool creationOfDemandsFinished = false;
  };

void CWeldingCompany::Start(unsigned thrCount) {//pocet pracovnich vlaken
    MAX_SIZE = 500 * (thrCount + customers.size());
    unsigned custId = 0;
    for(ACustomer cust : customers) {
      consumersThreads.push_back(thread(&CWeldingCompany::MakeDemands, this, cust, custId));
      custId++;
    }
    for(unsigned i = 0 ; i < thrCount ; i++) {
      workersThreads.push_back(thread(&CWeldingCompany::FulfillOrders, this, i));
    }
  }

void CWeldingCompany::Stop(void) {

  for (auto & t : consumersThreads) {
    t.join();
  }

  creationOfDemandsFinished = true;

  for (auto & t : workersThreads) {
    t.join();
  }
}
//-------------------------------------------------------------------------------------------------
#ifndef __PROGTEST__

int main(void) {
    using namespace std::placeholders;
    CWeldingCompany test;

    AProducer p1 = make_shared<CProducerSync>(bind(&CWeldingCompany::AddPriceList, &test, _1, _2));
    AProducerAsync p2 = make_shared<CProducerAsync>(bind(&CWeldingCompany::AddPriceList, &test, _1, _2));
    AProducerAsync p3 = make_shared<CProducerAsync>(bind(&CWeldingCompany::AddPriceList, &test, _1, _2));
    AProducerAsync p4 = make_shared<CProducerAsync>(bind(&CWeldingCompany::AddPriceList, &test, _1, _2));
    test.AddProducer(p1);
    test.AddProducer(p2);
    test.AddProducer(p3);
    test.AddProducer(p4);
    test.AddCustomer(make_shared<CCustomerTest>(2666));//kolik udela zakaznik requestu
    test.AddCustomer(make_shared<CCustomerTest>(2666));
    test.AddCustomer(make_shared<CCustomerTest>(2435));
    test.AddCustomer(make_shared<CCustomerTest>(2324));
    p2->Start();
    p3->Start();
    p4->Start();
    test.Start(2);
    test.Stop();
    p2->Stop();
    p3->Stop();
    p4->Stop();
    return 0;
}
#endif /* __PROGTEST__ */ 
