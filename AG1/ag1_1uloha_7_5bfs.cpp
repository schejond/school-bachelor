#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <deque>
#include <queue>
//---------------------------------------------------------------------------------------------------
//using namespace std;
//---------------------------------------------------------------------------------------------------
class City {
public:
    City()
    :   myGrocery(-1), minimalCntOfGroceries(1), sumOfDistances(0), enoughGroceries(false) {
    }

    ~City() = default;

    int const getMyGrocery() {
        return myGrocery;
    }
    //also add myGrocery to the groceriesSet
    void setMyGrocery(const int myGrocery) {
        this->myGrocery = myGrocery;
        groceriesSet.insert(myGrocery);
    }

    void setMinimalCntOfGroceries(const int minimalCntOfGroceries) {
        this->minimalCntOfGroceries = minimalCntOfGroceries;
    }

    bool const hasEnoughGroceries() {
        return enoughGroceries;
    }

    std::vector<City*> & getNeighbourCities() {
        return neighbourCitiesVector;
    }

    void addToSumOfDistances(const int sumToAdd) {
        sumOfDistances += sumToAdd;
    }

    void addNeighbour(City *neighbour) {
        if (std::find(neighbourCitiesVector.begin(), neighbourCitiesVector.end(), neighbour) == neighbourCitiesVector.end()) {
            //todo maybe remove the condition, because they are not sending wrong inputs
            this->neighbourCitiesVector.push_back(neighbour);
        }
    }

    void addItemToGroceriesSet(const int newGrocery) {
        groceriesSet.insert(newGrocery);
        if (groceriesSet.size() == minimalCntOfGroceries) {
            enoughGroceries = true;
        }
    }

    bool const groceriesSetContains(const int grocery) {
        return groceriesSet.find(grocery) != groceriesSet.end();
    }

    void const print() {
        std::cout << sumOfDistances << " ";
        std::set<int>::iterator iteratorToLastItem = groceriesSet.end();
        iteratorToLastItem--;
        for (std::set<int>::iterator it = groceriesSet.begin() ; it != iteratorToLastItem ; it++) {
            std::cout << *it << " ";
        }
        std::cout << *iteratorToLastItem << std::endl;
    }
private:
    int myGrocery;
    unsigned int minimalCntOfGroceries;
    int sumOfDistances;
    bool enoughGroceries;

    std::vector<City*> neighbourCitiesVector;
    std::set<int> groceriesSet;
};
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
class CountryMap {
public:
    CountryMap(const int cityCnt, const int routeCnt, const int groceriesTypeCnt, const int minimalCntOfGroceriesInCity)
            : cityCnt(cityCnt), routeCnt(routeCnt),
              minimalCntOfGroceriesInCity(minimalCntOfGroceriesInCity), finalRoutesCnt(0) {
    }

    ~CountryMap() = default;

    int const getCityCnt() {
        return cityCnt;
    }

    int const getRouteCnt() {
        return routeCnt;
    }
    //also sets the minimalCntOfGroceriesInCity for the city
    void addGroceryToCity(const int indexOfCity, const int indexOfGrocery) {
        City * targetCity = &cityIndexToCityMap[indexOfCity];
        targetCity->setMyGrocery(indexOfGrocery);
        targetCity->setMinimalCntOfGroceries(minimalCntOfGroceriesInCity);
    }

    void addRoute(const int cityIndex1, const int cityIndex2) {
        cityIndexToCityMap[cityIndex1].addNeighbour(&cityIndexToCityMap[cityIndex2]);
        cityIndexToCityMap[cityIndex2].addNeighbour(&cityIndexToCityMap[cityIndex1]);
    }

    void bfs(City * cityToFill, std::queue<std::pair<City*, int>> & queuePairCityPtrWithDistance, std::set<City*> & visitedCitiesSet) {
        if(queuePairCityPtrWithDistance.empty()) {
            return;
        }

        const int distance = queuePairCityPtrWithDistance.front().second + 1;
        City *cityForNeighboursDiscovery = queuePairCityPtrWithDistance.front().first;

        for (City * neighbourCity : cityForNeighboursDiscovery->getNeighbourCities()) {
            if (!(cityToFill->groceriesSetContains(neighbourCity->getMyGrocery()))) {
                cityToFill->addItemToGroceriesSet(neighbourCity->getMyGrocery());
                cityToFill->addToSumOfDistances(distance);
                finalRoutesCnt += distance;
                if (cityToFill->hasEnoughGroceries()) {
                    return;
                }
            }

            const int sizeOfVisitedCitiesSetBefore = visitedCitiesSet.size();//todo bude takhle nebo pomoci findu, to je asi fuk
            visitedCitiesSet.insert(neighbourCity);
            const int sizeOfVisitedCitiesSetAfter = visitedCitiesSet.size();
            if (sizeOfVisitedCitiesSetBefore == sizeOfVisitedCitiesSetAfter) {
                continue;
            }

            queuePairCityPtrWithDistance.push(std::make_pair(neighbourCity, distance));
        }

        queuePairCityPtrWithDistance.pop();

        return bfs(cityToFill, queuePairCityPtrWithDistance, visitedCitiesSet);
    }

    void collectAllData() {
        for (std::map<int, City>::iterator mapIterator = cityIndexToCityMap.begin() ; mapIterator != cityIndexToCityMap.end() ; mapIterator++) {
            City *ptr = &(mapIterator->second);
            
            std::queue<std::pair<City*, int>> queuePairCityPtrWithDistance;
            std::set<City*> visitedCitiesSet;

            queuePairCityPtrWithDistance.push(std::make_pair(ptr, 0));
            visitedCitiesSet.insert(ptr);
            
            bfs(ptr, queuePairCityPtrWithDistance, visitedCitiesSet);
        }
    }

    void const print() {
        std::cout << finalRoutesCnt << std::endl;
        for (unsigned int i = 0 ; i < cityIndexToCityMap.size() ; i++) {
            cityIndexToCityMap[i].print();
        }
    }
private:
    const int cityCnt;
    const int routeCnt;
    const int minimalCntOfGroceriesInCity;

    int finalRoutesCnt;

    std::map<int, City> cityIndexToCityMap;
};
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
int main() {
    int cityCnt = -1; //N
    int routesCnt = -1; //M
    int groceceriesTypeCnt = -1; //P
    int minimalVarietyOfGroceriesInOneCity = -1; //Q

    std::cin >> cityCnt >> routesCnt;
    std::cin >> groceceriesTypeCnt >> minimalVarietyOfGroceriesInOneCity;

    if (cityCnt <= 0 || routesCnt < 0 || groceceriesTypeCnt <  minimalVarietyOfGroceriesInOneCity || minimalVarietyOfGroceriesInOneCity < 1) {
        std::cout << "Chybny vstup!" << std::endl;
        return 1;
    }

    CountryMap cities(cityCnt, routesCnt, groceceriesTypeCnt, minimalVarietyOfGroceriesInOneCity);

    for (int cityIndex = 0 ; cityIndex < cities.getCityCnt() ; cityIndex++) {
        int groceryIndex = -1;
        std::cin >> groceryIndex;
        if (groceryIndex < 0 || groceryIndex > groceceriesTypeCnt-1) {
            std::cout << "Chybny vstup!" << std::endl;
            return 1;
        }
        cities.addGroceryToCity(cityIndex, groceryIndex);
    }

    for (int i = 0 ; i < cities.getRouteCnt() ; i++) {
        int cityIndex1 = -1;
        int cityIndex2 = -1;
        std::cin >> cityIndex1 >> cityIndex2;
        if (cityIndex1 < 0 || cityIndex1 > cities.getCityCnt()-1 || cityIndex2 < 0 || cityIndex2 > cities.getCityCnt()-1) {
            std::cout << "Chybny vstup!" << std::endl;
            return 1;
        }
        cities.addRoute(cityIndex1, cityIndex2);
    }
    if (minimalVarietyOfGroceriesInOneCity == 1) {
        cities.print();
        return 0;
    }

    cities.collectAllData();
    cities.print();

    return 0;
}